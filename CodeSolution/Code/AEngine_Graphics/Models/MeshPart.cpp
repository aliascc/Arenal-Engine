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
#include "MeshPart.h"
#include "GraphicDevice.h"
#include "Vertex\VertexDefs.h"
#include "Base\BaseFunctions.h"
#include "Vertex\IndexBuffer.h"
#include "Vertex\VertexBuffer.h"
#include "Vertex\IVertexBuffer.h"
#include "Vertex\Types\VertexPositionNormalColor.h"
#include "Vertex\Types\VertexPositionNormalTexture.h"
#include "Vertex\Types\VertexPositionNormalTextureTexture2.h"
#include "Vertex\Types\VertexPositionNormalTangentBinormalTexture.h"
#include "Vertex\Types\VertexPositionNormalTangentBinormalTextureTexture2.h"

//Always include last
#include "Memory\MemLeaks.h"

/********************
*   Function Defs   *
*********************/
MeshPart::MeshPart()
{
}

MeshPart::~MeshPart()
{
	DeleteMem(m_IndexBuffer);
	DeleteMem(m_VertexBuffer);
}

void MeshPart::SetVertexBuffer(IVertexBuffer* vb)
{
	AEAssert(vb != nullptr);

	if(vb == nullptr)
	{
		return;
	}

	DeleteMem(m_VertexBuffer);

	m_VertexBuffer = vb;
}

void MeshPart::SetIndexBuffer(IndexBuffer* ib)
{
	AEAssert(ib != nullptr);

	if(ib == nullptr)
	{
		return;
	}

	DeleteMem(m_IndexBuffer);

	m_IndexBuffer = ib;
}