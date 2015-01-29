/********************************************************
*
* Author: Carlos Chacón N.
*
* Created: 5/17/2013
*
* Desc:
*
*
* Copyright 2013
*
*********************************************************/


/**********************
*   System Includes   *
***********************/

/*************************
*   3rd Party Includes   *
**************************/

/***************************
*   Game Engine Includes   *
****************************/
#include "Base\BaseFunctions.h"
#include "GameObjectScriptGOC.h"
#include "AngelScript\AngelScriptDefs.h"
#include "AngelScript\AngelScriptManager.h"
#include "GameObject\Scripts\GameObjectScript.h"
#include "GameAssets\Assets\GameObjectScriptAsset.h"
#include "GameObject\Scripts\GameObjectScriptDefs.h"
#include "GameObject\Scripts\GameObjectScriptManager.h"

//Always include last
#include "Memory\MemLeaks.h"

/********************
*   Function Defs   *
*********************/
XETODO("Add mutex");
GameObjectScriptGOC::GameObjectScriptGOC(GameObject* gameObject, const std::wstring& name, AngelScriptManager* angelScriptManager, GameObjectScriptManager* gameObjectScriptManager)
	: GameObjectComponent(gameObject, GameObjectComponentType::GameObjectScript)
	, m_Name(name)
	, m_AngelScriptManager(angelScriptManager)
	, m_GameObjectScriptManager(gameObjectScriptManager)
{
	XEAssert(m_AngelScriptManager != nullptr);
	XEAssert(m_GameObjectScriptManager != nullptr);

	if (m_AngelScriptManager != nullptr && m_GameObjectScriptManager != nullptr)
	{
		m_ASContext = m_AngelScriptManager->GetASEngine()->CreateContext();

		m_IsReady = true;
	}
}

GameObjectScriptGOC::~GameObjectScriptGOC()
{
	RemoveGameObjectScriptAsset();

	if (m_ASContext != nullptr)
	{
		m_ASContext->Unprepare();

		ReleaseAngel(m_ASContext);
	}

	DeleteMem(m_TempGameObjectScriptProperties);
}

void GameObjectScriptGOC::GameObjectScriptAssetDeletion(GameAsset* asset)
{
	XEAssert(asset != nullptr);
	if (asset == nullptr)
	{
		return;
	}

	XEAssert(asset->GetGameContentType() == GameContentType::GameObjectScript);
	if (asset->GetGameContentType() != GameContentType::GameObjectScript)
	{
		return;
	}

	if (m_GameObjectScript.m_AssetID != asset->GetUniqueAssetID())
	{
		return;
	}

	ClearGameObjectScriptAsset(false);
}

void GameObjectScriptGOC::GameObjectScriptAssetPreReload(GameAsset* asset)
{
	XEAssert(asset != nullptr);
	if (asset == nullptr)
	{
		return;
	}

	XEAssert(asset->GetGameContentType() == GameContentType::GameObjectScript);
	if (asset->GetGameContentType() != GameContentType::GameObjectScript)
	{
		return;
	}

	if (m_GameObjectScriptInstance == nullptr)
	{
		return;
	}

	if (m_GameObjectScriptProperties != nullptr)
	{
		DeleteMem(m_TempGameObjectScriptProperties);

		m_TempGameObjectScriptProperties = m_GameObjectScriptProperties;
		m_GameObjectScriptProperties = nullptr;

		m_TempGameObjectScriptProperties->SaveCurrentValues();
		m_TempGameObjectScriptProperties->ClearPropertiesAddress();
	}

	std::wstring instanceName = m_GameObjectScriptInstance->m_Name;
	if (RemoveGameObjectScriptInstance() != XEResult::Ok)
	{
		XETODO("Add log");
		return;
	}

	m_TempInstanceName = instanceName;
}

void GameObjectScriptGOC::GameObjectScriptAssetReload(GameAsset* asset)
{
	XEAssert(asset != nullptr);
	if (asset == nullptr)
	{
		return;
	}

	XEAssert(asset->GetGameContentType() == GameContentType::GameObjectScript);
	if (asset->GetGameContentType() != GameContentType::GameObjectScript)
	{
		return;
	}

	if (m_GameObjectScriptInstance != nullptr)
	{
		return;
	}

	if (m_TempInstanceName.empty())
	{
		return;
	}

	if (CreateGameObjectScriptInstance(m_TempInstanceName) != XEResult::Ok)
	{
		XETODO("Add log");
		XETODO("Set old variable back to what they were");
		return;
	}

	if (m_TempGameObjectScriptProperties != nullptr)
	{
		XETODO("Check ret");
		m_TempGameObjectScriptProperties->CopySavedValuesToProperties(*m_GameObjectScriptProperties);

		DeleteMem(m_TempGameObjectScriptProperties);
	}

	m_TempInstanceName.clear();
}

