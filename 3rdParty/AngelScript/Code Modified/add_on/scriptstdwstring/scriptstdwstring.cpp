#include "scriptstdwstring.h"
#include <assert.h> // assert()
#include <sstream>  // std::wstringstream
#include <string.h> // strstr()
#include <stdio.h>	// sprintf()
#include <stdlib.h> // strtod()
#include <locale.h> // setlocale()
#include <map>      // std::map

using namespace std;

BEGIN_AS_NAMESPACE

#if AS_USE_WSTRINGPOOL == 1

// By keeping the literal strings in a pool the application
// performance is improved as there are less string copies created.

// The string pool will be kept as user data in the engine. We'll
// need a specific type to identify the string pool user data.
// We just define a number here that we assume nobody else is using for
// object type user data. The add-ons have reserved the numbers 1000
// through 1999 for this purpose, so we should be fine.
const asPWORD WSTRING_POOL = 2000;

// This global static variable is placed here rather than locally within the
// WStringFactory, due to memory leak detectors that don't see the deallocation
// of global variables. By placing the variable globally it will be initialized
// before the memory leak detector starts, thus it won't report the missing
// deallocation. An example of this the Marmalade leak detector initialized with
// IwGxInit() and finished with IwGxTerminate().
static const wstring emptyWString;

static const wstring &WStringFactory(asUINT length, const wchar_t *s)
{
	//Because UTF-16 is 2 bytes, length of the string is length/2
	length /= 2;
	// Each engine instance has its own wstring pool
	asIScriptContext *ctx = asGetActiveContext();
	if( ctx == 0 )
	{
		// The wstring factory can only be called from a script
		assert( ctx );
		return emptyWString;
	}
	asIScriptEngine *engine = ctx->GetEngine();

	// TODO: runtime optimize: Use unordered_map if C++11 is supported, i.e. MSVC10+, gcc 4.?+
	map<const wchar_t *, wstring> *pool = reinterpret_cast< map<const wchar_t *, wstring>* >(engine->GetUserData(WSTRING_POOL));

	if( !pool )
	{
		// The wstring pool hasn't been created yet, so we'll create it now
		asAcquireExclusiveLock();

		// Make sure the wstring pool wasn't created while we were waiting for the lock
		pool = reinterpret_cast< map<const wchar_t *, wstring>* >(engine->GetUserData(WSTRING_POOL));
		if( !pool )
		{
			#if defined(__S3E__)
			pool = new map<const wchar_t *, wstring>;
			#else
			pool = new (nothrow) map<const wchar_t *, wstring>;
			#endif
			if( pool == 0 )
			{
				ctx->SetException("Out of memory");
				asReleaseExclusiveLock();
				return emptyWString;
			}
			engine->SetUserData(pool, WSTRING_POOL);
		}

		asReleaseExclusiveLock();
	}

	// We can't let other threads modify the pool while we query it
	asAcquireSharedLock();

	// First check if a wstring object hasn't been created already
	map<const wchar_t *, wstring>::iterator it;
	it = pool->find(s);
	if( it != pool->end() )
	{
		asReleaseSharedLock();
		return it->second;
	}

	asReleaseSharedLock();

	// Acquire an exclusive lock so we can add the new wstring to the pool
	asAcquireExclusiveLock();

	// Make sure the wstring wasn't created while we were waiting for the exclusive lock
	it = pool->find(s);
	if( it == pool->end() )
	{
		// Create a new wstring object
		it = pool->insert(map<const wchar_t *, wstring>::value_type(s, wstring(s, length))).first;
	}

	asReleaseExclusiveLock();
	return it->second;
}

static void CleanupEngineWStringPool(asIScriptEngine *engine)
{
	map<const wchar_t *, wstring> *pool = reinterpret_cast< map<const wchar_t *, wstring>* >(engine->GetUserData(WSTRING_POOL));
	if( pool )
		delete pool;
}

#else
static wstring WStringFactory(asUINT length, const wchar_t *s)
{
	//Because UTF-16 is 2 bytes, length of the string is length/2
	return wstring(s, length/2);
}
#endif

static void ConstructWString(wstring *thisPointer)
{
	new(thisPointer) wstring();
}

static void CopyConstructWString(const wstring &other, wstring *thisPointer)
{
	new(thisPointer) wstring(other);
}

static void DestructWString(wstring *thisPointer)
{
	thisPointer->~wstring();
}

static wstring &AddAssignWStringToWString(const wstring &str, wstring &dest)
{
	// We don't register the method directly because some compilers
	// and standard libraries inline the definition, resulting in the
	// linker being unable to find the declaration.
	// Example: CLang/LLVM with XCode 4.3 on OSX 10.7
	dest += str;
	return dest;
}

// bool wstring::isEmpty()
// bool wstring::empty() // if AS_USE_STLNAMES == 1
static bool WStringIsEmpty(const wstring &str)
{
	// We don't register the method directly because some compilers
	// and standard libraries inline the definition, resulting in the
	// linker being unable to find the declaration
	// Example: CLang/LLVM with XCode 4.3 on OSX 10.7
	return str.empty();
}

static wstring &AssignUIntToWString(unsigned int i, wstring &dest)
{
	wostringstream stream;
	stream << i;
	dest = stream.str();
	return dest;
}

static wstring &AddAssignUIntToWString(unsigned int i, wstring &dest)
{
	wostringstream stream;
	stream << i;
	dest += stream.str();
	return dest;
}

static wstring AddWStringUInt(const wstring &str, unsigned int i)
{
	wostringstream stream;
	stream << i;
	return str + stream.str();
}

static wstring AddIntWString(int i, const wstring &str)
{
	wostringstream stream;
	stream << i;
	return stream.str() + str;
}

static wstring &AssignIntToWString(int i, wstring &dest)
{
	wostringstream stream;
	stream << i;
	dest = stream.str();
	return dest;
}

static wstring &AddAssignIntToWString(int i, wstring &dest)
{
	wostringstream stream;
	stream << i;
	dest += stream.str();
	return dest;
}

static wstring AddWStringInt(const wstring &str, int i)
{
	wostringstream stream;
	stream << i;
	return str + stream.str();
}

