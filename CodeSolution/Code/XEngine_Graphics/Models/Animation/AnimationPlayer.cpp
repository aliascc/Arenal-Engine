/********************************************************
*
* Author: Carlos Chacón N.
*
* Copyright 2012-2015
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
#include "KeyFrame.h"
#include "Animation.h"
#include "AnimationPlayer.h"
#include "Time\XETimeDefs.h"
#include "Base\BaseFunctions.h"
#include "Models\Skinning\Bone.h"
#include "Models\Skinning\Skeleton.h"
#include "Models\Skinning\SkinningDefs.h"

//Always include last
#include "Memory\MemLeaks.h"

/********************
*   Function Defs   *
*********************/
AnimationPlayer::AnimationPlayer()
{
	std::fill_n(m_BaseTransforms, XE_MAX_BONES, XEMathHelpers::Mat4Identity);
	std::fill_n(m_WorldTransforms, XE_MAX_BONES, XEMathHelpers::Mat4Identity);
	std::fill_n(m_BoneTransforms, XE_MAX_BONES, XEMathHelpers::Mat4Identity);
	std::fill_n(bonePosesCurrent, XE_MAX_BONES, BonePose());
	std::fill_n(bonePosesBlending, XE_MAX_BONES, BonePose());
}

AnimationPlayer::~AnimationPlayer()
{
}

XEResult AnimationPlayer::UpdateBaseTransformsBlending(float elapsedTime)
{
	if (m_Skeleton == nullptr)
	{
		XETODO("Add skeleton null return");
		return XEResult::NullObj;
	}

	XEAssert(m_CurrentAnimation != nullptr);
	XEAssert(m_BlendingAnimation != nullptr);
	if (m_CurrentAnimation == nullptr || m_BlendingAnimation == nullptr)
	{
		return XEResult::NullObj;
	}

	m_PlayedTime += elapsedTime;
	m_BlendTime += elapsedTime;
	
	//We only want to know if animation is in loop, 
	//if not just use the last frame for the blending
	if(m_PlayedTime > m_CurrentAnimation->GetDuration())
	{
		if(m_OnLoop)
		{
			RebootVariables();
		}
	}

	if(m_BlendTime > m_BlendingAnimation->GetDuration() || m_BlendTime > m_TotalBlendTime)
	{
		m_CurrentAnimation = m_BlendingAnimation;
		m_PlayedTime = m_BlendTime;
		m_CurrentKeyFrame = m_CurrentKeyFrameBlend;
		m_OnLoop = m_BlenAnimToLoop;

		m_IsBlending = false;
		m_BlendTime = 0.0f;
		m_CurrentKeyFrameBlend = 0;
		m_BlendingAnimation = nullptr;
		m_BlenAnimToLoop = false;

		return XEResult::Ok;
	}

	uint32_t size = m_CurrentAnimation->GetFrameCount();
	for(; m_CurrentKeyFrame < size; ++m_CurrentKeyFrame)
	{
		const KeyFrame& frame = m_CurrentAnimation->GetKeyFrame(m_CurrentKeyFrame);
		float frameTime = frame.GetTime();

		if(frameTime > m_PlayedTime)
		{
			break;
		}

        //Use this key frame
		int32_t boneIndex = frame.GetBoneIndex();

		bonePosesCurrent[boneIndex] = frame.GetBonePose();
	}
	
	size = m_BlendingAnimation->GetFrameCount();
	for(; m_CurrentKeyFrameBlend < size; ++m_CurrentKeyFrameBlend)
	{
		const KeyFrame& frame = m_BlendingAnimation->GetKeyFrame(m_CurrentKeyFrameBlend);
		float frameTime = frame.GetTime();

		if(frameTime > m_TotalBlendTime)
		{
			break;
		}

        //Use this key frame
		int32_t boneIndex = frame.GetBoneIndex();

		bonePosesBlending[boneIndex] = frame.GetBonePose();
	}

	
	for (uint32_t i = 0; i < m_Skeleton->GetSkeletonSize(); ++i)
	{
		BonePose finalBonePose;

		float delta = m_BlendTime / m_TotalBlendTime;

		BonePose& currentBonePose = bonePosesCurrent[i];
		BonePose& blendBonePose = bonePosesBlending[i];
		
		XETODO("Create helper for this");
		finalBonePose.m_Scale = glm::mix(currentBonePose.m_Scale, blendBonePose.m_Scale, delta);
		finalBonePose.m_Translation = glm::mix(currentBonePose.m_Translation, blendBonePose.m_Translation, delta);
		finalBonePose.m_RotationQ = glm::slerp(currentBonePose.m_RotationQ, blendBonePose.m_RotationQ, delta);
		
		finalBonePose.CalculateTransformMatrix();
		const glm::mat4& transformMatrix = finalBonePose.GetTransformMatrix();

		if(transformMatrix != XEMathHelpers::Mat4Identity)
		{
			m_BaseTransforms[i] = transformMatrix;
		}
	}

	return XEResult::Ok;
}

