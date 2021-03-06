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

/*************************
*   Precompiled Header   *
**************************/
#include "precomp_gamecomponents.h"

/**********************
*   System Includes   *
***********************/

/***************************
*   Game Engine Includes   *
****************************/
#include "Keyboard.h"
#include "InputHandler.h"
#include "InputManager.h"
#include "XBoxGamepadManager.h"

//Always include last
#include "Memory\MemLeaks.h"

/********************
*   Function Defs   *
*********************/
InputHandler::InputHandler(GameApp& gameApp, const std::string& gameComponentName, const std::string& serviceName, uint32_t callOrder)
    : GameComponent(gameApp, gameComponentName, callOrder)
    , m_InputManager(gameApp.GetInputManager())
    , m_ServiceName(serviceName)
{
    //Register this Game Component as a service so others can use it
    m_GameApp.RegisterGameService(m_ServiceName, this);
}

InputHandler::~InputHandler()
{
    //Un-register this component as a service because this class is been destroyed
    m_GameApp.UnRegisterGameService(m_ServiceName);
}

void InputHandler::Update(const TimerParams& timerParams)
{
    if (m_InputManager.IsKeyboardActive())
    {
        Keyboard* keyboard = m_InputManager.GetKeyboard();

        keyboard->Update();

        AETODO("See to remove this");
        if (keyboard->IsHoldingKey(AEKeys::LALT))
        {
            if (keyboard->WasKeyPressed(AEKeys::NUMPADENTER))
            {
                AETODO("send as command to command manager");
                //m_GameApp.SetFullScreen(true);
            }
            if (keyboard->WasKeyPressed(AEKeys::ENTER))
            {
                AETODO("send as command to command manager");
                //m_GameApp.SetFullScreen(false);
            }
        }
    }

    if (m_InputManager.IsXBoxGamepadManagerActive())
    {
        XBoxGamepadManager* xboxGamepadManager = m_InputManager.GetXBoxGamepadManager();

        xboxGamepadManager->Update(timerParams);
    }
}

Keyboard* InputHandler::GetKeyboard() const
{
    return m_InputManager.GetKeyboard();
}

XBoxGamepadManager* InputHandler::GetXBoxGamepadManager() const
{
    return m_InputManager.GetXBoxGamepadManager();
}

bool InputHandler::IsKeyboardActive() const
{
    return m_InputManager.IsKeyboardActive();
}

bool InputHandler::IsXBoxGamepadManagerActive() const
{
    AETODO("Check How to do this better, maybe pass the reference to input manager or use service locator at the start...");
    return m_InputManager.IsXBoxGamepadManagerActive();
}
