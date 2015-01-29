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

/*************************
*   3rd Party Includes   *
**************************/

/***************************
*   Game Engine Includes   *
****************************/
#include "MeshContent.h"
#include "ModelContent.h"
#include "SkeletonContent.h"
#include "AnimationContent.h"

//Always include last
#include "Memory\MemLeaks.h"

/********************
*   Function Defs   *
*********************/
ModelContent::ModelContent()
{
	m_SkeletonContent = new SkeletonContent();
}

ModelContent::~ModelContent()
{
	//Clean Skeleton
	DeleteMem(m_SkeletonContent);

	//Clean Animations
	for(auto itAnim : m_Animations)
	{
		DeleteMem(itAnim.second);
	}

	m_Animations.clear();

	//Clean Meshes
	for(auto itMesh : m_Meshes)
	{
		DeleteMem(itMesh.second);
	}

	m_Meshes.clear();
}
