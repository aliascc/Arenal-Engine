/********************************************************
*
* Author: Carlos Chacón N.
*
* Copyright 2012-2015
*
*********************************************************/

#pragma once

#ifndef _MESH_ANIMATION_GOC_H
#define _MESH_ANIMATION_GOC_H

/**********************
*   System Includes   *
***********************/
#include <map>
#include <string>

/*************************
*   3rd Party Includes   *
**************************/

/***************************
*   Game Engine Includes   *
****************************/
#include "Base\Base.h"
#include "GameObject\GameObjectDefs.h"
#include "GameObject\GameObjectComponent.h"

/********************
*   Forward Decls   *
*********************/
class Skeleton;
class Animation;
class SkeletonAsset;
class AnimationAsset;
class AnimationPlayer;

/***************
*   Typedefs   *
****************/
typedef std::map<uint64_t, GameObjectAssetPair<Animation>> AnimationAssetPairMap;

/*****************
*   Class Decl   *
******************/
class MeshAnimationGOC sealed : public GameObjectComponent
{

	private:

		/************************
		*   Private Variables   *
		*************************/
#pragma region Private Variables

		AnimationPlayer* m_AnimationPlayer;

		GameObjectAssetPair<Skeleton> m_SkeletonAsset;

		AnimationAssetPairMap m_AnimationAssetMap;

		bool m_OnLoop = false;

		bool m_BlendAnimation = false;

		float m_BlendTime = 0.0f;

#pragma endregion

		/***********************
		*   Private Methods    *
		************************/
#pragma region Private Methods

		template<class T>
		XEResult ClearAsset(GameObjectAssetPair<T>& goAssetPair, bool informGameAsset = true);

		void SkeletonAssetDeletion(GameAsset* asset);

		void AnimationAssetDeletion(GameAsset* asset);

#pragma endregion

	public:

		/****************************************
		 *   Constructor & Destructor Methods   *
		 ****************************************/
#pragma region Constructor & Destructor Methods

		/// <summary>
		/// MeshAnimationGOC Constructor
		/// </summary>
		/// <param name="gameObject>Game Object that this Component is attached too</param>
		MeshAnimationGOC(GameObject* gameObject);

		/// <summary>
		/// Default ScriptGOC Destructor
		/// </summary>
		virtual ~MeshAnimationGOC();

#pragma endregion

		/*******************
		 *   Get Methods   *
		 *******************/
#pragma region Get Methods

		inline bool GetOnLoop() const
		{
			return m_OnLoop;
		}

		inline bool GetBlendAnimation() const
		{
			return m_BlendAnimation;
		}

		inline float GetBlendTime() const
		{
			return m_BlendTime;
		}

		inline const AnimationAssetPairMap& GetAnimationAssetMap() const
		{
			return m_AnimationAssetMap;
		}

		inline AnimationPlayer* GetAnimationPlayer() const
		{
			return m_AnimationPlayer;
		}

		std::wstring GetSkeletonName() const;

		inline uint64_t MeshAnimationGOC::GetSkeletonAssetID() const
		{
			return m_SkeletonAsset.m_AssetID;
		}

#pragma endregion

		/******************
		*   Set Methods   *
		*******************/
#pragma region Set Methods

		inline void SetOnLoop(bool onLoop)
		{
			m_OnLoop = onLoop;
		}

		inline void SetBlendAnimation(bool blendAnim)
		{
			m_BlendAnimation = blendAnim;
		}

		inline void SetBlendTime(float blendTime)
		{
			m_BlendTime = blendTime;
		}

#pragma endregion
		
		/************************
		*   Framework Methods   *
		*************************/
#pragma region Framework Methods

		XEResult SetSkeletonAsset(SkeletonAsset* asset);

		XEResult RemoveSkeletonAsset();

		bool AnimationAssetExists(uint64_t animAssetID) const;

		XEResult AddAnimationAsset(AnimationAsset* asset);

		XEResult RemoveAnimationAsset(uint64_t animAssetID);

		XEResult PlayTestAnimation(uint64_t animAssetID);

		XEResult PauseAnimationPlayer();

#pragma endregion

};

#endif