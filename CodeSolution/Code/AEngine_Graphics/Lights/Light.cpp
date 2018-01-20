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

/***************************
*   Game Engine Includes   *
****************************/
#include "Light.h"
#include "Utils\Frustum.h"
#include "GraphicDevice.h"
#include "Time\AETimeDefs.h"

//Always include last
#include "Memory\MemLeaks.h"

/********************
*   Function Defs   *
*********************/
Light::Light(LightType lightType, const std::wstring& name, const glm::vec3& position, const glm::vec3& direction, const Color& color, float nearAtteniation, float farAttenuation, float intensity, bool enable)
    : UniqueAEObjectNamed(name)
    , m_Position(position)
    , m_Direction(direction)
    , m_Color(color)
    , m_NearAttenuation(nearAtteniation)
    , m_FarAttenuation(farAttenuation)
    , m_Intensity(intensity)
    , m_Enabled(enable)
    , m_LightType(lightType)
{
    m_Frustum = new Frustum();
}

Light::~Light()
{
    DeleteMem(m_Frustum);
}

Light& Light::operator=(const Light& other)
{
    this->m_Position            = other.m_Position;
    this->m_Direction           = other.m_Direction;
    this->m_Color               = other.m_Color;
    this->m_NearAttenuation     = other.m_NearAttenuation;
    this->m_FarAttenuation      = other.m_FarAttenuation;
    this->m_Intensity           = other.m_Intensity;
    this->m_Enabled             = other.m_Enabled;
    this->m_ShadowEnabled       = other.m_ShadowEnabled;
    this->m_ShadowTextureIndex  = other.m_ShadowTextureIndex;

    return *this;
}

bool Light::operator!=(const LightFX& other)
{
    return ( !(*this == other) );
}

bool Light::operator==(const LightFX& other)
{
    return    (
                this->m_LightType           == other.m_LightType                                        &&
                this->m_Color               == other.m_Color                                            &&
                this->m_NearAttenuation     == other.m_NearAttenuation                                  &&
                this->m_FarAttenuation      == other.m_FarAttenuation                                   &&
                this->m_Position            == other.m_Position                                         &&
                this->m_Direction           == other.m_Direction                                        &&
                this->m_Intensity           == other.m_Intensity                                        &&
                this->m_Enabled             == ((other.m_LightFXFlags & AE_LFXF_Enabled) != 0)          &&
                this->m_ShadowEnabled       == ((other.m_LightFXFlags & AE_LFXF_ShadowEnabled) != 0)    &&
                this->m_ShadowTextureIndex  == other.m_ShadowTextureIndex
            );
}

void Light::UpdateLighFX(LightFX& lightFX)
{
    memset(&lightFX, 0, sizeof(LightFX));

    lightFX.m_LightType             = this->m_LightType;
    lightFX.m_Color                 = this->m_Color;
    lightFX.m_NearAttenuation       = this->m_NearAttenuation;
    lightFX.m_FarAttenuation        = this->m_FarAttenuation;
    lightFX.m_Position              = this->m_Position;
    lightFX.m_Direction             = this->m_Direction;
    lightFX.m_Intensity             = this->m_Intensity;
    lightFX.m_ShadowTextureIndex    = this->m_ShadowTextureIndex;
    
    lightFX.m_LightFXFlags          |= (this->m_Enabled ? AE_LFXF_Enabled : 0x00);
    lightFX.m_LightFXFlags          |= (this->m_ShadowEnabled ? AE_LFXF_ShadowEnabled : 0x00);
}
