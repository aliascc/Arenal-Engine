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

#ifndef _GAME_COMMAND_MANAGER_H
#define _GAME_COMMAND_MANAGER_H

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
class GameCommandManager sealed : public Singleton<GameCommandManager>
{
    friend class Singleton<GameCommandManager>;

    typedef std::list<GameCommand*> GameCommandList;

private:

    /************************
    *   Private Variables   *
    *************************/
#pragma region Private Variables

    /// <summary>
    /// Game Command List to execute
    /// </summary>
    GameCommandList m_GameCommandList;

#pragma endregion

    /**********************
    *   Private Methods   *
    ***********************/
#pragma region Private Methods

#pragma endregion

    /***********************************************
    *   Private Constructor & Destructor Methods   *
    ************************************************/
#pragma region Constructor & Destructor Methods

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

#pragma endregion

public:

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

    /// <summary>
    /// Adds a new Game Command to execute at the end of the frame
    /// </summary>
    /// <param name="cmd">Game Command to execute</param>
    /// <returns>OK if command was added, otherwise error code</returns>
    AEResult AddCommand(GameCommand* cmd);

    /// <summary>
    /// Execute Game Commands
    /// </summary>
    void ExecuteCommands();

#pragma endregion

};

#endif