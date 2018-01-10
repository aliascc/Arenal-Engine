#include <assert.h>
#include <string.h>
#include "scriptwdictionary.h"
#include "../scriptarray/scriptarray.h"

BEGIN_AS_NAMESPACE

using namespace std;

//------------------------------------------------------------------------
// Object types are cached as user data to avoid costly runtime lookups

// We just define a number here that we assume nobody else is using for
// object type user data. The add-ons have reserved the numbers 1000
// through 1999 for this purpose, so we should be fine.
const asPWORD DICTIONARY_CACHE = 1003;

// This cache holds the object type of the wdictionary type and array type
// so it isn't necessary to look this up each time the wdictionary or array
// is created.
struct SDictionaryWCache
{
	asITypeInfo *dictType;
	asITypeInfo *arrayType;

	// This is called from RegisterScriptDictionaryW
	static void Setup(asIScriptEngine *engine)
	{
		SDictionaryWCache *cache = reinterpret_cast<SDictionaryWCache*>(engine->GetUserData(DICTIONARY_CACHE));
		if( cache == 0 )
		{
			cache = new SDictionaryWCache;
			engine->SetUserData(cache, DICTIONARY_CACHE);
			engine->SetEngineUserDataCleanupCallback(SDictionaryWCache::Cleanup, DICTIONARY_CACHE);

			cache->dictType = engine->GetTypeInfoByName("wdictionary");
			cache->arrayType = engine->GetTypeInfoByDecl("array<wstring>");
		}
	}

	// This is called from the engine when shutting down
	static void Cleanup(asIScriptEngine *engine)
	{
		SDictionaryWCache *cache = reinterpret_cast<SDictionaryWCache*>(engine->GetUserData(DICTIONARY_CACHE));
		if( cache )
			delete cache;
	}
};

//--------------------------------------------------------------------------
// CScriptDictionaryW implementation

CScriptDictionaryW *CScriptDictionaryW::Create(asIScriptEngine *engine)
{
	// Use the custom memory routine from AngelScript to allow application to better control how much memory is used
	CScriptDictionaryW *obj = (CScriptDictionaryW*)asAllocMem(sizeof(CScriptDictionaryW));
	new(obj) CScriptDictionaryW(engine);
	return obj;
}

CScriptDictionaryW *CScriptDictionaryW::Create(asBYTE *buffer)
{
	// Use the custom memory routine from AngelScript to allow application to better control how much memory is used
	CScriptDictionaryW *obj = (CScriptDictionaryW*)asAllocMem(sizeof(CScriptDictionaryW));
	new(obj) CScriptDictionaryW(buffer);
	return obj;
}

CScriptDictionaryW::CScriptDictionaryW(asIScriptEngine *engine)
{
	Init(engine);
}

void CScriptDictionaryW::Init(asIScriptEngine *e)
{
	// We start with one reference
	refCount = 1;
	gcFlag = false;

	// Keep a reference to the engine for as long as we live
	// We don't increment the reference counter, because the 
	// engine will hold a pointer to the object in the GC.
	engine = e;

	// The wdictionary object type is cached to avoid dynamically parsing it each time
	SDictionaryWCache *cache = reinterpret_cast<SDictionaryWCache*>(engine->GetUserData(DICTIONARY_CACHE));

	// Notify the garbage collector of this object
	engine->NotifyGarbageCollectorOfNewObject(this, cache->dictType);
}

CScriptDictionaryW::CScriptDictionaryW(asBYTE *buffer)
{
	// This constructor will always be called from a script
	// so we can get the engine from the active context
	asIScriptContext *ctx = asGetActiveContext();
	Init(ctx->GetEngine());

	// Initialize the wdictionary from the buffer
	asUINT length = *(asUINT*)buffer;
	buffer += 4;

	while( length-- )
	{
		// Align the buffer pointer on a 4 byte boundary in 
		// case previous value was smaller than 4 bytes
		if( asPWORD(buffer) & 0x3 )
			buffer += 4 - (asPWORD(buffer) & 0x3);

		// Get the name value pair from the buffer and insert it in the wdictionary
		dictKey_t name = *(dictKey_t*)buffer;
		buffer += sizeof(dictKey_t);

		// Get the type id of the value
		int typeId = *(int*)buffer;
		buffer += sizeof(int);

		// Depending on the type id, the value will inline in the buffer or a pointer
		void *ref = (void*)buffer;

		if( typeId >= asTYPEID_INT8 && typeId <= asTYPEID_DOUBLE )
		{
			// Convert primitive values to either int64 or double, so we can use the overloaded Set methods
			asINT64 i64;
			double d;
			switch( typeId )
			{
			case asTYPEID_INT8:   i64 = *(char*)          ref; break;
			case asTYPEID_INT16:  i64 = *(short*)         ref; break;
			case asTYPEID_INT32:  i64 = *(int*)           ref; break;
			case asTYPEID_INT64:  i64 = *(asINT64*)       ref; break;
			case asTYPEID_UINT8:  i64 = *(unsigned char*) ref; break;
			case asTYPEID_UINT16: i64 = *(unsigned short*)ref; break;
			case asTYPEID_UINT32: i64 = *(unsigned int*)  ref; break;
			case asTYPEID_UINT64: i64 = *(asINT64*)       ref; break;
			case asTYPEID_FLOAT:  d   = *(float*)         ref; break;
			case asTYPEID_DOUBLE: d   = *(double*)        ref; break;
			}
			
			if( typeId >= asTYPEID_FLOAT )
				Set(name, d);
			else
				Set(name, i64);
		}
		else
		{
			if( (typeId & asTYPEID_MASK_OBJECT) && 
				!(typeId & asTYPEID_OBJHANDLE) && 
				(engine->GetTypeInfoById(typeId)->GetFlags() & asOBJ_REF) )
			{
				// Dereference the pointer to get the reference to the actual object
				ref = *(void**)ref;
			}

			Set(name, ref, typeId);
		}

		// Advance the buffer pointer with the size of the value
		if( typeId & asTYPEID_MASK_OBJECT )
		{
			asITypeInfo *ti = engine->GetTypeInfoById(typeId);
			if( ti->GetFlags() & asOBJ_VALUE )
				buffer += ti->GetSize();
			else
				buffer += sizeof(void*);
		}
		else if( typeId == 0 )
		{
			// null pointer
			buffer += sizeof(void*);
		}
		else
		{
			buffer += engine->GetSizeOfPrimitiveType(typeId);
		}
	}
}

CScriptDictionaryW::~CScriptDictionaryW()
{
	// Delete all keys and values
	DeleteAll();
}

void CScriptDictionaryW::AddRef() const
{
	// We need to clear the GC flag
	gcFlag = false;
	asAtomicInc(refCount);
}

void CScriptDictionaryW::Release() const
{
	// We need to clear the GC flag
	gcFlag = false;
	if( asAtomicDec(refCount) == 0 )
	{
		this->~CScriptDictionaryW();
		asFreeMem(const_cast<CScriptDictionaryW*>(this));
	}
}

int CScriptDictionaryW::GetRefCount()
{
	return refCount;
}

void CScriptDictionaryW::SetGCFlag()
{
	gcFlag = true;
}

bool CScriptDictionaryW::GetGCFlag()
{
	return gcFlag;
}

