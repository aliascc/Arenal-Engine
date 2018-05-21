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

#ifndef _INPUT_MANAGER_H
#define _INPUT_MANAGER_H

/**********************
*   System Includes   *
***********************/

/*************************
*   3rd Party Includes   *
**************************/

/***************************
*   Game Engine Includes   *
****************************/
#include "InputDefs.h"

/********************
*   Forward Decls   *
*********************/
class Keyboard;
class XBoxGamepadManager;

/*****************
*   Class Decl   *
******************/

class InputManager sealed : public AEObject
{
    private:

    /************************
    *   Private Variables   *
    *************************/
#pragma region Private Variables

        /// <summary>
        /// Handles the Keyboard Input.
        /// </summary>
        Keyboard* m_Keyboard = nullptr;

        /// <summary>
        /// Handles the XBox Gamepad Input.
        /// </summary>
        XBoxGamepadManager* m_XBoxGamepadManager = nullptr;

        /// <summary>
        /// Defines if the class has been initialize and is ready to run.
        /// </summary>
        bool m_IsReady = false;

#pragma endregion

        /**********************
        *   Private Methods   *
        ***********************/
#pragma region Private Methods

        /// <summary>
        /// Cleans up the memory use by the manager.
        /// </summary>
        void CleanUp();

#pragma endregion

    public:

    /***************************************
    *   Constructor & Destructor Methods   *
    ****************************************/
#pragma region Constructor & Destructor Methods

        /// <summary>
        /// Default InputManager Constructor
        /// </summary>
        InputManager();

        /// <summary>
        /// Default InputManager Destructor
        /// </summary>
        virtual ~InputManager();

#pragma endregion

    /******************
    *   Get Methods   *
    *******************/
#pragma region Get Methods

        /// <summary>
        /// Get the Keyboard Input Handler
        /// </summary>
        inline Keyboard* GetKeyboard() const
        {
            return m_Keyboard;
        }

        /// <summary>
        /// Gets the XBox Gamepad Input Handler.
        /// </summary>
        inline XBoxGamepadManager* GetXBoxGamepadManager() const
        {
            return m_XBoxGamepadManager;
        }

#pragma endregion

    /******************
    *   Set Methods   *
    *******************/
#pragma region Set Methods

#pragma endregion

    /************************
    *   Framework Methods   *
    *************************/
#pragma region Framework Methods

        AEResult Initialize(const GameConfigInput& configInput);

        void ProcessKeyboardWindows(TCHAR keyCode, bool isExtended, bool isDown);

        void ReloadKeyboardLaylout();

        void LoseFocus();

        bool IsKeyboardActive() const;

        bool IsXBoxGamepadManagerActive() const;

#pragma endregion

};

#endif
