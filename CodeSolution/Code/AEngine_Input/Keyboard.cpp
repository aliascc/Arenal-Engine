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
    memset(m_InMemBuffer, 0, sizeof(uint8_t) * AE_MAX_KEYS);
    memset(m_PreviousKeyboardState, 0, sizeof(uint8_t) *  AE_MAX_KEYS);
    memset(m_CurrentKeyboardState, 0, sizeof(uint8_t) *  AE_MAX_KEYS);
}

void Keyboard::InitKeyVKMap()
{
    m_KeyMapVK[(uint32_t)AEKeys::N0]                = AEVKWindows::AE_VK_KEY_0;
    m_KeyMapVK[(uint32_t)AEKeys::N1]                = AEVKWindows::AE_VK_KEY_1;
    m_KeyMapVK[(uint32_t)AEKeys::N2]                = AEVKWindows::AE_VK_KEY_2;
    m_KeyMapVK[(uint32_t)AEKeys::N3]                = AEVKWindows::AE_VK_KEY_3;
    m_KeyMapVK[(uint32_t)AEKeys::N4]                = AEVKWindows::AE_VK_KEY_4;
    m_KeyMapVK[(uint32_t)AEKeys::N5]                = AEVKWindows::AE_VK_KEY_5;
    m_KeyMapVK[(uint32_t)AEKeys::N6]                = AEVKWindows::AE_VK_KEY_6;
    m_KeyMapVK[(uint32_t)AEKeys::N7]                = AEVKWindows::AE_VK_KEY_7;
    m_KeyMapVK[(uint32_t)AEKeys::N8]                = AEVKWindows::AE_VK_KEY_8;
    m_KeyMapVK[(uint32_t)AEKeys::N9]                = AEVKWindows::AE_VK_KEY_9;
    m_KeyMapVK[(uint32_t)AEKeys::A]                 = AEVKWindows::AE_VK_KEY_A;
    m_KeyMapVK[(uint32_t)AEKeys::B]                 = AEVKWindows::AE_VK_KEY_B;
    m_KeyMapVK[(uint32_t)AEKeys::C]                 = AEVKWindows::AE_VK_KEY_C;
    m_KeyMapVK[(uint32_t)AEKeys::D]                 = AEVKWindows::AE_VK_KEY_D;
    m_KeyMapVK[(uint32_t)AEKeys::E]                 = AEVKWindows::AE_VK_KEY_E;
    m_KeyMapVK[(uint32_t)AEKeys::F]                 = AEVKWindows::AE_VK_KEY_F;
    m_KeyMapVK[(uint32_t)AEKeys::G]                 = AEVKWindows::AE_VK_KEY_G;
    m_KeyMapVK[(uint32_t)AEKeys::H]                 = AEVKWindows::AE_VK_KEY_H;
    m_KeyMapVK[(uint32_t)AEKeys::I]                 = AEVKWindows::AE_VK_KEY_I;
    m_KeyMapVK[(uint32_t)AEKeys::J]                 = AEVKWindows::AE_VK_KEY_J;
    m_KeyMapVK[(uint32_t)AEKeys::K]                 = AEVKWindows::AE_VK_KEY_K;
    m_KeyMapVK[(uint32_t)AEKeys::L]                 = AEVKWindows::AE_VK_KEY_L;
    m_KeyMapVK[(uint32_t)AEKeys::M]                 = AEVKWindows::AE_VK_KEY_M;
    m_KeyMapVK[(uint32_t)AEKeys::N]                 = AEVKWindows::AE_VK_KEY_N;
    m_KeyMapVK[(uint32_t)AEKeys::O]                 = AEVKWindows::AE_VK_KEY_O;
    m_KeyMapVK[(uint32_t)AEKeys::P]                 = AEVKWindows::AE_VK_KEY_P;
    m_KeyMapVK[(uint32_t)AEKeys::Q]                 = AEVKWindows::AE_VK_KEY_Q;
    m_KeyMapVK[(uint32_t)AEKeys::R]                 = AEVKWindows::AE_VK_KEY_R;
    m_KeyMapVK[(uint32_t)AEKeys::S]                 = AEVKWindows::AE_VK_KEY_S;
    m_KeyMapVK[(uint32_t)AEKeys::T]                 = AEVKWindows::AE_VK_KEY_T;
    m_KeyMapVK[(uint32_t)AEKeys::U]                 = AEVKWindows::AE_VK_KEY_U;
    m_KeyMapVK[(uint32_t)AEKeys::V]                 = AEVKWindows::AE_VK_KEY_V;
    m_KeyMapVK[(uint32_t)AEKeys::W]                 = AEVKWindows::AE_VK_KEY_W;
    m_KeyMapVK[(uint32_t)AEKeys::X]                 = AEVKWindows::AE_VK_KEY_X;
    m_KeyMapVK[(uint32_t)AEKeys::Y]                 = AEVKWindows::AE_VK_KEY_Y;
    m_KeyMapVK[(uint32_t)AEKeys::Z]                 = AEVKWindows::AE_VK_KEY_Z;
    m_KeyMapVK[(uint32_t)AEKeys::NUMPAD0]           = AEVKWindows::AE_VK_KEY_NUMPAD0;
    m_KeyMapVK[(uint32_t)AEKeys::NUMPAD1]           = AEVKWindows::AE_VK_KEY_NUMPAD1;
    m_KeyMapVK[(uint32_t)AEKeys::NUMPAD2]           = AEVKWindows::AE_VK_KEY_NUMPAD2;
    m_KeyMapVK[(uint32_t)AEKeys::NUMPAD3]           = AEVKWindows::AE_VK_KEY_NUMPAD3;
    m_KeyMapVK[(uint32_t)AEKeys::NUMPAD4]           = AEVKWindows::AE_VK_KEY_NUMPAD4;
    m_KeyMapVK[(uint32_t)AEKeys::NUMPAD5]           = AEVKWindows::AE_VK_KEY_NUMPAD5;
    m_KeyMapVK[(uint32_t)AEKeys::NUMPAD6]           = AEVKWindows::AE_VK_KEY_NUMPAD6;
    m_KeyMapVK[(uint32_t)AEKeys::NUMPAD7]           = AEVKWindows::AE_VK_KEY_NUMPAD7;
    m_KeyMapVK[(uint32_t)AEKeys::NUMPAD8]           = AEVKWindows::AE_VK_KEY_NUMPAD8;
    m_KeyMapVK[(uint32_t)AEKeys::NUMPAD9]           = AEVKWindows::AE_VK_KEY_NUMPAD9;
    m_KeyMapVK[(uint32_t)AEKeys::MULTIPLY]          = AEVKWindows::AE_VK_KEY_MULTIPLY;
    m_KeyMapVK[(uint32_t)AEKeys::ADD]               = AEVKWindows::AE_VK_KEY_ADD;
    m_KeyMapVK[(uint32_t)AEKeys::SUBTRACT]          = AEVKWindows::AE_VK_KEY_SUBTRACT;
    m_KeyMapVK[(uint32_t)AEKeys::DECIMAL]           = AEVKWindows::AE_VK_KEY_DECIMAL;
    m_KeyMapVK[(uint32_t)AEKeys::DIVIDE]            = AEVKWindows::AE_VK_KEY_DIVIDE;
    m_KeyMapVK[(uint32_t)AEKeys::OEM_PLUS]          = AEVKWindows::AE_VK_KEY_OEM_PLUS;
    m_KeyMapVK[(uint32_t)AEKeys::OEM_MINUS]         = AEVKWindows::AE_VK_KEY_OEM_MINUS;
    m_KeyMapVK[(uint32_t)AEKeys::UNDERSCORE]        = AEVKWindows::AE_VK_KEY_UNDERSCORE;
    m_KeyMapVK[(uint32_t)AEKeys::OEM_COMMA]         = AEVKWindows::AE_VK_KEY_OEM_COMMA;
    m_KeyMapVK[(uint32_t)AEKeys::OEM_PERIOD]        = AEVKWindows::AE_VK_KEY_OEM_PERIOD;
    m_KeyMapVK[(uint32_t)AEKeys::SPACE]             = AEVKWindows::AE_VK_KEY_SPACE;
    m_KeyMapVK[(uint32_t)AEKeys::BACKSPACE]         = AEVKWindows::AE_VK_KEY_BACKSPACE;
    m_KeyMapVK[(uint32_t)AEKeys::F1]                = AEVKWindows::AE_VK_KEY_F1;
    m_KeyMapVK[(uint32_t)AEKeys::F2]                = AEVKWindows::AE_VK_KEY_F2;
    m_KeyMapVK[(uint32_t)AEKeys::F3]                = AEVKWindows::AE_VK_KEY_F3;
    m_KeyMapVK[(uint32_t)AEKeys::F4]                = AEVKWindows::AE_VK_KEY_F4;
    m_KeyMapVK[(uint32_t)AEKeys::F5]                = AEVKWindows::AE_VK_KEY_F5;
    m_KeyMapVK[(uint32_t)AEKeys::F6]                = AEVKWindows::AE_VK_KEY_F6;
    m_KeyMapVK[(uint32_t)AEKeys::F7]                = AEVKWindows::AE_VK_KEY_F7;
    m_KeyMapVK[(uint32_t)AEKeys::F8]                = AEVKWindows::AE_VK_KEY_F8;
    m_KeyMapVK[(uint32_t)AEKeys::F9]                = AEVKWindows::AE_VK_KEY_F9;
    m_KeyMapVK[(uint32_t)AEKeys::F10]               = AEVKWindows::AE_VK_KEY_F10;
    m_KeyMapVK[(uint32_t)AEKeys::F11]               = AEVKWindows::AE_VK_KEY_F11;
    m_KeyMapVK[(uint32_t)AEKeys::F12]               = AEVKWindows::AE_VK_KEY_F12;
    m_KeyMapVK[(uint32_t)AEKeys::F13]               = AEVKWindows::AE_VK_KEY_F13;
    m_KeyMapVK[(uint32_t)AEKeys::F14]               = AEVKWindows::AE_VK_KEY_F14;
    m_KeyMapVK[(uint32_t)AEKeys::F15]               = AEVKWindows::AE_VK_KEY_F15;
    m_KeyMapVK[(uint32_t)AEKeys::F16]               = AEVKWindows::AE_VK_KEY_F16;
    m_KeyMapVK[(uint32_t)AEKeys::F17]               = AEVKWindows::AE_VK_KEY_F17;
    m_KeyMapVK[(uint32_t)AEKeys::F18]               = AEVKWindows::AE_VK_KEY_F18;
    m_KeyMapVK[(uint32_t)AEKeys::F19]               = AEVKWindows::AE_VK_KEY_F19;
    m_KeyMapVK[(uint32_t)AEKeys::F20]               = AEVKWindows::AE_VK_KEY_F20;
    m_KeyMapVK[(uint32_t)AEKeys::F21]               = AEVKWindows::AE_VK_KEY_F21;
    m_KeyMapVK[(uint32_t)AEKeys::F22]               = AEVKWindows::AE_VK_KEY_F22;
    m_KeyMapVK[(uint32_t)AEKeys::F23]               = AEVKWindows::AE_VK_KEY_F23;
    m_KeyMapVK[(uint32_t)AEKeys::F24]               = AEVKWindows::AE_VK_KEY_F24;
    m_KeyMapVK[(uint32_t)AEKeys::TAB]               = AEVKWindows::AE_VK_KEY_TAB;
    m_KeyMapVK[(uint32_t)AEKeys::ENTER]             = AEVKWindows::AE_VK_KEY_ENTER;
    m_KeyMapVK[(uint32_t)AEKeys::NUMPADENTER]       = AEVKWindows::AE_VK_KEY_NUMPADENTER;
    m_KeyMapVK[(uint32_t)AEKeys::LSHIFT]            = AEVKWindows::AE_VK_KEY_LSHIFT;
    m_KeyMapVK[(uint32_t)AEKeys::RSHIFT]            = AEVKWindows::AE_VK_KEY_RSHIFT;
    m_KeyMapVK[(uint32_t)AEKeys::LCTRL]             = AEVKWindows::AE_VK_KEY_LCTRL;
    m_KeyMapVK[(uint32_t)AEKeys::RCTRL]             = AEVKWindows::AE_VK_KEY_RCTRL;
    m_KeyMapVK[(uint32_t)AEKeys::LALT]              = AEVKWindows::AE_VK_KEY_LALT;
    m_KeyMapVK[(uint32_t)AEKeys::RALT]              = AEVKWindows::AE_VK_KEY_RALT;
    m_KeyMapVK[(uint32_t)AEKeys::PAUSE]             = AEVKWindows::AE_VK_KEY_PAUSE;
    m_KeyMapVK[(uint32_t)AEKeys::ESCAPE]            = AEVKWindows::AE_VK_KEY_ESCAPE;
    m_KeyMapVK[(uint32_t)AEKeys::PAGEUP]            = AEVKWindows::AE_VK_KEY_PAGEUP;
    m_KeyMapVK[(uint32_t)AEKeys::PAGEDOWN]          = AEVKWindows::AE_VK_KEY_PAGEDOWN;
    m_KeyMapVK[(uint32_t)AEKeys::END]               = AEVKWindows::AE_VK_KEY_END;
    m_KeyMapVK[(uint32_t)AEKeys::HOME]              = AEVKWindows::AE_VK_KEY_HOME;
    m_KeyMapVK[(uint32_t)AEKeys::LEFT]              = AEVKWindows::AE_VK_KEY_LEFT;
    m_KeyMapVK[(uint32_t)AEKeys::UP]                = AEVKWindows::AE_VK_KEY_UP;
    m_KeyMapVK[(uint32_t)AEKeys::RIGHT]             = AEVKWindows::AE_VK_KEY_RIGHT;
    m_KeyMapVK[(uint32_t)AEKeys::DOWN]              = AEVKWindows::AE_VK_KEY_DOWN;
    m_KeyMapVK[(uint32_t)AEKeys::PRINTSCREEN]       = AEVKWindows::AE_VK_KEY_PRINTSCREEN;
    m_KeyMapVK[(uint32_t)AEKeys::INSERT]            = AEVKWindows::AE_VK_KEY_INSERT;
    m_KeyMapVK[(uint32_t)AEKeys::DEL]               = AEVKWindows::AE_VK_KEY_DELETE;
    m_KeyMapVK[(uint32_t)AEKeys::CAPSLOCK]          = AEVKWindows::AE_VK_KEY_CAPSLOCK;
    m_KeyMapVK[(uint32_t)AEKeys::NUMLOCK]           = AEVKWindows::AE_VK_KEY_NUMLOCK;
    m_KeyMapVK[(uint32_t)AEKeys::SCROLLLOCK]        = AEVKWindows::AE_VK_KEY_SCROLLLOCK;
    m_KeyMapVK[(uint32_t)AEKeys::VOLUME_MUTE]       = AEVKWindows::AE_VK_KEY_VOLUME_MUTE;
    m_KeyMapVK[(uint32_t)AEKeys::VOLUME_DOWN]       = AEVKWindows::AE_VK_KEY_VOLUME_DOWN;
    m_KeyMapVK[(uint32_t)AEKeys::VOLUME_UP]         = AEVKWindows::AE_VK_KEY_VOLUME_UP;
    m_KeyMapVK[(uint32_t)AEKeys::MEDIA_NEXT_TRACK]  = AEVKWindows::AE_VK_KEY_MEDIA_NEXT_TRACK;
    m_KeyMapVK[(uint32_t)AEKeys::MEDIA_PREV_TRACK]  = AEVKWindows::AE_VK_KEY_MEDIA_PREV_TRACK;
    m_KeyMapVK[(uint32_t)AEKeys::MEDIA_STOP]        = AEVKWindows::AE_VK_KEY_MEDIA_STOP;
    m_KeyMapVK[(uint32_t)AEKeys::MEDIA_PLAY_PAUSE]  = AEVKWindows::AE_VK_KEY_MEDIA_PLAY_PAUSE;
    m_KeyMapVK[(uint32_t)AEKeys::OEM_SEMICOLON]     = AEVKWindows::AE_VK_KEY_OEM_SEMICOLON;
    m_KeyMapVK[(uint32_t)AEKeys::OEM_COLON]         = AEVKWindows::AE_VK_KEY_OEM_COLON;
    m_KeyMapVK[(uint32_t)AEKeys::OEM_FWR_SLASH]     = AEVKWindows::AE_VK_KEY_OEM_FWR_SLASH;
    m_KeyMapVK[(uint32_t)AEKeys::OEM_QUESTION]      = AEVKWindows::AE_VK_KEY_OEM_QUESTION;
    m_KeyMapVK[(uint32_t)AEKeys::OEM_BACK_QUOTE]    = AEVKWindows::AE_VK_KEY_OEM_BACK_QUOTE;
    m_KeyMapVK[(uint32_t)AEKeys::OEM_TILDE]         = AEVKWindows::AE_VK_KEY_OEM_TILDE;
    m_KeyMapVK[(uint32_t)AEKeys::OEM_OPEN_BRACKET]  = AEVKWindows::AE_VK_KEY_OEM_OPEN_BRACKET;
    m_KeyMapVK[(uint32_t)AEKeys::OEM_OPEN_BRACE]    = AEVKWindows::AE_VK_KEY_OEM_OPEN_BRACE;
    m_KeyMapVK[(uint32_t)AEKeys::OEM_BACK_SLASH]    = AEVKWindows::AE_VK_KEY_OEM_BACK_SLASH;
    m_KeyMapVK[(uint32_t)AEKeys::OEM_PIPE]          = AEVKWindows::AE_VK_KEY_OEM_PIPE;
    m_KeyMapVK[(uint32_t)AEKeys::OEM_CLOSE_BRACKET] = AEVKWindows::AE_VK_KEY_OEM_CLOSE_BRACKET;
    m_KeyMapVK[(uint32_t)AEKeys::OEM_CLOSE_BRACE]   = AEVKWindows::AE_VK_KEY_OEM_CLOSE_BRACE;
    m_KeyMapVK[(uint32_t)AEKeys::OEM_SINGLE_QUOTE]  = AEVKWindows::AE_VK_KEY_OEM_SINGLE_QUOTE;
    m_KeyMapVK[(uint32_t)AEKeys::OEM_DOUBLE_QUOTE]  = AEVKWindows::AE_VK_KEY_OEM_DOUBLE_QUOTE;
    m_KeyMapVK[(uint32_t)AEKeys::OEM_1]             = AEVKWindows::AE_VK_KEY_OEM_1;
    m_KeyMapVK[(uint32_t)AEKeys::OEM_2]             = AEVKWindows::AE_VK_KEY_OEM_2;
    m_KeyMapVK[(uint32_t)AEKeys::OEM_3]             = AEVKWindows::AE_VK_KEY_OEM_3;
    m_KeyMapVK[(uint32_t)AEKeys::OEM_4]             = AEVKWindows::AE_VK_KEY_OEM_4;
    m_KeyMapVK[(uint32_t)AEKeys::OEM_5]             = AEVKWindows::AE_VK_KEY_OEM_5;
    m_KeyMapVK[(uint32_t)AEKeys::OEM_6]             = AEVKWindows::AE_VK_KEY_OEM_6;
    m_KeyMapVK[(uint32_t)AEKeys::OEM_7]             = AEVKWindows::AE_VK_KEY_OEM_7;
    m_KeyMapVK[(uint32_t)AEKeys::OEM_8]             = AEVKWindows::AE_VK_KEY_OEM_8;
    m_KeyMapVK[(uint32_t)AEKeys::OEM_9]             = AEVKWindows::AE_VK_KEY_OEM_9;
    m_KeyMapVK[(uint32_t)AEKeys::OEM_10]            = AEVKWindows::AE_VK_KEY_OEM_10;
    m_KeyMapVK[(uint32_t)AEKeys::OEM_11]            = AEVKWindows::AE_VK_KEY_OEM_11;
    m_KeyMapVK[(uint32_t)AEKeys::OEM_12]            = AEVKWindows::AE_VK_KEY_OEM_12;
    m_KeyMapVK[(uint32_t)AEKeys::OEM_13]            = AEVKWindows::AE_VK_KEY_OEM_13;
    m_KeyMapVK[(uint32_t)AEKeys::OEM_14]            = AEVKWindows::AE_VK_KEY_OEM_14;
    m_KeyMapVK[(uint32_t)AEKeys::OEM_15]            = AEVKWindows::AE_VK_KEY_OEM_15;
    m_KeyMapVK[(uint32_t)AEKeys::OEM_16]            = AEVKWindows::AE_VK_KEY_OEM_16;
    m_KeyMapVK[(uint32_t)AEKeys::OEM_17]            = AEVKWindows::AE_VK_KEY_OEM_17;
    m_KeyMapVK[(uint32_t)AEKeys::OEM_18]            = AEVKWindows::AE_VK_KEY_OEM_18;
    m_KeyMapVK[(uint32_t)AEKeys::OEM_19]            = AEVKWindows::AE_VK_KEY_OEM_19;
    m_KeyMapVK[(uint32_t)AEKeys::OEM_20]            = AEVKWindows::AE_VK_KEY_OEM_20;
    m_KeyMapVK[(uint32_t)AEKeys::OEM_21]            = AEVKWindows::AE_VK_KEY_OEM_21;
    m_KeyMapVK[(uint32_t)AEKeys::OEM_22]            = AEVKWindows::AE_VK_KEY_OEM_22;
    m_KeyMapVK[(uint32_t)AEKeys::OEM_23]            = AEVKWindows::AE_VK_KEY_OEM_23;
}

