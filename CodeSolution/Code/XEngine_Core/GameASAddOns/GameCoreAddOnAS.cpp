/********************************************************
*
* Author: Carlos Chacón N.
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
#include "GameCoreAddOnAS.h"
#include "GameASAddOnsDefs.h"
#include "Base\BaseFunctions.h"
#include "GameObject\GameObject.h"
#include "GameUtils\GameUtilsDefs.h"
#include "GameUtils\GameComponentCollection.h"
#include "GameObject\Components\AudioSourceGOC.h"
#include "GameObject\Scripts\GameObjectScriptDefs.h"
#include "GameObject\Scripts\GameObjectScriptManager.h"

//Always include last
#include "Memory\MemLeaks.h"

/********************
*   Function Defs   *
*********************/
GameCoreAddOnAS::GameCoreAddOnAS(GameComponentCollection* gameComponentCollection, GameObjectScriptManager* gameObjectScriptManager)
	: AngelScriptAddOn((uint32_t)XEGameASAddOnsID::GameCore)
	, m_GameComponentCollection(gameComponentCollection)
	, m_GameObjectScriptManager(gameObjectScriptManager)
{
	XEAssert(m_GameComponentCollection != nullptr);
	XEAssert(m_GameObjectScriptManager != nullptr);
}

GameCoreAddOnAS::~GameCoreAddOnAS()
{
}

XEResult GameCoreAddOnAS::Register(asIScriptEngine* engine)
{
	XEAssert(engine != nullptr);
	if (engine == nullptr)
	{
		return XEResult::NullParameter;
	}

	XEAssert(m_GameComponentCollection != nullptr);
	if (m_GameComponentCollection == nullptr)
	{
		XETODO("Add better return code");
		return XEResult::NullObj;
	}

	XEAssert(m_GameObjectScriptManager != nullptr);
	if (m_GameObjectScriptManager == nullptr)
	{
		XETODO("Add better return code");
		return XEResult::NullObj;
	}

	if (RegisterObjects(engine) != XEResult::Ok)
	{
		std::wstring errMsg = L"";
		XETODO("Add Log & Wrong Literal Change");
		//fastformat::fmt(errMsg, m_LocalizationManager->GetLiteral(L"AS_REG_BASIC_LOGGER_ERR_MSG"), __FUNCTIONW__);
		//m_Logger->AddNewLog(LogLevel::Error, errMsg);

		return XEResult::Fail;
	}

	if (RegisterGCBasicPropsObject(engine) != XEResult::Ok)
	{
		std::wstring errMsg = L"";
		XETODO("Add Log & Wrong Literal Change");
		//fastformat::fmt(errMsg, m_LocalizationManager->GetLiteral(L"AS_REG_BASIC_LOGGER_ERR_MSG"), __FUNCTIONW__);
		//m_Logger->AddNewLog(LogLevel::Error, errMsg);

		return XEResult::Fail;
	}

	if (RegisterGameComponentCollectionObject(engine) != XEResult::Ok)
	{
		std::wstring errMsg = L"";
		XETODO("Add Log & Wrong Literal Change");
		//fastformat::fmt(errMsg, m_LocalizationManager->GetLiteral(L"AS_REG_BASIC_LOGGER_ERR_MSG"), __FUNCTIONW__);
		//m_Logger->AddNewLog(LogLevel::Error, errMsg);

		return XEResult::Fail;
	}

	if (RegisterGameComponentCollectionObjectPointer(engine) != XEResult::Ok)
	{
		std::wstring errMsg = L"";
		XETODO("Add Log & Wrong Literal Change");
		//fastformat::fmt(errMsg, m_LocalizationManager->GetLiteral(L"AS_REG_BASIC_LOGGER_ERR_MSG"), __FUNCTIONW__);
		//m_Logger->AddNewLog(LogLevel::Error, errMsg);

		return XEResult::Fail;
	}

	if (RegisterGameObjectScriptInterface(engine) != XEResult::Ok)
	{
		std::wstring errMsg = L"";
		XETODO("Add Log & Wrong Literal Change");
		//fastformat::fmt(errMsg, m_LocalizationManager->GetLiteral(L"AS_REG_BASIC_LOGGER_ERR_MSG"), __FUNCTIONW__);
		//m_Logger->AddNewLog(LogLevel::Error, errMsg);

		return XEResult::Fail;
	}

	if (RegisterGameObjectScriptManagerObject(engine) != XEResult::Ok)
	{
		std::wstring errMsg = L"";
		XETODO("Add Log & Wrong Literal Change");
		//fastformat::fmt(errMsg, m_LocalizationManager->GetLiteral(L"AS_REG_BASIC_LOGGER_ERR_MSG"), __FUNCTIONW__);
		//m_Logger->AddNewLog(LogLevel::Error, errMsg);

		return XEResult::Fail;
	}

	if (RegisterGameObjectScriptManagerObjectPointer(engine) != XEResult::Ok)
	{
		std::wstring errMsg = L"";
		XETODO("Add Log & Wrong Literal Change");
		//fastformat::fmt(errMsg, m_LocalizationManager->GetLiteral(L"AS_REG_BASIC_LOGGER_ERR_MSG"), __FUNCTIONW__);
		//m_Logger->AddNewLog(LogLevel::Error, errMsg);

		return XEResult::Fail;
	}

	if (RegisterGameObjectObject(engine) != XEResult::Ok)
	{
		std::wstring errMsg = L"";
		XETODO("Add Log & Wrong Literal Change");
		//fastformat::fmt(errMsg, m_LocalizationManager->GetLiteral(L"AS_REG_BASIC_LOGGER_ERR_MSG"), __FUNCTIONW__);
		//m_Logger->AddNewLog(LogLevel::Error, errMsg);

		return XEResult::Fail;
	}

	if (RegisterAudioSourceGOCObject(engine) != XEResult::Ok)
	{
		std::wstring errMsg = L"";
		XETODO("Add Log & Wrong Literal Change");
		//fastformat::fmt(errMsg, m_LocalizationManager->GetLiteral(L"AS_REG_BASIC_LOGGER_ERR_MSG"), __FUNCTIONW__);
		//m_Logger->AddNewLog(LogLevel::Error, errMsg);

		return XEResult::Fail;
	}

	return XEResult::Ok;
}

