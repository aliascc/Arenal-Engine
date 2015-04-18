//
// Script std::wstring
//
// This function registers the std::wstring type with AngelScript to be used as the default wstring type.
//
// The wstring type is registered as a value type, thus may have performance issues if a lot of 
// wstring operations are performed in the script. However, for relatively few operations, this should
// not cause any problem for most applications.
//

#ifndef SCRIPTSTDWSTRING_H
#define SCRIPTSTDWSTRING_H

#ifndef ANGELSCRIPT_H 
// Avoid having to inform include path if header is already include before
#include <angelscript.h>
#endif

#include <string>

//---------------------------
// Compilation settings
//

// The use of the wstring pool can improve performance quite drastically
// for scripts that work with a lot of literal wstring constants. 
//
//  1 = on
//  0 = off

#ifndef AS_USE_WSTRINGPOOL
#define AS_USE_WSTRINGPOOL 1
#endif

// Sometimes it may be desired to use the same method names as used by C++ STL.
// This may for example reduce time when converting code from script to C++ or
// back.
//
//  0 = off
//  1 = on

#ifndef AS_USE_STLNAMES
#define AS_USE_STLNAMES 0
#endif

BEGIN_AS_NAMESPACE

void RegisterStdWString(asIScriptEngine *engine);
void RegisterStdWStringUtils(asIScriptEngine *engine);

END_AS_NAMESPACE

#endif
