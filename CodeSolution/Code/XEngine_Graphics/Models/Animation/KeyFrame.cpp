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

//Always include last
#include "Memory\MemLeaks.h"

/********************
*   Function Defs   *
*********************/
KeyFrame::KeyFrame()
{
}

KeyFrame::KeyFrame(int32_t boneIndex, float time, const BonePose& bonePose)
	: m_BoneIndex(boneIndex)
	, m_Time(time)
	, m_BonePose(bonePose)
{
}

KeyFrame::~KeyFrame()
{
}

bool KeyFrame::operator<(const KeyFrame& keyframe) const
{
	return (m_Time < keyframe.m_Time);
}
