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
#include "AudioSourceGOC.h"
#include "Base\BaseFunctions.h"
#include "GameAssets\GameAsset.h"
#include "GameObject\GameObject.h"
#include "GameAssets\GameAssetDefs.h"
#include "GameAssets\Assets\AudioAsset.h"

//Always include last
#include "Memory\MemLeaks.h"

/********************
*   Function Defs   *
*********************/
XETODO("Add mutex");
AudioSourceGOC::AudioSourceGOC(GameObject* gameObject, const std::wstring& name)
	: GameObjectComponent(gameObject, GameObjectComponentType::AudioSource)
	, m_Name(name)
{
}

AudioSourceGOC::~AudioSourceGOC()
{
	RemoveAudioAsset();
}

void AudioSourceGOC::AudioAssetDeletion(GameAsset* asset)
{
	XEAssert(asset != nullptr);
	if (asset == nullptr)
	{
		return;
	}

	XEAssert(asset->GetGameContentType() == GameContentType::Audio);
	if (asset->GetGameContentType() != GameContentType::Audio)
	{
		return;
	}

	if (m_Audio.m_AssetID != asset->GetUniqueAssetID())
	{
		return;
	}

	ClearAudioAsset(false);
}

void AudioSourceGOC::AudioAssetPreReload(GameAsset* asset)
{
	XEAssert(asset != nullptr);
	if (asset == nullptr)
	{
		return;
	}

	XEAssert(asset->GetGameContentType() == GameContentType::Audio);
	if (asset->GetGameContentType() != GameContentType::Audio)
	{
		return;
	}

	m_AudioPlayer.StopAndUninitialize();
}

void AudioSourceGOC::AudioAssetReload(GameAsset* asset)
{
	XEAssert(asset != nullptr);
	if (asset == nullptr)
	{
		return;
	}

	XEAssert(asset->GetGameContentType() == GameContentType::Audio);
	if (asset->GetGameContentType() != GameContentType::Audio)
	{
		return;
	}

	const uint8_t* audioData = m_Audio.m_ResourceAsset->GetAudioData();
	uint32_t audioDataSize = m_Audio.m_ResourceAsset->GetAudioDataSize();

	XETODO("Check return");
	m_AudioPlayer.Initialize(audioData, audioDataSize);
}

XEResult AudioSourceGOC::SetAudioAsset(AudioAsset* asset)
{
	XEAssert(asset != nullptr);
	if (asset == nullptr)
	{
		return XEResult::NullParameter;
	}

	if (asset->GetGameContentType() != GameContentType::Audio)
	{
		return XEResult::InvalidObjType;
	}

	if (m_Audio.m_ResourceAsset != nullptr)
	{
		XEResult ret = RemoveAudioAsset();

		if (ret != XEResult::Ok)
		{
			XETODO("Log error");

			XETODO("Add better return code");
			return XEResult::Fail;
		}
	}

	uint64_t callerID = XE_Base::GetNextUniqueID();

	XEResult ret = asset->RegisterEventHandlers(callerID, 
												std::bind(&AudioSourceGOC::AudioAssetDeletion,	this, std::placeholders::_1),
												std::bind(&AudioSourceGOC::AudioAssetReload,	this, std::placeholders::_1),
												std::bind(&AudioSourceGOC::AudioAssetPreReload,	this, std::placeholders::_1));
	if (ret != XEResult::Ok)
	{
		XETODO("Log error");

		return ret;
	}

	m_Audio.m_AssetID						= asset->GetUniqueAssetID();
	m_Audio.m_GameAsset						= asset;
	m_Audio.m_CallerID						= callerID;
	m_Audio.m_ResourceAsset					= asset->GetAudioReference();
	m_Audio.m_OnListenerObjDeletionEvent	= asset->GetOnListenerObjDeletionEventHandler();

	const uint8_t* audioData = m_Audio.m_ResourceAsset->GetAudioData();
	uint32_t audioDataSize = m_Audio.m_ResourceAsset->GetAudioDataSize();

	XETODO("Check return");
	m_AudioPlayer.Initialize(audioData, audioDataSize);

	return XEResult::Ok;
}

XEResult AudioSourceGOC::RemoveAudioAsset()
{
	return ClearAudioAsset(true);
}

XEResult AudioSourceGOC::ClearAudioAsset(bool informGameAsset)
{
	if (m_Audio.m_ResourceAsset != nullptr)
	{
		if (m_Audio.m_OnListenerObjDeletionEvent != nullptr && informGameAsset)
		{
			m_Audio.m_OnListenerObjDeletionEvent(m_Audio.m_CallerID);
		}

		m_Audio.m_AssetID						= 0;
		m_Audio.m_GameAsset						= nullptr;
		m_Audio.m_CallerID						= 0;
		m_Audio.m_OnListenerObjDeletionEvent	= nullptr;

		XERelease(m_Audio.m_ResourceAsset);

		m_AudioPlayer.StopAndUninitialize();
	}

	return XEResult::Ok;
}

std::wstring AudioSourceGOC::GetAudioName() const
{
	if (m_Audio.m_ResourceAsset != nullptr)
	{
		return m_Audio.m_ResourceAsset->GetName();
	}
	else
	{
		return L"";
	}
}
