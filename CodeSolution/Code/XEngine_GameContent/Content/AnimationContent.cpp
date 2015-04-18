/********************************************************
*
* Author: Carlos Chacón N.
*
* Created: 1/30/2013
*
* Desc:
*
*
* Copyright 2012
*
*********************************************************/

/**********************
*   System Includes   *
***********************/
#include <algorithm>

/*************************
*   3rd Party Includes   *
**************************/

/***************************
*   Game Engine Includes   *
****************************/
#include "AnimationContent.h"

//Always include last
#include "Memory\MemLeaks.h"

/********************
*   Function Defs   *
*********************/
AnimationContent::AnimationContent()
{
}

AnimationContent::~AnimationContent()
{
}

void AnimationContent::SortKeyFrames()
{
	std::sort(m_KeyFrames.begin(), m_KeyFrames.end());
}

