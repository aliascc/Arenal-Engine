#include "scriptstdwstring.h"
#include <assert.h> // assert()
#include <sstream>  // std::wstringstream
#include <string.h> // strstr()
#include <stdio.h>	// sprintf()
#include <stdlib.h> // wcstod()
#ifndef __psp2__
	#include <locale.h> // setlocale()
#endif

using namespace std;

// This macro is used to avoid warnings about unused variables.
// Usually where the variables are only used in debug mode.
#define UNUSED_VAR(x) (void)(x)

#ifdef AS_CAN_USE_CPP11
// The wstring factory doesn't need to keep a specific order in the
// cache, so the unordered_map is faster than the ordinary map
#include <unordered_map>  // std::unordered_map
BEGIN_AS_NAMESPACE
typedef unordered_map<wstring, int> map_t;
END_AS_NAMESPACE
#else
#include <map>      // std::map
BEGIN_AS_NAMESPACE
typedef map<wstring, int> map_t;
END_AS_NAMESPACE
#endif

class CStdWStringFactory : public asIStringFactory
{
public:
	CStdWStringFactory() {}
	~CStdWStringFactory() 
	{
		// The script engine must release each wstring 
		// constant that it has requested
		assert(wstringCache.size() == 0);
	}

	const void *GetStringConstant(const char *dataX, asUINT length)
	{
		const wchar_t* data = reinterpret_cast<const wchar_t*>(dataX);
		wstring str(data, length);
		map_t::iterator it = wstringCache.find(str);
		if (it != wstringCache.end())
			it->second++;
		else
			it = wstringCache.insert(map_t::value_type(str, 1)).first;

		return reinterpret_cast<const void*>(&it->first);
	}

	int  ReleaseStringConstant(const void *str)
	{
		if (str == 0)
			return asERROR;

		map_t::iterator it = wstringCache.find(*reinterpret_cast<const wstring*>(str));
		if (it == wstringCache.end())
			return asERROR;

		it->second--;
		if (it->second == 0)
			wstringCache.erase(it);
		return asSUCCESS;
	}

	int  GetRawStringData(const void *str, char *dataX, asUINT *length) const
	{
		wchar_t* data = reinterpret_cast<wchar_t*>(dataX);
		if (str == 0)
			return asERROR;

		if (length)
			*length = (asUINT)reinterpret_cast<const wstring*>(str)->length();

		if (data)
			memcpy(data, reinterpret_cast<const wstring*>(str)->c_str(), reinterpret_cast<const wstring*>(str)->length());

		return asSUCCESS;
	}

	// TODO: Make sure the access to the wstring cache is thread safe
	map_t wstringCache;
};

static CStdWStringFactory wstringFactory;


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

static wstring &AssignUInt64ToWString(asQWORD i, wstring &dest)
{
	wostringstream stream;
	stream << i;
	dest = stream.str();
	return dest;
}

static wstring &AddAssignUInt64ToWString(asQWORD i, wstring &dest)
{
	wostringstream stream;
	stream << i;
	dest += stream.str();
	return dest;
}

static wstring AddWStringUInt64(const wstring &str, asQWORD i)
{
	wostringstream stream;
	stream << i;
	return str + stream.str();
}

static wstring AddInt64WString(asINT64 i, const wstring &str)
{
	wostringstream stream;
	stream << i;
	return stream.str() + str;
}

static wstring &AssignInt64ToWString(asINT64 i, wstring &dest)
{
	wostringstream stream;
	stream << i;
	dest = stream.str();
	return dest;
}

static wstring &AddAssignInt64ToWString(asINT64 i, wstring &dest)
{
	wostringstream stream;
	stream << i;
	dest += stream.str();
	return dest;
}

static wstring AddWStringInt64(const wstring &str, asINT64 i)
{
	wostringstream stream;
	stream << i;
	return str + stream.str();
}

static wstring AddUInt64WString(asQWORD i, const wstring &str)
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

static wstring &AssignFloatToWString(float f, wstring &dest)
{
	wostringstream stream;
	stream << f;
	dest = stream.str();
	return dest;
}

static wstring &AddAssignFloatToWString(float f, wstring &dest)
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
	stream << (b ? L"true" : L"false");
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

static wstring AddWStringFloat(const wstring &str, float f)
{
	wostringstream stream;
	stream << f;
	return str + stream.str();
}

