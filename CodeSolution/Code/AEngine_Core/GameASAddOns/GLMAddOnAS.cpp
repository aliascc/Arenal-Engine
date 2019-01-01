/*
* Copyright (c) 2018 <Carlos Chacón>
* All rights reserved
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
* http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*/

/*************************
*   Precompiled Header   *
**************************/
#include "precomp_core.h"

/**********************
*   System Includes   *
***********************/
#include <new>

/*************************
*   3rd Party Includes   *
**************************/

/***************************
*   Game Engine Includes   *
****************************/
#include "GLMAddOnAS.h"
#include "GameASAddOnsDefs.h"

/********************
*   Function Defs   *
*********************/

/***************
*   Vector 3   *
****************/
#pragma region Vector 3

static void GLMVec3_Constructor(glm::vec3* self)
{
    new(self)glm::vec3();
}

static void GLMVec3_Constructor(const glm::vec3& other, glm::vec3* self)
{
    new(self)glm::vec3(other);
}

static void GLMVec3_Constructor(float x, float y, float z, glm::vec3* self)
{
    new(self)glm::vec3(x, y, z);
}

static void GLMVec3_Constructor(const glm::vec4& other, glm::vec3* self)
{
    new(self)glm::vec3(other);
}

#pragma endregion

/***************
*   Vector 4   *
****************/
#pragma region Vector 4

static void GLMVec4_Constructor(glm::vec4* self)
{
    new(self)glm::vec4();
}

static void GLMVec4_Constructor(const glm::vec4& other, glm::vec4* self)
{
    new(self)glm::vec4(other);
}

static void GLMVec4_Constructor(const glm::vec3& vector3, float w, glm::vec4* self)
{
    new(self)glm::vec4(vector3, w);
}

static void GLMVec4_Constructor(float x, float y, float z, float w, glm::vec4* self)
{
    new(self)glm::vec4(x, y, z, w);
}

static glm::vec3 GLMVec4_Cast(const glm::vec4& self)
{
    return (glm::vec3)self;
}

#pragma endregion

/*****************
*   Matrix 4x4   *
******************/
#pragma region Matrix4x4

static void GLMMat4_Constructor(glm::mat4* self)
{
    new(self)glm::mat4();
}

static void GLMMat4_Constructor(const glm::mat4& other, glm::mat4* self)
{
    new(self)glm::mat4(other);
}

#pragma endregion

/*****************
*   GLMAddOnAS   *
******************/
GLMAddOnAS::GLMAddOnAS()
    : AngelScriptAddOn((uint32_t)XEGameASAddOnsID::GLM)
{
}

GLMAddOnAS::~GLMAddOnAS()
{
}

AEResult GLMAddOnAS::Register(asIScriptEngine* engine)
{
    AEAssert(engine != nullptr);
    if (engine == nullptr)
    {
        return AEResult::NullParameter;
    }

    if (strstr(asGetLibraryOptions(), "AS_MAX_PORTABILITY"))
    {
        AEAssert(false);
        // TODO: implement support for generic calling convention

        return AEResult::Fail;
    }
    else
    {
        return RegisterGLM_Native(engine);
    }
}

AEResult GLMAddOnAS::RegisterGLM_Native(asIScriptEngine* engine)
{
    AEResult ret = AEResult::Ok;

    /////////////////////////////////////////////
    //Register GLM Objects
    ret = RegisterGLM_Objects_Native(engine);
    if (ret != AEResult::Ok)
    {
        return ret;
    }

    /////////////////////////////////////////////
    //Register Vector 3 Functions
    ret = RegisterGLM_Vec3_Functions_Native(engine);
    if (ret != AEResult::Ok)
    {
        return ret;
    }

    /////////////////////////////////////////////
    //Register Vector 4 Functions
    ret = RegisterGLM_Vec4_Functions_Native(engine);
    if (ret != AEResult::Ok)
    {
        return ret;
    }

    /////////////////////////////////////////////
    //Register Matrix 4x4 Functions
    ret = RegisterGLM_Mat4_Functions_Native(engine);
    if (ret != AEResult::Ok)
    {
        return ret;
    }

    /////////////////////////////////////////////
    //Finish
    return AEResult::Ok;
}

