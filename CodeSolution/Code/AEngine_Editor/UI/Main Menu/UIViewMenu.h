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

#pragma once

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

/********************
*   Forward Decls   *
*********************/
class ImGuiMenuItem;

/*****************
*   Class Decl   *
******************/

/// <summary>
/// UI View Menu
/// </summary>
class UIViewMenu sealed : public ImGuiMenu
{
private:

    /// <summary>
    /// Menu Item to view the Render Window
    /// </summary>
    ImGuiMenuItem* m_ViewRenderWindow;

    /// <summary>
    /// Callback of the View Render Menu Item on Clicked
    /// </summary>
    /// <param name="id">ID of the Menu Item</param>
    void ViewRenderWindowClicked(uint64_t id);

public:

    /// <summary>
    /// UIViewMenu Constructor
    /// </summary>
    /// <param name="imGuiManager">ImGuiManager Instance</param>
    /// <param name="renderPriority">Render Priority of the Menu</param>
    UIViewMenu(ImGuiManager& imGuiManager, uint32_t renderPriority);

    /// <summary>
    /// UIViewMenu Destructor
    /// </summary>
    ~UIViewMenu();

    /// <see cref="ImGuiMenu::Initialize(const TimerParams&)"/>
    AEResult Initialize() override;
};
