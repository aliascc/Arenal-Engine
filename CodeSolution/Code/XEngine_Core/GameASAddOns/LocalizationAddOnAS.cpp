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

XEResult LocalizationAddOnAS::Register(asIScriptEngine* engine)
{
	XEAssert(engine != nullptr);
	if (engine == nullptr)
	{
		return XEResult::NullParameter;
	}

	if (RegisterLocalizationObject(engine) != XEResult::Ok)
	{
		XETODO("Add log");
		std::wstring errMsg = L"";
		//fastformat::fmt(errMsg, m_LocalizationManager->GetLiteral(L"AS_REG_BASIC_LOC_ERR_MSG"), __FUNCTIONW__);
		//m_Logger->AddNewLog(LogLevel::Error, errMsg);

		return XEResult::Fail;
	}

	if (RegisterLocalizationObjectPointer(engine) != XEResult::Ok)
	{
		XETODO("Add log");
		std::wstring errMsg = L"";
		//fastformat::fmt(errMsg, m_LocalizationManager->GetLiteral(L"AS_REG_BASIC_LOC_ERR_MSG"), __FUNCTIONW__);
		//m_Logger->AddNewLog(LogLevel::Error, errMsg);

		return XEResult::Fail;
	}

	return XEResult::Ok;
}

XEResult LocalizationAddOnAS::RegisterLocalizationObject(asIScriptEngine* engine)
{
	int ret = 0;

	ret = engine->RegisterObjectType("LocalizationManager", 0, asOBJ_REF | asOBJ_NOHANDLE);
	if (ret < 0)
	{
		return XEResult::RegObjTypeFail;
	}

	ret = engine->RegisterObjectMethod("LocalizationManager", "const wstring& GetLiteral(const wstring& in) const", asMETHOD(LocalizationManager, GetLiteral), asCALL_THISCALL);
	if (ret < 0)
	{
		return XEResult::RegObjMethodFail;
	}

	ret = engine->RegisterObjectMethod("LocalizationManager", "XEResult Reload()", asMETHOD(LocalizationManager, Reload), asCALL_THISCALL);
	if (ret < 0)
	{
		return XEResult::RegObjMethodFail;
	}

	ret = engine->RegisterObjectMethod("LocalizationManager", "XEResult SetDefaultLanguage(const wstring& in)", asMETHOD(LocalizationManager, SetDefaultLanguage), asCALL_THISCALL);
	if (ret < 0)
	{
		return XEResult::RegObjMethodFail;
	}

	ret = engine->RegisterObjectMethod("LocalizationManager", "const wstring& GetDefaultLanguage() const", asMETHOD(LocalizationManager, GetDefaultLanguage), asCALL_THISCALL);
	if (ret < 0)
	{
		return XEResult::RegObjMethodFail;
	}

	return XEResult::Ok;
}

XEResult LocalizationAddOnAS::RegisterLocalizationObjectPointer(asIScriptEngine* engine)
{
	int ret = 0;

	XETODO("Change Names for Script");
	ret = engine->RegisterGlobalProperty("LocalizationManager m_LocalizationManager", XELOCMAN);
	if (ret < 0)
	{
		return XEResult::RegGlobalPropFail;
	}

	return XEResult::Ok;
}