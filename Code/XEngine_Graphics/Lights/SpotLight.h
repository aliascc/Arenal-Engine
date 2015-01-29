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

#ifndef _SPOT_LIGHT_H
#define _SPOT_LIGHT_H

/**********************
*   System Includes   *
***********************/
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
#include "Math\XEMathDefs.h"
#include "Color\XEColorDefs.h"

/************
*   Using   *
*************/

/********************
*   Forward Decls   *
*********************/

/*****************
*   Class Decl   *
******************/
class SpotLight sealed : public Light
{
	protected:
		//Variables
		float m_FallOffAngle = 0.0f;
		float m_Angle = 0.0f;

	public:

		/*****************************************
		 *   Constructor & Destructor Methods   *
		 *****************************************/
#pragma region Constructor & Destructor Methods

		/// <summary>
		/// SpotLight Constructor
		/// </summary>
		SpotLight(const std::wstring& name = L"Light", const glm::vec3& position = XEMathHelpers::Vec3fZero, const glm::vec3& direction = XEMathHelpers::Vec3fZero, const Color& color = XEColors::White, float nearAtteniation = 0.0f, float farAttenuation = 0.0f, float intensity = 1.0f, bool enabled = true, float fallOffAngle = 0.0f, float angle = 0.0f);

		/// <summary>
		/// Default SpotLight Destructor
		/// </summary>
		virtual ~SpotLight();

#pragma endregion

		/******************
		*   Get Methods   *
		*******************/
#pragma region Get Methods

		/// <summary>
		/// Gets the Fall off Angle the Spot Light
		/// </summary>
		/// <returns>Returns the Fall of Angle of the Spot Light</returns>
		inline float GetFallOffAngle() const
		{
			return m_FallOffAngle;
		}

		/// <summary>
		/// Gets the Angle the Spot Light
		/// </summary>
		/// <returns>Returns the Angle of the Spot Light</returns>
		inline float GetAngle() const
		{
			return m_Angle;
		}

#pragma endregion

		/******************
		*   Set Methods   *
		*******************/
#pragma region Set Methods

		/// <summary>
		/// Sets the Fall off Angle of the Spot Light
		/// </summary>
		/// <param name="fallOffAngle">New Fall off Angle of the Spot Light</param>
		inline void SetFallOffAngle(float fallOffAngle)
		{
			m_FallOffAngle = fallOffAngle;
		}

		/// <summary>
		/// Sets the Angle of the Spot Light
		/// </summary>
		/// <param name="angle">New Angle of the Spot Light</param>
		inline void SetAngle(float angle)
		{
			m_Angle = angle;
		}

#pragma endregion

		/************************
		*   Framework Methods   *
		*************************/
#pragma region Framework Methods

		void UpdateLightMatrices() override;

		/// <summary>
		/// Assignment Operator Overload
		/// </summary>
		/// <returns>True if they are the same</returns>
		Light& operator=(const Light& other) override;

		/// <summary>
		/// Equal Operator Overload
		/// </summary>
		/// <returns>True if they are the same</returns>
		bool operator==(const LightFX& other) override;

		/// <summary>
		/// Update the LightFX with the information of the light
		/// </summary>
		/// <param name="lightFX">LightFX Structure to Update</param>
		void UpdateLighFX(LightFX& lightFX) override;

#pragma endregion
};

#endif