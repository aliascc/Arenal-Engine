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

#ifndef _AUDIO_LISTENER_H
#define _AUDIO_LISTENER_H

/**********************
*   System Includes   *
***********************/
#include <stdint.h>

/*************************
*   3rd Party Includes   *
**************************/
#include "glm\glm.hpp"

/***************************
*   Game Engine Includes   *
****************************/
#include "Utils\Singleton.h"

/********************
*   Forward Decls   *
*********************/

/*****************
*   Class Decl   *
******************/

class AudioListener sealed : public Singleton<AudioListener>
{
    friend class Singleton<AudioListener>;

    private:

        /************************
        *   Private Variables   *
        *************************/
#pragma region Private Variables

#pragma endregion

        /***********************************************
        *   Private Constructor & Destructor Methods   *
        ************************************************/
#pragma region Private Constructor & Destructor Methods

        /// <summary>
        /// Default AudioListener Constructor
        /// </summary>
        AudioListener();

        /// <summary>
        /// Default AudioListener Destructor
        /// </summary>
        virtual ~AudioListener();

#pragma endregion

    public:

        /******************
        *   Get Methods   *
        *******************/
#pragma region Get Methods

        glm::vec3 GetPosition() const;

        glm::vec3 GetDirection() const;

        float GetGlobalVolume() const;

#pragma endregion

        /******************
        *   Set Methods   *
        *******************/
#pragma region Set Methods

        void SetPosition(const glm::vec3& pos);

        void SetGlobalVolume(float volume);

        void SetDirection(const glm::vec3& direction);

#pragma endregion

        /************************
        *   Framework Methods   *
        *************************/
#pragma region Framework Methods

#pragma endregion

};

#endif
