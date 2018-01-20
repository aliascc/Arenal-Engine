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

/**********************
*   System Includes   *
***********************/

/*************************
*   3rd Party Includes   *
**************************/

/***************************
*   Game Engine Includes   *
****************************/
#include "GameComponent.h"
#include "Logger\Logger.h"
#include "Base\BaseFunctions.h"
#include "GameServiceCollection.h"
#include "Localization\LocalizationManager.h"
#include "Localization\LocalizationManagerDefs.h"

//Always include last
#include "Memory\MemLeaks.h"

/*********************
 *   Function Defs   *
 *********************/
AETODO("Add log messages for errors");
GameServiceCollection::GameServiceCollection()
{
}

GameServiceCollection::~GameServiceCollection()
{
}

GameService* GameServiceCollection::GetGameService(const std::wstring& name)
{
    if(!Exists(name))
    {
        return nullptr;
    }

    return m_GameServiceMap[name];
}

bool GameServiceCollection::Exists(const std::wstring& name) const
{
    auto it = m_GameServiceMap.find(name);

    if(it != m_GameServiceMap.end())
    {
        return true;
    }

    return false;
}

AEResult GameServiceCollection::Add(const std::wstring& name, GameService* gameService)
{
    AEAssert(gameService != nullptr);
    if(Exists(name))
    {
        return AEResult::ObjExists;
    }

    m_GameServiceMap[name] = gameService;

    return AEResult::Ok;
}

AEResult GameServiceCollection::Remove(const std::wstring& name)
{
    if(!Exists(name))
    {
        return AEResult::NotFound;
    }

    auto it = m_GameServiceMap.find(name);

    m_GameServiceMap.erase(name);

    return AEResult::Ok;
}
