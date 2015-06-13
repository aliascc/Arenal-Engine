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
#include "Time\AETime.h"
#include "AudioPlayer.h"
#include "AudioAddOnAS.h"
#include "GameASAddOnsDefs.h"
#include "Base\BaseFunctions.h"

//Always include last
#include "Memory\MemLeaks.h"

/********************
*   Function Defs   *
*********************/

/*******************
*   AudioAddOnAS   *
********************/
AudioAddOnAS::AudioAddOnAS()
	: AngelScriptAddOn((uint32_t)XEGameASAddOnsID::Audio)
{
}

AudioAddOnAS::~AudioAddOnAS()
{
}

AEResult AudioAddOnAS::Register(asIScriptEngine* engine)
{
	AEAssert(engine != nullptr);
	if (engine == nullptr)
	{
		return AEResult::NullParameter;
	}

	if (RegisterAudioPlayerObject(engine) != AEResult::Ok)
	{
		AETODO("Add Log and return");
		std::wstring errMsg = L"";
		//fmt::format(m_LocalizationManager->GetLiteral(L"AS_REG_BASIC_TIME_ERR_MSG"), __FUNCTIONW__);
		//m_Logger->AddNewLog(LogLevel::Error, errMsg);

		return AEResult::Fail;
	}

	return AEResult::Ok;
}

AEResult AudioAddOnAS::RegisterAudioPlayerObject(asIScriptEngine* engine)
{
	int ret = 0;

	//////////////////////////
	//Register Object
	ret = engine->RegisterObjectType("AudioPlayer", 0, asOBJ_REF | asOBJ_NOCOUNT);
	if (ret < 0)
	{
		return AEResult::RegObjTypeFail;
	}

	//////////////////////////
	//Register Methods
	ret = engine->RegisterObjectMethod("AudioPlayer", "AEResult Play()", asMETHOD(AudioPlayer, Play), asCALL_THISCALL);
	if (ret < 0)
	{
		return AEResult::RegObjMethodFail;
	}

	ret = engine->RegisterObjectMethod("AudioPlayer", "AEResult Pause()", asMETHOD(AudioPlayer, Pause), asCALL_THISCALL);
	if (ret < 0)
	{
		return AEResult::RegObjMethodFail;
	}

	ret = engine->RegisterObjectMethod("AudioPlayer", "AEResult Stop()", asMETHOD(AudioPlayer, Stop), asCALL_THISCALL);
	if (ret < 0)
	{
		return AEResult::RegObjMethodFail;
	}

	//////////////////////////
	//Finish
	return AEResult::Ok;
}
