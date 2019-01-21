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
#include "ImGuiMenuSeparator.h"
#include "Localization/LocalizationManagerDefs.h"

//Always include last
#include "Memory\MemLeaks.h"

/*********************
*   Framework Defs   *
**********************/

#ifdef AE_EDITOR_MODE

ImGuiMenuSeparator::ImGuiMenuSeparator(const std::string& name, uint32_t renderPriority, bool visible)
    : ImGuiMenuObject(ImGuiMenuObjectType::Separator, name, "", renderPriority, visible)
{
}

ImGuiMenuSeparator::~ImGuiMenuSeparator()
{
}

void ImGuiMenuSeparator::Update(const TimerParams& timerParams)
{
    ImGui::Separator();
}

#endif //AE_EDITOR_MODE