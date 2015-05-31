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
#include "Time\XETime.h"
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

XEResult AudioAddOnAS::Register(asIScriptEngine* engine)
{
	XEAssert(engine != nullptr);
	if (engine == nullptr)
	{
		return XEResult::NullParameter;
	}

	if (RegisterAudioPlayerObject(engine) != XEResult::Ok)
	{
		XETODO("Add Log and return");
		std::wstring errMsg = L"";
		//fmt::format(m_LocalizationManager->GetLiteral(L"AS_REG_BASIC_TIME_ERR_MSG"), __FUNCTIONW__);
		//m_Logger->AddNewLog(LogLevel::Error, errMsg);

		return XEResult::Fail;
	}

	return XEResult::Ok;
}

XEResult AudioAddOnAS::RegisterAudioPlayerObject(asIScriptEngine* engine)
{
	int ret = 0;

	//////////////////////////
	//Register Object
	ret = engine->RegisterObjectType("AudioPlayer", 0, asOBJ_REF | asOBJ_NOCOUNT);
	if (ret < 0)
	{
		return XEResult::RegObjTypeFail;
	}

	//////////////////////////
	//Register Methods
	ret = engine->RegisterObjectMethod("AudioPlayer", "XEResult Play()", asMETHOD(AudioPlayer, Play), asCALL_THISCALL);
	if (ret < 0)
	{
		return XEResult::RegObjMethodFail;
	}

	ret = engine->RegisterObjectMethod("AudioPlayer", "XEResult Pause()", asMETHOD(AudioPlayer, Pause), asCALL_THISCALL);
	if (ret < 0)
	{
		return XEResult::RegObjMethodFail;
	}

	ret = engine->RegisterObjectMethod("AudioPlayer", "XEResult Stop()", asMETHOD(AudioPlayer, Stop), asCALL_THISCALL);
	if (ret < 0)
	{
		return XEResult::RegObjMethodFail;
	}

	//////////////////////////
	//Finish
	return XEResult::Ok;
}
