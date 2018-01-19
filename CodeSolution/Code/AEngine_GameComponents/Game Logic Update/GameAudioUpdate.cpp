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
#include "AudioPlayer.h"
#include "AudioListener.h"
#include "Time\AETimeDefs.h"
#include "GameAudioUpdate.h"
#include "GameApp\GameApp.h"
#include "GameObject\GameObject.h"
#include "GameObject\GameObjectManager.h"
#include "GameObject\Components\AudioSourceGOC.h"
#include "GameObject\Components\AudioListenerGOC.h"

//Always include last
#include "Memory\MemLeaks.h"

/********************
*   Function Defs   *
*********************/
GameAudioUpdate::GameAudioUpdate(GameApp* gameApp, const std::wstring& gameComponentName, uint32_t callOrder)
    : GameComponent(gameApp, gameComponentName, callOrder)
{
}

GameAudioUpdate::~GameAudioUpdate()
{
}

void GameAudioUpdate::Update(const TimerParams& timerParams)
{
    ///////////////////////////////////////////
    //Get Game Object Manager
    GameObjectManager* gameObjectManager = m_GameApp->GetGameObjectManager();

    ///////////////////////////////////////////
    //Update Audio Listener
    for (auto goIt : *gameObjectManager)
    {
        UpdateGameAudio(goIt.second, timerParams);
    }
    

    GameComponent::Update(timerParams);
}

void GameAudioUpdate::UpdateGameAudio(GameObject* gameObject, const TimerParams& timerParams)
{
    AEAssert(gameObject != nullptr);
    if (gameObject == nullptr)
    {
        return;
    }

    if (gameObject->HasAudioListenerGOC())
    {
        UpdateAudioListener(gameObject, AudioListener::GetInstance());
    }

    if (gameObject->HasAudioSourceGOCs())
    {
        UpdateAudioSources(gameObject);
    }

    for (auto goIt : *gameObject)
    {
        UpdateGameAudio(goIt.second, timerParams);
    }
}

void GameAudioUpdate::UpdateAudioListener(GameObject* gameObject, AudioListener* audioListener)
{
    if (gameObject == NULL || audioListener == NULL)
    {
        return;
    }

    audioListener->SetPosition(gameObject->GetWorldPosition());

    audioListener->SetDirection(gameObject->GetDirection());
}

void GameAudioUpdate::UpdateAudioSources(GameObject* gameObject)
{
    if (gameObject == NULL)
    {
        return;
    }

    const AudioSourceGOCList& audioSourceList = gameObject->GetAudioSourceGOCList();

    for (auto audioSourceGOC : audioSourceList)
    {
        AudioPlayer* audioPlayer = audioSourceGOC->GetAudioPlayer();

        audioPlayer->SetPosition(gameObject->GetWorldPosition());
    }
}
