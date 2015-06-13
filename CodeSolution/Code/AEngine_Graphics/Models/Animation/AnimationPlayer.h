/********************************************************
*
* Author: Carlos Chacón N.
*
* Copyright 2012-2015
*
*********************************************************/

#pragma once

#ifndef _ANIMATION_PLAYER_H
#define _ANIMATION_PLAYER_H

/**********************
*   System Includes   *
***********************/
#include <list>
#include <mutex>
#include <string>

/*************************
*   3rd Party Includes   *
**************************/
#include "glm\glm.hpp"

/***************************
*   Game Engine Includes   *
****************************/
#include "KeyFrame.h"
#include "Base\Base.h"
#include "Base\AEObject.h"
#include "AnimationDefs.h"
#include "Math\AEMathDefs.h"

/************
*   Using   *
*************/

/********************
*   Forward Decls   *
*********************/
class Skeleton;
class Animation;
struct TimerParams;

/*****************
*   Class Decl   *
******************/
class AnimationPlayer sealed : public AEObject
{
	private:

		/// <summary>
		/// Boost Mutex to Lock Thread to
		/// perform atomic operations
		/// </summary>
		std::mutex m_AnimationPlayerMutex;

		Animation* m_CurrentAnimation = nullptr;
		Animation* m_BlendingAnimation = nullptr;

		AETODO("correct names");
		BonePose bonePosesCurrent[AE_MAX_BONES];
		BonePose bonePosesBlending[AE_MAX_BONES];

		glm::mat4 m_BaseTransforms[AE_MAX_BONES];

		glm::mat4 m_WorldTransforms[AE_MAX_BONES];

		glm::mat4 m_BoneTransforms[AE_MAX_BONES];

		const Skeleton* m_Skeleton = nullptr;

		bool m_OnLoop = false;

		bool m_Playing = false;

		float m_PlayedTime = 0.0f;

		AnimationBlendType m_BlendType = AnimationBlendType::Lerp;

		float m_BlendTime = 0.0f;

		float m_TotalBlendTime = 0.0f;

		bool m_IsBlending = false;

		bool m_BlenAnimToLoop = false;

		uint32_t m_CurrentKeyFrameBlend = 0;

		bool m_Paused = false;

		uint32_t m_CurrentKeyFrame = 0;

		AEResult UpdateBaseTransformsBlending(float elapsedTime);
		AEResult UpdateBaseTransforms(float elapsedTime);
		AEResult UpdateWorldTransforms(const glm::mat4& world);
		AEResult UpdateBoneTransforms();

		AEResult RebootVariables(bool playing = true);

	public:
		AnimationPlayer();
		virtual ~AnimationPlayer();

		inline bool IsPlaying() const
		{
			return m_Playing;
		}

		inline bool IsPaused() const
		{
			return m_Paused;
		}

		inline bool OnLoop() const
		{
			return m_OnLoop;
		}

		inline float GetPlayedTime() const
		{
			return m_PlayedTime;
		}

		inline glm::mat4* GetBaseTransforms()
		{
			return m_BaseTransforms;
		}

		inline glm::mat4* GetBoneTransforms()
		{
			return m_BoneTransforms;
		}

		bool UsingAnimationClip(const Animation* animationClip) const;

		void SetSkeleton(const Skeleton* skeleton);

		AEResult BlendAnimtion(Animation* animClip, float blendTime, bool newAnimOnLoop = false, AnimationBlendType blendType = AnimationBlendType::Lerp);

		AEResult StartClip(Animation* animationClip, bool onLoop = false);

		AEResult StopClip();

		AEResult StopClipAndClearAnimations();

		AEResult PlayClip();

		AEResult PauseClip();
		
		AEResult Update(const TimerParams& timerParams, const glm::mat4& world = AEMathHelpers::Mat4Identity); 
};

#endif

