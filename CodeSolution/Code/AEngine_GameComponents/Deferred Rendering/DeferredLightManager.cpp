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
#include <assert.h>

/***************************
*   Game Engine Includes   *
****************************/
#include "DeferredLightManager.h"
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
}

void DeferredLightManager::Initialize()
{
}

void DeferredLightManager::LoadContent()
{
}

void DeferredLightManager::UnLoadContent()
{
}

void DeferredLightManager::OnLostDevice()
{
}

void DeferredLightManager::OnResetDevice()
{
}
