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
#include "UIMainMenu.h"
#include "ImGuiManager.h"
#include "ImGuiMainMenu.h"
#include "UI/Main Menu/UIFileMenu.h"
#include "UI/Main Menu/UIViewMenu.h"

//Always include last
#include "Memory\MemLeaks.h"

/********************
*   Function Defs   *
*********************/

UIMainMenu::UIMainMenu(ImGuiManager& imGuiManager)
    : m_ImGuiManager(imGuiManager)
    , m_ImGuiMainMenu(imGuiManager.GetImGuiMainMenu())
{
}

UIMainMenu::~UIMainMenu()
{
    DeleteMem(m_FileMenu);
    DeleteMem(m_ViewMenu);
}

void UIMainMenu::AddEditorMenus()
{
    m_FileMenu = new UIFileMenu(m_ImGuiManager, AE_UI_FILE_MENU_PRIORITY);
    m_FileMenu->Initialize();
    m_ImGuiMainMenu.AddMenuObject(m_FileMenu);

    m_ViewMenu = new UIViewMenu(m_ImGuiManager, AE_UI_VIEW_MENU_PRIORITY);
    m_ViewMenu->Initialize();
    m_ImGuiMainMenu.AddMenuObject(m_ViewMenu);
}
