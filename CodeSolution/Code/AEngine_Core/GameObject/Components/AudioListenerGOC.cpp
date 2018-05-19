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
#include "AudioManager.h"
#include "AudioListenerGOC.h"
#include "Base\BaseFunctions.h"
#include "GameObject\GameObject.h"

//Always include last
#include "Memory\MemLeaks.h"

/********************
*   Function Defs   *
*********************/
AETODO("Add mutex");
AudioListenerGOC::AudioListenerGOC(GameObject* gameObject, AudioManager* audioManager)
    : GameObjectComponent(gameObject, GameObjectComponentType::AudioListener)
    , m_AudioManager(audioManager)
{
    AEAssert(m_AudioManager != nullptr);
}

AudioListenerGOC::~AudioListenerGOC()
{
    if (m_AudioManager != nullptr)
    {
        m_AudioManager->SetIsListenerSet(false);
    }
}
