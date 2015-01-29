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
#include "LightShape.h"
#include "GraphicDevice.h"
#include "Math\XEMathDefs.h"
#include "Vertex\IndexBuffer.h"
#include "Base\BaseFunctions.h"
#include "Vertex\IVertexBuffer.h"

//Always include last
#include "Memory\MemLeaks.h"

/********************
*   Function Defs   *
*********************/
LightShape::LightShape(GraphicDevice* graphicDevice)
	: m_GraphicDevice(graphicDevice)
{
	XEAssert(m_GraphicDevice != nullptr);
}

LightShape::~LightShape()
{
	CleanUp();
}

void LightShape::CleanUp()
{
	for (size_t i = 0; i < m_VertexBufferVector.size(); i++)
	{
		DeleteMem(m_VertexBufferVector[i]);
	}

	m_VertexBufferVector.clear();
}

XEResult LightShape::DrawLightShape()
{
	if(!m_IsReady)
	{
		return XEResult::NotReady;
	}
	
	XETODO("add better return codes");
	for (size_t i = 0; i < m_VertexBufferVector.size(); i++)
	{
		if (m_GraphicDevice->SetVertexBuffer(m_VertexBufferVector[i]) != XEResult::Ok)
		{
			return XEResult::Fail;
		}

		if (m_GraphicDevice->SetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST) != XEResult::Ok)
		{
			return XEResult::Fail;
		}

		if (m_GraphicDevice->Draw(m_VertexBufferVector[i]->GetSize(), 0) != XEResult::Ok)
		{
			return XEResult::Fail;
		}
	}

	return XEResult::Ok;
}