static wstring AddUIntWString(unsigned int i, const wstring &str)
{
	wostringstream stream;
	stream << i;
	return stream.str() + str;
}

static wstring &AssignDoubleToWString(double f, wstring &dest)
{
	wostringstream stream;
	stream << f;
	dest = stream.str();
	return dest;
}

static wstring &AddAssignDoubleToWString(double f, wstring &dest)
{
	wostringstream stream;
	stream << f;
	dest += stream.str();
	return dest;
}

static wstring &AssignBoolToWString(bool b, wstring &dest)
{
	wostringstream stream;
	stream << (b ? "true" : "false");
	dest = stream.str();
	return dest;
}

static wstring &AddAssignBoolToWString(bool b, wstring &dest)
{
	wostringstream stream;
	stream << (b ? "true" : "false");
	dest += stream.str();
	return dest;
}

static wstring AddWStringDouble(const wstring &str, double f)
{
	wostringstream stream;
	stream << f;
	return str + stream.str();
}

static wstring AddDoubleWString(double f, const wstring &str)
{
	wostringstream stream;
	stream << f;
	return stream.str() + str;
}

static wstring AddWStringBool(const wstring &str, bool b)
{
	wostringstream stream;
	stream << (b ? "true" : "false");
	return str + stream.str();
}

static wstring AddBoolWString(bool b, const wstring &str)
{
	wostringstream stream;
	stream << (b ? L"true" : L"false");
	return stream.str() + str;
}

static wchar_t *WStringCharAt(unsigned int i, wstring &str)
{
	if( i >= str.size() )
	{
		// Set a script exception
		asIScriptContext *ctx = asGetActiveContext();
		ctx->SetException("Out of range");

		// Return a null pointer
		return 0;
	}

	return &str[i];
}

// AngelScript signature:
// int wstring::opCmp(const wstring &in) const
static int WStringCmp(const wstring &a, const wstring &b)
{
	int cmp = 0;
	if( a < b ) cmp = -1;
	else if( a > b ) cmp = 1;
	return cmp;
}

// This function returns the index of the first position where the subwstring
// exists in the input wstring. If the subwstring doesn't exist in the input
// wstring -1 is returned.
//
// AngelScript signature:
// int wstring::findFirst(const wstring &in sub, uint start = 0) const
static int WStringFindFirst(const wstring &sub, asUINT start, const wstring &str)
{
	// We don't register the method directly because the argument types change between 32bit and 64bit platforms
	return (int)str.find(sub, start);
}

// This function returns the index of the last position where the subwstring
// exists in the input wstring. If the subwstring doesn't exist in the input
// wstring -1 is returned.
//
// AngelScript signature:
// int wstring::findLast(const wstring &in sub, int start = -1) const
static int WStringFindLast(const wstring &sub, int start, const wstring &str)
{
	// We don't register the method directly because the argument types change between 32bit and 64bit platforms
	return (int)str.rfind(sub, (size_t)start);
}

// AngelScript signature:
// uint wstring::length() const
static asUINT WStringLength(const wstring &str)
{
	// We don't register the method directly because the return type changes between 32bit and 64bit platforms
	return (asUINT)str.length();
}


// AngelScript signature:
// void wstring::resize(uint l)
static void WStringResize(asUINT l, wstring &str)
{
	// We don't register the method directly because the argument types change between 32bit and 64bit platforms
	str.resize(l);
}

// AngelScript signature:
// wstring formatIntW(int64 val, const wstring &in options, uint width)
static wstring formatIntW(asINT64 value, const wstring &options, asUINT width)
{
	bool leftJustify = options.find(L"l") != wstring::npos;
	bool padWithZero = options.find(L"0") != wstring::npos;
	bool alwaysSign  = options.find(L"+") != wstring::npos;
	bool spaceOnSign = options.find(L" ") != wstring::npos;
	bool hexSmall    = options.find(L"h") != wstring::npos;
	bool hexLarge    = options.find(L"H") != wstring::npos;

	wstring fmt = L"%";
	if( leftJustify ) fmt += L"-";
	if( alwaysSign ) fmt += L"+";
	if( spaceOnSign ) fmt += L" ";
	if( padWithZero ) fmt += L"0";

#ifdef __GNUC__
#ifdef _LP64
	fmt += L"*l";
#else
	fmt += L"*ll";
#endif
#else
	fmt += L"*I64";
#endif

	if( hexSmall ) fmt += L"x";
	else if( hexLarge ) fmt += L"X";
	else fmt += L"d";

	wstring buf;
	buf.resize(width+20);
#if _MSC_VER >= 1400 && !defined(__S3E__)
	// MSVC 8.0 / 2005 or newer
	swprintf_s(&buf[0], buf.size(), fmt.c_str(), width, value);
#else
	swprintf(&buf[0], fmt.c_str(), width, value);
#endif
	buf.resize(wcslen(&buf[0]));

	return buf;
}

// AngelScript signature:
// wstring formatFloatW(double val, const wstring &in options, uint width, uint precision)
static wstring formatFloatW(double value, const wstring &options, asUINT width, asUINT precision)
{
	bool leftJustify = options.find(L"l") != wstring::npos;
	bool padWithZero = options.find(L"0") != wstring::npos;
	bool alwaysSign  = options.find(L"+") != wstring::npos;
	bool spaceOnSign = options.find(L" ") != wstring::npos;
	bool expSmall    = options.find(L"e") != wstring::npos;
	bool expLarge    = options.find(L"E") != wstring::npos;

	wstring fmt = L"%";
	if( leftJustify ) fmt += L"-";
	if( alwaysSign ) fmt += L"+";
	if( spaceOnSign ) fmt += L" ";
	if( padWithZero ) fmt += L"0";

	fmt += L"*.*";

	if( expSmall ) fmt += L"e";
	else if( expLarge ) fmt += L"E";
	else fmt += L"f";

	wstring buf;
	buf.resize(width+precision+50);
#if _MSC_VER >= 1400 && !defined(__S3E__)
	// MSVC 8.0 / 2005 or newer
	swprintf_s(&buf[0], buf.size(), fmt.c_str(), width, precision, value);
#else
	swprintf(&buf[0], fmt.c_str(), width, precision, value);
#endif
	buf.resize(wcslen(&buf[0]));

	return buf;
}

