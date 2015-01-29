/********************************************************
*
* Author: Carlos Chacón N.
*
* Desc:
*
*
* Copyright 2012-2014
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
#include "Keyboard.h"
#include "XBoxGamepad.h"
#include "InputManager.h"
#include "EditorCamera.h"
#include "Base\BaseFunctions.h"
#include "XBoxGamepadManager.h"

//Always include last
#include "Memory\MemLeaks.h"

/********************
*   Function Defs   *
*********************/
EditorCamera::EditorCamera(InputManager* input, const std::wstring& name, const glm::vec3& position, const glm::vec3& target, const glm::vec3& UP, const glm::ivec2& dimensions, float fov, float znear, float zfar)
	: Camera(name, position, target, UP, dimensions, fov, znear, zfar)
	, m_Input(input)
{
	XEAssert(m_Input != nullptr);

	if (m_Input != nullptr && m_Input->IsKeyboardActive())
	{
		m_IsInputReady = true;
	}
	else
	{
		XETODO("Add log message");
	}
}

EditorCamera::~EditorCamera()
{
}

XEResult EditorCamera::Update(const TimerParams& timerParams)
{
	XETODO("Need to change default keys to options");

	if (m_IsInputReady)
	{
		Keyboard* keyboard = m_Input->GetKeyboard();

		glm::vec3 movement = XEMathHelpers::Vec3fZero;
		float dt = (float)timerParams.m_ElapsedTime;

		if (keyboard->IsHoldingKey(XEKeys::UP))
		{
			m_Rotation.x += m_SpinRate * dt;
		}
		if (keyboard->IsHoldingKey(XEKeys::DOWN))
		{
			m_Rotation.x -= m_SpinRate * dt;
		}
		if (keyboard->IsHoldingKey(XEKeys::RIGHT))
		{
			m_Rotation.y -= m_SpinRate * dt;
		}
		if (keyboard->IsHoldingKey(XEKeys::LEFT))
		{
			m_Rotation.y += m_SpinRate * dt;
		}

		if (keyboard->IsHoldingKey(XEKeys::W))
		{
			++movement.z;
		}
		if (keyboard->IsHoldingKey(XEKeys::S))
		{
			--movement.z;
		}

		if (keyboard->IsHoldingKey(XEKeys::D))
		{
			--movement.x;
		}
		if (keyboard->IsHoldingKey(XEKeys::A))
		{
			++movement.x;
		}

		if (keyboard->IsHoldingKey(XEKeys::E))
		{
			++movement.y;
		}
		if (keyboard->IsHoldingKey(XEKeys::Q))
		{
			--movement.y;
		}

		m_Rotation.y = (m_Rotation.y > 360) ? m_Rotation.y - 360 : m_Rotation.y;
		m_Rotation.y = (m_Rotation.y < 0) ? m_Rotation.y + 360 : m_Rotation.y;

		m_Rotation.x = (m_Rotation.x > 89) ? 89 : m_Rotation.x;
		m_Rotation.x = (m_Rotation.x < -89) ? -89 : m_Rotation.x;

		if (glm::length2(movement) != 0.0f)
		{
			movement = glm::normalize(movement);
		}

		movement = movement * (m_MoveRate * 2.0f * dt);

		glm::mat4 rotationMatrix = XEMathHelpers::Mat4Identity;

		//Camera Yaw
		glm::mat4 rotationMatrixY = XEMathHelpers::CreateRotationY(m_Rotation.y);

		//Camera Pitch
		glm::mat4 rotationMatrixX = XEMathHelpers::CreateRotationX(m_Rotation.x);

		rotationMatrix = rotationMatrixY * rotationMatrixX;

		//Update Movement
		if (movement != XEMathHelpers::Vec3fZero)
		{
			movement = (glm::vec3)(rotationMatrix * glm::vec4(movement, 1.0f));

			m_Position = m_Position + movement;
		}

		//Create vector pointing the direction the camera is facing
		glm::vec4 transformedReference = (rotationMatrix * glm::vec4(m_FwrVector, 1.0f));

		//Calculate the position the camera is looking at
		m_Target = m_Position + (glm::vec3)transformedReference;

		//Update View Matrix
		CreateView();
	}

	return Camera::Update(timerParams);;
}

XEResult EditorCamera::ScreenDimensionsChanged(uint32_t newWidth, uint32_t newHeight)
{
	m_Dimensions.x = newWidth;
	m_Dimensions.y = newHeight;

	CreateProjection();

	return XEResult::Ok;
}
