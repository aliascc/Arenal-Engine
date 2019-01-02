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
#include "LogAddOnAS.h"
#include "GameASAddOnsDefs.h"

AETODO("Fix this");
/* Do not include it as we need to use the normal new
//Always include last
#include "Memory\MemLeaks.h"
*/

/********************
*   Function Defs   *
*********************/

/************
*   AELog   *
*************/
static void AELog_Constructor(AELog* self)
{
    new(self)AELog();
}

static void AELog_Destructor(AELog* self)
{
    self->~AELog();
}

/*****************
*   LogAddOnAS   *
******************/
LogAddOnAS::LogAddOnAS()
    : AngelScriptAddOn((uint32_t)XEGameASAddOnsID::Log)
{
}

LogAddOnAS::~LogAddOnAS()
{
}

AEResult LogAddOnAS::Register(asIScriptEngine* engine)
{
    AEAssert(engine != nullptr);
    if (engine == nullptr)
    {
        return AEResult::NullParameter;
    }

    if (RegisterLogLevelEnum(engine) != AEResult::Ok)
    {
        AETODO("Add Log");
        std::string errMsg = "";
        //fmt::format(m_LocalizationManager->GetLiteral("AS_REG_ENUM_ERR_MSG"), __FUNCTION__, L"LogLevel");
        //m_Logger->AddNewLog(LogLevel::Error, errMsg);

        return AEResult::Fail;
    }

    if (RegisterAELogObject(engine) != AEResult::Ok)
    {
        AETODO("Add Log");
        std::string errMsg = "";
        //fmt::format(m_LocalizationManager->GetLiteral("AS_REG_BASIC_LOGGER_ERR_MSG"), __FUNCTION__);
        //m_Logger->AddNewLog(LogLevel::Error, errMsg);

        return AEResult::Fail;
    }

    if (RegisterLoggerObject(engine) != AEResult::Ok)
    {
        AETODO("Add Log");
        std::string errMsg = "";
        //fmt::format(m_LocalizationManager->GetLiteral("AS_REG_BASIC_LOGGER_ERR_MSG"), __FUNCTION__);
        //m_Logger->AddNewLog(LogLevel::Error, errMsg);

        return AEResult::Fail;
    }

    if (RegisterLoggerObjectPointer(engine) != AEResult::Ok)
    {
        AETODO("Add Log");
        std::string errMsg = "";
        //fmt::format(m_LocalizationManager->GetLiteral("AS_REG_BASIC_LOGGER_ERR_MSG"), __FUNCTION__);
        //m_Logger->AddNewLog(LogLevel::Error, errMsg);

        return AEResult::Fail;
    }

    return AEResult::Ok;
}

AEResult LogAddOnAS::RegisterLogLevelEnum(asIScriptEngine* engine)
{
    int ret = 0;

    ///////////////////////
    //Register LogLevel
    ret = engine->RegisterEnum("LogLevel");
    if (ret < 0)
    {
        return AEResult::RegEnumFail;
    }

    ret = engine->RegisterEnumValue("LogLevel", "None", static_cast<uint32_t>(LogLevel::None));
    if (ret < 0)
    {
        return AEResult::RegEnumValFail;
    }

    ret = engine->RegisterEnumValue("LogLevel", "Info", static_cast<uint32_t>(LogLevel::Info));
    if (ret < 0)
    {
        return AEResult::RegEnumValFail;
    }

    ret = engine->RegisterEnumValue("LogLevel", "Warning", static_cast<uint32_t>(LogLevel::Warning));
    if (ret < 0)
    {
        return AEResult::RegEnumValFail;
    }

    ret = engine->RegisterEnumValue("LogLevel", "Error", static_cast<uint32_t>(LogLevel::Error));
    if (ret < 0)
    {
        return AEResult::RegEnumValFail;
    }

    ret = engine->RegisterEnumValue("LogLevel", "Debug", static_cast<uint32_t>(LogLevel::Debug));
    if (ret < 0)
    {
        return AEResult::RegEnumValFail;
    }

    return AEResult::Ok;
}

