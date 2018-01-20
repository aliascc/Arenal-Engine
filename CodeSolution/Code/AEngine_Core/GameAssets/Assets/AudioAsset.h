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

#ifndef _AUDIO_ASSET_H
#define _AUDIO_ASSET_H

/**********************
*   System Includes   *
***********************/
#include <string>
#include <stdint.h>

/*************************
*   3rd Party Includes   *
**************************/

/***************************
*   Game Engine Includes   *
****************************/
#include "Base\Base.h"
#include "AudioDefs.h"
#include "GameAssets\GameAsset.h"

/********************
*   Forward Decls   *
*********************/
class Audio;
class AudioManager;

/*****************
*   Class Decl   *
******************/
class AudioAsset sealed : public GameAsset
{
    private:

        /************************
        *   Private Variables   *
        *************************/
#pragma region Private Variables

        Audio* m_Audio = nullptr;

        /// <summary>
        /// Audio Manager to handle the Sounds
        /// </summary>
        AudioManager* m_AudioManager = nullptr;

#pragma endregion

        /**********************
        *   Private Methods   *
        ***********************/
#pragma region Private Methods
        
        /// <summary>
        /// Loads an Asset Resource to Memory
        /// </summary>
        /// <returns>AEResult::OK if successful</returns>
        AEResult LoadAssetResource() override;

#pragma endregion

    public:

        /***************************************
        *   Constructor & Destructor Methods   *
        ****************************************/
#pragma region Constructor & Destructor Methods

        /// <summary>
        /// AudioAsset Constructor
        /// </summary>
        /// <param name="filePath">File Path of Game Asset to Load</param>
        /// <param name="gameResourceManager">Resource Manager to handle the Asset Resource</param>
        /// <param name="audioManager">Audio Manager to handle the Sounds</param>
        AudioAsset(const std::wstring& filepath, GameResourceManager* gameResourceManager, AudioManager* audioManager);

        /// <summary>
        /// Default AudioAsset Destructor
        /// </summary>
        virtual ~AudioAsset();

#pragma endregion

        /*******************
        *   Get Methods    *
        ********************/
#pragma region Get Methods

        Audio* GetAudioReference();

#pragma endregion

        /******************
        *   Set Methods   *
        *******************/
#pragma region Set Methods

#pragma endregion

        /************************
        *   Framework Methods   *
        *************************/
#pragma region Framework Methods

#pragma endregion

};

#endif
