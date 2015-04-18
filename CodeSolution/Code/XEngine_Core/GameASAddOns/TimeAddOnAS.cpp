/********************************************************
*
* Author: Carlos Chacón N.
*
* Created: 9/15/2014
*
* Desc:
*
*
* Copyright 2012-2014
*
*********************************************************/

/**********************
*   System Includes   *
***********************/

/***************************
*   Game Engine Includes   *
****************************/
#include "Time\XETime.h"
#include "TimeAddOnAS.h"
#include "GameASAddOnsDefs.h"
#include "Base\BaseFunctions.h"

XETODO("Fix this");
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

XEResult TimeAddOnAS::Register(asIScriptEngine* engine)
{
	XEAssert(engine != nullptr);
	if (engine == nullptr)
	{
		return XEResult::NullParameter;
	}

	if (RegisterTimeStampObject(engine) != XEResult::Ok)
	{
		XETODO("Add Log and return");
		std::wstring errMsg = L"";
		//fastformat::fmt(errMsg, m_LocalizationManager->GetLiteral(L"AS_REG_BASIC_TIME_ERR_MSG"), __FUNCTIONW__);
		//m_Logger->AddNewLog(LogLevel::Error, errMsg);

		return XEResult::Fail;
	}

	if (RegisterTimerParamsObject(engine) != XEResult::Ok)
	{
		XETODO("Add Log and return");
		std::wstring errMsg = L"";
		//fastformat::fmt(errMsg, m_LocalizationManager->GetLiteral(L"AS_REG_BASIC_TIME_ERR_MSG"), __FUNCTIONW__);
		//m_Logger->AddNewLog(LogLevel::Error, errMsg);

		return XEResult::Fail;
	}

	return XEResult::Ok;
}

XEResult TimeAddOnAS::RegisterTimeStampObject(asIScriptEngine* engine)
{
	int ret = 0;

	/////////////////////
	//Register TimeStamp
	/////////////////////
	ret = engine->RegisterObjectType("TimeStamp", sizeof(TimeStamp), asOBJ_VALUE | asOBJ_POD | asOBJ_APP_CLASS);
	if (ret < 0)
	{
		return XEResult::RegObjTypeFail;
	}

	ret = engine->RegisterObjectProperty("TimeStamp", "uint m_Day", asOFFSET(TimeStamp, m_Day));
	if (ret < 0)
	{
		return XEResult::RegObjPropFail;
	}

	ret = engine->RegisterObjectProperty("TimeStamp", "uint m_Month", asOFFSET(TimeStamp, m_Month));
	if (ret < 0)
	{
		return XEResult::RegObjPropFail;
	}

	ret = engine->RegisterObjectProperty("TimeStamp", "uint m_Year", asOFFSET(TimeStamp, m_Year));
	if (ret < 0)
	{
		return XEResult::RegObjPropFail;
	}

	ret = engine->RegisterObjectProperty("TimeStamp", "uint m_Hour", asOFFSET(TimeStamp, m_Hour));
	if (ret < 0)
	{
		return XEResult::RegObjPropFail;
	}

	ret = engine->RegisterObjectProperty("TimeStamp", "uint m_Minute", asOFFSET(TimeStamp, m_Minute));
	if (ret < 0)
	{
		return XEResult::RegObjPropFail;
	}

	ret = engine->RegisterObjectProperty("TimeStamp", "uint m_Second", asOFFSET(TimeStamp, m_Second));
	if (ret < 0)
	{
		return XEResult::RegObjPropFail;
	}

	ret = engine->RegisterObjectMethod("TimeStamp", "wstring ToString() const", asMETHOD(TimeStamp, ToString), asCALL_THISCALL);
	if (ret < 0)
	{
		return XEResult::RegObjMethodFail;
	}

	ret = engine->RegisterObjectBehaviour("TimeStamp", asBEHAVE_CONSTRUCT, "void XEAS_TimeStamp_Constructor()", asFUNCTION(TimeStamp_Constructor), asCALL_CDECL_OBJLAST);
	if (ret < 0)
	{
		return XEResult::RegObjBehaviorFail;
	}

	ret = engine->RegisterObjectBehaviour("TimeStamp", asBEHAVE_DESTRUCT, "void XEAS_TimeStamp_Destructor()", asFUNCTION(TimeStamp_Destructor), asCALL_CDECL_OBJLAST);
	if (ret < 0)
	{
		return XEResult::RegObjBehaviorFail;
	}

	return XEResult::Ok;
}

XEResult TimeAddOnAS::RegisterTimerParamsObject(asIScriptEngine* engine)
{
	int ret = 0;

	////////////////////////
	//Register TimerParams
	////////////////////////
	ret = engine->RegisterObjectType("TimerParams", sizeof(TimerParams), asOBJ_VALUE | asOBJ_POD | asOBJ_APP_CLASS);
	if (ret < 0)
	{
		return XEResult::RegObjTypeFail;
	}

	ret = engine->RegisterObjectProperty("TimerParams", "double m_ElapsedTime", asOFFSET(TimerParams, m_ElapsedTime));
	if (ret < 0)
	{
		return XEResult::RegObjPropFail;
	}

	ret = engine->RegisterObjectProperty("TimerParams", "double m_PreviousCallTime", asOFFSET(TimerParams, m_PreviousCallTime));
	if (ret < 0)
	{
		return XEResult::RegObjPropFail;
	}

	ret = engine->RegisterObjectProperty("TimerParams", "double m_TotalElapsedTime", asOFFSET(TimerParams, m_TotalElapsedTime));
	if (ret < 0)
	{
		return XEResult::RegObjPropFail;
	}

	ret = engine->RegisterObjectBehaviour("TimerParams", asBEHAVE_CONSTRUCT, "void XEAS_TimerParams_Constructor()", asFUNCTION(TimerParams_Constructor), asCALL_CDECL_OBJLAST);
	if (ret < 0)
	{
		return XEResult::RegObjBehaviorFail;
	}

	ret = engine->RegisterObjectBehaviour("TimerParams", asBEHAVE_DESTRUCT, "void XEAS_TimerParams_Destructor()", asFUNCTION(TimerParams_Destructor), asCALL_CDECL_OBJLAST);
	if (ret < 0)
	{
		return XEResult::RegObjBehaviorFail;
	}

	return XEResult::Ok;
}
