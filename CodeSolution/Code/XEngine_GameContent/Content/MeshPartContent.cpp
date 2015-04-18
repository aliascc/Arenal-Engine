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
#include "MeshPartContent.h"
#include "VertexBufferContent.h"

//Always include last
#include "Memory\MemLeaks.h"

/********************
*   Function Defs   *
*********************/
MeshPartContent::MeshPartContent()
{
}

MeshPartContent::~MeshPartContent()
{
	DeleteMem(m_IdxBuf16);
	DeleteMem(m_VtxBuff);
}
