/********************************************************
*
* Author: Carlos Chacón N.
*
* Created: 6/19/2014
*
* Desc:
*
*
* Copyright 2012
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
#include "GameObjectScriptManager.h"

//Always include last
#include "Memory\MemLeaks.h"

XETODO("Add mutex");
/********************
*   Function Defs   *
*********************/
GameObjectScriptManager::GameObjectScriptManager()
{
}

GameObjectScriptManager::~GameObjectScriptManager()
{
	CleanUp();
}

void GameObjectScriptManager::CleanUp()
{
}

bool GameObjectScriptManager::Exists(const std::wstring& name)
{
	return ( m_GameObjectScriptMap.find(name) != m_GameObjectScriptMap.end() );
}

XEResult GameObjectScriptManager::Add(const std::wstring& name, GameObjectScriptInstance* instance)
{
	if (Exists(name))
	{
		return XEResult::ObjExists;
	}

	m_GameObjectScriptMap[name] = instance;

	return XEResult::Ok;
}

XEResult GameObjectScriptManager::Remove(const std::wstring& name)
{
	if (!Exists(name))
	{
		return XEResult::NotFound;
	}

	m_GameObjectScriptMap.erase(name);

	return XEResult::Ok;
}

GameObjectScriptInstance* GameObjectScriptManager::Get(const std::wstring& name)
{
	if (!Exists(name))
	{
		return nullptr;
	}

	return m_GameObjectScriptMap[name];
}

asIScriptObject* GameObjectScriptManager::GetScript(const std::wstring& name)
{
	if (!Exists(name))
	{
		return nullptr;
	}

	asIScriptObject* asObj = m_GameObjectScriptMap[name]->m_ASObj;

	asObj->AddRef();

	return asObj;
}

GameObjectScriptMapIt GameObjectScriptManager::begin()
{
	return m_GameObjectScriptMap.begin();
}

GameObjectScriptMapIt GameObjectScriptManager::end()
{
	return m_GameObjectScriptMap.end();
}

GameObjectScriptMapItConst GameObjectScriptManager::begin() const
{
	return m_GameObjectScriptMap.begin();
}

GameObjectScriptMapItConst GameObjectScriptManager::end() const
{
	return m_GameObjectScriptMap.end();
}

GameObjectScriptMapItConst GameObjectScriptManager::cbegin() const
{
	return m_GameObjectScriptMap.cbegin();
}

GameObjectScriptMapItConst GameObjectScriptManager::cend() const
{
	return m_GameObjectScriptMap.cend();
}

GameObjectScriptInstance* GameObjectScriptManager::operator[](const std::wstring& name)
{
	return Get(name);
}
