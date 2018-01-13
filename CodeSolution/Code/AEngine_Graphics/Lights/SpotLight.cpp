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
#include "SpotLight.h"
#include "Utils\Frustum.h"

//Always include last
#include "Memory\MemLeaks.h"

/********************
*   Function Defs   *
*********************/
SpotLight::SpotLight(const std::wstring& name, const glm::vec3& position, const glm::vec3& direction, const Color& color, float nearAtteniation, float farAttenuation, float intensity, bool enabled, float fallOffAngle, float angle)
	: Light(LightType::Spot, name, position, direction, color, farAttenuation, nearAtteniation, intensity, enabled)
	, m_FallOffAngle(fallOffAngle)
	, m_Angle(angle)
{
}

SpotLight::~SpotLight()
{
}

void SpotLight::UpdateLightMatrices()
{
	/////////////////////////////////////////////////
	//Update View Matrix
	m_ViewMatrix = glm::lookAt(m_Position, m_Position + m_Direction, AEMathHelpers::Vec3fUp);

	/////////////////////////////////////////////////
	//Update Projection Matrix
	float aspectRatio = (float)AE_LIGHT_SPOT_SHADOW_TEXTURE_WIDTH / (float)AE_LIGHT_SPOT_SHADOW_TEXTURE_WIDTH;

	m_ProjectionMatrix = glm::perspective(glm::radians(m_FallOffAngle), aspectRatio, 1.0f, m_FarAttenuation);

	/////////////////////////////////////////////////
	//Update Light Frustum
	m_Frustum->UpdateFrustum(m_ViewMatrix, m_ProjectionMatrix);
}

Light& SpotLight::operator=(const Light& other)
{
	if(other.GetLightType() == LightType::Spot)
	{
		const SpotLight* spotLight = reinterpret_cast<const SpotLight*>(&other);

		this->m_Angle			= spotLight->m_Angle;
		this->m_FallOffAngle	= spotLight->m_FallOffAngle;
	}

	return Light::operator=(other);
}

bool SpotLight::operator==(const LightFX& other)
{
	return	(
				this->m_Angle				== other.m_Angle			&&
				this->m_FallOffAngle		== other.m_FallOffAngle		&&
				Light::operator==(other)
			);
}

void SpotLight::UpdateLighFX(LightFX& lightFX)
{
	Light::UpdateLighFX(lightFX);

	lightFX.m_Angle				= this->m_Angle;
	lightFX.m_FallOffAngle		= this->m_FallOffAngle;
}