AEResult GLMAddOnAS::RegisterGLM_Objects_Native(asIScriptEngine* engine)
{
    int ret = 0;

    ////////////////////////////////////////////////
    //Register the types

    //Vector 3
    ret = engine->RegisterObjectType("vec3", sizeof(glm::vec3), asOBJ_VALUE | asOBJ_POD | asOBJ_APP_CLASS_CAK | asOBJ_APP_CLASS_ALLFLOATS);
    if (ret < 0)
    {
        return AEResult::RegObjTypeFail;
    }

    //Vector 4
    ret = engine->RegisterObjectType("vec4", sizeof(glm::vec4), asOBJ_VALUE | asOBJ_POD | asOBJ_APP_CLASS_CAK | asOBJ_APP_CLASS_ALLFLOATS);
    if (ret < 0)
    {
        return AEResult::RegObjTypeFail;
    }

    //Matrix 4x4
    ret = engine->RegisterObjectType("mat4", sizeof(glm::mat4), asOBJ_VALUE | asOBJ_POD | asOBJ_APP_CLASS_CAK | asOBJ_APP_CLASS_ALLFLOATS);
    if (ret < 0)
    {
        return AEResult::RegObjTypeFail;
    }

    return AEResult::Ok;
}

AEResult GLMAddOnAS::RegisterGLM_Vec3_Functions_Native(asIScriptEngine* engine)
{
    int ret = 0;

    ////////////////////////////////////////////////
    //Register the object properties
    ret = engine->RegisterObjectProperty("vec3", "float x", asOFFSET(glm::vec3, x));
    if (ret < 0)
    {
        return AEResult::RegObjPropFail;
    }

    ret = engine->RegisterObjectProperty("vec3", "float y", asOFFSET(glm::vec3, y));
    if (ret < 0)
    {
        return AEResult::RegObjPropFail;
    }

    ret = engine->RegisterObjectProperty("vec3", "float z", asOFFSET(glm::vec3, z));
    if (ret < 0)
    {
        return AEResult::RegObjPropFail;
    }

    ret = engine->RegisterObjectProperty("vec3", "float r", asOFFSET(glm::vec3, r));
    if (ret < 0)
    {
        return AEResult::RegObjPropFail;
    }

    ret = engine->RegisterObjectProperty("vec3", "float g", asOFFSET(glm::vec3, g));
    if (ret < 0)
    {
        return AEResult::RegObjPropFail;
    }

    ret = engine->RegisterObjectProperty("vec3", "float b", asOFFSET(glm::vec3, b));
    if (ret < 0)
    {
        return AEResult::RegObjPropFail;
    }

    ////////////////////////////////////////////////
    //Register the Constructors
    ret = engine->RegisterObjectBehaviour("vec3", asBEHAVE_CONSTRUCT, "void f()", asFUNCTIONPR(GLMVec3_Constructor, (glm::vec3*), void), asCALL_CDECL_OBJLAST);
    if (ret < 0)
    {
        return AEResult::RegObjBehaviorFail;
    }

    ret = engine->RegisterObjectBehaviour("vec3", asBEHAVE_CONSTRUCT, "void f(const vec3&in)", asFUNCTIONPR(GLMVec3_Constructor, (const glm::vec3&, glm::vec3*), void), asCALL_CDECL_OBJLAST);
    if (ret < 0)
    {
        return AEResult::RegObjBehaviorFail;
    }

    ret = engine->RegisterObjectBehaviour("vec3", asBEHAVE_CONSTRUCT, "void f(float, float, float)", asFUNCTIONPR(GLMVec3_Constructor, (float, float, float, glm::vec3*), void), asCALL_CDECL_OBJLAST);
    if (ret < 0)
    {
        return AEResult::RegObjBehaviorFail;
    }

    ret = engine->RegisterObjectBehaviour("vec3", asBEHAVE_CONSTRUCT, "void f(const vec4&in)", asFUNCTIONPR(GLMVec3_Constructor, (const glm::vec4&, glm::vec3*), void), asCALL_CDECL_OBJLAST);
    if (ret < 0)
    {
        return AEResult::RegObjBehaviorFail;
    }

    ////////////////////////////////////////////////
    // Register the operator overloads
    ret = engine->RegisterObjectMethod("vec3", "vec3& opAddAssign(const vec3&in)", asMETHODPR(glm::vec3, operator+=, (const glm::vec3&), glm::vec3&), asCALL_THISCALL);
    if (ret < 0)
    {
        return AEResult::RegObjBehaviorFail;
    }

    ret = engine->RegisterObjectMethod("vec3", "vec3& opSubAssign(const vec3&in)", asMETHODPR(glm::vec3, operator-=, (const glm::vec3&), glm::vec3&), asCALL_THISCALL);
    if (ret < 0)
    {
        return AEResult::RegObjBehaviorFail;
    }

    ret = engine->RegisterObjectMethod("vec3", "vec3& opMulAssign(const vec3&in)", asMETHODPR(glm::vec3, operator*=, (const glm::vec3&), glm::vec3&), asCALL_THISCALL);
    if (ret < 0)
    {
        return AEResult::RegObjBehaviorFail;
    }

    ret = engine->RegisterObjectMethod("vec3", "vec3& opMulAssign(const float&in)", asMETHODPR(glm::vec3, operator*=, (const float&), glm::vec3&), asCALL_THISCALL);
    if (ret < 0)
    {
        return AEResult::RegObjBehaviorFail;
    }

    ret = engine->RegisterObjectMethod("vec3", "vec3& opDivAssign(const vec3&in)", asMETHODPR(glm::vec3, operator/=, (const glm::vec3&), glm::vec3&), asCALL_THISCALL);
    if (ret < 0)
    {
        return AEResult::RegObjBehaviorFail;
    }

    ret = engine->RegisterObjectMethod("vec3", "vec3 opAdd(const vec3&in)", asFUNCTIONPR(glm::operator+, (const glm::vec3&, const glm::vec3&), glm::vec3), asCALL_CDECL_OBJFIRST);
    if (ret < 0)
    {
        return AEResult::RegObjBehaviorFail;
    }

    ret = engine->RegisterObjectMethod("vec3", "vec3 opSub(const vec3&in)", asFUNCTIONPR(glm::operator-, (const glm::vec3&, const glm::vec3&), glm::vec3), asCALL_CDECL_OBJFIRST);
    if (ret < 0)
    {
        return AEResult::RegObjBehaviorFail;
    }

    ret = engine->RegisterObjectMethod("vec3", "vec3 opMul(const vec3&in)", asFUNCTIONPR(glm::operator*, (const glm::vec3&, const glm::vec3&), glm::vec3), asCALL_CDECL_OBJFIRST);
    if (ret < 0)
    {
        return AEResult::RegObjBehaviorFail;
    }

    ret = engine->RegisterObjectMethod("vec3", "vec3 opMul(const float&in)", asFUNCTIONPR(glm::operator*, (const glm::vec3&, const float), glm::vec3), asCALL_CDECL_OBJFIRST);
    if (ret < 0)
    {
        return AEResult::RegObjBehaviorFail;
    }

    ret = engine->RegisterObjectMethod("vec3", "vec3 opDiv(const vec3&in)", asFUNCTIONPR(glm::operator/, (const glm::vec3&, const glm::vec3&), glm::vec3), asCALL_CDECL_OBJFIRST);
    if (ret < 0)
    {
        return AEResult::RegObjBehaviorFail;
    }

    ret = engine->RegisterObjectMethod("vec3", "bool opEquals(const vec3&in)", asFUNCTIONPR(glm::operator==, (const glm::vec3&, const glm::vec3&), bool), asCALL_CDECL_OBJFIRST);
    if (ret < 0)
    {
        return AEResult::RegObjBehaviorFail;
    }

    ////////////////////////////////////////////////
    // Register the Helper Functions and Constants
    ret = engine->SetDefaultNamespace(AE_AS_MATH_HELPERS_NAMESPACE);
    if (ret < 0)
    {
        AETODO("Set Namespace error");
        return AEResult::Fail;
    }

    //Register Length Square Function
    ret = engine->RegisterGlobalFunction("float length2(const vec3&in)", asFUNCTIONPR(glm::length2, (const glm::vec3&), float), asCALL_CDECL);
    if (ret < 0)
    {
        return AEResult::RegObjBehaviorFail;
    }

    //Register Normalize Function
    ret = engine->RegisterGlobalFunction("vec3 normalize(const vec3&in)", asFUNCTIONPR(glm::normalize, (const glm::vec3&), glm::vec3), asCALL_CDECL);
    if (ret < 0)
    {
        return AEResult::RegObjBehaviorFail;
    }

    //Register Constants Variables/Properties
    ret = engine->RegisterGlobalProperty("const vec3 Vec3fZero", (void*)&AEMathHelpers::Vec3fZero);
    if (ret < 0)
    {
        return AEResult::RegGlobalPropFail;
    }

    //Remove Namespace
    ret = engine->SetDefaultNamespace("");
    if (ret < 0)
    {
        AETODO("Set Namespace error");
        return AEResult::Fail;
    }

    return AEResult::Ok;
}

