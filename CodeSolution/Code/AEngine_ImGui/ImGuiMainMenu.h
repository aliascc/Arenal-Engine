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
#include "ImGuiWindow.h"

/*****************
*   Class Decl   *
******************/
class ImGuiMainMenu : public ImGuiObject
{
private:

    /************************
    *   Private Variables   *
    *************************/
#pragma region Private Variables

#pragma endregion

    /**********************
    *   Private Methods   *
    ***********************/
#pragma region Private Methods

#pragma endregion

public:

    /***************************************
    *   Constructor & Destructor Methods   *
    ****************************************/
#pragma region Constructor & Destructor Methods

    /// <summary>
    /// ImGUIObject Constructor
    /// </summary>
    ImGuiMainMenu();

    /// <summary>
    /// Default ImGUIObject Destructor
    /// </summary>
    virtual ~ImGuiMainMenu();

#pragma endregion

    /******************
    *   Get Methods   *
    *******************/
#pragma region Get Methods

#pragma endregion

    /******************
    *   Set Methods   *
    *******************/
#pragma region Set Methods

#pragma endregion

    /************************
    *   Framework Methods   *
    *************************/
#pragma region Framework Methods

    /// <see cref="ImGuiObject::UpdateWindow(const TimerParams&)"/>
    void Update(const TimerParams& timerParams) override;

#pragma endregion

};

#endif //AE_EDITOR_MODE