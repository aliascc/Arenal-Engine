
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

#pragma once

#ifndef _SKELETON_CONTENT_H
#define _SKELETON_CONTENT_H

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
#include "Base\XEObject.h"

/********************
*   Forward Decls   *
*********************/
class Bone;
struct TreeBone;

/******************
*   Struct Decl   *
*******************/
struct SkeletonContent sealed : public GameContent
{
	std::vector<Bone*> m_BoneHierarchy;
	TreeBone* m_DummyRootBone = nullptr;
	
	void CleanUpTreeBone(TreeBone* childBone);
	XEResult FlattenSkeleton(TreeBone* treeBone, int32_t parentIndex);

	SkeletonContent();
	virtual ~SkeletonContent();

	XEResult FlattenSkeleton();

	uint32_t GetFlattenSkeletonSize() const
	{
		return (uint32_t)m_BoneHierarchy.size();
	}
};

#endif