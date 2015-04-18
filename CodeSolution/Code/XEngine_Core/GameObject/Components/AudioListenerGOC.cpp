/********************************************************
*
* Author: Carlos Chacón N.
*
* Desc:
*
*
* Copyright 2012-2014
*
*********************************************************/


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
XETODO("Add mutex");
AudioListenerGOC::AudioListenerGOC(GameObject* gameObject, AudioManager* audioManager)
	: GameObjectComponent(gameObject, GameObjectComponentType::AudioListener)
	, m_AudioManager(audioManager)
{
	XEAssert(m_AudioManager != nullptr);
}

AudioListenerGOC::~AudioListenerGOC()
{
	if (m_AudioManager != nullptr)
	{
		m_AudioManager->SetIsListenerSet(false);
	}
}