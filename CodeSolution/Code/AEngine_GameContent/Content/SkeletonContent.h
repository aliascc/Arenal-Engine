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