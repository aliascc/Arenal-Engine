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
#include "Base\BaseFunctions.h"
#include "GameObjectScriptAsset.h"
#include "Resource\GameResourceManager.h"
#include "GameObject\Scripts\GameObjectScript.h"

//Always include last
#include "Memory\MemLeaks.h"

/********************
*   Function Defs   *
*********************/
XETODO("Check if this class needs a mutex");
GameObjectScriptAsset::GameObjectScriptAsset(const std::wstring& filePath, GameResourceManager* gameResourceManager, AngelScriptManager* angelScriptManager)
	: GameAsset(GameContentType::GameObjectScript, filePath, gameResourceManager)
	, m_AngelScriptManager(angelScriptManager)
{
	XEAssert(m_AngelScriptManager != nullptr);
}

GameObjectScriptAsset::~GameObjectScriptAsset()
{
	XERelease(m_GameObjectScript);
}

GameObjectScript* GameObjectScriptAsset::GetGameObjectScriptReference()
{
	if(m_GameObjectScript == nullptr)
	{
		return nullptr;
	}

	return reinterpret_cast<GameObjectScript*>(m_GameObjectScript->AddRef());
}

XEResult GameObjectScriptAsset::LoadAssetResource()
{
	XEAssert(m_AngelScriptManager != nullptr);
	if (m_AngelScriptManager == nullptr)
	{
		XETODO("add error for AS NULL");
		return XEResult::NullObj;
	}

	XEAssert(m_GameResourceManager != nullptr);
	if (m_GameResourceManager == nullptr)
	{
		return XEResult::GameResourceManagerNull;
	}

	XEAssert(!m_FilePath.empty());
	if(m_FilePath.empty())
	{
		return XEResult::EmptyFilename;
	}

	XEResult ret = XEResult::Ok;

	XETODO("Look into what happens if file changes location");

	if (m_GameObjectScript != nullptr)
	{
		XETODO("Check return value");
		ret = m_GameObjectScript->Load();

		return ret;
	}
	else
	{
		/////////////////////////////////////////////
		//Check if Game Resources contains this Asset
		m_GameObjectScript = (GameObjectScript*)m_GameResourceManager->AcquireGameResourceByStringID(m_FilePath, GameResourceType::GameObjectScript);

		if (m_GameObjectScript != nullptr)
		{
			XETODO("Check if we always need to reload");
			XETODO("Check return value");
			m_GameObjectScript->Load();

			return XEResult::Ok;
		}

		std::wstring scriptName = XE_Base::GetFilenameOnly(m_FilePath);
		m_GameObjectScript = new GameObjectScript(scriptName, m_AngelScriptManager);

		m_GameObjectScript->SetFileName(m_FilePath);

		if (m_GameObjectScript->Load() != XEResult::Ok)
		{
			XERelease(m_GameObjectScript);
			return XEResult::LoadFileFailed;
		}

		/////////////////////////////////////////////
		//Add to Resource Manager
		ret = m_GameResourceManager->ManageGameResource(m_GameObjectScript, m_FilePath);
		if(ret != XEResult::Ok)
		{
			XETODO("Add log");

			XERelease(m_GameObjectScript);

			return XEResult::ResourceManagedFailed;
		}
	}

	return XEResult::Ok;
}