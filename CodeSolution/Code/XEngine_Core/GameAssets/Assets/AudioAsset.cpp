
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

/*************************
*   3rd Party Includes   *
**************************/

/***************************
*   Game Engine Includes   *
****************************/
#include "Audio.h"
#include "AudioAsset.h"
#include "AudioManager.h"
#include "Base\BaseFunctions.h"
#include "Resource\GameResourceManager.h"

//Always include last
#include "Memory\MemLeaks.h"

/********************
*   Function Defs   *
*********************/

XETODO("Check if this class needs a mutex");
AudioAsset::AudioAsset(const std::wstring& filePath, GameResourceManager* gameResourceManager, AudioManager* audioManager)
	: GameAsset(GameContentType::Audio, filePath, gameResourceManager)
	, m_AudioManager(audioManager)
{
	XEAssert(m_AudioManager != nullptr);
}

AudioAsset::~AudioAsset()
{
	XERelease(m_Audio);
}

Audio* AudioAsset::GetAudioReference()
{
	if (m_Audio == nullptr)
	{
		return nullptr;
	}

	return reinterpret_cast<Audio*>(m_Audio->AddRef());
}

XEResult AudioAsset::LoadAssetResource()
{
	XEAssert(m_AudioManager != nullptr);
	if (m_AudioManager == nullptr)
	{
		XETODO("add error for Audio Manager NULL");
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

	if (m_Audio != nullptr)
	{
		XETODO("Check return value");
		ret = m_Audio->Load();

		return ret;
	}
	else
	{
		/////////////////////////////////////////////
		//Check if Game Resource Manager contains this Audio
		m_Audio = (Audio*)m_GameResourceManager->AcquireGameResourceByStringID(m_FilePath, GameResourceType::Audio);

		if (m_Audio != nullptr)
		{
			XETODO("Check if we always need to reload");
			XETODO("Check return value");
			m_Audio->Load();

			return XEResult::Ok;
		}

		/////////////////////////////////////////////
		//Create Resource
		m_Audio = new Audio(m_Name);

		/////////////////////////////////////////////
		//Load Resource
		ret = m_Audio->LoadFile(m_FilePath);
		if (ret != XEResult::Ok)
		{
			XETODO("Add log");

			XERelease(m_Audio);

			return XEResult::LoadFileFailed;
		}

		/////////////////////////////////////////////
		//Add to Resource Manager
		ret = m_GameResourceManager->ManageGameResource(m_Audio, m_FilePath);
		if (ret != XEResult::Ok)
		{
			XETODO("Add log");

			XERelease(m_Audio);

			return XEResult::ResourceManagedFailed;
		}
	}

	return XEResult::Ok;
}