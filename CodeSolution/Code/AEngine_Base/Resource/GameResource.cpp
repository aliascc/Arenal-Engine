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

/*************************
*   3rd Party Includes   *
**************************/

/***************************
*   Game Engine Includes   *
****************************/
#include "GameResource.h"
#include "Base\BaseFunctions.h"

//Always include last
#include "Memory\MemLeaks.h"

/********************
*   Function Defs   *
*********************/
AETODO("Allow constructor to be public and destructor only private, that way GRs are only created by new and not static, when this add ref++ in constructor")
GameResource::GameResource(const std::wstring& resourceName, GameResourceType resourceType)
	: UniqueAEObjectNamed(resourceName)
	, m_ResourceType(resourceType)
{
	//Increase reference count
	m_RefCount++;
}

GameResource::~GameResource()
{
	AEAssert(m_RefCount == 0);
}

GameResource* GameResource::AddRef()
{
	//This has to be an atomic operation
	std::lock_guard<std::mutex> lock(m_GameResourceMutex);

	m_RefCount++;

	return this;
}

void GameResource::SetNameWithoutLock(const std::wstring name)
{
	m_Name = name;
}

void GameResource::SetFileNameWithoutLock(const std::wstring& fileName)
{
	m_FileName = fileName;
}

void GameResource::SetName(const std::wstring name)
{
	//This has to be an atomic operation
	std::lock_guard<std::mutex> lock(m_GameResourceMutex);

	SetNameWithoutLock(name);
}

void GameResource::SetFileName(const std::wstring& fileName)
{
	//This has to be an atomic operation
	std::lock_guard<std::mutex> lock(m_GameResourceMutex);

	SetFileNameWithoutLock(fileName);
}

uint32_t GameResource::Release()
{
	uint32_t refCount = 0;

	//In Scope so the thread will lock with
	//boost lock and it will be release before
	//deleting
	{
		//This has to be an atomic operation
		std::lock_guard<std::mutex> lock(m_GameResourceMutex);

		if(m_RefCount == 0 && m_KeepAlive)
		{
			return 0;
		}

		if(m_RefCount == 0 && m_Deleting)
		{
			return 0;
		}

		AEAssert(m_RefCount != 0);
		if(m_RefCount == 0)
		{
			return -1;
		}

		//We assign this here because if we delete the
		//Game Resource its member variables will be gone
		//be the time we return the ref count
		refCount = --m_RefCount;

		if(m_RefCount == 0 && !m_KeepAlive)
		{
			m_Deleting = true;
		}

		if(m_ReleaseCallback != nullptr)
		{
			m_ReleaseCallback(this->GetUniqueID(), m_Deleting);
		}
	}

	if(m_Deleting)
	{
		delete this;
	}

	return refCount;
}