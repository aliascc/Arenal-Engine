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
#include "precomp_scripting.h"

/**********************
*   System Includes   *
***********************/

/*************************
*   3rd Party Includes   *
**************************/

/***************************
*   Game Engine Includes   *
****************************/
#include "AngelScriptDefs.h"
#include "AngelScriptManager.h"
#include "Add-ons\AngelScriptAddOn.h"

//Always include last
#include "Memory\MemLeaks.h"

/********************
*   Function Defs   *
*********************/
AETODO("Check for mutex");
AngelScriptManager::AngelScriptManager()
{
}

AngelScriptManager::~AngelScriptManager()
{
    ReleaseAngel(m_ASEngine);
}

AEResult AngelScriptManager::Initialize()
{
    if(m_IsReady)
    {
        return AEResult::Ok;
    }

    ////////////////////////////////////////////////////////////////////////
    //Initialize AS Engine
    m_ASEngine = asCreateScriptEngine(ANGELSCRIPT_VERSION);
    
    if(m_ASEngine == nullptr)
    {
        std::string errMsg = fmt::format(AELOCMAN.GetLiteral("AS_CREATE_ENGINE_FAIL_ERR_MSG"), __FUNCTION__);
        AELOGGER.AddNewLog(LogLevel::Error, errMsg);

        return AEResult::Fail;
    }

    ////////////////////////////////////////////////////////////////////////
    // Set engine to use UTF-16 encoding for string literals
    m_ASEngine->SetEngineProperty(asEP_SCRIPT_SCANNER, 1);
    m_ASEngine->SetEngineProperty(asEP_STRING_ENCODING, 1);
    m_ASEngine->SetEngineProperty(asEP_REQUIRE_ENUM_SCOPE, 1);

    int ret = 0;

    ////////////////////////////////////////////////////////////////////////
    //The script compiler will write any compiler messages to the callback.
    //We want to log this messages in the logger
    ret = m_ASEngine->SetMessageCallback(asMETHOD(AngelScriptManager, ASMessageCallback), this, asCALL_THISCALL);
    if(ret < 0)
    {
        std::string errMsg = fmt::format(AELOCMAN.GetLiteral("AS_SET_MSG_CALLBACK_ERR_MSG"), __FUNCTION__);
        AELOGGER.AddNewLog(LogLevel::Error, errMsg);

        return AEResult::Fail;
    }

    AEResult rst = AEResult::Ok;

    ////////////////////////////////////////////////////////////////////////
    //Register Array Helper
    RegisterScriptArray(m_ASEngine, true);

    ////////////////////////////////////////////////////////////////////////
    //Register STD String
    RegisterStdString(m_ASEngine);

    ////////////////////////////////////////////////////////////////////////
    //Register STD String
    RegisterStdStringUtils(m_ASEngine);

    ////////////////////////////////////////////////////////////////////////
    //Register Dictionary Helper
    RegisterScriptDictionary(m_ASEngine);

    ////////////////////////////////////////////////////////////////////////
    //Register the generic handle type, called 'ref' in the script
    RegisterScriptHandle(m_ASEngine);

    ////////////////////////////////////////////////////////////////////////
    //Set Ready Flag
    m_IsReady = true;

    return AEResult::Ok;
}

AEResult AngelScriptManager::RegisterAddOn(AngelScriptAddOn& addOn)
{
    if (!m_IsReady)
    {
        return AEResult::NotReady;
    }

    if (m_AddOnsRegisterIDs.find(addOn.GetAddOnID()) != m_AddOnsRegisterIDs.end())
    {
        return AEResult::ObjExists;
    }

    AEResult ret = addOn.Register(m_ASEngine);

    if (ret != AEResult::Ok)
    {
        AETODO("Add Log");
        return ret;
    }

    m_AddOnsRegisterIDs.insert(addOn.GetAddOnID());

    return AEResult::Ok;
}

