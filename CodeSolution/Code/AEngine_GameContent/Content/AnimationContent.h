/********************************************************
*
* Author: Carlos Chacón N.
*
* Copyright 2012-2015
*
*********************************************************/

#pragma once

#ifndef _ANIMATION_CONTENT_H
#define _ANIMATION_CONTENT_H

/**********************
*   System Includes   *
***********************/
#include <vector>

/*************************
*   3rd Party Includes   *
**************************/

/***************************
*   Game Engine Includes   *
****************************/
#include "Base\Base.h"
#include "GameContent.h"
#include "Base\AEObject.h"
#include "Models\Animation\KeyFrame.h"

/************
*   Using   *
*************/

/********************
*   Forward Decls   *
*********************/

/******************
*   Struct Decl   *
*******************/
struct AnimationContent sealed : public GameContent
{
	std::wstring m_Name = L"";

	std::vector<KeyFrame> m_KeyFrames;
	
	float m_Duration = 0.0f;

	AnimationContent();
	virtual ~AnimationContent();
	
	void SortKeyFrames();
};

#endif