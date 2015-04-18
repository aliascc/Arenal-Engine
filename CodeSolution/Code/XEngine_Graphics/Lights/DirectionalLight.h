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
#include "Math\XEMathDefs.h"
#include "Color\XEColorDefs.h"

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

		XEResult CalculateCascadeViewProjection(const Camera* camera, float farClip, float frustumNearClip, float frustumFarClip, glm::mat4& viewMatrix, glm::mat4& projectionMatrix, glm::vec3& shadowPos);

	public:

		/***************************************
		*   Constructor & Destructor Methods   *
		****************************************/
#pragma region Constructor & Destructor Methods

		/// <summary>
		/// DirectionalLight Constructor
		/// </summary>
		DirectionalLight(const std::wstring& name = L"Directional Light", const glm::vec3& position = XEMathHelpers::Vec3fZero, const glm::vec3& direction = XEMathHelpers::Vec3fZero, const Color& color = XEColors::White, float nearAtteniation = 0.0f, float farAttenuation = 0.0f, float intensity = 1.0f, bool enabled = true);

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

		XEResult UpdateCasacadeMatrices(const Camera* camera);

		void UpdateLightMatrices() override;

#pragma endregion

};

#endif