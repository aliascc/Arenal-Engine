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
#include "Camera\Camera.h"
#include "GameApp\GameApp.h"
#include "Time\AETimeDefs.h"
#include "GameObject\GameObject.h"
#include "GameObjectCameraUpdate.h"
#include "GameObject\GameObjectManager.h"
#include "GameObject\Components\CameraGOC.h"

//Always include last
#include "Memory\MemLeaks.h"

/********************
*   Function Defs   *
*********************/
GameObjectCameraUpdate::GameObjectCameraUpdate(GameApp* gameApp, const std::wstring& gameComponentName, uint32_t callOrder)
	: GameComponent(gameApp, gameComponentName, callOrder)
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
		UpdateCamera(goIt.second);
	}

	GameComponent::Update(timerParams);
}