AEResult GLMAddOnAS::RegisterGLM_Vec4_Functions_Native(asIScriptEngine* engine)
{
    int ret = 0;

    ////////////////////////////////////////////////
    //Register the object properties
    ret = engine->RegisterObjectProperty("vec4", "float x", asOFFSET(glm::vec4, x));
    if (ret < 0)
    {
        return AEResult::RegObjPropFail;
    }

    ret = engine->RegisterObjectProperty("vec4", "float y", asOFFSET(glm::vec4, y));
    if (ret < 0)
    {
        return AEResult::RegObjPropFail;
    }

    ret = engine->RegisterObjectProperty("vec4", "float z", asOFFSET(glm::vec4, z));
    if (ret < 0)
    {
        return AEResult::RegObjPropFail;
    }

    ret = engine->RegisterObjectProperty("vec4", "float w", asOFFSET(glm::vec4, w));
    if (ret < 0)
    {
        return AEResult::RegObjPropFail;
    }

    ret = engine->RegisterObjectProperty("vec4", "float r", asOFFSET(glm::vec4, r));
    if (ret < 0)
    {
        return AEResult::RegObjPropFail;
    }

    ret = engine->RegisterObjectProperty("vec4", "float g", asOFFSET(glm::vec4, g));
    if (ret < 0)
    {
        return AEResult::RegObjPropFail;
    }

    ret = engine->RegisterObjectProperty("vec4", "float b", asOFFSET(glm::vec4, b));
    if (ret < 0)
    {
        return AEResult::RegObjPropFail;
    }

    ret = engine->RegisterObjectProperty("vec4", "float a", asOFFSET(glm::vec4, a));
    if (ret < 0)
    {
        return AEResult::RegObjPropFail;
    }

    ////////////////////////////////////////////////
    //Register the Constructors
    ret = engine->RegisterObjectBehaviour("vec4", asBEHAVE_CONSTRUCT, "void f()", asFUNCTIONPR(GLMVec4_Constructor, (glm::vec4*), void), asCALL_CDECL_OBJLAST);
    if (ret < 0)
    {
        return AEResult::RegObjBehaviorFail;
    }

    ret = engine->RegisterObjectBehaviour("vec4", asBEHAVE_CONSTRUCT, "void f(const vec4&in)", asFUNCTIONPR(GLMVec4_Constructor, (const glm::vec4&, glm::vec4*), void), asCALL_CDECL_OBJLAST);
    if (ret < 0)
    {
        return AEResult::RegObjBehaviorFail;
    }

    ret = engine->RegisterObjectBehaviour("vec4", asBEHAVE_CONSTRUCT, "void f(const vec3&in, float)", asFUNCTIONPR(GLMVec4_Constructor, (const glm::vec3&, float, glm::vec4*), void), asCALL_CDECL_OBJLAST);
    if (ret < 0)
    {
        return AEResult::RegObjBehaviorFail;
    }

    ret = engine->RegisterObjectBehaviour("vec4", asBEHAVE_CONSTRUCT, "void f(float, float, float, float)", asFUNCTIONPR(GLMVec4_Constructor, (float, float, float, float, glm::vec4*), void), asCALL_CDECL_OBJLAST);
    if (ret < 0)
    {
        return AEResult::RegObjBehaviorFail;
    }

    ////////////////////////////////////////////////
    // Register the operator overloads
    ret = engine->RegisterObjectMethod("vec4", "vec4& opAddAssign(const vec4&in)", asMETHODPR(glm::vec4, operator+=, (const glm::vec4&), glm::vec4&), asCALL_THISCALL);
    if (ret < 0)
    {
        return AEResult::RegObjBehaviorFail;
    }

    ret = engine->RegisterObjectMethod("vec4", "vec4& opSubAssign(const vec4&in)", asMETHODPR(glm::vec4, operator-=, (const glm::vec4&), glm::vec4&), asCALL_THISCALL);
    if (ret < 0)
    {
        return AEResult::RegObjBehaviorFail;
    }

    ret = engine->RegisterObjectMethod("vec4", "vec4& opMulAssign(const vec4&in)", asMETHODPR(glm::vec4, operator*=, (const glm::vec4&), glm::vec4&), asCALL_THISCALL);
    if (ret < 0)
    {
        return AEResult::RegObjBehaviorFail;
    }

    ret = engine->RegisterObjectMethod("vec4", "vec4& opMulAssign(const float&in)", asMETHODPR(glm::vec4, operator*=, (const float&), glm::vec4&), asCALL_THISCALL);
    if (ret < 0)
    {
        return AEResult::RegObjBehaviorFail;
    }

    ret = engine->RegisterObjectMethod("vec4", "vec4& opDivAssign(const vec4&in)", asMETHODPR(glm::vec4, operator/=, (const glm::vec4&), glm::vec4&), asCALL_THISCALL);
    if (ret < 0)
    {
        return AEResult::RegObjBehaviorFail;
    }

    ret = engine->RegisterObjectMethod("vec4", "vec4 opAdd(const vec4&in)", asFUNCTIONPR(glm::operator+, (const glm::vec4&, const glm::vec4&), glm::vec4), asCALL_CDECL_OBJFIRST);
    if (ret < 0)
    {
        return AEResult::RegObjBehaviorFail;
    }

    ret = engine->RegisterObjectMethod("vec4", "vec4 opSub(const vec4&in)", asFUNCTIONPR(glm::operator-, (const glm::vec4&, const glm::vec4&), glm::vec4), asCALL_CDECL_OBJFIRST);
    if (ret < 0)
    {
        return AEResult::RegObjBehaviorFail;
    }

    ret = engine->RegisterObjectMethod("vec4", "vec4 opMul(const mat4&in)", asFUNCTIONPR(glm::operator*, (const glm::vec4&, const glm::mat4&), glm::vec4), asCALL_CDECL_OBJFIRST);
    if (ret < 0)
    {
        return AEResult::RegObjBehaviorFail;
    }

    ret = engine->RegisterObjectMethod("vec4", "vec4 opMul(const vec4&in)", asFUNCTIONPR(glm::operator*, (const glm::vec4&, const glm::vec4&), glm::vec4), asCALL_CDECL_OBJFIRST);
    if (ret < 0)
    {
        return AEResult::RegObjBehaviorFail;
    }

    AETODO("Fix this");
    /*ret = engine->RegisterObjectMethod("vec4", "vec4 opMul(const float&in)", asFUNCTIONPR(glm::operator*, (const glm::vec4&, const float), glm::vec4), asCALL_CDECL_OBJFIRST);
    if (ret < 0)
    {
        return AEResult::RegObjBehaviorFail;
    }*/

    ret = engine->RegisterObjectMethod("vec4", "vec4 opDiv(const vec4&in)", asFUNCTIONPR(glm::operator/, (const glm::vec4&, const glm::vec4&), glm::vec4), asCALL_CDECL_OBJFIRST);
    if (ret < 0)
    {
        return AEResult::RegObjBehaviorFail;
    }

    ret = engine->RegisterObjectMethod("vec4", "bool opEquals(const vec4&in)", asFUNCTIONPR(glm::operator==, (const glm::vec4&, const glm::vec4&), bool), asCALL_CDECL_OBJFIRST);
    if (ret < 0)
    {
        return AEResult::RegObjBehaviorFail;
    }

    ////////////////////////////////////////////////
    // Register Implicit Cast
    ret = engine->RegisterObjectMethod("vec4", "vec3 opImplConv() const", asFUNCTION(GLMVec4_Cast), asCALL_CDECL_OBJLAST);
    if (ret < 0)
    {
        return AEResult::RegObjBehaviorFail;
    }

    ////////////////////////////////////////////////
    // Register the Helper Functions and Constants
    ret = engine->SetDefaultNamespace(AE_AS_MATH_HELPERS_NAMESPACE);
    if (ret < 0)
    {
        AETODO("Set Namespace error");
        return AEResult::Fail;
    }

    //Register Length Square Function
    ret = engine->RegisterGlobalFunction("float length2(const vec4&in)", asFUNCTIONPR(glm::length2, (const glm::vec4&), float), asCALL_CDECL);
    if (ret < 0)
    {
        return AEResult::RegObjBehaviorFail;
    }

    //Register Normalize Function
    ret = engine->RegisterGlobalFunction("vec4 normalize(const vec4&in)", asFUNCTIONPR(glm::normalize, (const glm::vec4&), glm::vec4), asCALL_CDECL);
    if (ret < 0)
    {
        return AEResult::RegObjBehaviorFail;
    }

    //Remove Namespace
    ret = engine->SetDefaultNamespace("");
    if (ret < 0)
    {
        AETODO("Set Namespace error");
        return AEResult::Fail;
    }

    return AEResult::Ok;
}

