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
#include "UIManager.h"
#include "ImGuiManager.h"
#include "GameApp/GameApp.h"
#include "Main Menu/UIMainMenu.h"
#include "UI/Widgets/Render/UIRenderWidget.h"

//Always include last
#include "Memory\MemLeaks.h"

/********************
*   Function Defs   *
*********************/

UIManager::UIManager(GameApp& gameApp)
    : m_GameApp(gameApp)
    , m_ImGuiManager(gameApp.GetImGuiManager())
{
}

UIManager::~UIManager()
{
    DeleteMem(m_UIRenderWidget);
    DeleteMem(m_UIMainMenu);
}

AEResult UIManager::AddEditorWidgets()
{
    m_UIMainMenu = new UIMainMenu(m_ImGuiManager);
    m_UIMainMenu->AddEditorMenus();

    m_UIRenderWidget = new UIRenderWidget(m_ImGuiManager, m_GameApp, m_GameApp.GetGraphicsDevice());
    m_UIRenderWidget->SetIsVisible(true);

    m_ImGuiManager.AddImGuiWindow(m_UIRenderWidget);

    return AEResult::Ok;
}