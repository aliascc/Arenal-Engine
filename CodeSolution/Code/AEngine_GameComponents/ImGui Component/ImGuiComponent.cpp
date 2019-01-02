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
#include "ImGuiManager.h"
#include "ImGuiMainMenu.h"
#include "ImGuiComponent.h"

//Always include last
#include "Memory\MemLeaks.h"

/********************
*   Function Defs   *
*********************/
ImGuiComponent::ImGuiComponent(GameApp& gameApp, const std::string& gameComponentName, uint32_t callOrder)
    : DrawableGameComponent(gameApp, gameComponentName, callOrder)
    , m_ImGuiManager(gameApp.GetImGuiManager())
{
}

ImGuiComponent::~ImGuiComponent()
{
    CleanUp();
}

void ImGuiComponent::CleanUp()
{
    DeleteMem(m_ImGuiMainMenu);
}

void ImGuiComponent::Initialize()
{
    AEResult res = AEResult::Ok;

    m_ImGuiMainMenu = new ImGuiMainMenu();
    AEAssert(m_ImGuiMainMenu != nullptr);

    res = m_ImGuiManager.AddImGuiObject(m_ImGuiMainMenu);
    if (res != AEResult::Ok)
    {
        AETODO("Log error information");
    }
}

void ImGuiComponent::Update(const TimerParams& timerParams)
{
    m_ImGuiManager.Update(timerParams);
}

void ImGuiComponent::Render(const TimerParams& timerParams)
{
    m_ImGuiManager.Render(timerParams);
}
