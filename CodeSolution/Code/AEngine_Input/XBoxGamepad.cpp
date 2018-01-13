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

/*************************
*   3rd Party Includes   *
**************************/
#include <glm\glm.hpp>

/***************************
*   Game Engine Includes   *
****************************/
#include "XBoxGamepad.h"
#include "Time\AETimeDefs.h"
#include "Base\BaseFunctions.h"

//Always include last
#include "Memory\MemLeaks.h"

/********************
*   Function Defs   *
*********************/

XBoxGamepad::XBoxGamepad(uint32_t id)
	: m_ID(id)
{
	memset(&m_State, 0, sizeof(XINPUT_STATE));
	memset(&m_Capabilities, 0, sizeof(XINPUT_CAPABILITIES));
	memset(&m_Vibration, 0, sizeof(XINPUT_VIBRATION));

	memset(m_GamepadPrevState, 0, sizeof(uint8_t) * AE_MAX_XBOX_GP_BUTTONS);
	memset(m_GamepadCurrentState, 0, sizeof(uint8_t) * AE_MAX_XBOX_GP_BUTTONS);

	memset(m_GamepadMovementCurrentState, 0, sizeof(float) * AE_MAX_XBOX_GP_MOV_BUTTONS);

	AEAssert(id < XUSER_MAX_COUNT);
	if (id >= XUSER_MAX_COUNT)
	{
		AETODO("Log message");
		id = XUSER_MAX_COUNT - 1;
	}
}

XBoxGamepad::~XBoxGamepad()
{
}

AEResult XBoxGamepad::Initialize()
{
	DWORD dwResult = XInputGetCapabilities(m_ID, XINPUT_FLAG_GAMEPAD, &m_Capabilities);

	if (dwResult != ERROR_SUCCESS)
	{
		return AEResult::Fail;
	}

	//Simply get the state of the controller from XInput.
	dwResult = XInputGetState(m_ID, &m_State);

	if (dwResult != ERROR_SUCCESS)
	{
		return AEResult::Fail;
	}

	m_IsConnected = true;
	m_CurrentPacketNumber = m_State.dwPacketNumber;

	return AEResult::Ok;
}

bool XBoxGamepad::VerifyConnectionAndState(const TimerParams& timerParams)
{
	if (m_IsConnected)
	{
		//Simply get the state of the controller from XInput.
		DWORD dwResult = XInputGetState(m_ID, &m_State);

		if (dwResult != ERROR_SUCCESS)
		{
			m_IsConnected = false;
			return false;
		}

		m_CurrentPacketNumber = m_State.dwPacketNumber;
	}
	else
	{
		if (m_InitNextRetry >= m_InitWaitTimeRetry)
		{
			m_InitNextRetry = 0.0f;

			if (Initialize() != AEResult::Ok)
			{
				m_InitNextRetry += (float)timerParams.m_ElapsedTime;

				return false;
			}
		}
		else
		{
			m_InitNextRetry += (float)timerParams.m_ElapsedTime;

			return false;
		}
	}

	return true;
}

