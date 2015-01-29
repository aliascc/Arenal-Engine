/********************************************************
*
* Author: Carlos Chacón N.
*
* Desc:
*
*
* Copyright 2012-2014
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
#include "GameObject.h"
#include "GameObjectManager.h"

//Always include last
#include "Memory\MemLeaks.h"

/********************
*   Function Defs   *
*********************/
XETODO("Check for Mutex");
GameObjectManager::GameObjectManager()
{
}

GameObjectManager::~GameObjectManager()
{
	CleanUp();
}

void GameObjectManager::CleanUp()
{
	for(auto gameObjectIt : m_GameObjectMap)
	{
		DeleteMem(gameObjectIt.second);
	}

	m_GameObjectMap.clear();
}

void GameObjectManager::GameObjectMoveToChild(uint64_t id)
{
	if(!GameObjectExistsTopLevel(id))
	{
		XEAssert(false);
		XETODO("add log");
		return;
	}

	//////////////////////////////////////
	//Remove from Map to become a Child
	m_GameObjectMap.erase(id);

	XETODO("add debug log here");
}

bool GameObjectManager::GameObjectExistsTopLevel(uint64_t goID) const
{
	return (m_GameObjectMap.find(goID) != m_GameObjectMap.end());
}

GameObject* GameObjectManager::SearchGameObjectChilds(uint64_t goID, GameObject* gameObject) const
{
	XEAssert(gameObject != nullptr);
	if(gameObject == nullptr)
	{
		return nullptr;
	}

	if(gameObject->GetUniqueID() == goID)
	{
		return gameObject;
	}

	for(auto goIt : *gameObject)
	{
		GameObject* found = SearchGameObjectChilds(goID, goIt.second);

		if(found != nullptr)
		{
			return found;
		}
	}

	return nullptr;
}

GameObject* GameObjectManager::GetGameObject(uint64_t goID) const
{
	if(GameObjectExistsTopLevel(goID))
	{
		auto foundIt = m_GameObjectMap.find(goID);
		return foundIt->second;
	}

	for(auto goIt : m_GameObjectMap)
	{
		GameObject* found = SearchGameObjectChilds(goID, goIt.second);

		if(found != nullptr)
		{
			return found;
		}
	}

	return nullptr;
}

XEResult GameObjectManager::AddGameObject(GameObject* gameObject)
{
	XEAssert(gameObject != nullptr);

	if(gameObject == nullptr)
	{
		return XEResult::NullParameter;
	}

	if(GameObjectExistsTopLevel(gameObject->GetUniqueID()))
	{
		return XEResult::ObjExists;
	}

	if(gameObject->m_ManagerID != 0 && gameObject->GetManagerID() != this->GetUniqueID())
	{
		XETODO("better return and error message");
		return XEResult::Fail;
	}

	if (gameObject->m_ManagerID == 0)
	{
		gameObject->m_GameObjectMoveToChildCallback = std::bind(&GameObjectManager::GameObjectMoveToChild, this, std::placeholders::_1);

		gameObject->m_ManagerID = this->GetUniqueID();
	}

	if(gameObject->IsChild())
	{
		gameObject->GetParentGameObject()->RemoveChild(gameObject->GetUniqueID());
	}

	m_GameObjectMap[gameObject->GetUniqueID()] = gameObject;

	return XEResult::Ok;
}

XEResult GameObjectManager::SetSelectedGameObject(GameObject* gameObject)
{
	if (gameObject->m_ManagerID != 0 && gameObject->GetManagerID() != this->GetUniqueID())
	{
		XETODO("better return and error message");
		return XEResult::Fail;
	}

	m_SelectedGameObject = gameObject;

	return XEResult::Ok;
}

GameObjectMapIt GameObjectManager::begin()
{
	return m_GameObjectMap.begin();
}

GameObjectMapIt GameObjectManager::end()
{
	return m_GameObjectMap.end();
}

GameObjectMapItConst GameObjectManager::begin() const
{
	return m_GameObjectMap.begin();
}

GameObjectMapItConst GameObjectManager::end() const
{
	return m_GameObjectMap.end();
}

GameObjectMapItConst GameObjectManager::cbegin() const
{
	return m_GameObjectMap.cbegin();
}

GameObjectMapItConst GameObjectManager::cend() const
{
	return m_GameObjectMap.cend();
}
