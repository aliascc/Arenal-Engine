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
#include "GraphicDevice.h"
#include "OmniLightShape.h"
#include "Math\XEMathDefs.h"
#include "Base\BaseFunctions.h"
#include "Vertex\VertexBuffer.h"
#include "Vertex\Types\VertexPosition.h"

//Always include last
#include "Memory\MemLeaks.h"

/********************
*   Function Defs   *
*********************/
OmniLightShape::OmniLightShape(GraphicDevice* graphicDevice, uint32_t numEdges)
	: LightShape(graphicDevice)
	, m_NumEdges(numEdges)
{
}

OmniLightShape::~OmniLightShape()
{
}

XEResult OmniLightShape::BuildLightShape()
{
	///////////////////////////////////////////////////
	//Pre-checks
	XEAssert(m_GraphicDevice != nullptr);
	if (m_GraphicDevice == nullptr)
	{
		return XEResult::GraphicDeviceNull;
	}

	XEAssert(m_NumEdges != 0);
	if (m_NumEdges == 0)
	{
		return XEResult::ZeroSize;
	}

	///////////////////////////////////////////////////
	//Delete Current and set ready to false
	m_IsReady = false;
	CleanUp();

	///////////////////////////////////////////////////
	//Variables 
	XEResult ret = XEResult::Ok;
	uint32_t sizeOfVtx = m_NumEdges * 2;

	VertexPosition* vtxH = new VertexPosition[sizeOfVtx];
	VertexPosition* vtxV = new VertexPosition[sizeOfVtx];

	memset(vtxH, 0, sizeof(VertexPosition) * sizeOfVtx);
	memset(vtxV, 0, sizeof(VertexPosition) * sizeOfVtx);

	///////////////////////////////////////////////////
	//Init variable info
	float edgesFloat = static_cast<float>(m_NumEdges);
	float edgesHalf = edgesFloat / 2.0f;
	float radius = m_Size / 2.0f;

	float radiusRingRad = glm::radians(180.0f * edgesHalf) / edgesFloat;
	float radiusRing = radius * glm::sin(radiusRingRad);

	///////////////////////////////////////////////////
	//Construct circle
	uint32_t indexVtx = 0;
	for (float i = 0; i < m_NumEdges; i++, indexVtx += 2)
	{		
		//Calculate Point A
		vtxV[indexVtx].m_Position.x = vtxH[indexVtx].m_Position.x = radiusRing * glm::cos(glm::radians((360.0f * i) / edgesFloat));
		vtxV[indexVtx].m_Position.z = vtxH[indexVtx].m_Position.y = radius * glm::cos(glm::radians(180.0f * edgesHalf) / edgesFloat);
		vtxV[indexVtx].m_Position.y = vtxH[indexVtx].m_Position.z = radiusRing * glm::sin(glm::radians((360.0f * i) / edgesFloat));

		//Calculate Point B
		vtxV[indexVtx + 1].m_Position.x = vtxH[indexVtx + 1].m_Position.x = radiusRing * glm::cos(glm::radians((360.0f * (i + 1)) / edgesFloat));
		vtxV[indexVtx + 1].m_Position.z = vtxH[indexVtx + 1].m_Position.y = radius * glm::cos(glm::radians(180.0f * edgesHalf) / edgesFloat);
		vtxV[indexVtx + 1].m_Position.y = vtxH[indexVtx + 1].m_Position.z = radiusRing * glm::sin(glm::radians((360.0f * (i + 1)) / edgesFloat));
	}

	///////////////////////////////////////////////////
	//Create Vertex Buffers
	VertexBuffer<VertexPosition>* vertexBufferHorizontal = new VertexBuffer<VertexPosition>(m_GraphicDevice);
	VertexBuffer<VertexPosition>* vertexBufferVertical = new VertexBuffer<VertexPosition>(m_GraphicDevice);

	if (vertexBufferHorizontal->CopyVertexBuffer(vtxH, sizeOfVtx) != XEResult::Ok || vertexBufferVertical->CopyVertexBuffer(vtxV, sizeOfVtx) != XEResult::Ok)
	{
		XETODO("Better return message");
		ret = XEResult::Fail;
	}

	DeleteMemArr(vtxH);
	DeleteMemArr(vtxV);

	if (ret != XEResult::Ok)
	{
		DeleteMem(vertexBufferHorizontal);
		DeleteMem(vertexBufferVertical);
		return ret;
	}

	///////////////////////////////////////////////////
	//Add to Vector
	m_VertexBufferVector.push_back(vertexBufferHorizontal);
	m_VertexBufferVector.push_back(vertexBufferVertical);

	///////////////////////////////////////////////////
	//Finish
	m_IsReady = true;

	return XEResult::Ok;
}