AEResult AngelScriptManager::LoadScript(const std::string& scriptName, const std::string& module, asIScriptModule** asModule)
{
    if(!m_IsReady)
    {
        return AEResult::NotReady;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////
    // Check if the script has already been loaded
    asIScriptModule* tempASModule = m_ASEngine->GetModule(module.c_str());
    if (tempASModule != nullptr)
    {
        if (asModule != nullptr)
        {
            *asModule = tempASModule;
        }

        return AEResult::ASModuleAlreadyCreated;
    }

    CScriptBuilder builder;
    int ret = 0;

    ret = builder.StartNewModule(m_ASEngine, module.c_str());
    if(ret < 0)
    {
        return AEResult::ASModuleCreateFail;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////
    // Let the builder load the script, and do the necessary pre-processing (include files, etc)
    ret = builder.AddSectionFromFile(scriptName.c_str());
    if(ret < 0)
    {
        m_ASEngine->DiscardModule(module.c_str());
        return AEResult::ASAddSecModuleFail;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////
    //Build Module
    ret = builder.BuildModule();
    if(ret < 0)
    {
        m_ASEngine->DiscardModule(module.c_str());
        return AEResult::ASBuildModuleFail;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////
    // Assigned Module to Parameter if non null
    if (asModule != nullptr)
    {
        tempASModule = m_ASEngine->GetModule(module.c_str());

        *asModule = tempASModule;
    }

    return AEResult::Ok;
}

AEResult AngelScriptManager::LoadScript(const std::vector<std::string>& scriptNames, const std::string& module, asIScriptModule** asModule)
{
    if(!m_IsReady)
    {
        return AEResult::NotReady;
    }

    if(scriptNames.size() == 0)
    {
        return AEResult::ZeroSize;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////
    // Check if the script has already been loaded
    asIScriptModule* tempASModule = m_ASEngine->GetModule(module.c_str());

    if (tempASModule != nullptr)
    {
        if (asModule != nullptr)
        {
            *asModule = tempASModule;
        }

        return AEResult::ASModuleAlreadyCreated;
    }

    CScriptBuilder builder;
    int ret = 0;

    ret = builder.StartNewModule(m_ASEngine, module.c_str());
    if(ret < 0)
    {
        return AEResult::ASModuleCreateFail;
    }

    uint32_t size = (uint32_t)scriptNames.size();
    for(uint32_t i = 0; i < size; ++i)
    {
        /////////////////////////////////////////////////////////////////////////////////////////////////
        // Let the builder load the script, and do the necessary pre-processing (include files, etc)
        ret = builder.AddSectionFromFile(scriptNames[i].c_str());
        if(ret < 0)
        {
            m_ASEngine->DiscardModule(module.c_str());
            return AEResult::ASAddSecModuleFail;
        }
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////
    //Build Module
    ret = builder.BuildModule();
    if(ret < 0)
    {
        m_ASEngine->DiscardModule(module.c_str());
        return AEResult::ASBuildModuleFail;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////
    // Assigned Module to Parameter if non null
    if (asModule != nullptr)
    {
        tempASModule = m_ASEngine->GetModule(module.c_str());

        *asModule = tempASModule;
    }

    return AEResult::Ok;
}

AEResult AngelScriptManager::RemoveModule(const std::string& module)
{
    if(!m_IsReady)
    {
        return AEResult::NotReady;
    }

    int ret = m_ASEngine->DiscardModule(module.c_str());
    if(ret < 0)
    {
        return AEResult::ASDiscardModuleFail;
    }

    return AEResult::Ok;
}

void AngelScriptManager::ASMessageCallback(const asSMessageInfo& msg)
{
    LogLevel logLVL = LogLevel::Info;

    switch(msg.type)
    {
        case asMSGTYPE_INFORMATION:
            logLVL = LogLevel::Info;
            break;
        case asMSGTYPE_WARNING:
            logLVL = LogLevel::Warning;
            break;
        case asMSGTYPE_ERROR:
            logLVL = LogLevel::Error;
            break;
        default:
            logLVL = LogLevel::Debug;
            break;
    }

    std::string errMsg = fmt::format(AELOCMAN.GetLiteral("AS_ENGINE_CALLBACK_ERR_MSG"), __FUNCTION__, msg.section, msg.row, msg.col, msg.message);
    AELOGGER.AddNewLog(logLVL, errMsg);
}
