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
#include "ImGuiWindow.h"

/********************
*   Forward Decls   *
*********************/
class GameApp;
class GraphicDevice;

/*****************
*   Class Decl   *
******************/

class UIRenderWidget sealed : public ImGuiWindow
{
private:

    /// <summary>
    /// Game App Instance
    /// </summary>
    GameApp& m_GameApp;

    /// <summary>
    /// Graphic Device
    /// </summary>
    GraphicDevice& m_GraphicDevice;

    /// <see cref="ImGuiWindow::UpdateWindow(const TimerParams&)"/>
    void UpdateWindow(const TimerParams& timerParams) override;

public:

    /// <summary>
    /// Constructor for UIRenderWidget
    /// </summary>
    /// <param name="gameApp">Game App Instance</param>
    /// <param name="graphicDevice">Graphic Device of the application</param>
    UIRenderWidget(GameApp& gameApp, GraphicDevice& graphicDevice);

    /// <summary>
    /// Destructor for UIRenderWidget
    /// </summary>
    ~UIRenderWidget();
};