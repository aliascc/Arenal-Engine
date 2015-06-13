/********************************************************
*
* Author: Carlos Chac�n N.
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
AETODO("Check if this class needs a mutex");
GameObjectScriptAsset::GameObjectScriptAsset(const std::wstring& filePath, GameResourceManager* gameResourceManager, AngelScriptManager* angelScriptManager)
	: GameAsset(GameContentType::GameObjectScript, filePath, gameResourceManager)
	, m_AngelScriptManager(angelScriptManager)
{
	AEAssert(m_AngelScriptManager != nullptr);
}

GameObjectScriptAsset::~GameObjectScriptAsset()
{
	AERelease(m_GameObjectScript);
}

GameObjectScript* GameObjectScriptAsset::GetGameObjectScriptReference()
{
	if(m_GameObjectScript == nullptr)
	{
		return nullptr;
	}

	return reinterpret_cast<GameObjectScript*>(m_GameObjectScript->AddRef());
}

AEResult GameObjectScriptAsset::LoadAssetResource()
{
	AEAssert(m_AngelScriptManager != nullptr);
	if (m_AngelScriptManager == nullptr)
	{
		AETODO("add error for AS NULL");
		return AEResult::NullObj;
	}

	AEAssert(m_GameResourceManager != nullptr);
	if (m_GameResourceManager == nullptr)
	{
		return AEResult::GameResourceManagerNull;
	}

	AEAssert(!m_FilePath.empty());
	if(m_FilePath.empty())
	{
		return AEResult::EmptyFilename;
	}

	AEResult ret = AEResult::Ok;

	AETODO("Look into what happens if file changes location");

	if (m_GameObjectScript != nullptr)
	{
		AETODO("Check return value");
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
			AETODO("Check if we always need to reload");
			AETODO("Check return value");
			m_GameObjectScript->Load();

			return AEResult::Ok;
		}

		std::wstring scriptName = AE_Base::GetFilenameOnly(m_FilePath);
		m_GameObjectScript = new GameObjectScript(scriptName, m_AngelScriptManager);

		m_GameObjectScript->SetFileName(m_FilePath);

		if (m_GameObjectScript->Load() != AEResult::Ok)
		{
			AERelease(m_GameObjectScript);
			return AEResult::LoadFileFailed;
		}

		/////////////////////////////////////////////
		//Add to Resource Manager
		ret = m_GameResourceManager->ManageGameResource(m_GameObjectScript, m_FilePath);
		if(ret != AEResult::Ok)
		{
			AETODO("Add log");

			AERelease(m_GameObjectScript);

			return AEResult::ResourceManagedFailed;
		}
	}

	return AEResult::Ok;
}