/*
* Copyright (c) 2018 <Carlos Chac�n>
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
#include "MeshAnimationGOC.h"
#include "Base\BaseFunctions.h"
#include "Models\Skinning\Skeleton.h"
#include "Models\Animation\Animation.h"
#include "GameAssets\Assets\SkeletonAsset.h"
#include "GameAssets\Assets\AnimationAsset.h"
#include "Models\Animation\AnimationPlayer.h"

//Always include last
#include "Memory\MemLeaks.h"

/********************
*   Function Defs   *
*********************/
AETODO("Add mutex");
MeshAnimationGOC::MeshAnimationGOC(GameObject* gameObject)
    : GameObjectComponent(gameObject, GameObjectComponentType::MeshAnimation)
{
    m_AnimationPlayer = new AnimationPlayer();
}

MeshAnimationGOC::~MeshAnimationGOC()
{
    RemoveSkeletonAsset();

    DeleteMem(m_AnimationPlayer);
}

template <class T>
AEResult MeshAnimationGOC::ClearAsset(GameObjectAssetPair<T>& goAssetPair, bool informGameAsset)
{
    if (goAssetPair.m_ResourceAsset != nullptr)
    {
        if (goAssetPair.m_OnListenerObjDeletionEvent != nullptr && informGameAsset)
        {
            goAssetPair.m_OnListenerObjDeletionEvent(goAssetPair.m_CallerID);
        }

        goAssetPair.m_AssetID                        = 0;
        goAssetPair.m_CallerID                        = 0;
        goAssetPair.m_OnListenerObjDeletionEvent    = nullptr;

        AERelease(goAssetPair.m_ResourceAsset);
    }

    return AEResult::Ok;
}

void MeshAnimationGOC::SkeletonAssetDeletion(GameAsset* asset)
{
    AEAssert(asset != nullptr);
    if (asset == nullptr)
    {
        return;
    }

    AEAssert(asset->GetGameContentType() == GameContentType::Skeleton);
    if (asset->GetGameContentType() != GameContentType::Skeleton)
    {
        return;
    }

    if (m_SkeletonAsset.m_AssetID != asset->GetUniqueAssetID())
    {
        return;
    }

    m_AnimationPlayer->SetSkeleton(nullptr);

    ClearAsset<Skeleton>(m_SkeletonAsset, false);
}

AEResult MeshAnimationGOC::SetSkeletonAsset(SkeletonAsset* asset)
{
    AEAssert(asset != nullptr);
    if (asset == nullptr)
    {
        return AEResult::NullParameter;
    }

    if (asset->GetGameContentType() != GameContentType::Skeleton)
    {
        return AEResult::InvalidObjType;
    }

    if (m_SkeletonAsset.m_ResourceAsset != nullptr)
    {
        AEResult ret = RemoveSkeletonAsset();

        if (ret != AEResult::Ok)
        {
            AETODO("Log error");

            AETODO("Add better return code");
            return AEResult::Fail;
        }
    }

    uint64_t callerID = AE_Base::GetNextUniqueID();

    AEResult ret = asset->RegisterEventHandlers(callerID, std::bind(&MeshAnimationGOC::SkeletonAssetDeletion, this, std::placeholders::_1));
    if (ret != AEResult::Ok)
    {
        AETODO("Log error");

        return ret;
    }

    m_SkeletonAsset.m_AssetID                       = asset->GetUniqueAssetID();
    m_SkeletonAsset.m_CallerID                      = callerID;
    m_SkeletonAsset.m_ResourceAsset                 = asset->GetSkeletonReference();
    m_SkeletonAsset.m_OnListenerObjDeletionEvent    = asset->GetOnListenerObjDeletionEventHandler();
    m_SkeletonAsset.m_GameAsset                     = asset;

    m_AnimationPlayer->SetSkeleton(m_SkeletonAsset.m_ResourceAsset);

    return AEResult::Ok;
}

AEResult MeshAnimationGOC::RemoveSkeletonAsset()
{
    m_AnimationPlayer->SetSkeleton(nullptr);

    return ClearAsset<Skeleton>(m_SkeletonAsset, true);
}

