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

/**********************
*   System Includes   *
***********************/

/***************************
*   Game Engine Includes   *
****************************/
#include "GameCoreAddOnAS.h"
#include "GameASAddOnsDefs.h"
#include "Base\BaseFunctions.h"
#include "GameObject\GameObject.h"
#include "GameUtils\GameUtilsDefs.h"
#include "GameUtils\GameComponentCollection.h"
#include "GameObject\Components\AudioSourceGOC.h"
#include "GameObject\Scripts\GameObjectScriptDefs.h"
#include "GameObject\Scripts\GameObjectScriptManager.h"

//Always include last
#include "Memory\MemLeaks.h"

/********************
*   Function Defs   *
*********************/
GameCoreAddOnAS::GameCoreAddOnAS(GameComponentCollection* gameComponentCollection, GameObjectScriptManager* gameObjectScriptManager)
    : AngelScriptAddOn((uint32_t)XEGameASAddOnsID::GameCore)
    , m_GameComponentCollection(gameComponentCollection)
    , m_GameObjectScriptManager(gameObjectScriptManager)
{
    AEAssert(m_GameComponentCollection != nullptr);
    AEAssert(m_GameObjectScriptManager != nullptr);
}

GameCoreAddOnAS::~GameCoreAddOnAS()
{
}

AEResult GameCoreAddOnAS::Register(asIScriptEngine* engine)
{
    AEAssert(engine != nullptr);
    if (engine == nullptr)
    {
        return AEResult::NullParameter;
    }

    AEAssert(m_GameComponentCollection != nullptr);
    if (m_GameComponentCollection == nullptr)
    {
        AETODO("Add better return code");
        return AEResult::NullObj;
    }

    AEAssert(m_GameObjectScriptManager != nullptr);
    if (m_GameObjectScriptManager == nullptr)
    {
        AETODO("Add better return code");
        return AEResult::NullObj;
    }

    if (RegisterObjects(engine) != AEResult::Ok)
    {
        std::wstring errMsg = L"";
        AETODO("Add Log & Wrong Literal Change");
        //fmt::format(m_LocalizationManager->GetLiteral(L"AS_REG_BASIC_LOGGER_ERR_MSG"), __FUNCTIONW__);
        //m_Logger->AddNewLog(LogLevel::Error, errMsg);

        return AEResult::Fail;
    }

    if (RegisterGCBasicPropsObject(engine) != AEResult::Ok)
    {
        std::wstring errMsg = L"";
        AETODO("Add Log & Wrong Literal Change");
        //fmt::format(m_LocalizationManager->GetLiteral(L"AS_REG_BASIC_LOGGER_ERR_MSG"), __FUNCTIONW__);
        //m_Logger->AddNewLog(LogLevel::Error, errMsg);

        return AEResult::Fail;
    }

    if (RegisterGameComponentCollectionObject(engine) != AEResult::Ok)
    {
        std::wstring errMsg = L"";
        AETODO("Add Log & Wrong Literal Change");
        //fmt::format(m_LocalizationManager->GetLiteral(L"AS_REG_BASIC_LOGGER_ERR_MSG"), __FUNCTIONW__);
        //m_Logger->AddNewLog(LogLevel::Error, errMsg);

        return AEResult::Fail;
    }

    if (RegisterGameComponentCollectionObjectPointer(engine) != AEResult::Ok)
    {
        std::wstring errMsg = L"";
        AETODO("Add Log & Wrong Literal Change");
        //fmt::format(m_LocalizationManager->GetLiteral(L"AS_REG_BASIC_LOGGER_ERR_MSG"), __FUNCTIONW__);
        //m_Logger->AddNewLog(LogLevel::Error, errMsg);

        return AEResult::Fail;
    }

    if (RegisterGameObjectScriptInterface(engine) != AEResult::Ok)
    {
        std::wstring errMsg = L"";
        AETODO("Add Log & Wrong Literal Change");
        //fmt::format(m_LocalizationManager->GetLiteral(L"AS_REG_BASIC_LOGGER_ERR_MSG"), __FUNCTIONW__);
        //m_Logger->AddNewLog(LogLevel::Error, errMsg);

        return AEResult::Fail;
    }

    if (RegisterGameObjectScriptManagerObject(engine) != AEResult::Ok)
    {
        std::wstring errMsg = L"";
        AETODO("Add Log & Wrong Literal Change");
        //fmt::format(m_LocalizationManager->GetLiteral(L"AS_REG_BASIC_LOGGER_ERR_MSG"), __FUNCTIONW__);
        //m_Logger->AddNewLog(LogLevel::Error, errMsg);

        return AEResult::Fail;
    }

    if (RegisterGameObjectScriptManagerObjectPointer(engine) != AEResult::Ok)
    {
        std::wstring errMsg = L"";
        AETODO("Add Log & Wrong Literal Change");
        //fmt::format(m_LocalizationManager->GetLiteral(L"AS_REG_BASIC_LOGGER_ERR_MSG"), __FUNCTIONW__);
        //m_Logger->AddNewLog(LogLevel::Error, errMsg);

        return AEResult::Fail;
    }

    if (RegisterGameObjectObject(engine) != AEResult::Ok)
    {
        std::wstring errMsg = L"";
        AETODO("Add Log & Wrong Literal Change");
        //fmt::format(m_LocalizationManager->GetLiteral(L"AS_REG_BASIC_LOGGER_ERR_MSG"), __FUNCTIONW__);
        //m_Logger->AddNewLog(LogLevel::Error, errMsg);

        return AEResult::Fail;
    }

    if (RegisterAudioSourceGOCObject(engine) != AEResult::Ok)
    {
        std::wstring errMsg = L"";
        AETODO("Add Log & Wrong Literal Change");
        //fmt::format(m_LocalizationManager->GetLiteral(L"AS_REG_BASIC_LOGGER_ERR_MSG"), __FUNCTIONW__);
        //m_Logger->AddNewLog(LogLevel::Error, errMsg);

        return AEResult::Fail;
    }

    return AEResult::Ok;
}

