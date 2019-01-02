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

#ifndef _IMGUI_COMPONENT_H
#define _IMGUI_COMPONENT_H

/**********************
*   System Includes   *
***********************/

/*************************
*   3rd Party Includes   *
**************************/

/***************************
*   Game Engine Includes   *
****************************/
#include "AEGameComponentsDefs.h"

/********************
*   Forward Decls   *
*********************/
struct TimerParams;
class ImGuiManager;
class AudioListener;

/*****************
*   Class Decl   *
******************/

class ImGuiComponent sealed : public DrawableGameComponent
{
private:

    ImGuiManager& m_ImGuiManager;

    void CleanUp();

public:

    //Constructor Destructor.
    ImGuiComponent(GameApp& gameApp, const std::string& gameComponentName = AE_IMGUI_DEF_COMPONENT_NAME, uint32_t callOrder = AEGameComponentCallOrder::_AE_GCCO_ImGui);

    virtual ~ImGuiComponent();

    //Game Component Override methods
    void Initialize() override;

    void Update(const TimerParams& timerParams) override;

    void Render(const TimerParams& timerParams) override;
};

#endif
