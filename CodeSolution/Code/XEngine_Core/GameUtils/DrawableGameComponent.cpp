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
#include "Time\XETimeDefs.h"
#include "Base\BaseFunctions.h"
#include "DrawableGameComponent.h"

//Always include last
#include "Memory\MemLeaks.h"

/********************
*   Function Defs   *
*********************/
XETODO("add mutex");
DrawableGameComponent::DrawableGameComponent(GameApp* gameApp, const std::wstring& name, uint32_t callOrder)
	: GameComponent(gameApp, name, callOrder)
{
	XEAssert(gameApp != nullptr);
	XEAssert(gameApp->GetGraphicsDevice() != nullptr);

	m_GraphicDevice = gameApp->GetGraphicsDevice();
}

DrawableGameComponent::~DrawableGameComponent()
{
	m_GraphicDevice = nullptr;
}

void DrawableGameComponent::Render(const TimerParams& timerParams)
{
}