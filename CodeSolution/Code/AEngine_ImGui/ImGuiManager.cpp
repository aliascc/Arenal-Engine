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
#include "precomp_imgui.h"

#ifdef AE_EDITOR_MODE

/**********************
*   System Includes   *
***********************/

/*************************
*   3rd Party Includes   *
**************************/

/***************************
*   Game Engine Includes   *
****************************/
#include "ImGuiWindow.h"
#include "ImGuiManager.h"
#include "GraphicDevice.h"

//Always include last
#include "Memory\MemLeaks.h"

/*********************
*   Framework Defs   *
**********************/

ImGuiManager::ImGuiManager(GraphicDevice& graphicDevice)
    : m_GraphicDevice(graphicDevice)
{
}

ImGuiManager::~ImGuiManager()
{
}

AEResult ImGuiManager::AddWindow(ImGuiWindow* window)
{
    if (window == nullptr)
    {
        return AEResult::NullParameter;
    }

    return AEResult::Fail;
}

AEResult ImGuiManager::RemoveWindow(uint64_t windowID)
{
    return AEResult::Fail;
}

AEResult ImGuiManager::Initialize()
{
    return AEResult::Fail;
}

void ImGuiManager::PreRender()
{
    ImGui_ImplDX11_NewFrame();
}

void ImGuiManager::Render()
{
    ImGui::Render();
    ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}

#endif //AE_EDITOR_MODE