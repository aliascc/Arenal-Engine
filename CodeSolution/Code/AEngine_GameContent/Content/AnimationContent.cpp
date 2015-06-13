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

