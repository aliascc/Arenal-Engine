/********************************************************
*
* Author: Carlos Chacón N.
*
* Copyright 2012-2015
*
*********************************************************/

/**********************
*   System Includes   *
***********************/

/*************************
*   3rd Party Includes   *
**************************/
#include "SFML\Audio.hpp"

/***************************
*   Game Engine Includes   *
****************************/
#include "AudioListener.h"
#include "Base\BaseFunctions.h"

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
