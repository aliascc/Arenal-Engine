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
#include <assert.h>

/***************************
*   Game Engine Includes   *
****************************/
#include "DeferredLightManager.h"
#include "GameApp\GameApp.h"
#include "Utils\AETimeDefs.h"
#include "Lights\Light.h"

//Always include last
#include "Memory\MemLeaks.h"

/********************
*   Function Defs   *
*********************/
DeferredLightManager::DeferredLightManager(GameApp* gameApp, const std::string& serviceName, const std::string& name, uint32_t callOrder)
	: GameComponent(gameApp, name, callOrder)
	, m_ServiceName(serviceName)
{
	m_GameApp->RegisterGameService(m_ServiceName, this);

	LightCollection::LightsMapItConst it = m_LightCollection.Begin();
}

DeferredLightManager::~DeferredLightManager()
{
	UnLoadContent();
}

void DeferredLightManager::Update(const TimerParams& timerParams)
{
	GameComponent::Update(timerParams);
}

void DeferredLightManager::Initialize()
{
	GameComponent::Initialize();
}

void DeferredLightManager::LoadContent()
{
	GameComponent::LoadContent();
}

void DeferredLightManager::UnLoadContent()
{
	GameComponent::UnLoadContent();
}

void DeferredLightManager::OnLostDevice()
{
	GameComponent::OnLostDevice();
}

void DeferredLightManager::OnResetDevice()
{
	GameComponent::OnResetDevice();
}
