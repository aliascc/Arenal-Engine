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

#ifndef _FRUSTUM_H
#define _FRUSTUM_H

/**********************
*   System Includes   *
***********************/
#include <array>

/*************************
*   3rd Party Includes   *
**************************/
#include "glm\glm.hpp"

/***************************
*   Game Engine Includes   *
****************************/
#include "Base\Base.h"
#include "FrustumDefs.h"
#include "Base\AEObject.h"
#include "Math\AEMathDefs.h"

/************
*   Using   *
*************/

/********************
*   Forward Decls   *
*********************/

/***************
*   Typedefs   *
****************/
typedef std::array<glm::vec4, AE_FRUSTUM_PLANE_NUM> FrustumPlanes;
typedef std::array<glm::vec3, AE_FRUSTUM_CORNER_NUM> FrustumCorners;

/*****************
*   Class Decl   *
******************/
class Frustum sealed : public AEObject
{
	private:

		/************************
		*   Private Variables   *
		*************************/
#pragma region Private Variables

		/// <summary>
		/// View Matrix that Composes the Frustum
		/// </summary>
		glm::mat4 m_ViewMatrix = AEMathHelpers::Mat4Identity;

		/// <summary>
		/// Projection Matrix that Composes the Frustum
		/// </summary>
		glm::mat4 m_ProjectionMatrix = AEMathHelpers::Mat4Identity;

		/// <summary>
		/// The 6 Planes of the Frustum
		/// </summary>
		FrustumPlanes m_Planes;

		/// <summary>
		/// The 8 Corners of the Frustum in World Space
		/// </summary>
		FrustumCorners m_Corners;

		float m_TopSlope = 0.0f;

		float m_BottomSlope = 0.0f;

		float m_LeftSlope = 0.0f;

		float m_RightSlope = 0.0f;

		float m_Near = 0.0f;

		float m_Far = 0.0f;

#pragma endregion

		/**********************
		*   Private Methods   *
		***********************/
#pragma region Private Variables

		void CreateSlopes(const glm::mat4& projection);

#pragma endregion

	public:

		/***************************************
		*   Constructor & Destructor Methods   *
		****************************************/
#pragma region Constructor & Destructor Methods

		/// <summary>
		/// Frustum Constructor
		/// </summary>
		Frustum();

		/// <summary>
		/// Frustum Constructor
		/// </summary>
		/// <param name="viewMatrix">View Matrix</param>
		/// <param name="projMatrix">Projection Matrix</param>
		Frustum(const glm::mat4& viewMatrix, const glm::mat4& projMatrix);

		/// <summary>
		/// Default Frustum Destructor
		/// </summary>
		virtual ~Frustum();

#pragma endregion

		/******************
		*   Get Methods   *
		*******************/
#pragma region Get Methods

		inline float GetTopSlope() const
		{
			return m_TopSlope;
		}

		inline float GetBottomSlope() const
		{
			return m_BottomSlope;
		}

		inline float GetLeftSlope() const
		{
			return m_LeftSlope;
		}

		inline float GetRightSlope() const
		{
			return m_RightSlope;
		}

		/// <summary>
		/// Gets a Plane from the Frustum
		/// </summary>
		/// <param name="planeSide">Plane side to Get</plane>
		/// <returns>Light Projection Matrix</returns>
		inline const glm::vec4& GetPlane(uint32_t planeSide) const
		{
			AEAssert(planeSide < AE_FRUSTUM_PLANE_NUM);
			if (planeSide >= AE_FRUSTUM_PLANE_NUM)
			{
				return AEMathHelpers::Vec4fInfite;
			}

			return m_Planes[planeSide];
		}

		/// <summary>
		/// Gets the 8 Corners of the Frustum
		/// </summary>
		/// <returns>Corners of the Frustum</returns>
		inline const FrustumCorners& GetCorners() const
		{
			return m_Corners;
		}

#pragma endregion

		/******************
		*   Set Methods   *
		*******************/
#pragma region Set Methods

#pragma endregion

		/*************************
		 *   Framework Methods   *
		 *************************/
#pragma region Framework Methods

		/// <summary>
		/// Updates the Frustum with new Matrices
		/// </summary>
		/// <param name="viewMatrix">View Matrix</param>
		/// <param name="projMatrix">Projection Matrix</param>
		void UpdateFrustum(const glm::mat4& viewMatrix, const glm::mat4& projMatrix);

#pragma endregion

};

#endif