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

AETODO("Check if this class needs a mutex");
AudioAsset::AudioAsset(const std::wstring& filePath, GameResourceManager* gameResourceManager, AudioManager* audioManager)
    : GameAsset(GameContentType::Audio, filePath, gameResourceManager)
    , m_AudioManager(audioManager)
{
    AEAssert(m_AudioManager != nullptr);
}

AudioAsset::~AudioAsset()
{
    AERelease(m_Audio);
}

Audio* AudioAsset::GetAudioReference()
{
    if (m_Audio == nullptr)
    {
        return nullptr;
    }

    return reinterpret_cast<Audio*>(m_Audio->AddRef());
}

AEResult AudioAsset::LoadAssetResource()
{
    AEAssert(m_AudioManager != nullptr);
    if (m_AudioManager == nullptr)
    {
        AETODO("add error for Audio Manager NULL");
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

    if (m_Audio != nullptr)
    {
        AETODO("Check return value");
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
            AETODO("Check if we always need to reload");
            AETODO("Check return value");
            m_Audio->Load();

            return AEResult::Ok;
        }

        /////////////////////////////////////////////
        //Create Resource
        m_Audio = new Audio(m_Name);

        /////////////////////////////////////////////
        //Load Resource
        ret = m_Audio->LoadFile(m_FilePath);
        if (ret != AEResult::Ok)
        {
            AETODO("Add log");

            AERelease(m_Audio);

            return AEResult::LoadFileFailed;
        }

        /////////////////////////////////////////////
        //Add to Resource Manager
        ret = m_GameResourceManager->ManageGameResource(m_Audio, m_FilePath);
        if (ret != AEResult::Ok)
        {
            AETODO("Add log");

            AERelease(m_Audio);

            return AEResult::ResourceManagedFailed;
        }
    }

    return AEResult::Ok;
}
