/********************************************************
*
* Author: Carlos Chacón N.
*
* Copyright 2012-2015
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
#include "Base\AEObject.h"

/********************
*   Forward Decls   *
*********************/

/*****************
*   Class Decl   *
******************/

class Keyboard sealed : public AEObject
{
	private:

		//Variables
		HKL m_KeyboardLayout = nullptr;

		AEVKWindows m_KeyMapVK[AE_MAX_KEYS];

		uint8_t m_InMemBuffer[AE_MAX_KEYS];

		uint8_t m_PreviousKeyboardState[AE_MAX_KEYS];

		uint8_t m_CurrentKeyboardState[AE_MAX_KEYS];

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
		AEResult LockKeyboard(uint64_t& lockID);

		AEResult UnlockKeyboard(uint64_t lockID);

		void PressUnpressKey(AEKeys key, bool isDown);

		void PressKey(AEKeys key);

		void UnpressKey(AEKeys key);

		bool IsShiftPressMemBuffer() const;

		bool WasKeyPressed(AEKeys key, uint64_t lockID = 0) const;

		bool IsKeyUp(AEKeys key, uint64_t lockID = 0) const;

		bool IsKeyDown(AEKeys key, uint64_t lockID = 0) const;

		bool IsHoldingKey(AEKeys key, uint64_t lockID = 0) const;

		bool HasReleasedKey(AEKeys key, uint64_t lockID = 0) const;

		wchar_t GetCurrentPressedChar(uint64_t lockID = 0) const;

		AEResult Initialize();

		void Update();

		void LoseFocus();
};

#endif