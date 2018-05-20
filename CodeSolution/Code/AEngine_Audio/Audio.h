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

#ifndef _AUDIO_H
#define _AUDIO_H

/**********************
*   System Includes   *
***********************/

/*************************
*   3rd Party Includes   *
**************************/

/***************************
*   Game Engine Includes   *
****************************/
#include "Resource\GameResource.h"

/********************
*   Forward Decls   *
*********************/

/*****************
*   Class Decl   *
******************/

class Audio sealed : public GameResource
{
    protected:

        /**************************
        *   Protected Variables   *
        ***************************/
#pragma region Private Variables

        /// <summary>
        /// Audio Data Loaded into memory
        /// </summary>
        uint8_t* m_AudioData = nullptr;

        /// <summary>
        /// Size of Audio Data
        /// </summary>
        uint32_t m_AudioDataSize = 0;

#pragma endregion

    public:

        /***************************************
        *   Constructor & Destructor Methods   *
        ****************************************/
#pragma region Constructor & Destructor Methods

        /// <summary>
        /// Default Audio Constructor
        /// </summary>
        /// <param name="audioName">Audio Name</param>
        Audio(const std::string& audioName);

        /// <summary>
        /// Default Audio Destructor
        /// </summary>
        virtual ~Audio();

#pragma endregion

        /******************
        *   Get Methods   *
        *******************/
#pragma region Get Methods

        inline const uint8_t* GetAudioData() const
        {
            return m_AudioData;
        }

        inline const uint32_t GetAudioDataSize() const
        {
            return m_AudioDataSize;
        }

#pragma endregion

        /******************
        *   Set Methods   *
        *******************/
#pragma region Set Methods

#pragma endregion

        /********************************
        *   Virtual Framework Methods   *
        *********************************/
#pragma region Virtual Framework Methods

#pragma endregion

        /************************
        *   Framework Methods   *
        *************************/
#pragma region Framework Methods

        AEResult LoadFile(const std::string& file);

        AEResult Load() override;

#pragma endregion

};

#endif