std::string MeshAnimationGOC::GetSkeletonName() const
{
    if (m_SkeletonAsset.m_ResourceAsset != nullptr)
    {
        return m_SkeletonAsset.m_ResourceAsset->GetName();
    }
    else
    {
        return "";
    }
}

bool MeshAnimationGOC::AnimationAssetExists(uint64_t animAssetID)  const
{
    return (m_AnimationAssetMap.find(animAssetID) != m_AnimationAssetMap.end());
}

AEResult MeshAnimationGOC::AddAnimationAsset(AnimationAsset* asset)
{
    AEAssert(asset != nullptr);
    if (asset == nullptr)
    {
        return AEResult::NullParameter;
    }

    if (asset->GetGameContentType() != GameContentType::Animation)
    {
        return AEResult::InvalidObjType;
    }

    if (AnimationAssetExists(asset->GetUniqueAssetID()))
    {
        return AEResult::ObjExists;
    }

    GameObjectAssetPair<Animation> newAnimation;

    uint64_t callerID = AE_Base::GetNextUniqueID();

    AEResult ret = asset->RegisterEventHandlers(callerID, std::bind(&MeshAnimationGOC::AnimationAssetDeletion, this, std::placeholders::_1));
    if (ret != AEResult::Ok)
    {
        AETODO("Log error");

        return ret;
    }

    newAnimation.m_AssetID                      = asset->GetUniqueAssetID();
    newAnimation.m_CallerID                     = callerID;
    newAnimation.m_ResourceAsset                = asset->GetAnimationReference();
    newAnimation.m_OnListenerObjDeletionEvent   = asset->GetOnListenerObjDeletionEventHandler();
    newAnimation.m_GameAsset                    = asset;

    m_AnimationAssetMap[asset->GetUniqueAssetID()] = newAnimation;

    return AEResult::Ok;
}

void MeshAnimationGOC::AnimationAssetDeletion(GameAsset* asset)
{
    AEAssert(asset != nullptr);
    if (asset == nullptr)
    {
        return;
    }

    AEAssert(asset->GetGameContentType() == GameContentType::Animation);
    if (asset->GetGameContentType() != GameContentType::Animation)
    {
        return;
    }

    auto animAssetIt = m_AnimationAssetMap.find(asset->GetUniqueAssetID());
    if (animAssetIt == m_AnimationAssetMap.end())
    {
        return;
    }

    ///////////////////////////////////////////////////
    //Makes sure Animation Player is not referencing the
    //Animation to be deleted
    if (m_AnimationPlayer->UsingAnimationClip(animAssetIt->second.m_ResourceAsset))
    {
        m_AnimationPlayer->StopClipAndClearAnimations();
    }

    ClearAsset<Animation>(animAssetIt->second, false);

    m_AnimationAssetMap.erase(animAssetIt);
}

AEResult MeshAnimationGOC::RemoveAnimationAsset(uint64_t animAssetID)
{
    ///////////////////////////////////////////////////
    //Pre-checks
    if (!AnimationAssetExists(animAssetID))
    {
        return AEResult::NotFound;
    }

    ///////////////////////////////////////////////////
    //Makes sure Animation Player is not referencing the
    //Animation to be deleted
    auto animAssetIt = m_AnimationAssetMap.find(animAssetID);

    if (m_AnimationPlayer->UsingAnimationClip(animAssetIt->second.m_ResourceAsset))
    {
        m_AnimationPlayer->StopClipAndClearAnimations();
    }

    ClearAsset<Animation>(animAssetIt->second, false);

    m_AnimationAssetMap.erase(animAssetIt);

    return AEResult::Ok;
}

AEResult MeshAnimationGOC::PlayTestAnimation(uint64_t animAssetID)
{
    if (!AnimationAssetExists(animAssetID))
    {
        return AEResult::NotFound;
    }

    Animation* animClip = m_AnimationAssetMap[animAssetID].m_ResourceAsset;;

    if (m_BlendAnimation)
    {
        return m_AnimationPlayer->BlendAnimtion(animClip, m_BlendTime, m_OnLoop);
    }
    else
    {
        return m_AnimationPlayer->StartClip(animClip, m_OnLoop);
    }
}

AEResult MeshAnimationGOC::PauseAnimationPlayer()
{
    return m_AnimationPlayer->PauseClip();
}