static wstring AddFloatWString(float f, const wstring &str)
{
	wostringstream stream;
	stream << f;
	return stream.str() + str;
}

static wstring AddWStringBool(const wstring &str, bool b)
{
	wostringstream stream;
	stream << (b ? L"true" : L"false");
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
	return (int)str.find(sub, (size_t)(start < 0 ? wstring::npos : start));
}

// This function returns the index of the first position where the one of the bytes in subwstring
// exists in the input wstring. If the characters in the subwstring doesn't exist in the input
// wstring -1 is returned.
//
// AngelScript signature:
// int wstring::findFirstOf(const wstring &in sub, uint start = 0) const
static int WStringFindFirstOf(const wstring &sub, asUINT start, const wstring &str)
{
	// We don't register the method directly because the argument types change between 32bit and 64bit platforms
	return (int)str.find_first_of(sub, (size_t)(start < 0 ? wstring::npos : start));
}

// This function returns the index of the last position where the one of the bytes in subwstring
// exists in the input wstring. If the characters in the subwstring doesn't exist in the input
// wstring -1 is returned.
//
// AngelScript signature:
// int wstring::findLastOf(const wstring &in sub, uint start = -1) const
static int WStringFindLastOf(const wstring &sub, asUINT start, const wstring &str)
{
	// We don't register the method directly because the argument types change between 32bit and 64bit platforms
	return (int)str.find_last_of(sub, (size_t)(start < 0 ? wstring::npos : start));
}

// This function returns the index of the first position where a byte other than those in subwstring
// exists in the input wstring. If none is found -1 is returned.
//
// AngelScript signature:
// int wstring::findFirstNotOf(const wstring &in sub, uint start = 0) const
static int WStringFindFirstNotOf(const wstring &sub, asUINT start, const wstring &str)
{
	// We don't register the method directly because the argument types change between 32bit and 64bit platforms
	return (int)str.find_first_not_of(sub, (size_t)(start < 0 ? wstring::npos : start));
}

// This function returns the index of the last position where a byte other than those in subwstring
// exists in the input wstring. If none is found -1 is returned.
//
// AngelScript signature:
// int wstring::findLastNotOf(const wstring &in sub, uint start = -1) const
static int WStringFindLastNotOf(const wstring &sub, asUINT start, const wstring &str)
{
	// We don't register the method directly because the argument types change between 32bit and 64bit platforms
	return (int)str.find_last_of(sub, (size_t)(start < 0 ? wstring::npos : start));
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
	return (int)str.rfind(sub, (size_t)(start < 0 ? wstring::npos : start));
}

// AngelScript signature:
// void wstring::insert(uint pos, const wstring &in other)
static void WStringInsert(unsigned int pos, const wstring &other, wstring &str)
{
	// We don't register the method directly because the argument types change between 32bit and 64bit platforms
	str.insert(pos, other);
}

// AngelScript signature:
// void wstring::erase(uint pos, int count = -1)
static void WStringErase(unsigned int pos, int count, wstring &str)
{
	// We don't register the method directly because the argument types change between 32bit and 64bit platforms
	str.erase(pos, (size_t)(count < 0 ? wstring::npos : count));
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
// wstring formatInt(int64 val, const wstring &in options, uint width)
static wstring formatInt(asINT64 value, const wstring &options, asUINT width)
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

#ifdef _WIN32
	fmt += L"*I64";
#else
#ifdef _LP64
	fmt += L"*l";
#else
	fmt += L"*ll";
#endif
#endif

	if( hexSmall ) fmt += L"x";
	else if( hexLarge ) fmt += L"X";
	else fmt += L"d";

	wstring buf;
	buf.resize(width+30);
#if _MSC_VER >= 1400 && !defined(__S3E__)
	// MSVC 8.0 / 2005 or newer
	swprintf_s(&buf[0], buf.size(), fmt.c_str(), width, value);
#else
	sprintf(&buf[0], fmt.c_str(), width, value);
#endif
	buf.resize(wcslen(&buf[0]));

	return buf;
}

// AngelScript signature:
// wstring formatUInt(uint64 val, const wstring &in options, uint width)
static wstring formatUInt(asQWORD value, const wstring &options, asUINT width)
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

#ifdef _WIN32
	fmt += L"*I64";
#else
#ifdef _LP64
	fmt += L"*l";
#else
	fmt += L"*ll";
