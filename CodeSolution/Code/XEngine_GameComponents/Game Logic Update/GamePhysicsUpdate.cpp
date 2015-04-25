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
#include "AudioListener.h"
#include "PhysicsManager.h"
#include "Time\XETimeDefs.h"
#include "GameApp\GameApp.h"
#include "GamePhysicsUpdate.h"
#include "GamePhysicsUpdate.h"
#include "GameObject\GameObject.h"
#include "GameObject\GameObjectManager.h"
#include "GameObject\Components\AudioSourceGOC.h"
#include "GameObject\Components\AudioListenerGOC.h"

//Always include last
#include "Memory\MemLeaks.h"

/********************
*   Function Defs   *
*********************/
GamePhysicsUpdate::GamePhysicsUpdate(GameApp* gameApp, const std::wstring& gameComponentName, uint32_t callOrder)
	: GameComponent(gameApp, gameComponentName, callOrder)
{
}

GamePhysicsUpdate::~GamePhysicsUpdate()
{
}

void GamePhysicsUpdate::Update(const TimerParams& timerParams)
{
	if (m_GameApp->GetGameEditorPlayState() == GameEditorPlayState::Playing)
	{
		///////////////////////////////////////////
		//Get Physics Manager
		PhysicsManager* physicsManager = m_GameApp->GetPhysicsManager();

		///////////////////////////////////////////
		//Update Physics Manager
		physicsManager->Update(timerParams);
	}

	GameComponent::Update(timerParams);
}
