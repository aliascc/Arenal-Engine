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
#include "Time\AETime.h"
#include "TimeAddOnAS.h"
#include "GameASAddOnsDefs.h"
#include "Base\BaseFunctions.h"

AETODO("Fix this");
/* Do not include it as we need to use the normal new
//Always include last
#include "Memory\MemLeaks.h"
*/

/********************
*   Function Defs   *
*********************/

/****************
*   TimeStamp   *
*****************/
static void TimeStamp_Constructor(TimeStamp* self)
{
	new(self)TimeStamp();
}

static void TimeStamp_Destructor(TimeStamp* self)
{
	self->~TimeStamp();
}

/******************
*   TimerParams   *
*******************/
static void TimerParams_Constructor(TimerParams* self)
{
	new(self)TimerParams();
}

static void TimerParams_Destructor(TimerParams* self)
{
	self->~TimerParams();
}


/******************
*   TimeAddOnAS   *
*******************/
TimeAddOnAS::TimeAddOnAS()
	: AngelScriptAddOn((uint32_t)XEGameASAddOnsID::Time)
{
}

TimeAddOnAS::~TimeAddOnAS()
{
}

AEResult TimeAddOnAS::Register(asIScriptEngine* engine)
{
	AEAssert(engine != nullptr);
	if (engine == nullptr)
	{
		return AEResult::NullParameter;
	}

	if (RegisterTimeStampObject(engine) != AEResult::Ok)
	{
		AETODO("Add Log and return");
		std::wstring errMsg = L"";
		//fmt::format(m_LocalizationManager->GetLiteral(L"AS_REG_BASIC_TIME_ERR_MSG"), __FUNCTIONW__);
		//m_Logger->AddNewLog(LogLevel::Error, errMsg);

		return AEResult::Fail;
	}

	if (RegisterTimerParamsObject(engine) != AEResult::Ok)
	{
		AETODO("Add Log and return");
		std::wstring errMsg = L"";
		//fmt::format(m_LocalizationManager->GetLiteral(L"AS_REG_BASIC_TIME_ERR_MSG"), __FUNCTIONW__);
		//m_Logger->AddNewLog(LogLevel::Error, errMsg);

		return AEResult::Fail;
	}

	return AEResult::Ok;
}

AEResult TimeAddOnAS::RegisterTimeStampObject(asIScriptEngine* engine)
{
	int ret = 0;

	/////////////////////
	//Register TimeStamp
	/////////////////////
	ret = engine->RegisterObjectType("TimeStamp", sizeof(TimeStamp), asOBJ_VALUE | asOBJ_POD | asOBJ_APP_CLASS);
	if (ret < 0)
	{
		return AEResult::RegObjTypeFail;
	}

	ret = engine->RegisterObjectProperty("TimeStamp", "uint m_Day", asOFFSET(TimeStamp, m_Day));
	if (ret < 0)
	{
		return AEResult::RegObjPropFail;
	}

	ret = engine->RegisterObjectProperty("TimeStamp", "uint m_Month", asOFFSET(TimeStamp, m_Month));
	if (ret < 0)
	{
		return AEResult::RegObjPropFail;
	}

	ret = engine->RegisterObjectProperty("TimeStamp", "uint m_Year", asOFFSET(TimeStamp, m_Year));
	if (ret < 0)
	{
		return AEResult::RegObjPropFail;
	}

	ret = engine->RegisterObjectProperty("TimeStamp", "uint m_Hour", asOFFSET(TimeStamp, m_Hour));
	if (ret < 0)
	{
		return AEResult::RegObjPropFail;
	}

	ret = engine->RegisterObjectProperty("TimeStamp", "uint m_Minute", asOFFSET(TimeStamp, m_Minute));
	if (ret < 0)
	{
		return AEResult::RegObjPropFail;
	}

	ret = engine->RegisterObjectProperty("TimeStamp", "uint m_Second", asOFFSET(TimeStamp, m_Second));
	if (ret < 0)
	{
		return AEResult::RegObjPropFail;
	}

	ret = engine->RegisterObjectMethod("TimeStamp", "wstring ToString() const", asMETHOD(TimeStamp, ToString), asCALL_THISCALL);
	if (ret < 0)
	{
		return AEResult::RegObjMethodFail;
	}

	ret = engine->RegisterObjectBehaviour("TimeStamp", asBEHAVE_CONSTRUCT, "void AEAS_TimeStamp_Constructor()", asFUNCTION(TimeStamp_Constructor), asCALL_CDECL_OBJLAST);
	if (ret < 0)
	{
		return AEResult::RegObjBehaviorFail;
	}

	ret = engine->RegisterObjectBehaviour("TimeStamp", asBEHAVE_DESTRUCT, "void AEAS_TimeStamp_Destructor()", asFUNCTION(TimeStamp_Destructor), asCALL_CDECL_OBJLAST);
	if (ret < 0)
	{
		return AEResult::RegObjBehaviorFail;
	}

	return AEResult::Ok;
}

AEResult TimeAddOnAS::RegisterTimerParamsObject(asIScriptEngine* engine)
{
	int ret = 0;

	////////////////////////
	//Register TimerParams
	////////////////////////
	ret = engine->RegisterObjectType("TimerParams", sizeof(TimerParams), asOBJ_VALUE | asOBJ_POD | asOBJ_APP_CLASS);
	if (ret < 0)
	{
		return AEResult::RegObjTypeFail;
	}

	ret = engine->RegisterObjectProperty("TimerParams", "double m_ElapsedTime", asOFFSET(TimerParams, m_ElapsedTime));
	if (ret < 0)
	{
		return AEResult::RegObjPropFail;
	}

	ret = engine->RegisterObjectProperty("TimerParams", "double m_PreviousCallTime", asOFFSET(TimerParams, m_PreviousCallTime));
	if (ret < 0)
	{
		return AEResult::RegObjPropFail;
	}

	ret = engine->RegisterObjectProperty("TimerParams", "double m_TotalElapsedTime", asOFFSET(TimerParams, m_TotalElapsedTime));
	if (ret < 0)
	{
		return AEResult::RegObjPropFail;
	}

	ret = engine->RegisterObjectBehaviour("TimerParams", asBEHAVE_CONSTRUCT, "void AEAS_TimerParams_Constructor()", asFUNCTION(TimerParams_Constructor), asCALL_CDECL_OBJLAST);
	if (ret < 0)
	{
		return AEResult::RegObjBehaviorFail;
	}

	ret = engine->RegisterObjectBehaviour("TimerParams", asBEHAVE_DESTRUCT, "void AEAS_TimerParams_Destructor()", asFUNCTION(TimerParams_Destructor), asCALL_CDECL_OBJLAST);
	if (ret < 0)
	{
		return AEResult::RegObjBehaviorFail;
	}

	return AEResult::Ok;
}