AEResult LogAddOnAS::RegisterAELogObject(asIScriptEngine* engine)
{
    int ret = 0;

    ///////////////////
    //Register AELog
    ///////////////////
    ret = engine->RegisterObjectType("AELog", sizeof(AELog), asOBJ_VALUE | asOBJ_POD | asOBJ_APP_CLASS);
    if (ret < 0)
    {
        return AEResult::RegObjTypeFail;
    }

    ret = engine->RegisterObjectProperty("AELog", "string m_Log", asOFFSET(AELog, m_Log));
    if (ret < 0)
    {
        return AEResult::RegObjPropFail;
    }

    ret = engine->RegisterObjectProperty("AELog", "LogLevel m_Level", asOFFSET(AELog, m_Level));
    if (ret < 0)
    {
        return AEResult::RegObjPropFail;
    }

    ret = engine->RegisterObjectProperty("AELog", "uint m_LogLine", asOFFSET(AELog, m_LogLine));
    if (ret < 0)
    {
        return AEResult::RegObjPropFail;
    }

    ret = engine->RegisterObjectProperty("AELog", "TimeStamp m_TimeStamp", asOFFSET(AELog, m_TimeStamp));
    if (ret < 0)
    {
        return AEResult::RegObjPropFail;
    }

    ret = engine->RegisterObjectMethod("AELog", "string ToString() const", asMETHOD(AELog, ToString), asCALL_THISCALL);
    if (ret < 0)
    {
        return AEResult::RegObjMethodFail;
    }

    ret = engine->RegisterObjectBehaviour("AELog", asBEHAVE_CONSTRUCT, "void AEAS_AELog_Constructor()", asFUNCTION(AELog_Constructor), asCALL_CDECL_OBJLAST);
    if (ret < 0)
    {
        return AEResult::RegObjBehaviorFail;
    }

    ret = engine->RegisterObjectBehaviour("AELog", asBEHAVE_DESTRUCT, "void AEAS_AELog_Destructor()", asFUNCTION(AELog_Destructor), asCALL_CDECL_OBJLAST);
    if (ret < 0)
    {
        return AEResult::RegObjBehaviorFail;
    }

    ret = engine->RegisterObjectMethod("AELog", "AELog &opAssign(const AELog& in)", asMETHOD(AELog, operator=), asCALL_THISCALL);
    if (ret < 0)
    {
        return AEResult::RegObjMethodFail;
    }

    return AEResult::Ok;
}

