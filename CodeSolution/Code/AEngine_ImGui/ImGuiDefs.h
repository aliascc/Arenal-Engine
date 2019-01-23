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

/************
*   Enums   *
*************/

enum class ImGuiMenuObjectType : uint32_t
{
    Menu        = 0,
    Item        = 1,
    Separator   = 2
};

/**************
*   Defines   *
***************/

#define AE_LITERAL_UI_MAIN_MENU_NAME        "AE_UI_MAIN_MENU_NAME"

#define AE_DOCKING_SPACE_NAME               "AE Docking Space"

#define AE_DOCKING_SPACE_WINDOW_NAME        "AE Docking Space Window"

/**************
*   Typedef   *
***************/
typedef std::function<void(uint64_t)> MenuItemClicked;

#endif //AE_EDITOR_MODE