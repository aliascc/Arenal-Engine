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

#ifdef AE_EDITOR_MODE

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

/*******************
*   Forward Decl   *
********************/

/*****************
*   Class Decl   *
******************/

class ImGuiMainMenu sealed : public ImGuiMenu
{
private:

public:

    /// <summary>
    /// ImGUIObject Constructor
    /// </summary>
    /// <param name="imGuiManager">ImGuiManager Instance</param>
    ImGuiMainMenu(ImGuiManager& imGuiManager);

    /// <summary>
    /// Default ImGUIObject Destructor
    /// </summary>
    virtual ~ImGuiMainMenu();

    /// <see cref="ImGuiMenu::Initialize(const TimerParams&)"/>
    AEResult Initialize() override;

    /// <see cref="ImGuiObject::Update(const TimerParams&)"/>
    void Update(const TimerParams& timerParams) override;
};

#endif //AE_EDITOR_MODE