// AngelScript signature:
// int64 parseIntW(const wstring &in val, uint base = 10, uint &out byteCount = 0)
static asINT64 parseIntW(const wstring &val, asUINT base, asUINT *byteCount)
{
	// Only accept base 10 and 16
	if( base != 10 && base != 16 )
	{
		if( byteCount ) *byteCount = 0;
		return 0;
	}

	const wchar_t *end = &val[0];

	// Determine the sign
	bool sign = false;
	if( *end == '-' )
	{
		sign = true;
		end++;
	}
	else if( *end == L'+' )
		end++;

	asINT64 res = 0;
	if( base == 10 )
	{
		while( *end >= L'0' && *end <= L'9' )
		{
			res *= 10;
			res += *end++ - L'0';
		}
	}
	else if( base == 16 )
	{
		while( (*end >= L'0' && *end <= L'9') ||
		       (*end >= L'a' && *end <= L'f') ||
		       (*end >= L'A' && *end <= L'F') )
		{
			res *= 16;
			if( *end >= L'0' && *end <= L'9' )
				res += *end++ - L'0';
			else if( *end >= L'a' && *end <= L'f' )
				res += *end++ - L'a' + 10;
			else if( *end >= L'A' && *end <= L'F' )
				res += *end++ - L'A' + 10;
		}
	}

	if( byteCount )
		*byteCount = asUINT(size_t(end - val.c_str()));

	if( sign )
		res = -res;

	return res;
}

// AngelScript signature:
// double parseFloatW(const wstring &in val, uint &out byteCount = 0)
double parseFloatW(const wstring &val, asUINT *byteCount)
{
	wchar_t *end;

    // WinCE doesn't have setlocale. Some quick testing on my current platform
    // still manages to parse the numbers such as "3.14" even if the decimal for the
    // locale is ",".
#if !defined(_WIN32_WCE) && !defined(ANDROID)
	// Set the locale to C so that we are guaranteed to parse the float value correctly
	char *orig = setlocale(LC_NUMERIC, 0);
	setlocale(LC_NUMERIC, "C");
#endif

	double res = wcstod(val.c_str(), &end);

#if !defined(_WIN32_WCE) && !defined(ANDROID)
	// Restore the locale
	setlocale(LC_NUMERIC, orig);
#endif

	if( byteCount )
		*byteCount = asUINT(size_t(end - val.c_str()));

	return res;
}

// This function returns a wstring containing the subwstring of the input wstring
// determined by the starting index and count of characters.
//
// AngelScript signature:
// wstring wstring::substr(uint start = 0, int count = -1) const
static wstring WStringSubWString(asUINT start, int count, const wstring &str)
{
	// Check for out-of-bounds
	wstring ret;
	if( start < str.length() && count != 0 )
		ret = str.substr(start, count);

	return ret;
}

// WString equality comparison.
// Returns true iff lhs is equal to rhs.
//
// For some reason gcc 4.7 has difficulties resolving the
// asFUNCTIONPR(operator==, (const wstring &, const wstring &)
// makro, so this wrapper was introduced as work around.
static bool WStringEquals(const std::wstring& lhs, const std::wstring& rhs)
{
    return lhs == rhs;
}

