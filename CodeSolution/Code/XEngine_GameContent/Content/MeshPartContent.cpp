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
