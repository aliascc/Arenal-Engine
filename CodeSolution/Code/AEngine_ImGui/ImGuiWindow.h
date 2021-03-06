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
class ImGuiWindow : public ImGuiObject
{
private:

    /// <summary>
    /// Executes the steps needed before the calls
    /// to Im Gui
    /// </summary>
    /// <returns>True if the window can be drawn, false otherwise</returns>
    bool PreUpdate();

protected:

    /// <summary>
    /// Determines if the Window is visible or not
    /// </summary>
    bool m_Visible = false;

    /// <summary>
    /// Flags describing the behaviour of the Im Gui Window
    /// </summary>
    ImGuiWindowFlags m_Flags = ImGuiWindowFlags_None;

    /// <summary>
    /// Determines if the Close Icon will be shown
    /// </summary>
    bool m_ShowCloseIcon = true;

    /// <summary>
    /// Determines if the Window has a menu bar enabled
    /// </summary>
    bool m_EnableMenu = false;

    /// <summary>
    /// Updates the window with the new information for the
    /// Im GUI render to display
    /// </summary>
    /// <param name="timerParams">Game Timer Parameters</param>
    virtual void UpdateWindow(const TimerParams& timerParams) = 0;

public:

    /// <summary>
    /// ImGUIObject Constructor
    /// </summary>
    /// <param name="imGuiManager">ImGuiManager Instance</param>
    /// <param name="name">Name of the Window</param>
    /// <param name="literalName">Literal Name of the Window</param>
    /// <param name="renderPriority">Render Priority of the Window</param>
    /// <param name="visible">Determines if the window is visible or not</param>
    /// <param name="flags">Im Gui Window Flags</param>
    /// <param name="showCloseIcon">Determines if the Close Icon will be shown</param>
    ImGuiWindow(ImGuiManager& imGuiManager, const std::string& name, const std::string& literalName, uint32_t renderPriority, bool visible = true, ImGuiWindowFlags flags = ImGuiWindowFlags_None, bool showCloseIcon = true);

    /// <summary>
    /// Default ImGUIObject Destructor
    /// </summary>
    virtual ~ImGuiWindow();

    /// <summary>
    /// Gets if the Window is visible
    /// </summary>
    /// <returns>True if window is visible, false if it is not</returns>
    inline bool IsVisible() const
    {
        return m_Visible;
    }

    /// <summary>
    /// Gets the Flag of the Im Gui Window
    /// </summary>
    /// <returns>Im Gui Window Flags</returns>
    inline ImGuiWindowFlags GetFlags() const
    {
        return m_Flags;
    }

    /// <summary>
    /// Gets if the Window has its menu enabled
    /// </summary>
    /// <returns>True if window has its menu enabled, false if it is not</returns>
    inline bool GetEnableMenu() const
    {
        return m_EnableMenu;
    }

    /// <summary>
    /// Sets if the Window is visible or not
    /// </summary>
    /// <param name="value">Visible Value of the Window</param>
    inline void SetIsVisible(bool value)
    {
        m_Visible = value;
    }

    /// <summary>
    /// Sets the Im Gui Window Flags
    /// </summary>
    /// <param name="value">Im Gui Window Flags</param>
    inline void SetFlags(ImGuiWindowFlags value)
    {
        m_Flags = value;
    }

    /// <summary>
    /// Sets if the Window has its menu enabled
    /// </summary>
    /// <param name="value">Enables or disables the menu</param>
    inline void SetEnableMenu(bool value)
    {
        m_EnableMenu = value;
    }

    /// <summary>
    /// Call to Update the Im Gui Window
    /// </summary>
    /// <param name="timerParams">Game Timer Parameters</param>
    void Update(const TimerParams& timerParams) override;

};

#endif //AE_EDITOR_MODE