void RegisterStdWString_Native(asIScriptEngine *engine)
{
	int r;


	// Register the wstring type
	r = engine->RegisterObjectType("wstring", sizeof(wstring), asOBJ_VALUE | asOBJ_APP_CLASS_CDAK); assert( r >= 0 );

#if AS_USE_WSTRINGPOOL == 1
	// Register the wstring factory
	r = engine->RegisterStringFactory("const wstring &", asFUNCTION(WStringFactory), asCALL_CDECL); assert( r >= 0 );

	// Register the cleanup callback for the wstring pool
	engine->SetEngineUserDataCleanupCallback(CleanupEngineWStringPool, WSTRING_POOL);
#else
	// Register the wstring factory
	r = engine->RegisterStringFactory("wstring", asFUNCTION(WStringFactory), asCALL_CDECL); assert( r >= 0 );
#endif

	// Register the object operator overloads
	r = engine->RegisterObjectBehaviour("wstring", asBEHAVE_CONSTRUCT,  "void f()",                    asFUNCTION(ConstructWString), asCALL_CDECL_OBJLAST); assert( r >= 0 );
	r = engine->RegisterObjectBehaviour("wstring", asBEHAVE_CONSTRUCT,  "void f(const wstring &in)",    asFUNCTION(CopyConstructWString), asCALL_CDECL_OBJLAST); assert( r >= 0 );
	r = engine->RegisterObjectBehaviour("wstring", asBEHAVE_DESTRUCT,   "void f()",                    asFUNCTION(DestructWString),  asCALL_CDECL_OBJLAST); assert( r >= 0 );
	r = engine->RegisterObjectMethod("wstring", "wstring &opAssign(const wstring &in)", asMETHODPR(wstring, operator =, (const wstring&), wstring&), asCALL_THISCALL); assert( r >= 0 );
	// Need to use a wrapper on Mac OS X 10.7/XCode 4.3 and CLang/LLVM, otherwise the linker fails
	r = engine->RegisterObjectMethod("wstring", "wstring &opAddAssign(const wstring &in)", asFUNCTION(AddAssignWStringToWString), asCALL_CDECL_OBJLAST); assert( r >= 0 );
//	r = engine->RegisterObjectMethod("wstring", "wstring &opAddAssign(const wstring &in)", asMETHODPR(wstring, operator+=, (const wstring&), wstring&), asCALL_THISCALL); assert( r >= 0 );

	// Need to use a wrapper for operator== otherwise gcc 4.7 fails to compile
	r = engine->RegisterObjectMethod("wstring", "bool opEquals(const wstring &in) const", asFUNCTIONPR(WStringEquals, (const wstring &, const wstring &), bool), asCALL_CDECL_OBJFIRST); assert( r >= 0 );
	r = engine->RegisterObjectMethod("wstring", "int opCmp(const wstring &in) const", asFUNCTION(WStringCmp), asCALL_CDECL_OBJFIRST); assert( r >= 0 );
	r = engine->RegisterObjectMethod("wstring", "wstring opAdd(const wstring &in) const", asFUNCTIONPR(operator +, (const wstring &, const wstring &), wstring), asCALL_CDECL_OBJFIRST); assert( r >= 0 );

	// The wstring length can be accessed through methods or through virtual property
	r = engine->RegisterObjectMethod("wstring", "uint length() const", asFUNCTION(WStringLength), asCALL_CDECL_OBJLAST); assert( r >= 0 );
	r = engine->RegisterObjectMethod("wstring", "void resize(uint)", asFUNCTION(WStringResize), asCALL_CDECL_OBJLAST); assert( r >= 0 );
	r = engine->RegisterObjectMethod("wstring", "uint get_length() const", asFUNCTION(WStringLength), asCALL_CDECL_OBJLAST); assert( r >= 0 );
	r = engine->RegisterObjectMethod("wstring", "void set_length(uint)", asFUNCTION(WStringResize), asCALL_CDECL_OBJLAST); assert( r >= 0 );
	// Need to use a wrapper on Mac OS X 10.7/XCode 4.3 and CLang/LLVM, otherwise the linker fails
//	r = engine->RegisterObjectMethod("wstring", "bool isEmpty() const", asMETHOD(wstring, empty), asCALL_THISCALL); assert( r >= 0 );
	r = engine->RegisterObjectMethod("wstring", "bool isEmpty() const", asFUNCTION(WStringIsEmpty), asCALL_CDECL_OBJLAST); assert( r >= 0 );

	// Register the index operator, both as a mutator and as an inspector
	// Note that we don't register the operator[] directly, as it doesn't do bounds checking
	r = engine->RegisterObjectMethod("wstring", "uint8 &opIndex(uint)", asFUNCTION(WStringCharAt), asCALL_CDECL_OBJLAST); assert( r >= 0 );
	r = engine->RegisterObjectMethod("wstring", "const uint8 &opIndex(uint) const", asFUNCTION(WStringCharAt), asCALL_CDECL_OBJLAST); assert( r >= 0 );

	// Automatic conversion from values
	r = engine->RegisterObjectMethod("wstring", "wstring &opAssign(double)", asFUNCTION(AssignDoubleToWString), asCALL_CDECL_OBJLAST); assert( r >= 0 );
	r = engine->RegisterObjectMethod("wstring", "wstring &opAddAssign(double)", asFUNCTION(AddAssignDoubleToWString), asCALL_CDECL_OBJLAST); assert( r >= 0 );
	r = engine->RegisterObjectMethod("wstring", "wstring opAdd(double) const", asFUNCTION(AddWStringDouble), asCALL_CDECL_OBJFIRST); assert( r >= 0 );
	r = engine->RegisterObjectMethod("wstring", "wstring opAdd_r(double) const", asFUNCTION(AddDoubleWString), asCALL_CDECL_OBJLAST); assert( r >= 0 );

	r = engine->RegisterObjectMethod("wstring", "wstring &opAssign(int)", asFUNCTION(AssignIntToWString), asCALL_CDECL_OBJLAST); assert( r >= 0 );
	r = engine->RegisterObjectMethod("wstring", "wstring &opAddAssign(int)", asFUNCTION(AddAssignIntToWString), asCALL_CDECL_OBJLAST); assert( r >= 0 );
	r = engine->RegisterObjectMethod("wstring", "wstring opAdd(int) const", asFUNCTION(AddWStringInt), asCALL_CDECL_OBJFIRST); assert( r >= 0 );
	r = engine->RegisterObjectMethod("wstring", "wstring opAdd_r(int) const", asFUNCTION(AddIntWString), asCALL_CDECL_OBJLAST); assert( r >= 0 );

	r = engine->RegisterObjectMethod("wstring", "wstring &opAssign(uint)", asFUNCTION(AssignUIntToWString), asCALL_CDECL_OBJLAST); assert( r >= 0 );
	r = engine->RegisterObjectMethod("wstring", "wstring &opAddAssign(uint)", asFUNCTION(AddAssignUIntToWString), asCALL_CDECL_OBJLAST); assert( r >= 0 );
	r = engine->RegisterObjectMethod("wstring", "wstring opAdd(uint) const", asFUNCTION(AddWStringUInt), asCALL_CDECL_OBJFIRST); assert( r >= 0 );
	r = engine->RegisterObjectMethod("wstring", "wstring opAdd_r(uint) const", asFUNCTION(AddUIntWString), asCALL_CDECL_OBJLAST); assert( r >= 0 );

	r = engine->RegisterObjectMethod("wstring", "wstring &opAssign(bool)", asFUNCTION(AssignBoolToWString), asCALL_CDECL_OBJLAST); assert( r >= 0 );
	r = engine->RegisterObjectMethod("wstring", "wstring &opAddAssign(bool)", asFUNCTION(AddAssignBoolToWString), asCALL_CDECL_OBJLAST); assert( r >= 0 );
	r = engine->RegisterObjectMethod("wstring", "wstring opAdd(bool) const", asFUNCTION(AddWStringBool), asCALL_CDECL_OBJFIRST); assert( r >= 0 );
	r = engine->RegisterObjectMethod("wstring", "wstring opAdd_r(bool) const", asFUNCTION(AddBoolWString), asCALL_CDECL_OBJLAST); assert( r >= 0 );

	// Utilities
	r = engine->RegisterObjectMethod("wstring", "wstring substr(uint start = 0, int count = -1) const", asFUNCTION(WStringSubWString), asCALL_CDECL_OBJLAST); assert( r >= 0 );
	r = engine->RegisterObjectMethod("wstring", "int findFirst(const wstring &in, uint start = 0) const", asFUNCTION(WStringFindFirst), asCALL_CDECL_OBJLAST); assert( r >= 0 );
	r = engine->RegisterObjectMethod("wstring", "int findLast(const wstring &in, int start = -1) const", asFUNCTION(WStringFindLast), asCALL_CDECL_OBJLAST); assert( r >= 0 );

	r = engine->RegisterGlobalFunction("wstring formatIntW(int64 val, const wstring &in options, uint width = 0)", asFUNCTION(formatIntW), asCALL_CDECL); assert(r >= 0);
	r = engine->RegisterGlobalFunction("wstring formatFloatW(double val, const wstring &in options, uint width = 0, uint precision = 0)", asFUNCTION(formatFloatW), asCALL_CDECL); assert(r >= 0);
	r = engine->RegisterGlobalFunction("int64 parseIntW(const wstring &in, uint base = 10, uint &out byteCount = 0)", asFUNCTION(parseIntW), asCALL_CDECL); assert(r >= 0);
	r = engine->RegisterGlobalFunction("double parseFloatW(const wstring &in, uint &out byteCount = 0)", asFUNCTION(parseFloatW), asCALL_CDECL); assert(r >= 0);

#if AS_USE_STLNAMES == 1
	// Same as length
	r = engine->RegisterObjectMethod("wstring", "uint size() const", asFUNCTION(WStringLength), asCALL_CDECL_OBJLAST); assert( r >= 0 );
	// Same as isEmpty
	r = engine->RegisterObjectMethod("wstring", "bool empty() const", asFUNCTION(WStringIsEmpty), asCALL_CDECL_OBJLAST); assert( r >= 0 );
	// Same as findFirst
	r = engine->RegisterObjectMethod("wstring", "int find(const wstring &in, uint start = 0) const", asFUNCTION(WStringFindFirst), asCALL_CDECL_OBJLAST); assert( r >= 0 );
	// Same as findLast
	r = engine->RegisterObjectMethod("wstring", "int rfind(const wstring &in, int start = -1) const", asFUNCTION(WStringFindLast), asCALL_CDECL_OBJLAST); assert( r >= 0 );
#endif

	// TODO: Implement the following
	// findFirstOf
	// findLastOf
	// findFirstNotOf
	// findLastNotOf
	// findAndReplace - replaces a text found in the wstring
	// replaceRange - replaces a range of bytes in the wstring
	// trim/trimLeft/trimRight
	// multiply/times/opMul/opMul_r - takes the wstring and multiplies it n times, e.g. "-".multiply(5) returns "-----"
}

