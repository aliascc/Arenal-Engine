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

#ifndef _INPUT_HANDLER_H
#define _INPUT_HANDLER_H

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
#include "AEGameComponentsDefs.h"

/********************
*   Forward Decls   *
*********************/
class GameApp;
class Keyboard;
struct TimerParams;
class XBoxGamepadManager;

/*****************
*   Class Decl   *
******************/
class InputHandler sealed : public GameComponent
{
    private:

    /************************
    *   Private Variables   *
    *************************/
#pragma region Private Variables

        /// <summary>
        /// Name of the Game Service that will be registered for input handler.
        /// </summary>
        std::string m_ServiceName = "";

#pragma endregion

    public:

    /***************************************
    *   Constructor & Destructor Methods   *
    ****************************************/
#pragma region Constructor & Destructor Methods

        /// <summary>
        /// Default InputManager Constructor
        /// </summary>
        InputHandler(GameApp& gameApp, GameResourceManager& gameResourceManager, const std::string& gameComponentName = AE_INPUT_HANDLER_DEF_COMPONENT_NAME, const std::string& serviceName = AE_INPUT_HANDLER_DEF_SERVICE_NAME, uint32_t callOrder = AEGameComponentCallOrder::_AE_GCCO_InputHandler);

        /// <summary>
        /// Default InputHandler Destructor
        /// </summary>
        virtual ~InputHandler();

#pragma endregion

        /******************
        *   Get Methods   *
        *******************/
#pragma region Get Methods

        bool IsKeyboardActive() const;

        bool IsXBoxGamepadManagerActive() const;

        Keyboard* GetKeyboard() const;

        XBoxGamepadManager* GetXBoxGamepadManager() const;

#pragma endregion

        /************************
        *   Framework Methods   *
        *************************/
#pragma region Framework Methods

        void Update(const TimerParams& timerParams) override;

#pragma endregion
};

#endif
