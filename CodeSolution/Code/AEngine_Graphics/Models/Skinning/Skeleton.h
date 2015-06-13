/********************************************************
*
* Author: Carlos Chacón N.
*
* Copyright 2012-2015
*
*********************************************************/

#pragma once

#ifndef _SKELETON_H
#define _SKELETON_H

/**********************
*   System Includes   *
***********************/
#include <vector>
#include <stdint.h>

/*************************
*   3rd Party Includes   *
**************************/

/***************************
*   Game Engine Includes   *
****************************/
#include "Base\Base.h"
#include "Resource\GameResource.h"

/************
*   Using   *
*************/

/********************
*   Forward Decls   *
*********************/
class Bone;

/*****************
*   Class Decl   *
******************/
class Skeleton sealed : public GameResource
{
	private:
		std::vector<Bone*> m_BoneHierarchy;

		void CleanUp();

		virtual ~Skeleton();

	public:

		/// <summary>
		/// Skeleton Constructor
		/// </summary>
		/// <param name="resourceName">Resource Name</param>
		Skeleton(const std::wstring& resourceName);

		inline uint32_t GetSkeletonSize() const
		{
			return (uint32_t)m_BoneHierarchy.size();
		}
		
		AEResult Load() override;

		Bone* GetBone(uint32_t index) const;

		Bone* operator[](uint32_t index) const;
		
		AEResult AddBone(Bone* bone);
};

#endif