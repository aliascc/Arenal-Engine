/********************************************************
*
* Author: Carlos Chacón N.
*
* Copyright 2012-2015
*
*********************************************************/

/**********************
*   System Includes   *
***********************/
#include <assert.h>

/*************************
*   3rd Party Includes   *
**************************/
#include "glm\gtx\norm.hpp"
#include "glm\gtc\matrix_transform.hpp"

/***************************
*   Game Engine Includes   *
****************************/
#include "GameObjectCamera.h"
#include "Base\BaseFunctions.h"

//Always include last
#include "Memory\MemLeaks.h"

/********************
*   Function Defs   *
*********************/
GameObjectCamera::GameObjectCamera(const std::wstring& name, const glm::vec3& position, const glm::vec3& target, const glm::vec3& UP, const glm::ivec2& dimensions, float fov, float znear, float zfar)
	: Camera(name, position, target, UP, dimensions, fov, znear, zfar)
{
}

GameObjectCamera::~GameObjectCamera()
{
}

XEResult GameObjectCamera::ScreenDimensionsChanged(uint32_t newWidth, uint32_t newHeight)
{
	m_Dimensions.x = newWidth;
	m_Dimensions.y = newHeight;

	CreateProjection();

	return XEResult::Ok;
}
