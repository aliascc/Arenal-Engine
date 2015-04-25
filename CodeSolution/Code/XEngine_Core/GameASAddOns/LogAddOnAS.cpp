/********************************************************
*
* Author: Carlos Chacón N.
*
* Copyright 2012-2015
*
*********************************************************/

/**********************
*   System Includes   *
***********************/

/***************************
*   Game Engine Includes   *
****************************/
#include "LogAddOnAS.h"
#include "Logger\Logger.h"
#include "GameASAddOnsDefs.h"
#include "Logger\LoggerDefs.h"
#include "Base\BaseFunctions.h"

XETODO("Fix this");
/* Do not include it as we need to use the normal new
//Always include last
#include "Memory\MemLeaks.h"
*/

/********************
*   Function Defs   *
*********************/

/************
*   XELog   *
*************/
static void XELog_Constructor(XELog* self)
{
	new(self)XELog();
}

static void XELog_Destructor(XELog* self)
{
	self->~XELog();
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

XEResult LogAddOnAS::Register(asIScriptEngine* engine)
{
	XEAssert(engine != nullptr);
	if (engine == nullptr)
	{
		return XEResult::NullParameter;
	}

	if (RegisterLogLevelEnum(engine) != XEResult::Ok)
	{
		XETODO("Add Log");
		std::wstring errMsg = L"";
		//fastformat::fmt(errMsg, m_LocalizationManager->GetLiteral(L"AS_REG_ENUM_ERR_MSG"), __FUNCTIONW__, L"LogLevel");
		//m_Logger->AddNewLog(LogLevel::Error, errMsg);

		return XEResult::Fail;
	}

	if (RegisterXELogObject(engine) != XEResult::Ok)
	{
		XETODO("Add Log");
		std::wstring errMsg = L"";
		//fastformat::fmt(errMsg, m_LocalizationManager->GetLiteral(L"AS_REG_BASIC_LOGGER_ERR_MSG"), __FUNCTIONW__);
		//m_Logger->AddNewLog(LogLevel::Error, errMsg);

		return XEResult::Fail;
	}

	if (RegisterLoggerObject(engine) != XEResult::Ok)
	{
		XETODO("Add Log");
		std::wstring errMsg = L"";
		//fastformat::fmt(errMsg, m_LocalizationManager->GetLiteral(L"AS_REG_BASIC_LOGGER_ERR_MSG"), __FUNCTIONW__);
		//m_Logger->AddNewLog(LogLevel::Error, errMsg);

		return XEResult::Fail;
	}

	if (RegisterLoggerObjectPointer(engine) != XEResult::Ok)
	{
		XETODO("Add Log");
		std::wstring errMsg = L"";
		//fastformat::fmt(errMsg, m_LocalizationManager->GetLiteral(L"AS_REG_BASIC_LOGGER_ERR_MSG"), __FUNCTIONW__);
		//m_Logger->AddNewLog(LogLevel::Error, errMsg);

		return XEResult::Fail;
	}

	return XEResult::Ok;
}

XEResult LogAddOnAS::RegisterLogLevelEnum(asIScriptEngine* engine)
{
	int ret = 0;

	///////////////////////
	//Register LogLevel
	ret = engine->RegisterEnum("LogLevel");
	if (ret < 0)
	{
		return XEResult::RegEnumFail;
	}

	ret = engine->RegisterEnumValue("LogLevel", "None", static_cast<uint32_t>(LogLevel::None));
	if (ret < 0)
	{
		return XEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("LogLevel", "Info", static_cast<uint32_t>(LogLevel::Info));
	if (ret < 0)
	{
		return XEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("LogLevel", "Warning", static_cast<uint32_t>(LogLevel::Warning));
	if (ret < 0)
	{
		return XEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("LogLevel", "Error", static_cast<uint32_t>(LogLevel::Error));
	if (ret < 0)
	{
		return XEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("LogLevel", "Debug", static_cast<uint32_t>(LogLevel::Debug));
	if (ret < 0)
	{
		return XEResult::RegEnumValFail;
	}

	return XEResult::Ok;
}

XEResult LogAddOnAS::RegisterXELogObject(asIScriptEngine* engine)
{
	int ret = 0;

	///////////////////
	//Register XELog
	///////////////////
	ret = engine->RegisterObjectType("XELog", sizeof(XELog), asOBJ_VALUE | asOBJ_POD | asOBJ_APP_CLASS);
	if (ret < 0)
	{
		return XEResult::RegObjTypeFail;
	}

	ret = engine->RegisterObjectProperty("XELog", "wstring m_Log", asOFFSET(XELog, m_Log));
	if (ret < 0)
	{
		return XEResult::RegObjPropFail;
	}

	ret = engine->RegisterObjectProperty("XELog", "LogLevel m_Level", asOFFSET(XELog, m_Level));
	if (ret < 0)
	{
		return XEResult::RegObjPropFail;
	}

	ret = engine->RegisterObjectProperty("XELog", "uint m_LogLine", asOFFSET(XELog, m_LogLine));
	if (ret < 0)
	{
		return XEResult::RegObjPropFail;
	}

	ret = engine->RegisterObjectProperty("XELog", "TimeStamp m_TimeStamp", asOFFSET(XELog, m_TimeStamp));
	if (ret < 0)
	{
		return XEResult::RegObjPropFail;
	}

	ret = engine->RegisterObjectMethod("XELog", "wstring ToString() const", asMETHOD(XELog, ToString), asCALL_THISCALL);
	if (ret < 0)
	{
		return XEResult::RegObjMethodFail;
	}

	ret = engine->RegisterObjectBehaviour("XELog", asBEHAVE_CONSTRUCT, "void XEAS_XELog_Constructor()", asFUNCTION(XELog_Constructor), asCALL_CDECL_OBJLAST);
	if (ret < 0)
	{
		return XEResult::RegObjBehaviorFail;
	}

	ret = engine->RegisterObjectBehaviour("XELog", asBEHAVE_DESTRUCT, "void XEAS_XELog_Destructor()", asFUNCTION(XELog_Destructor), asCALL_CDECL_OBJLAST);
	if (ret < 0)
	{
		return XEResult::RegObjBehaviorFail;
	}

	ret = engine->RegisterObjectMethod("XELog", "XELog &opAssign(const XELog& in)", asMETHOD(XELog, operator=), asCALL_THISCALL);
	if (ret < 0)
	{
		return XEResult::RegObjMethodFail;
	}

	return XEResult::Ok;
}

XEResult LogAddOnAS::RegisterLoggerObject(asIScriptEngine* engine)
{
	int ret = 0;

	///////////////////
	//Register Logger
	///////////////////
	ret = engine->RegisterObjectType("Logger", 0, asOBJ_REF | asOBJ_NOHANDLE);
	if (ret < 0)
	{
		return XEResult::RegObjTypeFail;
	}

	ret = engine->RegisterObjectMethod("Logger", "void AddNewLog(LogLevel, const wstring& in)", asMETHOD(Logger, AddNewLog), asCALL_THISCALL);
	if (ret < 0)
	{
		return XEResult::RegObjMethodFail;
	}

	ret = engine->RegisterObjectMethod("Logger", "XEResult SaveLogsInFile()", asMETHOD(Logger, SaveLogsInFile), asCALL_THISCALL);
	if (ret < 0)
	{
		return XEResult::RegObjMethodFail;
	}

	ret = engine->RegisterObjectMethod("Logger", "bool ErrorsExist() const", asMETHOD(Logger, ErrorsExist), asCALL_THISCALL);
	if (ret < 0)
	{
		return XEResult::RegObjMethodFail;
	}

	ret = engine->RegisterObjectMethod("Logger", "bool WarningsExist() const", asMETHOD(Logger, WarningsExist), asCALL_THISCALL);
	if (ret < 0)
	{
		return XEResult::RegObjMethodFail;
	}

	ret = engine->RegisterObjectMethod("Logger", "void ResetErrors()", asMETHOD(Logger, ResetErrors), asCALL_THISCALL);
	if (ret < 0)
	{
		return XEResult::RegObjMethodFail;
	}

	ret = engine->RegisterObjectMethod("Logger", "void ResetWarning()", asMETHOD(Logger, ResetWarning), asCALL_THISCALL);
	if (ret < 0)
	{
		return XEResult::RegObjMethodFail;
	}

	ret = engine->RegisterObjectMethod("Logger", "void SetCapacity(uint)", asMETHOD(Logger, SetCapacity), asCALL_THISCALL);
	if (ret < 0)
	{
		return XEResult::RegObjMethodFail;
	}

	ret = engine->RegisterObjectMethod("Logger", "void SetLogLevel(LogLevel)", asMETHOD(Logger, SetLogLevel), asCALL_THISCALL);
	if (ret < 0)
	{
		return XEResult::RegObjMethodFail;
	}

	ret = engine->RegisterObjectMethod("Logger", "XEResult SetLogFilename(const wstring& in)", asMETHOD(Logger, SetLogFilename), asCALL_THISCALL);
	if (ret < 0)
	{
		return XEResult::RegObjMethodFail;
	}

	ret = engine->RegisterObjectMethod("Logger", "uint GetCapacity() const", asMETHOD(Logger, GetCapacity), asCALL_THISCALL);
	if (ret < 0)
	{
		return XEResult::RegObjMethodFail;
	}

	ret = engine->RegisterObjectMethod("Logger", "LogLevel GetLogLevel() const", asMETHOD(Logger, GetLogLevel), asCALL_THISCALL);
	if (ret < 0)
	{
		return XEResult::RegObjMethodFail;
	}

	ret = engine->RegisterObjectMethod("Logger", "const wstring& GetLogFilename() const", asMETHOD(Logger, GetLogFilename), asCALL_THISCALL);
	if (ret < 0)
	{
		return XEResult::RegObjMethodFail;
	}

	ret = engine->RegisterObjectMethod("Logger", "XEResult ActivateLogToFile()", asMETHOD(Logger, ActivateLogToFile), asCALL_THISCALL);
	if (ret < 0)
	{
		return XEResult::RegObjMethodFail;
	}

	ret = engine->RegisterObjectMethod("Logger", "XEResult DeactivateLogToFile()", asMETHOD(Logger, DeactivateLogToFile), asCALL_THISCALL);
	if (ret < 0)
	{
		return XEResult::RegObjMethodFail;
	}

	ret = engine->RegisterObjectMethod("Logger", "array<XELog>@ GetLogsScript()", asMETHOD(Logger, GetLogsScript), asCALL_THISCALL);
	if (ret < 0)
	{
		return XEResult::RegObjMethodFail;
	}

	///////////////////////////////////
	//Register XELoggerHelper Methods
	///////////////////////////////////
	ret = engine->RegisterGlobalFunction("wstring LogLevelStr(LogLevel)", asFUNCTION(XELogHelpers::LogLevelStr), asCALL_CDECL);
	if (ret < 0)
	{
		return XEResult::RegGlobalFuncFail;
	}

	return XEResult::Ok;
}

XEResult LogAddOnAS::RegisterLoggerObjectPointer(asIScriptEngine* engine)
{
	int ret = 0;

	////////////////////////////////////
	//Set XELog Array Type to Logger
	asIObjectType* scriptXEArrayType = engine->GetObjectTypeById(engine->GetTypeIdByDecl("array<XELog>"));
	XELOGGER->SetScriptXELogArrayType(scriptXEArrayType);

	XETODO("Change Names for Script");
	ret = engine->RegisterGlobalProperty("Logger m_Logger", XELOGGER);
	if (ret < 0)
	{
		return XEResult::RegGlobalPropFail;
	}

	return XEResult::Ok;
}