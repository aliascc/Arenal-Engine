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
#include "GraphicDevice.h"
#include "UIRenderWidget.h"
#include "GameApp/GameApp.h"
#include "Core Game Command/CoreCommands.h"
#include "Game Command/GameCommandManager.h"

//Always include last
#include "Memory\MemLeaks.h"

/********************
*   Function Defs   *
*********************/

UIRenderWidget::UIRenderWidget(ImGuiManager& imGuiManager, GameApp& gameApp, GraphicDevice& graphicDevice)
    : ImGuiWindow(imGuiManager, AE_UI_RENDER_WINDOW_NAME, AE_LITERAL_UI_RENDER_WINDOW_NAME, AE_UI_RENDER_WINDOW_PRIORITY)
    , m_GameApp(gameApp)
    , m_GraphicDevice(graphicDevice)
{
}

UIRenderWidget::~UIRenderWidget()
{
}

void UIRenderWidget::UpdateWindow(const TimerParams& timerParams)
{
    const ImVec2 pos    = ImGui::GetCursorScreenPos();
    const ImVec2 pos2   = ImGui::GetContentRegionAvail();

    const ImVec2 size   = ImVec2(pos.x + pos2.x, pos.y + pos2.y);

    const ImVec2 windowSize = ImGui::GetWindowSize();

    GraphicsPresentationParameters& graphicPP = m_GraphicDevice.GetGraphicPP();

    if (graphicPP.m_GameBackBufferWidth != windowSize.x || graphicPP.m_GameBackBufferHeight != windowSize.y)
    {
        glm::ivec2 newSize = { windowSize.x, windowSize.y };
        ResizeCommand* rc = new ResizeCommand(m_GameApp, newSize);
        GameCommandManager::GetInstance().AddCommand(rc);
    }

    //////////////////////
    // Adds the Render Target to the List of Images for the Window
    // to render
    ImTextureID user_texture_id = m_GraphicDevice.GetGameRenderTargetSRV();
    ImDrawList* draw_list = ImGui::GetWindowDrawList();
    draw_list->AddImage(user_texture_id, pos, size);
}