#endif
#endif

	if( hexSmall ) fmt += L"x";
	else if( hexLarge ) fmt += L"X";
	else fmt += L"u";

	wstring buf;
	buf.resize(width+30);
#if _MSC_VER >= 1400 && !defined(__S3E__)
	// MSVC 8.0 / 2005 or newer
	swprintf_s(&buf[0], buf.size(), fmt.c_str(), width, value);
#else
	sprintf(&buf[0], fmt.c_str(), width, value);
#endif
	buf.resize(wcslen(&buf[0]));

	return buf;
}

// AngelScript signature:
// wstring formatFloat(double val, const wstring &in options, uint width, uint precision)
static wstring formatFloat(double value, const wstring &options, asUINT width, asUINT precision)
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
	sprintf(&buf[0], fmt.c_str(), width, precision, value);
#endif
	buf.resize(wcslen(&buf[0]));

	return buf;
}

// AngelScript signature:
// int64 parseInt(const wstring &in val, uint base = 10, uint &out byteCount = 0)
static asINT64 parseInt(const wstring &val, asUINT base, asUINT *byteCount)
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
	if( *end == L'-' )
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
// uint64 parseUInt(const wstring &in val, uint base = 10, uint &out byteCount = 0)
static asQWORD parseUInt(const wstring &val, asUINT base, asUINT *byteCount)
{
	// Only accept base 10 and 16
	if (base != 10 && base != 16)
	{
		if (byteCount) *byteCount = 0;
		return 0;
	}

	const wchar_t *end = &val[0];

	asQWORD res = 0;
	if (base == 10)
	{
		while (*end >= L'0' && *end <= L'9')
		{
			res *= 10;
			res += *end++ - L'0';
		}
	}
	else if (base == 16)
	{
		while ((*end >= L'0' && *end <= L'9') ||
			(*end >= L'a' && *end <= L'f') ||
			(*end >= L'A' && *end <= L'F'))
		{
			res *= 16;
			if (*end >= L'0' && *end <= L'9')
				res += *end++ - L'0';
			else if (*end >= L'a' && *end <= L'f')
				res += *end++ - L'a' + 10;
			else if (*end >= L'A' && *end <= L'F')
				res += *end++ - L'A' + 10;
		}
	}

	if (byteCount)
		*byteCount = asUINT(size_t(end - val.c_str()));

	return res;
}

