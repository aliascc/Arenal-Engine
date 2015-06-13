/********************************************************
*
* Author: Carlos Chacón N.
*
* Copyright 2012-2015
*
*********************************************************/

/**********************
*   System Includes   *
***********************/

/*************************
*   3rd Party Includes   *
**************************/

/***************************
*   Game Engine Includes   *
****************************/
#include "Keyboard.h"
#include "InputManager.h"
#include "XBoxGamepadManager.h"
#include "Base\BaseFunctions.h"

//Always include last
#include "Memory\MemLeaks.h"

/********************
*   Function Defs   *
*********************/

InputManager::InputManager()
{
}

InputManager::~InputManager()
{
	CleanUp();
}

void InputManager::CleanUp()
{
	DeleteMem(m_Keyboard);
	DeleteMem(m_XBoxGamepadManager);
}

AEResult InputManager::Initialize(const GameConfigInput& configInput)
{
	if (m_IsReady)
	{
		return AEResult::Ok;
	}

	AEResult ret = AEResult::Ok;

	if (configInput.m_KeyboardEnabled)
	{
		m_Keyboard = new Keyboard();

		ret = m_Keyboard->Initialize();
		if (ret != AEResult::Ok)
		{
			CleanUp();

			AETODO("Log error");
			return ret;
		}
	}

	if (configInput.m_XBoxGamepadEnabled)
	{
		m_XBoxGamepadManager = new XBoxGamepadManager();
	}

	m_IsReady = true;

	return AEResult::Ok;
}