#if AS_USE_WSTRINGPOOL == 1
static void WStringFactoryGeneric(asIScriptGeneric *gen)
{
  //Because UTF-16 is 2 bytes, length of the string is length/2
  asUINT length = gen->GetArgDWord(0) / 2;
  const wchar_t *s = (const wchar_t*)gen->GetArgAddress(1);

  // Return a reference to a wstring
  gen->SetReturnAddress(const_cast<wstring*>(&WStringFactory(length, s)));
}
#else
static void WStringFactoryGeneric(asIScriptGeneric *gen)
{
  //Because UTF-16 is 2 bytes, length of the string is length/2
  asUINT length = gen->GetArgDWord(0) / 2;
  const wchar_t *s = (const wchar_t*)gen->GetArgAddress(1);

  // Return a wstring value
  new (gen->GetAddressOfReturnLocation()) wstring(WStringFactory(length, s));
}
#endif

static void ConstructWStringGeneric(asIScriptGeneric * gen)
{
  new (gen->GetObject()) wstring();
}

static void CopyConstructWStringGeneric(asIScriptGeneric * gen)
{
  wstring * a = static_cast<wstring *>(gen->GetArgObject(0));
  new (gen->GetObject()) wstring(*a);
}

static void DestructWStringGeneric(asIScriptGeneric * gen)
{
  wstring * ptr = static_cast<wstring *>(gen->GetObject());
  ptr->~wstring();
}

static void AssignWStringGeneric(asIScriptGeneric *gen)
{
  wstring * a = static_cast<wstring *>(gen->GetArgObject(0));
  wstring * self = static_cast<wstring *>(gen->GetObject());
  *self = *a;
  gen->SetReturnAddress(self);
}

static void AddAssignWStringGeneric(asIScriptGeneric *gen)
{
  wstring * a = static_cast<wstring *>(gen->GetArgObject(0));
  wstring * self = static_cast<wstring *>(gen->GetObject());
  *self += *a;
  gen->SetReturnAddress(self);
}

static void WStringEqualsGeneric(asIScriptGeneric * gen)
{
  wstring * a = static_cast<wstring *>(gen->GetObject());
  wstring * b = static_cast<wstring *>(gen->GetArgAddress(0));
  *(bool*)gen->GetAddressOfReturnLocation() = (*a == *b);
}

static void WStringCmpGeneric(asIScriptGeneric * gen)
{
  wstring * a = static_cast<wstring *>(gen->GetObject());
  wstring * b = static_cast<wstring *>(gen->GetArgAddress(0));

  int cmp = 0;
  if( *a < *b ) cmp = -1;
  else if( *a > *b ) cmp = 1;

  *(int*)gen->GetAddressOfReturnLocation() = cmp;
}

static void WStringAddGeneric(asIScriptGeneric * gen)
{
  wstring * a = static_cast<wstring *>(gen->GetObject());
  wstring * b = static_cast<wstring *>(gen->GetArgAddress(0));
  wstring ret_val = *a + *b;
  gen->SetReturnObject(&ret_val);
}

static void WStringLengthGeneric(asIScriptGeneric * gen)
{
  wstring * self = static_cast<wstring *>(gen->GetObject());
  *static_cast<asUINT *>(gen->GetAddressOfReturnLocation()) = (asUINT)self->length();
}

static void WStringIsEmptyGeneric(asIScriptGeneric * gen)
{
  wstring * self = reinterpret_cast<wstring *>(gen->GetObject());
  *reinterpret_cast<bool *>(gen->GetAddressOfReturnLocation()) = WStringIsEmpty(*self);
}

static void WStringResizeGeneric(asIScriptGeneric * gen)
{
  wstring * self = static_cast<wstring *>(gen->GetObject());
  self->resize(*static_cast<asUINT *>(gen->GetAddressOfArg(0)));
}