XEResult GameCoreAddOnAS::RegisterObjects(asIScriptEngine* engine)
{
	int ret = 0;

	///////////////////////////////
	//Register Objects
	ret = engine->RegisterObjectType("GCBasicProps", sizeof(GCBasicProps), asOBJ_VALUE | asOBJ_POD | asOBJ_APP_CLASS);
	if (ret < 0)
	{
		return XEResult::RegObjTypeFail;
	}

	ret = engine->RegisterObjectType("GameComponentCollection", 0, asOBJ_REF | asOBJ_NOHANDLE);
	if (ret < 0)
	{
		return XEResult::RegObjTypeFail;
	}

	ret = engine->RegisterObjectType("GameObjectScriptManager", 0, asOBJ_REF | asOBJ_NOHANDLE);
	if (ret < 0)
	{
		return XEResult::RegObjTypeFail;
	}

	ret = engine->RegisterObjectType("GameObject", 0, asOBJ_REF | asOBJ_NOCOUNT);
	if (ret < 0)
	{
		return XEResult::RegObjTypeFail;
	}

	ret = engine->RegisterObjectType("AudioSourceGOC", 0, asOBJ_REF | asOBJ_NOCOUNT);
	if (ret < 0)
	{
		return XEResult::RegObjTypeFail;
	}

	///////////////////////////////
	//Finish
	return XEResult::Ok;
}

