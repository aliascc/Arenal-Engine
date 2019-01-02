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
#include "precomp_base.h"

/**********************
*   System Includes   *
***********************/

/*************************
*   3rd Party Includes   *
**************************/

/***************************
*   Game Engine Includes   *
****************************/
#include "GameCommand.h"
#include "GameCommandManager.h"

//Always include last
#include "Memory\MemLeaks.h"

/********************
*   Function Defs   *
*********************/

GameCommandManager::GameCommandManager()
{
}

GameCommandManager::~GameCommandManager()
{
}

AEResult GameCommandManager::AddCommand(GameCommand* cmd)
{
    if (cmd == nullptr)
    {
        return AEResult::NullParameter;
    }

    m_GameCommandList.push_back(cmd);

    return AEResult::Ok;
}

void GameCommandManager::ExecuteCommands()
{
    if (m_GameCommandList.size() == 0)
    {
        return;
    }

    for (GameCommand* gc : m_GameCommandList)
    {
        gc->Execute();

        DeleteMem(gc);
    }

    m_GameCommandList.clear();
}
