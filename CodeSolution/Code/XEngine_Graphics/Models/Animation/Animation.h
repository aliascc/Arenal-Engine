
/********************************************************
*
* Author: Carlos Chacón N.
*
* Created: 12/31/2012
*
* Desc:
*
*
* Copyright 2012
*
*********************************************************/

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

		XEResult Load() override;

		XEResult CheckCallBacks(uint32_t currentKeyFrame);

		XEResult AddAnimationCallBack(float time, AnimationCallBackFunc animCallBack);

		XEResult AddKeyFrame(const KeyFrame& keyFrame);
		
		void Update(const TimerParams& timerParams); 

		const KeyFrame& GetKeyFrame(uint32_t index);

		const KeyFrame& operator[](uint32_t index);

		// Prevent copying.
		Animation(Animation const&) = delete;
		Animation& operator= (Animation const&) = delete;
};

#endif

