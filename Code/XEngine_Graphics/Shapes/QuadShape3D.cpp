/********************************************************
*
* Author: Carlos Chacón N.
*
* Created: 2/7/2014
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

/***************************
*   Game Engine Includes   *
****************************/
#include "QuadShape3D.h"
#include "GraphicDevice.h"
#include "Math\XEMathDefs.h"
#include "Vertex\IndexBuffer.h"
#include "Base\BaseFunctions.h"

//Always include last
#include "Memory\MemLeaks.h"

/********************
*   Function Defs   *
*********************/
QuadShape3D::QuadShape3D(GraphicDevice* graphicDevice, bool clockWise)
	: QuadShape(graphicDevice, clockWise)
{
}

QuadShape3D::~QuadShape3D()
{
}

XEResult QuadShape3D::BuildQuad()
{
	XEAssert(m_GraphicDevice != nullptr);
	if (m_GraphicDevice == nullptr)
	{
		return XEResult::GraphicDeviceNull;
	}

	m_IsReady = false;
	DeleteMem(m_IB);
	DeleteMem(m_VB);

	//Clean Vertex Size
	memset(m_Vtx, 0, sizeof(VertexPositionTexture)* 4);
	
	//Top Left
	m_Vtx[0].m_Position.x = -0.5f;
	m_Vtx[0].m_Position.y = 0.5f;
	m_Vtx[0].m_Position.z = 0.0f;
	m_Vtx[0].m_TexCoord.x = 0.0f;
	m_Vtx[0].m_TexCoord.y = 0.0f;
	
	//Top Right
	m_Vtx[1].m_Position.x = 0.5f;
	m_Vtx[1].m_Position.y = 0.5f;
	m_Vtx[1].m_Position.z = 0.0f;
	m_Vtx[1].m_TexCoord.x = 1.0f;
	m_Vtx[1].m_TexCoord.y = 0.0f;
	
	//Bottom Right
	m_Vtx[2].m_Position.x = 0.5f;
	m_Vtx[2].m_Position.y = -0.5f;
	m_Vtx[2].m_Position.z = 0.0f;
	m_Vtx[2].m_TexCoord.x = 1.0f;
	m_Vtx[2].m_TexCoord.y = 1.0f;
	
	//Bottom Left
	m_Vtx[3].m_Position.x = -0.5f;
	m_Vtx[3].m_Position.y = -0.5f;
	m_Vtx[3].m_Position.z = 0.0f;
	m_Vtx[3].m_TexCoord.x = 0.0f;
	m_Vtx[3].m_TexCoord.y = 1.0f;

	GraphicBufferUsage usage = GraphicBufferUsage::Static;
	GraphicBufferAccess access = GraphicBufferAccess::None;

	m_VB = new VertexBuffer<VertexPositionTexture>(m_GraphicDevice, usage, access);
	m_VB->CopyVertexBuffer(m_Vtx, 4);
	m_VB->BuildVertexBuffer();

	uint16_t idxs[6];

	if(m_ClockWise)
	{
		idxs[0] = 0;
		idxs[1] = 1;
		idxs[2] = 2;
		idxs[3] = 2;
		idxs[4] = 3;
		idxs[5] = 0;
	}
	else
	{
		idxs[0] = 0;
		idxs[1] = 3;
		idxs[2] = 2;
		idxs[3] = 2;
		idxs[4] = 1;
		idxs[5] = 0;
	}

	m_IB = new IndexBuffer(m_GraphicDevice);
	m_IB->CopyToIndexBuffer(idxs, 6);
	m_IB->BuildIndexBuffer();

	m_IsReady = true;

	return XEResult::Ok;
}