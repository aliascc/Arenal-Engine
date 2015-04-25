/********************************************************
*
* Author: Carlos Chacón N.
*
* Copyright 2012-2015
*
*********************************************************/

#pragma once

#ifndef _XBOX_GAMEPAD_MANAGER_H
#define _XBOX_GAMEPAD_MANAGER_H

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
class XBoxGamepad;
struct TimerParams;

/*****************
*   Class Decl   *
******************/

class XBoxGamepadManager sealed : public XEObject
{
	private:

		/************************
		*   Private Variables   *
		*************************/
#pragma region Private Variables

		/// <summary>
		/// XBox Gamepads
		/// </summary>
		XBoxGamepad* m_Gamepads[XUSER_MAX_COUNT];

#pragma endregion

	public:

	/***************************************
	*   Constructor & Destructor Methods   *
	****************************************/
#pragma region Constructor & Destructor Methods

		/// <summary>
		/// Default XBoxGamepadManager Constructor
		/// </summary>
		XBoxGamepadManager();

		/// <summary>
		/// Default XBoxGamepadManager Destructor
		/// </summary>
		virtual ~XBoxGamepadManager();

#pragma endregion

		/******************
		*   Get Methods   *
		*******************/
#pragma region Get Methods

		inline uint32_t GetMaxGamepads() const
		{
			return XUSER_MAX_COUNT;
		}

		XBoxGamepad& GetGamepad(uint32_t index) const;

#pragma endregion

		/******************
		*   Set Methods   *
		*******************/
#pragma region Set Methods

#pragma endregion

		/************************
		*   Framework Methods   *
		*************************/
#pragma region Framework Methods

		/// <summary>
		/// Updates the states of the Gamepads.
		/// </summary>
		/// <param name="timerParams">Game Timer Parameters</param>
		void Update(const TimerParams& timerParams);

		XBoxGamepad& operator[](uint32_t index) const;

#pragma endregion

};

#endif