AEResult GameCoreAddOnAS::RegisterObjects(asIScriptEngine* engine)
{
    int ret = 0;

    ///////////////////////////////
    //Register Objects
    ret = engine->RegisterObjectType("GCBasicProps", sizeof(GCBasicProps), asOBJ_VALUE | asOBJ_POD | asOBJ_APP_CLASS);
    if (ret < 0)
    {
        return AEResult::RegObjTypeFail;
    }

    ret = engine->RegisterObjectType("GameComponentCollection", 0, asOBJ_REF | asOBJ_NOHANDLE);
    if (ret < 0)
    {
        return AEResult::RegObjTypeFail;
    }

    ret = engine->RegisterObjectType("GameObjectScriptManager", 0, asOBJ_REF | asOBJ_NOHANDLE);
    if (ret < 0)
    {
        return AEResult::RegObjTypeFail;
    }

    ret = engine->RegisterObjectType("GameObject", 0, asOBJ_REF | asOBJ_NOCOUNT);
    if (ret < 0)
    {
        return AEResult::RegObjTypeFail;
    }

    ret = engine->RegisterObjectType("AudioSourceGOC", 0, asOBJ_REF | asOBJ_NOCOUNT);
    if (ret < 0)
    {
        return AEResult::RegObjTypeFail;
    }

    ///////////////////////////////
    //Finish
    return AEResult::Ok;
}

AEResult GameCoreAddOnAS::RegisterGCBasicPropsObject(asIScriptEngine* engine)
{
    int ret = 0;

    ret = engine->RegisterObjectProperty("GCBasicProps", "uint64 m_GCID", asOFFSET(GCBasicProps, m_GCID));
    if (ret < 0)
    {
        return AEResult::RegObjPropFail;
    }

    ret = engine->RegisterObjectProperty("GCBasicProps", "wstring m_GCName", asOFFSET(GCBasicProps, m_GCName));
    if (ret < 0)
    {
        return AEResult::RegObjPropFail;
    }

    ret = engine->RegisterObjectProperty("GCBasicProps", "uint m_GCCallOrder", asOFFSET(GCBasicProps, m_GCCallOrder));
    if (ret < 0)
    {
        return AEResult::RegObjPropFail;
    }

    ret = engine->RegisterObjectProperty("GCBasicProps", "bool m_Drawable", asOFFSET(GCBasicProps, m_Drawable));
    if (ret < 0)
    {
        return AEResult::RegObjPropFail;
    }

    ret = engine->RegisterObjectProperty("GCBasicProps", "bool m_Enabled", asOFFSET(GCBasicProps, m_Enabled));
    if (ret < 0)
    {
        return AEResult::RegObjPropFail;
    }

    ret = engine->RegisterObjectProperty("GCBasicProps", "bool m_Visibled", asOFFSET(GCBasicProps, m_Visibled));
    if (ret < 0)
    {
        return AEResult::RegObjPropFail;
    }

    ret = engine->RegisterObjectBehaviour("GCBasicProps", asBEHAVE_CONSTRUCT, "void AEAS_GCBasicPropsConstructor()", asFUNCTIONPR(GCBasicProps::Constructor, (GCBasicProps *), void), asCALL_CDECL_OBJFIRST);
    if (ret < 0)
    {
        return AEResult::RegObjBehaviorFail;
    }

    ret = engine->RegisterObjectBehaviour("GCBasicProps", asBEHAVE_DESTRUCT, "void AEAS_GCBasicDestructor()", asFUNCTIONPR(GCBasicProps::Destructor, (GCBasicProps *), void), asCALL_CDECL_OBJFIRST);
    if (ret < 0)
    {
        return AEResult::RegObjBehaviorFail;
    }

    ret = engine->RegisterObjectMethod("GCBasicProps", "GCBasicProps &opAssign(const GCBasicProps &in)", asMETHOD(GCBasicProps, operator=), asCALL_THISCALL);
    if (ret < 0)
    {
        return AEResult::RegObjMethodFail;
    }

    return AEResult::Ok;
}

