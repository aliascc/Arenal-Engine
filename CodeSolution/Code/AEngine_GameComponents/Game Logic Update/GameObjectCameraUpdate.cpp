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
#include "precomp_gamecomponents.h"

/**********************
*   System Includes   *
***********************/

/***************************
*   Game Engine Includes   *
****************************/
#include "Camera\Camera.h"
#include "GameObjectCameraUpdate.h"
#include "GameObject\Components\CameraGOC.h"

//Always include last
#include "Memory\MemLeaks.h"

/********************
*   Function Defs   *
*********************/
GameObjectCameraUpdate::GameObjectCameraUpdate(GameApp& gameApp, const std::string& gameComponentName, uint32_t callOrder)
    : GameComponent(gameApp, gameComponentName, callOrder)
    , m_GameObjectManager(gameApp.GetGameObjectManager())
{
}

GameObjectCameraUpdate::~GameObjectCameraUpdate()
{
}

void GameObjectCameraUpdate::UpdateCamera(GameObject* gameObject)
{
    AEAssert(gameObject != nullptr);
    if (gameObject == nullptr)
    {
        return;
    }

    if (gameObject->HasCameraGOC())
    {
        Camera* camera = gameObject->GetCameraGOC()->GetCamera();

        AEAssert(camera != nullptr);
        if(camera != nullptr)
        {
            camera->SetNewPositionAndRotation(gameObject->GetWorldPosition(), gameObject->GetWorldRotationAngles());
        }
    }

    for (auto goIt : *gameObject)
    {
        UpdateCamera(goIt.second);
    }
}

void GameObjectCameraUpdate::Update(const TimerParams& timerParams)
{
    ///////////////////////////////////////////
    //Call Methods for the Script
    for (auto goIt : m_GameObjectManager)
    {
        UpdateCamera(goIt.second);
    }
}
