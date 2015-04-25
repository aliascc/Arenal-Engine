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

/***************************
*   Game Engine Includes   *
****************************/
#include "QuadShape.h"
#include "GraphicDevice.h"
#include "Math\XEMathDefs.h"
#include "Vertex\IndexBuffer.h"
#include "Base\BaseFunctions.h"

//Always include last
#include "Memory\MemLeaks.h"

/********************
*   Function Defs   *
*********************/
QuadShape::QuadShape(GraphicDevice* graphicDevice, bool clockWise)
	: m_ClockWise(clockWise)
	, m_GraphicDevice(graphicDevice)
{
	XEAssert(m_GraphicDevice != nullptr);
}

QuadShape::~QuadShape()
{
	DeleteMem(m_IB);
	DeleteMem(m_VB);
}

XEResult QuadShape::DrawQuad()
{
	if(!m_IsReady)
	{
		return XEResult::NotReady;
	}

	XETODO("add better return codes");
	if(m_GraphicDevice->SetVertexBuffer(m_VB) != XEResult::Ok)
	{
		return XEResult::Fail;
	}

	if(m_GraphicDevice->SetIndexBuffer(m_IB) != XEResult::Ok)
	{
		return XEResult::Fail;
	}

	if(m_GraphicDevice->SetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST) != XEResult::Ok)
	{
		return XEResult::Fail;
	}

	if(m_GraphicDevice->DrawIndexed(0, 0, m_IB->GetSize()) != XEResult::Ok)
	{
		return XEResult::Fail;
	}

	return XEResult::Ok;
}