AEResult LogAddOnAS::RegisterLoggerObject(asIScriptEngine* engine)
{
    int ret = 0;

    ///////////////////
    //Register Logger
    ///////////////////
    ret = engine->RegisterObjectType("Logger", 0, asOBJ_REF | asOBJ_NOHANDLE);
    if (ret < 0)
    {
        return AEResult::RegObjTypeFail;
    }

    ret = engine->RegisterObjectMethod("Logger", "void AddNewLog(LogLevel, const string& in)", asMETHOD(Logger, AddNewLog), asCALL_THISCALL);
    if (ret < 0)
    {
        return AEResult::RegObjMethodFail;
    }

    ret = engine->RegisterObjectMethod("Logger", "AEResult SaveLogsInFile()", asMETHOD(Logger, SaveLogsInFile), asCALL_THISCALL);
    if (ret < 0)
    {
        return AEResult::RegObjMethodFail;
    }

    ret = engine->RegisterObjectMethod("Logger", "bool ErrorsExist() const", asMETHOD(Logger, ErrorsExist), asCALL_THISCALL);
    if (ret < 0)
    {
        return AEResult::RegObjMethodFail;
    }

    ret = engine->RegisterObjectMethod("Logger", "bool WarningsExist() const", asMETHOD(Logger, WarningsExist), asCALL_THISCALL);
    if (ret < 0)
    {
        return AEResult::RegObjMethodFail;
    }

    ret = engine->RegisterObjectMethod("Logger", "void ResetErrors()", asMETHOD(Logger, ResetErrors), asCALL_THISCALL);
    if (ret < 0)
    {
        return AEResult::RegObjMethodFail;
    }

    ret = engine->RegisterObjectMethod("Logger", "void ResetWarning()", asMETHOD(Logger, ResetWarning), asCALL_THISCALL);
    if (ret < 0)
    {
        return AEResult::RegObjMethodFail;
    }

    ret = engine->RegisterObjectMethod("Logger", "void SetCapacity(uint)", asMETHOD(Logger, SetCapacity), asCALL_THISCALL);
    if (ret < 0)
    {
        return AEResult::RegObjMethodFail;
    }

    ret = engine->RegisterObjectMethod("Logger", "void SetLogLevel(LogLevel)", asMETHOD(Logger, SetLogLevel), asCALL_THISCALL);
    if (ret < 0)
    {
        return AEResult::RegObjMethodFail;
    }

    ret = engine->RegisterObjectMethod("Logger", "AEResult SetLogFilename(const string& in)", asMETHOD(Logger, SetLogFilename), asCALL_THISCALL);
    if (ret < 0)
    {
        return AEResult::RegObjMethodFail;
    }

    ret = engine->RegisterObjectMethod("Logger", "uint GetCapacity() const", asMETHOD(Logger, GetCapacity), asCALL_THISCALL);
    if (ret < 0)
    {
        return AEResult::RegObjMethodFail;
    }

    ret = engine->RegisterObjectMethod("Logger", "LogLevel GetLogLevel() const", asMETHOD(Logger, GetLogLevel), asCALL_THISCALL);
    if (ret < 0)
    {
        return AEResult::RegObjMethodFail;
    }

    ret = engine->RegisterObjectMethod("Logger", "const string& GetLogFilename() const", asMETHOD(Logger, GetLogFilename), asCALL_THISCALL);
    if (ret < 0)
    {
        return AEResult::RegObjMethodFail;
    }

    ret = engine->RegisterObjectMethod("Logger", "AEResult ActivateLogToFile()", asMETHOD(Logger, ActivateLogToFile), asCALL_THISCALL);
    if (ret < 0)
    {
        return AEResult::RegObjMethodFail;
    }

    ret = engine->RegisterObjectMethod("Logger", "AEResult DeactivateLogToFile()", asMETHOD(Logger, DeactivateLogToFile), asCALL_THISCALL);
    if (ret < 0)
    {
        return AEResult::RegObjMethodFail;
    }

    ret = engine->RegisterObjectMethod("Logger", "array<AELog>@ GetLogsScript()", asMETHOD(Logger, GetLogsScript), asCALL_THISCALL);
    if (ret < 0)
    {
        return AEResult::RegObjMethodFail;
    }

    ///////////////////////////////////
    //Register AELOGGERHelper Methods
    ///////////////////////////////////
    ret = engine->RegisterGlobalFunction("string LogLevelStr(LogLevel)", asFUNCTION(AELogHelpers::LogLevelStr), asCALL_CDECL);
    if (ret < 0)
    {
        return AEResult::RegGlobalFuncFail;
    }

    return AEResult::Ok;
}

AEResult LogAddOnAS::RegisterLoggerObjectPointer(asIScriptEngine* engine)
{
    int ret = 0;

    ////////////////////////////////////
    //Set AELog Array Type to Logger
    asITypeInfo* scriptXEArrayType = engine->GetObjectTypeByIndex(engine->GetTypeIdByDecl("array<AELog>"));
    AELOGGER.SetScriptAELogArrayType(scriptXEArrayType);

    AETODO("Change Names for Script");
    AETODO("Fix for reference");
    /*ret = engine->RegisterGlobalProperty("Logger m_Logger", AELOGGER);
    if (ret < 0)
    {
        return AEResult::RegGlobalPropFail;
    }*/

    return AEResult::Ok;
}
