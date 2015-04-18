/********************************************************
*
* Author: Carlos Chacón N.
*
* Created: 5/17/2013
*
* Desc:
*
*
* Copyright 2013
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
XETODO("Add mutex");
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
XEResult MeshAnimationGOC::ClearAsset(GameObjectAssetPair<T>& goAssetPair, bool informGameAsset)
{
	if (goAssetPair.m_ResourceAsset != nullptr)
	{
		if (goAssetPair.m_OnListenerObjDeletionEvent != nullptr && informGameAsset)
		{
			goAssetPair.m_OnListenerObjDeletionEvent(goAssetPair.m_CallerID);
		}

		goAssetPair.m_AssetID						= 0;
		goAssetPair.m_CallerID						= 0;
		goAssetPair.m_OnListenerObjDeletionEvent	= nullptr;

		XERelease(goAssetPair.m_ResourceAsset);
	}

	return XEResult::Ok;
}

void MeshAnimationGOC::SkeletonAssetDeletion(GameAsset* asset)
{
	XEAssert(asset != nullptr);
	if (asset == nullptr)
	{
		return;
	}

	XEAssert(asset->GetGameContentType() == GameContentType::Skeleton);
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

XEResult MeshAnimationGOC::SetSkeletonAsset(SkeletonAsset* asset)
{
	XEAssert(asset != nullptr);
	if (asset == nullptr)
	{
		return XEResult::NullParameter;
	}

	if (asset->GetGameContentType() != GameContentType::Skeleton)
	{
		return XEResult::InvalidObjType;
	}

	if (m_SkeletonAsset.m_ResourceAsset != nullptr)
	{
		XEResult ret = RemoveSkeletonAsset();

		if (ret != XEResult::Ok)
		{
			XETODO("Log error");

			XETODO("Add better return code");
			return XEResult::Fail;
		}
	}

	uint64_t callerID = XE_Base::GetNextUniqueID();

	XEResult ret = asset->RegisterEventHandlers(callerID, std::bind(&MeshAnimationGOC::SkeletonAssetDeletion, this, std::placeholders::_1));
	if (ret != XEResult::Ok)
	{
		XETODO("Log error");

		return ret;
	}

	m_SkeletonAsset.m_AssetID						= asset->GetUniqueAssetID();
	m_SkeletonAsset.m_CallerID						= callerID;
	m_SkeletonAsset.m_ResourceAsset					= asset->GetSkeletonReference();
	m_SkeletonAsset.m_OnListenerObjDeletionEvent	= asset->GetOnListenerObjDeletionEventHandler();
	m_SkeletonAsset.m_GameAsset						= asset;

	m_AnimationPlayer->SetSkeleton(m_SkeletonAsset.m_ResourceAsset);

	return XEResult::Ok;
}

XEResult MeshAnimationGOC::RemoveSkeletonAsset()
{
	m_AnimationPlayer->SetSkeleton(nullptr);

	return ClearAsset<Skeleton>(m_SkeletonAsset, true);
}

std::wstring MeshAnimationGOC::GetSkeletonName() const
{
	if (m_SkeletonAsset.m_ResourceAsset != nullptr)
	{
		return m_SkeletonAsset.m_ResourceAsset->GetName();
	}
	else
	{
		return L"";
	}
}

bool MeshAnimationGOC::AnimationAssetExists(uint64_t animAssetID)  const
{
	return (m_AnimationAssetMap.find(animAssetID) != m_AnimationAssetMap.end());
}

XEResult MeshAnimationGOC::AddAnimationAsset(AnimationAsset* asset)
{
	XEAssert(asset != nullptr);
	if (asset == nullptr)
	{
		return XEResult::NullParameter;
	}

	if (asset->GetGameContentType() != GameContentType::Animation)
	{
		return XEResult::InvalidObjType;
	}

	if (AnimationAssetExists(asset->GetUniqueAssetID()))
	{
		return XEResult::ObjExists;
	}

	GameObjectAssetPair<Animation> newAnimation;

	uint64_t callerID = XE_Base::GetNextUniqueID();

	XEResult ret = asset->RegisterEventHandlers(callerID, std::bind(&MeshAnimationGOC::AnimationAssetDeletion, this, std::placeholders::_1));
	if (ret != XEResult::Ok)
	{
		XETODO("Log error");

		return ret;
	}

	newAnimation.m_AssetID							= asset->GetUniqueAssetID();
	newAnimation.m_CallerID							= callerID;
	newAnimation.m_ResourceAsset					= asset->GetAnimationReference();
	newAnimation.m_OnListenerObjDeletionEvent		= asset->GetOnListenerObjDeletionEventHandler();
	newAnimation.m_GameAsset						= asset;

	m_AnimationAssetMap[asset->GetUniqueAssetID()] = newAnimation;

	return XEResult::Ok;
}

void MeshAnimationGOC::AnimationAssetDeletion(GameAsset* asset)
{
	XEAssert(asset != nullptr);
	if (asset == nullptr)
	{
		return;
	}

	XEAssert(asset->GetGameContentType() == GameContentType::Animation);
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

XEResult MeshAnimationGOC::RemoveAnimationAsset(uint64_t animAssetID)
{
	///////////////////////////////////////////////////
	//Pre-checks
	if (!AnimationAssetExists(animAssetID))
	{
		return XEResult::NotFound;
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

	return XEResult::Ok;
}

XEResult MeshAnimationGOC::PlayTestAnimation(uint64_t animAssetID)
{
	if (!AnimationAssetExists(animAssetID))
	{
		return XEResult::NotFound;
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

XEResult MeshAnimationGOC::PauseAnimationPlayer()
{
	return m_AnimationPlayer->PauseClip();
}