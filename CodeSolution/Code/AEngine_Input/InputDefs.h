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
#include "Base\AEObject.h"

/**************
*   Defines   *
***************/

/// <summary>
/// Define bit for when a key/button has been pressed
/// </summary>
#define AE_INPUT_PRESS                  0x01

/// <summary>
/// Define bit for when a developer sends a command to un-press a key/button.
/// </summary>
#define AE_DEV_INPUT_UNPRESS            0xFE

/// <summary>
/// Default Time to wait between XBox Gamepad initialize retries.
/// </summary>
#define AE_XBOX_GAMEPAD_INIT_WAIT_TIME  5.0f

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
enum class AEVKWindows : uint16_t
{
    AE_VK_KEY_0                         = 0x0030,
    AE_VK_KEY_1                         = 0x0031,
    AE_VK_KEY_2                         = 0x0032,
    AE_VK_KEY_3                         = 0x0033,
    AE_VK_KEY_4                         = 0x0034,
    AE_VK_KEY_5                         = 0x0035,
    AE_VK_KEY_6                         = 0x0036,
    AE_VK_KEY_7                         = 0x0037,
    AE_VK_KEY_8                         = 0x0038,
    AE_VK_KEY_9                         = 0x0039,
    AE_VK_KEY_A                         = 0x0041,
    AE_VK_KEY_B                         = 0x0042,
    AE_VK_KEY_C                         = 0x0043,
    AE_VK_KEY_D                         = 0x0044,
    AE_VK_KEY_E                         = 0x0045,
    AE_VK_KEY_F                         = 0x0046,
    AE_VK_KEY_G                         = 0x0047,
    AE_VK_KEY_H                         = 0x0048,
    AE_VK_KEY_I                         = 0x0049,
    AE_VK_KEY_J                         = 0x004A,
    AE_VK_KEY_K                         = 0x004B,
    AE_VK_KEY_L                         = 0x004C,
    AE_VK_KEY_M                         = 0x004D,
    AE_VK_KEY_N                         = 0x004E,
    AE_VK_KEY_O                         = 0x004F,
    AE_VK_KEY_P                         = 0x0050,
    AE_VK_KEY_Q                         = 0x0051,
    AE_VK_KEY_R                         = 0x0052,
    AE_VK_KEY_S                         = 0x0053,
    AE_VK_KEY_T                         = 0x0054,
    AE_VK_KEY_U                         = 0x0055,
    AE_VK_KEY_V                         = 0x0056,
    AE_VK_KEY_W                         = 0x0057,
    AE_VK_KEY_X                         = 0x0058,
    AE_VK_KEY_Y                         = 0x0059,
    AE_VK_KEY_Z                         = 0x005A,
    AE_VK_KEY_NUMPAD0                   = 0x0060,
    AE_VK_KEY_NUMPAD1                   = 0x0061,
    AE_VK_KEY_NUMPAD2                   = 0x0062,
    AE_VK_KEY_NUMPAD3                   = 0x0063,
    AE_VK_KEY_NUMPAD4                   = 0x0064,
    AE_VK_KEY_NUMPAD5                   = 0x0065,
    AE_VK_KEY_NUMPAD6                   = 0x0066,
    AE_VK_KEY_NUMPAD7                   = 0x0067,
    AE_VK_KEY_NUMPAD8                   = 0x0068,
    AE_VK_KEY_NUMPAD9                   = 0x0069,
    AE_VK_KEY_MULTIPLY                  = 0x006A,
    AE_VK_KEY_ADD                       = 0x006B,
    AE_VK_KEY_SUBTRACT                  = 0x006D,
    AE_VK_KEY_DECIMAL                   = 0x006E,
    AE_VK_KEY_DIVIDE                    = 0x006F,
    AE_VK_KEY_OEM_PLUS                  = 0x00BB,
    AE_VK_KEY_OEM_MINUS                 = 0x00BD,
    AE_VK_KEY_UNDERSCORE                = 0x006C,
    AE_VK_KEY_OEM_COMMA                 = 0x00BC,
    AE_VK_KEY_OEM_PERIOD                = 0x00BE,
    AE_VK_KEY_SPACE                     = 0x0020,
    AE_VK_KEY_BACKSPACE                 = 0x0008,
    AE_VK_KEY_F1                        = 0x0070,
    AE_VK_KEY_F2                        = 0x0071,
    AE_VK_KEY_F3                        = 0x0072,
    AE_VK_KEY_F4                        = 0x0073,
    AE_VK_KEY_F5                        = 0x0074,
    AE_VK_KEY_F6                        = 0x0075,
    AE_VK_KEY_F7                        = 0x0076,
    AE_VK_KEY_F8                        = 0x0077,
    AE_VK_KEY_F9                        = 0x0078,
    AE_VK_KEY_F10                       = 0x0079,
    AE_VK_KEY_F11                       = 0x007A,
    AE_VK_KEY_F12                       = 0x007B,
    AE_VK_KEY_F13                       = 0x007C,
    AE_VK_KEY_F14                       = 0x007D,
    AE_VK_KEY_F15                       = 0x007E,
    AE_VK_KEY_F16                       = 0x007F,
    AE_VK_KEY_F17                       = 0x0080,
    AE_VK_KEY_F18                       = 0x0081,
    AE_VK_KEY_F19                       = 0x0082,
    AE_VK_KEY_F20                       = 0x0083,
    AE_VK_KEY_F21                       = 0x0084,
    AE_VK_KEY_F22                       = 0x0085,
    AE_VK_KEY_F23                       = 0x0086,
    AE_VK_KEY_F24                       = 0x0087,
    AE_VK_KEY_TAB                       = 0x0009,
    AE_VK_KEY_ENTER                     = 0x000D,               //-->Same as Enter as they only differ with Extended Key in Engine
    AE_VK_KEY_NUMPADENTER               = AE_VK_KEY_ENTER,      //--^
    AE_VK_KEY_LSHIFT                    = 0x0010,               //-->Same as Shift as they only differ with Extended Key in Engine
    AE_VK_KEY_RSHIFT                    = AE_VK_KEY_LSHIFT,     //--^
    AE_VK_KEY_LCTRL                     = 0x0011,               //-->Same as CTRL as they only differ with Extended Key in Engine
    AE_VK_KEY_RCTRL                     = AE_VK_KEY_LCTRL,      //--^
    AE_VK_KEY_LALT                      = 0x0012,               //-->Same as ALTT as they only differ with Extended Key in Engine
    AE_VK_KEY_RALT                      = AE_VK_KEY_LALT,       //--^
    AE_VK_KEY_PAUSE                     = 0x0013,
    AE_VK_KEY_ESCAPE                    = 0x001B,
    AE_VK_KEY_PAGEUP                    = 0x0021,
    AE_VK_KEY_PAGEDOWN                  = 0x0022,
    AE_VK_KEY_END                       = 0x0023,
    AE_VK_KEY_HOME                      = 0x0024,
    AE_VK_KEY_LEFT                      = 0x0025,
    AE_VK_KEY_UP                        = 0x0026,
    AE_VK_KEY_RIGHT                     = 0x0027,
    AE_VK_KEY_DOWN                      = 0x0028,
    AE_VK_KEY_PRINTSCREEN               = 0x002C,
    AE_VK_KEY_INSERT                    = 0x002D,
    AE_VK_KEY_DELETE                    = 0x002E,
    AE_VK_KEY_CAPSLOCK                  = 0x0014,
    AE_VK_KEY_NUMLOCK                   = 0x0090,
    AE_VK_KEY_SCROLLLOCK                = 0x0091,
    AE_VK_KEY_VOLUME_MUTE               = 0x00AD,
    AE_VK_KEY_VOLUME_DOWN               = 0x00AE,
    AE_VK_KEY_VOLUME_UP                 = 0x00AF,
    AE_VK_KEY_MEDIA_NEXT_TRACK          = 0x00B0,
    AE_VK_KEY_MEDIA_PREV_TRACK          = 0x00B1,
    AE_VK_KEY_MEDIA_STOP                = 0x00B2,
    AE_VK_KEY_MEDIA_PLAY_PAUSE          = 0x00B3,
    AE_VK_KEY_OEM_SEMICOLON             = 0x00BA,                        //-->Same as Semicolon as they only differ with Shift in Engine        
    AE_VK_KEY_OEM_COLON                 = AE_VK_KEY_OEM_SEMICOLON,       //--^
    AE_VK_KEY_OEM_FWR_SLASH             = 0x00BF,                        //-->Same as Forward Slash as they only differ with Shift in Engine
    AE_VK_KEY_OEM_QUESTION              = AE_VK_KEY_OEM_FWR_SLASH,       //--^
    AE_VK_KEY_OEM_BACK_QUOTE            = 0x00C0,                        //-->Same as Back Quote as they only differ with Shift in Engine
    AE_VK_KEY_OEM_TILDE                 = AE_VK_KEY_OEM_BACK_QUOTE,      //--^
    AE_VK_KEY_OEM_OPEN_BRACKET          = 0x00DB,                        //-->Same as Open Bracket as they only differ with Shift in Engine
    AE_VK_KEY_OEM_OPEN_BRACE            = AE_VK_KEY_OEM_OPEN_BRACKET,    //--^
    AE_VK_KEY_OEM_BACK_SLASH            = 0x00DC,                        //-->Same as Back slash as they only differ with Shift in Engine
    AE_VK_KEY_OEM_PIPE                  = AE_VK_KEY_OEM_BACK_SLASH,      //--^
    AE_VK_KEY_OEM_CLOSE_BRACKET         = 0x00DD,                        //-->Same as Close Bracket as they only differ with Shift in Engine
    AE_VK_KEY_OEM_CLOSE_BRACE           = AE_VK_KEY_OEM_CLOSE_BRACKET,   //--^
    AE_VK_KEY_OEM_SINGLE_QUOTE          = 0x00DE,                        //-->Same as Single Quote as they only differ with Shift in Engine
    AE_VK_KEY_OEM_DOUBLE_QUOTE          = AE_VK_KEY_OEM_SINGLE_QUOTE,    //--^
    AE_VK_KEY_OEM_1                     = 0x0092,
    AE_VK_KEY_OEM_2                     = 0x0093,
    AE_VK_KEY_OEM_3                     = 0x0094,
    AE_VK_KEY_OEM_4                     = 0x0095,
    AE_VK_KEY_OEM_5                     = 0x0096,
    AE_VK_KEY_OEM_6                     = 0x00DF,
    AE_VK_KEY_OEM_7                     = 0x00E1,
    AE_VK_KEY_OEM_8                     = 0x00E3,
    AE_VK_KEY_OEM_9                     = 0x00E4,
    AE_VK_KEY_OEM_10                    = 0x00E6,
    AE_VK_KEY_OEM_11                    = 0x00E9,
    AE_VK_KEY_OEM_12                    = 0x00EA,
    AE_VK_KEY_OEM_13                    = 0x00EB,
    AE_VK_KEY_OEM_14                    = 0x00EC,
    AE_VK_KEY_OEM_15                    = 0x00ED,
    AE_VK_KEY_OEM_16                    = 0x00EE,
    AE_VK_KEY_OEM_17                    = 0x00EF,
    AE_VK_KEY_OEM_18                    = 0x00F0,
    AE_VK_KEY_OEM_19                    = 0x00F1,
    AE_VK_KEY_OEM_20                    = 0x00F2,
    AE_VK_KEY_OEM_21                    = 0x00F3,
    AE_VK_KEY_OEM_22                    = 0x00F4,
    AE_VK_KEY_OEM_23                    = 0x00F5,
};

enum class AEXBoxGamepadButton : uint32_t
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

enum class AEXBoxGamepadMovements : uint32_t
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

enum class AEKeys : uint32_t
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
/// Define for Max Number of Keyboard Keys in AEKeys enum
/// </summary>
#define AE_MAX_KEYS                    (uint32_t)AEKeys::__MaxKeys

/// <summary>
/// Define for Max Number of Buttons in AEXBoxGamepadButton enum
/// </summary>
#define AE_MAX_XBOX_GP_BUTTONS        (uint32_t)AEXBoxGamepadButton::__MaxButtons

/// <summary>
/// Define for Max Number of Movement Buttons in AEXBoxGamepadMovements enum
/// </summary>
#define AE_MAX_XBOX_GP_MOV_BUTTONS    (uint32_t)AEXBoxGamepadMovements::__MaxMovementButtons

/*******************
*   Structs Decl   *
********************/

/// <summary>    
/// This structure is use to save the variables needed to initialize
/// the input devices.
/// </summary>
struct GameConfigInput sealed : public AEObject
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