AEResult Keyboard::Initialize()
{
    std::lock_guard<std::mutex> lock(m_KeyboardMutex);

    //Get Keyboard Layout
    m_KeyboardLayout = GetKeyboardLayout(0);

    return AEResult::Ok;
}

AEResult Keyboard::LockKeyboard(uint64_t& lockID)
{
    std::lock_guard<std::mutex> lock(m_KeyboardMutex);

    if(m_LockID != 0)
    {
        return AEResult::KeyboardLockFailed;
    }

    m_LockID = lockID = AE_Base::GetNextUniqueID();

    return AEResult::Ok;
}

AEResult Keyboard::UnlockKeyboard(uint64_t lockID)
{
    std::lock_guard<std::mutex> lock(m_KeyboardMutex);

    if(m_LockID != lockID)
    {
        return AEResult::KeyboardUnLockFailed;
    }

    m_LockID = 0;

    return AEResult::Ok;
}

bool Keyboard::IsShiftPressMemBuffer() const
{
    return ( (m_InMemBuffer[(uint32_t)AEKeys::LSHIFT] & AE_INPUT_PRESS) || (m_InMemBuffer[(uint32_t)AEKeys::RSHIFT] & AE_INPUT_PRESS));
}

void Keyboard::PressUnpressKey(AEKeys key, bool isDown)
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

