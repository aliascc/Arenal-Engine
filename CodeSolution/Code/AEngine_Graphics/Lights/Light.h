/********************************************************
*
* Author: Carlos Chacón N.
*
* Copyright 2012-2015
*
*********************************************************/

#pragma once

#ifndef _LIGHT_H
#define _LIGHT_H

/**********************
*   System Includes   *
***********************/
#include <string>
#include <limits>

/*************************
*   3rd Party Includes   *
**************************/
#include "glm\glm.hpp"

/***************************
*   Game Engine Includes   *
****************************/
#include "LightDefs.h"
#include "Color\Color.h"
#include "Math\AEMathDefs.h"
#include "Color\AEColorDefs.h"
#include "Base\UniqueAEObjectNamed.h"

/************
*   Using   *
*************/
class GraphicDevice;

/********************
*   Forward Decls   *
*********************/
class Frustum;
struct TimerParams;

/*****************
*   Class Decl   *
******************/
class Light abstract : public UniqueAEObjectNamed
{
	friend class LightManager;

	private:

		/************************
		*   Private Variables   *
		*************************/
#pragma region Private Variables

		/// <summary>
		/// Type of Light
		/// </summary>
		LightType m_LightType = LightType::Invalid;

#pragma endregion

	protected:

		/**************************
		*   Protected Variables   *
		***************************/
#pragma region Protected Variables

		/// <summary>
		/// Position of the Light
		/// </summary>
		glm::vec3 m_Position = AEMathHelpers::Vec3fZero;

		/// <summary>
		/// Direction the Light is pointing to
		/// </summary>
		glm::vec3 m_Direction = AEMathHelpers::Vec3fZero;

		/// <summary>
		/// Color of the Light
		/// </summary>
		Color m_Color = AEColors::White;

		/// <summary>
		/// Near Attenuation of the Light
		/// </summary>
		float m_NearAttenuation = 0.0f;

		/// <summary>
		/// Far Attenuation of the Light
		/// </summary>
		float m_FarAttenuation = 0.0f;

		/// <summary>
		/// Intensity of the Light
		/// </summary>
		float m_Intensity = 0.0f;

		/// <summary>
		/// Determines if the Light is enabled or not
		/// </summary>
		bool m_Enabled = false;

		/// <summary>
		/// Determines if it projects a shadow
		/// </summary>
		bool m_ShadowEnabled = false;

		/// <summary>
		/// Shadow Texture Index
		/// </summary>
		uint32_t m_ShadowTextureIndex = AE_LIGHT_SHADOW_DISABLE_INDEX;

		/// <summary>
		/// View Matrix of the Light
		/// </summary>
		glm::mat4 m_ViewMatrix = AEMathHelpers::Mat4Identity;

		/// <summary>
		/// Projection Matrix of the Light
		/// </summary>
		glm::mat4 m_ProjectionMatrix = AEMathHelpers::Mat4Identity;

		/// <summary>
		/// Frustum of the Light
		/// </summary>
		Frustum* m_Frustum = nullptr;

#pragma	endregion

	public:

		/***************************************
		*   Constructor & Destructor Methods   *
		****************************************/
#pragma region Constructor & Destructor Methods

		/// <summary>
		/// Light Constructor
		/// </summary>
		Light(LightType lightType, const std::wstring& name = L"Light", const glm::vec3& position = AEMathHelpers::Vec3fZero, const glm::vec3& direction = AEMathHelpers::Vec3fZero, const Color& color = AEColors::White, float nearAtteniation = 0.0f, float farAttenuation = 0.0f, float intensity = 1.0f, bool enable = true);

		/// <summary>
		/// Default Light Destructor
		/// </summary>
		virtual ~Light();

#pragma endregion

		/******************
		*   Get Methods   *
		*******************/
#pragma region Get Methods

		/// <summary>
		/// Gets the Type of the Light
		/// </summary>
		/// <returns>Returns the Type of the Light</returns>
		inline LightType GetLightType() const
		{
			return m_LightType;
		}

		/// <summary>
		/// Gets the Position of the Light
		/// </summary>
		/// <returns>Returns the Light Position</returns>
		inline const glm::vec3& GetPosition() const
		{ 
			return m_Position; 
		}

		/// <summary>
		/// Gets the Direction of the Light
		/// </summary>
		/// <returns>Light Direction</returns>
		inline const glm::vec3& GetDirection() const
		{ 
			return m_Direction; 
		}

		/// <summary>
		/// Gets the Color of the Light
		/// </summary>
		/// <returns>Light Color</returns>
		inline const Color& GetColor() const
		{ 
			return m_Color; 
		}

		/// <summary>
		/// Gets the Near Attenuation of the Light
		/// </summary>
		inline float GetNearAttenuation() const
		{ 
			return m_NearAttenuation; 
		}

