/********************************************************
*
* Author: Carlos Chacón N.
*
* Copyright 2012-2015
*
*********************************************************/

#pragma once

#ifndef _KEY_FRAME_H
#define _KEY_FRAME_H

/**********************
*   System Includes   *
***********************/
#include <stdint.h>

/*************************
*   3rd Party Includes   *
**************************/

/***************************
*   Game Engine Includes   *
****************************/
#include "Base\Base.h"
#include "Base\AEObject.h"
#include "Models\Skinning\SkinningDefs.h"

/********************
*   Forward Decls   *
*********************/

/*****************
*   Class Decl   *
******************/
class KeyFrame sealed : public AEObject
{
	private:
		BonePose m_BonePose;

		int32_t m_BoneIndex = -1;

		float m_Time = 0.0f;

	public:
		KeyFrame();
		KeyFrame(int32_t boneIndex, float time, const BonePose& bonePose);
		~KeyFrame();

		inline void SetTime(float time)
		{
			m_Time = time;
		}

		inline void SetBonePose(const BonePose& bonePose)
		{
			m_BonePose = bonePose;
		}

		inline void SetBoneIndex(int32_t boneIndex)
		{
			m_BoneIndex = boneIndex;
		}

		inline float GetTime() const
		{
			return m_Time;
		}

		inline int32_t GetBoneIndex() const
		{
			return m_BoneIndex;
		}

		inline const BonePose& GetBonePose() const
		{
			return m_BonePose;
		}

		bool operator<(const KeyFrame& keyframe) const;
};

#endif

