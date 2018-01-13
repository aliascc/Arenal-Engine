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

#ifndef _DIRECTIONAL_LIGHT_H
#define _DIRECTIONAL_LIGHT_H

/**********************
*   System Includes   *
***********************/
#include <array>
#include <string>

/*************************
*   3rd Party Includes   *
**************************/
#include "glm\glm.hpp"

/***************************
*   Game Engine Includes   *
****************************/
#include "Light.h"
#include "Color\Color.h"
#include "Utils\Frustum.h"
#include "Math\AEMathDefs.h"
#include "Color\AEColorDefs.h"

/************
*   Using   *
*************/

/********************
*   Forward Decls   *
*********************/
class Camera;

/*****************
*   Class Decl   *
******************/
class DirectionalLight sealed : public Light
{
	private:

		LightCascadeInfo m_LightCascadeInfo;

		void GetMaxMinPoints(const FrustumCorners& corners, glm::vec3& maxPoint, glm::vec3& minPoint);

		AEResult CalculateCascadeViewProjection(const Camera* camera, float farClip, float frustumNearClip, float frustumFarClip, glm::mat4& viewMatrix, glm::mat4& projectionMatrix, glm::vec3& shadowPos);

	public:

		/***************************************
		*   Constructor & Destructor Methods   *
		****************************************/
#pragma region Constructor & Destructor Methods

		/// <summary>
		/// DirectionalLight Constructor
		/// </summary>
		DirectionalLight(const std::wstring& name = L"Directional Light", const glm::vec3& position = AEMathHelpers::Vec3fZero, const glm::vec3& direction = AEMathHelpers::Vec3fZero, const Color& color = AEColors::White, float nearAtteniation = 0.0f, float farAttenuation = 0.0f, float intensity = 1.0f, bool enabled = true);

		/// <summary>
		/// Default DirectionalLight Destructor
		/// </summary>
		virtual ~DirectionalLight();

#pragma endregion

		/******************
		*   Get Methods   *
		*******************/
#pragma region Get Methods

		const LightCascadeInfo& GetLightCascadeInfo() const
		{
			return m_LightCascadeInfo;
		}

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

		AEResult UpdateCasacadeMatrices(const Camera* camera);

		void UpdateLightMatrices() override;

#pragma endregion

};

#endif