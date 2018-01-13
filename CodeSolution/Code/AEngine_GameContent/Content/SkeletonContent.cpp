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

/**********************
*   System Includes   *
***********************/

/*************************
*   3rd Party Includes   *
**************************/

/***************************
*   Game Engine Includes   *
****************************/
#include "SkeletonContent.h"
#include "Content\ContentDefs.h"
#include "Models\Skinning\Bone.h"
#include "Models\Skinning\SkinningDefs.h"

//Always include last
#include "Memory\MemLeaks.h"

/********************
*   Function Defs   *
*********************/
SkeletonContent::SkeletonContent()
{
	m_DummyRootBone = new TreeBone();
}

SkeletonContent::~SkeletonContent()
{
	m_BoneHierarchy.clear();

	CleanUpTreeBone(m_DummyRootBone);

	m_DummyRootBone = nullptr;
}

void SkeletonContent::CleanUpTreeBone(TreeBone* childBone)
{
	if(childBone == nullptr)
	{
		return;
	}
	
	for(auto itBones : childBone->m_ChildBones)
	{
		CleanUpTreeBone(itBones);
	}

	childBone->m_ChildBones.clear();

	DeleteMem(childBone);
}

AEResult SkeletonContent::FlattenSkeleton()
{
	m_BoneHierarchy.clear();
	
	for(auto itBones : m_DummyRootBone->m_ChildBones)
	{
		FlattenSkeleton(itBones, -1);
	}
	
	uint32_t size = (uint32_t)m_BoneHierarchy.size();

	AETODO("Add warning if skeleton bone passes 96");
	AEAssert(size <= 96);

	return AEResult::Ok;
}
	
AEResult SkeletonContent::FlattenSkeleton(TreeBone* treeBone, int32_t parentIndex)
{
	treeBone->m_Bone->SetParentIndex(parentIndex);
	m_BoneHierarchy.push_back(treeBone->m_Bone);

	int32_t index = (int32_t)(m_BoneHierarchy.size() - 1);
	treeBone->m_Bone->SetIndex(index);
	
	for(auto itBones : treeBone->m_ChildBones)
	{
		FlattenSkeleton(itBones, index);
	}

	return AEResult::Ok;
}