		/// <summary>
		/// Gets the Far Attenuation of the Light
		/// </summary>
		inline float GetFarAttenuation() const
		{ 
			return m_FarAttenuation; 
		}

		/// <summary>
		/// Gets the Intensity of the Light
		/// </summary>
		inline float GetIntensity() const
		{ 
			return m_Intensity; 
		}

		/// <summary>
		/// Gets if the Light is enabled or not
		/// </summary>
		/// <returns>True if the Light is enabled</returns>
		inline bool IsEnabled() const
		{
			return m_Enabled;
		}

		/// <summary>
		/// Gets if the Light projects a shadow
		/// </summary>
		/// <returns>True if the Light projects a shadow</returns>
		inline bool IsShadowEnabled() const
		{
			return m_ShadowEnabled && m_ShadowTextureIndex != AE_LIGHT_SHADOW_DISABLE_INDEX;
		}

		/// <summary>
		/// Gets the Shadow Texture Index
		/// </summary>
		/// <returns>Shadow Texture Index</returns>
		inline uint32_t GetShadowTextureIndex() const
		{
			return m_ShadowTextureIndex;
		}

		/// <summary>
		/// Gets the Light View Matrix
		/// </summary>
		/// <returns>Light View Matrix</returns>
		inline const glm::mat4& GetViewMatrix() const
		{
			return m_ViewMatrix;
		}

		/// <summary>
		/// Gets the Light Projection Matrix
		/// </summary>
		/// <returns>Light Projection Matrix</returns>
		inline const glm::mat4& GetProjectionMatrix() const
		{
			return m_ProjectionMatrix;
		}

		/// <summary>
		/// Gets the Frustum of the Light
		/// </summary>
		/// <returns>Light Frustum</returns>
		inline const Frustum* GetFrustum() const
		{
			return m_Frustum;
		}

#pragma endregion

		/******************
		*   Set Methods   *
		*******************/
#pragma region Set Methods

		/// <summary>
		/// Sets the Position of the Light
		/// </summary>
		/// <param name="position">New Position of the Light</param>
		inline void SetPosition(const glm::vec3& position)
		{
			m_Position = position; 
		}

		/// <summary>
		/// Sets the Direction of the Light
		/// </summary>
		/// <param name="direction">New Direction of the Light</param>
		inline void SetDirection(const glm::vec3& direction)
		{ 
			m_Direction = direction; 
		}

		/// <summary>
		/// Sets the Color of the Light
		/// </summary>
		/// <param name="color">New Color of the Light</param>
		inline void SetColor(const Color& color)
		{ 
			m_Color = color; 
		}

		/// <summary>
		/// Sets the Near Attenuation of the Light
		/// </summary>
		/// <param name="nearAttenuation">New Near Attenuation of the Light</param>
		inline void SetNearAttenuation(float nearAttenuation)
		{ 
			m_NearAttenuation = nearAttenuation; 
		}

		/// <summary>
		/// Sets the Far Attenuation of the Light
		/// </summary>
		/// <param name="farAttenuation">New Far Attenuation of the Light</param>
		inline void SetFarAttenuation(float farAttenuation)
		{ 
			m_FarAttenuation = farAttenuation; 
		}

		/// <summary>
		/// Sets if the Intensity of the Light
		/// </summary>
		/// <param name="intensity">Intensity of the Light, normally from 0.0 to 1.0</param>
		inline void SetIntensity(float intensity)
		{
			m_Intensity = intensity;
		}

		/// <summary>
		/// Sets if the Light is enabled or not
		/// </summary>
		/// <param name="enable">True if the light is enabled</param>
		inline void SetEnabled(bool enable)
		{
			m_Enabled = enable;
		}

		/// <summary>
		/// Sets if the Light shadow enabled or not
		/// </summary>
		/// <param name="shadowEnable">True if the light Shadow enabled</param>
		inline void SetShadowEnabled(bool shadowEnable)
		{
			m_ShadowEnabled = shadowEnable;
		}

#pragma endregion

		/*************************
		 *   Framework Methods   *
		 *************************/
#pragma region Framework Methods

		virtual void UpdateLightMatrices() = 0;

		/// <summary>
		/// Assignment Operator Overload
		/// </summary>
		/// <returns>True if they are the same</returns>
		virtual Light& operator=(const Light& other);

		/// <summary>
		/// Not Equal Operator Overload
		/// </summary>
		/// <returns>True if they are not the same</returns>
		bool operator!=(const LightFX& other);

		/// <summary>
		/// Equal Operator Overload
		/// </summary>
		/// <returns>True if they are the same</returns>
		virtual bool operator==(const LightFX& other);

		/// <summary>
		/// Update the LightFX with the information of the light
		/// </summary>
		/// <param name="lightFX">LightFX Structure to Update</param>
		virtual void UpdateLighFX(LightFX& lightFX);

#pragma endregion

};

#endif