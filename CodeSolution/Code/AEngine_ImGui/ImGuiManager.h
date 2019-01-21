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
*   Using   *
*************/

/********************
*   Forward Decls   *
*********************/
class ImGuiObject;
class ImGuiWindow;
class GraphicDevice;
class ImGuiMainMenu;

/*****************
*   Class Decl   *
******************/
class ImGuiManager sealed : public AEObject
{
    //Typedef for STL Game Component List 
    typedef std::vector<ImGuiWindow*>                   ImGuiWindowVector;
    typedef std::unordered_map<uint64_t, ImGuiWindow*>  ImGuiWindowMap;

private:

    /// <summary>
    /// Graphic Device of the Game App
    /// </summary>
    GraphicDevice& m_GraphicDevice;

    ImGuiMainMenu* m_ImGuiMainMenu = nullptr;

    ImGuiWindowVector m_ImGuiWindows;
    ImGuiWindowMap m_ImGuiWindowMap;

    /// <summary>
    /// ImGui Docking Flags
    /// </summary>
    ImGuiWindowFlags m_DockingFlags = ImGuiWindowFlags_None;

    /// <summary>
    /// ImGui Docking WIndow Flags
    /// </summary>
    ImGuiDockNodeFlags m_DockingWindowFlags = ImGuiDockNodeFlags_None;

    /// <summary>
    /// Declares if the Manager has been initialized and
    /// it is ready to run.
    /// </summary>
    bool m_IsReady = false;

    /// <summary>
    /// Clean up ImGuiManager resources
    /// </summary>
    void CleanUp();

    /// <summary>
    /// Update Docking Space
    /// </summary>
    void UpdateDockingSpace();

public:

    /// <summary>
    /// ImGuiManager Constructor
    /// </summary>
    /// <param name="graphicDevice">Graphic Device of the Game App</param>
    ImGuiManager(GraphicDevice& graphicDevice);

    /// <summary>
    /// Default ImGuiManager Destructor
    /// </summary>
    virtual ~ImGuiManager();

    //Delete copy constructor/operator
    ImGuiManager(const ImGuiManager&) = delete;
    ImGuiManager& operator=(const ImGuiManager&) = delete;

    inline bool IsReady() const
    {
        return m_IsReady;
    }

    /// <summary>
    /// Sorts the Windows
    /// </summary>
    void SortWindows();

    /// <summary>
    /// Adds a ImGui Windowto the manager
    /// </summary>
    /// <param name="window">ImGui Window to process</param>
    /// <returns>OK if it was added, otherwise an error code</returns>
    AEResult AddImGuiWindow(ImGuiWindow* imGuiWindow);

    /// <summary>
    /// Removes a ImGui Window from the Manager
    /// </summary>
    /// <param name="imGuiObjectID">Unique ID of the ImGui Window to remove</param>
    /// <returns>OK if the ImGui Window was removed from the manager, otherwise error code</returns>
    AEResult RemoveImGuiWindow(const uint64_t imGuiWindowID);

    /// <summary>
    /// Initializes Im Gui
    /// </summary>
    /// <returns>OK if Initializing of Im Gui was successful, otherwise error code</returns>
    AEResult Initialize();

    /// <summary>
    /// Run Update in all the windows
    /// </summary>
    /// <param name="timerParams">Game Timer Parameters</param>
    void Update(const TimerParams& timerParams);

    /// <summary>
    /// Render and Updates Im Gui
    /// </summary>
    /// <param name="timerParams">Game Timer Parameters</param>
    void Render(const TimerParams& timerParams);

    /// <summary>
    /// When the Graphic Device is going to a reset mode
    /// </summary>
    void OnLostDevice();

    /// <summary>
    /// When the Graphic Device is coming out of a reset mode
    /// </summary>
    void OnResetDevice();
};

#endif //AE_EDITOR_MODE