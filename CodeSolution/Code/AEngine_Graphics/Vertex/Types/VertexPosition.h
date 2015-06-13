/********************************************************
*
* Author: Carlos Chacón N.
*
* Copyright 2012-2015
*
*********************************************************/

#pragma once

#ifndef _VERTEX_POSITION_H
#define _VERTEX_POSITION_H

/**********************
*   System Includes   *
***********************/
#include <string>
#include <stdint.h>
#include <d3d11_1.h>

/*************************
*   3rd Party Includes   *
**************************/
#include "glm\glm.hpp"

/***************************
*   Game Engine Includes   *
****************************/
#include "Base\Base.h"
#include "Math\AEMathDefs.h"
#include "Vertex\VertexDefs.h"
#include "Vertex\VertexLayout.h"


#if defined(DEBUG) | defined(_DEBUG)
	#if defined(_WIN64) | defined (WIN64) 
#include "Compiled Materials\HLSL\DummyVertexPosition_x64_d.h"		
	#else
#include "Compiled Materials\HLSL\DummyVertexPosition_x86_d.h"
	#endif
#else
	#if defined(_WIN64) | defined (WIN64) 
#include "Compiled Materials\HLSL\DummyVertexPosition_x64.h"
	#else
#include "Compiled Materials\HLSL\DummyVertexPosition_x86.h"
	#endif
#endif

/************
*   Using   *
*************/

/********************
*   Forward Decls   *
*********************/
class GraphicDevice;

/******************
*   Struct Decl   *
*******************/
struct VertexPosition
{
	//Position
	glm::vec3 m_Position = AEMathHelpers::Vec3fZero;

	//Vertex Layout
	static VertexLayout* m_VertexLayout;

	//Get Vertex Declaration
	static const VertexLayout* GetVertexLayout(GraphicDevice* graphicDevice)
	{
		if(m_VertexLayout != nullptr)
		{
			return m_VertexLayout;
		}
		
		D3D11_INPUT_ELEMENT_DESC vertexDesc[] =
		{
			{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0}
		};

		m_VertexLayout = new VertexLayout();

		AEResult ret = m_VertexLayout->BuildVertexLayout(graphicDevice, DummyVertexPosition, sizeof(DummyVertexPosition), vertexDesc, 1, L"VertexPositionLayout");

		if(ret != AEResult::Ok)
		{
			DeleteMem(m_VertexLayout);
		}

		return m_VertexLayout;
	}

	//Delete Vertex Layout
	static void DeleteVertexLayout()
	{
		DeleteMem(m_VertexLayout);
	}

	static VertexType GetVertexType()
	{
		return VertexType::VtxPos;
	}

	static uint32_t VertexSize()
	{
		return sizeof(VertexPosition);
	}
};

#endif