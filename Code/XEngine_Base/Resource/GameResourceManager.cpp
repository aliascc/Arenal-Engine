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
#include <functional>

/*************************
*   3rd Party Includes   *
**************************/
#include "fastformat\fastformat.hpp"
#include "fastformat\sinks\ostream.hpp"

/***************************
*   Game Engine Includes   *
****************************/
#include "GameResource.h"
#include "Logger\Logger.h"
#include "Base\BaseFunctions.h"
#include "GameResourceManager.h"
#include "Localization\LocalizationManager.h"
#include "Localization\LocalizationManagerDefs.h"

//Always include last
#include "Memory\MemLeaks.h"

/********************
*   Function Defs   *
*********************/
GameResourceManager::GameResourceManager()
{
}

GameResourceManager::~GameResourceManager()
{
}

bool GameResourceManager::GameResourceExists(uint64_t id)
{
	return (m_GameResourceMap.find(id) != m_GameResourceMap.end());
}

bool GameResourceManager::GameResourceStringIDExists(const std::wstring& stringId)
{
	return (m_StringIDToGameResource.find(stringId) != m_StringIDToGameResource.end());
}

GameResource* GameResourceManager::AcquireGameResourceByStringID(const std::wstring& stringId, GameResourceType checkType)
{
	std::lock_guard<std::mutex> lock(m_GameResourceManagerMutex);

	if(!GameResourceStringIDExists(stringId))
	{
		return nullptr;
	}

	uint64_t id = m_StringIDToGameResource[stringId];	

	if(!GameResourceExists(id))
	{
		m_StringIDToGameResource.erase(stringId);

		return nullptr;
	}

	GameResource* resource = m_GameResourceMap[id];

	//////////////////////////////////
	//Do a check for Resource Type
	if(checkType != GameResourceType::Unknown && resource->GetGameResourceType() != checkType)
	{
		return nullptr;
	}

	return resource->AddRef();
}

GameResource* GameResourceManager::AcquireGameResourceByID(uint64_t id, GameResourceType checkType)
{
	std::lock_guard<std::mutex> lock(m_GameResourceManagerMutex);

	if(!GameResourceExists(id))
	{
		return nullptr;
	}

	GameResource* resource = m_GameResourceMap[id];

	//////////////////////////////////
	//Do a check for Resource Type
	if(checkType != GameResourceType::Unknown && resource->GetGameResourceType() != checkType)
	{
		return nullptr;
	}

	return resource->AddRef();	
}

XEResult GameResourceManager::ManageGameResource(GameResource* gameResource, const std::wstring& stringID, bool keepAlive)
{
	XEAssert(gameResource != nullptr);
	if(gameResource == nullptr)
	{
		return XEResult::NullParameter;
	}

	std::lock_guard<std::mutex> lock(m_GameResourceManagerMutex);
	std::lock_guard<std::mutex> lockResource(gameResource->m_GameResourceMutex);

	if(gameResource->IsManaged())
	{
		return XEResult::ResourceIsManaged;
	}

	if(GameResourceExists(gameResource->GetUniqueID()))
	{
		return XEResult::ObjExists;
	}

	if(!stringID.empty())
	{
		if(GameResourceStringIDExists(stringID))
		{
			uint64_t id = m_StringIDToGameResource[stringID];

			if(GameResourceExists(id))
			{
				std::wstring msgerr = L"";
				fastformat::fmt(msgerr, XELOCMAN->GetLiteral(L"GAME_RESOURCE_STR_ID_EXISTS_ERR_MSG"), __FUNCTIONW__, stringID);
				XELOGGER->AddNewLog(LogLevel::Error, msgerr);

				return XEResult::ObjExists;
			}
		}
	}

	gameResource->m_ReleaseCallback		= std::bind(&GameResourceManager::GameResourceReleaseCallback, this, std::placeholders::_1, std::placeholders::_2);
	gameResource->m_Managed				= true;
	gameResource->m_KeepAlive			= keepAlive;
	gameResource->m_StringIdentifier	= stringID;

	m_GameResourceMap[gameResource->GetUniqueID()] = gameResource;

	if(!stringID.empty())
	{
		m_StringIDToGameResource[stringID] = gameResource->GetUniqueID();
	}

	return XEResult::Ok;
}

XEResult GameResourceManager::UnManageGameResource(uint64_t id)
{
	std::lock_guard<std::mutex> lock(m_GameResourceManagerMutex);

	if(!GameResourceExists(id))
	{
		return XEResult::NotFound;
	}

	GameResource* gameResource = m_GameResourceMap[id];
	
	std::lock_guard<std::mutex> lockResource(gameResource->m_GameResourceMutex);

	gameResource->m_Managed = false;
	gameResource->m_ReleaseCallback = nullptr;
	gameResource->m_KeepAlive = false;
	gameResource->m_StringIdentifier = L"";

	m_GameResourceMap.erase(m_GameResourceMap.find(id));

	return XEResult::Ok;
}

void GameResourceManager::GameResourceReleaseCallback(uint64_t id, bool wasDeleted)
{
	if(!wasDeleted)
	{
		return;
	}
	
	std::lock_guard<std::mutex> lock(m_GameResourceManagerMutex);
	
	if(!GameResourceExists(id))
	{
		std::wstring msgerr = L"";
		fastformat::fmt(msgerr, XELOCMAN->GetLiteral(L"GAME_RESOURCE_RELEASE_ID_NOT_FOUND_ERR_MSG"), __FUNCTIONW__, id);
		XELOGGER->AddNewLog(LogLevel::Warning, msgerr);

		return;
	}

	auto it = m_GameResourceMap.find(id);

	m_GameResourceMap.erase(it);
}
