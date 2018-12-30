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
#include "precomp_gamecomponents.h"

/**********************
*   System Includes   *
***********************/

/***************************
*   Game Engine Includes   *
****************************/
#include "GameApp\GameApp.h"
#include "GameAnimationsUpdate.h"
#include "GameObject\GameObject.h"
#include "GameObject\GameObjectManager.h"
#include "Models\Animation\AnimationPlayer.h"
#include "GameObject\Components\MeshAnimationGOC.h"

//Always include last
#include "Memory\MemLeaks.h"

/********************
*   Function Defs   *
*********************/
GameAnimationsUpdate::GameAnimationsUpdate(GameApp& gameApp, GameResourceManager& gameResourceManager, const std::string& gameComponentName, uint32_t callOrder)
    : GameComponent(gameApp, gameResourceManager, gameComponentName, callOrder)
{
}

GameAnimationsUpdate::~GameAnimationsUpdate()
{
}

void GameAnimationsUpdate::Update(const TimerParams& timerParams)
{
    ///////////////////////////////////////////
    //Get Game Object Manager
    GameObjectManager* gameObjectManager = m_GameApp.GetGameObjectManager();

    AEAssert(gameObjectManager != nullptr);
    if (gameObjectManager == nullptr)
    {
        return;
    }

    ///////////////////////////////////////////
    //Update all Animation Objects information
    for (auto goIt : *gameObjectManager)
    {
        UpdateGameAnimationObjects(goIt.second, timerParams);
    }

    GameComponent::Update(timerParams);
}

void GameAnimationsUpdate::UpdateGameAnimationObjects(GameObject* gameObject, const TimerParams& timerParams)
{
    AEAssert(gameObject != nullptr);
    if (gameObject == nullptr)
    {
        return;
    }

    if (gameObject->HasMeshAnimationGOC())
    {
        MeshAnimationGOC* meshAnimGOC = gameObject->GetMeshAnimationGOC();

        AETODO("Check if we need to pass transform");
        meshAnimGOC->GetAnimationPlayer()->Update(timerParams);// , gameObject->GetWorldTransform());
    }

    for (auto goIt : *gameObject)
    {
        UpdateGameAnimationObjects(goIt.second, timerParams);
    }
}
