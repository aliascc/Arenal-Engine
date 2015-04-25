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

/***************************
*   Game Engine Includes   *
****************************/
#include "AudioPlayer.h"
#include "AudioListener.h"
#include "Time\XETimeDefs.h"
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
	XEAssert(gameObject != nullptr);
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