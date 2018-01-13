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
	AEAssert(m_Input != nullptr);

	if (m_Input != nullptr && m_Input->IsKeyboardActive())
	{
		m_IsInputReady = true;
	}
	else
	{
		AETODO("Add log message");
	}
}

EditorCamera::~EditorCamera()
{
}

AEResult EditorCamera::Update(const TimerParams& timerParams)
{
	AETODO("Need to change default keys to options");

	if (m_IsInputReady)
	{
		Keyboard* keyboard = m_Input->GetKeyboard();

		glm::vec3 movement = AEMathHelpers::Vec3fZero;
		float dt = (float)timerParams.m_ElapsedTime;

		if (keyboard->IsHoldingKey(AEKeys::UP))
		{
			m_Rotation.x += m_SpinRate * dt;
		}
		if (keyboard->IsHoldingKey(AEKeys::DOWN))
		{
			m_Rotation.x -= m_SpinRate * dt;
		}
		if (keyboard->IsHoldingKey(AEKeys::RIGHT))
		{
			m_Rotation.y -= m_SpinRate * dt;
		}
		if (keyboard->IsHoldingKey(AEKeys::LEFT))
		{
			m_Rotation.y += m_SpinRate * dt;
		}

		if (keyboard->IsHoldingKey(AEKeys::W))
		{
			++movement.z;
		}
		if (keyboard->IsHoldingKey(AEKeys::S))
		{
			--movement.z;
		}

		if (keyboard->IsHoldingKey(AEKeys::D))
		{
			--movement.x;
		}
		if (keyboard->IsHoldingKey(AEKeys::A))
		{
			++movement.x;
		}

		if (keyboard->IsHoldingKey(AEKeys::E))
		{
			++movement.y;
		}
		if (keyboard->IsHoldingKey(AEKeys::Q))
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

		glm::mat4 rotationMatrix = AEMathHelpers::Mat4Identity;

		//Camera Yaw
		glm::mat4 rotationMatrixY = AEMathHelpers::CreateRotationY(m_Rotation.y);

		//Camera Pitch
		glm::mat4 rotationMatrixX = AEMathHelpers::CreateRotationX(m_Rotation.x);

		rotationMatrix = rotationMatrixY * rotationMatrixX;

		//Update Movement
		if (movement != AEMathHelpers::Vec3fZero)
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

AEResult EditorCamera::ScreenDimensionsChanged(uint32_t newWidth, uint32_t newHeight)
{
	m_Dimensions.x = newWidth;
	m_Dimensions.y = newHeight;

	CreateProjection();

	return AEResult::Ok;
}
