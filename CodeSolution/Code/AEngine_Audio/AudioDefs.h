/** \file
* Contains definitions for handling Audio
*/

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

#ifndef _AUDIO_DEFS_H
#define _AUDIO_DEFS_H

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
#include "Base\AEObject.h"
#include "Math\AEMathDefs.h"

/********************
*   Forward Decls   *
*********************/

/**************
*   Defines   *
***************/

/************
*   Enums   *
*************/
enum class AudioType : uint32_t
{
    Sound = 0,
    Music
};

/**************
*   Typedef   *
***************/

/******************
*   Struct Decl   *
*******************/

struct AudioPlayerState sealed : public AEObject
{
    float m_Volume = 100.0f;

    float m_Attenuation = 1.0f;

    float m_MinDistance = 1.0f;

    float m_Pitch = 1.0f;

    glm::vec3 m_Position = AEMathHelpers::Vec3fZero;

    bool m_Loop = false;

    bool m_Is3D = false;

    AudioPlayerState()
    {
    }
};

#endif