void Keyboard::PressKey(AEKeys key)
{
    std::lock_guard<std::mutex> lock(m_KeyboardMutex);

    m_InMemBuffer[(uint32_t)key] |= AE_INPUT_PRESS;
}

void Keyboard::UnpressKey(AEKeys key)
{
    std::lock_guard<std::mutex> lock(m_KeyboardMutex);

    m_InMemBuffer[(uint32_t)key] &= AE_DEV_INPUT_UNPRESS;
}

bool Keyboard::WasKeyPressed(AEKeys key, uint64_t lockID) const
{
    if(m_LockID != 0 && m_LockID != lockID)
    {
        return false;
    }

    if(!(m_PreviousKeyboardState[(uint32_t)key] & AE_INPUT_PRESS) && (m_CurrentKeyboardState[(uint32_t)key] & AE_INPUT_PRESS))
    {
        return true;
    }

    return false;
}

bool Keyboard::IsKeyUp(AEKeys key, uint64_t lockID) const
{
    if(m_LockID != 0 && m_LockID != lockID)
    {
        return false;
    }

    if(!(m_CurrentKeyboardState[(uint32_t)key] & AE_INPUT_PRESS))
    {
        return true;
    }

    return false;
}

bool Keyboard::IsKeyDown(AEKeys key, uint64_t lockID) const
{
    if(m_LockID != 0 && m_LockID != lockID)
    {
        return false;
    }

    if(m_CurrentKeyboardState[(uint32_t)key] & AE_INPUT_PRESS)
    {
        return true;
    }

    return false;
}

