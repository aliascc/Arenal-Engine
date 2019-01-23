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

/*******************
*   Forward Decl   *
********************/
class GameApp;
class UIMainMenu;
class ImGuiManager;
class UIRenderWidget;

/****************
*   Class Def   *
*****************/

class UIManager : public AEObject
{
private:

    GameApp& m_GameApp;

    /// <summary>
    /// ImGui Manager Instance
    /// </summary>
    ImGuiManager& m_ImGuiManager;

    /// <summary>
    /// UI Render Widget
    /// </summary>
    UIRenderWidget* m_UIRenderWidget = nullptr;

    /// <summary>
    /// UI Main Menu
    /// </summary>
    UIMainMenu* m_UIMainMenu = nullptr;

public:

    /// <summary>
    /// Default Constructor for UIManager
    /// </summary>
    /// <param name="gameApp">Game App Instance</param>
    UIManager(GameApp& gameApp);

    /// <summary>
    /// Destructor for UIManager
    /// </summary>
    ~UIManager();

    /// <summary>
    /// Adds the Editor Widgets to the ImGui Manager
    /// </summary>
    /// <returns>Returns AEResult::Ok if successful, otherwise an error code</returns>
    AEResult AddEditorWidgets();
};