void CScriptDictionaryW::EnumReferences(asIScriptEngine *inEngine)
{
	// TODO: If garbage collection can be done from a separate thread, then this method must be
	//       protected so that it doesn't get lost during the iteration if the wdictionary is modified

	// Call the gc enum callback for each of the objects
	dictMap_t::iterator it;
	for( it = dict.begin(); it != dict.end(); it++ )
	{
		if( it->second.m_typeId & asTYPEID_MASK_OBJECT )
			inEngine->GCEnumCallback(it->second.m_valueObj);
	}
}

void CScriptDictionaryW::ReleaseAllReferences(asIScriptEngine * /*engine*/)
{
	// We're being told to release all references in 
	// order to break circular references for dead objects
	DeleteAll();
}

CScriptDictionaryW &CScriptDictionaryW::operator =(const CScriptDictionaryW &other)
{
	// Clear everything we had before
	DeleteAll();

	// Do a shallow copy of the wdictionary
	dictMap_t::const_iterator it;
	for( it = other.dict.begin(); it != other.dict.end(); it++ )
	{
		if( it->second.m_typeId & asTYPEID_OBJHANDLE )
			Set(it->first, (void*)&it->second.m_valueObj, it->second.m_typeId);
		else if( it->second.m_typeId & asTYPEID_MASK_OBJECT )
			Set(it->first, (void*)it->second.m_valueObj, it->second.m_typeId);
		else
			Set(it->first, (void*)&it->second.m_valueInt, it->second.m_typeId);
	}

	return *this;
}

CScriptDictWValue *CScriptDictionaryW::operator[](const dictKey_t &key)
{
	// Return the existing value if it exists, else insert an empty value
	return &dict[key];
}

const CScriptDictWValue *CScriptDictionaryW::operator[](const dictKey_t &key) const
{
	// Return the existing value if it exists
	dictMap_t::const_iterator it;
	it = dict.find(key);
	if( it != dict.end() )
		return &it->second;

	// Else raise an exception
	asIScriptContext *ctx = asGetActiveContext();
	if( ctx )
		ctx->SetException("Invalid access to non-existing value");

	return 0;
}

void CScriptDictionaryW::Set(const dictKey_t &key, void *value, int typeId)
{
	dictMap_t::iterator it;
	it = dict.find(key);
	if( it == dict.end() )
		it = dict.insert(dictMap_t::value_type(key, CScriptDictWValue())).first;

	it->second.Set(engine, value, typeId);
}

// This overloaded method is implemented so that all integer and
// unsigned integers types will be stored in the wdictionary as int64
// through implicit conversions. This simplifies the management of the
// numeric types when the script retrieves the stored value using a 
// different type.
void CScriptDictionaryW::Set(const dictKey_t &key, const asINT64 &value)
{
	Set(key, const_cast<asINT64*>(&value), asTYPEID_INT64);
}

// This overloaded method is implemented so that all floating point types 
// will be stored in the wdictionary as double through implicit conversions. 
// This simplifies the management of the numeric types when the script 
// retrieves the stored value using a different type.
void CScriptDictionaryW::Set(const dictKey_t &key, const double &value)
{
	Set(key, const_cast<double*>(&value), asTYPEID_DOUBLE);
}

// Returns true if the value was successfully retrieved
bool CScriptDictionaryW::Get(const dictKey_t &key, void *value, int typeId) const
{
	dictMap_t::const_iterator it;
	it = dict.find(key);
	if( it != dict.end() )
		return it->second.Get(engine, value, typeId);

	// AngelScript has already initialized the value with a default value,
	// so we don't have to do anything if we don't find the element, or if 
	// the element is incompatible with the requested type.

	return false;
}

// Returns the type id of the stored value
int CScriptDictionaryW::GetTypeId(const dictKey_t &key) const
{
	dictMap_t::const_iterator it;
	it = dict.find(key);
	if( it != dict.end() )
		return it->second.m_typeId;

	return -1;
}

bool CScriptDictionaryW::Get(const dictKey_t &key, asINT64 &value) const
{
	return Get(key, &value, asTYPEID_INT64);
}

bool CScriptDictionaryW::Get(const dictKey_t &key, double &value) const
{
	return Get(key, &value, asTYPEID_DOUBLE);
}

bool CScriptDictionaryW::Exists(const dictKey_t &key) const
{
	dictMap_t::const_iterator it;
	it = dict.find(key);
	if( it != dict.end() )
		return true;

	return false;
}

bool CScriptDictionaryW::IsEmpty() const
{
	if( dict.size() == 0 )
		return true;

	return false;
}

asUINT CScriptDictionaryW::GetSize() const
{
	return asUINT(dict.size());
}

bool CScriptDictionaryW::Delete(const dictKey_t &key)
{
	dictMap_t::iterator it;
	it = dict.find(key);
	if( it != dict.end() )
	{
		it->second.FreeValue(engine);
		dict.erase(it);
		return true;
	}

	return false;
}

void CScriptDictionaryW::DeleteAll()
{
	dictMap_t::iterator it;
	for( it = dict.begin(); it != dict.end(); it++ )
		it->second.FreeValue(engine);

	dict.clear();
}

CScriptArray* CScriptDictionaryW::GetKeys() const
{
	// Retrieve the object type for the array<wstring> from the cache
	SDictionaryWCache *cache = reinterpret_cast<SDictionaryWCache*>(engine->GetUserData(DICTIONARY_CACHE));
	asITypeInfo *ti = cache->arrayType;

	// Create the array object
	CScriptArray *array = CScriptArray::Create(ti, asUINT(dict.size()));
	long current = -1;
	dictMap_t::const_iterator it;
	for( it = dict.begin(); it != dict.end(); it++ )
	{
		current++;
		*(dictKey_t*)array->At(current) = it->first;
	}

	return array;
}

//--------------------------------------------------------------------------
// Generic wrappers

void ScriptDictionaryWFactory_Generic(asIScriptGeneric *gen)
{
	*(CScriptDictionaryW**)gen->GetAddressOfReturnLocation() = CScriptDictionaryW::Create(gen->GetEngine());
}

void ScriptDictionaryWListFactory_Generic(asIScriptGeneric *gen)
{
	asBYTE *buffer = (asBYTE*)gen->GetArgAddress(0);
	*(CScriptDictionaryW**)gen->GetAddressOfReturnLocation() = CScriptDictionaryW::Create(buffer);
}

void ScriptDictionaryWAddRef_Generic(asIScriptGeneric *gen)
{
	CScriptDictionaryW *dict = (CScriptDictionaryW*)gen->GetObject();
	dict->AddRef();
}

void ScriptDictionaryWRelease_Generic(asIScriptGeneric *gen)
{
	CScriptDictionaryW *dict = (CScriptDictionaryW*)gen->GetObject();
	dict->Release();
}

void ScriptDictionaryWAssign_Generic(asIScriptGeneric *gen)
{
	CScriptDictionaryW *dict = (CScriptDictionaryW*)gen->GetObject();
	CScriptDictionaryW *other = *(CScriptDictionaryW**)gen->GetAddressOfArg(0);
	*dict = *other;
	*(CScriptDictionaryW**)gen->GetAddressOfReturnLocation() = dict;
}

void ScriptDictionaryWSet_Generic(asIScriptGeneric *gen)
{
	CScriptDictionaryW *dict = (CScriptDictionaryW*)gen->GetObject();
	dictKey_t *key = *(dictKey_t**)gen->GetAddressOfArg(0);
	void *ref = *(void**)gen->GetAddressOfArg(1);
	int typeId = gen->GetArgTypeId(1);
	dict->Set(*key, ref, typeId);
}

