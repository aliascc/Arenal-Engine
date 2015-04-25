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
#include "Base\BaseFunctions.h"

//Always include last
#include "Memory\MemLeaks.h"

/********************
*   Function Defs   *
*********************/
Keyboard::Keyboard()
{
	InitBuffers();
	InitKeyVKMap();
}

Keyboard::~Keyboard()
{
}

void Keyboard::InitBuffers()
{
	memset(m_InMemBuffer, 0, sizeof(uint8_t) * XE_MAX_KEYS);
	memset(m_PreviousKeyboardState, 0, sizeof(uint8_t) *  XE_MAX_KEYS);
	memset(m_CurrentKeyboardState, 0, sizeof(uint8_t) *  XE_MAX_KEYS);
}

void Keyboard::InitKeyVKMap()
{
	m_KeyMapVK[(uint32_t)XEKeys::N0]				= XEVKWindows::XE_VK_KEY_0;
	m_KeyMapVK[(uint32_t)XEKeys::N1]				= XEVKWindows::XE_VK_KEY_1;
	m_KeyMapVK[(uint32_t)XEKeys::N2]				= XEVKWindows::XE_VK_KEY_2;
	m_KeyMapVK[(uint32_t)XEKeys::N3]				= XEVKWindows::XE_VK_KEY_3;
	m_KeyMapVK[(uint32_t)XEKeys::N4]				= XEVKWindows::XE_VK_KEY_4;
	m_KeyMapVK[(uint32_t)XEKeys::N5]				= XEVKWindows::XE_VK_KEY_5;
	m_KeyMapVK[(uint32_t)XEKeys::N6]				= XEVKWindows::XE_VK_KEY_6;
	m_KeyMapVK[(uint32_t)XEKeys::N7]				= XEVKWindows::XE_VK_KEY_7;
	m_KeyMapVK[(uint32_t)XEKeys::N8]				= XEVKWindows::XE_VK_KEY_8;
	m_KeyMapVK[(uint32_t)XEKeys::N9]				= XEVKWindows::XE_VK_KEY_9;
	m_KeyMapVK[(uint32_t)XEKeys::A]					= XEVKWindows::XE_VK_KEY_A;
	m_KeyMapVK[(uint32_t)XEKeys::B]					= XEVKWindows::XE_VK_KEY_B;
	m_KeyMapVK[(uint32_t)XEKeys::C]					= XEVKWindows::XE_VK_KEY_C;
	m_KeyMapVK[(uint32_t)XEKeys::D]					= XEVKWindows::XE_VK_KEY_D;
	m_KeyMapVK[(uint32_t)XEKeys::E]					= XEVKWindows::XE_VK_KEY_E;
	m_KeyMapVK[(uint32_t)XEKeys::F]					= XEVKWindows::XE_VK_KEY_F;
	m_KeyMapVK[(uint32_t)XEKeys::G]					= XEVKWindows::XE_VK_KEY_G;
	m_KeyMapVK[(uint32_t)XEKeys::H]					= XEVKWindows::XE_VK_KEY_H;
	m_KeyMapVK[(uint32_t)XEKeys::I]					= XEVKWindows::XE_VK_KEY_I;
	m_KeyMapVK[(uint32_t)XEKeys::J]					= XEVKWindows::XE_VK_KEY_J;
	m_KeyMapVK[(uint32_t)XEKeys::K]					= XEVKWindows::XE_VK_KEY_K;
	m_KeyMapVK[(uint32_t)XEKeys::L]					= XEVKWindows::XE_VK_KEY_L;
	m_KeyMapVK[(uint32_t)XEKeys::M]					= XEVKWindows::XE_VK_KEY_M;
	m_KeyMapVK[(uint32_t)XEKeys::N]					= XEVKWindows::XE_VK_KEY_N;
	m_KeyMapVK[(uint32_t)XEKeys::O]					= XEVKWindows::XE_VK_KEY_O;
	m_KeyMapVK[(uint32_t)XEKeys::P]					= XEVKWindows::XE_VK_KEY_P;
	m_KeyMapVK[(uint32_t)XEKeys::Q]					= XEVKWindows::XE_VK_KEY_Q;
	m_KeyMapVK[(uint32_t)XEKeys::R]					= XEVKWindows::XE_VK_KEY_R;
	m_KeyMapVK[(uint32_t)XEKeys::S]					= XEVKWindows::XE_VK_KEY_S;
	m_KeyMapVK[(uint32_t)XEKeys::T]					= XEVKWindows::XE_VK_KEY_T;
	m_KeyMapVK[(uint32_t)XEKeys::U]					= XEVKWindows::XE_VK_KEY_U;
	m_KeyMapVK[(uint32_t)XEKeys::V]					= XEVKWindows::XE_VK_KEY_V;
	m_KeyMapVK[(uint32_t)XEKeys::W]					= XEVKWindows::XE_VK_KEY_W;
	m_KeyMapVK[(uint32_t)XEKeys::X]					= XEVKWindows::XE_VK_KEY_X;
	m_KeyMapVK[(uint32_t)XEKeys::Y]					= XEVKWindows::XE_VK_KEY_Y;
	m_KeyMapVK[(uint32_t)XEKeys::Z]					= XEVKWindows::XE_VK_KEY_Z;
	m_KeyMapVK[(uint32_t)XEKeys::NUMPAD0]			= XEVKWindows::XE_VK_KEY_NUMPAD0;
	m_KeyMapVK[(uint32_t)XEKeys::NUMPAD1]			= XEVKWindows::XE_VK_KEY_NUMPAD1;
	m_KeyMapVK[(uint32_t)XEKeys::NUMPAD2]			= XEVKWindows::XE_VK_KEY_NUMPAD2;
	m_KeyMapVK[(uint32_t)XEKeys::NUMPAD3]			= XEVKWindows::XE_VK_KEY_NUMPAD3;
	m_KeyMapVK[(uint32_t)XEKeys::NUMPAD4]			= XEVKWindows::XE_VK_KEY_NUMPAD4;
	m_KeyMapVK[(uint32_t)XEKeys::NUMPAD5]			= XEVKWindows::XE_VK_KEY_NUMPAD5;
	m_KeyMapVK[(uint32_t)XEKeys::NUMPAD6]			= XEVKWindows::XE_VK_KEY_NUMPAD6;
	m_KeyMapVK[(uint32_t)XEKeys::NUMPAD7]			= XEVKWindows::XE_VK_KEY_NUMPAD7;
	m_KeyMapVK[(uint32_t)XEKeys::NUMPAD8]			= XEVKWindows::XE_VK_KEY_NUMPAD8;
	m_KeyMapVK[(uint32_t)XEKeys::NUMPAD9]			= XEVKWindows::XE_VK_KEY_NUMPAD9;
	m_KeyMapVK[(uint32_t)XEKeys::MULTIPLY]			= XEVKWindows::XE_VK_KEY_MULTIPLY;
	m_KeyMapVK[(uint32_t)XEKeys::ADD]				= XEVKWindows::XE_VK_KEY_ADD;
	m_KeyMapVK[(uint32_t)XEKeys::SUBTRACT]			= XEVKWindows::XE_VK_KEY_SUBTRACT;
	m_KeyMapVK[(uint32_t)XEKeys::DECIMAL]			= XEVKWindows::XE_VK_KEY_DECIMAL;
	m_KeyMapVK[(uint32_t)XEKeys::DIVIDE]			= XEVKWindows::XE_VK_KEY_DIVIDE;
	m_KeyMapVK[(uint32_t)XEKeys::OEM_PLUS]			= XEVKWindows::XE_VK_KEY_OEM_PLUS;
	m_KeyMapVK[(uint32_t)XEKeys::OEM_MINUS]			= XEVKWindows::XE_VK_KEY_OEM_MINUS;
	m_KeyMapVK[(uint32_t)XEKeys::UNDERSCORE]		= XEVKWindows::XE_VK_KEY_UNDERSCORE;
	m_KeyMapVK[(uint32_t)XEKeys::OEM_COMMA]			= XEVKWindows::XE_VK_KEY_OEM_COMMA;
	m_KeyMapVK[(uint32_t)XEKeys::OEM_PERIOD]		= XEVKWindows::XE_VK_KEY_OEM_PERIOD;
	m_KeyMapVK[(uint32_t)XEKeys::SPACE]				= XEVKWindows::XE_VK_KEY_SPACE;
	m_KeyMapVK[(uint32_t)XEKeys::BACKSPACE]			= XEVKWindows::XE_VK_KEY_BACKSPACE;
	m_KeyMapVK[(uint32_t)XEKeys::F1]				= XEVKWindows::XE_VK_KEY_F1;
	m_KeyMapVK[(uint32_t)XEKeys::F2]				= XEVKWindows::XE_VK_KEY_F2;
	m_KeyMapVK[(uint32_t)XEKeys::F3]				= XEVKWindows::XE_VK_KEY_F3;
	m_KeyMapVK[(uint32_t)XEKeys::F4]				= XEVKWindows::XE_VK_KEY_F4;
	m_KeyMapVK[(uint32_t)XEKeys::F5]				= XEVKWindows::XE_VK_KEY_F5;
	m_KeyMapVK[(uint32_t)XEKeys::F6]				= XEVKWindows::XE_VK_KEY_F6;
	m_KeyMapVK[(uint32_t)XEKeys::F7]				= XEVKWindows::XE_VK_KEY_F7;
	m_KeyMapVK[(uint32_t)XEKeys::F8]				= XEVKWindows::XE_VK_KEY_F8;
	m_KeyMapVK[(uint32_t)XEKeys::F9]				= XEVKWindows::XE_VK_KEY_F9;
	m_KeyMapVK[(uint32_t)XEKeys::F10]				= XEVKWindows::XE_VK_KEY_F10;
	m_KeyMapVK[(uint32_t)XEKeys::F11]				= XEVKWindows::XE_VK_KEY_F11;
	m_KeyMapVK[(uint32_t)XEKeys::F12]				= XEVKWindows::XE_VK_KEY_F12;
	m_KeyMapVK[(uint32_t)XEKeys::F13]				= XEVKWindows::XE_VK_KEY_F13;
	m_KeyMapVK[(uint32_t)XEKeys::F14]				= XEVKWindows::XE_VK_KEY_F14;
	m_KeyMapVK[(uint32_t)XEKeys::F15]				= XEVKWindows::XE_VK_KEY_F15;
	m_KeyMapVK[(uint32_t)XEKeys::F16]				= XEVKWindows::XE_VK_KEY_F16;
	m_KeyMapVK[(uint32_t)XEKeys::F17]				= XEVKWindows::XE_VK_KEY_F17;
	m_KeyMapVK[(uint32_t)XEKeys::F18]				= XEVKWindows::XE_VK_KEY_F18;
	m_KeyMapVK[(uint32_t)XEKeys::F19]				= XEVKWindows::XE_VK_KEY_F19;
	m_KeyMapVK[(uint32_t)XEKeys::F20]				= XEVKWindows::XE_VK_KEY_F20;
	m_KeyMapVK[(uint32_t)XEKeys::F21]				= XEVKWindows::XE_VK_KEY_F21;
	m_KeyMapVK[(uint32_t)XEKeys::F22]				= XEVKWindows::XE_VK_KEY_F22;
	m_KeyMapVK[(uint32_t)XEKeys::F23]				= XEVKWindows::XE_VK_KEY_F23;
	m_KeyMapVK[(uint32_t)XEKeys::F24]				= XEVKWindows::XE_VK_KEY_F24;
	m_KeyMapVK[(uint32_t)XEKeys::TAB]				= XEVKWindows::XE_VK_KEY_TAB;
	m_KeyMapVK[(uint32_t)XEKeys::ENTER]				= XEVKWindows::XE_VK_KEY_ENTER;
	m_KeyMapVK[(uint32_t)XEKeys::NUMPADENTER]		= XEVKWindows::XE_VK_KEY_NUMPADENTER;
	m_KeyMapVK[(uint32_t)XEKeys::LSHIFT]		 	= XEVKWindows::XE_VK_KEY_LSHIFT;
	m_KeyMapVK[(uint32_t)XEKeys::RSHIFT]		 	= XEVKWindows::XE_VK_KEY_RSHIFT;
	m_KeyMapVK[(uint32_t)XEKeys::LCTRL]		 		= XEVKWindows::XE_VK_KEY_LCTRL;
	m_KeyMapVK[(uint32_t)XEKeys::RCTRL]		 		= XEVKWindows::XE_VK_KEY_RCTRL;
	m_KeyMapVK[(uint32_t)XEKeys::LALT]			 	= XEVKWindows::XE_VK_KEY_LALT;
	m_KeyMapVK[(uint32_t)XEKeys::RALT]			 	= XEVKWindows::XE_VK_KEY_RALT;
	m_KeyMapVK[(uint32_t)XEKeys::PAUSE]		 		= XEVKWindows::XE_VK_KEY_PAUSE;
	m_KeyMapVK[(uint32_t)XEKeys::ESCAPE]		 	= XEVKWindows::XE_VK_KEY_ESCAPE;
	m_KeyMapVK[(uint32_t)XEKeys::PAGEUP]		 	= XEVKWindows::XE_VK_KEY_PAGEUP;
	m_KeyMapVK[(uint32_t)XEKeys::PAGEDOWN]		 	= XEVKWindows::XE_VK_KEY_PAGEDOWN;
	m_KeyMapVK[(uint32_t)XEKeys::END]			 	= XEVKWindows::XE_VK_KEY_END;
	m_KeyMapVK[(uint32_t)XEKeys::HOME]			 	= XEVKWindows::XE_VK_KEY_HOME;
	m_KeyMapVK[(uint32_t)XEKeys::LEFT]			 	= XEVKWindows::XE_VK_KEY_LEFT;
	m_KeyMapVK[(uint32_t)XEKeys::UP]			 	= XEVKWindows::XE_VK_KEY_UP;
	m_KeyMapVK[(uint32_t)XEKeys::RIGHT]		 		= XEVKWindows::XE_VK_KEY_RIGHT;
	m_KeyMapVK[(uint32_t)XEKeys::DOWN]			 	= XEVKWindows::XE_VK_KEY_DOWN;
	m_KeyMapVK[(uint32_t)XEKeys::PRINTSCREEN]	 	= XEVKWindows::XE_VK_KEY_PRINTSCREEN;
	m_KeyMapVK[(uint32_t)XEKeys::INSERT]		 	= XEVKWindows::XE_VK_KEY_INSERT;
	m_KeyMapVK[(uint32_t)XEKeys::DEL]				= XEVKWindows::XE_VK_KEY_DELETE;
	m_KeyMapVK[(uint32_t)XEKeys::CAPSLOCK]		 	= XEVKWindows::XE_VK_KEY_CAPSLOCK;
	m_KeyMapVK[(uint32_t)XEKeys::NUMLOCK]		 	= XEVKWindows::XE_VK_KEY_NUMLOCK;
	m_KeyMapVK[(uint32_t)XEKeys::SCROLLLOCK]	 	= XEVKWindows::XE_VK_KEY_SCROLLLOCK;
	m_KeyMapVK[(uint32_t)XEKeys::VOLUME_MUTE]	 	= XEVKWindows::XE_VK_KEY_VOLUME_MUTE;
	m_KeyMapVK[(uint32_t)XEKeys::VOLUME_DOWN]	 	= XEVKWindows::XE_VK_KEY_VOLUME_DOWN;
	m_KeyMapVK[(uint32_t)XEKeys::VOLUME_UP]	 		= XEVKWindows::XE_VK_KEY_VOLUME_UP;
	m_KeyMapVK[(uint32_t)XEKeys::MEDIA_NEXT_TRACK] 	= XEVKWindows::XE_VK_KEY_MEDIA_NEXT_TRACK;
	m_KeyMapVK[(uint32_t)XEKeys::MEDIA_PREV_TRACK] 	= XEVKWindows::XE_VK_KEY_MEDIA_PREV_TRACK;
	m_KeyMapVK[(uint32_t)XEKeys::MEDIA_STOP]	 	= XEVKWindows::XE_VK_KEY_MEDIA_STOP;
	m_KeyMapVK[(uint32_t)XEKeys::MEDIA_PLAY_PAUSE] 	= XEVKWindows::XE_VK_KEY_MEDIA_PLAY_PAUSE;
	m_KeyMapVK[(uint32_t)XEKeys::OEM_SEMICOLON] 	= XEVKWindows::XE_VK_KEY_OEM_SEMICOLON;
	m_KeyMapVK[(uint32_t)XEKeys::OEM_COLON]	 		= XEVKWindows::XE_VK_KEY_OEM_COLON;
	m_KeyMapVK[(uint32_t)XEKeys::OEM_FWR_SLASH] 	= XEVKWindows::XE_VK_KEY_OEM_FWR_SLASH;
	m_KeyMapVK[(uint32_t)XEKeys::OEM_QUESTION]		= XEVKWindows::XE_VK_KEY_OEM_QUESTION;
	m_KeyMapVK[(uint32_t)XEKeys::OEM_BACK_QUOTE]	= XEVKWindows::XE_VK_KEY_OEM_BACK_QUOTE;
	m_KeyMapVK[(uint32_t)XEKeys::OEM_TILDE]			= XEVKWindows::XE_VK_KEY_OEM_TILDE;
	m_KeyMapVK[(uint32_t)XEKeys::OEM_OPEN_BRACKET]	= XEVKWindows::XE_VK_KEY_OEM_OPEN_BRACKET;
	m_KeyMapVK[(uint32_t)XEKeys::OEM_OPEN_BRACE]	= XEVKWindows::XE_VK_KEY_OEM_OPEN_BRACE;
	m_KeyMapVK[(uint32_t)XEKeys::OEM_BACK_SLASH]	= XEVKWindows::XE_VK_KEY_OEM_BACK_SLASH;
	m_KeyMapVK[(uint32_t)XEKeys::OEM_PIPE]			= XEVKWindows::XE_VK_KEY_OEM_PIPE;
	m_KeyMapVK[(uint32_t)XEKeys::OEM_CLOSE_BRACKET]	= XEVKWindows::XE_VK_KEY_OEM_CLOSE_BRACKET;
	m_KeyMapVK[(uint32_t)XEKeys::OEM_CLOSE_BRACE]	= XEVKWindows::XE_VK_KEY_OEM_CLOSE_BRACE;
	m_KeyMapVK[(uint32_t)XEKeys::OEM_SINGLE_QUOTE]	= XEVKWindows::XE_VK_KEY_OEM_SINGLE_QUOTE;
	m_KeyMapVK[(uint32_t)XEKeys::OEM_DOUBLE_QUOTE]	= XEVKWindows::XE_VK_KEY_OEM_DOUBLE_QUOTE;
	m_KeyMapVK[(uint32_t)XEKeys::OEM_1]				= XEVKWindows::XE_VK_KEY_OEM_1;
	m_KeyMapVK[(uint32_t)XEKeys::OEM_2]				= XEVKWindows::XE_VK_KEY_OEM_2;
	m_KeyMapVK[(uint32_t)XEKeys::OEM_3]				= XEVKWindows::XE_VK_KEY_OEM_3;
	m_KeyMapVK[(uint32_t)XEKeys::OEM_4]				= XEVKWindows::XE_VK_KEY_OEM_4;
	m_KeyMapVK[(uint32_t)XEKeys::OEM_5]				= XEVKWindows::XE_VK_KEY_OEM_5;
	m_KeyMapVK[(uint32_t)XEKeys::OEM_6]				= XEVKWindows::XE_VK_KEY_OEM_6;
	m_KeyMapVK[(uint32_t)XEKeys::OEM_7]				= XEVKWindows::XE_VK_KEY_OEM_7;
	m_KeyMapVK[(uint32_t)XEKeys::OEM_8]				= XEVKWindows::XE_VK_KEY_OEM_8;
	m_KeyMapVK[(uint32_t)XEKeys::OEM_9]				= XEVKWindows::XE_VK_KEY_OEM_9;
	m_KeyMapVK[(uint32_t)XEKeys::OEM_10]			= XEVKWindows::XE_VK_KEY_OEM_10;
	m_KeyMapVK[(uint32_t)XEKeys::OEM_11]			= XEVKWindows::XE_VK_KEY_OEM_11;
	m_KeyMapVK[(uint32_t)XEKeys::OEM_12]			= XEVKWindows::XE_VK_KEY_OEM_12;
	m_KeyMapVK[(uint32_t)XEKeys::OEM_13]			= XEVKWindows::XE_VK_KEY_OEM_13;
	m_KeyMapVK[(uint32_t)XEKeys::OEM_14]			= XEVKWindows::XE_VK_KEY_OEM_14;
	m_KeyMapVK[(uint32_t)XEKeys::OEM_15]			= XEVKWindows::XE_VK_KEY_OEM_15;
	m_KeyMapVK[(uint32_t)XEKeys::OEM_16]			= XEVKWindows::XE_VK_KEY_OEM_16;
	m_KeyMapVK[(uint32_t)XEKeys::OEM_17]			= XEVKWindows::XE_VK_KEY_OEM_17;
	m_KeyMapVK[(uint32_t)XEKeys::OEM_18]			= XEVKWindows::XE_VK_KEY_OEM_18;
	m_KeyMapVK[(uint32_t)XEKeys::OEM_19]			= XEVKWindows::XE_VK_KEY_OEM_19;
	m_KeyMapVK[(uint32_t)XEKeys::OEM_20]			= XEVKWindows::XE_VK_KEY_OEM_20;
	m_KeyMapVK[(uint32_t)XEKeys::OEM_21]			= XEVKWindows::XE_VK_KEY_OEM_21;
	m_KeyMapVK[(uint32_t)XEKeys::OEM_22]			= XEVKWindows::XE_VK_KEY_OEM_22;
	m_KeyMapVK[(uint32_t)XEKeys::OEM_23]			= XEVKWindows::XE_VK_KEY_OEM_23;
}

