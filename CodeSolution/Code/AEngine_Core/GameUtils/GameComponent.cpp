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
#include "GameApp\GameApp.h"
#include "Time\AETimeDefs.h"
#include "Base\BaseFunctions.h"
#include "Resource\GameResourceManager.h"

//Always include last
#include "Memory\MemLeaks.h"
/********************
*   Function Defs   *
*********************/
AETODO("add mutex");
GameComponent::GameComponent(GameApp* gameApp, const std::wstring& name, uint32_t callOrder)
    : GameService(name)
    , m_GameApp(gameApp)
    , m_CallOrder(callOrder)
{
    AEAssert(gameApp != nullptr);

    m_GameResourceManager = gameApp->GetGameResourceManager();
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

    if(m_NeedSortChangeCallback != nullptr)
    {
        m_NeedSortChangeCallback();
    }
}

void GameComponent::ConstantUpdate(const TimerParams& timerParams)
{
}

void GameComponent::Update(const TimerParams& timerParams)
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
