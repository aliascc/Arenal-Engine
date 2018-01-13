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
#include "Base\AEObject.h"

/********************
*   Forward Decls   *
*********************/
class XBoxGamepad;
struct TimerParams;

/*****************
*   Class Decl   *
******************/

class XBoxGamepadManager sealed : public AEObject
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