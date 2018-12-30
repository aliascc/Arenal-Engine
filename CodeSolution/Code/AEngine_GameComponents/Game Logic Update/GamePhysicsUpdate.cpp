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
#include "AudioListener.h"
#include "PhysicsManager.h"
#include "GameApp\GameApp.h"
#include "GamePhysicsUpdate.h"
#include "GamePhysicsUpdate.h"
#include "GameObject\GameObject.h"
#include "GameObject\GameObjectManager.h"
#include "GameObject\Components\AudioSourceGOC.h"
#include "GameObject\Components\AudioListenerGOC.h"

//Always include last
#include "Memory\MemLeaks.h"

/********************
*   Function Defs   *
*********************/
GamePhysicsUpdate::GamePhysicsUpdate(GameApp& gameApp, GameResourceManager& gameResourceManager, const std::string& gameComponentName, uint32_t callOrder)
    : GameComponent(gameApp, gameResourceManager, gameComponentName, callOrder)
{
}

GamePhysicsUpdate::~GamePhysicsUpdate()
{
}

void GamePhysicsUpdate::Update(const TimerParams& timerParams)
{
    if (m_GameApp.GetGameEditorPlayState() == GameEditorPlayState::Playing)
    {
        ///////////////////////////////////////////
        //Get Physics Manager
        PhysicsManager* physicsManager = m_GameApp.GetPhysicsManager();

        ///////////////////////////////////////////
        //Update Physics Manager
        physicsManager->Update(timerParams);
    }

    GameComponent::Update(timerParams);
}
