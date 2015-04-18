/********************************************************
*
* Author: Carlos Chacón N.
*
* Created: 2/22/2012
*
* Desc:
*
*
* Copyright 2012
*
*********************************************************/

#pragma once

#ifndef _INPUT_HANDLER_H
#define _INPUT_HANDLER_H

/**********************
*   System Includes   *
***********************/
#include <string>
#include <stdint.h>

/*************************
*   3rd Party Includes   *
**************************/

/***************************
*   Game Engine Includes   *
****************************/
#include "InputDefs.h"
#include "XEGameComponentsDefs.h"
#include "GameUtils\GameComponent.h"

/********************
*   Forward Decls   *
*********************/
class GameApp;
class Keyboard;
struct TimerParams;
class XBoxGamepadManager;

/*****************
*   Class Decl   *
******************/
class InputHandler sealed : public GameComponent
{
	private:

	/************************
	*   Private Variables   *
	*************************/
#pragma region Private Variables

		/// <summary>
		/// Name of the Game Service that will be registered for input handler.
		/// </summary>
		std::wstring m_ServiceName = L"";

#pragma endregion

	public:

	/***************************************
	*   Constructor & Destructor Methods   *
	****************************************/
#pragma region Constructor & Destructor Methods

		/// <summary>
		/// Default InputManager Constructor
		/// </summary>
		InputHandler(GameApp* gameApp, const std::wstring& gameComponentName = XE_INPUT_HANDLER_DEF_COMPONENT_NAME, const std::wstring& serviceName = XE_INPUT_HANDLER_DEF_SERVICE_NAME, uint32_t callOrder = XEGameComponentCallOrder::_XE_GCCO_InputHandler);

		/// <summary>
		/// Default InputHandler Destructor
		/// </summary>
		virtual ~InputHandler();

#pragma endregion

		/******************
		*   Get Methods   *
		*******************/
#pragma region Get Methods

		bool IsKeyboardActive() const;

		bool IsXBoxGamepadManagerActive() const;

		Keyboard* GetKeyboard() const;

		XBoxGamepadManager* GetXBoxGamepadManager() const;

#pragma endregion

		/************************
		*   Framework Methods   *
		*************************/
#pragma region Framework Methods

		void Update(const TimerParams& timerParams) override;

#pragma endregion
};

#endif