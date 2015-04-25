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
#include "GameApp\GameApp.h"
#include "Time\XETimeDefs.h"
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
	XEAssert(gameObject != nullptr);
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
				XEAssert(false);
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

	XEAssert(gameObjectManager != nullptr);
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

	XEAssert(gameObjectManager != nullptr);
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

	XEAssert(gameObjectManager != nullptr);
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

	XEAssert(gameObjectManager != nullptr);
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

