/********************************************************
*
* Author: Carlos Chacón N.
*
* Created: 2/21/2012
*
* Desc:
*
*
* Copyright 2012
*
*********************************************************/
#pragma once

#ifndef _INPUT_DEFS_H
#define _INPUT_DEFS_H

/**********************
*   System Includes   *
***********************/
#include <stdint.h>
#include <Windows.h>
#include <Xinput.h>

/*************************
*   3rd Party Includes   *
**************************/

/***************************
*   Game Engine Includes   *
****************************/
#include "Base\XEObject.h"

/**************
*   Defines   *
***************/

/// <summary>
/// Define bit for when a key/button has been pressed
/// </summary>
#define XE_INPUT_PRESS					0x01

/// <summary>
/// Define bit for when a developer sends a command to un-press a key/button.
/// </summary>
#define XE_DEV_INPUT_UNPRESS			0xFE

/// <summary>
/// Default Time to wait between XBox Gamepad initialize retries.
/// </summary>
#define XE_XBOX_GAMEPAD_INIT_WAIT_TIME	5.0f

/***************
*   Typedefs   *
****************/

/****************
*   Constants   *
*****************/

/************
*   Enums   *
*************/

/// <summary>
/// Windows Virtual Key Codes: http://msdn.microsoft.com/en-us/library/windows/desktop/dd375731(v=vs.85).aspx
/// Extracted From WinUser.h
/// </summary>
enum class XEVKWindows : uint16_t
{
	XE_VK_KEY_0 = 0x0030,
	XE_VK_KEY_1 = 0x0031,
	XE_VK_KEY_2 = 0x0032,
	XE_VK_KEY_3 = 0x0033,
	XE_VK_KEY_4 = 0x0034,
	XE_VK_KEY_5 = 0x0035,
	XE_VK_KEY_6 = 0x0036,
	XE_VK_KEY_7 = 0x0037,
	XE_VK_KEY_8 = 0x0038,
	XE_VK_KEY_9 = 0x0039,
	XE_VK_KEY_A = 0x0041,
	XE_VK_KEY_B = 0x0042,
	XE_VK_KEY_C = 0x0043,
	XE_VK_KEY_D = 0x0044,
	XE_VK_KEY_E = 0x0045,
	XE_VK_KEY_F = 0x0046,
	XE_VK_KEY_G = 0x0047,
	XE_VK_KEY_H = 0x0048,
	XE_VK_KEY_I = 0x0049,
	XE_VK_KEY_J = 0x004A,
	XE_VK_KEY_K = 0x004B,
	XE_VK_KEY_L = 0x004C,
	XE_VK_KEY_M = 0x004D,
	XE_VK_KEY_N = 0x004E,
	XE_VK_KEY_O = 0x004F,
	XE_VK_KEY_P = 0x0050,
	XE_VK_KEY_Q = 0x0051,
	XE_VK_KEY_R = 0x0052,
	XE_VK_KEY_S = 0x0053,
	XE_VK_KEY_T = 0x0054,
	XE_VK_KEY_U = 0x0055,
	XE_VK_KEY_V = 0x0056,
	XE_VK_KEY_W = 0x0057,
	XE_VK_KEY_X = 0x0058,
	XE_VK_KEY_Y = 0x0059,
	XE_VK_KEY_Z = 0x005A,
	XE_VK_KEY_NUMPAD0 = 0x0060,
	XE_VK_KEY_NUMPAD1 = 0x0061,
	XE_VK_KEY_NUMPAD2 = 0x0062,
	XE_VK_KEY_NUMPAD3 = 0x0063,
	XE_VK_KEY_NUMPAD4 = 0x0064,
	XE_VK_KEY_NUMPAD5 = 0x0065,
	XE_VK_KEY_NUMPAD6 = 0x0066,
	XE_VK_KEY_NUMPAD7 = 0x0067,
	XE_VK_KEY_NUMPAD8 = 0x0068,
	XE_VK_KEY_NUMPAD9 = 0x0069,
	XE_VK_KEY_MULTIPLY = 0x006A,
	XE_VK_KEY_ADD = 0x006B,
	XE_VK_KEY_SUBTRACT = 0x006D,
	XE_VK_KEY_DECIMAL = 0x006E,
	XE_VK_KEY_DIVIDE = 0x006F,
	XE_VK_KEY_OEM_PLUS = 0x00BB,
	XE_VK_KEY_OEM_MINUS = 0x00BD,
	XE_VK_KEY_UNDERSCORE = 0x006C,
	XE_VK_KEY_OEM_COMMA = 0x00BC,
	XE_VK_KEY_OEM_PERIOD = 0x00BE,
	XE_VK_KEY_SPACE = 0x0020,
	XE_VK_KEY_BACKSPACE = 0x0008,
	XE_VK_KEY_F1 = 0x0070,
	XE_VK_KEY_F2 = 0x0071,
	XE_VK_KEY_F3 = 0x0072,
	XE_VK_KEY_F4 = 0x0073,
	XE_VK_KEY_F5 = 0x0074,
	XE_VK_KEY_F6 = 0x0075,
	XE_VK_KEY_F7 = 0x0076,
	XE_VK_KEY_F8 = 0x0077,
	XE_VK_KEY_F9 = 0x0078,
	XE_VK_KEY_F10 = 0x0079,
	XE_VK_KEY_F11 = 0x007A,
	XE_VK_KEY_F12 = 0x007B,
	XE_VK_KEY_F13 = 0x007C,
	XE_VK_KEY_F14 = 0x007D,
	XE_VK_KEY_F15 = 0x007E,
	XE_VK_KEY_F16 = 0x007F,
	XE_VK_KEY_F17 = 0x0080,
	XE_VK_KEY_F18 = 0x0081,
	XE_VK_KEY_F19 = 0x0082,
	XE_VK_KEY_F20 = 0x0083,
	XE_VK_KEY_F21 = 0x0084,
	XE_VK_KEY_F22 = 0x0085,
	XE_VK_KEY_F23 = 0x0086,
	XE_VK_KEY_F24 = 0x0087,
	XE_VK_KEY_TAB = 0x0009,
	XE_VK_KEY_ENTER = 0x000D,			//-->Same as Enter as they only differ with Extended Key in Engine
	XE_VK_KEY_NUMPADENTER = XE_VK_KEY_ENTER,	//--^
	XE_VK_KEY_LSHIFT = 0x0010,			//-->Same as Shift as they only differ with Extended Key in Engine
	XE_VK_KEY_RSHIFT = XE_VK_KEY_LSHIFT,	//--^
	XE_VK_KEY_LCTRL = 0x0011,			//-->Same as CTRL as they only differ with Extended Key in Engine
	XE_VK_KEY_RCTRL = XE_VK_KEY_LCTRL,	//--^
	XE_VK_KEY_LALT = 0x0012,			//-->Same as ALTT as they only differ with Extended Key in Engine
	XE_VK_KEY_RALT = XE_VK_KEY_LALT,	//--^
	XE_VK_KEY_PAUSE = 0x0013,
	XE_VK_KEY_ESCAPE = 0x001B,
	XE_VK_KEY_PAGEUP = 0x0021,
	XE_VK_KEY_PAGEDOWN = 0x0022,
	XE_VK_KEY_END = 0x0023,
	XE_VK_KEY_HOME = 0x0024,
	XE_VK_KEY_LEFT = 0x0025,
	XE_VK_KEY_UP = 0x0026,
	XE_VK_KEY_RIGHT = 0x0027,
	XE_VK_KEY_DOWN = 0x0028,
	XE_VK_KEY_PRINTSCREEN = 0x002C,
	XE_VK_KEY_INSERT = 0x002D,
	XE_VK_KEY_DELETE = 0x002E,
	XE_VK_KEY_CAPSLOCK = 0x0014,
	XE_VK_KEY_NUMLOCK = 0x0090,
	XE_VK_KEY_SCROLLLOCK = 0x0091,
	XE_VK_KEY_VOLUME_MUTE = 0x00AD,
	XE_VK_KEY_VOLUME_DOWN = 0x00AE,
	XE_VK_KEY_VOLUME_UP = 0x00AF,
	XE_VK_KEY_MEDIA_NEXT_TRACK = 0x00B0,
	XE_VK_KEY_MEDIA_PREV_TRACK = 0x00B1,
	XE_VK_KEY_MEDIA_STOP = 0x00B2,
	XE_VK_KEY_MEDIA_PLAY_PAUSE = 0x00B3,
	XE_VK_KEY_OEM_SEMICOLON = 0x00BA,						//-->Same as Semicolon as they only differ with Shift in Engine		
	XE_VK_KEY_OEM_COLON = XE_VK_KEY_OEM_SEMICOLON,		//--^
	XE_VK_KEY_OEM_FWR_SLASH = 0x00BF,						//-->Same as Forward Slash as they only differ with Shift in Engine
	XE_VK_KEY_OEM_QUESTION = XE_VK_KEY_OEM_FWR_SLASH,		//--^
	XE_VK_KEY_OEM_BACK_QUOTE = 0x00C0,						//-->Same as Back Quote as they only differ with Shift in Engine
	XE_VK_KEY_OEM_TILDE = XE_VK_KEY_OEM_BACK_QUOTE,		//--^
	XE_VK_KEY_OEM_OPEN_BRACKET = 0x00DB,						//-->Same as Open Bracket as they only differ with Shift in Engine
	XE_VK_KEY_OEM_OPEN_BRACE = XE_VK_KEY_OEM_OPEN_BRACKET,	//--^
	XE_VK_KEY_OEM_BACK_SLASH = 0x00DC,						//-->Same as Back slash as they only differ with Shift in Engine
	XE_VK_KEY_OEM_PIPE = XE_VK_KEY_OEM_BACK_SLASH,		//--^
	XE_VK_KEY_OEM_CLOSE_BRACKET = 0x00DD,						//-->Same as Close Bracket as they only differ with Shift in Engine
	XE_VK_KEY_OEM_CLOSE_BRACE = XE_VK_KEY_OEM_CLOSE_BRACKET,	//--^
	XE_VK_KEY_OEM_SINGLE_QUOTE = 0x00DE,						//-->Same as Single Quote as they only differ with Shift in Engine
	XE_VK_KEY_OEM_DOUBLE_QUOTE = XE_VK_KEY_OEM_SINGLE_QUOTE,	//--^
	XE_VK_KEY_OEM_1 = 0x0092,
	XE_VK_KEY_OEM_2 = 0x0093,
	XE_VK_KEY_OEM_3 = 0x0094,
	XE_VK_KEY_OEM_4 = 0x0095,
	XE_VK_KEY_OEM_5 = 0x0096,
	XE_VK_KEY_OEM_6 = 0x00DF,
	XE_VK_KEY_OEM_7 = 0x00E1,
	XE_VK_KEY_OEM_8 = 0x00E3,
	XE_VK_KEY_OEM_9 = 0x00E4,
	XE_VK_KEY_OEM_10 = 0x00E6,
	XE_VK_KEY_OEM_11 = 0x00E9,
	XE_VK_KEY_OEM_12 = 0x00EA,
	XE_VK_KEY_OEM_13 = 0x00EB,
	XE_VK_KEY_OEM_14 = 0x00EC,
	XE_VK_KEY_OEM_15 = 0x00ED,
	XE_VK_KEY_OEM_16 = 0x00EE,
	XE_VK_KEY_OEM_17 = 0x00EF,
	XE_VK_KEY_OEM_18 = 0x00F0,
	XE_VK_KEY_OEM_19 = 0x00F1,
	XE_VK_KEY_OEM_20 = 0x00F2,
	XE_VK_KEY_OEM_21 = 0x00F3,
	XE_VK_KEY_OEM_22 = 0x00F4,
	XE_VK_KEY_OEM_23 = 0x00F5,
};

