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
#include "GameComponent.h"
#include "Logger\Logger.h"
#include "Base\BaseFunctions.h"
#include "GameServiceCollection.h"
#include "Localization\LocalizationManager.h"
#include "Localization\LocalizationManagerDefs.h"

//Always include last
#include "Memory\MemLeaks.h"

/*********************
 *   Function Defs   *
 *********************/
XETODO("Add log messages for errors");
GameServiceCollection::GameServiceCollection()
{
}

GameServiceCollection::~GameServiceCollection()
{
}

GameService* GameServiceCollection::GetGameService(const std::wstring& name)
{
	if(!Exists(name))
	{
		return nullptr;
	}

	return m_GameServiceMap[name];
}

bool GameServiceCollection::Exists(const std::wstring& name) const
{
	auto it = m_GameServiceMap.find(name);

	if(it != m_GameServiceMap.end())
	{
		return true;
	}

	return false;
}

XEResult GameServiceCollection::Add(const std::wstring& name, GameService* gameService)
{
	XEAssert(gameService != nullptr);
	if(Exists(name))
	{
		return XEResult::ObjExists;
	}

	m_GameServiceMap[name] = gameService;

	return XEResult::Ok;
}

XEResult GameServiceCollection::Remove(const std::wstring& name)
{
	if(!Exists(name))
	{
		return XEResult::NotFound;
	}

	auto it = m_GameServiceMap.find(name);

	m_GameServiceMap.erase(name);

	return XEResult::Ok;
}
