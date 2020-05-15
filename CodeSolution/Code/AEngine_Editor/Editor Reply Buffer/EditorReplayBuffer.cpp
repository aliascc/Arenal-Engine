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

/*************************
*   Precompiled Header   *
**************************/
#include "precomp_editor.h"

/**********************
*   System Includes   *
***********************/

/*************************
*   3rd Party Includes   *
**************************/

/***************************
*   Game Engine Includes   *
****************************/
#include "EditorReplayBuffer.h"
#include "EditorReplayCommand.h"

//Always include last
#include "Memory\MemLeaks.h"

/********************
*   Function Defs   *
*********************/

EditorReplayBuffer::EditorReplayBuffer()
{
}

EditorReplayBuffer::~EditorReplayBuffer()
{
}

void EditorReplayBuffer::AddCommand(EditorReplayCommand* cmd)
{
    if (cmd == nullptr)
    {
        return;
    }

    m_ToExecuteCommands.push_back(cmd);
    m_AvailableUndoCommands.push_back(cmd);

    if (m_AvailableUndoCommands.size() >= m_MaxUndoCommands)
    {
        EditorReplayCommand* discardCmd = m_AvailableUndoCommands.front();
        m_AvailableUndoCommands.pop_front();

        DeleteMem(discardCmd);
    }
}

void EditorReplayBuffer::RedoLastCommand()
{
    if (m_AvailableRedoCommands.empty())
    {
        return;
    }

    EditorReplayCommand* cmd = m_AvailableRedoCommands.back();
    m_AvailableRedoCommands.pop_back();

    m_ToRedoCommands.push_back(cmd);
    m_AvailableUndoCommands.push_back(cmd);

    if (m_AvailableUndoCommands.size() >= m_MaxUndoCommands)
    {
        EditorReplayCommand* discardCmd = m_AvailableUndoCommands.front();
        m_AvailableUndoCommands.pop_front();

        DeleteMem(discardCmd);
    }
}

void EditorReplayBuffer::UndoLastCommand()
{
    if (m_AvailableUndoCommands.empty())
    {
        return;
    }

    EditorReplayCommand* cmd = m_AvailableUndoCommands.back();
    m_AvailableUndoCommands.pop_back();

    m_ToUndoCommands.push_back(cmd);
    m_AvailableRedoCommands.push_back(cmd);

    if (m_AvailableRedoCommands.size() >= m_MaxRedoCommands)
    {
        EditorReplayCommand* discardCmd = m_AvailableRedoCommands.front();
        m_AvailableRedoCommands.pop_front();

        DeleteMem(discardCmd);
    }
}

void EditorReplayBuffer::ExecuteCommands()
{
    if (!m_ToExecuteCommands.empty())
    {
        for (EditorReplayCommand* cmd : m_ToExecuteCommands)
        {
            cmd->Execute();
        }

        m_ToExecuteCommands.clear();
    }

    if (!m_ToUndoCommands.empty())
    {
        for (EditorReplayCommand* cmd : m_ToUndoCommands)
        {
            cmd->Undo();
        }

        m_ToUndoCommands.clear();
    }

    if (!m_ToRedoCommands.empty())
    {
        for (EditorReplayCommand* cmd : m_ToRedoCommands)
        {
            cmd->Execute();
        }

        m_ToRedoCommands.clear();
    }
}