XEResult GameCoreAddOnAS::RegisterGCBasicPropsObject(asIScriptEngine* engine)
{
	int ret = 0;

	ret = engine->RegisterObjectProperty("GCBasicProps", "uint64 m_GCID", asOFFSET(GCBasicProps, m_GCID));
	if (ret < 0)
	{
		return XEResult::RegObjPropFail;
	}

	ret = engine->RegisterObjectProperty("GCBasicProps", "wstring m_GCName", asOFFSET(GCBasicProps, m_GCName));
	if (ret < 0)
	{
		return XEResult::RegObjPropFail;
	}

	ret = engine->RegisterObjectProperty("GCBasicProps", "uint m_GCCallOrder", asOFFSET(GCBasicProps, m_GCCallOrder));
	if (ret < 0)
	{
		return XEResult::RegObjPropFail;
	}

	ret = engine->RegisterObjectProperty("GCBasicProps", "bool m_Drawable", asOFFSET(GCBasicProps, m_Drawable));
	if (ret < 0)
	{
		return XEResult::RegObjPropFail;
	}

	ret = engine->RegisterObjectProperty("GCBasicProps", "bool m_Enabled", asOFFSET(GCBasicProps, m_Enabled));
	if (ret < 0)
	{
		return XEResult::RegObjPropFail;
	}

	ret = engine->RegisterObjectProperty("GCBasicProps", "bool m_Visibled", asOFFSET(GCBasicProps, m_Visibled));
	if (ret < 0)
	{
		return XEResult::RegObjPropFail;
	}

	ret = engine->RegisterObjectBehaviour("GCBasicProps", asBEHAVE_CONSTRUCT, "void XEAS_GCBasicPropsConstructor()", asFUNCTIONPR(GCBasicProps::Constructor, (GCBasicProps *), void), asCALL_CDECL_OBJFIRST);
	if (ret < 0)
	{
		return XEResult::RegObjBehaviorFail;
	}

	ret = engine->RegisterObjectBehaviour("GCBasicProps", asBEHAVE_DESTRUCT, "void XEAS_GCBasicDestructor()", asFUNCTIONPR(GCBasicProps::Destructor, (GCBasicProps *), void), asCALL_CDECL_OBJFIRST);
	if (ret < 0)
	{
		return XEResult::RegObjBehaviorFail;
	}

	ret = engine->RegisterObjectMethod("GCBasicProps", "GCBasicProps &opAssign(const GCBasicProps &in)", asMETHOD(GCBasicProps, operator=), asCALL_THISCALL);
	if (ret < 0)
	{
		return XEResult::RegObjMethodFail;
	}

	return XEResult::Ok;
}

XEResult GameCoreAddOnAS::RegisterGameComponentCollectionObject(asIScriptEngine* engine)
{
	int ret = 0;

	ret = engine->RegisterObjectMethod("GameComponentCollection", "uint GetSize() const", asMETHOD(GameComponentCollection, GetSize), asCALL_THISCALL);
	if (ret < 0)
	{
		return XEResult::RegObjMethodFail;
	}

	ret = engine->RegisterObjectMethod("GameComponentCollection", "XEResult SetGCDrawCallOrder(uint, uint)", asMETHOD(GameComponentCollection, SetGCDrawCallOrder), asCALL_THISCALL);
	if (ret < 0)
	{
		return XEResult::RegObjMethodFail;
	}

	ret = engine->RegisterObjectMethod("GameComponentCollection", "XEResult SetGCVisible(uint, bool)", asMETHOD(GameComponentCollection, SetGCVisible), asCALL_THISCALL);
	if (ret < 0)
	{
		return XEResult::RegObjMethodFail;
	}

	ret = engine->RegisterObjectMethod("GameComponentCollection", "XEResult SetGCEnable(uint, bool)", asMETHOD(GameComponentCollection, SetGCEnable), asCALL_THISCALL);
	if (ret < 0)
	{
		return XEResult::RegObjMethodFail;
	}

	ret = engine->RegisterObjectMethod("GameComponentCollection", "GCBasicProps GetGCBasicProps(uint)", asMETHOD(GameComponentCollection, GetGCBasicProps), asCALL_THISCALL);
	if (ret < 0)
	{
		return XEResult::RegObjMethodFail;
	}

	ret = engine->RegisterObjectMethod("GameComponentCollection", "GCBasicProps GetGCBasicPropsID(uint)", asMETHOD(GameComponentCollection, GetGCBasicPropsID), asCALL_THISCALL);
	if (ret < 0)
	{
		return XEResult::RegObjMethodFail;
	}

	ret = engine->RegisterObjectMethod("GameComponentCollection", "bool DoesIDExist(uint) const", asMETHOD(GameComponentCollection, DoesIDExist), asCALL_THISCALL);
	if (ret < 0)
	{
		return XEResult::RegObjMethodFail;
	}

	ret = engine->RegisterObjectMethod("GameComponentCollection", "bool ValidIndex(uint) const", asMETHOD(GameComponentCollection, ValidIndex), asCALL_THISCALL);
	if (ret < 0)
	{
		return XEResult::RegObjMethodFail;
	}

	return XEResult::Ok;
}

XEResult GameCoreAddOnAS::RegisterGameComponentCollectionObjectPointer(asIScriptEngine* engine)
{
	int ret = 0;

	XETODO("Change Names for Script");
	ret = engine->RegisterGlobalProperty("GameComponentCollection m_GameComponentCollection", m_GameComponentCollection);
	if (ret < 0)
	{
		return XEResult::RegGlobalPropFail;
	}

	return XEResult::Ok;
}

