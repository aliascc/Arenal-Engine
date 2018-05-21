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
#include "MeshGOC.h"
#include "Models\Mesh.h"
#include "GameAssets\Assets\MeshAsset.h"

//Always include last
#include "Memory\MemLeaks.h"

/********************
*   Function Defs   *
*********************/
AETODO("Add mutex");
MeshGOC::MeshGOC(GameObject* gameObject)
    : GameObjectComponent(gameObject, GameObjectComponentType::Mesh)
{
}

MeshGOC::~MeshGOC()
{
    RemoveMeshAsset();
}

void MeshGOC::MeshAssetDeletion(GameAsset* asset)
{
    AEAssert(asset != nullptr);

    if(asset == nullptr)
    {
        return;
    }

    AEAssert(asset->GetGameContentType() == GameContentType::Mesh);
    if(asset->GetGameContentType() != GameContentType::Mesh)
    {
        return;
    }

    if(m_Mesh.m_AssetID != asset->GetUniqueAssetID())
    {
        return;
    }

    ClearMeshAsset(false);
}

AEResult MeshGOC::SetMeshAsset(MeshAsset* asset)
{
    AEAssert(asset != nullptr);

    if(asset == nullptr)
    {
        return AEResult::NullParameter;
    }

    if(asset->GetGameContentType() != GameContentType::Mesh)
    {
        return AEResult::InvalidObjType;
    }

    if(m_Mesh.m_ResourceAsset != nullptr)
    {
        AEResult ret = RemoveMeshAsset();

        if(ret != AEResult::Ok)
        {
            AETODO("Log error");

            AETODO("Add better return code");
            return AEResult::Fail;
        }
    }

    uint64_t callerID = AE_Base::GetNextUniqueID();

    AEResult ret = asset->RegisterEventHandlers(callerID, std::bind(&MeshGOC::MeshAssetDeletion, this, std::placeholders::_1));
    if(ret != AEResult::Ok)
    {
        AETODO("Log error");

        return ret;
    }

    m_Mesh.m_AssetID                    = asset->GetUniqueAssetID();
    m_Mesh.m_CallerID                   = callerID;
    m_Mesh.m_ResourceAsset              = asset->GetMeshReference();
    m_Mesh.m_OnListenerObjDeletionEvent = asset->GetOnListenerObjDeletionEventHandler();

    return AEResult::Ok;
}

AEResult MeshGOC::RemoveMeshAsset()
{
    return ClearMeshAsset(true);
}

AEResult MeshGOC::ClearMeshAsset(bool informGameAsset)
{
    if(m_Mesh.m_ResourceAsset != nullptr)
    {
        if(m_Mesh.m_OnListenerObjDeletionEvent != nullptr && informGameAsset)
        {
            m_Mesh.m_OnListenerObjDeletionEvent(m_Mesh.m_CallerID);
        }

        m_Mesh.m_AssetID                    = 0;
        m_Mesh.m_CallerID                   = 0;
        m_Mesh.m_OnListenerObjDeletionEvent = nullptr;

        AERelease(m_Mesh.m_ResourceAsset);
    }

    return AEResult::Ok;
}

std::string MeshGOC::GetMeshName() const
{
    if(m_Mesh.m_ResourceAsset != nullptr)
    {
        return m_Mesh.m_ResourceAsset->GetName();
    }
    else
    {
        return "";
    }
}
