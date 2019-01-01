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

#ifndef _IM_GUI_WINDOW_H
#define _IM_GUI_WINDOW_H

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

/*****************
*   Class Decl   *
******************/
class ImGuiWindow : public AEObject
{
private:

    /**********************
    *   Private Methods   *
    ***********************/
#pragma region Private Methods

    /// <summary>
    /// Executes the steps needed before the calls
    /// to Im Gui
    /// </summary>
    /// <returns>True if the window can be drawn, false otherwise</returns>
    bool PreUpdate();

#pragma endregion


protected:

    /**************************
    *   Protected Variables   *
    ***************************/
#pragma region Protected Variables

    /// <summary>
    /// Name of the Window
    /// </summary>
    std::string m_Name = "";

    /// <summary>
    /// Determines if the Window is visible or not
    /// </summary>
    bool m_Visible = false;

    /// <summary>
    /// Flags describing the behaviour of the Im Gui Window
    /// </summary>
    ImGuiWindowFlags m_Flags = 0;

    /// <summary>
    /// Determines if the Close Icon will be shown
    /// </summary>
    bool m_ShowCloseIcon = true;

#pragma endregion

    /************************
    *   Protected Methods   *
    *************************/
#pragma region Protected Methods

    /// <summary>
    /// Updates the window with the new information for the
    /// Im GUI render to display
    /// </summary>
    /// <param name="timerParams">Game Timer Parameters</param>
    virtual void UpdateWindow(const TimerParams& timerParams) = 0;

#pragma endregion

public:

    /***************************************
    *   Constructor & Destructor Methods   *
    ****************************************/
#pragma region Constructor & Destructor Methods

    /// <summary>
    /// ImGUIObject Constructor
    /// </summary>
    /// <param name="name">Name of the Window</param>
    /// <param name="visible">Determines if the window is visible or not</param>
    /// <param name="flags">Im Gui Window Flags</param>
    /// <param name="showCloseIcon">Determines if the Close Icon will be shown</param>
    ImGuiWindow(const std::string& name, bool visible = true, ImGuiWindowFlags flags = 0, bool showCloseIcon = true);

    /// <summary>
    /// Default ImGUIObject Destructor
    /// </summary>
    virtual ~ImGuiWindow();

#pragma endregion

    /******************
    *   Get Methods   *
    *******************/
#pragma region Get Methods

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
    /// Gets the name of the Window
    /// </summary>
    /// <returns>Name of the Window</returns>
    inline const std::string& GetName() const
    {
        return m_Name;
    }

#pragma endregion

    /******************
    *   Set Methods   *
    *******************/
#pragma region Set Methods

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
    /// Sets the name of the Window
    /// </summary>
    /// <param name="value">Name of the Window</param>
    inline void SetName(const std::string& value)
    {
        m_Name = value;
    }

#pragma endregion

    /************************
    *   Framework Methods   *
    *************************/
#pragma region Framework Methods

    /// <summary>
    /// Call to Update the Im Gui Window
    /// </summary>
    /// <param name="timerParams">Game Timer Parameters</param>
    void Update(const TimerParams& timerParams);

#pragma endregion

};

#endif //AE_EDITOR_MODE

#endif