AEResult GLMAddOnAS::RegisterGLM_Mat4_Functions_Native(asIScriptEngine* engine)
{
    int ret = 0;

    ////////////////////////////////////////////////
    //Register the object properties

    ////////////////////////////////////////////////
    //Register the Constructors
    ret = engine->RegisterObjectBehaviour("mat4", asBEHAVE_CONSTRUCT, "void f()", asFUNCTIONPR(GLMMat4_Constructor, (glm::mat4*), void), asCALL_CDECL_OBJLAST);
    if (ret < 0)
    {
        return AEResult::RegObjBehaviorFail;
    }

    ret = engine->RegisterObjectBehaviour("mat4", asBEHAVE_CONSTRUCT, "void f(const mat4&in)", asFUNCTIONPR(GLMMat4_Constructor, (const glm::mat4&, glm::mat4*), void), asCALL_CDECL_OBJLAST);
    if (ret < 0)
    {
        return AEResult::RegObjBehaviorFail;
    }

    ////////////////////////////////////////////////
    // Register the operator overloads
    ret = engine->RegisterObjectMethod("mat4", "vec4& opIndex(int)",   asMETHODPR(glm::mat4, operator[], (int), glm::vec4&), asCALL_THISCALL);
    if (ret < 0)
    {
        return AEResult::RegObjBehaviorFail;
    }

    ret = engine->RegisterObjectMethod("mat4", "mat4 opMul(const mat4&in)", asFUNCTIONPR(glm::operator*, (const glm::mat4&, const glm::mat4&), glm::mat4), asCALL_CDECL_OBJFIRST);
    if (ret < 0)
    {
        return AEResult::RegObjBehaviorFail;
    }

    ret = engine->RegisterObjectMethod("mat4", "vec4 opMul(const vec4&in)", asFUNCTIONPR(glm::operator*, (const glm::mat4&, const glm::vec4&), glm::vec4), asCALL_CDECL_OBJFIRST);
    if (ret < 0)
    {
        return AEResult::RegObjBehaviorFail;
    }

    ////////////////////////////////////////////////
    // Register the Helper Functions and Constants
    ret = engine->SetDefaultNamespace(AE_AS_MATH_HELPERS_NAMESPACE);
    if (ret < 0)
    {
        AETODO("Set Namespace error");
        return AEResult::Fail;
    }

    //Register Rotation on X Function
    ret = engine->RegisterGlobalFunction("mat4 CreateRotationX(float)", asFUNCTIONPR(AEMathHelpers::CreateRotationX, (float), glm::mat4), asCALL_CDECL);
    if (ret < 0)
    {
        return AEResult::RegObjBehaviorFail;
    }

    //Register Rotation on Y Function
    ret = engine->RegisterGlobalFunction("mat4 CreateRotationY(float)", asFUNCTIONPR(AEMathHelpers::CreateRotationY, (float), glm::mat4), asCALL_CDECL);
    if (ret < 0)
    {
        return AEResult::RegObjBehaviorFail;
    }

    //Register Rotation on Z Function
    ret = engine->RegisterGlobalFunction("mat4 CreateRotationZ(float)", asFUNCTIONPR(AEMathHelpers::CreateRotationZ, (float), glm::mat4), asCALL_CDECL);
    if (ret < 0)
    {
        return AEResult::RegObjBehaviorFail;
    }

    //Register Constants Variables/Properties
    ret = engine->RegisterGlobalProperty("const mat4 Mat4Identity", (void*)&AEMathHelpers::Mat4Identity);
    if (ret < 0)
    {
        return AEResult::RegGlobalPropFail;
    }

    //Remove Namespace
    ret = engine->SetDefaultNamespace("");
    if (ret < 0)
    {
        AETODO("Set Namespace error");
        return AEResult::Fail;
    }

    return AEResult::Ok;
}
