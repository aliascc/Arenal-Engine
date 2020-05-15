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
#include "EditorReplayDefs.h"

/********************
*   Forward Decls   *
*********************/
class EditorReplayCommand;

/*****************
*   Class Decl   *
******************/

/// <summary>
/// Game Command Manager Class
/// </summary>
class EditorReplayBuffer sealed : public Singleton<EditorReplayBuffer>
{
    friend class Singleton<EditorReplayBuffer>;

    typedef std::list<EditorReplayCommand*> EditorReplayCommandList;

private:

    /// <summary>
    /// Editor Replay Command List to execute
    /// </summary>
    EditorReplayCommandList m_ToExecuteCommands;

    /// <summary>
    /// Editor Replay Command List to Redo if needed
    /// </summary>
    EditorReplayCommandList m_AvailableRedoCommands;

    /// <summary>
    /// Editor Replay Command List to Redo
    /// </summary>
    EditorReplayCommandList m_ToRedoCommands;

    /// <summary>
    /// Editor Replay Command List to Undo if needed
    /// </summary>
    EditorReplayCommandList m_AvailableUndoCommands;

    /// <summary>
    /// Editor Replay Command List to Undo
    /// </summary>
    EditorReplayCommandList m_ToUndoCommands;

    /// <summary>
    /// Max Redo Commands to buffer
    /// </summary>
    size_t m_MaxRedoCommands = AE_EDITOR_REPLAY_REDO_MAX_COMMANDS;

    /// <summary>
    /// Max Undo Commands to buffer
    /// </summary>
    size_t m_MaxUndoCommands = AE_EDITOR_REPLAY_UNDO_MAX_COMMANDS;

    /// <summary>
    /// EditorReplayBuffer Constructor
    /// </summary>
    EditorReplayBuffer();

    /// <summary>
    /// Default EditorReplayBuffer Destructor
    /// </summary>
    virtual ~EditorReplayBuffer();

    // Prevent copy-construction / assignment
    EditorReplayBuffer(const EditorReplayBuffer&) = delete;
    EditorReplayBuffer& operator=(const EditorReplayBuffer&) = delete;

public:

    /// <summary>
    /// Adds a new Editor Command to execute at the end of the frame
    /// </summary>
    /// <param name="cmd">Editor Command to execute</param>
    void AddCommand(EditorReplayCommand* cmd);

    /// <summary>
    /// Redo the last command that was undone
    /// </summary>
    void RedoLastCommand();

    /// <summary>
    /// Undo the last command executes
    /// </summary>
    void UndoLastCommand();

    /// <summary>
    /// Execute Game Commands
    /// </summary>
    void ExecuteCommands();
};