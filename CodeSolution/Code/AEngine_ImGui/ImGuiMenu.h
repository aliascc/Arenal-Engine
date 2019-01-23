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

/// <summary>
/// ImGui Menu Container
/// </summary>
class ImGuiMenu : public ImGuiMenuObject
{
    typedef std::unordered_map<std::string, ImGuiMenuObject*> ImGuiMenuObjectMap;
    typedef std::vector<ImGuiMenuObject*> ImGuiMenuObjectVector;

protected:

    /// <summary>
    /// Map for the Menu Objects
    /// </summary>
    ImGuiMenuObjectMap m_ImGuiMenuObjectMap;

    /// <summary>
    /// Vector for the Menu Objects
    /// </summary>
    ImGuiMenuObjectVector m_ImGuiMenuObjectVector;

    /// <summary>
    /// Calls the Update Methods for the Menu Objects this menu contains
    /// </summary>
    /// <param name="timerParams">Game Timer Parameters</param>
    void UpdateMethods(const TimerParams& timerParams);

public:

    /// <summary>
    /// ImGuiMenu Constructor
    /// </summary>
    /// <param name="imGuiManager">ImGuiManager Instance</param>
    /// <param name="name">Name of the Menu</param>
    /// <param name="literalName">Literal for the Menu Name</param>
    /// <param name="renderPriority">Render Priority for the menu</param>
    /// <param name="visible">Determines if the menu is visible or not</param>
    ImGuiMenu(ImGuiManager& imGuiManager, const std::string& name, const std::string& literalName, uint32_t renderPriority, bool visible = true);

    /// <summary>
    /// Default ImGuiMenu Destructor
    /// </summary>
    virtual ~ImGuiMenu();

    /// <summary>
    /// Adds a Menu object to the Menu Instance
    /// </summary>
    /// <param name="menuObject">Menu Object to add</param>
    /// <returns>AEResult::Ok if successful otherwise an error code</returns>
    AEResult AddMenuObject(ImGuiMenuObject* menuObject);

    /// <summary>
    /// Removes a Menu Object from this Menu Instance
    /// </summary>
    /// <param name="menuObjectID">ID of the Menu object to be removed</param>
    /// <returns>AEResult::Ok if successful otherwise an error code</returns>
    AEResult RemoveMenuObject(uint64_t menuObjectID);

    /// <summary>
    /// Gets a sub-menu object fromo this Menu Instance
    /// </summary>
    /// <param name="menuName">Sub-menu name</param>
    /// <param name="subMenu">Pointer to where the sub-menu will be saved to if found</param>
    /// <returns>AEResult::Ok if successful otherwise an error code</returns>
    AEResult GetSubMenu(const std::string& menuName, ImGuiMenu** subMenu);

    /// <summary>
    /// Gets a sub-menu object fromo this Menu Instance
    /// </summary>
    /// <param name="subMenuObjectID">Sub-menu Object ID</param>
    /// <param name="subMenu">Pointer to where the sub-menu will be saved to if found</param>
    /// <returns>AEResult::Ok if successful otherwise an error code</returns>
    AEResult GetSubMenu(uint64_t subMenuObjectID, ImGuiMenu** subMenu);

    /// <summary>
    /// Gets the Leaf sub menu within this Menu instance.
    /// Using "/" as separator will track down the last menu if found.
    /// e.g. "Tools/Views/Debug" will return the "Debug" sub-menu that is 
    /// found inside Views which is inside Tools.
    /// </summary>
    /// <param name="menuTree">Menu Tree Name</param>
    /// <param name="subMenu">Pointer to where the sub-menu will be saved to if found</param>
    /// <returns>AEResult::Ok if successful otherwise an error code</returns>
    AEResult GetSubMenuLeaf(const std::string& menuTree, ImGuiMenu** subMenu);

    /// <see cref="ImGuiObject::Update(const TimerParams&)"/>
    virtual void Update(const TimerParams& timerParams) override;

    /// <summary>
    /// Initializes the ImGui Menu
    /// </summary>
    /// <returns>AEResult::Ok if successful otherwise an error code</returns>
    virtual AEResult Initialize() = 0;
};

#endif //AE_EDITOR_MODE