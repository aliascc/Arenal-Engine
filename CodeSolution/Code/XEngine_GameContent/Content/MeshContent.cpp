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
#include "MeshContent.h"
#include "MeshPartContent.h"

//Always include last
#include "Memory\MemLeaks.h"

/********************
*   Function Defs   *
*********************/
MeshContent::MeshContent()
{
}

MeshContent::~MeshContent()
{
	uint32_t size = (uint32_t)m_MeshParts.size();
	for(uint32_t i = 0; i < size; ++i)
	{
		DeleteMem(m_MeshParts[i]);
	}

	m_MeshParts.clear();
}