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
class ImGuiMenuItem : public ImGuiMenuObject
{
    typedef std::unordered_map<uint64_t, MenuItemClicked> MenuItemClickedMap;

private:

    /// <summary>
    /// Callback Map for when the Menu Item is clicked
    /// </summary>
    MenuItemClickedMap m_MenuItemClickedMap;

public:

    /// <summary>
    /// ImGuiMenuItem Constructor
    /// </summary>
    /// <param name="imGuiManager">ImGuiManager Instance</param>
    /// <param name="name">Name of the Menu Item</param>
    /// <param name="menuNameLiteral">Literal for the Menu Item Name</param>
    /// <param name="renderPriority">Render Priority for the menu item</param>
    /// <param name="visible">Determines if the menu item is visible or not</param>
    ImGuiMenuItem(ImGuiManager& imGuiManager, const std::string& name, const std::string& menuNameLiteral, uint32_t renderPriority, bool visible = true);

    /// <summary>
    /// Default ImGuiMenuItem Destructor
    /// </summary>
    virtual ~ImGuiMenuItem();

    /// <see cref="ImGuiObject::Update(const TimerParams&)"/>
    void Update(const TimerParams& timerParams) override;

    /// <summary>
    /// Registers a method to be callback when the Menu Item is selcted
    /// </summary>
    /// <param name="id">ID of the class Registering this callback</param>
    /// <param name="callback">Function to be called when the Menu item is selcted</param>
    void RegisterOnClickCallback(uint64_t id, const MenuItemClicked& callback);
    
    /// <summary>
    /// Unregisters a method to be callback when the Menu Item is selcted
    /// </summary>
    /// <param name="id">ID of the class that registered the callback</param>
    void UnRegisterOnClickCallback(uint64_t id);
};

#endif //AE_EDITOR_MODE