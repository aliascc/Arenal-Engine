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
#include "ImGuiMenu.h"
#include "Localization/LocalizationManagerDefs.h"

//Always include last
#include "Memory\MemLeaks.h"

/*********************
*   Framework Defs   *
**********************/

#ifdef AE_EDITOR_MODE

ImGuiMenu::ImGuiMenu(const std::string& name, const std::string& menuNameLiteral, uint32_t renderPriority, bool visible)
    : ImGuiMenuObject(name, renderPriority, visible)
    , m_MenuNameLiteral(menuNameLiteral)
{
}

ImGuiMenu::~ImGuiMenu()
{
    m_ImGuiMenuObjectVector.clear();
}

AEResult ImGuiMenu::AddMenuObject(ImGuiMenuObject* menuObject)
{
    if (menuObject == nullptr)
    {
        return AEResult::NullParameter;
    }

    m_ImGuiMenuObjectVector.push_back(menuObject);

    std::sort(m_ImGuiMenuObjectVector.begin(), m_ImGuiMenuObjectVector.end(),
        [](ImGuiMenuObject* left, ImGuiMenuObject* right)
    {
        return left->GetRenderPriority() < right->GetRenderPriority();
    });

    return AEResult::Ok;
}

AEResult ImGuiMenu::RemoveMenuObject(uint64_t menuObjectID)
{
    auto it = m_ImGuiMenuObjectVector.begin();
    for (; (*it)->GetUniqueID() != menuObjectID && it != m_ImGuiMenuObjectVector.end(); it++);

    if (it == m_ImGuiMenuObjectVector.end())
    {
        return AEResult::NotFound;
    }

    m_ImGuiMenuObjectVector.erase(it);

    return AEResult::Ok;
}

void ImGuiMenu::UpdateMethods(const TimerParams& timerParams)
{
    size_t size = m_ImGuiMenuObjectVector.size();
    for (size_t i = 0; i < size; i++)
    {
        ImGuiMenuObject* menuObject = m_ImGuiMenuObjectVector[i];
        if (menuObject->IsVisible())
        {
            menuObject->Update(timerParams);
        }
    }
}

void ImGuiMenu::Update(const TimerParams& timerParams)
{
    const std::string menuName = AELOCMAN.GetLiteral(m_MenuNameLiteral);

    if (!ImGui::BeginMenu(menuName.c_str()))
    {
        return;
    }

    UpdateMethods(timerParams);

    /*
    if (ImGui::BeginMenu("Edit"))
    {
        if (ImGui::MenuItem("Undo", "CTRL+Z")) {}
        if (ImGui::MenuItem("Redo", "CTRL+Y", false, false)) {}  // Disabled item
        ImGui::Separator();
        if (ImGui::MenuItem("Cut", "CTRL+X")) {}
        if (ImGui::MenuItem("Copy", "CTRL+C")) {}
        if (ImGui::MenuItem("Paste", "CTRL+V")) {}
        ImGui::EndMenu();
    }*/

    ImGui::EndMenu();
}

#endif //AE_EDITOR_MODE