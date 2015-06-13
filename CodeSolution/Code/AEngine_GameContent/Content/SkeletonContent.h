/********************************************************
*
* Author: Carlos Chacón N.
*
* Copyright 2012-2015
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
#include "Base\AEObject.h"

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
	AEResult FlattenSkeleton(TreeBone* treeBone, int32_t parentIndex);

	SkeletonContent();
	virtual ~SkeletonContent();

	AEResult FlattenSkeleton();

	uint32_t GetFlattenSkeletonSize() const
	{
		return (uint32_t)m_BoneHierarchy.size();
	}
};

#endif