void InputManager::ProcessKeyboardWindows(TCHAR keyCode, bool isExtended, bool isDown)
{
	if (m_Keyboard == nullptr)
	{
		return;
	}

	///////////////////////////////////////////////////////////////////////////////////////////
	//Process Virtual Key Codes from Windows into Game Keyboard Buffer
	//Windows Virtual Key Codes: http://msdn.microsoft.com/en-us/library/windows/desktop/dd375731(v=vs.85).aspx

	switch ((AEVKWindows)keyCode)
	{
		//Numbers
		case AEVKWindows::AE_VK_KEY_0:
			m_Keyboard->PressUnpressKey(AEKeys::N0, isDown);
			break;
		case AEVKWindows::AE_VK_KEY_1:
			m_Keyboard->PressUnpressKey(AEKeys::N1, isDown);
			break;
		case AEVKWindows::AE_VK_KEY_2:
			m_Keyboard->PressUnpressKey(AEKeys::N2, isDown);
			break;
		case AEVKWindows::AE_VK_KEY_3:
			m_Keyboard->PressUnpressKey(AEKeys::N3, isDown);
			break;
		case AEVKWindows::AE_VK_KEY_4:
			m_Keyboard->PressUnpressKey(AEKeys::N4, isDown);
			break;
		case AEVKWindows::AE_VK_KEY_5:
			m_Keyboard->PressUnpressKey(AEKeys::N5, isDown);
			break;
		case AEVKWindows::AE_VK_KEY_6:
			m_Keyboard->PressUnpressKey(AEKeys::N6, isDown);
			break;
		case AEVKWindows::AE_VK_KEY_7:
			m_Keyboard->PressUnpressKey(AEKeys::N7, isDown);
			break;
		case AEVKWindows::AE_VK_KEY_8:
			m_Keyboard->PressUnpressKey(AEKeys::N8, isDown);
			break;
		case AEVKWindows::AE_VK_KEY_9:
			m_Keyboard->PressUnpressKey(AEKeys::N9, isDown);
			break;

		//Normal Letters
		case AEVKWindows::AE_VK_KEY_A:
			m_Keyboard->PressUnpressKey(AEKeys::A, isDown);
			break;
		case AEVKWindows::AE_VK_KEY_B:
			m_Keyboard->PressUnpressKey(AEKeys::B, isDown);
			break;
		case AEVKWindows::AE_VK_KEY_C:
			m_Keyboard->PressUnpressKey(AEKeys::C, isDown);
			break;
		case AEVKWindows::AE_VK_KEY_D:
			m_Keyboard->PressUnpressKey(AEKeys::D, isDown);
			break;
		case AEVKWindows::AE_VK_KEY_E:
			m_Keyboard->PressUnpressKey(AEKeys::E, isDown);
			break;
		case AEVKWindows::AE_VK_KEY_F:
			m_Keyboard->PressUnpressKey(AEKeys::F, isDown);
			break;
		case AEVKWindows::AE_VK_KEY_G:
			m_Keyboard->PressUnpressKey(AEKeys::G, isDown);
			break;
		case AEVKWindows::AE_VK_KEY_H:
			m_Keyboard->PressUnpressKey(AEKeys::H, isDown);
			break;
		case AEVKWindows::AE_VK_KEY_I:
			m_Keyboard->PressUnpressKey(AEKeys::I, isDown);
			break;
		case AEVKWindows::AE_VK_KEY_J:
			m_Keyboard->PressUnpressKey(AEKeys::J, isDown);
			break;
		case AEVKWindows::AE_VK_KEY_K:
			m_Keyboard->PressUnpressKey(AEKeys::K, isDown);
			break;
		case AEVKWindows::AE_VK_KEY_L:
			m_Keyboard->PressUnpressKey(AEKeys::L, isDown);
			break;
		case AEVKWindows::AE_VK_KEY_M:
			m_Keyboard->PressUnpressKey(AEKeys::M, isDown);
			break;
		case AEVKWindows::AE_VK_KEY_N:
			m_Keyboard->PressUnpressKey(AEKeys::N, isDown);
			break;
		case AEVKWindows::AE_VK_KEY_O:
			m_Keyboard->PressUnpressKey(AEKeys::O, isDown);
			break;
		case AEVKWindows::AE_VK_KEY_P:
			m_Keyboard->PressUnpressKey(AEKeys::P, isDown);
			break;
		case AEVKWindows::AE_VK_KEY_Q:
			m_Keyboard->PressUnpressKey(AEKeys::Q, isDown);
			break;
		case AEVKWindows::AE_VK_KEY_R:
			m_Keyboard->PressUnpressKey(AEKeys::R, isDown);
			break;
		case AEVKWindows::AE_VK_KEY_S:
			m_Keyboard->PressUnpressKey(AEKeys::S, isDown);
			break;
		case AEVKWindows::AE_VK_KEY_T:
			m_Keyboard->PressUnpressKey(AEKeys::T, isDown);
			break;
		case AEVKWindows::AE_VK_KEY_U:
			m_Keyboard->PressUnpressKey(AEKeys::U, isDown);
			break;
		case AEVKWindows::AE_VK_KEY_V:
			m_Keyboard->PressUnpressKey(AEKeys::V, isDown);
			break;
		case AEVKWindows::AE_VK_KEY_W:
			m_Keyboard->PressUnpressKey(AEKeys::W, isDown);
			break;
		case AEVKWindows::AE_VK_KEY_X:
			m_Keyboard->PressUnpressKey(AEKeys::X, isDown);
			break;
		case AEVKWindows::AE_VK_KEY_Y:
			m_Keyboard->PressUnpressKey(AEKeys::Y, isDown);
			break;
		case AEVKWindows::AE_VK_KEY_Z:
			m_Keyboard->PressUnpressKey(AEKeys::Z, isDown);
			break;

		//Numpad
		case AEVKWindows::AE_VK_KEY_NUMPAD0:
			m_Keyboard->PressUnpressKey(AEKeys::NUMPAD0, isDown);
			break;
		case AEVKWindows::AE_VK_KEY_NUMPAD1:
			m_Keyboard->PressUnpressKey(AEKeys::NUMPAD1, isDown);
			break;
		case AEVKWindows::AE_VK_KEY_NUMPAD2:
			m_Keyboard->PressUnpressKey(AEKeys::NUMPAD2, isDown);
			break;
		case AEVKWindows::AE_VK_KEY_NUMPAD3:
			m_Keyboard->PressUnpressKey(AEKeys::NUMPAD3, isDown);
			break;
		case AEVKWindows::AE_VK_KEY_NUMPAD4:
			m_Keyboard->PressUnpressKey(AEKeys::NUMPAD4, isDown);
			break;
		case AEVKWindows::AE_VK_KEY_NUMPAD5:
			m_Keyboard->PressUnpressKey(AEKeys::NUMPAD5, isDown);
			break;
		case AEVKWindows::AE_VK_KEY_NUMPAD6:
			m_Keyboard->PressUnpressKey(AEKeys::NUMPAD6, isDown);
			break;
		case AEVKWindows::AE_VK_KEY_NUMPAD7:
			m_Keyboard->PressUnpressKey(AEKeys::NUMPAD7, isDown);
			break;
		case AEVKWindows::AE_VK_KEY_NUMPAD8:
			m_Keyboard->PressUnpressKey(AEKeys::NUMPAD8, isDown);
			break;
		case AEVKWindows::AE_VK_KEY_NUMPAD9:
			m_Keyboard->PressUnpressKey(AEKeys::NUMPAD9, isDown);
			break;

		//Math
		case AEVKWindows::AE_VK_KEY_MULTIPLY:
			m_Keyboard->PressUnpressKey(AEKeys::MULTIPLY, isDown);
			break;
		case AEVKWindows::AE_VK_KEY_ADD:
			m_Keyboard->PressUnpressKey(AEKeys::ADD, isDown);
			break;
		case AEVKWindows::AE_VK_KEY_SUBTRACT:
			m_Keyboard->PressUnpressKey(AEKeys::SUBTRACT, isDown);
			break;
		case AEVKWindows::AE_VK_KEY_DECIMAL:
			m_Keyboard->PressUnpressKey(AEKeys::DECIMAL, isDown);
			break;
		case AEVKWindows::AE_VK_KEY_DIVIDE:
			m_Keyboard->PressUnpressKey(AEKeys::DIVIDE, isDown);
			break;
		case AEVKWindows::AE_VK_KEY_OEM_PLUS:
			m_Keyboard->PressUnpressKey(AEKeys::OEM_PLUS, isDown);
			break;
		case AEVKWindows::AE_VK_KEY_OEM_MINUS:
			m_Keyboard->PressUnpressKey(AEKeys::OEM_MINUS, isDown);
			break;

		//Grammar
		case AEVKWindows::AE_VK_KEY_UNDERSCORE:
			m_Keyboard->PressUnpressKey(AEKeys::UNDERSCORE, isDown);
			break;
		case AEVKWindows::AE_VK_KEY_OEM_COMMA:
			m_Keyboard->PressUnpressKey(AEKeys::OEM_COMMA, isDown);
			break;
		case AEVKWindows::AE_VK_KEY_OEM_PERIOD:
			m_Keyboard->PressUnpressKey(AEKeys::OEM_PERIOD, isDown);
			break;
		case AEVKWindows::AE_VK_KEY_SPACE:
			m_Keyboard->PressUnpressKey(AEKeys::SPACE, isDown);
			break;
		case AEVKWindows::AE_VK_KEY_BACKSPACE:
			m_Keyboard->PressUnpressKey(AEKeys::BACKSPACE, isDown);
			break;

		//Functions Keys
		case AEVKWindows::AE_VK_KEY_F1:
			m_Keyboard->PressUnpressKey(AEKeys::F1, isDown);
			break;
		case AEVKWindows::AE_VK_KEY_F2:
			m_Keyboard->PressUnpressKey(AEKeys::F2, isDown);
			break;
		case AEVKWindows::AE_VK_KEY_F3:
			m_Keyboard->PressUnpressKey(AEKeys::F3, isDown);
			break;
		case AEVKWindows::AE_VK_KEY_F4:
			m_Keyboard->PressUnpressKey(AEKeys::F4, isDown);
			break;
		case AEVKWindows::AE_VK_KEY_F5:
			m_Keyboard->PressUnpressKey(AEKeys::F5, isDown);
			break;
		case AEVKWindows::AE_VK_KEY_F6:
			m_Keyboard->PressUnpressKey(AEKeys::F6, isDown);
			break;
		case AEVKWindows::AE_VK_KEY_F7:
			m_Keyboard->PressUnpressKey(AEKeys::F7, isDown);
			break;
		case AEVKWindows::AE_VK_KEY_F8:
			m_Keyboard->PressUnpressKey(AEKeys::F8, isDown);
			break;
		case AEVKWindows::AE_VK_KEY_F9:
			m_Keyboard->PressUnpressKey(AEKeys::F9, isDown);
			break;
		case AEVKWindows::AE_VK_KEY_F10:
			m_Keyboard->PressUnpressKey(AEKeys::F10, isDown);
			break;
		case AEVKWindows::AE_VK_KEY_F11:
			m_Keyboard->PressUnpressKey(AEKeys::F11, isDown);
			break;
		case AEVKWindows::AE_VK_KEY_F12:
			m_Keyboard->PressUnpressKey(AEKeys::F12, isDown);
			break;
		case AEVKWindows::AE_VK_KEY_F13:
			m_Keyboard->PressUnpressKey(AEKeys::F13, isDown);
			break;
		case AEVKWindows::AE_VK_KEY_F14:
			m_Keyboard->PressUnpressKey(AEKeys::F14, isDown);
			break;
		case AEVKWindows::AE_VK_KEY_F15:
			m_Keyboard->PressUnpressKey(AEKeys::F15, isDown);
			break;
		case AEVKWindows::AE_VK_KEY_F16:
			m_Keyboard->PressUnpressKey(AEKeys::F16, isDown);
			break;
		case AEVKWindows::AE_VK_KEY_F17:
			m_Keyboard->PressUnpressKey(AEKeys::F17, isDown);
			break;
		case AEVKWindows::AE_VK_KEY_F18:
			m_Keyboard->PressUnpressKey(AEKeys::F18, isDown);
			break;
		case AEVKWindows::AE_VK_KEY_F19:
			m_Keyboard->PressUnpressKey(AEKeys::F19, isDown);
			break;
		case AEVKWindows::AE_VK_KEY_F20:
			m_Keyboard->PressUnpressKey(AEKeys::F20, isDown);
			break;
		case AEVKWindows::AE_VK_KEY_F21:
			m_Keyboard->PressUnpressKey(AEKeys::F21, isDown);
			break;
		case AEVKWindows::AE_VK_KEY_F22:
			m_Keyboard->PressUnpressKey(AEKeys::F22, isDown);
			break;
		case AEVKWindows::AE_VK_KEY_F23:
			m_Keyboard->PressUnpressKey(AEKeys::F23, isDown);
			break;
		case AEVKWindows::AE_VK_KEY_F24:
			m_Keyboard->PressUnpressKey(AEKeys::F24, isDown);
			break;

		//Special Keys
		case AEVKWindows::AE_VK_KEY_TAB:
			m_Keyboard->PressUnpressKey(AEKeys::TAB, isDown);
			break;
		case AEVKWindows::AE_VK_KEY_ENTER:
			if (isExtended)
			{
				m_Keyboard->PressUnpressKey(AEKeys::NUMPADENTER, isDown);
			}
			else
			{
				m_Keyboard->PressUnpressKey(AEKeys::ENTER, isDown);
			}
			break;
		case AEVKWindows::AE_VK_KEY_LSHIFT:
			if (isExtended)
			{
				m_Keyboard->PressUnpressKey(AEKeys::RSHIFT, isDown);
			}
			else
			{
				m_Keyboard->PressUnpressKey(AEKeys::LSHIFT, isDown);
			}
			break;
		case AEVKWindows::AE_VK_KEY_LCTRL:
			if (isExtended)
			{
				m_Keyboard->PressUnpressKey(AEKeys::RCTRL, isDown);
			}
			else
			{
				m_Keyboard->PressUnpressKey(AEKeys::LCTRL, isDown);
			}
			break;
		case AEVKWindows::AE_VK_KEY_LALT:
			if (isExtended)
			{
				m_Keyboard->PressUnpressKey(AEKeys::RALT, isDown);
			}
			else
			{
				m_Keyboard->PressUnpressKey(AEKeys::LALT, isDown);
			}
			break;
		case AEVKWindows::AE_VK_KEY_PAUSE:
			m_Keyboard->PressUnpressKey(AEKeys::PAUSE, isDown);
			break;
		case AEVKWindows::AE_VK_KEY_ESCAPE:
			m_Keyboard->PressUnpressKey(AEKeys::ESCAPE, isDown);
			break;
		case AEVKWindows::AE_VK_KEY_PAGEUP:
			m_Keyboard->PressUnpressKey(AEKeys::PAGEUP, isDown);
			break;
		case AEVKWindows::AE_VK_KEY_PAGEDOWN:
			m_Keyboard->PressUnpressKey(AEKeys::PAGEDOWN, isDown);
			break;
		case AEVKWindows::AE_VK_KEY_END:
			m_Keyboard->PressUnpressKey(AEKeys::END, isDown);
			break;
		case AEVKWindows::AE_VK_KEY_HOME:
			m_Keyboard->PressUnpressKey(AEKeys::HOME, isDown);
			break;
		case AEVKWindows::AE_VK_KEY_LEFT:
			m_Keyboard->PressUnpressKey(AEKeys::LEFT, isDown);
			break;
		case AEVKWindows::AE_VK_KEY_UP:
			m_Keyboard->PressUnpressKey(AEKeys::UP, isDown);
			break;
		case AEVKWindows::AE_VK_KEY_RIGHT:
			m_Keyboard->PressUnpressKey(AEKeys::RIGHT, isDown);
			break;
		case AEVKWindows::AE_VK_KEY_DOWN:
			m_Keyboard->PressUnpressKey(AEKeys::DOWN, isDown);
			break;
		case AEVKWindows::AE_VK_KEY_PRINTSCREEN:
			m_Keyboard->PressUnpressKey(AEKeys::PRINTSCREEN, isDown);
			break;
		case AEVKWindows::AE_VK_KEY_INSERT:
			m_Keyboard->PressUnpressKey(AEKeys::INSERT, isDown);
			break;
		case AEVKWindows::AE_VK_KEY_DELETE:
			m_Keyboard->PressUnpressKey(AEKeys::DEL, isDown);
			break;

		//Lock Keys
		case AEVKWindows::AE_VK_KEY_CAPSLOCK:
			m_Keyboard->PressUnpressKey(AEKeys::CAPSLOCK, isDown);
			break;
		case AEVKWindows::AE_VK_KEY_NUMLOCK:
			m_Keyboard->PressUnpressKey(AEKeys::NUMLOCK, isDown);
			break;
		case AEVKWindows::AE_VK_KEY_SCROLLLOCK:
			m_Keyboard->PressUnpressKey(AEKeys::SCROLLLOCK, isDown);
			break;

		//Music Keys
		case AEVKWindows::AE_VK_KEY_VOLUME_MUTE:
			m_Keyboard->PressUnpressKey(AEKeys::VOLUME_MUTE, isDown);
			break;
		case AEVKWindows::AE_VK_KEY_VOLUME_DOWN:
			m_Keyboard->PressUnpressKey(AEKeys::VOLUME_DOWN, isDown);
			break;
		case AEVKWindows::AE_VK_KEY_VOLUME_UP:
			m_Keyboard->PressUnpressKey(AEKeys::VOLUME_UP, isDown);
			break;
		case AEVKWindows::AE_VK_KEY_MEDIA_NEXT_TRACK:
			m_Keyboard->PressUnpressKey(AEKeys::MEDIA_NEXT_TRACK, isDown);
			break;
		case AEVKWindows::AE_VK_KEY_MEDIA_PREV_TRACK:
			m_Keyboard->PressUnpressKey(AEKeys::MEDIA_PREV_TRACK, isDown);
			break;
		case AEVKWindows::AE_VK_KEY_MEDIA_STOP:
			m_Keyboard->PressUnpressKey(AEKeys::MEDIA_STOP, isDown);
			break;
		case AEVKWindows::AE_VK_KEY_MEDIA_PLAY_PAUSE:
			m_Keyboard->PressUnpressKey(AEKeys::MEDIA_PLAY_PAUSE, isDown);
			break;

		//OEM Keys
		case AEVKWindows::AE_VK_KEY_OEM_SEMICOLON:
			if (m_Keyboard->IsShiftPressMemBuffer())
			{
				m_Keyboard->PressUnpressKey(AEKeys::OEM_COLON, isDown);
			}
			else
			{
				m_Keyboard->PressUnpressKey(AEKeys::OEM_SEMICOLON, isDown);
			}
			break;
		case AEVKWindows::AE_VK_KEY_OEM_FWR_SLASH:
			if (m_Keyboard->IsShiftPressMemBuffer())
			{
				m_Keyboard->PressUnpressKey(AEKeys::OEM_QUESTION, isDown);
			}
			else
			{
				m_Keyboard->PressUnpressKey(AEKeys::OEM_FWR_SLASH, isDown);
			}
			break;
		case AEVKWindows::AE_VK_KEY_OEM_TILDE:
			if (m_Keyboard->IsShiftPressMemBuffer())
			{
				m_Keyboard->PressUnpressKey(AEKeys::OEM_BACK_QUOTE, isDown);
			}
			else
			{
				m_Keyboard->PressUnpressKey(AEKeys::OEM_TILDE, isDown);
			}
			break;
		case AEVKWindows::AE_VK_KEY_OEM_OPEN_BRACKET:
			if (m_Keyboard->IsShiftPressMemBuffer())
			{
				m_Keyboard->PressUnpressKey(AEKeys::OEM_OPEN_BRACE, isDown);
			}
			else
			{
				m_Keyboard->PressUnpressKey(AEKeys::OEM_OPEN_BRACKET, isDown);
			}
			break;
		case AEVKWindows::AE_VK_KEY_OEM_BACK_SLASH:
			if (m_Keyboard->IsShiftPressMemBuffer())
			{
				m_Keyboard->PressUnpressKey(AEKeys::OEM_PIPE, isDown);
			}
			else
			{
				m_Keyboard->PressUnpressKey(AEKeys::OEM_BACK_SLASH, isDown);
			}
			break;
		case AEVKWindows::AE_VK_KEY_OEM_CLOSE_BRACKET:
			if (m_Keyboard->IsShiftPressMemBuffer())
			{
				m_Keyboard->PressUnpressKey(AEKeys::OEM_CLOSE_BRACE, isDown);
			}
			else
			{
				m_Keyboard->PressUnpressKey(AEKeys::OEM_CLOSE_BRACKET, isDown);
			}
			break;
		case AEVKWindows::AE_VK_KEY_OEM_SINGLE_QUOTE:
			if (m_Keyboard->IsShiftPressMemBuffer())
			{
				m_Keyboard->PressUnpressKey(AEKeys::OEM_DOUBLE_QUOTE, isDown);
			}
			else
			{
				m_Keyboard->PressUnpressKey(AEKeys::OEM_SINGLE_QUOTE, isDown);
			}
			break;
		case AEVKWindows::AE_VK_KEY_OEM_1:
			m_Keyboard->PressUnpressKey(AEKeys::OEM_1, isDown);
			break;
		case AEVKWindows::AE_VK_KEY_OEM_2:
			m_Keyboard->PressUnpressKey(AEKeys::OEM_2, isDown);
			break;
		case AEVKWindows::AE_VK_KEY_OEM_3:
			m_Keyboard->PressUnpressKey(AEKeys::OEM_3, isDown);
			break;
		case AEVKWindows::AE_VK_KEY_OEM_4:
			m_Keyboard->PressUnpressKey(AEKeys::OEM_4, isDown);
			break;
		case AEVKWindows::AE_VK_KEY_OEM_5:
			m_Keyboard->PressUnpressKey(AEKeys::OEM_5, isDown);
			break;
		case AEVKWindows::AE_VK_KEY_OEM_6:
			m_Keyboard->PressUnpressKey(AEKeys::OEM_6, isDown);
			break;
		case AEVKWindows::AE_VK_KEY_OEM_7:
			m_Keyboard->PressUnpressKey(AEKeys::OEM_7, isDown);
			break;
		case AEVKWindows::AE_VK_KEY_OEM_8:
			m_Keyboard->PressUnpressKey(AEKeys::OEM_8, isDown);
			break;
		case AEVKWindows::AE_VK_KEY_OEM_9:
			m_Keyboard->PressUnpressKey(AEKeys::OEM_9, isDown);
			break;
		case AEVKWindows::AE_VK_KEY_OEM_10:
			m_Keyboard->PressUnpressKey(AEKeys::OEM_10, isDown);
			break;
		case AEVKWindows::AE_VK_KEY_OEM_11:
			m_Keyboard->PressUnpressKey(AEKeys::OEM_11, isDown);
			break;
		case AEVKWindows::AE_VK_KEY_OEM_12:
			m_Keyboard->PressUnpressKey(AEKeys::OEM_12, isDown);
			break;
		case AEVKWindows::AE_VK_KEY_OEM_13:
			m_Keyboard->PressUnpressKey(AEKeys::OEM_13, isDown);
			break;
		case AEVKWindows::AE_VK_KEY_OEM_14:
			m_Keyboard->PressUnpressKey(AEKeys::OEM_14, isDown);
			break;
		case AEVKWindows::AE_VK_KEY_OEM_15:
			m_Keyboard->PressUnpressKey(AEKeys::OEM_15, isDown);
			break;
		case AEVKWindows::AE_VK_KEY_OEM_16:
			m_Keyboard->PressUnpressKey(AEKeys::OEM_16, isDown);
			break;
		case AEVKWindows::AE_VK_KEY_OEM_17:
			m_Keyboard->PressUnpressKey(AEKeys::OEM_17, isDown);
			break;
		case AEVKWindows::AE_VK_KEY_OEM_18:
			m_Keyboard->PressUnpressKey(AEKeys::OEM_18, isDown);
			break;
		case AEVKWindows::AE_VK_KEY_OEM_19:
			m_Keyboard->PressUnpressKey(AEKeys::OEM_19, isDown);
			break;
		case AEVKWindows::AE_VK_KEY_OEM_20:
			m_Keyboard->PressUnpressKey(AEKeys::OEM_20, isDown);
			break;
		case AEVKWindows::AE_VK_KEY_OEM_21:
			m_Keyboard->PressUnpressKey(AEKeys::OEM_21, isDown);
			break;
		case AEVKWindows::AE_VK_KEY_OEM_22:
			m_Keyboard->PressUnpressKey(AEKeys::OEM_22, isDown);
			break;
		case AEVKWindows::AE_VK_KEY_OEM_23:
			m_Keyboard->PressUnpressKey(AEKeys::OEM_23, isDown);
			break;
	}
}

void InputManager::ReloadKeyboardLaylout()
{
	if (m_Keyboard == nullptr)
	{
		return;
	}

	m_Keyboard->Initialize();
}

void InputManager::LoseFocus()
{
	if (m_Keyboard != nullptr)
	{
		m_Keyboard->LoseFocus();
	}
}

bool InputManager::IsKeyboardActive() const
{
	return (m_Keyboard != nullptr);
}

bool InputManager::IsXBoxGamepadManagerActive() const
{
	return (m_XBoxGamepadManager != nullptr);
}
