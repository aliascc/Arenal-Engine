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

#pragma once

#ifndef _AUDIO_SOUND_SOURCE_GOC_H
#define _AUDIO_SOUND_SOURCE_GOC_H

/**********************
*   System Includes   *
***********************/

/*************************
*   3rd Party Includes   *
**************************/

/***************************
*   Game Engine Includes   *
****************************/
#include "Base\Base.h"
#include "AudioPlayer.h"
#include "GameObject\GameObjectDefs.h"
#include "GameObject\GameObjectComponent.h"

/********************
*   Forward Decls   *
*********************/
class Audio;
class GameAsset;
class GameObject;
class AudioAsset;

/*****************
*   Class Decl   *
******************/

class AudioSourceGOC sealed : public GameObjectComponent
{
    private:

        /************************
        *   Private Variables   *
        *************************/
#pragma region Private Variables

        AudioPlayer m_AudioPlayer;

        std::wstring m_Name = L"";

        /// <summary>
        /// Game Object Pair with Audio Sound that this Game Object Component uses
        /// </summary>
        GameObjectAssetPair<Audio> m_Audio;

        /// <summary>
        /// Is Ready flag that determines if the object 
        /// is initialize and ready to run.
        /// </summary>
        bool m_IsReady = false;

#pragma endregion

        /**********************
        *   Private Methods   *
        ***********************/
#pragma region Private Methods

        void CleanUp();

        void AudioAssetDeletion(GameAsset* asset);

        void AudioAssetReload(GameAsset* asset);

        void AudioAssetPreReload(GameAsset* asset);

        AEResult ClearAudioAsset(bool informAsset = true);

#pragma endregion

    public:

        /***************************************
        *   Constructor & Destructor Methods   *
        ****************************************/
#pragma region Constructor & Destructor Methods

        /// <summary>
        /// AudioSourceGOC Constructor
        /// </summary>
        /// <param name="gameObject">Game Object that this Component is attached too</param>
        /// <param name="name">Name of the Audio.</param>
        AudioSourceGOC(GameObject* gameObject, const std::wstring& name);

        /// <summary>
        /// Default AudioSourceGOC Destructor
        /// </summary>
        virtual ~AudioSourceGOC();

#pragma endregion

        /******************
        *   Get Methods   *
        *******************/
#pragma region Get Methods

        inline const std::wstring& GetName() const
        {
            return m_Name;
        }

        inline AudioPlayer* GetAudioPlayer()
        {
            return &m_AudioPlayer;
        }

#pragma endregion

        /******************
        *   Set Methods   *
        *******************/
#pragma region Set Methods

        inline void SetName(const std::wstring& name)
        {
            m_Name = name;
        }

        inline uint64_t GetAudioAssetID() const
        {
            return m_Audio.m_AssetID;
        }

        std::wstring GetAudioName() const;

        AEResult SetAudioAsset(AudioAsset* asset);

#pragma endregion
        
        /************************
        *   Framework Methods   *
        *************************/
#pragma region Framework Methods

        AEResult RemoveAudioAsset();

#pragma endregion

};

#endif
