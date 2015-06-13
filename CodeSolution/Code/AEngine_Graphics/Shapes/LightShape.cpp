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
#include "LightShape.h"
#include "GraphicDevice.h"
#include "Math\AEMathDefs.h"
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
	AEAssert(m_GraphicDevice != nullptr);
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

AEResult LightShape::DrawLightShape()
{
	if(!m_IsReady)
	{
		return AEResult::NotReady;
	}
	
	AETODO("add better return codes");
	for (size_t i = 0; i < m_VertexBufferVector.size(); i++)
	{
		if (m_GraphicDevice->SetVertexBuffer(m_VertexBufferVector[i]) != AEResult::Ok)
		{
			return AEResult::Fail;
		}

		if (m_GraphicDevice->SetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST) != AEResult::Ok)
		{
			return AEResult::Fail;
		}

		if (m_GraphicDevice->Draw(m_VertexBufferVector[i]->GetSize(), 0) != AEResult::Ok)
		{
			return AEResult::Fail;
		}
	}

	return AEResult::Ok;
}
