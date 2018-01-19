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

/***************************
*   Game Engine Includes   *
****************************/
#include "GameApp\GameApp.h"
#include "Time\AETimeDefs.h"
#include "GameObject\GameObject.h"
#include "GameObjectScriptCaller.h"
#include "GameObject\GameObjectManager.h"

//Always include last
#include "Memory\MemLeaks.h"

/********************
*   Function Defs   *
*********************/
GameObjectScriptCaller::GameObjectScriptCaller(GameApp* gameApp, const std::wstring& gameComponentName, uint32_t callOrder)
    : GameComponent(gameApp, gameComponentName, callOrder)
{
}

GameObjectScriptCaller::~GameObjectScriptCaller()
{
}

void GameObjectScriptCaller::ScriptCaller(GameObject* gameObject, CallMethod callMethod, const TimerParams& timerParams)
{
    AEAssert(gameObject != nullptr);
    if (gameObject == nullptr)
    {
        return;
    }

    if (gameObject->HasGameObjectScriptGOCs())
    {
        switch (callMethod)
        {
            case GameObjectScriptCaller::CallMethod::Initialize:
                gameObject->Initialize();
                break;

            case GameObjectScriptCaller::CallMethod::ConstantUpdate:
                gameObject->ConstantUpdate(timerParams);
                break;

            case GameObjectScriptCaller::CallMethod::Update:
                gameObject->Update(timerParams);
                break;

            case GameObjectScriptCaller::CallMethod::PostUpdate:
                gameObject->PostUpdate(timerParams);
                break;

            default:
                AEAssert(false);
                return;
        }
    }

    for (auto goIt : *gameObject)
    {
        ScriptCaller(goIt.second, callMethod, timerParams);
    }
}

void GameObjectScriptCaller::Initialize()
{
    ///////////////////////////////////////////
    //Get Game Object Manager
    GameObjectManager* gameObjectManager = m_GameApp->GetGameObjectManager();

    AEAssert(gameObjectManager != nullptr);
    if (gameObjectManager == nullptr)
    {
        return;
    }

    ///////////////////////////////////////////
    //Call Methods for the Script
    for (auto goIt : *gameObjectManager)
    {
        ScriptCaller(goIt.second, CallMethod::Initialize);
    }

    GameComponent::Initialize();
}

void GameObjectScriptCaller::ConstantUpdate(const TimerParams& timerParams)
{
    ///////////////////////////////////////////
    //Get Game Object Manager
    GameObjectManager* gameObjectManager = m_GameApp->GetGameObjectManager();

    AEAssert(gameObjectManager != nullptr);
    if (gameObjectManager == nullptr)
    {
        return;
    }

    ///////////////////////////////////////////
    //Call Methods for the Script
    for (auto goIt : *gameObjectManager)
    {
        ScriptCaller(goIt.second, CallMethod::ConstantUpdate, timerParams);
    }

    GameComponent::ConstantUpdate(timerParams);
}

void GameObjectScriptCaller::Update(const TimerParams& timerParams)
{
    ///////////////////////////////////////////
    //Get Game Object Manager
    GameObjectManager* gameObjectManager = m_GameApp->GetGameObjectManager();

    AEAssert(gameObjectManager != nullptr);
    if (gameObjectManager == nullptr)
    {
        return;
    }

    ///////////////////////////////////////////
    //Call Methods for the Script
    for (auto goIt : *gameObjectManager)
    {
        ScriptCaller(goIt.second, CallMethod::Update, timerParams);
    }

    GameComponent::Update(timerParams);
}

void GameObjectScriptCaller::PostUpdate(const TimerParams& timerParams)
{
    ///////////////////////////////////////////
    //Get Game Object Manager
    GameObjectManager* gameObjectManager = m_GameApp->GetGameObjectManager();

    AEAssert(gameObjectManager != nullptr);
    if (gameObjectManager == nullptr)
    {
        return;
    }

    ///////////////////////////////////////////
    //Call Methods for the Script
    for (auto goIt : *gameObjectManager)
    {
        ScriptCaller(goIt.second, CallMethod::PostUpdate, timerParams);
    }

    GameComponent::PostUpdate(timerParams);
}
