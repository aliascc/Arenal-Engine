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

#ifndef _ANIMATION_PLAYER_H
#define _ANIMATION_PLAYER_H

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

