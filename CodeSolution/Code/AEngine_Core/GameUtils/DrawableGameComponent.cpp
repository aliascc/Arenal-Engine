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

/***************************
*   Game Engine Includes   *
****************************/
#include "GraphicDevice.h"
#include "GameApp\GameApp.h"
#include "Time\AETimeDefs.h"
#include "Base\BaseFunctions.h"
#include "DrawableGameComponent.h"

//Always include last
#include "Memory\MemLeaks.h"

/********************
*   Function Defs   *
*********************/
AETODO("add mutex");
DrawableGameComponent::DrawableGameComponent(GameApp* gameApp, const std::wstring& name, uint32_t callOrder)
	: GameComponent(gameApp, name, callOrder)
{
	AEAssert(gameApp != nullptr);
	AEAssert(gameApp->GetGraphicsDevice() != nullptr);

	m_GraphicDevice = gameApp->GetGraphicsDevice();
}

DrawableGameComponent::~DrawableGameComponent()
{
	m_GraphicDevice = nullptr;
}

void DrawableGameComponent::Render(const TimerParams& timerParams)
{
}