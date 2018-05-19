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
#include "precomp_core.h"

/**********************
*   System Includes   *
***********************/

/*************************
*   3rd Party Includes   *
**************************/

/***************************
*   Game Engine Includes   *
****************************/
#include "SkeletonAsset.h"
#include "Base\BaseFunctions.h"
#include "Models\Skinning\Skeleton.h"
#include "Resource\GameResourceManager.h"

//Always include last
#include "Memory\MemLeaks.h"

/********************
*   Function Defs   *
*********************/
AETODO("Check if this class needs a mutex");
SkeletonAsset::SkeletonAsset(const std::string& filePath, GameResourceManager* gameResourceManager)
    : GameAsset(GameContentType::Skeleton, filePath, gameResourceManager)
{
}

SkeletonAsset::~SkeletonAsset()
{
    AERelease(m_Skeleton);
}

Skeleton* SkeletonAsset::GetSkeletonReference()
{
    if(m_Skeleton == nullptr)
    {
        return nullptr;
    }

    return reinterpret_cast<Skeleton*>(m_Skeleton->AddRef());
}

AEResult SkeletonAsset::LoadAssetResource()
{
    AEAssert(m_GameResourceManager != nullptr);
    if (m_GameResourceManager == nullptr)
    {
        return AEResult::GameResourceManagerNull;
    }

    AEAssert(!m_FilePath.empty());
    if(m_FilePath.empty())
    {
        return AEResult::EmptyFilename;
    }

    AEResult ret = AEResult::Ok;

    AETODO("Look into what happens if file changes location");

    if(m_Skeleton != nullptr)
    {
        AETODO("Check return value");
        ret = m_Skeleton->Load();
    }
    else
    {
        /////////////////////////////////////////////
        //Check if Game Resources contains this Mesh
        m_Skeleton = (Skeleton*)m_GameResourceManager->AcquireGameResourceByStringID(m_FilePath, GameResourceType::Skeleton);

        if(m_Skeleton != nullptr)
        {
            AETODO("Check if we always need to reload");
            AETODO("Check return value");
            m_Skeleton->Load();

            return AEResult::Ok;
        }
        
        /////////////////////////////////////////////
        //Create Resource
        m_Skeleton = new Skeleton(m_Name);
        
        /////////////////////////////////////////////
        //Set File Name and Load
        m_Skeleton->SetFileName(m_FilePath);
                
        AETODO("Check return value");
        m_Skeleton->Load();

        /////////////////////////////////////////////
        //Add to Resource Manager
        ret = m_GameResourceManager->ManageGameResource(m_Skeleton, m_FilePath);
        if(ret != AEResult::Ok)
        {
            AETODO("Add log");

            AERelease(m_Skeleton);

            return AEResult::ResourceManagedFailed;
        }
    }

    return AEResult::Ok;
}
