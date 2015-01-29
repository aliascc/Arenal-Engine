/********************************************************
*
* Author: Carlos Chacón N.
*
* Created: 7/13/2014
*
* Desc:
*
*
* Copyright 2014
*
*********************************************************/

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
#include "Time\XETimeDefs.h"
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

	memset(m_GamepadPrevState, 0, sizeof(uint8_t) * XE_MAX_XBOX_GP_BUTTONS);
	memset(m_GamepadCurrentState, 0, sizeof(uint8_t) * XE_MAX_XBOX_GP_BUTTONS);

	memset(m_GamepadMovementCurrentState, 0, sizeof(float) * XE_MAX_XBOX_GP_MOV_BUTTONS);

	XEAssert(id < XUSER_MAX_COUNT);
	if (id >= XUSER_MAX_COUNT)
	{
		XETODO("Log message");
		id = XUSER_MAX_COUNT - 1;
	}
}

XBoxGamepad::~XBoxGamepad()
{
}

XEResult XBoxGamepad::Initialize()
{
	DWORD dwResult = XInputGetCapabilities(m_ID, XINPUT_FLAG_GAMEPAD, &m_Capabilities);

	if (dwResult != ERROR_SUCCESS)
	{
		return XEResult::Fail;
	}

	//Simply get the state of the controller from XInput.
	dwResult = XInputGetState(m_ID, &m_State);

	if (dwResult != ERROR_SUCCESS)
	{
		return XEResult::Fail;
	}

	m_IsConnected = true;
	m_CurrentPacketNumber = m_State.dwPacketNumber;

	return XEResult::Ok;
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

			if (Initialize() != XEResult::Ok)
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
	memcpy(m_GamepadPrevState, m_GamepadCurrentState, sizeof(uint8_t) * XE_MAX_XBOX_GP_BUTTONS);

	///////////////////////////////////////////
	//Set states to 0.
	memset(m_GamepadCurrentState, 0, sizeof(uint8_t) * XE_MAX_XBOX_GP_BUTTONS);
	memset(m_GamepadMovementCurrentState, 0, sizeof(float) * XE_MAX_XBOX_GP_MOV_BUTTONS);

	///////////////////////////////////////////
	//Set current states of the XBox Gamepad
	m_GamepadCurrentState[(uint32_t)XEXBoxGamepadButton::Y]				= (m_State.Gamepad.wButtons & XINPUT_GAMEPAD_Y)					? XE_INPUT_PRESS : 0;
	m_GamepadCurrentState[(uint32_t)XEXBoxGamepadButton::X]				= (m_State.Gamepad.wButtons & XINPUT_GAMEPAD_X)					? XE_INPUT_PRESS : 0;
	m_GamepadCurrentState[(uint32_t)XEXBoxGamepadButton::A]				= (m_State.Gamepad.wButtons & XINPUT_GAMEPAD_A)					? XE_INPUT_PRESS : 0;
	m_GamepadCurrentState[(uint32_t)XEXBoxGamepadButton::B]				= (m_State.Gamepad.wButtons & XINPUT_GAMEPAD_B)					? XE_INPUT_PRESS : 0;
	m_GamepadCurrentState[(uint32_t)XEXBoxGamepadButton::RightTop]		= (m_State.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER)	? XE_INPUT_PRESS : 0;
	m_GamepadCurrentState[(uint32_t)XEXBoxGamepadButton::LeftTop]		= (m_State.Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_SHOULDER)		? XE_INPUT_PRESS : 0;
	m_GamepadCurrentState[(uint32_t)XEXBoxGamepadButton::DPadUp]		= (m_State.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_UP)			? XE_INPUT_PRESS : 0;
	m_GamepadCurrentState[(uint32_t)XEXBoxGamepadButton::DPadDown]		= (m_State.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_DOWN)			? XE_INPUT_PRESS : 0;
	m_GamepadCurrentState[(uint32_t)XEXBoxGamepadButton::DPadLeft]		= (m_State.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_LEFT)			? XE_INPUT_PRESS : 0;
	m_GamepadCurrentState[(uint32_t)XEXBoxGamepadButton::DPadRight]		= (m_State.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_RIGHT)		? XE_INPUT_PRESS : 0;
	m_GamepadCurrentState[(uint32_t)XEXBoxGamepadButton::Back]			= (m_State.Gamepad.wButtons & XINPUT_GAMEPAD_BACK)				? XE_INPUT_PRESS : 0;
	m_GamepadCurrentState[(uint32_t)XEXBoxGamepadButton::Start]			= (m_State.Gamepad.wButtons & XINPUT_GAMEPAD_START)				? XE_INPUT_PRESS : 0;
	m_GamepadCurrentState[(uint32_t)XEXBoxGamepadButton::RightThumb]	= (m_State.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_THUMB)		? XE_INPUT_PRESS : 0;
	m_GamepadCurrentState[(uint32_t)XEXBoxGamepadButton::LeftThumb]		= (m_State.Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_THUMB)		? XE_INPUT_PRESS : 0;

	///////////////////////////////////////////
	//Set movements for Thumbs
	m_GamepadMovementCurrentState[(uint32_t)XEXBoxGamepadMovements::LeftThumbY]		= GetThumbMovement(m_State.Gamepad.sThumbLY, m_LeftThumbDeadZone);
	m_GamepadMovementCurrentState[(uint32_t)XEXBoxGamepadMovements::LeftThumbX]		= GetThumbMovement(m_State.Gamepad.sThumbLX, m_LeftThumbDeadZone);
	m_GamepadMovementCurrentState[(uint32_t)XEXBoxGamepadMovements::RightThumbY]	= GetThumbMovement(m_State.Gamepad.sThumbRY, m_RightThumbDeadZone);
	m_GamepadMovementCurrentState[(uint32_t)XEXBoxGamepadMovements::RightThumbX]	= GetThumbMovement(m_State.Gamepad.sThumbRX, m_RightThumbDeadZone);

	///////////////////////////////////////////
	//Set movements for Triggers and State
	m_GamepadMovementCurrentState[(uint32_t)XEXBoxGamepadMovements::LeftTrigger]	= (float)m_State.Gamepad.bLeftTrigger / (float)UINT8_MAX;
	m_GamepadMovementCurrentState[(uint32_t)XEXBoxGamepadMovements::RightTrigger]	= (float)m_State.Gamepad.bRightTrigger / (float)UINT8_MAX;

	m_GamepadCurrentState[(uint32_t)XEXBoxGamepadButton::RightBottom]	= (m_GamepadMovementCurrentState[(uint32_t)XEXBoxGamepadMovements::LeftTrigger] == 1.0f) ? XE_INPUT_PRESS : 0;
	m_GamepadCurrentState[(uint32_t)XEXBoxGamepadButton::LeftBottom]	= (m_GamepadMovementCurrentState[(uint32_t)XEXBoxGamepadMovements::RightTrigger] == 1.0f) ? XE_INPUT_PRESS : 0;

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

bool XBoxGamepad::WasButtonPressed(XEXBoxGamepadButton button) const
{
	if (!(m_GamepadPrevState[(uint32_t)button] & XE_INPUT_PRESS) && (m_GamepadCurrentState[(uint32_t)button] & XE_INPUT_PRESS))
	{
		return true;
	}

	return false;
}

bool XBoxGamepad::IsButtonUp(XEXBoxGamepadButton button) const
{
	if (!(m_GamepadCurrentState[(uint32_t)button] & XE_INPUT_PRESS))
	{
		return true;
	}

	return false;
}

bool XBoxGamepad::IsButtonDown(XEXBoxGamepadButton button) const
{
	if (m_GamepadCurrentState[(uint32_t)button] & XE_INPUT_PRESS)
	{
		return true;
	}

	return false;
}

bool XBoxGamepad::IsHoldingButton(XEXBoxGamepadButton button) const
{
	if ((m_GamepadPrevState[(uint32_t)button] & XE_INPUT_PRESS) && (m_GamepadCurrentState[(uint32_t)button] & XE_INPUT_PRESS))
	{
		return true;
	}

	return false;
}

bool XBoxGamepad::HasReleasedButton(XEXBoxGamepadButton button) const
{
	if ((m_GamepadPrevState[(uint32_t)button] & XE_INPUT_PRESS) && !(m_GamepadCurrentState[(uint32_t)button] & XE_INPUT_PRESS))
	{
		return true;
	}

	return false;
}

float XBoxGamepad::GetMovement(XEXBoxGamepadMovements movButton) const
{
	return m_GamepadMovementCurrentState[(uint32_t)movButton];
}