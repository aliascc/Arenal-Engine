/********************************************************
*
* Author: Carlos Chacón N.
*
* Copyright 2012-2015
*
*********************************************************/

#pragma once

#ifndef _CAMERA_H
#define _CAMERA_H

/**********************
*   System Includes   *
***********************/

/*************************
*   3rd Party Includes   *
**************************/
#include "glm\glm.hpp"

/***************************
*   Game Engine Includes   *
****************************/
#include "Base\Base.h"
#include "Utils\Frustum.h"
#include "Math\AEMathDefs.h"
#include "Base\UniqueAEObjectNamed.h"

/************
*   Using   *
*************/

/********************
*   Forward Decls   *
*********************/
struct TimerParams;

/*****************
*   Class Decl   *
******************/
class Camera abstract : public UniqueAEObjectNamed
{
	protected:
		//Variables
		glm::mat4 m_ViewMatrix = AEMathHelpers::Mat4Identity;
		glm::mat4 m_ProjectionMatrix = AEMathHelpers::Mat4Identity;
		glm::vec3 m_Position = AEMathHelpers::Vec3fZero;
		glm::vec3 m_Target = AEMathHelpers::Vec3fZero;
		glm::vec3 m_UPVector = AEMathHelpers::Vec3fUp;
		glm::ivec2 m_Dimensions = AEMathHelpers::Vec2iZero;

		glm::vec3 m_Rotation = AEMathHelpers::Vec3fZero;

		glm::vec3 m_FwrVector = AEMathHelpers::Vec3fFwrZP;

		float m_ZNear = 0.0f;
		float m_ZFar = 0.0f;
		float m_FoV = 0.0f;

		Frustum m_Frustum;

		//Private Methods
		void CreateView();
		void CreateProjection();

	public:
		//Constructor Destructor.
		Camera(const std::wstring& name, const glm::vec3& position = AEMathHelpers::Vec3fZero, const glm::vec3& target = AEMathHelpers::Vec3fZero, const glm::vec3& UP = AEMathHelpers::Vec3fUp, const glm::ivec2& dimensions = AEMathHelpers::Vec2iZero, float fov = 0.0f, float znear = 1.0f, float zfar = 1000.0f);

		virtual ~Camera();

		//Gets
		inline const glm::mat4&	GetProjectionMatrix() const
		{
			return m_ProjectionMatrix;
		}

		inline const glm::mat4& GetViewMatrix() const
		{
			return m_ViewMatrix;
		}

		inline const glm::vec3& GetPosition() const
		{
			return m_Position;
		}

		inline const glm::ivec2& GetDimensions() const
		{
			return m_Dimensions;
		}

		inline float GetZNear() const
		{
			return m_ZNear;
		}

		inline float GetZFar() const
		{
			return m_ZFar;
		}

		inline const Frustum& GetFrustum() const
		{
			return m_Frustum;
		}

		inline const glm::vec3& GetUpVector() const
		{
			return m_UPVector;
		}

		float GetAspectRatio() const;

		glm::vec3 GetDirectionVector() const;

		glm::mat4 GetWorldTransform() const;

		//Framework Methods
		void SetNewPositionAndRotation(const glm::vec3& position, const glm::vec3& rotation);

		virtual AEResult Update(const TimerParams& timerParams);

		virtual AEResult ScreenDimensionsChanged(uint32_t newWidth, uint32_t newHeight) = 0;
};

#endif