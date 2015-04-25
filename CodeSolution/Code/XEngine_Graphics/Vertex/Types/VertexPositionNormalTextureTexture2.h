/********************************************************
*
* Author: Carlos Chacón N.
*
* Copyright 2012-2015
*
*********************************************************/

#pragma once

#ifndef _VERTEX_POSITION_NORMAL_TEXTURE_TEXTURE2_H
#define _VERTEX_POSITION_NORMAL_TEXTURE_TEXTURE2_H

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
#include "Math\XEMathDefs.h"
#include "Vertex\VertexDefs.h"
#include "Vertex\VertexLayout.h"

#if defined(DEBUG) | defined(_DEBUG)
	#if defined(_WIN64) | defined (WIN64) 
#include "Compiled Materials\HLSL\DummyVertexPositionNormalTextureTexture2_x64_d.h"		
	#else
#include "Compiled Materials\HLSL\DummyVertexPositionNormalTextureTexture2_x86_d.h"
	#endif
#else
	#if defined(_WIN64) | defined (WIN64) 
#include "Compiled Materials\HLSL\DummyVertexPositionNormalTextureTexture2_x64.h"
	#else
#include "Compiled Materials\HLSL\DummyVertexPositionNormalTextureTexture2_x86.h"
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
struct VertexPositionNormalTextureTexture2
{
	//Position
	glm::vec3 m_Position = XEMathHelpers::Vec3fZero;

	//Normal 
	glm::vec3 m_Normal = XEMathHelpers::Vec3fZero;

	//Texture Coordinates
	glm::vec2 m_TexCoord = XEMathHelpers::Vec2fZero;

	//Texture Coordinates 2
	glm::vec2 m_TexCoord2 = XEMathHelpers::Vec2fZero;

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
			{"NORMAL",		0,	DXGI_FORMAT_R32G32B32_FLOAT,	0,	12,	D3D11_INPUT_PER_VERTEX_DATA,	0},
			{"TEXCOORD",	0,	DXGI_FORMAT_R32G32_FLOAT,		0,	24,	D3D11_INPUT_PER_VERTEX_DATA,	0},
			{"TEXCOORD",	1,	DXGI_FORMAT_R32G32_FLOAT,		0,	32,	D3D11_INPUT_PER_VERTEX_DATA,	0}
		};

		m_VertexLayout = new VertexLayout();

		XEResult ret = m_VertexLayout->BuildVertexLayout(graphicDevice, DummyVertexPositionNormalTextureTexture2, sizeof(DummyVertexPositionNormalTextureTexture2), vertexDesc, 4, L"VertexPositionNormalTextureTexture2Layout");

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
		return VertexType::VtxPosNorTexTex2;
	}

	static uint32_t VertexSize()
	{
		return sizeof(VertexPositionNormalTextureTexture2);
	}
};

#endif