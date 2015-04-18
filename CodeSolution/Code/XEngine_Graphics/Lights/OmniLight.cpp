/********************************************************
*
* Author: Carlos Chacón N.
*
* Created: 3/22/2012
*
* Desc:
*
*
* Copyright 2012
*
*********************************************************/

/**********************
*   System Includes   *
***********************/

/***************************
*   Game Engine Includes   *
****************************/
#include "OmniLight.h"

//Always include last
#include "Memory\MemLeaks.h"

/********************
*   Function Defs   *
*********************/
OmniLight::OmniLight(const std::wstring& name, const glm::vec3& position, const glm::vec3& direction, const Color& color, float nearAtteniation, float farAttenuation, float intensity, bool enabled)
	: Light(LightType::Omni, name, position, direction, color, farAttenuation, nearAtteniation, intensity, enabled)
{
}

OmniLight::~OmniLight()
{
}

void OmniLight::UpdateLightMatrices()
{
}
