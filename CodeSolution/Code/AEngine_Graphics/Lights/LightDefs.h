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

#ifndef _LIGHT_DEFS_H
#define _LIGHT_DEFS_H

/**********************
*   System Includes   *
***********************/
#include <limits>
#include <stdint.h>
#include <d3d11_1.h>

/*************************
*   3rd Party Includes   *
**************************/
#include "glm\glm.hpp"

/***************************
*   Game Engine Includes   *
****************************/
#include "Base\Base.h"
#include "Color\Color.h"
#include "Utils\Frustum.h"
#include "Base\AEObject.h"
#include "Math\AEMathDefs.h"
#include "Color\AEColorDefs.h"

/************
*   Using   *
*************/

/**************
*   Defines   *
***************/

#undef max

#define AE_MAX_LIGHTS                                128

#define AE_LIGHT_SPOT_SHADOW_TEXTURE_HEIGHT          1024
#define AE_LIGHT_SPOT_SHADOW_TEXTURE_WIDTH           1024

#define AE_LIGHT_NUM_CASCADE_MAPS                    3

#define AE_LIGHT_DIR_SHADOW_TEXTURE_HEIGHT           2048
#define AE_LIGHT_DIR_SHADOW_TEXTURE_WIDTH            2048

#define AE_LIGHT_SHADOW_TEXTURE_FORMAT               DXGI_FORMAT_R32G32_FLOAT

#define AE_LIGHT_SHADOW_DISABLE_INDEX                std::numeric_limits<uint32_t>::max()

/****************
*   Constants   *
*****************/

/************
*   Enums   *
*************/
enum class LightType : uint32_t
{
    Invalid = 0,
    Spot,
    Omni,
    Directional
};

enum LightFXFlags
{
    AE_LFXF_None             = 0x00,
    AE_LFXF_Enabled          = 0x01,
    AE_LFXF_ShadowEnabled    = 0x02
};

/********************
*   Forward Decls   *
*********************/

/***************
*   Typedefs   *
****************/
typedef std::array<Frustum, AE_LIGHT_NUM_CASCADE_MAPS> CascadeFrustums;
typedef std::array<glm::mat4, AE_LIGHT_NUM_CASCADE_MAPS> CascadeViewMatrix;
typedef std::array<glm::mat4, AE_LIGHT_NUM_CASCADE_MAPS> CascadeProjectionMatrix;
typedef std::array<float, AE_LIGHT_NUM_CASCADE_MAPS + 1> CascadeDepths;
typedef std::array<glm::vec3, AE_LIGHT_NUM_CASCADE_MAPS> CascadeShadowPosition;

/*************
*   Struct   *
**************/
struct LightCascadeInfo
{
    CascadeFrustums m_CascadeFrustums;

    CascadeViewMatrix m_CascadeViewMatrix;

    CascadeProjectionMatrix m_CascadeProjectionMatrix;

    CascadeDepths m_CascadeDepths;

    CascadeShadowPosition m_CascadeShadowPosition;

    LightCascadeInfo();
};

struct LightCascadeInfoFX
{
    glm::mat4 m_CascadeViewProjectionMatrix[AE_LIGHT_NUM_CASCADE_MAPS];
    float m_CascadeDepths[AE_LIGHT_NUM_CASCADE_MAPS + 1];

    LightCascadeInfoFX();
};

struct LightFX
{
    glm::vec4 m_Color = AEColors::Transparent;

    float m_NearAttenuation = 0.0f;

    float m_FarAttenuation = 0.0f;

    float m_FallOffAngle = 0.0f;

    float m_Angle = 0.0f;

    glm::vec3 m_Position = AEMathHelpers::Vec3fZero;

    LightType m_LightType = LightType::Invalid;

    glm::vec3 m_Direction = AEMathHelpers::Vec3fZero;

    float m_Intensity = 0.0f;

    uint32_t m_LightFXFlags = 0x00;

    uint32_t m_ShadowTextureIndex = 0;

    LightFX();

    /// <summary>
    /// Not Equal Operator Overload
    /// </summary>
    /// <returns>True if they are not the same</returns>
    bool operator!=(const LightFX& other);

    /// <summary>
    /// Equal Operator Overload
    /// </summary>
    /// <returns>True if they are the same</returns>
    bool operator==(const LightFX& other);

};

struct SpotLightShadowInfoFX
{
    glm::mat4 m_ViewProjectionMatrix = AEMathHelpers::Mat4Identity;

    SpotLightShadowInfoFX();
};

#endif
