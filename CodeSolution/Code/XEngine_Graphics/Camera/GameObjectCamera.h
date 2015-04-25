/********************************************************
*
* Author: Carlos Chacón N.
*
* Copyright 2012-2015
*
*********************************************************/

#pragma once

#ifndef _GAME_OBJECT_CAMERA_H
#define _GAME_OBJECT_CAMERA_H

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
#include "Camera\Camera.h"
#include "Camera\CameraDefs.h"

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

class GameObjectCamera sealed : public Camera
{
	private:

		/************************
		*   Private Variables   *
		*************************/
#pragma region Private Variables

#pragma endregion

		/**********************
		*   Private Methods   *
		***********************/
#pragma region Private Methods

#pragma endregion

	public:

		/***************************************
		*   Constructor & Destructor Methods   *
		****************************************/
#pragma region Constructor & Destructor Methods

		/// <summary>
		/// CameraManager Constructor
		/// </summary>
		GameObjectCamera(const std::wstring& name, const glm::vec3& position = XEMathHelpers::Vec3fZero, const glm::vec3& target = XEMathHelpers::Vec3fZero, const glm::vec3& UP = XEMathHelpers::Vec3fUp, const glm::ivec2& dimensions = XEMathHelpers::Vec2iZero, float fov = 0.0f, float znear = 1.0f, float zfar = 1000.0f);

		/// <summary>
		/// Default CameraManager Destructor
		/// </summary>
		virtual ~GameObjectCamera();

#pragma endregion

		/******************
		*   Get Methods   *
		*******************/
#pragma region Get Methods

#pragma endregion

		/************************
		*   Framework Methods   *
		*************************/
#pragma region Framework Methods

		XEResult ScreenDimensionsChanged(uint32_t newWidth, uint32_t newHeight) override;

#pragma endregion

};

#endif