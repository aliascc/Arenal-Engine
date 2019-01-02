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
class GraphicDevice;

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

    /// <summary>
    /// Graphic Device of the Game App
    /// </summary>
    GraphicDevice& m_GraphicDevice;

    std::vector<ImGuiObject*> m_ImGuiObjects;

    std::unordered_map<uint64_t, size_t> m_ImGuiObjectIndex;

#pragma endregion

    /**********************
    *   Private Methods   *
    ***********************/
#pragma region Private Methods

    /// <summary>
    /// Get the ImGui Object Index
    /// </summary>
    /// <param name="windowID">ImGui Object Unique ID</param>
    /// <param name="windowIndex">ImGui Object Index in the vector</param>
    /// <returns>OK if it was added, otherwise an error code</returns>
    inline AEResult GetImGuiObjectIndex(const uint64_t imGuiObjectID, size_t& imGuiObjectIndex)
    {
        auto it = m_ImGuiObjectIndex.find(imGuiObjectID);
        if (it == m_ImGuiObjectIndex.end())
        {
            return AEResult::NotFound;
        }

        imGuiObjectIndex = it->second;

        return AEResult::Ok;
    }

    /// <summary>
    /// Clean up ImGuiManager resources
    /// </summary>
    void CleanUp();

#pragma endregion

public:

    /***************************************
    *   Constructor & Destructor Methods   *
    ****************************************/
#pragma region Constructor & Destructor Methods

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

#pragma endregion

    /************************
    *   Framework Methods   *
    *************************/
#pragma region Framework Methods

    /// <summary>
    /// Adds a ImGui Object to the manager
    /// </summary>
    /// <param name="window">ImGui Object to process</param>
    /// <returns>OK if it was added, otherwise an error code</returns>
    AEResult AddImGuiObject(ImGuiObject* imGuiObject);

    /// <summary>
    /// Removes a ImGui Object from the Manager
    /// </summary>
    /// <param name="imGuiObjectID">Unique ID of the ImGui Object to remove</param>
    /// <param name="freeMemory">Deletes the ImGui Object from the memory</param>
    /// <returns>OK if the ImGui Object was removed from the manager, otherwise error code</returns>
    AEResult RemoveImGuiObject(const uint64_t imGuiObjectID, bool freeMemory = false);

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

#pragma endregion

};

#endif // AE_EDITOR_MODE