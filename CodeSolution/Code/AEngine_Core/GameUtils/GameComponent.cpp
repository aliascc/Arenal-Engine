/*
* Copyright (c) 2018 <Carlos Chac�n>
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
#include "GameComponent.h"
#include "GameApp\GameApp.h"
#include "Resource\GameResourceManager.h"
#include "Core Game Command/CoreCommands.h"

//Always include last
#include "Memory\MemLeaks.h"
/********************
*   Function Defs   *
*********************/
AETODO("add mutex");
GameComponent::GameComponent(GameApp& gameApp, const std::string& name, uint32_t callOrder)
    : GameService(name)
    , m_GameApp(gameApp)
    , m_GameResourceManager(m_GameApp.GetGameResourceManager())
    , m_CallOrder(callOrder)
{
}

GameComponent::~GameComponent()
{
    UnLoadContent();
}

void GameComponent::SetCallOrder(uint32_t callOrder)
{
    if(m_CallOrder == callOrder)
    {
        return;
    }

    m_CallOrder = callOrder;

    GCSortCommand* sortCommand = new GCSortCommand(m_GameApp.GetGameComponentCollection());
    GameCommandManager::GetInstance().AddCommand(sortCommand);
}

void GameComponent::ConstantUpdate(const TimerParams& timerParams)
{
}

void GameComponent::PostUpdate(const TimerParams& timerParams)
{
}

void GameComponent::Initialize()
{
}

void GameComponent::LoadContent()
{
}

void GameComponent::UnLoadContent()
{
}

void GameComponent::OnLostDevice()
{
}

void GameComponent::OnResetDevice()
{
}
