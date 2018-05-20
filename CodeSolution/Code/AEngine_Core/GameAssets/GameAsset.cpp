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
#include "GameAsset.h"
#include "Base\BaseFunctions.h"
#include "Resource\GameResourceManager.h"

//Always include last
#include "Memory\MemLeaks.h"

/********************
*   Function Defs   *
*********************/
GameAsset::GameAsset(GameContentType gameContentType, const std::string& filePath, GameResourceManager* gameResourceManager)
    : m_GameContentType(gameContentType)
    , m_FilePath(filePath)
    , m_GameResourceManager(gameResourceManager)
{
    AEAssert(!filePath.empty());
    AEAssert(gameResourceManager != nullptr);

    m_OnListenerObjDeletionEventHandler = std::bind(&GameAsset::UnregisterEventHandlers, this, std::placeholders::_1);
}

GameAsset::~GameAsset()
{
    for(auto eventHandlers : m_GameAssetEventHandlersMap)
    {
        eventHandlers.second.m_OnGameAssetDeletionEvent(this);
    }
    m_GameAssetEventHandlersMap.clear();

    if(m_OnGameAssetDeletionNotifyManagerEvent != nullptr)
    {
        m_OnGameAssetDeletionNotifyManagerEvent(this->GetUniqueAssetID());
    }
}

AEResult GameAsset::RegisterEventHandlers(uint64_t id, OnGameAssetDeletionEvent onGameAssetDeletionEvent, OnGameAssetReloadEvent onGameAssetReloadEvent, OnGameAssetPreReloadEvent onGameAssetPreReloadEvent)
{
    AEAssert(onGameAssetDeletionEvent != nullptr);
    if(onGameAssetDeletionEvent == nullptr)
    {
        return AEResult::NullParameter;
    }

    std::lock_guard<std::mutex> lock(m_GameAssetMutex);

    if(m_GameAssetEventHandlersMap.find(id) != m_GameAssetEventHandlersMap.end())
    {
        return AEResult::ObjExists;
    }

    m_GameAssetEventHandlersMap[id] = GameAssetEventHandlers(id, onGameAssetDeletionEvent, onGameAssetReloadEvent, onGameAssetPreReloadEvent);

    return AEResult::Ok;
}

AEResult GameAsset::UnregisterEventHandlers(uint64_t id)
{
    std::lock_guard<std::mutex> lock(m_GameAssetMutex);

    auto evenHandlerstIt = m_GameAssetEventHandlersMap.find(id);
    if(evenHandlerstIt == m_GameAssetEventHandlersMap.end())
    {
        AEResult::NotFound;
    }
    
    m_GameAssetEventHandlersMap.erase(evenHandlerstIt);

    return AEResult::Ok;
}

AEResult GameAsset::LoadAsset()
{
    if (m_GameResourceManager == nullptr)
    {
        return AEResult::GameResourceManagerNull;
    }

    std::lock_guard<std::mutex> lock(m_GameAssetMutex);

    if (m_IsLoaded)
    {
        for (auto eventHandlers : m_GameAssetEventHandlersMap)
        {
            const GameAssetEventHandlers& gaEventHandlers = eventHandlers.second;
            if (gaEventHandlers.m_OnGameAssetPreReloadEvent != nullptr)
            {
                gaEventHandlers.m_OnGameAssetPreReloadEvent(this);
            }
        }

        m_IsLoaded = false;
    }

    AEResult ret = LoadAssetResource();
    if(ret != AEResult::Ok)
    {
        AETODO("Add log");

        return ret;
    }

    m_IsLoaded = true;

    for(auto eventHandlers : m_GameAssetEventHandlersMap)
    {
        const GameAssetEventHandlers& gaEventHandlers = eventHandlers.second;
        if(gaEventHandlers.m_OnGameAssetReloadEvent != nullptr)
        {
            gaEventHandlers.m_OnGameAssetReloadEvent(this);
        }
    }

    return AEResult::Ok;
}
