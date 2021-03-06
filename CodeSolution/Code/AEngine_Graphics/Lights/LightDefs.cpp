/*
* Copyright (c) 2018 <Carlos Chac�n>
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
#include "precomp_graphics.h"

/**********************
*   System Includes   *
***********************/

/*************************
*   3rd Party Includes   *
**************************/

/***************************
*   Game Engine Includes   *
****************************/
#include "Light.h"
#include "LightDefs.h"

//Always include last
#include "Memory\MemLeaks.h"

/********************
*   Function Defs   *
*********************/

/***********************
*   LightCascadeInfo   *
************************/
LightCascadeInfo::LightCascadeInfo()
{
    std::fill(m_CascadeViewMatrix.begin(), m_CascadeViewMatrix.end(), AEMathHelpers::Mat4Identity);
    std::fill(m_CascadeProjectionMatrix.begin(), m_CascadeProjectionMatrix.end(), AEMathHelpers::Mat4Identity);
    std::fill(m_CascadeDepths.begin(), m_CascadeDepths.end(), 0.0f);
    std::fill(m_CascadeShadowPosition.begin(), m_CascadeShadowPosition.end(), AEMathHelpers::Vec3fZero);
}

/*************************
*   LightCascadeInfoFX   *
**************************/
LightCascadeInfoFX::LightCascadeInfoFX()
{
    memset(m_CascadeViewProjectionMatrix, 0, sizeof(glm::mat4) * AE_LIGHT_NUM_CASCADE_MAPS);
    memset(m_CascadeDepths, 0, sizeof(float) * AE_LIGHT_NUM_CASCADE_MAPS);
}

/**************
*   LightFX   *
***************/
LightFX::LightFX()
{
}

bool LightFX::operator!=(const LightFX& other)
{
    return ( !(*this == other) );
}

bool LightFX::operator==(const LightFX& other)
{
    return    (
                this->m_LightType           == other.m_LightType            &&
                this->m_Color               == other.m_Color                &&
                this->m_NearAttenuation     == other.m_NearAttenuation      &&
                this->m_FarAttenuation      == other.m_FarAttenuation       &&
                this->m_Position            == other.m_Position             &&
                this->m_Direction           == other.m_Direction            &&
                this->m_Intensity           == other.m_Intensity            &&
                this->m_Angle               == other.m_Angle                &&
                this->m_FallOffAngle        == other.m_FallOffAngle         &&
                this->m_LightFXFlags        == other.m_LightFXFlags         &&
                this->m_ShadowTextureIndex  == other.m_ShadowTextureIndex
            );
}

/****************************
*   SpotLightShadowInfoFX   *
*****************************/
SpotLightShadowInfoFX::SpotLightShadowInfoFX()
{
}