XEResult Keyboard::Initialize()
{
	std::lock_guard<std::mutex> lock(m_KeyboardMutex);

	//Get Keyboard Layout
	m_KeyboardLayout = GetKeyboardLayout(0);

	return XEResult::Ok;
}

XEResult Keyboard::LockKeyboard(uint64_t& lockID)
{
	std::lock_guard<std::mutex> lock(m_KeyboardMutex);

	if(m_LockID != 0)
	{
		return XEResult::KeyboardLockFailed;
	}

	m_LockID = lockID = XE_Base::GetNextUniqueID();

	return XEResult::Ok;
}

XEResult Keyboard::UnlockKeyboard(uint64_t lockID)
{
	std::lock_guard<std::mutex> lock(m_KeyboardMutex);

	if(m_LockID != lockID)
	{
		return XEResult::KeyboardUnLockFailed;
	}

	m_LockID = 0;

	return XEResult::Ok;
}

bool Keyboard::IsShiftPressMemBuffer() const
{
	return ( (m_InMemBuffer[(uint32_t)XEKeys::LSHIFT] & XE_INPUT_PRESS) || (m_InMemBuffer[(uint32_t)XEKeys::RSHIFT] & XE_INPUT_PRESS));
}

void Keyboard::PressUnpressKey(XEKeys key, bool isDown)
{
	if(isDown)
	{
		PressKey(key);
	}
	else
	{
		UnpressKey(key);
	}
}