XEResult AnimationPlayer::UpdateBaseTransforms(float elapsedTime)
{
	XEAssert(m_CurrentAnimation != nullptr);
	if(m_CurrentAnimation == nullptr)
	{
		return XEResult::NullObj;
	}

	m_PlayedTime += elapsedTime;

	if(m_PlayedTime > m_CurrentAnimation->GetDuration())
	{
		if(m_OnLoop)
		{
			RebootVariables();
		}
		else
		{
			StopClip();
		}
	}

	uint32_t size = m_CurrentAnimation->GetFrameCount();

	for(; m_CurrentKeyFrame < size; ++m_CurrentKeyFrame)
	{
		const KeyFrame& frame = m_CurrentAnimation->GetKeyFrame(m_CurrentKeyFrame);
		float frameTime = frame.GetTime();

		if(frameTime > m_PlayedTime)
		{
			break;
		}

        //Use this key frame
		int32_t boneIndex = frame.GetBoneIndex();
		const glm::mat4& transformMatrix = frame.GetBonePose().GetTransformMatrix();

		bonePosesCurrent[boneIndex] = frame.GetBonePose();
		m_BaseTransforms[boneIndex] = transformMatrix;
	}

	return XEResult::Ok;
}

XEResult AnimationPlayer::UpdateWorldTransforms(const glm::mat4& world)
{
	if (m_Skeleton == nullptr)
	{
		XETODO("Add skeleton null return");
		return XEResult::NullObj;
	}

	XEAssert(m_CurrentAnimation != nullptr);
	if(m_CurrentAnimation == nullptr)
	{
		return XEResult::NullObj;
	}

	uint32_t size = m_Skeleton->GetSkeletonSize();

	for(uint32_t i = 0; i < size; ++i)
    {
		int32_t parentIndex = m_Skeleton->GetBone(i)->GetParentIndex();

		//Root Bones
		if(parentIndex == -1)
		{
			m_WorldTransforms[i] = world * m_BaseTransforms[i];
		}
		else //Child Bones
		{
			m_WorldTransforms[i] = m_WorldTransforms[parentIndex] * m_BaseTransforms[i];
		}
    }

	return XEResult::Ok;
}

XEResult AnimationPlayer::UpdateBoneTransforms()
{
	if (m_Skeleton == nullptr)
	{
		XETODO("Add skeleton null return");
		return XEResult::NullObj;
	}

	XEAssert(m_CurrentAnimation != nullptr);
	if(m_CurrentAnimation == nullptr)
	{
		return XEResult::NullObj;
	}

	if (m_Skeleton == nullptr)
	{
		return XEResult::NullObj;
	}

	uint32_t size = m_Skeleton->GetSkeletonSize();

	for(uint32_t i = 0; i < size; ++i)
    {
		const glm::mat4& offsetMatrix = m_Skeleton->GetBone(i)->GetOffSetMatrix();
		
		m_BoneTransforms[i] = m_WorldTransforms[i] * offsetMatrix;
    }

	return XEResult::Ok;
}

XEResult AnimationPlayer::RebootVariables(bool playing)
{
	if (m_Skeleton == nullptr)
	{
		XETODO("Add skeleton null return");
		return XEResult::NullObj;
	}

	if(m_CurrentAnimation == nullptr)
	{
		return XEResult::NullObj;
	}

	uint32_t size = m_Skeleton->GetSkeletonSize();

	for(uint32_t i = 0; i < size; ++i)
	{
		m_BaseTransforms[i] = m_Skeleton->GetBone(i)->GetLocalMatrix();
	}

	m_Playing = playing;
	m_Paused = false;
	m_PlayedTime = 0.0f;
	m_CurrentKeyFrame = 0;

	std::fill_n(bonePosesCurrent, XE_MAX_BONES, BonePose());

	return XEResult::Ok;
}

XEResult AnimationPlayer::StartClip(Animation* animation, bool onLoop)
{
	std::lock_guard<std::mutex> lock(m_AnimationPlayerMutex);

	if (m_Skeleton == nullptr)
	{
		XETODO("Add Skeleton Null to return");
		return XEResult::Fail;
	}

	XEAssert(animation != nullptr);
	if(animation == nullptr)
	{
		return XEResult::NullParameter;
	}

	m_BlendingAnimation = nullptr;
	m_CurrentAnimation = animation;
	m_OnLoop = onLoop;

	return RebootVariables();
}

