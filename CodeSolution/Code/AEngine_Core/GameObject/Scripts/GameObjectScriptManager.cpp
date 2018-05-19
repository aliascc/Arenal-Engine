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
#include "precomp_core.h"

/**********************
*   System Includes   *
***********************/

/*************************
*   3rd Party Includes   *
**************************/

/***************************
*   Game Engine Includes   *
****************************/
#include "GameObjectScriptManager.h"

//Always include last
#include "Memory\MemLeaks.h"

AETODO("Add mutex");
/********************
*   Function Defs   *
*********************/
GameObjectScriptManager::GameObjectScriptManager()
{
}

GameObjectScriptManager::~GameObjectScriptManager()
{
    CleanUp();
}

void GameObjectScriptManager::CleanUp()
{
}

bool GameObjectScriptManager::Exists(const std::string& name)
{
    return ( m_GameObjectScriptMap.find(name) != m_GameObjectScriptMap.end() );
}

AEResult GameObjectScriptManager::Add(const std::string& name, GameObjectScriptInstance* instance)
{
    if (Exists(name))
    {
        return AEResult::ObjExists;
    }

    m_GameObjectScriptMap[name] = instance;

    return AEResult::Ok;
}

AEResult GameObjectScriptManager::Remove(const std::string& name)
{
    if (!Exists(name))
    {
        return AEResult::NotFound;
    }

    m_GameObjectScriptMap.erase(name);

    return AEResult::Ok;
}

GameObjectScriptInstance* GameObjectScriptManager::Get(const std::string& name)
{
    if (!Exists(name))
    {
        return nullptr;
    }

    return m_GameObjectScriptMap[name];
}

asIScriptObject* GameObjectScriptManager::GetScript(const std::string& name)
{
    if (!Exists(name))
    {
        return nullptr;
    }

    asIScriptObject* asObj = m_GameObjectScriptMap[name]->m_ASObj;

    asObj->AddRef();

    return asObj;
}

GameObjectScriptMapIt GameObjectScriptManager::begin()
{
    return m_GameObjectScriptMap.begin();
}

GameObjectScriptMapIt GameObjectScriptManager::end()
{
    return m_GameObjectScriptMap.end();
}

GameObjectScriptMapItConst GameObjectScriptManager::begin() const
{
    return m_GameObjectScriptMap.begin();
}

GameObjectScriptMapItConst GameObjectScriptManager::end() const
{
    return m_GameObjectScriptMap.end();
}

GameObjectScriptMapItConst GameObjectScriptManager::cbegin() const
{
    return m_GameObjectScriptMap.cbegin();
}

GameObjectScriptMapItConst GameObjectScriptManager::cend() const
{
    return m_GameObjectScriptMap.cend();
}

GameObjectScriptInstance* GameObjectScriptManager::operator[](const std::string& name)
{
    return Get(name);
}
