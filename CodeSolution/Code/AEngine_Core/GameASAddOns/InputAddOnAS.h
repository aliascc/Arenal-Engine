/*
* Copyright (c) 2018 <Carlos Chac�n>
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

#ifndef _INPUT_ADD_ON_AS_H
#define _INPUT_ADD_ON_AS_H

/**********************
*   System Includes   *
***********************/

/*************************
*   3rd Party Includes   *
**************************/

/***************************
*   Game Engine Includes   *
****************************/
#include "AngelScript\Add-ons\AngelScriptAddOn.h"

/************
*   Using   *
*************/

/********************
*   Forward Decls   *
*********************/
class InputManager;

/*****************
*   Class Decl   *
******************/

class InputAddOnAS sealed : public AngelScriptAddOn
{
    private:

        /************************
        *   Private Variables   *
        *************************/
#pragma region Private Methods

        /// <summary>
        /// Input Object for current Game App
        /// </summary>
        InputManager* m_InputManager = nullptr;

#pragma endregion

        /**********************
        *   Private Methods   *
        ***********************/
#pragma region Private Methods

        /// <summary>
        /// Registers AEKeys Enum Object with Angel Script Manager
        /// </summary>
        /// <param name="engine">AS Engine to register with.</param>
        /// <returns>Return Ok if registration was successfully</returns>
        AEResult RegisterAEKeysEnum(asIScriptEngine* engine);

        /// <summary>
        /// Registers AEXBoxGamepadMovements Enum Object with Angel Script Manager
        /// </summary>
        /// <param name="engine">AS Engine to register with.</param>
        /// <returns>Return Ok if registration was successfully</returns>
        AEResult RegisterAEXBoxGamepadMovementsEnum(asIScriptEngine* engine);

        /// <summary>
        /// Registers AEXBoxGamepadButton Enum Object with Angel Script Manager
        /// </summary>
        /// <param name="engine">AS Engine to register with.</param>
        /// <returns>Return Ok if registration was successfully</returns>
        AEResult RegisterAEXBoxGamepadButtonEnum(asIScriptEngine* engine);

        /// <summary>
        /// Registers Keyboard Object with Angel Script Manager
        /// </summary>
        /// <param name="engine">AS Engine to register with.</param>
        /// <returns>Return Ok if registration was successfully</returns>
        AEResult RegisterKeyboardObject(asIScriptEngine* engine);

        /// <summary>
        /// Registers XBox Gamepad Object with Angel Script Manager
        /// </summary>
        /// <param name="engine">AS Engine to register with.</param>
        /// <returns>Return Ok if registration was successfully</returns>
        AEResult RegisterXBoxGamepadObject(asIScriptEngine* engine);

        /// <summary>
        /// Registers XBox Gamepad Manager Object with Angel Script Manager
        /// </summary>
        /// <param name="engine">AS Engine to register with.</param>
        /// <returns>Return Ok if registration was successfully</returns>
        AEResult RegisterXBoxGamepadManagerObject(asIScriptEngine* engine);

        /// <summary>
        /// Registers Input Manager Object with Angel Script Manager
        /// </summary>
        /// <param name="engine">AS Engine to register with.</param>
        /// <returns>Return Ok if registration was successfully</returns>
        AEResult RegisterInputManagerObject(asIScriptEngine* engine);

        /// <summary>
        /// Registers Input Manager Object Pointer with Angel Script Manager
        /// </summary>
        /// <param name="engine">AS Engine to register with.</param>
        /// <returns>Return Ok if registration was successfully</returns>
        AEResult RegisterInputManagerObjectPointer(asIScriptEngine* engine);

#pragma endregion

    public:

        /***************************************
        *   Constructor & Destructor Methods   *
        ****************************************/
#pragma region Constructor & Destructor Methods

        /// <summary>
        /// InputAddOnAS Constructor
        /// </summary>
        /// <param name="inputManager">Input Manager Object for current Game App.</param>
        InputAddOnAS(InputManager* inputManager);

        /// <summary>
        /// Default InputAddOnAS Destructor
        /// </summary>
        virtual ~InputAddOnAS();

#pragma endregion

        /******************
        *   Get Methods   *
        *******************/
#pragma region Get Methods

#pragma endregion

        /************************
        *   Framework Methods   *
        *************************/
#pragma region Framework Methods

        /// <summary>
        /// Registers the Add-on with the AS Engine.
        /// </summary>
        /// <param name="engine">AS Engine to register with.</param>
        AEResult Register(asIScriptEngine* engine) override;

#pragma endregion

};

#endif