void Keyboard::PressKey(XEKeys key)
{
	std::lock_guard<std::mutex> lock(m_KeyboardMutex);

	m_InMemBuffer[(uint32_t)key] |= XE_INPUT_PRESS;
}

void Keyboard::UnpressKey(XEKeys key)
{
	std::lock_guard<std::mutex> lock(m_KeyboardMutex);

	m_InMemBuffer[(uint32_t)key] &= XE_DEV_INPUT_UNPRESS;
}

bool Keyboard::WasKeyPressed(XEKeys key, uint64_t lockID) const
{
	if(m_LockID != 0 && m_LockID != lockID)
	{
		return false;
	}

	if(!(m_PreviousKeyboardState[(uint32_t)key] & XE_INPUT_PRESS) && (m_CurrentKeyboardState[(uint32_t)key] & XE_INPUT_PRESS))
	{
		return true;
	}

	return false;
}

bool Keyboard::IsKeyUp(XEKeys key, uint64_t lockID) const
{
	if(m_LockID != 0 && m_LockID != lockID)
	{
		return false;
	}

	if(!(m_CurrentKeyboardState[(uint32_t)key] & XE_INPUT_PRESS))
	{
		return true;
	}

	return false;
}

bool Keyboard::IsKeyDown(XEKeys key, uint64_t lockID) const
{
	if(m_LockID != 0 && m_LockID != lockID)
	{
		return false;
	}

	if(m_CurrentKeyboardState[(uint32_t)key] & XE_INPUT_PRESS)
	{
		return true;
	}

	return false;
}

