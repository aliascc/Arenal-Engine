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
		std::wstring errMsg = L"";
		//fmt::format(m_LocalizationManager->GetLiteral(L"AS_REG_BASIC_LOC_ERR_MSG"), __FUNCTIONW__);
		//m_Logger->AddNewLog(LogLevel::Error, errMsg);

		return AEResult::Fail;
	}

	if (RegisterLocalizationObjectPointer(engine) != AEResult::Ok)
	{
		AETODO("Add log");
		std::wstring errMsg = L"";
		//fmt::format(m_LocalizationManager->GetLiteral(L"AS_REG_BASIC_LOC_ERR_MSG"), __FUNCTIONW__);
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

	ret = engine->RegisterObjectMethod("LocalizationManager", "const wstring& GetLiteral(const wstring& in) const", asMETHOD(LocalizationManager, GetLiteral), asCALL_THISCALL);
	if (ret < 0)
	{
		return AEResult::RegObjMethodFail;
	}

	ret = engine->RegisterObjectMethod("LocalizationManager", "AEResult ReloadAll()", asMETHOD(LocalizationManager, ReloadAll), asCALL_THISCALL);
	if (ret < 0)
	{
		return AEResult::RegObjMethodFail;
	}

	ret = engine->RegisterObjectMethod("LocalizationManager", "AEResult SetDefaultLanguage(const wstring& in)", asMETHOD(LocalizationManager, SetDefaultLanguage), asCALL_THISCALL);
	if (ret < 0)
	{
		return AEResult::RegObjMethodFail;
	}

	ret = engine->RegisterObjectMethod("LocalizationManager", "const wstring& GetDefaultLanguage() const", asMETHOD(LocalizationManager, GetDefaultLanguage), asCALL_THISCALL);
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