AEResult GameCoreAddOnAS::RegisterGameComponentCollectionObject(asIScriptEngine* engine)
{
    int ret = 0;

    ret = engine->RegisterObjectMethod("GameComponentCollection", "uint GetSize() const", asMETHOD(GameComponentCollection, GetSize), asCALL_THISCALL);
    if (ret < 0)
    {
        return AEResult::RegObjMethodFail;
    }

    ret = engine->RegisterObjectMethod("GameComponentCollection", "AEResult SetGCDrawCallOrder(uint, uint)", asMETHOD(GameComponentCollection, SetGCDrawCallOrder), asCALL_THISCALL);
    if (ret < 0)
    {
        return AEResult::RegObjMethodFail;
    }

    ret = engine->RegisterObjectMethod("GameComponentCollection", "AEResult SetGCVisible(uint, bool)", asMETHOD(GameComponentCollection, SetGCVisible), asCALL_THISCALL);
    if (ret < 0)
    {
        return AEResult::RegObjMethodFail;
    }

    ret = engine->RegisterObjectMethod("GameComponentCollection", "AEResult SetGCEnable(uint, bool)", asMETHOD(GameComponentCollection, SetGCEnable), asCALL_THISCALL);
    if (ret < 0)
    {
        return AEResult::RegObjMethodFail;
    }

    ret = engine->RegisterObjectMethod("GameComponentCollection", "GCBasicProps GetGCBasicProps(uint)", asMETHOD(GameComponentCollection, GetGCBasicProps), asCALL_THISCALL);
    if (ret < 0)
    {
        return AEResult::RegObjMethodFail;
    }

    ret = engine->RegisterObjectMethod("GameComponentCollection", "GCBasicProps GetGCBasicPropsID(uint)", asMETHOD(GameComponentCollection, GetGCBasicPropsID), asCALL_THISCALL);
    if (ret < 0)
    {
        return AEResult::RegObjMethodFail;
    }

    ret = engine->RegisterObjectMethod("GameComponentCollection", "bool DoesIDExist(uint) const", asMETHOD(GameComponentCollection, DoesIDExist), asCALL_THISCALL);
    if (ret < 0)
    {
        return AEResult::RegObjMethodFail;
    }

    ret = engine->RegisterObjectMethod("GameComponentCollection", "bool ValidIndex(uint) const", asMETHOD(GameComponentCollection, ValidIndex), asCALL_THISCALL);
    if (ret < 0)
    {
        return AEResult::RegObjMethodFail;
    }

    return AEResult::Ok;
}

AEResult GameCoreAddOnAS::RegisterGameComponentCollectionObjectPointer(asIScriptEngine* engine)
{
    int ret = 0;

    AETODO("Change Names for Script");
    ret = engine->RegisterGlobalProperty("GameComponentCollection m_GameComponentCollection", m_GameComponentCollection);
    if (ret < 0)
    {
        return AEResult::RegGlobalPropFail;
    }

    return AEResult::Ok;
}