static void WStringFindFirst_Generic(asIScriptGeneric * gen)
{
	wstring *find = reinterpret_cast<wstring*>(gen->GetArgAddress(0));
	asUINT start = gen->GetArgDWord(1);
	wstring *self = reinterpret_cast<wstring *>(gen->GetObject());
	*reinterpret_cast<int *>(gen->GetAddressOfReturnLocation()) = WStringFindFirst(*find, start, *self);
}

static void WStringFindLast_Generic(asIScriptGeneric * gen)
{
	wstring *find = reinterpret_cast<wstring*>(gen->GetArgAddress(0));
	asUINT start = gen->GetArgDWord(1);
	wstring *self = reinterpret_cast<wstring *>(gen->GetObject());
	*reinterpret_cast<int *>(gen->GetAddressOfReturnLocation()) = WStringFindLast(*find, start, *self);
}

static void formatIntW_Generic(asIScriptGeneric * gen)
{
	asINT64 val = gen->GetArgQWord(0);
	wstring *options = reinterpret_cast<wstring*>(gen->GetArgAddress(1));
	asUINT width = gen->GetArgDWord(2);
	new(gen->GetAddressOfReturnLocation()) wstring(formatIntW(val, *options, width));
}

static void formatFloatW_Generic(asIScriptGeneric *gen)
{
	double val = gen->GetArgDouble(0);
	wstring *options = reinterpret_cast<wstring*>(gen->GetArgAddress(1));
	asUINT width = gen->GetArgDWord(2);
	asUINT precision = gen->GetArgDWord(3);
	new(gen->GetAddressOfReturnLocation()) wstring(formatFloatW(val, *options, width, precision));
}

static void parseIntW_Generic(asIScriptGeneric *gen)
{
	wstring *str = reinterpret_cast<wstring*>(gen->GetArgAddress(0));
	asUINT base = gen->GetArgDWord(1);
	asUINT *byteCount = reinterpret_cast<asUINT*>(gen->GetArgAddress(2));
	gen->SetReturnQWord(parseIntW(*str,base,byteCount));
}

static void parseFloatW_Generic(asIScriptGeneric *gen)
{
	wstring *str = reinterpret_cast<wstring*>(gen->GetArgAddress(0));
	asUINT *byteCount = reinterpret_cast<asUINT*>(gen->GetArgAddress(1));
	gen->SetReturnDouble(parseFloatW(*str,byteCount));
}

static void WStringCharAtGeneric(asIScriptGeneric * gen)
{
  unsigned int index = gen->GetArgDWord(0);
  wstring * self = static_cast<wstring *>(gen->GetObject());

  if (index >= self->size())
  {
    // Set a script exception
    asIScriptContext *ctx = asGetActiveContext();
    ctx->SetException("Out of range");

    gen->SetReturnAddress(0);
  }
  else
  {
    gen->SetReturnAddress(&(self->operator [](index)));
  }
}

static void AssignInt2WStringGeneric(asIScriptGeneric *gen)
{
	int *a = static_cast<int*>(gen->GetAddressOfArg(0));
	wstring *self = static_cast<wstring*>(gen->GetObject());
	std::wstringstream sstr;
	sstr << *a;
	*self = sstr.str();
	gen->SetReturnAddress(self);
}

static void AssignUInt2WStringGeneric(asIScriptGeneric *gen)
{
	unsigned int *a = static_cast<unsigned int*>(gen->GetAddressOfArg(0));
	wstring *self = static_cast<wstring*>(gen->GetObject());
	std::wstringstream sstr;
	sstr << *a;
	*self = sstr.str();
	gen->SetReturnAddress(self);
}

static void AssignDouble2WStringGeneric(asIScriptGeneric *gen)
{
	double *a = static_cast<double*>(gen->GetAddressOfArg(0));
	wstring *self = static_cast<wstring*>(gen->GetObject());
	std::wstringstream sstr;
	sstr << *a;
	*self = sstr.str();
	gen->SetReturnAddress(self);
}

static void AssignBool2WStringGeneric(asIScriptGeneric *gen)
{
	bool *a = static_cast<bool*>(gen->GetAddressOfArg(0));
	wstring *self = static_cast<wstring*>(gen->GetObject());
	std::wstringstream sstr;
	sstr << (*a ? "true" : "false");
	*self = sstr.str();
	gen->SetReturnAddress(self);
}

static void AddAssignDouble2WStringGeneric(asIScriptGeneric * gen)
{
  double * a = static_cast<double *>(gen->GetAddressOfArg(0));
  wstring * self = static_cast<wstring *>(gen->GetObject());
  std::wstringstream sstr;
  sstr << *a;
  *self += sstr.str();
  gen->SetReturnAddress(self);
}

static void AddAssignInt2WStringGeneric(asIScriptGeneric * gen)
{
  int * a = static_cast<int *>(gen->GetAddressOfArg(0));
  wstring * self = static_cast<wstring *>(gen->GetObject());
  std::wstringstream sstr;
  sstr << *a;
  *self += sstr.str();
  gen->SetReturnAddress(self);
}

static void AddAssignUInt2WStringGeneric(asIScriptGeneric * gen)
{
  unsigned int * a = static_cast<unsigned int *>(gen->GetAddressOfArg(0));
  wstring * self = static_cast<wstring *>(gen->GetObject());
  std::wstringstream sstr;
  sstr << *a;
  *self += sstr.str();
  gen->SetReturnAddress(self);
}

static void AddAssignBool2WStringGeneric(asIScriptGeneric * gen)
{
  bool * a = static_cast<bool *>(gen->GetAddressOfArg(0));
  wstring * self = static_cast<wstring *>(gen->GetObject());
  std::wstringstream sstr;
  sstr << (*a ? "true" : "false");
  *self += sstr.str();
  gen->SetReturnAddress(self);
}

static void AddWString2DoubleGeneric(asIScriptGeneric * gen)
{
  wstring * a = static_cast<wstring *>(gen->GetObject());
  double * b = static_cast<double *>(gen->GetAddressOfArg(0));
  std::wstringstream sstr;
  sstr << *a << *b;
  std::wstring ret_val = sstr.str();
  gen->SetReturnObject(&ret_val);
}

