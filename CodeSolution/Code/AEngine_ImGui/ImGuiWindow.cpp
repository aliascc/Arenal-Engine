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

//Always include last
#include "Memory\MemLeaks.h"

/*********************
*   Framework Defs   *
**********************/

ImGuiWindow::ImGuiWindow(const std::string& name, bool visible, ImGuiWindowFlags flags, bool showCloseIcon)
    : m_Name(name)
    , m_Visible(visible)
    , m_Flags(flags)
    , m_ShowCloseIcon(showCloseIcon)
{
}

ImGuiWindow::~ImGuiWindow()
{
}

bool ImGuiWindow::PreUpdate()
{
    if (!ImGui::Begin("Benchmark", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav))
    {
        ImGui::End();
        return false;
    }

    return true;
}

void ImGuiWindow::Update()
{
    if (!m_Visible)
    {
        return;
    }

    bool ret = PreUpdate();
    if (!ret)
    {
        return;
    }

    UpdateWindow();

    ImGui::End();
}
