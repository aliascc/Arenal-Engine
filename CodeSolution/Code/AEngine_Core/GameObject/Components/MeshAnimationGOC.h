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
		AEResult ClearAsset(GameObjectAssetPair<T>& goAssetPair, bool informGameAsset = true);

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
		/// <param name="gameObject">Game Object that this Component is attached too</param>
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

		AEResult SetSkeletonAsset(SkeletonAsset* asset);

		AEResult RemoveSkeletonAsset();

		bool AnimationAssetExists(uint64_t animAssetID) const;

		AEResult AddAnimationAsset(AnimationAsset* asset);

		AEResult RemoveAnimationAsset(uint64_t animAssetID);

		AEResult PlayTestAnimation(uint64_t animAssetID);

		AEResult PauseAnimationPlayer();

#pragma endregion

};

#endif