static void AddWString2IntGeneric(asIScriptGeneric * gen)
{
  wstring * a = static_cast<wstring *>(gen->GetObject());
  int * b = static_cast<int *>(gen->GetAddressOfArg(0));
  std::wstringstream sstr;
  sstr << *a << *b;
  std::wstring ret_val = sstr.str();
  gen->SetReturnObject(&ret_val);
}

static void AddWString2UIntGeneric(asIScriptGeneric * gen)
{
  wstring * a = static_cast<wstring *>(gen->GetObject());
  unsigned int * b = static_cast<unsigned int *>(gen->GetAddressOfArg(0));
  std::wstringstream sstr;
  sstr << *a << *b;
  std::wstring ret_val = sstr.str();
  gen->SetReturnObject(&ret_val);
}

static void AddWString2BoolGeneric(asIScriptGeneric * gen)
{
  wstring * a = static_cast<wstring *>(gen->GetObject());
  bool * b = static_cast<bool *>(gen->GetAddressOfArg(0));
  std::wstringstream sstr;
  sstr << *a << (*b ? "true" : "false");
  std::wstring ret_val = sstr.str();
  gen->SetReturnObject(&ret_val);
}

static void AddDouble2WStringGeneric(asIScriptGeneric * gen)
{
  double* a = static_cast<double *>(gen->GetAddressOfArg(0));
  wstring * b = static_cast<wstring *>(gen->GetObject());
  std::wstringstream sstr;
  sstr << *a << *b;
  std::wstring ret_val = sstr.str();
  gen->SetReturnObject(&ret_val);
}

static void AddInt2WStringGeneric(asIScriptGeneric * gen)
{
  int* a = static_cast<int *>(gen->GetAddressOfArg(0));
  wstring * b = static_cast<wstring *>(gen->GetObject());
  std::wstringstream sstr;
  sstr << *a << *b;
  std::wstring ret_val = sstr.str();
  gen->SetReturnObject(&ret_val);
}

static void AddUInt2WStringGeneric(asIScriptGeneric * gen)
{
  unsigned int* a = static_cast<unsigned int *>(gen->GetAddressOfArg(0));
  wstring * b = static_cast<wstring *>(gen->GetObject());
  std::wstringstream sstr;
  sstr << *a << *b;
  std::wstring ret_val = sstr.str();
  gen->SetReturnObject(&ret_val);
}

static void AddBool2WStringGeneric(asIScriptGeneric * gen)
{
  bool* a = static_cast<bool *>(gen->GetAddressOfArg(0));
  wstring * b = static_cast<wstring *>(gen->GetObject());
  std::wstringstream sstr;
  sstr << (*a ? "true" : "false") << *b;
  std::wstring ret_val = sstr.str();
  gen->SetReturnObject(&ret_val);
}

static void WStringSubWString_Generic(asIScriptGeneric *gen)
{
    // Get the arguments
    wstring *str   = (wstring*)gen->GetObject();
    asUINT  start = *(int*)gen->GetAddressOfArg(0);
    int     count = *(int*)gen->GetAddressOfArg(1);

	// Return the subwstring
    new(gen->GetAddressOfReturnLocation()) wstring(WStringSubWString(start, count, *str));
}