// AngelScript signature:
// double parseFloat(const wstring &in val, uint &out byteCount = 0)
double parseFloat(const wstring &val, asUINT *byteCount)
{
	wchar_t *end;

	// WinCE doesn't have setlocale. Some quick testing on my current platform
	// still manages to parse the numbers such as "3.14" even if the decimal for the
	// locale is ",".
#if !defined(_WIN32_WCE) && !defined(ANDROID) && !defined(__psp2__)
	// Set the locale to C so that we are guaranteed to parse the float value correctly
	char *orig = setlocale(LC_NUMERIC, 0);
	setlocale(LC_NUMERIC, "C");
#endif

	double res = wcstod(val.c_str(), &end);

#if !defined(_WIN32_WCE) && !defined(ANDROID) && !defined(__psp2__)
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
		ret = str.substr(start, (size_t)(count < 0 ? wstring::npos : count));

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
	int r = 0;
	UNUSED_VAR(r);

	// Register the wstring type
#if AS_CAN_USE_CPP11
	// With C++11 it is possible to use asGetTypeTraits to automatically determine the correct flags to use
	r = engine->RegisterObjectType("wstring", sizeof(wstring), asOBJ_VALUE | asGetTypeTraits<wstring>()); assert( r >= 0 );
#else
	r = engine->RegisterObjectType("wstring", sizeof(wstring), asOBJ_VALUE | asOBJ_APP_CLASS_CDAK); assert( r >= 0 );
#endif

	r = engine->RegisterStringFactory("wstring", &wstringFactory);

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

	r = engine->RegisterObjectMethod("wstring", "wstring &opAssign(float)", asFUNCTION(AssignFloatToWString), asCALL_CDECL_OBJLAST); assert( r >= 0 );
	r = engine->RegisterObjectMethod("wstring", "wstring &opAddAssign(float)", asFUNCTION(AddAssignFloatToWString), asCALL_CDECL_OBJLAST); assert( r >= 0 );
	r = engine->RegisterObjectMethod("wstring", "wstring opAdd(float) const", asFUNCTION(AddWStringFloat), asCALL_CDECL_OBJFIRST); assert( r >= 0 );
	r = engine->RegisterObjectMethod("wstring", "wstring opAdd_r(float) const", asFUNCTION(AddFloatWString), asCALL_CDECL_OBJLAST); assert( r >= 0 );

	r = engine->RegisterObjectMethod("wstring", "wstring &opAssign(int64)", asFUNCTION(AssignInt64ToWString), asCALL_CDECL_OBJLAST); assert( r >= 0 );
	r = engine->RegisterObjectMethod("wstring", "wstring &opAddAssign(int64)", asFUNCTION(AddAssignInt64ToWString), asCALL_CDECL_OBJLAST); assert( r >= 0 );
	r = engine->RegisterObjectMethod("wstring", "wstring opAdd(int64) const", asFUNCTION(AddWStringInt64), asCALL_CDECL_OBJFIRST); assert( r >= 0 );
	r = engine->RegisterObjectMethod("wstring", "wstring opAdd_r(int64) const", asFUNCTION(AddInt64WString), asCALL_CDECL_OBJLAST); assert( r >= 0 );

	r = engine->RegisterObjectMethod("wstring", "wstring &opAssign(uint64)", asFUNCTION(AssignUInt64ToWString), asCALL_CDECL_OBJLAST); assert( r >= 0 );
	r = engine->RegisterObjectMethod("wstring", "wstring &opAddAssign(uint64)", asFUNCTION(AddAssignUInt64ToWString), asCALL_CDECL_OBJLAST); assert( r >= 0 );
	r = engine->RegisterObjectMethod("wstring", "wstring opAdd(uint64) const", asFUNCTION(AddWStringUInt64), asCALL_CDECL_OBJFIRST); assert( r >= 0 );
	r = engine->RegisterObjectMethod("wstring", "wstring opAdd_r(uint64) const", asFUNCTION(AddUInt64WString), asCALL_CDECL_OBJLAST); assert( r >= 0 );

	r = engine->RegisterObjectMethod("wstring", "wstring &opAssign(bool)", asFUNCTION(AssignBoolToWString), asCALL_CDECL_OBJLAST); assert( r >= 0 );
	r = engine->RegisterObjectMethod("wstring", "wstring &opAddAssign(bool)", asFUNCTION(AddAssignBoolToWString), asCALL_CDECL_OBJLAST); assert( r >= 0 );
	r = engine->RegisterObjectMethod("wstring", "wstring opAdd(bool) const", asFUNCTION(AddWStringBool), asCALL_CDECL_OBJFIRST); assert( r >= 0 );
	r = engine->RegisterObjectMethod("wstring", "wstring opAdd_r(bool) const", asFUNCTION(AddBoolWString), asCALL_CDECL_OBJLAST); assert( r >= 0 );

	// Utilities
	r = engine->RegisterObjectMethod("wstring", "wstring substr(uint start = 0, int count = -1) const", asFUNCTION(WStringSubWString), asCALL_CDECL_OBJLAST); assert( r >= 0 );
	r = engine->RegisterObjectMethod("wstring", "int findFirst(const wstring &in, uint start = 0) const", asFUNCTION(WStringFindFirst), asCALL_CDECL_OBJLAST); assert( r >= 0 );
	r = engine->RegisterObjectMethod("wstring", "int findFirstOf(const wstring &in, uint start = 0) const", asFUNCTION(WStringFindFirstOf), asCALL_CDECL_OBJLAST); assert(r >= 0);
	r = engine->RegisterObjectMethod("wstring", "int findFirstNotOf(const wstring &in, uint start = 0) const", asFUNCTION(WStringFindFirstNotOf), asCALL_CDECL_OBJLAST); assert(r >= 0);
	r = engine->RegisterObjectMethod("wstring", "int findLast(const wstring &in, int start = -1) const", asFUNCTION(WStringFindLast), asCALL_CDECL_OBJLAST); assert( r >= 0 );
	r = engine->RegisterObjectMethod("wstring", "int findLastOf(const wstring &in, int start = -1) const", asFUNCTION(WStringFindLastOf), asCALL_CDECL_OBJLAST); assert(r >= 0);
	r = engine->RegisterObjectMethod("wstring", "int findLastNotOf(const wstring &in, int start = -1) const", asFUNCTION(WStringFindLastNotOf), asCALL_CDECL_OBJLAST); assert(r >= 0);
	r = engine->RegisterObjectMethod("wstring", "void insert(uint pos, const wstring &in other)", asFUNCTION(WStringInsert), asCALL_CDECL_OBJLAST); assert(r >= 0);
	r = engine->RegisterObjectMethod("wstring", "void erase(uint pos, int count = -1)", asFUNCTION(WStringErase), asCALL_CDECL_OBJLAST); assert(r >= 0);


	r = engine->RegisterGlobalFunction("wstring formatInt(int64 val, const wstring &in options = \"\", uint width = 0)", asFUNCTION(formatInt), asCALL_CDECL); assert(r >= 0);
	r = engine->RegisterGlobalFunction("wstring formatUInt(uint64 val, const wstring &in options = \"\", uint width = 0)", asFUNCTION(formatUInt), asCALL_CDECL); assert(r >= 0);
	r = engine->RegisterGlobalFunction("wstring formatFloat(double val, const wstring &in options = \"\", uint width = 0, uint precision = 0)", asFUNCTION(formatFloat), asCALL_CDECL); assert(r >= 0);
	r = engine->RegisterGlobalFunction("int64 parseInt(const wstring &in, uint base = 10, uint &out byteCount = 0)", asFUNCTION(parseInt), asCALL_CDECL); assert(r >= 0);
	r = engine->RegisterGlobalFunction("uint64 parseUInt(const wstring &in, uint base = 10, uint &out byteCount = 0)", asFUNCTION(parseUInt), asCALL_CDECL); assert(r >= 0);
	r = engine->RegisterGlobalFunction("double parseFloat(const wstring &in, uint &out byteCount = 0)", asFUNCTION(parseFloat), asCALL_CDECL); assert(r >= 0);

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
	// findAndReplace - replaces a text found in the wstring
	// replaceRange - replaces a range of bytes in the wstring
	// multiply/times/opMul/opMul_r - takes the wstring and multiplies it n times, e.g. "-".multiply(5) returns "-----"
}

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

