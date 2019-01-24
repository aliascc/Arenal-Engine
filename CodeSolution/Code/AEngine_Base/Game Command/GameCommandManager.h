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

/**********************
*   System Includes   *
***********************/

/*************************
*   3rd Party Includes   *
**************************/

/***************************
*   Game Engine Includes   *
****************************/
#include "Utils/Singleton.h"

/********************
*   Forward Decls   *
*********************/
class GameCommand;

/*****************
*   Class Decl   *
******************/

/// <summary>
/// Game Command Manager Class
/// </summary>
class GameCommandManager sealed : public Singleton<GameCommandManager>
{
    friend class Singleton<GameCommandManager>;

    typedef std::list<GameCommand*> GameCommandList;

private:

    /// <summary>
    /// Game Command List to execute
    /// </summary>
    GameCommandList m_GameCommandList;

    /// <summary>
    /// GameCommandManager Constructor
    /// </summary>
    GameCommandManager();

    /// <summary>
    /// Default GameCommandManager Destructor
    /// </summary>
    virtual ~GameCommandManager();

    // Prevent copy-construction / assignment
    GameCommandManager(const GameCommandManager&) = delete;
    GameCommandManager& operator=(const GameCommandManager&) = delete;

public:

    /// <summary>
    /// Adds a new Game Command to execute at the end of the frame
    /// </summary>
    /// <param name="cmd">Game Command to execute</param>
    void AddCommand(GameCommand* cmd);

    /// <summary>
    /// Execute Game Commands
    /// </summary>
    void ExecuteCommands();

};