void ScriptDictionaryWSetInt_Generic(asIScriptGeneric *gen)
{
	CScriptDictionaryW *dict = (CScriptDictionaryW*)gen->GetObject();
	dictKey_t *key = *(dictKey_t**)gen->GetAddressOfArg(0);
	void *ref = *(void**)gen->GetAddressOfArg(1);
	dict->Set(*key, *(asINT64*)ref);
}

void ScriptDictionaryWSetFlt_Generic(asIScriptGeneric *gen)
{
	CScriptDictionaryW *dict = (CScriptDictionaryW*)gen->GetObject();
	dictKey_t *key = *(dictKey_t**)gen->GetAddressOfArg(0);
	void *ref = *(void**)gen->GetAddressOfArg(1);
	dict->Set(*key, *(double*)ref);
}

void ScriptDictionaryWGet_Generic(asIScriptGeneric *gen)
{
	CScriptDictionaryW *dict = (CScriptDictionaryW*)gen->GetObject();
	dictKey_t *key = *(dictKey_t**)gen->GetAddressOfArg(0);
	void *ref = *(void**)gen->GetAddressOfArg(1);
	int typeId = gen->GetArgTypeId(1);
	*(bool*)gen->GetAddressOfReturnLocation() = dict->Get(*key, ref, typeId);
}

void ScriptDictionaryWGetInt_Generic(asIScriptGeneric *gen)
{
	CScriptDictionaryW *dict = (CScriptDictionaryW*)gen->GetObject();
	dictKey_t *key = *(dictKey_t**)gen->GetAddressOfArg(0);
	void *ref = *(void**)gen->GetAddressOfArg(1);
	*(bool*)gen->GetAddressOfReturnLocation() = dict->Get(*key, *(asINT64*)ref);
}

void ScriptDictionaryWGetFlt_Generic(asIScriptGeneric *gen)
{
	CScriptDictionaryW *dict = (CScriptDictionaryW*)gen->GetObject();
	dictKey_t *key = *(dictKey_t**)gen->GetAddressOfArg(0);
	void *ref = *(void**)gen->GetAddressOfArg(1);
	*(bool*)gen->GetAddressOfReturnLocation() = dict->Get(*key, *(double*)ref);
}

void ScriptDictionaryWExists_Generic(asIScriptGeneric *gen)
{
	CScriptDictionaryW *dict = (CScriptDictionaryW*)gen->GetObject();
	dictKey_t *key = *(dictKey_t**)gen->GetAddressOfArg(0);
	bool ret = dict->Exists(*key);
	*(bool*)gen->GetAddressOfReturnLocation() = ret;
}

void ScriptDictionaryWIsEmpty_Generic(asIScriptGeneric *gen)
{
	CScriptDictionaryW *dict = (CScriptDictionaryW*)gen->GetObject();
	bool ret = dict->IsEmpty();
	*(bool*)gen->GetAddressOfReturnLocation() = ret;
}

void ScriptDictionaryWGetSize_Generic(asIScriptGeneric *gen)
{
	CScriptDictionaryW *dict = (CScriptDictionaryW*)gen->GetObject();
	asUINT ret = dict->GetSize();
	*(asUINT*)gen->GetAddressOfReturnLocation() = ret;
}

void ScriptDictionaryWDelete_Generic(asIScriptGeneric *gen)
{
	CScriptDictionaryW *dict = (CScriptDictionaryW*)gen->GetObject();
	dictKey_t *key = *(dictKey_t**)gen->GetAddressOfArg(0);
	*(bool*)gen->GetAddressOfReturnLocation() = dict->Delete(*key);
}

void ScriptDictionaryWDeleteAll_Generic(asIScriptGeneric *gen)
{
	CScriptDictionaryW *dict = (CScriptDictionaryW*)gen->GetObject();
	dict->DeleteAll();
}

static void ScriptDictionaryWGetRefCount_Generic(asIScriptGeneric *gen)
{
	CScriptDictionaryW *self = (CScriptDictionaryW*)gen->GetObject();
	*(int*)gen->GetAddressOfReturnLocation() = self->GetRefCount();
}

static void ScriptDictionaryWSetGCFlag_Generic(asIScriptGeneric *gen)
{
	CScriptDictionaryW *self = (CScriptDictionaryW*)gen->GetObject();
	self->SetGCFlag();
}

static void ScriptDictionaryWGetGCFlag_Generic(asIScriptGeneric *gen)
{
	CScriptDictionaryW *self = (CScriptDictionaryW*)gen->GetObject();
	*(bool*)gen->GetAddressOfReturnLocation() = self->GetGCFlag();
}

static void ScriptDictionaryWEnumReferences_Generic(asIScriptGeneric *gen)
{
	CScriptDictionaryW *self = (CScriptDictionaryW*)gen->GetObject();
	asIScriptEngine *engine = *(asIScriptEngine**)gen->GetAddressOfArg(0);
	self->EnumReferences(engine);
}

static void ScriptDictionaryWReleaseAllReferences_Generic(asIScriptGeneric *gen)
{
	CScriptDictionaryW *self = (CScriptDictionaryW*)gen->GetObject();
	asIScriptEngine *engine = *(asIScriptEngine**)gen->GetAddressOfArg(0);
	self->ReleaseAllReferences(engine);
}

static void CScriptDictionaryWGetKeys_Generic(asIScriptGeneric *gen)
{
	CScriptDictionaryW *self = (CScriptDictionaryW*)gen->GetObject();
	*(CScriptArray**)gen->GetAddressOfReturnLocation() = self->GetKeys();
}

static void CScriptDictionaryW_opIndex_Generic(asIScriptGeneric *gen)
{
	CScriptDictionaryW *self = (CScriptDictionaryW*)gen->GetObject();
	dictKey_t *key = *(dictKey_t**)gen->GetAddressOfArg(0);
	*(CScriptDictWValue**)gen->GetAddressOfReturnLocation() = self->operator[](*key);
}

static void CScriptDictionaryW_opIndex_const_Generic(asIScriptGeneric *gen)
{
	const CScriptDictionaryW *self = (const CScriptDictionaryW*)gen->GetObject();
	dictKey_t *key = *(dictKey_t**)gen->GetAddressOfArg(0);
	*(const CScriptDictWValue**)gen->GetAddressOfReturnLocation() = self->operator[](*key);
}


//-------------------------------------------------------------------------
// CScriptDictWValue

CScriptDictWValue::CScriptDictWValue()
{
	m_valueObj = 0;
	m_typeId   = 0;
}

CScriptDictWValue::CScriptDictWValue(asIScriptEngine *engine, void *value, int typeId)
{
	m_valueObj = 0;
	m_typeId   = 0;
	Set(engine, value, typeId);
}

CScriptDictWValue::~CScriptDictWValue()
{
	// Must not hold an object when destroyed, as then the object will never be freed
	assert( (m_typeId & asTYPEID_MASK_OBJECT) == 0 );
}

void CScriptDictWValue::FreeValue(asIScriptEngine *engine)
{
	// If it is a handle or a ref counted object, call release
	if( m_typeId & asTYPEID_MASK_OBJECT )
	{
		// Let the engine release the object
		engine->ReleaseScriptObject(m_valueObj, engine->GetTypeInfoById(m_typeId));
		m_valueObj = 0;
		m_typeId = 0;
	}

	// For primitives, there's nothing to do
}

