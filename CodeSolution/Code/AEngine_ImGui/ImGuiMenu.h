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
#include "ImGuiMenuObject.h"

/*****************
*   Class Decl   *
******************/
class ImGuiMenu : public ImGuiMenuObject
{
    typedef std::vector<ImGuiMenuObject*> ImGuiMenuObjectVector;

protected:

    /**********************
    *   Private Methods   *
    ***********************/
#pragma region Private Methods

    std::vector<ImGuiMenuObject*> m_ImGuiMenuObjectVector;

    std::string m_MenuNameLiteral;

#pragma endregion

    /**************************
    *   Protected Variables   *
    ***************************/
#pragma region Protected Variables

#pragma endregion

    /************************
    *   Protected Methods   *
    *************************/
#pragma region Protected Methods

    void UpdateMethods(const TimerParams& timerParams);

#pragma endregion

public:

    /***************************************
    *   Constructor & Destructor Methods   *
    ****************************************/
#pragma region Constructor & Destructor Methods

    /// <summary>
    /// ImGuiMenu Constructor
    /// </summary>
    /// <param name="name">Name of the Menu</param>
    /// <param name="menuNameLiteral">Literal for the Menu Name</param>
    /// <param name="renderPriority">Render Priority for the menu</param>
    /// <param name="visible">Determines if the menu is visible or not</param>
    ImGuiMenu(const std::string& name, const std::string& menuNameLiteral, uint32_t renderPriority, bool visible = true);

    /// <summary>
    /// Default ImGuiMenu Destructor
    /// </summary>
    virtual ~ImGuiMenu();

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

    AEResult AddMenuObject(ImGuiMenuObject* menuObject);

    AEResult RemoveMenuObject(uint64_t menuObjectID);

    /// <see cref="ImGuiObject::UpdateWindow(const TimerParams&)"/>
    virtual void Update(const TimerParams& timerParams) override;

#pragma endregion

};

#endif //AE_EDITOR_MODE