XEResult GameObjectScriptGOC::SetGameObjectScriptAsset(GameObjectScriptAsset* asset)
{
	XEAssert(asset != nullptr);
	if (asset == nullptr)
	{
		return XEResult::NullParameter;
	}

	if (asset->GetGameContentType() != GameContentType::GameObjectScript)
	{
		return XEResult::InvalidObjType;
	}

	if (m_GameObjectScript.m_ResourceAsset != nullptr)
	{
		XEResult ret = RemoveGameObjectScriptAsset();

		if (ret != XEResult::Ok)
		{
			XETODO("Log error");

			XETODO("Add better return code");
			return XEResult::Fail;
		}
	}

	uint64_t callerID = XE_Base::GetNextUniqueID();

	XEResult ret = asset->RegisterEventHandlers(callerID, 
												std::bind(&GameObjectScriptGOC::GameObjectScriptAssetDeletion, this, std::placeholders::_1),
												std::bind(&GameObjectScriptGOC::GameObjectScriptAssetReload, this, std::placeholders::_1),
												std::bind(&GameObjectScriptGOC::GameObjectScriptAssetPreReload, this, std::placeholders::_1));
	if (ret != XEResult::Ok)
	{
		XETODO("Log error");

		return ret;
	}

	m_GameObjectScript.m_AssetID					= asset->GetUniqueAssetID();
	m_GameObjectScript.m_GameAsset					= asset;
	m_GameObjectScript.m_CallerID					= callerID;
	m_GameObjectScript.m_ResourceAsset				= asset->GetGameObjectScriptReference();
	m_GameObjectScript.m_OnListenerObjDeletionEvent	= asset->GetOnListenerObjDeletionEventHandler();

	return XEResult::Ok;
}

XEResult GameObjectScriptGOC::RemoveGameObjectScriptAsset()
{
	return ClearGameObjectScriptAsset(true);
}

XEResult GameObjectScriptGOC::ClearGameObjectScriptAsset(bool informGameAsset)
{
	if (m_GameObjectScript.m_ResourceAsset != nullptr)
	{
		if (m_GameObjectScript.m_OnListenerObjDeletionEvent != nullptr && informGameAsset)
		{
			m_GameObjectScript.m_OnListenerObjDeletionEvent(m_GameObjectScript.m_CallerID);
		}

		m_GameObjectScript.m_AssetID					= 0;
		m_GameObjectScript.m_GameAsset					= nullptr;
		m_GameObjectScript.m_CallerID					= 0;
		m_GameObjectScript.m_OnListenerObjDeletionEvent	= nullptr;

		XERelease(m_GameObjectScript.m_ResourceAsset);

		DeleteMem(m_GameObjectScriptProperties);

		DeleteMem(m_GameObjectScriptInstance);
	}

	return XEResult::Ok;
}

std::wstring GameObjectScriptGOC::GetGameObjectScriptName() const
{
	if (m_GameObjectScript.m_ResourceAsset != nullptr)
	{
		return m_GameObjectScript.m_ResourceAsset->GetName();
	}
	else
	{
		return L"";
	}
}

std::wstring GameObjectScriptGOC::GetGameObjectScriptInstanceName() const
{
	if (m_GameObjectScriptInstance != nullptr)
	{
		return m_GameObjectScriptInstance->m_Name;
	}
	else
	{
		return L"";
	}
}