enum class XEXBoxGamepadButton : uint32_t
{
	Y = 0,
	X,
	A,
	B,
	RightTop,
	RightBottom,
	LeftTop,
	LeftBottom,
	DPadUp,
	DPadDown,
	DPadLeft,
	DPadRight,
	Back,
	Start,
	RightThumb,
	LeftThumb,

	//////////////////
	__MaxButtons
};

enum class XEXBoxGamepadMovements : uint32_t
{
	LeftThumbY = 0,
	LeftThumbX,
	RightThumbY,
	RightThumbX,
	LeftTrigger,
	RightTrigger,

	///////////////

	__MaxMovementButtons
};

enum class XEKeys : uint32_t
{
	//Numbers
	N0 = 0,
	N1,
	N2,
	N3,
	N4,
	N5,
	N6,
	N7,
	N8,
	N9,

	//Normal Letters
	A,
	B,
	C,
	D,
	E,
	F,
	G,
	H,
	I,
	J,
	K,
	L,
	M,
	N,
	O,
	P,
	Q,
	R,
	S,
	T,
	U,
	V,
	W,
	X,
	Y,
	Z,

	//Numpad
	NUMPAD0,
	NUMPAD1,
	NUMPAD2,
	NUMPAD3,
	NUMPAD4,
	NUMPAD5,
	NUMPAD6,
	NUMPAD7,
	NUMPAD8,
	NUMPAD9,
	NUMPADENTER,

