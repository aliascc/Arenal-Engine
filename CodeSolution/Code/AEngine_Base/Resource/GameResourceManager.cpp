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
#include <functional>

/*************************
*   3rd Party Includes   *
**************************/
#include "cppformat\format.h"

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

AEResult GameResourceManager::ManageGameResource(GameResource* gameResource, const std::wstring& stringID, bool keepAlive)
{
	AEAssert(gameResource != nullptr);
	if(gameResource == nullptr)
	{
		return AEResult::NullParameter;
	}

	std::lock_guard<std::mutex> lock(m_GameResourceManagerMutex);
	std::lock_guard<std::mutex> lockResource(gameResource->m_GameResourceMutex);

	if(gameResource->IsManaged())
	{
		return AEResult::ResourceIsManaged;
	}

	if(GameResourceExists(gameResource->GetUniqueID()))
	{
		return AEResult::ObjExists;
	}

	if(!stringID.empty())
	{
		if(GameResourceStringIDExists(stringID))
		{
			uint64_t id = m_StringIDToGameResource[stringID];

			if(GameResourceExists(id))
			{
				std::wstring msgerr = fmt::format(AELOCMAN->GetLiteral(L"GAME_RESOURCE_STR_ID_EXISTS_ERR_MSG"), __FUNCTIONW__, stringID);
				AELOGGER->AddNewLog(LogLevel::Error, msgerr);

				return AEResult::ObjExists;
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

	return AEResult::Ok;
}

AEResult GameResourceManager::UnManageGameResource(uint64_t id)
{
	std::lock_guard<std::mutex> lock(m_GameResourceManagerMutex);

	if(!GameResourceExists(id))
	{
		return AEResult::NotFound;
	}

	GameResource* gameResource = m_GameResourceMap[id];
	
	std::lock_guard<std::mutex> lockResource(gameResource->m_GameResourceMutex);

	gameResource->m_Managed = false;
	gameResource->m_ReleaseCallback = nullptr;
	gameResource->m_KeepAlive = false;
	gameResource->m_StringIdentifier = L"";

	m_GameResourceMap.erase(m_GameResourceMap.find(id));

	return AEResult::Ok;
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
		std::wstring msgerr = fmt::format(AELOCMAN->GetLiteral(L"GAME_RESOURCE_RELEASE_ID_NOT_FOUND_ERR_MSG"), __FUNCTIONW__, id);
		AELOGGER->AddNewLog(LogLevel::Warning, msgerr);

		return;
	}

	auto it = m_GameResourceMap.find(id);

	m_GameResourceMap.erase(it);
}