bool Keyboard::IsHoldingKey(XEKeys key, uint64_t lockID) const
{
	if(m_LockID != 0 && m_LockID != lockID)
	{
		return false;
	}

	if((m_PreviousKeyboardState[(uint32_t)key] & XE_INPUT_PRESS) && (m_CurrentKeyboardState[(uint32_t)key] & XE_INPUT_PRESS))
	{
		return true;
	}

	return false;
}

bool Keyboard::HasReleasedKey(XEKeys key, uint64_t lockID) const
{
	if(m_LockID != 0 && m_LockID != lockID)
	{
		return false;
	}

	if((m_PreviousKeyboardState[(uint32_t)key] & XE_INPUT_PRESS) && !(m_CurrentKeyboardState[(uint32_t)key] & XE_INPUT_PRESS))
	{
		return true;
	}

	return false;
}

wchar_t Keyboard::GetCurrentPressedChar(uint64_t lockID) const
{
	if(m_LockID != 0 && m_LockID != lockID)
	{
		return L'';
	}

	//ToUnicodeEx returns a key for ctrl + ANYKEY, we do not want any, so it if it is press, return no key
	if(m_CurrentKeyboardState[(uint32_t)XEKeys::LCTRL] & XE_INPUT_PRESS || m_CurrentKeyboardState[(uint32_t)XEKeys::RCTRL] & XE_INPUT_PRESS)
	{
		return L'';
	}

	//ToUnicodeEx returns a key for backspace, we do not want any, so it if it is press, return no key
	if(m_CurrentKeyboardState[(uint32_t)XEKeys::BACKSPACE] & XE_INPUT_PRESS)
	{
		return L'';
	}

	BYTE state[256];
	memset(state, 0, sizeof(BYTE) * 256);

	if (GetKeyboardState(state) == FALSE)
	{
		return L'';
	}

	//For Shift/Ctrl/Alt/Caps Keys we need to set them
	//0x80 = down 
	//0x01 = toggled (for Caps)
	state[VK_CAPITAL] = (BYTE)GetKeyState(VK_CAPITAL);
	state[VK_SHIFT] = (BYTE)GetKeyState(VK_SHIFT);

	for(uint32_t i = 0; i < XE_MAX_KEYS; ++i)
	{
		if(!(m_PreviousKeyboardState[i] & XE_INPUT_PRESS) && (m_CurrentKeyboardState[i] & XE_INPUT_PRESS))
		{
			uint32_t vk = (uint32_t)m_KeyMapVK[i];

			uint32_t scanCode = MapVirtualKeyEx(vk, MAPVK_VK_TO_VSC, m_KeyboardLayout);
			wchar_t result[] = { 0, 0 };

			if (ToUnicodeEx(vk, scanCode, state, result, 2, 0, m_KeyboardLayout) > 0)
			{
				return result[0];
			}
		}
	}

	return L'';
}

void Keyboard::Update()
{
	std::lock_guard<std::mutex> lock(m_KeyboardMutex);

	HRESULT hr = S_OK;
	
	//Copy Current Key Space to Previous
	memcpy(m_PreviousKeyboardState, m_CurrentKeyboardState, XE_MAX_KEYS);

	//Copy In Memory Buffer to Current
	memcpy(m_CurrentKeyboardState, m_InMemBuffer, XE_MAX_KEYS);
}

void Keyboard::LoseFocus()
{
	//If Keyboard Looses Focus, clean memory buffer
	ZeroMemory(m_InMemBuffer, XE_MAX_KEYS);
}

