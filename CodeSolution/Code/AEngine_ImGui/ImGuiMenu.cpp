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

ImGuiMenu::ImGuiMenu(ImGuiManager& imGuiManager, const std::string& name, const std::string& literalName, uint32_t renderPriority, bool visible)
    : ImGuiMenuObject(imGuiManager, ImGuiMenuObjectType::Menu, name, literalName, renderPriority, visible)
{
}

ImGuiMenu::~ImGuiMenu()
{
    m_ImGuiMenuObjectVector.clear();
    m_ImGuiMenuObjectMap.clear();
}

AEResult ImGuiMenu::AddMenuObject(ImGuiMenuObject* menuObject)
{
    if (menuObject == nullptr)
    {
        return AEResult::NullParameter;
    }

    m_ImGuiMenuObjectMap[menuObject->GetName()] = menuObject;
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
    for (; (*it)->GetUniqueID() != menuObjectID && it != m_ImGuiMenuObjectVector.end(); ++it);

    if (it == m_ImGuiMenuObjectVector.end())
    {
        return AEResult::NotFound;
    }

    m_ImGuiMenuObjectMap.erase((*it)->GetName());
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

AEResult ImGuiMenu::GetSubMenu(const std::string& menuName, ImGuiMenu** subMenu)
{
    if (subMenu == nullptr)
    {
        return AEResult::NullParameter;
    }

    *subMenu = nullptr;

    auto& it = m_ImGuiMenuObjectMap.find(menuName);
    if (it == m_ImGuiMenuObjectMap.end())
    {
        return AEResult::NotFound;
    }

    if (it->second->GetImGuiMenuObjectType() != ImGuiMenuObjectType::Menu)
    {
        return AEResult::InvalidObjType;
    }

    *subMenu = reinterpret_cast<ImGuiMenu*>(it->second);

    return AEResult::Ok;
}

AEResult ImGuiMenu::GetSubMenu(uint64_t subMenuObjectID, ImGuiMenu** subMenu)
{
    if (subMenu == nullptr)
    {
        return AEResult::NullParameter;
    }

    *subMenu = nullptr;

    size_t size = m_ImGuiMenuObjectVector.size();
    for (size_t i = 0; i < size; i++)
    {
        ImGuiMenuObject* temp = m_ImGuiMenuObjectVector[i];
        if (temp->GetUniqueID() == subMenuObjectID)
        {
            if (temp->GetImGuiMenuObjectType() != ImGuiMenuObjectType::Menu)
            {
                return AEResult::InvalidObjType;
            }
            else
            {
                *subMenu = reinterpret_cast<ImGuiMenu*>(temp);
                break;
            }
        }
    }

    if (subMenu == nullptr)
    {
        return AEResult::NotFound;
    }

    return AEResult::Ok;
}

AEResult ImGuiMenu::GetSubMenuLeaf(const std::string& menuTree, ImGuiMenu** subMenu)
{
    if (subMenu == nullptr)
    {
        return AEResult::NullParameter;
    }
    *subMenu = nullptr;

    // Get the Names of the Sub Menus
    std::vector<std::string> subMenuNames;
    AE_Base::SplitString(menuTree, subMenuNames, "/", true);

    if (subMenuNames.empty())
    {
        return AEResult::NotFound;
    }

    AEResult res = AEResult::Ok;
    ImGuiMenu* temp = this;

    size_t size = subMenuNames.size();
    for (size_t i = 0; i < size && res == AEResult::Ok; ++i)
    {
        res = temp->GetSubMenu(subMenuNames[i], &temp);
    }

    *subMenu = temp;
    return res;
}

void ImGuiMenu::Update(const TimerParams& timerParams)
{
    const std::string& menuName = AELOCMAN.GetLiteral(m_LiteralName);

    if (!ImGui::BeginMenu(menuName.c_str()))
    {
        return;
    }

    UpdateMethods(timerParams);

    ImGui::EndMenu();
}

#endif //AE_EDITOR_MODE