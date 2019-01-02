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
#include "Base/Named.h"

/*****************
*   Class Decl   *
******************/
class ImGuiObject : public Named
{

protected:

    /**************************
    *   Protected Variables   *
    ***************************/
#pragma region Protected Variables

    /// <summary>
    /// Determines if the Window is visible or not
    /// </summary>
    bool m_Visible = false;

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
    ImGuiObject(const std::string& name, bool visible = true)
        : Named(name)
        , m_Visible(visible)
    {
    }

    /// <summary>
    /// Default ImGUIObject Destructor
    /// </summary>
    virtual ~ImGuiObject()
    {
    }

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

#pragma endregion

    /************************
    *   Framework Methods   *
    *************************/
#pragma region Framework Methods

    /// <summary>
    /// Call to Update the Im Gui Window
    /// </summary>
    /// <param name="timerParams">Game Timer Parameters</param>
    virtual void Update(const TimerParams& timerParams) = 0;

#pragma endregion

};

#endif //AE_EDITOR_MODE