void XBoxGamepad::Update(const TimerParams& timerParams)
{
	if (!VerifyConnectionAndState(timerParams))
	{
		return;
	}

	///////////////////////////////////////////
	//Copy Current States to previous states
	memcpy(m_GamepadPrevState, m_GamepadCurrentState, sizeof(uint8_t) * AE_MAX_XBOX_GP_BUTTONS);

	///////////////////////////////////////////
	//Set states to 0.
	memset(m_GamepadCurrentState, 0, sizeof(uint8_t) * AE_MAX_XBOX_GP_BUTTONS);
	memset(m_GamepadMovementCurrentState, 0, sizeof(float) * AE_MAX_XBOX_GP_MOV_BUTTONS);

	///////////////////////////////////////////
	//Set current states of the XBox Gamepad
	m_GamepadCurrentState[(uint32_t)AEXBoxGamepadButton::Y]				= (m_State.Gamepad.wButtons & XINPUT_GAMEPAD_Y)					? AE_INPUT_PRESS : 0;
	m_GamepadCurrentState[(uint32_t)AEXBoxGamepadButton::X]				= (m_State.Gamepad.wButtons & XINPUT_GAMEPAD_X)					? AE_INPUT_PRESS : 0;
	m_GamepadCurrentState[(uint32_t)AEXBoxGamepadButton::A]				= (m_State.Gamepad.wButtons & XINPUT_GAMEPAD_A)					? AE_INPUT_PRESS : 0;
	m_GamepadCurrentState[(uint32_t)AEXBoxGamepadButton::B]				= (m_State.Gamepad.wButtons & XINPUT_GAMEPAD_B)					? AE_INPUT_PRESS : 0;
	m_GamepadCurrentState[(uint32_t)AEXBoxGamepadButton::RightTop]		= (m_State.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER)	? AE_INPUT_PRESS : 0;
	m_GamepadCurrentState[(uint32_t)AEXBoxGamepadButton::LeftTop]		= (m_State.Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_SHOULDER)		? AE_INPUT_PRESS : 0;
	m_GamepadCurrentState[(uint32_t)AEXBoxGamepadButton::DPadUp]		= (m_State.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_UP)			? AE_INPUT_PRESS : 0;
	m_GamepadCurrentState[(uint32_t)AEXBoxGamepadButton::DPadDown]		= (m_State.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_DOWN)			? AE_INPUT_PRESS : 0;
	m_GamepadCurrentState[(uint32_t)AEXBoxGamepadButton::DPadLeft]		= (m_State.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_LEFT)			? AE_INPUT_PRESS : 0;
	m_GamepadCurrentState[(uint32_t)AEXBoxGamepadButton::DPadRight]		= (m_State.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_RIGHT)		? AE_INPUT_PRESS : 0;
	m_GamepadCurrentState[(uint32_t)AEXBoxGamepadButton::Back]			= (m_State.Gamepad.wButtons & XINPUT_GAMEPAD_BACK)				? AE_INPUT_PRESS : 0;
	m_GamepadCurrentState[(uint32_t)AEXBoxGamepadButton::Start]			= (m_State.Gamepad.wButtons & XINPUT_GAMEPAD_START)				? AE_INPUT_PRESS : 0;
	m_GamepadCurrentState[(uint32_t)AEXBoxGamepadButton::RightThumb]	= (m_State.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_THUMB)		? AE_INPUT_PRESS : 0;
	m_GamepadCurrentState[(uint32_t)AEXBoxGamepadButton::LeftThumb]		= (m_State.Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_THUMB)		? AE_INPUT_PRESS : 0;

	///////////////////////////////////////////
	//Set movements for Thumbs
	m_GamepadMovementCurrentState[(uint32_t)AEXBoxGamepadMovements::LeftThumbY]		= GetThumbMovement(m_State.Gamepad.sThumbLY, m_LeftThumbDeadZone);
	m_GamepadMovementCurrentState[(uint32_t)AEXBoxGamepadMovements::LeftThumbX]		= GetThumbMovement(m_State.Gamepad.sThumbLX, m_LeftThumbDeadZone);
	m_GamepadMovementCurrentState[(uint32_t)AEXBoxGamepadMovements::RightThumbY]	= GetThumbMovement(m_State.Gamepad.sThumbRY, m_RightThumbDeadZone);
	m_GamepadMovementCurrentState[(uint32_t)AEXBoxGamepadMovements::RightThumbX]	= GetThumbMovement(m_State.Gamepad.sThumbRX, m_RightThumbDeadZone);

	///////////////////////////////////////////
	//Set movements for Triggers and State
	m_GamepadMovementCurrentState[(uint32_t)AEXBoxGamepadMovements::LeftTrigger]	= (float)m_State.Gamepad.bLeftTrigger / (float)UINT8_MAX;
	m_GamepadMovementCurrentState[(uint32_t)AEXBoxGamepadMovements::RightTrigger]	= (float)m_State.Gamepad.bRightTrigger / (float)UINT8_MAX;

	m_GamepadCurrentState[(uint32_t)AEXBoxGamepadButton::RightBottom]	= (m_GamepadMovementCurrentState[(uint32_t)AEXBoxGamepadMovements::LeftTrigger] == 1.0f) ? AE_INPUT_PRESS : 0;
	m_GamepadCurrentState[(uint32_t)AEXBoxGamepadButton::LeftBottom]	= (m_GamepadMovementCurrentState[(uint32_t)AEXBoxGamepadMovements::RightTrigger] == 1.0f) ? AE_INPUT_PRESS : 0;

	///////////////////////////////////////////
	//Finish
}

float XBoxGamepad::GetThumbMovement(int16_t axisMovement, int16_t deadZone)
{
	if (axisMovement > deadZone)
	{
		axisMovement -= deadZone;

		int16_t max = INT16_MAX - deadZone;

		return ((float)axisMovement / max);
	}
	else if (axisMovement < -deadZone)
	{
		axisMovement += deadZone;

		int16_t min = INT16_MIN + deadZone;

		return -((float)axisMovement / min);
	}
	else
	{
		return 0.0f;
	}
}

void XBoxGamepad::SetVibration(float right, float left)
{
	if (!m_IsConnected)
	{
		return;
	}

	right = glm::clamp(right, 0.0f, 1.0f);
	left = glm::clamp(left, 0.0f, 1.0f);

	uint16_t amountRight = (uint16_t)((float)UINT16_MAX * right);
	uint16_t amountLeft = (uint16_t)((float)UINT16_MAX * left);

	m_Vibration.wLeftMotorSpeed = amountLeft;
	m_Vibration.wRightMotorSpeed = amountRight;

	XInputSetState(m_ID, &m_Vibration);
}

bool XBoxGamepad::WasButtonPressed(AEXBoxGamepadButton button) const
{
	if (!(m_GamepadPrevState[(uint32_t)button] & AE_INPUT_PRESS) && (m_GamepadCurrentState[(uint32_t)button] & AE_INPUT_PRESS))
	{
		return true;
	}

	return false;
}

bool XBoxGamepad::IsButtonUp(AEXBoxGamepadButton button) const
{
	if (!(m_GamepadCurrentState[(uint32_t)button] & AE_INPUT_PRESS))
	{
		return true;
	}

	return false;
}

bool XBoxGamepad::IsButtonDown(AEXBoxGamepadButton button) const
{
	if (m_GamepadCurrentState[(uint32_t)button] & AE_INPUT_PRESS)
	{
		return true;
	}

	return false;
}

bool XBoxGamepad::IsHoldingButton(AEXBoxGamepadButton button) const
{
	if ((m_GamepadPrevState[(uint32_t)button] & AE_INPUT_PRESS) && (m_GamepadCurrentState[(uint32_t)button] & AE_INPUT_PRESS))
	{
		return true;
	}

	return false;
}

bool XBoxGamepad::HasReleasedButton(AEXBoxGamepadButton button) const
{
	if ((m_GamepadPrevState[(uint32_t)button] & AE_INPUT_PRESS) && !(m_GamepadCurrentState[(uint32_t)button] & AE_INPUT_PRESS))
	{
		return true;
	}

	return false;
}

float XBoxGamepad::GetMovement(AEXBoxGamepadMovements movButton) const
{
	return m_GamepadMovementCurrentState[(uint32_t)movButton];
}