bool Keyboard::IsHoldingKey(AEKeys key, uint64_t lockID) const
{
    if(m_LockID != 0 && m_LockID != lockID)
    {
        return false;
    }

    if((m_PreviousKeyboardState[(uint32_t)key] & AE_INPUT_PRESS) && (m_CurrentKeyboardState[(uint32_t)key] & AE_INPUT_PRESS))
    {
        return true;
    }

    return false;
}

bool Keyboard::HasReleasedKey(AEKeys key, uint64_t lockID) const
{
    if(m_LockID != 0 && m_LockID != lockID)
    {
        return false;
    }

    if((m_PreviousKeyboardState[(uint32_t)key] & AE_INPUT_PRESS) && !(m_CurrentKeyboardState[(uint32_t)key] & AE_INPUT_PRESS))
    {
        return true;
    }

    return false;
}

wchar_t Keyboard::GetCurrentPressedChar(uint64_t lockID) const
{
    if(m_LockID != 0 && m_LockID != lockID)
    {
        return L'\0';
    }

    //ToUnicodeEx returns a key for ctrl + ANYKEY, we do not want any, so it if it is press, return no key
    if(m_CurrentKeyboardState[(uint32_t)AEKeys::LCTRL] & AE_INPUT_PRESS || m_CurrentKeyboardState[(uint32_t)AEKeys::RCTRL] & AE_INPUT_PRESS)
    {
        return L'\0';
    }

    //ToUnicodeEx returns a key for backspace, we do not want any, so it if it is press, return no key
    if(m_CurrentKeyboardState[(uint32_t)AEKeys::BACKSPACE] & AE_INPUT_PRESS)
    {
        return L'\0';
    }

    BYTE state[256];
    memset(state, 0, sizeof(BYTE) * 256);

    if (GetKeyboardState(state) == FALSE)
    {
        return L'\0';
    }

    //For Shift/Ctrl/Alt/Caps Keys we need to set them
    //0x80 = down 
    //0x01 = toggled (for Caps)
    state[VK_CAPITAL] = (BYTE)GetKeyState(VK_CAPITAL);
    state[VK_SHIFT] = (BYTE)GetKeyState(VK_SHIFT);

    for(uint32_t i = 0; i < AE_MAX_KEYS; ++i)
    {
        if(!(m_PreviousKeyboardState[i] & AE_INPUT_PRESS) && (m_CurrentKeyboardState[i] & AE_INPUT_PRESS))
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

    return L'\0';
}

void Keyboard::Update()
{
    std::lock_guard<std::mutex> lock(m_KeyboardMutex);

    HRESULT hr = S_OK;
    
    //Copy Current Key Space to Previous
    memcpy(m_PreviousKeyboardState, m_CurrentKeyboardState, AE_MAX_KEYS);

    //Copy In Memory Buffer to Current
    memcpy(m_CurrentKeyboardState, m_InMemBuffer, AE_MAX_KEYS);
}

void Keyboard::LoseFocus()
{
    //If Keyboard Looses Focus, clean memory buffer
    ZeroMemory(m_InMemBuffer, AE_MAX_KEYS);
}

