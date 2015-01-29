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
#include <list>

/*************************
*   3rd Party Includes   *
**************************/

/***************************
*   Game Engine Includes   *
****************************/
#include "GameAsset.h"
#include "Base\BaseFunctions.h"
#include "Resource\GameResourceManager.h"

//Always include last
#include "Memory\MemLeaks.h"

/********************
*   Function Defs   *
*********************/
GameAsset::GameAsset(GameContentType gameContentType, const std::wstring& filePath, GameResourceManager* gameResourceManager)
	: m_GameContentType(gameContentType)
	, m_FilePath(filePath)
	, m_GameResourceManager(gameResourceManager)
{
	XEAssert(!filePath.empty());
	XEAssert(gameResourceManager != nullptr);

	m_OnListenerObjDeletionEventHandler = std::bind(&GameAsset::UnregisterEventHandlers, this, std::placeholders::_1);
}

GameAsset::~GameAsset()
{
	for(auto eventHandlers : m_GameAssetEventHandlersMap)
	{
		eventHandlers.second.m_OnGameAssetDeletionEvent(this);
	}
	m_GameAssetEventHandlersMap.clear();

	if(m_OnGameAssetDeletionNotifyManagerEvent != nullptr)
	{
		m_OnGameAssetDeletionNotifyManagerEvent(this->GetUniqueAssetID());
	}
}

XEResult GameAsset::RegisterEventHandlers(uint64_t id, OnGameAssetDeletionEvent onGameAssetDeletionEvent, OnGameAssetReloadEvent onGameAssetReloadEvent, OnGameAssetPreReloadEvent onGameAssetPreReloadEvent)
{
	XEAssert(onGameAssetDeletionEvent != nullptr);
	if(onGameAssetDeletionEvent == nullptr)
	{
		return XEResult::NullParameter;
	}

	std::lock_guard<std::mutex> lock(m_GameAssetMutex);

	if(m_GameAssetEventHandlersMap.find(id) != m_GameAssetEventHandlersMap.end())
	{
		return XEResult::ObjExists;
	}

	m_GameAssetEventHandlersMap[id] = GameAssetEventHandlers(id, onGameAssetDeletionEvent, onGameAssetReloadEvent, onGameAssetPreReloadEvent);

	return XEResult::Ok;
}

XEResult GameAsset::UnregisterEventHandlers(uint64_t id)
{
	std::lock_guard<std::mutex> lock(m_GameAssetMutex);

	auto evenHandlerstIt = m_GameAssetEventHandlersMap.find(id);
	if(evenHandlerstIt == m_GameAssetEventHandlersMap.end())
	{
		XEResult::NotFound;
	}
	
	m_GameAssetEventHandlersMap.erase(evenHandlerstIt);

	return XEResult::Ok;
}

XEResult GameAsset::LoadAsset()
{
	if (m_GameResourceManager == nullptr)
	{
		return XEResult::GameResourceManagerNull;
	}

	std::lock_guard<std::mutex> lock(m_GameAssetMutex);

	if (m_IsLoaded)
	{
		for (auto eventHandlers : m_GameAssetEventHandlersMap)
		{
			const GameAssetEventHandlers& gaEventHandlers = eventHandlers.second;
			if (gaEventHandlers.m_OnGameAssetPreReloadEvent != nullptr)
			{
				gaEventHandlers.m_OnGameAssetPreReloadEvent(this);
			}
		}

		m_IsLoaded = false;
	}

	XEResult ret = LoadAssetResource();
	if(ret != XEResult::Ok)
	{
		XETODO("Add log");

		return ret;
	}

	m_IsLoaded = true;

	for(auto eventHandlers : m_GameAssetEventHandlersMap)
	{
		const GameAssetEventHandlers& gaEventHandlers = eventHandlers.second;
		if(gaEventHandlers.m_OnGameAssetReloadEvent != nullptr)
		{
			gaEventHandlers.m_OnGameAssetReloadEvent(this);
		}
	}

	return XEResult::Ok;
}