XEResult GameCoreAddOnAS::RegisterGameObjectScriptInterface(asIScriptEngine* engine)
{
	int ret = 0;

	ret = engine->RegisterInterface(XE_GOS_INTERFACE_NAME);
	if (ret < 0)
	{
		return XEResult::RegInterfaceFail;
	}

	ret = engine->RegisterInterfaceMethod(XE_GOS_INTERFACE_NAME, XE_GOS_INIT_FUNC);
	if (ret < 0)
	{
		return XEResult::RegObjMethodFail;
	}

	ret = engine->RegisterInterfaceMethod(XE_GOS_INTERFACE_NAME, XE_GOS_CONSTANT_UPDATE_FUNC);
	if (ret < 0)
	{
		return XEResult::RegObjMethodFail;
	}

	ret = engine->RegisterInterfaceMethod(XE_GOS_INTERFACE_NAME, XE_GOS_UPDATE_FUNC);
	if (ret < 0)
	{
		return XEResult::RegObjMethodFail;
	}

	ret = engine->RegisterInterfaceMethod(XE_GOS_INTERFACE_NAME, XE_GOS_POST_UPDATE_FUNC);
	if (ret < 0)
	{
		return XEResult::RegObjMethodFail;
	}

	return XEResult::Ok;
}

XEResult GameCoreAddOnAS::RegisterGameObjectScriptManagerObject(asIScriptEngine* engine)
{
	int ret = 0;

	std::string getScriptMethodDecl = XE_GOS_INTERFACE_NAME;
	getScriptMethodDecl += "@ GetScript(const wstring&in)";
	ret = engine->RegisterObjectMethod("GameObjectScriptManager", getScriptMethodDecl.c_str(), asMETHOD(GameObjectScriptManager, GetScript), asCALL_THISCALL);
	if (ret < 0)
	{
		return XEResult::RegObjMethodFail;
	}

	return XEResult::Ok;
}

XEResult GameCoreAddOnAS::RegisterGameObjectScriptManagerObjectPointer(asIScriptEngine* engine)
{
	int ret = 0;

	XETODO("Change Names for Script");
	ret = engine->RegisterGlobalProperty("GameObjectScriptManager m_GameObjectScriptManager", m_GameObjectScriptManager);
	if (ret < 0)
	{
		return XEResult::RegGlobalPropFail;
	}

	return XEResult::Ok;
}

XEResult GameCoreAddOnAS::RegisterGameObjectObject(asIScriptEngine* engine)
{
	int ret = 0;

	//////////////////////////////
	//Register Methods
	ret = engine->RegisterObjectMethod("GameObject", "const vec3& GetRotation() const", asMETHOD(GameObject, GetRotation), asCALL_THISCALL);
	if (ret < 0)
	{
		return XEResult::RegObjMethodFail;
	}

	ret = engine->RegisterObjectMethod("GameObject", "void SetRotation(const vec3&in)", asMETHOD(GameObject, SetRotation), asCALL_THISCALL);
	if (ret < 0)
	{
		return XEResult::RegObjMethodFail;
	}

	ret = engine->RegisterObjectMethod("GameObject", "const vec3& GetPosition() const", asMETHOD(GameObject, GetPosition), asCALL_THISCALL);
	if (ret < 0)
	{
		return XEResult::RegObjMethodFail;
	}

	ret = engine->RegisterObjectMethod("GameObject", "void SetPosition(const vec3&in)", asMETHOD(GameObject, SetPosition), asCALL_THISCALL);
	if (ret < 0)
	{
		return XEResult::RegObjMethodFail;
	}

	ret = engine->RegisterObjectMethod("GameObject", "AudioSourceGOC@ GetAudioSourceGOC(const wstring&in)", asMETHODPR(GameObject, GetAudioSourceGOC, (const std::wstring&), AudioSourceGOC*), asCALL_THISCALL);
	if (ret < 0)
	{
		return XEResult::RegObjMethodFail;
	}

	//////////////////////////////
	//Finish
	return XEResult::Ok;

}

XEResult GameCoreAddOnAS::RegisterAudioSourceGOCObject(asIScriptEngine* engine)
{
	int ret = 0;

	//////////////////////////////
	//Register Methods
	ret = engine->RegisterObjectMethod("AudioSourceGOC", "AudioPlayer@ GetAudioPlayer()", asMETHOD(AudioSourceGOC, GetAudioPlayer), asCALL_THISCALL);
	if (ret < 0)
	{
		return XEResult::RegObjMethodFail;
	}

	//////////////////////////////
	//Finish
	return XEResult::Ok;
}
