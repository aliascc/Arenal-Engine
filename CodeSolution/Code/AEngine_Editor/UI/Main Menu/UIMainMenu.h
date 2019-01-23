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

/********************
*   Forward Decls   *
*********************/
class UIFileMenu;
class UIViewMenu;
class ImGuiMainMenu;

/*****************
*   Class Decl   *
******************/

class UIMainMenu : public AEObject
{
private:

    /// <summary>
    /// ImGui Manager Instance
    /// </summary>
    ImGuiManager& m_ImGuiManager;

    /// <summary>
    /// ImGui Main Menu
    /// </summary>
    ImGuiMainMenu& m_ImGuiMainMenu;

    /// <summary>
    /// File Menu
    /// </summary>
    UIFileMenu* m_FileMenu = nullptr;

    /// <summary>
    /// View Menu
    /// </summary>
    UIViewMenu* m_ViewMenu = nullptr;

public:

    /// <summary>
    /// UIMain Menu Constructor
    /// </summary>
    /// <param name="imGuiManage">ImGui Manager</param>
    UIMainMenu(ImGuiManager& imGuiManager);

    /// <summary>
    /// UIMain Menu Destructor
    /// </summary>
    ~UIMainMenu();

    /// <summary>
    /// Add the Editor Menus
    /// </summary>
    void AddEditorMenus();
};