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

#ifndef _ANIMATION_H
#define _ANIMATION_H

/**********************
*   System Includes   *
***********************/
#include <vector>
#include <string>

/*************************
*   3rd Party Includes   *
**************************/

/***************************
*   Game Engine Includes   *
****************************/
#include "KeyFrame.h"
#include "Base\Base.h"
#include "AnimationDefs.h"
#include "Resource\GameResource.h"

/********************
*   Forward Decls   *
*********************/
struct TimerParams;

/*****************
*   Class Decl   *
******************/
class Animation sealed : public GameResource
{
	private:

		std::vector<AnimationCallBack> m_AnimationCallbacks;

		std::vector<KeyFrame> m_KeyFrames;

		uint32_t m_LastKeyFrameCheck = 0;

		float m_Duration = 0.0f;

		bool m_NeedSort = false;
		
		void CleanUp();
		
		void SortKeyFrames();

		virtual ~Animation();

	public:

		/// <summary>
		/// Animation Constructor
		/// </summary>
		/// <param name="resourceName">Resource Name</param>
		Animation(const std::wstring& resourceName);

		inline float GetDuration() const
		{
			return m_Duration;
		}

		inline uint32_t GetFrameCount() const
		{
			return (uint32_t)m_KeyFrames.size();
		}

		AEResult Load() override;

		AEResult CheckCallBacks(uint32_t currentKeyFrame);

		AEResult AddAnimationCallBack(float time, AnimationCallBackFunc animCallBack);

		AEResult AddKeyFrame(const KeyFrame& keyFrame);
		
		void Update(const TimerParams& timerParams); 

		const KeyFrame& GetKeyFrame(uint32_t index);

		const KeyFrame& operator[](uint32_t index);

		// Prevent copying.
		Animation(Animation const&) = delete;
		Animation& operator= (Animation const&) = delete;
};

#endif

