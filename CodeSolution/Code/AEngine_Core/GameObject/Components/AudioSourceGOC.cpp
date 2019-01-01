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

/*************************
*   Precompiled Header   *
**************************/
#include "precomp_core.h"

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
#include "GameAssets\GameAsset.h"
#include "GameObject\GameObject.h"
#include "GameAssets\GameAssetDefs.h"
#include "GameAssets\Assets\AudioAsset.h"

//Always include last
#include "Memory\MemLeaks.h"

/********************
*   Function Defs   *
*********************/
AETODO("Add mutex");
AudioSourceGOC::AudioSourceGOC(GameObject& gameObject, const std::string& name)
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
    AEAssert(asset != nullptr);
    if (asset == nullptr)
    {
        return;
    }

    AEAssert(asset->GetGameContentType() == GameContentType::Audio);
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
    AEAssert(asset != nullptr);
    if (asset == nullptr)
    {
        return;
    }

    AEAssert(asset->GetGameContentType() == GameContentType::Audio);
    if (asset->GetGameContentType() != GameContentType::Audio)
    {
        return;
    }

    m_AudioPlayer.StopAndUninitialize();
}

void AudioSourceGOC::AudioAssetReload(GameAsset* asset)
{
    AEAssert(asset != nullptr);
    if (asset == nullptr)
    {
        return;
    }

    AEAssert(asset->GetGameContentType() == GameContentType::Audio);
    if (asset->GetGameContentType() != GameContentType::Audio)
    {
        return;
    }

    const uint8_t* audioData = m_Audio.m_ResourceAsset->GetAudioData();
    uint32_t audioDataSize = m_Audio.m_ResourceAsset->GetAudioDataSize();

    AETODO("Check return");
    m_AudioPlayer.Initialize(audioData, audioDataSize);
}

AEResult AudioSourceGOC::SetAudioAsset(AudioAsset* asset)
{
    AEAssert(asset != nullptr);
    if (asset == nullptr)
    {
        return AEResult::NullParameter;
    }

    if (asset->GetGameContentType() != GameContentType::Audio)
    {
        return AEResult::InvalidObjType;
    }

    if (m_Audio.m_ResourceAsset != nullptr)
    {
        AEResult ret = RemoveAudioAsset();

        if (ret != AEResult::Ok)
        {
            AETODO("Log error");

            AETODO("Add better return code");
            return AEResult::Fail;
        }
    }

    uint64_t callerID = AE_Base::GetNextUniqueID();

    AEResult ret = asset->RegisterEventHandlers(callerID, 
                                                std::bind(&AudioSourceGOC::AudioAssetDeletion,    this, std::placeholders::_1),
                                                std::bind(&AudioSourceGOC::AudioAssetReload,    this, std::placeholders::_1),
                                                std::bind(&AudioSourceGOC::AudioAssetPreReload,    this, std::placeholders::_1));
    if (ret != AEResult::Ok)
    {
        AETODO("Log error");

        return ret;
    }

    m_Audio.m_AssetID                       = asset->GetUniqueAssetID();
    m_Audio.m_GameAsset                     = asset;
    m_Audio.m_CallerID                      = callerID;
    m_Audio.m_ResourceAsset                 = asset->GetAudioReference();
    m_Audio.m_OnListenerObjDeletionEvent    = asset->GetOnListenerObjDeletionEventHandler();

    const uint8_t* audioData = m_Audio.m_ResourceAsset->GetAudioData();
    uint32_t audioDataSize = m_Audio.m_ResourceAsset->GetAudioDataSize();

    AETODO("Check return");
    m_AudioPlayer.Initialize(audioData, audioDataSize);

    return AEResult::Ok;
}

AEResult AudioSourceGOC::RemoveAudioAsset()
{
    return ClearAudioAsset(true);
}

AEResult AudioSourceGOC::ClearAudioAsset(bool informGameAsset)
{
    if (m_Audio.m_ResourceAsset != nullptr)
    {
        if (m_Audio.m_OnListenerObjDeletionEvent != nullptr && informGameAsset)
        {
            m_Audio.m_OnListenerObjDeletionEvent(m_Audio.m_CallerID);
        }

        m_Audio.m_AssetID                       = 0;
        m_Audio.m_GameAsset                     = nullptr;
        m_Audio.m_CallerID                      = 0;
        m_Audio.m_OnListenerObjDeletionEvent    = nullptr;

        AERelease(m_Audio.m_ResourceAsset);

        m_AudioPlayer.StopAndUninitialize();
    }

    return AEResult::Ok;
}

std::string AudioSourceGOC::GetAudioName() const
{
    if (m_Audio.m_ResourceAsset != nullptr)
    {
        return m_Audio.m_ResourceAsset->GetName();
    }
    else
    {
        return "";
    }
}
