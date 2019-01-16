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

#ifndef _RESIZE_COMMAND_H
#define _RESIZE_COMMAND_H

/**********************
*   System Includes   *
***********************/

/*************************
*   3rd Party Includes   *
**************************/

/***************************
*   Game Engine Includes   *
****************************/
#include "Game Command/GameCommand.h"

/********************
*   Forward Decls   *
*********************/
class GameApp;
class GameComponentCollection;

/*******************************
*   ResizeCommand Class Decl   *
********************************/

class ResizeCommand sealed : public GameCommand
{
private:

    /// <summary>
    /// Game App
    /// </summary>
    GameApp& m_GameApp;

    /// <summary>
    /// New size that is been requested
    /// </summary>
    glm::ivec2 m_NewSize;

public:

    /// <summary>
    /// ResizeCommand Constructor
    /// </summary>
    /// <param name="gameApp">Current Game App</param>
    /// <param name="m_NewSize">Resize requested</param>
    ResizeCommand(GameApp& gameApp, const glm::ivec2& newSize);

    /// <summary>
    /// Default ResizeCommand Destructor
    /// </summary>
    virtual ~ResizeCommand();

    /// <summary>
    /// Executes the Game Command
    /// </summary>
    void Execute() override;
};

/*******************************
*   GCSortCommand Class Decl   *
********************************/

class GCSortCommand sealed : public GameCommand
{
private:

    GameComponentCollection& m_GameComponentCollection;

public:

    /// <summary>
    /// GCSortCommand Constructor
    /// </summary>
    /// <param name="gameComponentCollection">Game Component Collection to sort</param>
    GCSortCommand(GameComponentCollection& gameComponentCollection);

    /// <summary>
    /// Default GCSortCommand Destructor
    /// </summary>
    virtual ~GCSortCommand();

    /// <summary>
    /// Executes the Game Command
    /// </summary>
    void Execute() override;
};

#endif