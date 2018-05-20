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

#ifndef _KEYBOARD_H
#define _KEYBOARD_H

/**********************
*   System Includes   *
***********************/

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

        char GetCurrentPressedChar(uint64_t lockID = 0) const;

        AEResult Initialize();

        void Update();

        void LoseFocus();
};

#endif
