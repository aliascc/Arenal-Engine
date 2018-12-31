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
#include "GameAnimationsUpdate.h"
#include "Models\Animation\AnimationPlayer.h"
#include "GameObject\Components\MeshAnimationGOC.h"

//Always include last
#include "Memory\MemLeaks.h"

/********************
*   Function Defs   *
*********************/
GameAnimationsUpdate::GameAnimationsUpdate(GameApp& gameApp, const std::string& gameComponentName, uint32_t callOrder)
    : GameComponent(gameApp, gameComponentName, callOrder)
    , m_GameObjectManager(gameApp.GetGameObjectManager())
{
}

GameAnimationsUpdate::~GameAnimationsUpdate()
{
}

void GameAnimationsUpdate::Update(const TimerParams& timerParams)
{
    ///////////////////////////////////////////
    //Update all Animation Objects information
    for (auto goIt : m_GameObjectManager)
    {
        UpdateGameAnimationObjects(goIt.second, timerParams);
    }
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
