
/********************************************************
*
* Author: Carlos Chacón N.
*
* Created: 2/21/2012
*
* Desc:
*
*
* Copyright 2012-2014
*
*********************************************************/
#pragma once

#ifndef _KEYBOARD_H
#define _KEYBOARD_H

/**********************
*   System Includes   *
***********************/
#include <mutex>
#include <vector>
#include <stdint.h>
#include <Windows.h>

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

/*****************
*   Class Decl   *
******************/

class Keyboard sealed : public XEObject
{
	private:

		//Variables
		HKL m_KeyboardLayout = nullptr;

		XEVKWindows m_KeyMapVK[XE_MAX_KEYS];

		uint8_t m_InMemBuffer[XE_MAX_KEYS];

		uint8_t m_PreviousKeyboardState[XE_MAX_KEYS];

		uint8_t m_CurrentKeyboardState[XE_MAX_KEYS];

		std::vector<uint32_t> m_WritableCharsDX;

		uint64_t m_LockID = 0;

		std::mutex m_KeyboardMutex;

		//Private Methods
		void InitBuffers();

		void InitKeyVKMap();

		void Destroy();

	public:

		Keyboard();
		virtual ~Keyboard();

		//Framework Methods
		XEResult LockKeyboard(uint64_t& lockID);

		XEResult UnlockKeyboard(uint64_t lockID);

		void PressUnpressKey(XEKeys key, bool isDown);

		void PressKey(XEKeys key);

		void UnpressKey(XEKeys key);

		bool IsShiftPressMemBuffer() const;

		bool WasKeyPressed(XEKeys key, uint64_t lockID = 0) const;

		bool IsKeyUp(XEKeys key, uint64_t lockID = 0) const;

		bool IsKeyDown(XEKeys key, uint64_t lockID = 0) const;

		bool IsHoldingKey(XEKeys key, uint64_t lockID = 0) const;

		bool HasReleasedKey(XEKeys key, uint64_t lockID = 0) const;

		wchar_t GetCurrentPressedChar(uint64_t lockID = 0) const;

		XEResult Initialize();

		void Update();

		void LoseFocus();
};

#endif