void CScriptDictWValue::Set(asIScriptEngine *engine, void *value, int typeId)
{
	FreeValue(engine);

	m_typeId = typeId;
	if( typeId & asTYPEID_OBJHANDLE )
	{
		// We're receiving a reference to the handle, so we need to dereference it
		m_valueObj = *(void**)value;
		engine->AddRefScriptObject(m_valueObj, engine->GetTypeInfoById(typeId));
	}
	else if( typeId & asTYPEID_MASK_OBJECT )
	{
		// Create a copy of the object
		m_valueObj = engine->CreateScriptObjectCopy(value, engine->GetTypeInfoById(typeId));
	}
	else
	{
		// Copy the primitive value
		// We receive a pointer to the value.
		int size = engine->GetSizeOfPrimitiveType(typeId);
		memcpy(&m_valueInt, value, size);
	}
}

void CScriptDictWValue::Set(asIScriptEngine *engine, CScriptDictWValue &value)
{
	if( value.m_typeId & asTYPEID_OBJHANDLE )
		Set(engine, (void*)&value.m_valueObj, value.m_typeId);
	else if( value.m_typeId & asTYPEID_MASK_OBJECT )
		Set(engine, (void*)value.m_valueObj, value.m_typeId);
	else
		Set(engine, (void*)&value.m_valueInt, value.m_typeId);
}

// This overloaded method is implemented so that all integer and
// unsigned integers types will be stored in the wdictionary as int64
// through implicit conversions. This simplifies the management of the
// numeric types when the script retrieves the stored value using a 
// different type.
void CScriptDictWValue::Set(asIScriptEngine *engine, const asINT64 &value)
{
	Set(engine, const_cast<asINT64*>(&value), asTYPEID_INT64);
}

// This overloaded method is implemented so that all floating point types 
// will be stored in the wdictionary as double through implicit conversions. 
// This simplifies the management of the numeric types when the script 
// retrieves the stored value using a different type.
void CScriptDictWValue::Set(asIScriptEngine *engine, const double &value)
{
	Set(engine, const_cast<double*>(&value), asTYPEID_DOUBLE);
}

bool CScriptDictWValue::Get(asIScriptEngine *engine, void *value, int typeId) const
{
	// Return the value
	if( typeId & asTYPEID_OBJHANDLE )
	{
		// A handle can be retrieved if the stored type is a handle of same or compatible type
		// or if the stored type is an object that implements the interface that the handle refer to.
		if( (m_typeId & asTYPEID_MASK_OBJECT) )
		{
			// Don't allow the get if the stored handle is to a const, but the desired handle is not
			if( (m_typeId & asTYPEID_HANDLETOCONST) && !(typeId & asTYPEID_HANDLETOCONST) )
				return false;

			// RefCastObject will increment the refcount if successful
			engine->RefCastObject(m_valueObj, engine->GetTypeInfoById(m_typeId), engine->GetTypeInfoById(typeId), reinterpret_cast<void**>(value));

			return true;
		}
	}
	else if( typeId & asTYPEID_MASK_OBJECT )
	{
		// Verify that the copy can be made
		bool isCompatible = false;

		// Allow a handle to be value assigned if the wanted type is not a handle
		if( (m_typeId & ~(asTYPEID_OBJHANDLE | asTYPEID_HANDLETOCONST) ) == typeId && m_valueObj != 0 )
			isCompatible = true;

		// Copy the object into the given reference
		if( isCompatible )
		{
			engine->AssignScriptObject(value, m_valueObj, engine->GetTypeInfoById(typeId));

			return true;
		}
	}
	else
	{
		if( m_typeId == typeId )
		{
			int size = engine->GetSizeOfPrimitiveType(typeId);
			memcpy(value, &m_valueInt, size);
			return true;
		}

		// We know all numbers are stored as either int64 or double, since we register overloaded functions for those
		// Only bool and enums needs to be treated separately
		if( typeId == asTYPEID_DOUBLE )
		{
			if( m_typeId == asTYPEID_INT64 )
				*(double*)value = double(m_valueInt);
			else if (m_typeId == asTYPEID_BOOL)
			{
				// Use memcpy instead of type cast to make sure the code is endianess agnostic
				char localValue;
				memcpy(&localValue, &m_valueInt, sizeof(char));
				*(double*)value = localValue ? 1.0 : 0.0;
			}
			else if (m_typeId > asTYPEID_DOUBLE && (m_typeId & asTYPEID_MASK_OBJECT) == 0)
			{
				// Use memcpy instead of type cast to make sure the code is endianess agnostic
				int localValue;
				memcpy(&localValue, &m_valueInt, sizeof(int));
				*(double*)value = double(localValue); // enums are 32bit
			}
			else
			{
				// The stored type is an object
				// TODO: Check if the object has a conversion operator to a primitive value
				*(double*)value = 0;
			}
			return true;
		}
		else if( typeId == asTYPEID_INT64 )
		{
			if( m_typeId == asTYPEID_DOUBLE )
				*(asINT64*)value = asINT64(m_valueFlt);
			else if (m_typeId == asTYPEID_BOOL)
			{
				// Use memcpy instead of type cast to make sure the code is endianess agnostic
				char localValue;
				memcpy(&localValue, &m_valueInt, sizeof(char));
				*(asINT64*)value = localValue ? 1 : 0;
			}
			else if (m_typeId > asTYPEID_DOUBLE && (m_typeId & asTYPEID_MASK_OBJECT) == 0)
			{
				// Use memcpy instead of type cast to make sure the code is endianess agnostic
				int localValue;
				memcpy(&localValue, &m_valueInt, sizeof(int));
				*(asINT64*)value = localValue; // enums are 32bit
			}
			else
			{
				// The stored type is an object
				// TODO: Check if the object has a conversion operator to a primitive value
				*(asINT64*)value = 0;
			}
			return true;
		}
		else if( typeId > asTYPEID_DOUBLE && (m_typeId & asTYPEID_MASK_OBJECT) == 0 )
		{
			// The desired type is an enum. These are always 32bit integers
			if( m_typeId == asTYPEID_DOUBLE )
				*(int*)value = int(m_valueFlt);
			else if( m_typeId == asTYPEID_INT64 )
				*(int*)value = int(m_valueInt);
			else if (m_typeId == asTYPEID_BOOL)
			{
				// Use memcpy instead of type cast to make sure the code is endianess agnostic
				char localValue;
				memcpy(&localValue, &m_valueInt, sizeof(char));
				*(int*)value = localValue ? 1 : 0;
			}
			else if (m_typeId > asTYPEID_DOUBLE && (m_typeId & asTYPEID_MASK_OBJECT) == 0)
			{
				// Use memcpy instead of type cast to make sure the code is endianess agnostic
				int localValue;
				memcpy(&localValue, &m_valueInt, sizeof(int));
				*(int*)value = localValue; // enums are 32bit
			}
			else
			{
				// The stored type is an object
				// TODO: Check if the object has a conversion operator to a primitive value
				*(int*)value = 0;
			}
		}
		else if( typeId == asTYPEID_BOOL )
		{
			if (m_typeId & asTYPEID_OBJHANDLE)
			{
				// TODO: Check if the object has a conversion operator to a primitive value
				*(bool*)value = m_valueObj ? true : false;
			}
			else if( m_typeId & asTYPEID_MASK_OBJECT )
			{
				// TODO: Check if the object has a conversion operator to a primitive value
				*(bool*)value = true;
			}
			else
			{
				// Compare only the bytes that were actually set
				asQWORD zero = 0;
				int size = engine->GetSizeOfPrimitiveType(m_typeId);
				*(bool*)value = memcmp(&m_valueInt, &zero, size) == 0 ? false : true;
			}
		}
	}

	// It was not possible to retrieve the value using the desired typeId
	return false;
}

