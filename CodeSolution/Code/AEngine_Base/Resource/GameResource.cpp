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

/*************************
*   Precompiled Header   *
**************************/
#include "precomp_base.h"

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
GameResource::GameResource(const std::string& resourceName, GameResourceType resourceType)
    : Named(resourceName)
    , m_ResourceType(resourceType)
{
    //Increase reference count
    m_RefCount.fetch_add(1);
}

GameResource::~GameResource()
{
    AEAssert(m_RefCount == 0);
}

GameResource* GameResource::AddRef()
{
    //Increase reference count
    m_RefCount.fetch_add(1);

    return this;
}

void GameResource::SetNameWithoutLock(const std::string name)
{
    m_Name = name;
}

void GameResource::SetFileNameWithoutLock(const std::string& fileName)
{
    m_FileName = fileName;
}

void GameResource::SetName(const std::string name)
{
    //This has to be an atomic operation
    std::lock_guard<std::mutex> lock(m_GameResourceMutex);

    SetNameWithoutLock(name);
}

void GameResource::SetFileName(const std::string& fileName)
{
    //This has to be an atomic operation
    std::lock_guard<std::mutex> lock(m_GameResourceMutex);

    SetFileNameWithoutLock(fileName);
}

int64_t GameResource::Release()
{
    int64_t refZero = 0;

    int64_t refCount = m_RefCount.fetch_sub(1);

    if (!m_RefCount.compare_exchange_strong(refZero, 0))
    {
        if (refCount < 0)
        {
            return 0;
        }

        return (refCount - 1);
    }

    {
        std::lock_guard<std::mutex> lock(m_GameResourceMutex);

        if (m_ReleaseCallback != nullptr)
        {
            m_ReleaseCallback(this->GetUniqueID());
        }
    }

    //Delete the memory
    delete this;

    return 0;
}
