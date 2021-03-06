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
#include "precomp_audio.h"

/**********************
*   System Includes   *
***********************/

/*************************
*   3rd Party Includes   *
**************************/

/***************************
*   Game Engine Includes   *
****************************/
#include "AudioListener.h"

//Always include last
#include "Memory\MemLeaks.h"

/********************
*   Function Defs   *
*********************/

AudioListener::AudioListener()
{
    sf::Listener::setDirection(0, 0, 1);
}

AudioListener::~AudioListener()
{
}

glm::vec3 AudioListener::GetPosition() const
{
    sf::Vector3f pos = sf::Listener::getPosition();
    return glm::vec3(pos.x, pos.y, pos.z);
}

float AudioListener::GetGlobalVolume() const
{
    return sf::Listener::getGlobalVolume();
}

glm::vec3 AudioListener::GetDirection() const
{
    sf::Vector3f dir = sf::Listener::getDirection();
    return glm::vec3(dir.x, dir.y, dir.z);
}

void AudioListener::SetPosition(const glm::vec3& pos)
{
    sf::Listener::setPosition(pos.x, pos.y, pos.z);
}

void AudioListener::SetGlobalVolume(float volume)
{
    sf::Listener::setGlobalVolume(volume);
}

void AudioListener::SetDirection(const glm::vec3& direction)
{
    sf::Listener::setDirection(direction.x, direction.y, direction.z);
}
