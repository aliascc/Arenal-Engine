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
#include "precomp_editor.h"

/**********************
*   System Includes   *
***********************/

/*************************
*   3rd Party Includes   *
**************************/

/***************************
*   Game Engine Includes   *
****************************/
#include "UI/UIDefs.h"
#include "ImGuiWindow.h"
#include "ImGuiManager.h"
#include "ImGuiMenuItem.h"

//Always include last
#include "Memory\MemLeaks.h"
#include "UIViewMenu.h"

/********************
*   Function Defs   *
*********************/

UIViewMenu::UIViewMenu(ImGuiManager& imGuiManager, uint32_t renderPriority)
    : ImGuiMenu(imGuiManager, AE_UI_VIEW_MENU_NAME, AE_LITERAL_UI_VIEW_MENU_NAME, renderPriority)
{
}

UIViewMenu::~UIViewMenu()
{
    if (m_ViewRenderWindow != nullptr)
    {
        m_ViewRenderWindow->UnRegisterOnClickCallback(GetUniqueID());
        DeleteMem(m_ViewRenderWindow);
    }
}

void UIViewMenu::ViewRenderWindowClicked(uint64_t id)
{
    ImGuiWindow* renderWindow = m_ImGuiManager.GetImGuiWindow(AE_UI_RENDER_WINDOW_NAME);

    AEAssert(renderWindow != nullptr);
    if (renderWindow == nullptr)
    {
        return;
    }

    renderWindow->SetIsVisible(true);
}

AEResult UIViewMenu::Initialize()
{
    m_ViewRenderWindow = new ImGuiMenuItem(m_ImGuiManager, AE_UI_VIEW_RENDER_WINDOW_NAME, AE_LITERAL_UI_VIEW_MENU_RENDER_WINDOW_NAME, AE_UI_VIEW_MENU_RENDER_WINDOW_PRIORITY);
    AddMenuObject(m_ViewRenderWindow);

    MenuItemClicked cb = std::bind(&UIViewMenu::ViewRenderWindowClicked, this, std::placeholders::_1);
    m_ViewRenderWindow->RegisterOnClickCallback(GetUniqueID(), cb);

    return AEResult::Ok;
}