XEResult AnimationPlayer::StopClip()
{
	m_Playing = false;
	m_Paused = false;

	return XEResult::Ok;
}

XEResult AnimationPlayer::StopClipAndClearAnimations()
{
	std::lock_guard<std::mutex> lock(m_AnimationPlayerMutex);

	m_Playing = false;
	m_Paused = false;

	m_BlendingAnimation = nullptr;
	m_CurrentAnimation = nullptr;

	return XEResult::Ok;
}

XEResult AnimationPlayer::PlayClip()
{
	std::lock_guard<std::mutex> lock(m_AnimationPlayerMutex);

	if (m_Skeleton == nullptr)
	{
		XETODO("Add Skeleton Null to return");
		return XEResult::Fail;
	}

	if(m_CurrentAnimation == nullptr)
	{
		return XEResult::NullObj;
	}

	//Starting after clip stopped
	if(!m_Playing)
	{
		RebootVariables();
	}
	else
	{
		//Resuming after pause
		m_Paused = false;
	}

	return XEResult::Ok;
}

XEResult AnimationPlayer::PauseClip()
{
	if(m_CurrentAnimation == nullptr)
	{
		return XEResult::NullObj;
	}

	m_Paused = true;

	return XEResult::Ok;
}

XEResult AnimationPlayer::BlendAnimtion(Animation* animClip, float blendTime, bool newAnimOnLoop, AnimationBlendType blendType)
{
	if(animClip == nullptr)
	{
		return XEResult::NullParameter;
	}

	if(m_CurrentAnimation == nullptr)
	{
		return XEResult::NullObj;
	}

	m_Playing = true;

	m_BlenAnimToLoop = newAnimOnLoop;

	m_BlendingAnimation = animClip;

	m_TotalBlendTime = blendTime;

	m_BlendType = blendType;

	m_IsBlending = true;
	
	std::fill_n(bonePosesBlending, XE_MAX_BONES, BonePose());

	return XEResult::Ok;
}

XEResult AnimationPlayer::Update(const TimerParams& timerParams, const glm::mat4& world)
{
	std::lock_guard<std::mutex> lock(m_AnimationPlayerMutex);

	if (m_Skeleton == nullptr)
	{
		XETODO("Add Skeleton Null to return");
		return XEResult::Fail;
	}

	if(m_CurrentAnimation == nullptr)
	{
		return XEResult::Ok;
	}

	//If it is stop or in pause do nothing
	if(!m_Playing || m_Paused)
	{
		return XEResult::Ok;
	}

	XEResult ret = XEResult::Ok;

	float elapsedTime = (float)timerParams.m_ElapsedTime;

	m_CurrentAnimation->Update(timerParams);

	if(m_IsBlending)
	{
		if(m_BlendingAnimation == nullptr)
		{
			return XEResult::Ok;
		}

		m_BlendingAnimation->Update(timerParams);

		ret = UpdateBaseTransformsBlending(elapsedTime);
		if(ret != XEResult::Ok)
		{
			return ret;
		}
	}
	else
	{
		ret = UpdateBaseTransforms(elapsedTime);
		if(ret != XEResult::Ok)
		{
			return ret;
		}
	}

	ret = UpdateWorldTransforms(world);
	if(ret != XEResult::Ok)
	{
		return ret;
	}

	ret = UpdateBoneTransforms();
	if(ret != XEResult::Ok)
	{
		return ret;
	}

	m_CurrentAnimation->CheckCallBacks(m_CurrentKeyFrame);

	if(m_IsBlending)
	{
		m_BlendingAnimation->CheckCallBacks(m_CurrentKeyFrameBlend);
	}

	return XEResult::Ok;
}

void AnimationPlayer::SetSkeleton(const Skeleton* skeleton)
{
	std::lock_guard<std::mutex> lock(m_AnimationPlayerMutex);

	m_Skeleton = skeleton;
}

bool AnimationPlayer::UsingAnimationClip(const Animation* animationClip) const
{
	if (animationClip == nullptr)
	{
		return false;
	}

	bool usingAnim = false;

	if (m_CurrentAnimation != nullptr && m_CurrentAnimation->GetUniqueID() == animationClip->GetUniqueID())
	{
		return true;
	}

	if (m_BlendingAnimation != nullptr && m_BlendingAnimation->GetUniqueID() == animationClip->GetUniqueID())
	{
		return true;
	}

	return false;
}
