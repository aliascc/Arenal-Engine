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
#include "Base\BaseFunctions.h"
#include "GameObjectComponent.h"

//Always include last
#include "Memory\MemLeaks.h"

/********************
*   Function Defs   *
*********************/

XETODO("Add mutex");
GameObjectComponent::GameObjectComponent(GameObject* gameObject, GameObjectComponentType gameObjectComponentType)
	: m_GameObject(gameObject)
	, m_GameObjectComponentType(gameObjectComponentType)
{
	XEAssert(gameObject != nullptr);
	if (gameObject == nullptr)
	{
		return;
	}

	m_IsReady = true;
}

GameObjectComponent::~GameObjectComponent()
{
}
