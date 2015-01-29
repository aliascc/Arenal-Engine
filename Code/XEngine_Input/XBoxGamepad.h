
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
#pragma once

#ifndef _XBOX_GAMEPAD_H
#define _XBOX_GAMEPAD_H

/**********************
*   System Includes   *
***********************/
#include <Windows.h>
#include <Xinput.h>

/*************************
*   3rd Party Includes   *
**************************/

/***************************
*   Game Engine Includes   *
****************************/
#include "Base\Base.h"
#include "InputDefs.h"
#include "Base\XEObject.h"

/********************
*   Forward Decls   *
*********************/
struct TimerParams;

/*****************
*   Class Decl   *
******************/

class XBoxGamepad sealed : public XEObject
{
	private:

	/************************
	*   Private Variables   *
	*************************/
#pragma region Private Variables

		/// <summary>
		/// Struct that contains the Capabilities of the Gamepad.
		/// </summary>
		XINPUT_CAPABILITIES m_Capabilities;

		/// <summary>
		/// Struct that contains the Vibration state of the Gamepad.
		/// </summary>
		XINPUT_VIBRATION m_Vibration;

		/// <summary>
		/// Struct that contains the button, triggers and thumb sticks states of the Gamepad.
		/// </summary>
		XINPUT_STATE m_State;

		/// <summary>
		/// Current Packet Number of the State of the Gamepad, use to determine if
		/// the Gamepad has had a change in state.
		/// </summary>
		DWORD m_CurrentPacketNumber = 0;

		/// <summary>
		/// Defines if the controller is connected and ready
		/// </summary>
		bool m_IsConnected = false;

		/// <summary>
		/// Controller ID, can be 0 - (XUSER_MAX_COUNT - 1).
		/// </summary>
		uint32_t m_ID = 0;

		/// <summary>
		/// Time that has pass until the next retry we do for Gamepad initialization.
		/// </summary>
		/// <remarks>
		/// This is set by default to the max float value so the first time it will always try to 
		/// initialize the controller.
		/// </remarks>
		float m_InitNextRetry = FLT_MAX;

		/// <summary>
		/// Time to wait between petitions to initialize Gamepad.
		/// </summary>
		float m_InitWaitTimeRetry = XE_XBOX_GAMEPAD_INIT_WAIT_TIME;

		/// <summary>
		/// Previous State of the Gamepad Buttons
		/// </summary>
		uint8_t m_GamepadPrevState[XE_MAX_XBOX_GP_BUTTONS];

		/// <summary>
		/// Current State of the Gamepad Buttons
		/// </summary>
		uint8_t m_GamepadCurrentState[XE_MAX_XBOX_GP_BUTTONS]; 

		/// <summary>
		/// Current State of the Gamepad Movement Buttons
		/// </summary>
		float m_GamepadMovementCurrentState[XE_MAX_XBOX_GP_MOV_BUTTONS];

		/// <summary>
		/// Left Thumb Axis Dead Zone, due to hardware sensitivity issues, even if the thumb stick has not move,
		/// false positive movements can be recorded, because of this we have a dead zone.
		/// </summary>
		int16_t m_LeftThumbDeadZone = XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE;

		/// <summary>
		/// Right Thumb Axis Dead Zone, due to hardware sensitivity issues, even if the thumb stick has not move,
		/// false positive movements can be recorded, because of this we have a dead zone.
		/// </summary>
		int16_t m_RightThumbDeadZone = XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE;;

#pragma endregion

		/**********************
		*   Private Methods   *
		***********************/
#pragma region Private Methods

		/// <summary>
		/// Initializes the Gamepad
		/// </summary>
		/// <returns>XEResult::Ok if Gamepad was initialized correctly, XEResult::Fail if it was not.</returns>
		XEResult Initialize();

		/// <summary>
		/// Verifies that the Gamepad is connect and the state has changed.
		/// </summary>
		/// <returns>True if Gamepad is connected and the state has changed, false otherwise.</returns>
		bool VerifyConnectionAndState(const TimerParams& timerParams);

		/// <summary>
		/// Gets the movement (-1.0-1.0) from the a thumb axis, takes into account the dead zone for the axis.
		/// </summary>
		/// <remarks>
		/// -1.0 will be down or left
		/// 1.0 will be up or right
		/// </remarks>
		/// <returns>Movement amount from -1.0 to 1.0.</returns>
		float GetThumbMovement(int16_t axisMovement, int16_t deadZone);

#pragma endregion

	public:

	/***************************************
	*   Constructor & Destructor Methods   *
	****************************************/
#pragma region Constructor & Destructor Methods

		/// <summary>
		/// Default XBoxGamepad Constructor
		/// </summary>
		/// <param name="id">Controller ID, can be 0 - (XUSER_MAX_COUNT - 1).</param>
		XBoxGamepad(uint32_t id);

		/// <summary>
		/// Default XBoxGamepad Destructor
		/// </summary>
		virtual ~XBoxGamepad();

#pragma endregion

	/******************
	*   Get Methods   *
	*******************/
#pragma region Get Methods