	//Math
	MULTIPLY,
	ADD,
	SUBTRACT,
	DECIMAL,
	DIVIDE,
	OEM_PLUS,
	OEM_MINUS,
	
	//Grammar
	UNDERSCORE,
	OEM_COMMA,
	OEM_PERIOD,
	SPACE,
	BACKSPACE,

	//Functions Keys
	F1,
	F2,
	F3,
	F4,
	F5,
	F6,
	F7,
	F8,
	F9,
	F10,
	F11,
	F12,
	F13,
	F14,
	F15,
	F16,
	F17,
	F18,
	F19,
	F20,
	F21,
	F22,
	F23,
	F24,

	//Special Keys
	TAB,
	ENTER,
	LSHIFT,
	RSHIFT,
	LCTRL,
	RCTRL,
	LALT,
	RALT,
	PAUSE,
	ESCAPE,
	PAGEUP,
	PAGEDOWN,
	END,
	HOME,
	LEFT,
	UP,
	RIGHT,
	DOWN,
	PRINTSCREEN,
	INSERT,
	DEL,

	//Lock Keys
	CAPSLOCK,
	NUMLOCK,
	SCROLLLOCK,

	//Music Keys
	VOLUME_MUTE,
	VOLUME_DOWN,
	VOLUME_UP,
	MEDIA_NEXT_TRACK,
	MEDIA_PREV_TRACK,
	MEDIA_STOP,
	MEDIA_PLAY_PAUSE,