const void * CScriptDictWValue::GetAddressOfValue() const
{
	if( (m_typeId & asTYPEID_MASK_OBJECT) && !(m_typeId & asTYPEID_OBJHANDLE) )
	{
		// Return the address to the object directly
		return m_valueObj;
	}
	
	// Return the address of the primitive or the pointer to the object
	return reinterpret_cast<const void*>(&m_valueObj);
}

bool CScriptDictWValue::Get(asIScriptEngine *engine, asINT64 &value) const
{
	return Get(engine, &value, asTYPEID_INT64);
}

bool CScriptDictWValue::Get(asIScriptEngine *engine, double &value) const
{
	return Get(engine, &value, asTYPEID_DOUBLE);
}

int CScriptDictWValue::GetTypeId() const
{
	return m_typeId;
}

static void CScriptDictWValue_Construct(void *mem)
{
	new(mem) CScriptDictWValue();
}

static void CScriptDictWValue_Destruct(CScriptDictWValue *obj)
{
	asIScriptContext *ctx = asGetActiveContext();
	if( ctx )
	{
		asIScriptEngine *engine = ctx->GetEngine();
		obj->FreeValue(engine);
	}
	obj->~CScriptDictWValue();
}

static CScriptDictWValue &CScriptDictWValue_opAssign(void *ref, int typeId, CScriptDictWValue *obj)
{
	asIScriptContext *ctx = asGetActiveContext();
	if( ctx )
	{
		asIScriptEngine *engine = ctx->GetEngine();
		obj->Set(engine, ref, typeId);
	}
	return *obj;
}

static CScriptDictWValue &CScriptDictWValue_opAssign(const CScriptDictWValue &other, CScriptDictWValue *obj)
{
	asIScriptContext *ctx = asGetActiveContext();
	if( ctx ) 
	{
		asIScriptEngine *engine = ctx->GetEngine();
		obj->Set(engine, const_cast<CScriptDictWValue&>(other));
	}

	return *obj;
}

static CScriptDictWValue &CScriptDictWValue_opAssign(double val, CScriptDictWValue *obj)
{
	return CScriptDictWValue_opAssign(&val, asTYPEID_DOUBLE, obj);
}

static CScriptDictWValue &CScriptDictWValue_opAssign(asINT64 val, CScriptDictWValue *obj)
{
	return CScriptDictWValue_opAssign(&val, asTYPEID_INT64, obj);
}

static void CScriptDictWValue_opCast(void *ref, int typeId, CScriptDictWValue *obj)
{
	asIScriptContext *ctx = asGetActiveContext();
	if( ctx )
	{
		asIScriptEngine *engine = ctx->GetEngine();
		obj->Get(engine, ref, typeId);
	}
}

static asINT64 CScriptDictWValue_opConvInt(CScriptDictWValue *obj)
{
	asINT64 value;
	CScriptDictWValue_opCast(&value, asTYPEID_INT64, obj);
	return value;
}

static double CScriptDictWValue_opConvDouble(CScriptDictWValue *obj)
{
	double value;
	CScriptDictWValue_opCast(&value, asTYPEID_DOUBLE, obj);
	return value;
}

//-------------------------------------------------------------------
// generic wrapper for CScriptDictWValue

static void CScriptDictWValue_opConvDouble_Generic(asIScriptGeneric *gen)
{
	CScriptDictWValue *self = (CScriptDictWValue*)gen->GetObject();
	double value;
	self->Get(gen->GetEngine(), value);
	*(double*)gen->GetAddressOfReturnLocation() = value;
}

static void CScriptDictWValue_opConvInt_Generic(asIScriptGeneric *gen)
{
	CScriptDictWValue *self = (CScriptDictWValue*)gen->GetObject();
	asINT64 value;
	self->Get(gen->GetEngine(), value);
	*(asINT64*)gen->GetAddressOfReturnLocation() = value;
}

static void CScriptDictWValue_opCast_Generic(asIScriptGeneric *gen)
{
	CScriptDictWValue *self = (CScriptDictWValue*)gen->GetObject();
	self->Get(gen->GetEngine(), gen->GetArgAddress(0), gen->GetArgTypeId(0));
}

static void CScriptDictWValue_opAssign_int64_Generic(asIScriptGeneric *gen)
{
	CScriptDictWValue *self = (CScriptDictWValue*)gen->GetObject();
	*(CScriptDictWValue**)gen->GetAddressOfReturnLocation() = &CScriptDictWValue_opAssign((asINT64)gen->GetArgQWord(0), self);
}

static void CScriptDictWValue_opAssign_double_Generic(asIScriptGeneric *gen)
{
	CScriptDictWValue *self = (CScriptDictWValue*)gen->GetObject();
	*(CScriptDictWValue**)gen->GetAddressOfReturnLocation() = &CScriptDictWValue_opAssign(gen->GetArgDouble(0), self);
}

static void CScriptDictWValue_opAssign_Generic(asIScriptGeneric *gen)
{
	CScriptDictWValue *self = (CScriptDictWValue*)gen->GetObject();
	*(CScriptDictWValue**)gen->GetAddressOfReturnLocation() = &CScriptDictWValue_opAssign(gen->GetArgAddress(0), gen->GetArgTypeId(0), self);
}

static void CScriptDictWValue_opCopyAssign_Generic(asIScriptGeneric *gen)
{
	CScriptDictWValue *self = (CScriptDictWValue*)gen->GetObject();
	*(CScriptDictWValue**)gen->GetAddressOfReturnLocation() = &CScriptDictWValue_opAssign(*reinterpret_cast<CScriptDictWValue*>(gen->GetArgAddress(0)), self);
}

static void CScriptDictWValue_Construct_Generic(asIScriptGeneric *gen)
{
	CScriptDictWValue *self = (CScriptDictWValue*)gen->GetObject();
	CScriptDictWValue_Construct(self);
}

static void CScriptDictWValue_Destruct_Generic(asIScriptGeneric *gen)
{
	CScriptDictWValue *self = (CScriptDictWValue*)gen->GetObject();
	CScriptDictWValue_Destruct(self);
}

//--------------------------------------------------------------------------
// Register the type

void RegisterScriptDictionaryW(asIScriptEngine *engine)
{
	if( strstr(asGetLibraryOptions(), "AS_MAX_PORTABILITY") )
		RegisterScriptDictionaryW_Generic(engine);
	else
		RegisterScriptDictionaryW_Native(engine);
}

