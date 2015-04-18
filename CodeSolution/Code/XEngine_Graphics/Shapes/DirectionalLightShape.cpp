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
#include "GraphicDevice.h"
#include "Math\XEMathDefs.h"
#include "Base\BaseFunctions.h"
#include "Vertex\VertexBuffer.h"
#include "DirectionalLightShape.h"
#include "Vertex\Types\VertexPosition.h"

//Always include last
#include "Memory\MemLeaks.h"

/********************
*   Function Defs   *
*********************/
DirectionalLightShape::DirectionalLightShape(GraphicDevice* graphicDevice)
	: LightShape(graphicDevice)
{
}

DirectionalLightShape::~DirectionalLightShape()
{
}

XEResult DirectionalLightShape::BuildLightShape()
{
	///////////////////////////////////////////////////
	//Pre-checks
	XEAssert(m_GraphicDevice != nullptr);
	if (m_GraphicDevice == nullptr)
	{
		return XEResult::GraphicDeviceNull;
	}

	///////////////////////////////////////////////////
	//Delete Current and set ready to false
	m_IsReady = false;
	CleanUp();

	///////////////////////////////////////////////////
	//Variables 

	//Size if Calculate as 4 Lines for Arrow Tip and 1 for Arrow Line
	//For total of 10 Vertexes
	const uint32_t sizeOfVtx = 10;

	VertexPosition vtxs[sizeOfVtx];
	memset(vtxs, 0, sizeof(VertexPosition)* sizeOfVtx);

	///////////////////////////////////////////////////
	//Init Vertexs

	//Arrow Line
	vtxs[0].m_Position = XEMathHelpers::Vec3fZero;
	vtxs[1].m_Position = glm::vec3(0.0f, 0.0f, 1.0f);

	//Arrow Tip

	// #1
	vtxs[2].m_Position = glm::vec3(0.0f, 0.0f, XE_LIGHT_SHAPE_DIR_DEFAULT_ARROW_SIZE);
	vtxs[3].m_Position = glm::vec3(0.0f, XE_LIGHT_SHAPE_DIR_DEFAULT_ARROW_TIP_HEIGHT_SIZE, XE_LIGHT_SHAPE_DIR_DEFAULT_ARROW_SIZE - XE_LIGHT_SHAPE_DIR_DEFAULT_ARROW_TIP_SIZE);

	// #2
	vtxs[4].m_Position = glm::vec3(0.0f, 0.0f, XE_LIGHT_SHAPE_DIR_DEFAULT_ARROW_SIZE);
	vtxs[5].m_Position = glm::vec3(0.0f, -XE_LIGHT_SHAPE_DIR_DEFAULT_ARROW_TIP_HEIGHT_SIZE, XE_LIGHT_SHAPE_DIR_DEFAULT_ARROW_SIZE - XE_LIGHT_SHAPE_DIR_DEFAULT_ARROW_TIP_SIZE);

	// #3
	vtxs[6].m_Position = glm::vec3(0.0f, 0.0f, XE_LIGHT_SHAPE_DIR_DEFAULT_ARROW_SIZE);
	vtxs[7].m_Position = glm::vec3(XE_LIGHT_SHAPE_DIR_DEFAULT_ARROW_TIP_HEIGHT_SIZE, 0.0f, XE_LIGHT_SHAPE_DIR_DEFAULT_ARROW_SIZE - XE_LIGHT_SHAPE_DIR_DEFAULT_ARROW_TIP_SIZE);

	// #4
	vtxs[8].m_Position = glm::vec3(0.0f, 0.0f, XE_LIGHT_SHAPE_DIR_DEFAULT_ARROW_SIZE);
	vtxs[9].m_Position = glm::vec3(-XE_LIGHT_SHAPE_DIR_DEFAULT_ARROW_TIP_HEIGHT_SIZE, 0.0f, XE_LIGHT_SHAPE_DIR_DEFAULT_ARROW_SIZE - XE_LIGHT_SHAPE_DIR_DEFAULT_ARROW_TIP_SIZE);

	///////////////////////////////////////////////////
	//Create Vertex Buffer
	VertexBuffer<VertexPosition>* vertexBuffer = new VertexBuffer<VertexPosition>(m_GraphicDevice);

	if (vertexBuffer->CopyVertexBuffer(vtxs, sizeOfVtx) != XEResult::Ok)
	{
		XETODO("Better return message");
		return XEResult::Fail;
	}

	///////////////////////////////////////////////////
	//Add to Vector
	m_VertexBufferVector.push_back(vertexBuffer);

	///////////////////////////////////////////////////
	//Finish
	m_IsReady = true;

	return XEResult::Ok;
}
