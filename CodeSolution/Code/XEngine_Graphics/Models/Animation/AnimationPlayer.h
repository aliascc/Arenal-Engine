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
#include "Base\XEObject.h"
#include "AnimationDefs.h"
#include "Math\XEMathDefs.h"

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
class AnimationPlayer sealed : public XEObject
{
	private:

		/// <summary>
		/// Boost Mutex to Lock Thread to
		/// perform atomic operations
		/// </summary>
		std::mutex m_AnimationPlayerMutex;

		Animation* m_CurrentAnimation = nullptr;
		Animation* m_BlendingAnimation = nullptr;

		XETODO("correct names");
		BonePose bonePosesCurrent[XE_MAX_BONES];
		BonePose bonePosesBlending[XE_MAX_BONES];

		glm::mat4 m_BaseTransforms[XE_MAX_BONES];

		glm::mat4 m_WorldTransforms[XE_MAX_BONES];

		glm::mat4 m_BoneTransforms[XE_MAX_BONES];

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

		XEResult UpdateBaseTransformsBlending(float elapsedTime);
		XEResult UpdateBaseTransforms(float elapsedTime);
		XEResult UpdateWorldTransforms(const glm::mat4& world);
		XEResult UpdateBoneTransforms();

		XEResult RebootVariables(bool playing = true);

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

		XEResult BlendAnimtion(Animation* animClip, float blendTime, bool newAnimOnLoop = false, AnimationBlendType blendType = AnimationBlendType::Lerp);

		XEResult StartClip(Animation* animationClip, bool onLoop = false);

		XEResult StopClip();

		XEResult StopClipAndClearAnimations();

		XEResult PlayClip();

		XEResult PauseClip();
		
		XEResult Update(const TimerParams& timerParams, const glm::mat4& world = XEMathHelpers::Mat4Identity); 
};

#endif