void RegisterStdWString_Generic(asIScriptEngine *engine)
{
	int r;

	// Register the wstring type
	r = engine->RegisterObjectType("wstring", sizeof(wstring), asOBJ_VALUE | asOBJ_APP_CLASS_CDAK); assert( r >= 0 );

#if AS_USE_WSTRINGPOOL == 1
	// Register the wstring factory
	r = engine->RegisterStringFactory("const wstring &", asFUNCTION(WStringFactoryGeneric), asCALL_GENERIC); assert( r >= 0 );

	// Register the cleanup callback for the wstring pool
	engine->SetEngineUserDataCleanupCallback(CleanupEngineWStringPool, WSTRING_POOL);
#else
	// Register the wstring factory
	r = engine->RegisterWStringFactory("wstring", asFUNCTION(WStringFactoryGeneric), asCALL_GENERIC); assert( r >= 0 );
#endif

	// Register the object operator overloads
	r = engine->RegisterObjectBehaviour("wstring", asBEHAVE_CONSTRUCT,  "void f()",                    asFUNCTION(ConstructWStringGeneric), asCALL_GENERIC); assert( r >= 0 );
	r = engine->RegisterObjectBehaviour("wstring", asBEHAVE_CONSTRUCT,  "void f(const wstring &in)",    asFUNCTION(CopyConstructWStringGeneric), asCALL_GENERIC); assert( r >= 0 );
	r = engine->RegisterObjectBehaviour("wstring", asBEHAVE_DESTRUCT,   "void f()",                    asFUNCTION(DestructWStringGeneric),  asCALL_GENERIC); assert( r >= 0 );
	r = engine->RegisterObjectMethod("wstring", "wstring &opAssign(const wstring &in)", asFUNCTION(AssignWStringGeneric),    asCALL_GENERIC); assert( r >= 0 );
	r = engine->RegisterObjectMethod("wstring", "wstring &opAddAssign(const wstring &in)", asFUNCTION(AddAssignWStringGeneric), asCALL_GENERIC); assert( r >= 0 );

	r = engine->RegisterObjectMethod("wstring", "bool opEquals(const wstring &in) const", asFUNCTION(WStringEqualsGeneric), asCALL_GENERIC); assert( r >= 0 );
	r = engine->RegisterObjectMethod("wstring", "int opCmp(const wstring &in) const", asFUNCTION(WStringCmpGeneric), asCALL_GENERIC); assert( r >= 0 );
	r = engine->RegisterObjectMethod("wstring", "wstring opAdd(const wstring &in) const", asFUNCTION(WStringAddGeneric), asCALL_GENERIC); assert( r >= 0 );

	// Register the object methods
	r = engine->RegisterObjectMethod("wstring", "uint length() const", asFUNCTION(WStringLengthGeneric), asCALL_GENERIC); assert( r >= 0 );
	r = engine->RegisterObjectMethod("wstring", "void resize(uint)",   asFUNCTION(WStringResizeGeneric), asCALL_GENERIC); assert( r >= 0 );
	r = engine->RegisterObjectMethod("wstring", "uint get_length() const", asFUNCTION(WStringLengthGeneric), asCALL_GENERIC); assert( r >= 0 );
	r = engine->RegisterObjectMethod("wstring", "void set_length(uint)", asFUNCTION(WStringResizeGeneric), asCALL_GENERIC); assert( r >= 0 );
	r = engine->RegisterObjectMethod("wstring", "bool isEmpty() const", asFUNCTION(WStringIsEmptyGeneric), asCALL_GENERIC); assert( r >= 0 );

	// Register the index operator, both as a mutator and as an inspector
	r = engine->RegisterObjectMethod("wstring", "uint8 &opIndex(uint)", asFUNCTION(WStringCharAtGeneric), asCALL_GENERIC); assert( r >= 0 );
	r = engine->RegisterObjectMethod("wstring", "const uint8 &opIndex(uint) const", asFUNCTION(WStringCharAtGeneric), asCALL_GENERIC); assert( r >= 0 );

	// Automatic conversion from values
	r = engine->RegisterObjectMethod("wstring", "wstring &opAssign(double)", asFUNCTION(AssignDouble2WStringGeneric), asCALL_GENERIC); assert( r >= 0 );
	r = engine->RegisterObjectMethod("wstring", "wstring &opAddAssign(double)", asFUNCTION(AddAssignDouble2WStringGeneric), asCALL_GENERIC); assert( r >= 0 );
	r = engine->RegisterObjectMethod("wstring", "wstring opAdd(double) const", asFUNCTION(AddWString2DoubleGeneric), asCALL_GENERIC); assert( r >= 0 );
	r = engine->RegisterObjectMethod("wstring", "wstring opAdd_r(double) const", asFUNCTION(AddDouble2WStringGeneric), asCALL_GENERIC); assert( r >= 0 );

	r = engine->RegisterObjectMethod("wstring", "wstring &opAssign(int)", asFUNCTION(AssignInt2WStringGeneric), asCALL_GENERIC); assert( r >= 0 );
	r = engine->RegisterObjectMethod("wstring", "wstring &opAddAssign(int)", asFUNCTION(AddAssignInt2WStringGeneric), asCALL_GENERIC); assert( r >= 0 );
	r = engine->RegisterObjectMethod("wstring", "wstring opAdd(int) const", asFUNCTION(AddWString2IntGeneric), asCALL_GENERIC); assert( r >= 0 );
	r = engine->RegisterObjectMethod("wstring", "wstring opAdd_r(int) const", asFUNCTION(AddInt2WStringGeneric), asCALL_GENERIC); assert( r >= 0 );

	r = engine->RegisterObjectMethod("wstring", "wstring &opAssign(uint)", asFUNCTION(AssignUInt2WStringGeneric), asCALL_GENERIC); assert( r >= 0 );
	r = engine->RegisterObjectMethod("wstring", "wstring &opAddAssign(uint)", asFUNCTION(AddAssignUInt2WStringGeneric), asCALL_GENERIC); assert( r >= 0 );
	r = engine->RegisterObjectMethod("wstring", "wstring opAdd(uint) const", asFUNCTION(AddWString2UIntGeneric), asCALL_GENERIC); assert( r >= 0 );
	r = engine->RegisterObjectMethod("wstring", "wstring opAdd_r(uint) const", asFUNCTION(AddUInt2WStringGeneric), asCALL_GENERIC); assert( r >= 0 );

	r = engine->RegisterObjectMethod("wstring", "wstring &opAssign(bool)", asFUNCTION(AssignBool2WStringGeneric), asCALL_GENERIC); assert( r >= 0 );
	r = engine->RegisterObjectMethod("wstring", "wstring &opAddAssign(bool)", asFUNCTION(AddAssignBool2WStringGeneric), asCALL_GENERIC); assert( r >= 0 );
	r = engine->RegisterObjectMethod("wstring", "wstring opAdd(bool) const", asFUNCTION(AddWString2BoolGeneric), asCALL_GENERIC); assert( r >= 0 );
	r = engine->RegisterObjectMethod("wstring", "wstring opAdd_r(bool) const", asFUNCTION(AddBool2WStringGeneric), asCALL_GENERIC); assert( r >= 0 );

	r = engine->RegisterObjectMethod("wstring", "wstring substr(uint start = 0, int count = -1) const", asFUNCTION(WStringSubWString_Generic), asCALL_GENERIC); assert( r >= 0 );
	r = engine->RegisterObjectMethod("wstring", "int findFirst(const wstring &in, uint start = 0) const", asFUNCTION(WStringFindFirst_Generic), asCALL_GENERIC); assert( r >= 0 );
	r = engine->RegisterObjectMethod("wstring", "int findLast(const wstring &in, int start = -1) const", asFUNCTION(WStringFindLast_Generic), asCALL_GENERIC); assert( r >= 0 );

	r = engine->RegisterGlobalFunction("wstring formatIntW(int64 val, const wstring &in options, uint width = 0)", asFUNCTION(formatIntW_Generic), asCALL_GENERIC); assert(r >= 0);
	r = engine->RegisterGlobalFunction("wstring formatFloatW(double val, const wstring &in options, uint width = 0, uint precision = 0)", asFUNCTION(formatFloatW_Generic), asCALL_GENERIC); assert(r >= 0);
	r = engine->RegisterGlobalFunction("int64 parseIntW(const wstring &in, uint base = 10, uint &out byteCount = 0)", asFUNCTION(parseIntW_Generic), asCALL_GENERIC); assert(r >= 0);
	r = engine->RegisterGlobalFunction("double parseFloatW(const wstring &in, uint &out byteCount = 0)", asFUNCTION(parseFloatW_Generic), asCALL_GENERIC); assert(r >= 0);
}

void RegisterStdWString(asIScriptEngine * engine)
{
	if (strstr(asGetLibraryOptions(), "AS_MAX_PORTABILITY"))
		RegisterStdWString_Generic(engine);
	else
		RegisterStdWString_Native(engine);
}

END_AS_NAMESPACE




