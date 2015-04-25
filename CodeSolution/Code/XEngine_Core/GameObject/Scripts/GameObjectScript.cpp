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

/*************************
*   3rd Party Includes   *
**************************/

/***************************
*   Game Engine Includes   *
****************************/
#include "GameContentDefs.h"
#include "GameObjectScript.h"
#include "Base\BaseFunctions.h"
#include "AngelScript\AngelScriptDefs.h"
#include "AngelScript\AngelScriptManager.h"

//Always include last
#include "Memory\MemLeaks.h"

/********************
*   Function Defs   *
*********************/
GameObjectScript::GameObjectScript(const std::wstring& resourceName, AngelScriptManager* angelScriptManager)
	: GameResource(resourceName, GameResourceType::Animation)
	, m_AngelScriptManager(angelScriptManager)
{
	XEAssert(angelScriptManager);
}

GameObjectScript::~GameObjectScript()
{
	CleanUp();
}

void GameObjectScript::CleanUp()
{
	if (m_GameObjectScriptFunctions != nullptr)
	{
		DeleteMem(m_GameObjectScriptFunctions);

		m_AngelScriptManager->RemoveModule(m_Name);
	}
}

XEResult GameObjectScript::Load()
{
	/////////////////////////////////////////
	//Pre-checks
	if (m_AngelScriptManager == nullptr)
	{
		XETODO("Add error for AS is null");
		return XEResult::NullObj;
	}

	std::lock_guard<std::mutex> lock(m_GameResourceMutex);

	XEAssert(!m_FileName.empty());
	if (m_FileName.empty())
	{
		return XEResult::EmptyFilename;
	}

	XEAssert(!m_Name.empty());
	if (m_Name.empty())
	{
		return XEResult::EmptyName;
	}

	/////////////////////////////////////////
	//Remove Memory from old script
	CleanUp();

	/////////////////////////////////////////
	//Variables
	int retInt = 0;
	XEResult ret = XEResult::Ok;
	asIScriptModule* asModule = nullptr;

	/////////////////////////////////////////
	//Load scripts
	ret = m_AngelScriptManager->LoadScript(m_FileName, m_Name, &asModule);
	if (ret != XEResult::Ok)
	{
		return ret;
	}

	if (asModule->GetObjectTypeCount() == 0)
	{
		m_AngelScriptManager->RemoveModule(m_Name);

		XETODO("set correct error code");
		return XEResult::Fail;
	}

	/////////////////////////////////////////
	//Verify that Game Object Script Interface appears
	asIObjectType* objType = asModule->GetObjectTypeByIndex(0);

	bool interfaceFound = false;
	uint32_t interfaceCount = objType->GetInterfaceCount();
	for (uint32_t i = 0; i < interfaceCount; i++)
	{
		if (strcmp(objType->GetInterface(i)->GetName(), XE_GOS_INTERFACE_NAME) == 0)
		{
			interfaceFound = true;
			i = interfaceCount;
		}
	}

	if (!interfaceFound)
	{
		m_AngelScriptManager->RemoveModule(m_Name);

		XETODO("set correct error code");
		XETODO("log error");
		return XEResult::Fail;
	}

	/////////////////////////////////////////
	//Get Interface Functions
	m_GameObjectScriptFunctions = new GameObjectScriptFunctions();

	m_GameObjectScriptFunctions->m_Constructor		= objType->GetFactoryByIndex(0);
	m_GameObjectScriptFunctions->m_Initialize		= objType->GetMethodByDecl(XE_GOS_INIT_FUNC);
	m_GameObjectScriptFunctions->m_ConstantUpdate	= objType->GetMethodByDecl(XE_GOS_CONSTANT_UPDATE_FUNC);
	m_GameObjectScriptFunctions->m_Update			= objType->GetMethodByDecl(XE_GOS_UPDATE_FUNC);
	m_GameObjectScriptFunctions->m_PostUpdate		= objType->GetMethodByDecl(XE_GOS_POST_UPDATE_FUNC);

	m_GameObjectScriptFunctions->m_Constructor->AddRef();
	m_GameObjectScriptFunctions->m_Initialize->AddRef();
	m_GameObjectScriptFunctions->m_ConstantUpdate->AddRef();
	m_GameObjectScriptFunctions->m_Update->AddRef();
	m_GameObjectScriptFunctions->m_PostUpdate->AddRef();

	return XEResult::Ok;
}