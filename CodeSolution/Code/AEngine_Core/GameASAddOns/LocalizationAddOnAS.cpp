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

/***************************
*   Game Engine Includes   *
****************************/
#include "GameASAddOnsDefs.h"
#include "Base\BaseFunctions.h"
#include "LocalizationAddOnAS.h"
#include "Localization\LocalizationManager.h"
#include "Localization\LocalizationManagerDefs.h"

//Always include last
#include "Memory\MemLeaks.h"

/********************
*   Function Defs   *
*********************/
LocalizationAddOnAS::LocalizationAddOnAS()
    : AngelScriptAddOn((uint32_t)XEGameASAddOnsID::Localization)
{
}

LocalizationAddOnAS::~LocalizationAddOnAS()
{
}

AEResult LocalizationAddOnAS::Register(asIScriptEngine* engine)
{
    AEAssert(engine != nullptr);
    if (engine == nullptr)
    {
        return AEResult::NullParameter;
    }

    if (RegisterLocalizationObject(engine) != AEResult::Ok)
    {
        AETODO("Add log");
        std::string errMsg = "";
        //fmt::format(m_LocalizationManager->GetLiteral("AS_REG_BASIC_LOC_ERR_MSG"), __FUNCTION__);
        //m_Logger->AddNewLog(LogLevel::Error, errMsg);

        return AEResult::Fail;
    }

    if (RegisterLocalizationObjectPointer(engine) != AEResult::Ok)
    {
        AETODO("Add log");
        std::string errMsg = "";
        //fmt::format(m_LocalizationManager->GetLiteral("AS_REG_BASIC_LOC_ERR_MSG"), __FUNCTION__);
        //m_Logger->AddNewLog(LogLevel::Error, errMsg);

        return AEResult::Fail;
    }

    return AEResult::Ok;
}

AEResult LocalizationAddOnAS::RegisterLocalizationObject(asIScriptEngine* engine)
{
    int ret = 0;

    ret = engine->RegisterObjectType("LocalizationManager", 0, asOBJ_REF | asOBJ_NOHANDLE);
    if (ret < 0)
    {
        return AEResult::RegObjTypeFail;
    }

    ret = engine->RegisterObjectMethod("LocalizationManager", "const string& GetLiteral(const string& in) const", asMETHOD(LocalizationManager, GetLiteral), asCALL_THISCALL);
    if (ret < 0)
    {
        return AEResult::RegObjMethodFail;
    }

    ret = engine->RegisterObjectMethod("LocalizationManager", "AEResult ReloadAll()", asMETHOD(LocalizationManager, ReloadAll), asCALL_THISCALL);
    if (ret < 0)
    {
        return AEResult::RegObjMethodFail;
    }

    ret = engine->RegisterObjectMethod("LocalizationManager", "AEResult SetDefaultLanguage(const string& in)", asMETHOD(LocalizationManager, SetDefaultLanguage), asCALL_THISCALL);
    if (ret < 0)
    {
        return AEResult::RegObjMethodFail;
    }

    ret = engine->RegisterObjectMethod("LocalizationManager", "const string& GetDefaultLanguage() const", asMETHOD(LocalizationManager, GetDefaultLanguage), asCALL_THISCALL);
    if (ret < 0)
    {
        return AEResult::RegObjMethodFail;
    }

    return AEResult::Ok;
}

AEResult LocalizationAddOnAS::RegisterLocalizationObjectPointer(asIScriptEngine* engine)
{
    int ret = 0;

    AETODO("Change Names for Script");
    ret = engine->RegisterGlobalProperty("LocalizationManager m_LocalizationManager", AELOCMAN);
    if (ret < 0)
    {
        return AEResult::RegGlobalPropFail;
    }

    return AEResult::Ok;
}