static void WStringInsert_Generic(asIScriptGeneric *gen)
{
	wstring * self = static_cast<wstring *>(gen->GetObject());
	asUINT pos = gen->GetArgDWord(0);
	wstring *other = reinterpret_cast<wstring*>(gen->GetArgAddress(1));
	WStringInsert(pos, *other, *self);
}

static void WStringErase_Generic(asIScriptGeneric *gen)
{
	wstring * self = static_cast<wstring *>(gen->GetObject());
	asUINT pos = gen->GetArgDWord(0);
	int count = int(gen->GetArgDWord(1));
	WStringErase(pos, count, *self);
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

static void WStringFindFirstOf_Generic(asIScriptGeneric * gen)
{
	wstring *find = reinterpret_cast<wstring*>(gen->GetArgAddress(0));
	asUINT start = gen->GetArgDWord(1);
	wstring *self = reinterpret_cast<wstring *>(gen->GetObject());
	*reinterpret_cast<int *>(gen->GetAddressOfReturnLocation()) = WStringFindFirstOf(*find, start, *self);
}

static void WStringFindLastOf_Generic(asIScriptGeneric * gen)
{
	wstring *find = reinterpret_cast<wstring*>(gen->GetArgAddress(0));
	asUINT start = gen->GetArgDWord(1);
	wstring *self = reinterpret_cast<wstring *>(gen->GetObject());
	*reinterpret_cast<int *>(gen->GetAddressOfReturnLocation()) = WStringFindLastOf(*find, start, *self);
}

static void WStringFindFirstNotOf_Generic(asIScriptGeneric * gen)
{
	wstring *find = reinterpret_cast<wstring*>(gen->GetArgAddress(0));
	asUINT start = gen->GetArgDWord(1);
	wstring *self = reinterpret_cast<wstring *>(gen->GetObject());
	*reinterpret_cast<int *>(gen->GetAddressOfReturnLocation()) = WStringFindFirstNotOf(*find, start, *self);
}

static void WStringFindLastNotOf_Generic(asIScriptGeneric * gen)
{
	wstring *find = reinterpret_cast<wstring*>(gen->GetArgAddress(0));
	asUINT start = gen->GetArgDWord(1);
	wstring *self = reinterpret_cast<wstring *>(gen->GetObject());
	*reinterpret_cast<int *>(gen->GetAddressOfReturnLocation()) = WStringFindLastNotOf(*find, start, *self);
}

static void formatInt_Generic(asIScriptGeneric * gen)
{
	asINT64 val = gen->GetArgQWord(0);
	wstring *options = reinterpret_cast<wstring*>(gen->GetArgAddress(1));
	asUINT width = gen->GetArgDWord(2);
	new(gen->GetAddressOfReturnLocation()) wstring(formatInt(val, *options, width));
}

static void formatUInt_Generic(asIScriptGeneric * gen)
{
	asQWORD val = gen->GetArgQWord(0);
	wstring *options = reinterpret_cast<wstring*>(gen->GetArgAddress(1));
	asUINT width = gen->GetArgDWord(2);
	new(gen->GetAddressOfReturnLocation()) wstring(formatUInt(val, *options, width));
}

static void formatFloat_Generic(asIScriptGeneric *gen)
{
	double val = gen->GetArgDouble(0);
	wstring *options = reinterpret_cast<wstring*>(gen->GetArgAddress(1));
	asUINT width = gen->GetArgDWord(2);
	asUINT precision = gen->GetArgDWord(3);
	new(gen->GetAddressOfReturnLocation()) wstring(formatFloat(val, *options, width, precision));
}

static void parseInt_Generic(asIScriptGeneric *gen)
{
	wstring *str = reinterpret_cast<wstring*>(gen->GetArgAddress(0));
	asUINT base = gen->GetArgDWord(1);
	asUINT *byteCount = reinterpret_cast<asUINT*>(gen->GetArgAddress(2));
	gen->SetReturnQWord(parseInt(*str,base,byteCount));
}

static void parseUInt_Generic(asIScriptGeneric *gen)
{
	wstring *str = reinterpret_cast<wstring*>(gen->GetArgAddress(0));
	asUINT base = gen->GetArgDWord(1);
	asUINT *byteCount = reinterpret_cast<asUINT*>(gen->GetArgAddress(2));
	gen->SetReturnQWord(parseUInt(*str, base, byteCount));
}

static void parseFloat_Generic(asIScriptGeneric *gen)
{
	wstring *str = reinterpret_cast<wstring*>(gen->GetArgAddress(0));
	asUINT *byteCount = reinterpret_cast<asUINT*>(gen->GetArgAddress(1));
	gen->SetReturnDouble(parseFloat(*str,byteCount));
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
	asINT64 *a = static_cast<asINT64*>(gen->GetAddressOfArg(0));
	wstring *self = static_cast<wstring*>(gen->GetObject());
	std::wstringstream sstr;
	sstr << *a;
	*self = sstr.str();
	gen->SetReturnAddress(self);
}

static void AssignUInt2WStringGeneric(asIScriptGeneric *gen)
{
	asQWORD *a = static_cast<asQWORD*>(gen->GetAddressOfArg(0));
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

static void AssignFloat2WStringGeneric(asIScriptGeneric *gen)
{
	float *a = static_cast<float*>(gen->GetAddressOfArg(0));
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

static void AddAssignFloat2WStringGeneric(asIScriptGeneric * gen)
{
	float * a = static_cast<float *>(gen->GetAddressOfArg(0));
	wstring * self = static_cast<wstring *>(gen->GetObject());
	std::wstringstream sstr;
	sstr << *a;
	*self += sstr.str();
	gen->SetReturnAddress(self);
}

static void AddAssignInt2WStringGeneric(asIScriptGeneric * gen)
{
	asINT64 * a = static_cast<asINT64 *>(gen->GetAddressOfArg(0));
	wstring * self = static_cast<wstring *>(gen->GetObject());
	std::wstringstream sstr;
	sstr << *a;
	*self += sstr.str();
	gen->SetReturnAddress(self);
}

static void AddAssignUInt2WStringGeneric(asIScriptGeneric * gen)
{
	asQWORD * a = static_cast<asQWORD *>(gen->GetAddressOfArg(0));
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

static void AddWString2FloatGeneric(asIScriptGeneric * gen)
{
	wstring * a = static_cast<wstring *>(gen->GetObject());
	float * b = static_cast<float *>(gen->GetAddressOfArg(0));
	std::wstringstream sstr;
	sstr << *a << *b;
	std::wstring ret_val = sstr.str();
	gen->SetReturnObject(&ret_val);
}

static void AddWString2IntGeneric(asIScriptGeneric * gen)
{
	wstring * a = static_cast<wstring *>(gen->GetObject());
	asINT64 * b = static_cast<asINT64 *>(gen->GetAddressOfArg(0));
	std::wstringstream sstr;
	sstr << *a << *b;
	std::wstring ret_val = sstr.str();
	gen->SetReturnObject(&ret_val);
}

static void AddWString2UIntGeneric(asIScriptGeneric * gen)
{
	wstring * a = static_cast<wstring *>(gen->GetObject());
	asQWORD * b = static_cast<asQWORD *>(gen->GetAddressOfArg(0));
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

static void AddFloat2WStringGeneric(asIScriptGeneric * gen)
{
	float* a = static_cast<float *>(gen->GetAddressOfArg(0));
	wstring * b = static_cast<wstring *>(gen->GetObject());
	std::wstringstream sstr;
	sstr << *a << *b;
	std::wstring ret_val = sstr.str();
	gen->SetReturnObject(&ret_val);
}

static void AddInt2WStringGeneric(asIScriptGeneric * gen)
{
	asINT64* a = static_cast<asINT64 *>(gen->GetAddressOfArg(0));
	wstring * b = static_cast<wstring *>(gen->GetObject());
	std::wstringstream sstr;
	sstr << *a << *b;
	std::wstring ret_val = sstr.str();
	gen->SetReturnObject(&ret_val);
}

static void AddUInt2WStringGeneric(asIScriptGeneric * gen)
{
	asQWORD* a = static_cast<asQWORD *>(gen->GetAddressOfArg(0));
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
	int r = 0;
	UNUSED_VAR(r);

	// Register the wstring type
	r = engine->RegisterObjectType("wstring", sizeof(wstring), asOBJ_VALUE | asOBJ_APP_CLASS_CDAK); assert( r >= 0 );

	r = engine->RegisterStringFactory("wstring", &wstringFactory);

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

	r = engine->RegisterObjectMethod("wstring", "wstring &opAssign(float)", asFUNCTION(AssignFloat2WStringGeneric), asCALL_GENERIC); assert( r >= 0 );
	r = engine->RegisterObjectMethod("wstring", "wstring &opAddAssign(float)", asFUNCTION(AddAssignFloat2WStringGeneric), asCALL_GENERIC); assert( r >= 0 );
	r = engine->RegisterObjectMethod("wstring", "wstring opAdd(float) const", asFUNCTION(AddWString2FloatGeneric), asCALL_GENERIC); assert( r >= 0 );
	r = engine->RegisterObjectMethod("wstring", "wstring opAdd_r(float) const", asFUNCTION(AddFloat2WStringGeneric), asCALL_GENERIC); assert( r >= 0 );

	r = engine->RegisterObjectMethod("wstring", "wstring &opAssign(int64)", asFUNCTION(AssignInt2WStringGeneric), asCALL_GENERIC); assert( r >= 0 );
	r = engine->RegisterObjectMethod("wstring", "wstring &opAddAssign(int64)", asFUNCTION(AddAssignInt2WStringGeneric), asCALL_GENERIC); assert( r >= 0 );
	r = engine->RegisterObjectMethod("wstring", "wstring opAdd(int64) const", asFUNCTION(AddWString2IntGeneric), asCALL_GENERIC); assert( r >= 0 );
	r = engine->RegisterObjectMethod("wstring", "wstring opAdd_r(int64) const", asFUNCTION(AddInt2WStringGeneric), asCALL_GENERIC); assert( r >= 0 );

	r = engine->RegisterObjectMethod("wstring", "wstring &opAssign(uint64)", asFUNCTION(AssignUInt2WStringGeneric), asCALL_GENERIC); assert( r >= 0 );
	r = engine->RegisterObjectMethod("wstring", "wstring &opAddAssign(uint64)", asFUNCTION(AddAssignUInt2WStringGeneric), asCALL_GENERIC); assert( r >= 0 );
	r = engine->RegisterObjectMethod("wstring", "wstring opAdd(uint64) const", asFUNCTION(AddWString2UIntGeneric), asCALL_GENERIC); assert( r >= 0 );
	r = engine->RegisterObjectMethod("wstring", "wstring opAdd_r(uint64) const", asFUNCTION(AddUInt2WStringGeneric), asCALL_GENERIC); assert( r >= 0 );

	r = engine->RegisterObjectMethod("wstring", "wstring &opAssign(bool)", asFUNCTION(AssignBool2WStringGeneric), asCALL_GENERIC); assert( r >= 0 );
	r = engine->RegisterObjectMethod("wstring", "wstring &opAddAssign(bool)", asFUNCTION(AddAssignBool2WStringGeneric), asCALL_GENERIC); assert( r >= 0 );
	r = engine->RegisterObjectMethod("wstring", "wstring opAdd(bool) const", asFUNCTION(AddWString2BoolGeneric), asCALL_GENERIC); assert( r >= 0 );
	r = engine->RegisterObjectMethod("wstring", "wstring opAdd_r(bool) const", asFUNCTION(AddBool2WStringGeneric), asCALL_GENERIC); assert( r >= 0 );

	r = engine->RegisterObjectMethod("wstring", "wstring substr(uint start = 0, int count = -1) const", asFUNCTION(WStringSubWString_Generic), asCALL_GENERIC); assert(r >= 0);
	r = engine->RegisterObjectMethod("wstring", "int findFirst(const wstring &in, uint start = 0) const", asFUNCTION(WStringFindFirst_Generic), asCALL_GENERIC); assert(r >= 0);
	r = engine->RegisterObjectMethod("wstring", "int findFirstOf(const wstring &in, uint start = 0) const", asFUNCTION(WStringFindFirstOf_Generic), asCALL_GENERIC); assert(r >= 0);
	r = engine->RegisterObjectMethod("wstring", "int findFirstNotOf(const wstring &in, uint start = 0) const", asFUNCTION(WStringFindFirstNotOf_Generic), asCALL_GENERIC); assert(r >= 0);
	r = engine->RegisterObjectMethod("wstring", "int findLast(const wstring &in, int start = -1) const", asFUNCTION(WStringFindLast_Generic), asCALL_GENERIC); assert(r >= 0);
	r = engine->RegisterObjectMethod("wstring", "int findLastOf(const wstring &in, int start = -1) const", asFUNCTION(WStringFindLastOf_Generic), asCALL_GENERIC); assert(r >= 0);
	r = engine->RegisterObjectMethod("wstring", "int findLastNotOf(const wstring &in, int start = -1) const", asFUNCTION(WStringFindLastNotOf_Generic), asCALL_GENERIC); assert(r >= 0);
	r = engine->RegisterObjectMethod("wstring", "void insert(uint pos, const wstring &in other)", asFUNCTION(WStringInsert_Generic), asCALL_GENERIC); assert(r >= 0);
	r = engine->RegisterObjectMethod("wstring", "void erase(uint pos, int count = -1)", asFUNCTION(WStringErase_Generic), asCALL_GENERIC); assert(r >= 0);


	r = engine->RegisterGlobalFunction("wstring formatInt(int64 val, const wstring &in options = \"\", uint width = 0)", asFUNCTION(formatInt_Generic), asCALL_GENERIC); assert(r >= 0);
	r = engine->RegisterGlobalFunction("wstring formatUInt(uint64 val, const wstring &in options = \"\", uint width = 0)", asFUNCTION(formatUInt_Generic), asCALL_GENERIC); assert(r >= 0);
	r = engine->RegisterGlobalFunction("wstring formatFloat(double val, const wstring &in options = \"\", uint width = 0, uint precision = 0)", asFUNCTION(formatFloat_Generic), asCALL_GENERIC); assert(r >= 0);
	r = engine->RegisterGlobalFunction("int64 parseInt(const wstring &in, uint base = 10, uint &out byteCount = 0)", asFUNCTION(parseInt_Generic), asCALL_GENERIC); assert(r >= 0);
	r = engine->RegisterGlobalFunction("uint64 parseUInt(const wstring &in, uint base = 10, uint &out byteCount = 0)", asFUNCTION(parseUInt_Generic), asCALL_GENERIC); assert(r >= 0);
	r = engine->RegisterGlobalFunction("double parseFloat(const wstring &in, uint &out byteCount = 0)", asFUNCTION(parseFloat_Generic), asCALL_GENERIC); assert(r >= 0);
}

void RegisterStdWString(asIScriptEngine * engine)
{
	if (strstr(asGetLibraryOptions(), "AS_MAX_PORTABILITY"))
		RegisterStdWString_Generic(engine);
	else
		RegisterStdWString_Native(engine);
}

END_AS_NAMESPACE




