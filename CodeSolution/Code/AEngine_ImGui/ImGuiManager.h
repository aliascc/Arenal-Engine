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

#ifndef _IM_GUI_MANAGER_H
#define _IM_GUI_MANAGER_H

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
*   Using   *
*************/

/********************
*   Forward Decls   *
*********************/
class ImGuiWindow;

/*****************
*   Class Decl   *
******************/
class ImGuiManager sealed : public AEObject
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
    /// ImGuiManager Constructor
    /// </summary>
    ImGuiManager();

    /// <summary>
    /// Default ImGuiManager Destructor
    /// </summary>
    virtual ~ImGuiManager();

#pragma endregion

    /************************
    *   Framework Methods   *
    *************************/
#pragma region Framework Methods

    /// <summary>
    /// Adds a Window to the manager
    /// </summary>
    /// <param name="window">ImGui Window to process</param>
    /// <returns>OK if it was added, otherwise an error code</returns>
    AEResult AddWindow(ImGuiWindow* window);

    /// <summary>
    /// Removes a Window from the Manager
    /// </summary>
    /// <param name="windowID">Unique ID of the WIndow to remove</param>
    /// <returns>OK if the window was removed from the manager, otherwise error code</returns>
    AEResult RemoveWindow(uint64_t windowID);

#pragma endregion

};

#endif