/********************************************************
*
* Author: Carlos Chacón N.
*
* Copyright 2012-2015
*
*********************************************************/

#pragma once

#ifndef _MESH_PART_CONTENT_H
#define _MESH_PART_CONTENT_H

/**********************
*   System Includes   *
***********************/
#include <stdint.h>

/*************************
*   3rd Party Includes   *
**************************/

/***************************
*   Game Engine Includes   *
****************************/
#include "Base\Base.h"
#include "GameContent.h"
#include "Base\AEObject.h"
#include "Vertex\VertexDefs.h"

/************
*   Using   *
*************/

/********************
*   Forward Decls   *
*********************/
struct IVertexBufferContent;

/******************
*   Struct Decl   *
*******************/
struct MeshPartContent sealed : public GameContent
{
	IVertexBufferContent* m_VtxBuff = nullptr;

	uint32_t m_MatID = 0;

	VertexType m_VertexType = VertexType::VtxNone;

	uint16_t* m_IdxBuf16 = nullptr;

	uint32_t m_IdxSize = 0;

	MeshPartContent();
	virtual ~MeshPartContent();
};

#endif