XEResult GameObjectScriptGOC::CreateGameObjectScriptInstance(const std::wstring& instanceName)
{
	/////////////////////////////////////////
	//Pre-checks
	if (!m_IsReady)
	{
		return XEResult::NotReady;
	}

	if (!m_GameObjectScript.m_GameAsset->IsLoaded())
	{
		return XEResult::GameAssetNotLoaded;
	}

	XEAssert(m_GameObjectScriptInstance == nullptr && m_GameObjectScriptProperties == nullptr);
	if (m_GameObjectScriptInstance != nullptr || m_GameObjectScriptProperties != nullptr)
	{
		return XEResult::ObjExists;
	}

	XEAssert(!instanceName.empty());
	if (instanceName.empty())
	{
		return XEResult::EmptyName;
	}

	XEAssert(m_GameObjectScript.m_ResourceAsset != nullptr);
	if (m_GameObjectScript.m_ResourceAsset == nullptr)
	{
		return XEResult::NullObj;
	}


	const GameObjectScriptFunctions* gosFunctions = m_GameObjectScript.m_ResourceAsset->GetGameObjectScriptFunctions();
	XEAssert(gosFunctions != nullptr);
	if (gosFunctions == nullptr)
	{
		return XEResult::NullObj;
	}

	/////////////////////////////////////////
	//Variables
	int retInt = 0;
	XEResult ret = XEResult::Ok;

	/////////////////////////////////////////
	//Verify that the instances does not exist
	if (m_GameObjectScriptManager->Exists(instanceName))
	{
		XETODO("Add log error");
		return XEResult::ObjExists;
	}

	/////////////////////////////////////////
	//Create Instance
	retInt = m_ASContext->Prepare(gosFunctions->m_Constructor);
	if (retInt < 0)
	{
		return XEResult::ASPrepareContextFailed;
	}

	retInt = m_ASContext->SetArgObject(0, m_GameObject);
	if (retInt < 0)
	{
		return XEResult::ASPrepareContextFailed;
	}

	retInt = m_ASContext->Execute();
	if (retInt != asEXECUTION_FINISHED)
	{
		if (retInt == asEXECUTION_EXCEPTION)
		{
			XETODO("Add logs with more info");
			//std::cout << "Exception: " << ctx->GetExceptionString() << std::endl;
			//std::cout << "Function: " << ctx->GetExceptionFunction()->GetDeclaration() << std::endl;
			//std::cout << "Line: " << ctx->GetExceptionLineNumber() << std::endl;

			// It is possible to print more information about the location of the 
			// exception, for example the call stack, values of variables, etc if 
			// that is of interest.
		}

		XETODO("log error");
		XETODO("Add execute error code");
		return XEResult::Fail;
	}

	m_GameObjectScriptInstance = new GameObjectScriptInstance();

	m_GameObjectScriptInstance->m_Name = instanceName;

	m_GameObjectScriptInstance->m_ASObj = *((asIScriptObject**)m_ASContext->GetAddressOfReturnValue());
	m_GameObjectScriptInstance->m_ASObj->AddRef();

	m_ASContext->Unprepare();

	XETODO("Check Return");
	m_GameObjectScriptManager->Add(instanceName, m_GameObjectScriptInstance);

	m_GameObjectScriptProperties = new GameObjectScriptProperties();

	uint32_t propertyCount = m_GameObjectScriptInstance->m_ASObj->GetPropertyCount();
	for (uint32_t i = 0; i < propertyCount; i++)
	{
		asETypeIdFlags type = (asETypeIdFlags)m_GameObjectScriptInstance->m_ASObj->GetPropertyTypeId(i);

		if (!XEAngelScriptHelpers::TypeIsBasic(type))
		{
			continue;
		}

		GameObjectScriptProperty* gosProperty = new GameObjectScriptProperty();

		gosProperty->m_PropertyType		= type;
		gosProperty->m_PropertyName		= m_GameObjectScriptInstance->m_ASObj->GetPropertyName(i);
		gosProperty->m_PropertyAddress	= m_GameObjectScriptInstance->m_ASObj->GetAddressOfProperty(i);

		m_GameObjectScriptProperties->m_PropertiesMap[gosProperty->m_PropertyName] = gosProperty;
	}

	return XEResult::Ok;
}

XEResult GameObjectScriptGOC::RemoveGameObjectScriptInstance()
{
	/////////////////////////////////////////
	//Pre-checks
	if (!m_IsReady)
	{
		return XEResult::NotReady;
	}

	XEAssert(m_GameObjectScriptInstance != nullptr);
	if (m_GameObjectScriptInstance == nullptr)
	{
		return XEResult::NullObj;
	}

	/////////////////////////////////////////
	//Remove Instance
	m_GameObjectScriptManager->Remove(m_GameObjectScriptInstance->m_Name);

	DeleteMem(m_GameObjectScriptProperties);

	DeleteMem(m_GameObjectScriptInstance);

	/////////////////////////////////////////
	//Finish
	return XEResult::Ok;
}

XEResult GameObjectScriptGOC::ExecuteScriptInitialize()
{
	/////////////////////////////////////////
	//Pre-checks
	if (!m_IsReady)
	{
		return XEResult::NotReady;
	}

	if (m_GameObjectScriptInstance == nullptr)
	{
		XETODO("set better return code");
		return XEResult::NullObj;
	}

	int retInt = 0;

	/////////////////////////////////////////
	//Prepare Context Initialize
	retInt = m_ASContext->Prepare(m_GameObjectScript.m_ResourceAsset->GetGameObjectScriptFunctions()->m_Initialize);
	if (retInt < 0)
	{
		return XEResult::ASPrepareContextFailed;
	}

	retInt = m_ASContext->SetObject(m_GameObjectScriptInstance->m_ASObj);
	if (retInt < 0)
	{
		XETODO("Add set object error code");
		return XEResult::Fail;
	}

	/////////////////////////////////////////
	//Execute Call
	retInt = m_ASContext->Execute();
	if (retInt < 0)
	{
		XETODO("Add execute error code");
		return XEResult::Fail;
	}

	/////////////////////////////////////////
	//Unprepare Context
	m_ASContext->Unprepare();

	return XEResult::Ok;
}