void RegisterScriptDictionaryW_Native(asIScriptEngine *engine)
{
	int r;

	// The array<wstring> type must be available
	assert( engine->GetTypeInfoByDecl("array<wstring>") );

#if AS_CAN_USE_CPP11
	// With C++11 it is possible to use asGetTypeTraits to automatically determine the correct flags that represents the C++ class
	r = engine->RegisterObjectType("wdictionaryValue", sizeof(CScriptDictWValue), asOBJ_VALUE | asOBJ_ASHANDLE | asGetTypeTraits<CScriptDictWValue>()); assert( r >= 0 );
#else
	r = engine->RegisterObjectType("wdictionaryValue", sizeof(CScriptDictWValue), asOBJ_VALUE | asOBJ_ASHANDLE | asOBJ_APP_CLASS_CD); assert( r >= 0 );
#endif
	r = engine->RegisterObjectBehaviour("wdictionaryValue", asBEHAVE_CONSTRUCT, "void f()", asFUNCTION(CScriptDictWValue_Construct), asCALL_CDECL_OBJLAST); assert( r >= 0 );
	r = engine->RegisterObjectBehaviour("wdictionaryValue", asBEHAVE_DESTRUCT, "void f()", asFUNCTION(CScriptDictWValue_Destruct), asCALL_CDECL_OBJLAST); assert( r >= 0 );
	r = engine->RegisterObjectMethod("wdictionaryValue", "wdictionaryValue &opAssign(const wdictionaryValue &in)", asFUNCTIONPR(CScriptDictWValue_opAssign, (const CScriptDictWValue &, CScriptDictWValue *), CScriptDictWValue &), asCALL_CDECL_OBJLAST); assert( r >= 0 );
	r = engine->RegisterObjectMethod("wdictionaryValue", "wdictionaryValue &opHndlAssign(const ?&in)", asFUNCTIONPR(CScriptDictWValue_opAssign, (void *, int, CScriptDictWValue*), CScriptDictWValue &), asCALL_CDECL_OBJLAST); assert( r >= 0 );
	r = engine->RegisterObjectMethod("wdictionaryValue", "wdictionaryValue &opAssign(const ?&in)", asFUNCTIONPR(CScriptDictWValue_opAssign, (void *, int, CScriptDictWValue*), CScriptDictWValue &), asCALL_CDECL_OBJLAST); assert( r >= 0 );
	r = engine->RegisterObjectMethod("wdictionaryValue", "wdictionaryValue &opAssign(double)", asFUNCTIONPR(CScriptDictWValue_opAssign, (double, CScriptDictWValue*), CScriptDictWValue &), asCALL_CDECL_OBJLAST); assert( r >= 0 );
	r = engine->RegisterObjectMethod("wdictionaryValue", "wdictionaryValue &opAssign(int64)", asFUNCTIONPR(CScriptDictWValue_opAssign, (asINT64, CScriptDictWValue*), CScriptDictWValue &), asCALL_CDECL_OBJLAST); assert( r >= 0 );
	r = engine->RegisterObjectMethod("wdictionaryValue", "void opCast(?&out)", asFUNCTIONPR(CScriptDictWValue_opCast, (void *, int, CScriptDictWValue*), void), asCALL_CDECL_OBJLAST); assert( r >= 0 );
	r = engine->RegisterObjectMethod("wdictionaryValue", "void opConv(?&out)", asFUNCTIONPR(CScriptDictWValue_opCast, (void *, int, CScriptDictWValue*), void), asCALL_CDECL_OBJLAST); assert( r >= 0 );
	r = engine->RegisterObjectMethod("wdictionaryValue", "int64 opConv()", asFUNCTIONPR(CScriptDictWValue_opConvInt, (CScriptDictWValue*), asINT64), asCALL_CDECL_OBJLAST); assert( r >= 0 );
	r = engine->RegisterObjectMethod("wdictionaryValue", "double opConv()", asFUNCTIONPR(CScriptDictWValue_opConvDouble, (CScriptDictWValue*), double), asCALL_CDECL_OBJLAST); assert( r >= 0 );
	
	r = engine->RegisterObjectType("wdictionary", sizeof(CScriptDictionaryW), asOBJ_REF | asOBJ_GC); assert( r >= 0 );
	// Use the generic interface to construct the object since we need the engine pointer, we could also have retrieved the engine pointer from the active context
	r = engine->RegisterObjectBehaviour("wdictionary", asBEHAVE_FACTORY, "wdictionary@ f()", asFUNCTION(ScriptDictionaryWFactory_Generic), asCALL_GENERIC); assert( r >= 0 );
	r = engine->RegisterObjectBehaviour("wdictionary", asBEHAVE_LIST_FACTORY, "wdictionary @f(int &in) {repeat {wstring, ?}}", asFUNCTION(ScriptDictionaryWListFactory_Generic), asCALL_GENERIC); assert( r >= 0 );
	r = engine->RegisterObjectBehaviour("wdictionary", asBEHAVE_ADDREF, "void f()", asMETHOD(CScriptDictionaryW,AddRef), asCALL_THISCALL); assert( r >= 0 );
	r = engine->RegisterObjectBehaviour("wdictionary", asBEHAVE_RELEASE, "void f()", asMETHOD(CScriptDictionaryW,Release), asCALL_THISCALL); assert( r >= 0 );

	r = engine->RegisterObjectMethod("wdictionary", "wdictionary &opAssign(const wdictionary &in)", asMETHODPR(CScriptDictionaryW, operator=, (const CScriptDictionaryW &), CScriptDictionaryW&), asCALL_THISCALL); assert( r >= 0 );

	r = engine->RegisterObjectMethod("wdictionary", "void set(const wstring &in, const ?&in)", asMETHODPR(CScriptDictionaryW,Set,(const dictKey_t&,void*,int),void), asCALL_THISCALL); assert( r >= 0 );
	r = engine->RegisterObjectMethod("wdictionary", "bool get(const wstring &in, ?&out) const", asMETHODPR(CScriptDictionaryW,Get,(const dictKey_t&,void*,int) const,bool), asCALL_THISCALL); assert( r >= 0 );

	r = engine->RegisterObjectMethod("wdictionary", "void set(const wstring &in, const int64&in)", asMETHODPR(CScriptDictionaryW,Set,(const dictKey_t&,const asINT64&),void), asCALL_THISCALL); assert( r >= 0 );
	r = engine->RegisterObjectMethod("wdictionary", "bool get(const wstring &in, int64&out) const", asMETHODPR(CScriptDictionaryW,Get,(const dictKey_t&,asINT64&) const,bool), asCALL_THISCALL); assert( r >= 0 );

	r = engine->RegisterObjectMethod("wdictionary", "void set(const wstring &in, const double&in)", asMETHODPR(CScriptDictionaryW,Set,(const dictKey_t&,const double&),void), asCALL_THISCALL); assert( r >= 0 );
	r = engine->RegisterObjectMethod("wdictionary", "bool get(const wstring &in, double&out) const", asMETHODPR(CScriptDictionaryW,Get,(const dictKey_t&,double&) const,bool), asCALL_THISCALL); assert( r >= 0 );

	r = engine->RegisterObjectMethod("wdictionary", "bool exists(const wstring &in) const", asMETHOD(CScriptDictionaryW,Exists), asCALL_THISCALL); assert( r >= 0 );
	r = engine->RegisterObjectMethod("wdictionary", "bool isEmpty() const", asMETHOD(CScriptDictionaryW, IsEmpty), asCALL_THISCALL); assert( r >= 0 );
	r = engine->RegisterObjectMethod("wdictionary", "uint getSize() const", asMETHOD(CScriptDictionaryW, GetSize), asCALL_THISCALL); assert( r >= 0 );
	r = engine->RegisterObjectMethod("wdictionary", "bool delete(const wstring &in)", asMETHOD(CScriptDictionaryW,Delete), asCALL_THISCALL); assert( r >= 0 );
	r = engine->RegisterObjectMethod("wdictionary", "void deleteAll()", asMETHOD(CScriptDictionaryW,DeleteAll), asCALL_THISCALL); assert( r >= 0 );

	r = engine->RegisterObjectMethod("wdictionary", "array<wstring> @getKeys() const", asMETHOD(CScriptDictionaryW,GetKeys), asCALL_THISCALL); assert( r >= 0 );

	r = engine->RegisterObjectMethod("wdictionary", "wdictionaryValue &opIndex(const wstring &in)", asMETHODPR(CScriptDictionaryW, operator[], (const dictKey_t &), CScriptDictWValue*), asCALL_THISCALL); assert( r >= 0 );
	r = engine->RegisterObjectMethod("wdictionary", "const wdictionaryValue &opIndex(const wstring &in) const", asMETHODPR(CScriptDictionaryW, operator[], (const dictKey_t &) const, const CScriptDictWValue*), asCALL_THISCALL); assert( r >= 0 );

	// Register GC behaviours
	r = engine->RegisterObjectBehaviour("wdictionary", asBEHAVE_GETREFCOUNT, "int f()", asMETHOD(CScriptDictionaryW,GetRefCount), asCALL_THISCALL); assert( r >= 0 );
	r = engine->RegisterObjectBehaviour("wdictionary", asBEHAVE_SETGCFLAG, "void f()", asMETHOD(CScriptDictionaryW,SetGCFlag), asCALL_THISCALL); assert( r >= 0 );
	r = engine->RegisterObjectBehaviour("wdictionary", asBEHAVE_GETGCFLAG, "bool f()", asMETHOD(CScriptDictionaryW,GetGCFlag), asCALL_THISCALL); assert( r >= 0 );
	r = engine->RegisterObjectBehaviour("wdictionary", asBEHAVE_ENUMREFS, "void f(int&in)", asMETHOD(CScriptDictionaryW,EnumReferences), asCALL_THISCALL); assert( r >= 0 );
	r = engine->RegisterObjectBehaviour("wdictionary", asBEHAVE_RELEASEREFS, "void f(int&in)", asMETHOD(CScriptDictionaryW,ReleaseAllReferences), asCALL_THISCALL); assert( r >= 0 );

#if AS_USE_STLNAMES == 1
	// Same as isEmpty
	r = engine->RegisterObjectMethod("wdictionary", "bool empty() const", asMETHOD(CScriptDictionaryW, IsEmpty), asCALL_THISCALL); assert( r >= 0 );
	// Same as getSize
	r = engine->RegisterObjectMethod("wdictionary", "uint size() const", asMETHOD(CScriptDictionaryW, GetSize), asCALL_THISCALL); assert( r >= 0 );
	// Same as delete
	r = engine->RegisterObjectMethod("wdictionary", "void erase(const wstring &in)", asMETHOD(CScriptDictionaryW,Delete), asCALL_THISCALL); assert( r >= 0 );
	// Same as deleteAll
	r = engine->RegisterObjectMethod("wdictionary", "void clear()", asMETHOD(CScriptDictionaryW,DeleteAll), asCALL_THISCALL); assert( r >= 0 );
#endif

	// Cache some things the wdictionary will need at runtime
	SDictionaryWCache::Setup(engine);
}

