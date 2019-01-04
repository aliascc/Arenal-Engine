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
#include "ImGuiObject.h"

/*****************
*   Class Decl   *
******************/
class ImGuiMenuObject : public ImGuiObject
{

protected:

    /**************************
    *   Protected Variables   *
    ***************************/
#pragma region Protected Variables

#pragma endregion

public:

    /***************************************
    *   Constructor & Destructor Methods   *
    ****************************************/
#pragma region Constructor & Destructor Methods

    /// <summary>
    /// ImGuiMenuObject Constructor
    /// </summary>
    /// <param name="name">Name of the menu</param>
    /// <param name="renderPriority">Render Priority of the menu</param>
    /// <param name="visible">Determines if the menu is visible or not</param>
    ImGuiMenuObject(const std::string& name, uint32_t renderPriority, bool visible = true)
        : ImGuiObject(name, renderPriority, m_Visible)
    {
    }

    /// <summary>
    /// Default ImGuiMenuObject Destructor
    /// </summary>
    virtual ~ImGuiMenuObject()
    {
    }

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

#pragma endregion

};

#endif //AE_EDITOR_MODE