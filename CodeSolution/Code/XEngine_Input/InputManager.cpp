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

XEResult InputManager::Initialize(const GameConfigInput& configInput)
{
	if (m_IsReady)
	{
		return XEResult::Ok;
	}

	XEResult ret = XEResult::Ok;

	if (configInput.m_KeyboardEnabled)
	{
		m_Keyboard = new Keyboard();

		ret = m_Keyboard->Initialize();
		if (ret != XEResult::Ok)
		{
			CleanUp();

			XETODO("Log error");
			return ret;
		}
	}

	if (configInput.m_XBoxGamepadEnabled)
	{
		m_XBoxGamepadManager = new XBoxGamepadManager();
	}

	m_IsReady = true;

	return XEResult::Ok;
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

	switch ((XEVKWindows)keyCode)
	{
		//Numbers
		case XEVKWindows::XE_VK_KEY_0:
			m_Keyboard->PressUnpressKey(XEKeys::N0, isDown);
			break;
		case XEVKWindows::XE_VK_KEY_1:
			m_Keyboard->PressUnpressKey(XEKeys::N1, isDown);
			break;
		case XEVKWindows::XE_VK_KEY_2:
			m_Keyboard->PressUnpressKey(XEKeys::N2, isDown);
			break;
		case XEVKWindows::XE_VK_KEY_3:
			m_Keyboard->PressUnpressKey(XEKeys::N3, isDown);
			break;
		case XEVKWindows::XE_VK_KEY_4:
			m_Keyboard->PressUnpressKey(XEKeys::N4, isDown);
			break;
		case XEVKWindows::XE_VK_KEY_5:
			m_Keyboard->PressUnpressKey(XEKeys::N5, isDown);
			break;
		case XEVKWindows::XE_VK_KEY_6:
			m_Keyboard->PressUnpressKey(XEKeys::N6, isDown);
			break;
		case XEVKWindows::XE_VK_KEY_7:
			m_Keyboard->PressUnpressKey(XEKeys::N7, isDown);
			break;
		case XEVKWindows::XE_VK_KEY_8:
			m_Keyboard->PressUnpressKey(XEKeys::N8, isDown);
			break;
		case XEVKWindows::XE_VK_KEY_9:
			m_Keyboard->PressUnpressKey(XEKeys::N9, isDown);
			break;

		//Normal Letters
		case XEVKWindows::XE_VK_KEY_A:
			m_Keyboard->PressUnpressKey(XEKeys::A, isDown);
			break;
		case XEVKWindows::XE_VK_KEY_B:
			m_Keyboard->PressUnpressKey(XEKeys::B, isDown);
			break;
		case XEVKWindows::XE_VK_KEY_C:
			m_Keyboard->PressUnpressKey(XEKeys::C, isDown);
			break;
		case XEVKWindows::XE_VK_KEY_D:
			m_Keyboard->PressUnpressKey(XEKeys::D, isDown);
			break;
		case XEVKWindows::XE_VK_KEY_E:
			m_Keyboard->PressUnpressKey(XEKeys::E, isDown);
			break;
		case XEVKWindows::XE_VK_KEY_F:
			m_Keyboard->PressUnpressKey(XEKeys::F, isDown);
			break;
		case XEVKWindows::XE_VK_KEY_G:
			m_Keyboard->PressUnpressKey(XEKeys::G, isDown);
			break;
		case XEVKWindows::XE_VK_KEY_H:
			m_Keyboard->PressUnpressKey(XEKeys::H, isDown);
			break;
		case XEVKWindows::XE_VK_KEY_I:
			m_Keyboard->PressUnpressKey(XEKeys::I, isDown);
			break;
		case XEVKWindows::XE_VK_KEY_J:
			m_Keyboard->PressUnpressKey(XEKeys::J, isDown);
			break;
		case XEVKWindows::XE_VK_KEY_K:
			m_Keyboard->PressUnpressKey(XEKeys::K, isDown);
			break;
		case XEVKWindows::XE_VK_KEY_L:
			m_Keyboard->PressUnpressKey(XEKeys::L, isDown);
			break;
		case XEVKWindows::XE_VK_KEY_M:
			m_Keyboard->PressUnpressKey(XEKeys::M, isDown);
			break;
		case XEVKWindows::XE_VK_KEY_N:
			m_Keyboard->PressUnpressKey(XEKeys::N, isDown);
			break;
		case XEVKWindows::XE_VK_KEY_O:
			m_Keyboard->PressUnpressKey(XEKeys::O, isDown);
			break;
		case XEVKWindows::XE_VK_KEY_P:
			m_Keyboard->PressUnpressKey(XEKeys::P, isDown);
			break;
		case XEVKWindows::XE_VK_KEY_Q:
			m_Keyboard->PressUnpressKey(XEKeys::Q, isDown);
			break;
		case XEVKWindows::XE_VK_KEY_R:
			m_Keyboard->PressUnpressKey(XEKeys::R, isDown);
			break;
		case XEVKWindows::XE_VK_KEY_S:
			m_Keyboard->PressUnpressKey(XEKeys::S, isDown);
			break;
		case XEVKWindows::XE_VK_KEY_T:
			m_Keyboard->PressUnpressKey(XEKeys::T, isDown);
			break;
		case XEVKWindows::XE_VK_KEY_U:
			m_Keyboard->PressUnpressKey(XEKeys::U, isDown);
			break;
		case XEVKWindows::XE_VK_KEY_V:
			m_Keyboard->PressUnpressKey(XEKeys::V, isDown);
			break;
		case XEVKWindows::XE_VK_KEY_W:
			m_Keyboard->PressUnpressKey(XEKeys::W, isDown);
			break;
		case XEVKWindows::XE_VK_KEY_X:
			m_Keyboard->PressUnpressKey(XEKeys::X, isDown);
			break;
		case XEVKWindows::XE_VK_KEY_Y:
			m_Keyboard->PressUnpressKey(XEKeys::Y, isDown);
			break;
		case XEVKWindows::XE_VK_KEY_Z:
			m_Keyboard->PressUnpressKey(XEKeys::Z, isDown);
			break;

		//Numpad
		case XEVKWindows::XE_VK_KEY_NUMPAD0:
			m_Keyboard->PressUnpressKey(XEKeys::NUMPAD0, isDown);
			break;
		case XEVKWindows::XE_VK_KEY_NUMPAD1:
			m_Keyboard->PressUnpressKey(XEKeys::NUMPAD1, isDown);
			break;
		case XEVKWindows::XE_VK_KEY_NUMPAD2:
			m_Keyboard->PressUnpressKey(XEKeys::NUMPAD2, isDown);
			break;
		case XEVKWindows::XE_VK_KEY_NUMPAD3:
			m_Keyboard->PressUnpressKey(XEKeys::NUMPAD3, isDown);
			break;
		case XEVKWindows::XE_VK_KEY_NUMPAD4:
			m_Keyboard->PressUnpressKey(XEKeys::NUMPAD4, isDown);
			break;
		case XEVKWindows::XE_VK_KEY_NUMPAD5:
			m_Keyboard->PressUnpressKey(XEKeys::NUMPAD5, isDown);
			break;
		case XEVKWindows::XE_VK_KEY_NUMPAD6:
			m_Keyboard->PressUnpressKey(XEKeys::NUMPAD6, isDown);
			break;
		case XEVKWindows::XE_VK_KEY_NUMPAD7:
			m_Keyboard->PressUnpressKey(XEKeys::NUMPAD7, isDown);
			break;
		case XEVKWindows::XE_VK_KEY_NUMPAD8:
			m_Keyboard->PressUnpressKey(XEKeys::NUMPAD8, isDown);
			break;
		case XEVKWindows::XE_VK_KEY_NUMPAD9:
			m_Keyboard->PressUnpressKey(XEKeys::NUMPAD9, isDown);
			break;

		//Math
		case XEVKWindows::XE_VK_KEY_MULTIPLY:
			m_Keyboard->PressUnpressKey(XEKeys::MULTIPLY, isDown);
			break;
		case XEVKWindows::XE_VK_KEY_ADD:
			m_Keyboard->PressUnpressKey(XEKeys::ADD, isDown);
			break;
		case XEVKWindows::XE_VK_KEY_SUBTRACT:
			m_Keyboard->PressUnpressKey(XEKeys::SUBTRACT, isDown);
			break;
		case XEVKWindows::XE_VK_KEY_DECIMAL:
			m_Keyboard->PressUnpressKey(XEKeys::DECIMAL, isDown);
			break;
		case XEVKWindows::XE_VK_KEY_DIVIDE:
			m_Keyboard->PressUnpressKey(XEKeys::DIVIDE, isDown);
			break;
		case XEVKWindows::XE_VK_KEY_OEM_PLUS:
			m_Keyboard->PressUnpressKey(XEKeys::OEM_PLUS, isDown);
			break;
		case XEVKWindows::XE_VK_KEY_OEM_MINUS:
			m_Keyboard->PressUnpressKey(XEKeys::OEM_MINUS, isDown);
			break;

		//Grammar
		case XEVKWindows::XE_VK_KEY_UNDERSCORE:
			m_Keyboard->PressUnpressKey(XEKeys::UNDERSCORE, isDown);
			break;
		case XEVKWindows::XE_VK_KEY_OEM_COMMA:
			m_Keyboard->PressUnpressKey(XEKeys::OEM_COMMA, isDown);
			break;
		case XEVKWindows::XE_VK_KEY_OEM_PERIOD:
			m_Keyboard->PressUnpressKey(XEKeys::OEM_PERIOD, isDown);
			break;
		case XEVKWindows::XE_VK_KEY_SPACE:
			m_Keyboard->PressUnpressKey(XEKeys::SPACE, isDown);
			break;
		case XEVKWindows::XE_VK_KEY_BACKSPACE:
			m_Keyboard->PressUnpressKey(XEKeys::BACKSPACE, isDown);
			break;

		//Functions Keys
		case XEVKWindows::XE_VK_KEY_F1:
			m_Keyboard->PressUnpressKey(XEKeys::F1, isDown);
			break;
		case XEVKWindows::XE_VK_KEY_F2:
			m_Keyboard->PressUnpressKey(XEKeys::F2, isDown);
			break;
		case XEVKWindows::XE_VK_KEY_F3:
			m_Keyboard->PressUnpressKey(XEKeys::F3, isDown);
			break;
		case XEVKWindows::XE_VK_KEY_F4:
			m_Keyboard->PressUnpressKey(XEKeys::F4, isDown);
			break;
		case XEVKWindows::XE_VK_KEY_F5:
			m_Keyboard->PressUnpressKey(XEKeys::F5, isDown);
			break;
		case XEVKWindows::XE_VK_KEY_F6:
			m_Keyboard->PressUnpressKey(XEKeys::F6, isDown);
			break;
		case XEVKWindows::XE_VK_KEY_F7:
			m_Keyboard->PressUnpressKey(XEKeys::F7, isDown);
			break;
		case XEVKWindows::XE_VK_KEY_F8:
			m_Keyboard->PressUnpressKey(XEKeys::F8, isDown);
			break;
		case XEVKWindows::XE_VK_KEY_F9:
			m_Keyboard->PressUnpressKey(XEKeys::F9, isDown);
			break;
		case XEVKWindows::XE_VK_KEY_F10:
			m_Keyboard->PressUnpressKey(XEKeys::F10, isDown);
			break;
		case XEVKWindows::XE_VK_KEY_F11:
			m_Keyboard->PressUnpressKey(XEKeys::F11, isDown);
			break;
		case XEVKWindows::XE_VK_KEY_F12:
			m_Keyboard->PressUnpressKey(XEKeys::F12, isDown);
			break;
		case XEVKWindows::XE_VK_KEY_F13:
			m_Keyboard->PressUnpressKey(XEKeys::F13, isDown);
			break;
		case XEVKWindows::XE_VK_KEY_F14:
			m_Keyboard->PressUnpressKey(XEKeys::F14, isDown);
			break;
		case XEVKWindows::XE_VK_KEY_F15:
			m_Keyboard->PressUnpressKey(XEKeys::F15, isDown);
			break;
		case XEVKWindows::XE_VK_KEY_F16:
			m_Keyboard->PressUnpressKey(XEKeys::F16, isDown);
			break;
		case XEVKWindows::XE_VK_KEY_F17:
			m_Keyboard->PressUnpressKey(XEKeys::F17, isDown);
			break;
		case XEVKWindows::XE_VK_KEY_F18:
			m_Keyboard->PressUnpressKey(XEKeys::F18, isDown);
			break;
		case XEVKWindows::XE_VK_KEY_F19:
			m_Keyboard->PressUnpressKey(XEKeys::F19, isDown);
			break;
		case XEVKWindows::XE_VK_KEY_F20:
			m_Keyboard->PressUnpressKey(XEKeys::F20, isDown);
			break;
		case XEVKWindows::XE_VK_KEY_F21:
			m_Keyboard->PressUnpressKey(XEKeys::F21, isDown);
			break;
		case XEVKWindows::XE_VK_KEY_F22:
			m_Keyboard->PressUnpressKey(XEKeys::F22, isDown);
			break;
		case XEVKWindows::XE_VK_KEY_F23:
			m_Keyboard->PressUnpressKey(XEKeys::F23, isDown);
			break;
		case XEVKWindows::XE_VK_KEY_F24:
			m_Keyboard->PressUnpressKey(XEKeys::F24, isDown);
			break;

		//Special Keys
		case XEVKWindows::XE_VK_KEY_TAB:
			m_Keyboard->PressUnpressKey(XEKeys::TAB, isDown);
			break;
		case XEVKWindows::XE_VK_KEY_ENTER:
			if (isExtended)
			{
				m_Keyboard->PressUnpressKey(XEKeys::NUMPADENTER, isDown);
			}
			else
			{
				m_Keyboard->PressUnpressKey(XEKeys::ENTER, isDown);
			}
			break;
		case XEVKWindows::XE_VK_KEY_LSHIFT:
			if (isExtended)
			{
				m_Keyboard->PressUnpressKey(XEKeys::RSHIFT, isDown);
			}
			else
			{
				m_Keyboard->PressUnpressKey(XEKeys::LSHIFT, isDown);
			}
			break;
		case XEVKWindows::XE_VK_KEY_LCTRL:
			if (isExtended)
			{
				m_Keyboard->PressUnpressKey(XEKeys::RCTRL, isDown);
			}
			else
			{
				m_Keyboard->PressUnpressKey(XEKeys::LCTRL, isDown);
			}
			break;
		case XEVKWindows::XE_VK_KEY_LALT:
			if (isExtended)
			{
				m_Keyboard->PressUnpressKey(XEKeys::RALT, isDown);
			}
			else
			{
				m_Keyboard->PressUnpressKey(XEKeys::LALT, isDown);
			}
			break;
		case XEVKWindows::XE_VK_KEY_PAUSE:
			m_Keyboard->PressUnpressKey(XEKeys::PAUSE, isDown);
			break;
		case XEVKWindows::XE_VK_KEY_ESCAPE:
			m_Keyboard->PressUnpressKey(XEKeys::ESCAPE, isDown);
			break;
		case XEVKWindows::XE_VK_KEY_PAGEUP:
			m_Keyboard->PressUnpressKey(XEKeys::PAGEUP, isDown);
			break;
		case XEVKWindows::XE_VK_KEY_PAGEDOWN:
			m_Keyboard->PressUnpressKey(XEKeys::PAGEDOWN, isDown);
			break;
		case XEVKWindows::XE_VK_KEY_END:
			m_Keyboard->PressUnpressKey(XEKeys::END, isDown);
			break;
		case XEVKWindows::XE_VK_KEY_HOME:
			m_Keyboard->PressUnpressKey(XEKeys::HOME, isDown);
			break;
		case XEVKWindows::XE_VK_KEY_LEFT:
			m_Keyboard->PressUnpressKey(XEKeys::LEFT, isDown);
			break;
		case XEVKWindows::XE_VK_KEY_UP:
			m_Keyboard->PressUnpressKey(XEKeys::UP, isDown);
			break;
		case XEVKWindows::XE_VK_KEY_RIGHT:
			m_Keyboard->PressUnpressKey(XEKeys::RIGHT, isDown);
			break;
		case XEVKWindows::XE_VK_KEY_DOWN:
			m_Keyboard->PressUnpressKey(XEKeys::DOWN, isDown);
			break;
		case XEVKWindows::XE_VK_KEY_PRINTSCREEN:
			m_Keyboard->PressUnpressKey(XEKeys::PRINTSCREEN, isDown);
			break;
		case XEVKWindows::XE_VK_KEY_INSERT:
			m_Keyboard->PressUnpressKey(XEKeys::INSERT, isDown);
			break;
		case XEVKWindows::XE_VK_KEY_DELETE:
			m_Keyboard->PressUnpressKey(XEKeys::DEL, isDown);
			break;

		//Lock Keys
		case XEVKWindows::XE_VK_KEY_CAPSLOCK:
			m_Keyboard->PressUnpressKey(XEKeys::CAPSLOCK, isDown);
			break;
		case XEVKWindows::XE_VK_KEY_NUMLOCK:
			m_Keyboard->PressUnpressKey(XEKeys::NUMLOCK, isDown);
			break;
		case XEVKWindows::XE_VK_KEY_SCROLLLOCK:
			m_Keyboard->PressUnpressKey(XEKeys::SCROLLLOCK, isDown);
			break;

		//Music Keys
		case XEVKWindows::XE_VK_KEY_VOLUME_MUTE:
			m_Keyboard->PressUnpressKey(XEKeys::VOLUME_MUTE, isDown);
			break;
		case XEVKWindows::XE_VK_KEY_VOLUME_DOWN:
			m_Keyboard->PressUnpressKey(XEKeys::VOLUME_DOWN, isDown);
			break;
		case XEVKWindows::XE_VK_KEY_VOLUME_UP:
			m_Keyboard->PressUnpressKey(XEKeys::VOLUME_UP, isDown);
			break;
		case XEVKWindows::XE_VK_KEY_MEDIA_NEXT_TRACK:
			m_Keyboard->PressUnpressKey(XEKeys::MEDIA_NEXT_TRACK, isDown);
			break;
		case XEVKWindows::XE_VK_KEY_MEDIA_PREV_TRACK:
			m_Keyboard->PressUnpressKey(XEKeys::MEDIA_PREV_TRACK, isDown);
			break;
		case XEVKWindows::XE_VK_KEY_MEDIA_STOP:
			m_Keyboard->PressUnpressKey(XEKeys::MEDIA_STOP, isDown);
			break;
		case XEVKWindows::XE_VK_KEY_MEDIA_PLAY_PAUSE:
			m_Keyboard->PressUnpressKey(XEKeys::MEDIA_PLAY_PAUSE, isDown);
			break;

		//OEM Keys
		case XEVKWindows::XE_VK_KEY_OEM_SEMICOLON:
			if (m_Keyboard->IsShiftPressMemBuffer())
			{
				m_Keyboard->PressUnpressKey(XEKeys::OEM_COLON, isDown);
			}
			else
			{
				m_Keyboard->PressUnpressKey(XEKeys::OEM_SEMICOLON, isDown);
			}
			break;
		case XEVKWindows::XE_VK_KEY_OEM_FWR_SLASH:
			if (m_Keyboard->IsShiftPressMemBuffer())
			{
				m_Keyboard->PressUnpressKey(XEKeys::OEM_QUESTION, isDown);
			}
			else
			{
				m_Keyboard->PressUnpressKey(XEKeys::OEM_FWR_SLASH, isDown);
			}
			break;
		case XEVKWindows::XE_VK_KEY_OEM_TILDE:
			if (m_Keyboard->IsShiftPressMemBuffer())
			{
				m_Keyboard->PressUnpressKey(XEKeys::OEM_BACK_QUOTE, isDown);
			}
			else
			{
				m_Keyboard->PressUnpressKey(XEKeys::OEM_TILDE, isDown);
			}
			break;
		case XEVKWindows::XE_VK_KEY_OEM_OPEN_BRACKET:
			if (m_Keyboard->IsShiftPressMemBuffer())
			{
				m_Keyboard->PressUnpressKey(XEKeys::OEM_OPEN_BRACE, isDown);
			}
			else
			{
				m_Keyboard->PressUnpressKey(XEKeys::OEM_OPEN_BRACKET, isDown);
			}
			break;
		case XEVKWindows::XE_VK_KEY_OEM_BACK_SLASH:
			if (m_Keyboard->IsShiftPressMemBuffer())
			{
				m_Keyboard->PressUnpressKey(XEKeys::OEM_PIPE, isDown);
			}
			else
			{
				m_Keyboard->PressUnpressKey(XEKeys::OEM_BACK_SLASH, isDown);
			}
			break;
		case XEVKWindows::XE_VK_KEY_OEM_CLOSE_BRACKET:
			if (m_Keyboard->IsShiftPressMemBuffer())
			{
				m_Keyboard->PressUnpressKey(XEKeys::OEM_CLOSE_BRACE, isDown);
			}
			else
			{
				m_Keyboard->PressUnpressKey(XEKeys::OEM_CLOSE_BRACKET, isDown);
			}
			break;
		case XEVKWindows::XE_VK_KEY_OEM_SINGLE_QUOTE:
			if (m_Keyboard->IsShiftPressMemBuffer())
			{
				m_Keyboard->PressUnpressKey(XEKeys::OEM_DOUBLE_QUOTE, isDown);
			}
			else
			{
				m_Keyboard->PressUnpressKey(XEKeys::OEM_SINGLE_QUOTE, isDown);
			}
			break;
		case XEVKWindows::XE_VK_KEY_OEM_1:
			m_Keyboard->PressUnpressKey(XEKeys::OEM_1, isDown);
			break;
		case XEVKWindows::XE_VK_KEY_OEM_2:
			m_Keyboard->PressUnpressKey(XEKeys::OEM_2, isDown);
			break;
		case XEVKWindows::XE_VK_KEY_OEM_3:
			m_Keyboard->PressUnpressKey(XEKeys::OEM_3, isDown);
			break;
		case XEVKWindows::XE_VK_KEY_OEM_4:
			m_Keyboard->PressUnpressKey(XEKeys::OEM_4, isDown);
			break;
		case XEVKWindows::XE_VK_KEY_OEM_5:
			m_Keyboard->PressUnpressKey(XEKeys::OEM_5, isDown);
			break;
		case XEVKWindows::XE_VK_KEY_OEM_6:
			m_Keyboard->PressUnpressKey(XEKeys::OEM_6, isDown);
			break;
		case XEVKWindows::XE_VK_KEY_OEM_7:
			m_Keyboard->PressUnpressKey(XEKeys::OEM_7, isDown);
			break;
		case XEVKWindows::XE_VK_KEY_OEM_8:
			m_Keyboard->PressUnpressKey(XEKeys::OEM_8, isDown);
			break;
		case XEVKWindows::XE_VK_KEY_OEM_9:
			m_Keyboard->PressUnpressKey(XEKeys::OEM_9, isDown);
			break;
		case XEVKWindows::XE_VK_KEY_OEM_10:
			m_Keyboard->PressUnpressKey(XEKeys::OEM_10, isDown);
			break;
		case XEVKWindows::XE_VK_KEY_OEM_11:
			m_Keyboard->PressUnpressKey(XEKeys::OEM_11, isDown);
			break;
		case XEVKWindows::XE_VK_KEY_OEM_12:
			m_Keyboard->PressUnpressKey(XEKeys::OEM_12, isDown);
			break;
		case XEVKWindows::XE_VK_KEY_OEM_13:
			m_Keyboard->PressUnpressKey(XEKeys::OEM_13, isDown);
			break;
		case XEVKWindows::XE_VK_KEY_OEM_14:
			m_Keyboard->PressUnpressKey(XEKeys::OEM_14, isDown);
			break;
		case XEVKWindows::XE_VK_KEY_OEM_15:
			m_Keyboard->PressUnpressKey(XEKeys::OEM_15, isDown);
			break;
		case XEVKWindows::XE_VK_KEY_OEM_16:
			m_Keyboard->PressUnpressKey(XEKeys::OEM_16, isDown);
			break;
		case XEVKWindows::XE_VK_KEY_OEM_17:
			m_Keyboard->PressUnpressKey(XEKeys::OEM_17, isDown);
			break;
		case XEVKWindows::XE_VK_KEY_OEM_18:
			m_Keyboard->PressUnpressKey(XEKeys::OEM_18, isDown);
			break;
		case XEVKWindows::XE_VK_KEY_OEM_19:
			m_Keyboard->PressUnpressKey(XEKeys::OEM_19, isDown);
			break;
		case XEVKWindows::XE_VK_KEY_OEM_20:
			m_Keyboard->PressUnpressKey(XEKeys::OEM_20, isDown);
			break;
		case XEVKWindows::XE_VK_KEY_OEM_21:
			m_Keyboard->PressUnpressKey(XEKeys::OEM_21, isDown);
			break;
		case XEVKWindows::XE_VK_KEY_OEM_22:
			m_Keyboard->PressUnpressKey(XEKeys::OEM_22, isDown);
			break;
		case XEVKWindows::XE_VK_KEY_OEM_23:
			m_Keyboard->PressUnpressKey(XEKeys::OEM_23, isDown);
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