AEResult GameCoreAddOnAS::RegisterGameObjectScriptInterface(asIScriptEngine* engine)
{
    int ret = 0;

    ret = engine->RegisterInterface(AE_GOS_INTERFACE_NAME);
    if (ret < 0)
    {
        return AEResult::RegInterfaceFail;
    }

    ret = engine->RegisterInterfaceMethod(AE_GOS_INTERFACE_NAME, AE_GOS_INIT_FUNC);
    if (ret < 0)
    {
        return AEResult::RegObjMethodFail;
    }

    ret = engine->RegisterInterfaceMethod(AE_GOS_INTERFACE_NAME, AE_GOS_CONSTANT_UPDATE_FUNC);
    if (ret < 0)
    {
        return AEResult::RegObjMethodFail;
    }

    ret = engine->RegisterInterfaceMethod(AE_GOS_INTERFACE_NAME, AE_GOS_UPDATE_FUNC);
    if (ret < 0)
    {
        return AEResult::RegObjMethodFail;
    }

    ret = engine->RegisterInterfaceMethod(AE_GOS_INTERFACE_NAME, AE_GOS_POST_UPDATE_FUNC);
    if (ret < 0)
    {
        return AEResult::RegObjMethodFail;
    }

    return AEResult::Ok;
}

AEResult GameCoreAddOnAS::RegisterGameObjectScriptManagerObject(asIScriptEngine* engine)
{
    int ret = 0;

    std::string getScriptMethodDecl = AE_GOS_INTERFACE_NAME;
    getScriptMethodDecl += "@ GetScript(const wstring&in)";
    ret = engine->RegisterObjectMethod("GameObjectScriptManager", getScriptMethodDecl.c_str(), asMETHOD(GameObjectScriptManager, GetScript), asCALL_THISCALL);
    if (ret < 0)
    {
        return AEResult::RegObjMethodFail;
    }

    return AEResult::Ok;
}

AEResult GameCoreAddOnAS::RegisterGameObjectScriptManagerObjectPointer(asIScriptEngine* engine)
{
    int ret = 0;

    AETODO("Change Names for Script");
    ret = engine->RegisterGlobalProperty("GameObjectScriptManager m_GameObjectScriptManager", m_GameObjectScriptManager);
    if (ret < 0)
    {
        return AEResult::RegGlobalPropFail;
    }

    return AEResult::Ok;
}

AEResult GameCoreAddOnAS::RegisterGameObjectObject(asIScriptEngine* engine)
{
    int ret = 0;

    //////////////////////////////
    //Register Methods
    ret = engine->RegisterObjectMethod("GameObject", "const vec3& GetRotation() const", asMETHOD(GameObject, GetRotation), asCALL_THISCALL);
    if (ret < 0)
    {
        return AEResult::RegObjMethodFail;
    }

    ret = engine->RegisterObjectMethod("GameObject", "void SetRotation(const vec3&in)", asMETHOD(GameObject, SetRotation), asCALL_THISCALL);
    if (ret < 0)
    {
        return AEResult::RegObjMethodFail;
    }

    ret = engine->RegisterObjectMethod("GameObject", "const vec3& GetPosition() const", asMETHOD(GameObject, GetPosition), asCALL_THISCALL);
    if (ret < 0)
    {
        return AEResult::RegObjMethodFail;
    }

    ret = engine->RegisterObjectMethod("GameObject", "void SetPosition(const vec3&in)", asMETHOD(GameObject, SetPosition), asCALL_THISCALL);
    if (ret < 0)
    {
        return AEResult::RegObjMethodFail;
    }

    ret = engine->RegisterObjectMethod("GameObject", "AudioSourceGOC@ GetAudioSourceGOC(const wstring&in)", asMETHODPR(GameObject, GetAudioSourceGOC, (const std::wstring&), AudioSourceGOC*), asCALL_THISCALL);
    if (ret < 0)
    {
        return AEResult::RegObjMethodFail;
    }

    //////////////////////////////
    //Finish
    return AEResult::Ok;

}

AEResult GameCoreAddOnAS::RegisterAudioSourceGOCObject(asIScriptEngine* engine)
{
    int ret = 0;

    //////////////////////////////
    //Register Methods
    ret = engine->RegisterObjectMethod("AudioSourceGOC", "AudioPlayer@ GetAudioPlayer()", asMETHOD(AudioSourceGOC, GetAudioPlayer), asCALL_THISCALL);
    if (ret < 0)
    {
        return AEResult::RegObjMethodFail;
    }

    //////////////////////////////
    //Finish
    return AEResult::Ok;
}