void RegisterScriptDictionaryW_Generic(asIScriptEngine *engine)
{
	int r;

	// Register the cleanup callback for the object type cache
	engine->SetEngineUserDataCleanupCallback(SDictionaryWCache::Cleanup, DICTIONARY_CACHE);

#if AS_CAN_USE_CPP11
	// With C++11 it is possible to use asGetTypeTraits to automatically determine the correct flags that represents the C++ class
	r = engine->RegisterObjectType("wdictionaryValue", sizeof(CScriptDictWValue), asOBJ_VALUE | asOBJ_ASHANDLE | asGetTypeTraits<CScriptDictWValue>()); assert( r >= 0 );
#else
	r = engine->RegisterObjectType("wdictionaryValue", sizeof(CScriptDictWValue), asOBJ_VALUE | asOBJ_ASHANDLE | asOBJ_APP_CLASS_CD); assert( r >= 0 );
#endif
	r = engine->RegisterObjectBehaviour("wdictionaryValue", asBEHAVE_CONSTRUCT, "void f()", asFUNCTION(CScriptDictWValue_Construct_Generic), asCALL_GENERIC); assert( r >= 0 );
	r = engine->RegisterObjectBehaviour("wdictionaryValue", asBEHAVE_DESTRUCT, "void f()", asFUNCTION(CScriptDictWValue_Destruct_Generic), asCALL_GENERIC); assert( r >= 0 );
	r = engine->RegisterObjectMethod("wdictionaryValue", "wdictionaryValue &opAssign(const wdictionaryValue &in)", asFUNCTION(CScriptDictWValue_opCopyAssign_Generic), asCALL_GENERIC); assert( r >= 0 );
	r = engine->RegisterObjectMethod("wdictionaryValue", "wdictionaryValue &opHndlAssign(const ?&in)", asFUNCTION(CScriptDictWValue_opAssign_Generic), asCALL_GENERIC); assert( r >= 0 );
	r = engine->RegisterObjectMethod("wdictionaryValue", "wdictionaryValue &opAssign(const ?&in)", asFUNCTION(CScriptDictWValue_opAssign_Generic), asCALL_GENERIC); assert( r >= 0 );
	r = engine->RegisterObjectMethod("wdictionaryValue", "wdictionaryValue &opAssign(double)", asFUNCTION(CScriptDictWValue_opAssign_double_Generic), asCALL_GENERIC); assert( r >= 0 );
	r = engine->RegisterObjectMethod("wdictionaryValue", "wdictionaryValue &opAssign(int64)", asFUNCTION(CScriptDictWValue_opAssign_int64_Generic), asCALL_GENERIC); assert( r >= 0 );
	r = engine->RegisterObjectMethod("wdictionaryValue", "void opCast(?&out)", asFUNCTION(CScriptDictWValue_opCast_Generic), asCALL_GENERIC); assert( r >= 0 );
	r = engine->RegisterObjectMethod("wdictionaryValue", "void opConv(?&out)", asFUNCTION(CScriptDictWValue_opCast_Generic), asCALL_GENERIC); assert( r >= 0 );
	r = engine->RegisterObjectMethod("wdictionaryValue", "int64 opConv()", asFUNCTION(CScriptDictWValue_opConvInt_Generic), asCALL_GENERIC); assert( r >= 0 );
	r = engine->RegisterObjectMethod("wdictionaryValue", "double opConv()", asFUNCTION(CScriptDictWValue_opConvDouble_Generic), asCALL_GENERIC); assert( r >= 0 );

	r = engine->RegisterObjectType("wdictionary", sizeof(CScriptDictionaryW), asOBJ_REF | asOBJ_GC); assert( r >= 0 );
	r = engine->RegisterObjectBehaviour("wdictionary", asBEHAVE_FACTORY, "wdictionary@ f()", asFUNCTION(ScriptDictionaryWFactory_Generic), asCALL_GENERIC); assert( r>= 0 );
	r = engine->RegisterObjectBehaviour("wdictionary", asBEHAVE_LIST_FACTORY, "wdictionary @f(int &in) {repeat {wstring, ?}}", asFUNCTION(ScriptDictionaryWListFactory_Generic), asCALL_GENERIC); assert( r >= 0 );
	r = engine->RegisterObjectBehaviour("wdictionary", asBEHAVE_ADDREF, "void f()", asFUNCTION(ScriptDictionaryWAddRef_Generic), asCALL_GENERIC); assert( r >= 0 );
	r = engine->RegisterObjectBehaviour("wdictionary", asBEHAVE_RELEASE, "void f()", asFUNCTION(ScriptDictionaryWRelease_Generic), asCALL_GENERIC); assert( r >= 0 );

	r = engine->RegisterObjectMethod("wdictionary", "wdictionary &opAssign(const wdictionary &in)", asFUNCTION(ScriptDictionaryWAssign_Generic), asCALL_GENERIC); assert( r >= 0 );

	r = engine->RegisterObjectMethod("wdictionary", "void set(const wstring &in, const ?&in)", asFUNCTION(ScriptDictionaryWSet_Generic), asCALL_GENERIC); assert( r >= 0 );
	r = engine->RegisterObjectMethod("wdictionary", "bool get(const wstring &in, ?&out) const", asFUNCTION(ScriptDictionaryWGet_Generic), asCALL_GENERIC); assert( r >= 0 );

	r = engine->RegisterObjectMethod("wdictionary", "void set(const wstring &in, const int64&in)", asFUNCTION(ScriptDictionaryWSetInt_Generic), asCALL_GENERIC); assert( r >= 0 );
	r = engine->RegisterObjectMethod("wdictionary", "bool get(const wstring &in, int64&out) const", asFUNCTION(ScriptDictionaryWGetInt_Generic), asCALL_GENERIC); assert( r >= 0 );

	r = engine->RegisterObjectMethod("wdictionary", "void set(const wstring &in, const double&in)", asFUNCTION(ScriptDictionaryWSetFlt_Generic), asCALL_GENERIC); assert( r >= 0 );
	r = engine->RegisterObjectMethod("wdictionary", "bool get(const wstring &in, double&out) const", asFUNCTION(ScriptDictionaryWGetFlt_Generic), asCALL_GENERIC); assert( r >= 0 );

	r = engine->RegisterObjectMethod("wdictionary", "bool exists(const wstring &in) const", asFUNCTION(ScriptDictionaryWExists_Generic), asCALL_GENERIC); assert( r >= 0 );
	r = engine->RegisterObjectMethod("wdictionary", "bool isEmpty() const", asFUNCTION(ScriptDictionaryWIsEmpty_Generic), asCALL_GENERIC); assert( r >= 0 );
	r = engine->RegisterObjectMethod("wdictionary", "uint getSize() const", asFUNCTION(ScriptDictionaryWGetSize_Generic), asCALL_GENERIC); assert( r >= 0 );
	r = engine->RegisterObjectMethod("wdictionary", "bool delete(const wstring &in)", asFUNCTION(ScriptDictionaryWDelete_Generic), asCALL_GENERIC); assert( r >= 0 );
	r = engine->RegisterObjectMethod("wdictionary", "void deleteAll()", asFUNCTION(ScriptDictionaryWDeleteAll_Generic), asCALL_GENERIC); assert( r >= 0 );

	r = engine->RegisterObjectMethod("wdictionary", "array<wstring> @getKeys() const", asFUNCTION(CScriptDictionaryWGetKeys_Generic), asCALL_GENERIC); assert( r >= 0 );

	r = engine->RegisterObjectMethod("wdictionary", "wdictionaryValue &opIndex(const wstring &in)", asFUNCTION(CScriptDictionaryW_opIndex_Generic), asCALL_GENERIC); assert( r >= 0 );
	r = engine->RegisterObjectMethod("wdictionary", "const wdictionaryValue &opIndex(const wstring &in) const", asFUNCTION(CScriptDictionaryW_opIndex_const_Generic), asCALL_GENERIC); assert( r >= 0 );

	// Register GC behaviours
	r = engine->RegisterObjectBehaviour("wdictionary", asBEHAVE_GETREFCOUNT, "int f()", asFUNCTION(ScriptDictionaryWGetRefCount_Generic), asCALL_GENERIC); assert( r >= 0 );
	r = engine->RegisterObjectBehaviour("wdictionary", asBEHAVE_SETGCFLAG, "void f()", asFUNCTION(ScriptDictionaryWSetGCFlag_Generic), asCALL_GENERIC); assert( r >= 0 );
	r = engine->RegisterObjectBehaviour("wdictionary", asBEHAVE_GETGCFLAG, "bool f()", asFUNCTION(ScriptDictionaryWGetGCFlag_Generic), asCALL_GENERIC); assert( r >= 0 );
	r = engine->RegisterObjectBehaviour("wdictionary", asBEHAVE_ENUMREFS, "void f(int&in)", asFUNCTION(ScriptDictionaryWEnumReferences_Generic), asCALL_GENERIC); assert( r >= 0 );
	r = engine->RegisterObjectBehaviour("wdictionary", asBEHAVE_RELEASEREFS, "void f(int&in)", asFUNCTION(ScriptDictionaryWReleaseAllReferences_Generic), asCALL_GENERIC); assert( r >= 0 );

	// Cache some things the wdictionary will need at runtime
	SDictionaryWCache::Setup(engine);
}

