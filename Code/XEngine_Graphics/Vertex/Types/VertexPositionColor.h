
/********************************************************
*
* Author: Carlos Chacón N.
*
* Created: 2/21/2012
*
* Desc:
*
*
* Copyright 2012
*
*********************************************************/
#pragma once

#ifndef _VERTEX_POSITION_COLOR_H
#define _VERTEX_POSITION_COLOR_H

/**********************
*   System Includes   *
***********************/
#include <string>
#include <stdint.h>

/*************************
*   3rd Party Includes   *
**************************/
#include "glm\glm.hpp"

/***************************
*   Game Engine Includes   *
****************************/
#include "Base\Base.h"
#include "Color\Color.h"
#include "Color\XEColorDefs.h"
#include "Math\XEMathDefs.h"
#include "Vertex\VertexDefs.h"
#include "Vertex\VertexLayout.h"

#if defined(DEBUG) | defined(_DEBUG)
	#if defined(_WIN64) | defined (WIN64) 
#include "Compiled Materials\HLSL\DummyVertexPositionColor_x64_d.h"		
	#else
#include "Compiled Materials\HLSL\DummyVertexPositionColor_x86_d.h"
	#endif
#else
	#if defined(_WIN64) | defined (WIN64) 
#include "Compiled Materials\HLSL\DummyVertexPositionColor_x64.h"
	#else
#include "Compiled Materials\HLSL\DummyVertexPositionColor_x86.h"
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
struct VertexPositionColor
{
	//Position
	glm::vec3 m_Position = XEMathHelpers::Vec3fZero;

	//Color 
	Color m_Color = XEColors::White;

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
			{"POSITION",	0,	DXGI_FORMAT_R32G32B32_FLOAT,	0,	0,	D3D11_INPUT_PER_VERTEX_DATA,	0},
			{"COLOR",		0,	DXGI_FORMAT_B8G8R8A8_UNORM,		0,	12,	D3D11_INPUT_PER_VERTEX_DATA,	0}
		};

		m_VertexLayout = new VertexLayout();

		XEResult ret = m_VertexLayout->BuildVertexLayout(graphicDevice, DummyVertexPositionColor, sizeof(DummyVertexPositionColor), vertexDesc, 2, L"VertexPositionColorLayout");

		if(ret != XEResult::Ok)
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
		return VertexType::VtxPosCol;
	}

	static uint32_t VertexSize()
	{
		return sizeof(VertexPositionColor);
	}
};

#endif