XEResult GameObjectScriptGOC::ExecuteScriptConstantUpdate(const TimerParams& timerParams)
{
	/////////////////////////////////////////
	//Pre-checks
	if (!m_IsReady)
	{
		return XEResult::NotReady;
	}

	if (m_GameObjectScriptInstance == nullptr)
	{
		XETODO("set better return code");
		return XEResult::NullObj;
	}

	int retInt = 0;

	/////////////////////////////////////////
	//Prepare Context Constant Update
	retInt = m_ASContext->Prepare(m_GameObjectScript.m_ResourceAsset->GetGameObjectScriptFunctions()->m_ConstantUpdate);
	if (retInt < 0)
	{
		return XEResult::ASPrepareContextFailed;
	}

	retInt = m_ASContext->SetObject(m_GameObjectScriptInstance->m_ASObj);
	if (retInt < 0)
	{
		XETODO("Add set object error code");
		return XEResult::Fail;
	}

	void* parameter = (void*)(&timerParams);
	retInt = m_ASContext->SetArgObject(0, parameter);
	if (retInt < 0)
	{
		return XEResult::ASSetArgObjectFailed;
	}

	/////////////////////////////////////////
	//Execute Call
	retInt = m_ASContext->Execute();
	if (retInt < 0)
	{
		XETODO("Add execute error code");
		return XEResult::Fail;
	}

	/////////////////////////////////////////
	//Unprepare Context
	m_ASContext->Unprepare();

	return XEResult::Ok;
}

XEResult GameObjectScriptGOC::ExecuteScriptUpdate(const TimerParams& timerParams)
{
	/////////////////////////////////////////
	//Pre-checks
	if (!m_IsReady)
	{
		return XEResult::NotReady;
	}

	if (m_GameObjectScriptInstance == nullptr)
	{
		XETODO("set better return code");
		return XEResult::NullObj;
	}

	int retInt = 0;

	/////////////////////////////////////////
	//Prepare Context Update
	retInt = m_ASContext->Prepare(m_GameObjectScript.m_ResourceAsset->GetGameObjectScriptFunctions()->m_Update);
	if (retInt < 0)
	{
		return XEResult::ASPrepareContextFailed;
	}

	retInt = m_ASContext->SetObject(m_GameObjectScriptInstance->m_ASObj);
	if (retInt < 0)
	{
		XETODO("Add set object error code");
		return XEResult::Fail;
	}

	void* parameter = (void*)(&timerParams);
	retInt = m_ASContext->SetArgObject(0, parameter);
	if (retInt < 0)
	{
		return XEResult::ASSetArgObjectFailed;
	}

	/////////////////////////////////////////
	//Execute Call
	retInt = m_ASContext->Execute();
	if (retInt < 0)
	{
		XETODO("Add execute error code");
		return XEResult::Fail;
	}

	/////////////////////////////////////////
	//Unprepare Context
	m_ASContext->Unprepare();

	return XEResult::Ok;
}

XEResult GameObjectScriptGOC::ExecuteScriptPostUpdate(const TimerParams& timerParams)
{
	/////////////////////////////////////////
	//Pre-checks
	if (!m_IsReady)
	{
		return XEResult::NotReady;
	}

	if (m_GameObjectScriptInstance == nullptr)
	{
		XETODO("set better return code");
		return XEResult::NullObj;
	}

	int retInt = 0;

	/////////////////////////////////////////
	//Prepare Context Post Update
	retInt = m_ASContext->Prepare(m_GameObjectScript.m_ResourceAsset->GetGameObjectScriptFunctions()->m_PostUpdate);
	if (retInt < 0)
	{
		return XEResult::ASPrepareContextFailed;
	}

	retInt = m_ASContext->SetObject(m_GameObjectScriptInstance->m_ASObj);
	if (retInt < 0)
	{
		XETODO("Add set object error code");
		return XEResult::Fail;
	}

	void* parameter = (void*)(&timerParams);
	retInt = m_ASContext->SetArgObject(0, parameter);
	if (retInt < 0)
	{
		return XEResult::ASSetArgObjectFailed;
	}

	/////////////////////////////////////////
	//Execute Call
	retInt = m_ASContext->Execute();
	if (retInt < 0)
	{
		XETODO("Add execute error code");
		return XEResult::Fail;
	}

	/////////////////////////////////////////
	//Unprepare Context
	m_ASContext->Unprepare();

	return XEResult::Ok;
}