	//OEM Keys
	OEM_SEMICOLON,
	OEM_COLON,
	OEM_FWR_SLASH,
	OEM_QUESTION,
	OEM_BACK_QUOTE,
	OEM_TILDE,
	OEM_OPEN_BRACKET,
	OEM_OPEN_BRACE,
	OEM_BACK_SLASH,
	OEM_PIPE,
	OEM_CLOSE_BRACKET,
	OEM_CLOSE_BRACE,
	OEM_SINGLE_QUOTE,
	OEM_DOUBLE_QUOTE,
	OEM_1,
	OEM_2,
	OEM_3,
	OEM_4,
	OEM_5,
	OEM_6,
	OEM_7,
	OEM_8,
	OEM_9,
	OEM_10,
	OEM_11,
	OEM_12,
	OEM_13,
	OEM_14,
	OEM_15,
	OEM_16,
	OEM_17,
	OEM_18,
	OEM_19,
	OEM_20,
	OEM_21,
	OEM_22,
	OEM_23,

	//--------------//
	__MaxKeys
};

/****************
*   Defines 2   *
*****************/

/// <summary>
/// Define for Max Number of Keyboard Keys in XEKeys enum
/// </summary>
#define XE_MAX_KEYS					(uint32_t)XEKeys::__MaxKeys

/// <summary>
/// Define for Max Number of Buttons in XEXBoxGamepadButton enum
/// </summary>
#define XE_MAX_XBOX_GP_BUTTONS		(uint32_t)XEXBoxGamepadButton::__MaxButtons

/// <summary>
/// Define for Max Number of Movement Buttons in XEXBoxGamepadMovements enum
/// </summary>
#define XE_MAX_XBOX_GP_MOV_BUTTONS	(uint32_t)XEXBoxGamepadMovements::__MaxMovementButtons

/*******************
*   Structs Decl   *
********************/

/// <summary>	
/// This structure is use to save the variables needed to initialize
/// the input devices.
/// </summary>
struct GameConfigInput sealed : public XEObject
{
	/// <summary>
	/// Defines if Keyboard will be available and should be initialized
	/// </summary>
	bool m_KeyboardEnabled = false;

	/// <summary>
	/// Defines if XBox Gamepad will be available and should be initialized
	/// </summary>
	bool m_XBoxGamepadEnabled = false;

	/// <summary>	
	/// Default GameConfigInput Constructor.
	/// </summary>
	GameConfigInput()
	{
	}
};

#endif