//------------------------------------------------------------------
// Iterator implementation

CScriptDictionaryW::CIterator CScriptDictionaryW::begin() const
{
	return CIterator(*this, dict.begin());
}

CScriptDictionaryW::CIterator CScriptDictionaryW::end() const
{
	return CIterator(*this, dict.end());
}

CScriptDictionaryW::CIterator CScriptDictionaryW::find(const dictKey_t &key) const
{
	return CIterator(*this, dict.find(key));
}

CScriptDictionaryW::CIterator::CIterator(
		const CScriptDictionaryW &dict,
		dictMap_t::const_iterator it)
	: m_it(it), m_dict(dict)
{}

void CScriptDictionaryW::CIterator::operator++() 
{ 
	++m_it; 
}

void CScriptDictionaryW::CIterator::operator++(int) 
{ 
	++m_it;

	// Normally the post increment would return a copy of the object with the original state,
	// but it is rarely used so we skip this extra copy to avoid unnecessary overhead
}

CScriptDictionaryW::CIterator &CScriptDictionaryW::CIterator::operator*()
{
	return *this;
}

bool CScriptDictionaryW::CIterator::operator==(const CIterator &other) const 
{ 
	return m_it == other.m_it;
}

bool CScriptDictionaryW::CIterator::operator!=(const CIterator &other) const 
{ 
	return m_it != other.m_it; 
}

const dictKey_t &CScriptDictionaryW::CIterator::GetKey() const 
{ 
	return m_it->first; 
}

int CScriptDictionaryW::CIterator::GetTypeId() const
{ 
	return m_it->second.m_typeId; 
}

bool CScriptDictionaryW::CIterator::GetValue(asINT64 &value) const
{ 
	return m_it->second.Get(m_dict.engine, &value, asTYPEID_INT64); 
}

bool CScriptDictionaryW::CIterator::GetValue(double &value) const
{ 
	return m_it->second.Get(m_dict.engine, &value, asTYPEID_DOUBLE); 
}

bool CScriptDictionaryW::CIterator::GetValue(void *value, int typeId) const
{ 
	return m_it->second.Get(m_dict.engine, value, typeId); 
}

const void *CScriptDictionaryW::CIterator::GetAddressOfValue() const
{
	return m_it->second.GetAddressOfValue();
}

END_AS_NAMESPACE