		/// <summary>
		/// Returns if the controller is connected and ready
		/// </summary>
		inline bool IsConnected() const
		{
			return m_IsConnected;
		}

		/// <summary>
		/// Gets the time to wait between Gamepad initialize petitions.
		/// </summary>
		inline float GetInitWaitTime() const
		{
			return m_InitWaitTimeRetry;
		}

		/// <summary>
		/// Gets Left Thumb Stick Dead Zone
		/// </summary>
		/// <returns>Left Thumb Stick Dead Zone</returns>
		int16_t GetLeftThumbDeadZone() const
		{
			return m_LeftThumbDeadZone;
		}

		/// <summary>
		/// Gets Right Thumb Stick Dead Zone
		/// </summary>
		/// <returns>Right Thumb Stick Dead Zone</returns>
		int16_t GetRightThumbDeadZone() const
		{
			return m_RightThumbDeadZone;
		}

#pragma endregion

	/******************
	*   Set Methods   *
	*******************/
#pragma region Set Methods

		/// <summary>
		/// Sets the time to wait between Gamepad initialize petitions.
		/// </summary>
		/// <remarks>
		/// If a negative value is given, it will be substitute to 0.
		/// </remarks>
		/// <param name="initWaitTime">New Initialize Wait Time</param>
		inline void SetInitWaitTime(float initWaitTime)
		{
			m_InitWaitTimeRetry = (initWaitTime >= 0.0f) ? initWaitTime : 0.0f;
		}

		/// <summary>
		/// Sets Left Thumb Stick Dead Zone
		/// </summary>
		/// <remarks>
		/// If a negative value is given, it will be set as a positive one.
		/// </remarks>
		/// <param name="leftThumbDeadZone">New Left Thumb Dead Zone</param>
		void SetLeftThumbDeadZone(int16_t leftThumbDeadZone)
		{
			m_LeftThumbDeadZone = abs(leftThumbDeadZone);
		}

		/// <summary>
		/// Gets Right Thumb Stick Dead Zone
		/// </summary>
		/// <remarks>
		/// If a negative value is given, it will be set as a positive one.
		/// </remarks>
		/// <param name="rightThumbDeadZone">New Right Thumb Dead Zone</param>
		void SetRightThumbDeadZone(int16_t rightThumbDeadZone)
		{
			m_RightThumbDeadZone = abs(rightThumbDeadZone);
		}

#pragma endregion

	/************************
	*   Framework Methods   *
	*************************/
#pragma region Framework Methods

		/// <summary>
		/// Sets the vibration state for the Gamepad.
		/// </summary>
		/// <param name="right">0.0-1.0 amount of vibration for the right side of the Gamepad.</param>
		/// <param name="left">0.0-1.0 amount of vibration for the left side of the Gamepad.</param>
		void SetVibration(float right, float left);

		/// <summary>
		/// Updates the state of the Gamepad.
		/// </summary>
		/// <param name="timerParams">Game Timer Parameters</param>
		void Update(const TimerParams& timerParams);

		/// <summary>
		/// Checks to see if a Gamepad button was pressed.
		/// </summary>
		/// <param name="button">Gamepad Button to check.</param>
		/// <returns>True if Gamepad button was pressed, false otherwise.</returns>
		bool WasButtonPressed(XEXBoxGamepadButton button) const;

		/// <summary>
		/// Checks to see if a Gamepad button is up (not pressed).
		/// </summary>
		/// <param name="button">Gamepad Button to check.</param>
		/// <returns>True if Gamepad button is up (not pressed), false otherwise.</returns>
		bool IsButtonUp(XEXBoxGamepadButton button) const;

		/// <summary>
		/// Checks to see if a Gamepad button is pressed.
		/// </summary>
		/// <param name="button">Gamepad Button to check.</param>
		/// <returns>True if Gamepad button is pressed, false otherwise.</returns>
		bool IsButtonDown(XEXBoxGamepadButton button) const;

		/// <summary>
		/// Checks to see if a Gamepad button is being held down.
		/// </summary>
		/// <param name="button">Gamepad Button to check.</param>
		/// <returns>True if Gamepad button is being held down, false otherwise.</returns>
		bool IsHoldingButton(XEXBoxGamepadButton button) const;

		/// <summary>
		/// Checks to see if a Gamepad button was been pressed and it was released.
		/// </summary>
		/// <param name="button">Gamepad Button to check.</param>
		/// <returns>True if Gamepad button was been pressed and it was released, false otherwise.</returns>
		bool HasReleasedButton(XEXBoxGamepadButton button) const;

		/// <summary>
		/// Gets the movement from a Gamepad Movement Button
		/// </summary>
		/// <remarks>
		/// If Thumb Sticks
		///		-1.0 will be down or left
		///		1.0 will be up or right
		///
		/// If Triggers
		///		0.0 not pressed, 1.0 fully pressed.
		/// </remarks>
		/// <param name="movButton">Gamepad Movement Button to check.</param>
		/// <returns>Depending on the button -1.0 to 1.0, see remarks.</returns>
		float GetMovement(XEXBoxGamepadMovements movButton) const;

#pragma endregion

};

#endif