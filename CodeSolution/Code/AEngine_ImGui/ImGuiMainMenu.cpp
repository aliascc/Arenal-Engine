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
#include "precomp_imgui.h"

/**********************
*   System Includes   *
***********************/

/*************************
*   3rd Party Includes   *
**************************/

/***************************
*   Game Engine Includes   *
****************************/
#include "ImGuiDefs.h"
#include "ImGuiMainMenu.h"

//Always include last
#include "Memory\MemLeaks.h"

/*********************
*   Framework Defs   *
**********************/

#ifdef AE_EDITOR_MODE

ImGuiMainMenu::ImGuiMainMenu(ImGuiManager& imGuiManager)
    : ImGuiMenu(imGuiManager, "Main Menu", AE_LITERAL_UI_MAIN_MENU_NAME, 0, true)
{
}

ImGuiMainMenu::~ImGuiMainMenu()
{
}

AEResult ImGuiMainMenu::Initialize()
{
    return AEResult::Ok;
}

void ImGuiMainMenu::Update(const TimerParams& timerParams)
{
    if (!ImGui::BeginMainMenuBar())
    {
        return;
    }

    UpdateMethods(timerParams);

    ImGui::EndMainMenuBar();
}

#endif //AE_EDITOR_MODE