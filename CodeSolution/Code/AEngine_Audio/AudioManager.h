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

#ifndef _AUDIO_MANAGER_H
#define _AUDIO_MANAGER_H

/**********************
*   System Includes   *
***********************/
#include <mutex>
#include <stdint.h>

/*************************
*   3rd Party Includes   *
**************************/

/***************************
*   Game Engine Includes   *
****************************/
#include "Base\AEObject.h"

/********************
*   Forward Decls   *
*********************/

/*****************
*   Class Decl   *
******************/

class AudioManager sealed : public AEObject
{
    private:

        /************************
        *   Private Variables   *
        *************************/
#pragma region Private Variables

        /// <summary>
        /// Defines if Listener is set or not 
        /// to a Game Object
        /// </summary>
        bool m_IsListenerSet = false;

        std::mutex m_AudioManagerMutex;

#pragma endregion

    public:

        /***************************************
        *   Constructor & Destructor Methods   *
        ****************************************/
#pragma region Constructor & Destructor Methods

        /// <summary>
        /// Default AudioManager Constructor
        /// </summary>
        AudioManager();

        /// <summary>
        /// Default AudioManager Destructor
        /// </summary>
        virtual ~AudioManager();

#pragma endregion

        /******************
        *   Get Methods   *
        *******************/
#pragma region Get Methods

        /// <summary>
        /// Gets if Listener is set to a Game Object
        /// </summary>
        inline bool IsListenerSet() const
        {
            return m_IsListenerSet;
        }

#pragma endregion

        /******************
        *   Set Methods   *
        *******************/
#pragma region Set Methods

        void SetIsListenerSet(bool isListenerSet)
        {
            m_IsListenerSet = isListenerSet;
        }

#pragma endregion

        /************************
        *   Framework Methods   *
        *************************/
#pragma region Framework Methods

#pragma endregion

};

#endif
