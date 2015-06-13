/********************************************************
*
* Author: Carlos Chacón N.
*
* Copyright 2012-2015
*
*********************************************************/

#pragma once

#ifndef _VERTEX_LAYOUT_H
#define _VERTEX_LAYOUT_H

/**********************
*   System Includes   *
***********************/
#include <string>
#include <vector>
#include <stdint.h>
#include <d3d11_1.h>

/*************************
*   3rd Party Includes   *
**************************/

/***************************
*   Game Engine Includes   *
****************************/
#include "Base\Base.h"
#include "Base\Named.h"

/********************
*   Forward Decls   *
*********************/
class GraphicDevice;

/*****************
*   Class Decl   *
******************/
class VertexLayout sealed : public Named
{
	private:
		ID3D11InputLayout* m_VertexLayoutDX = nullptr;
		bool m_IsReady = false;

	public:
		//Constructor and Destructor
		VertexLayout();
		virtual ~VertexLayout();

		//Framework Methods
		AEResult BuildVertexLayout(GraphicDevice* graphicDevice, const BYTE shaderByteCode[], uint32_t shadeByteCodeSize, const D3D11_INPUT_ELEMENT_DESC vertexDesc[], uint32_t elementCount, const std::wstring& name = L"");

		//Get Methods
		inline ID3D11InputLayout* GetDXLayout() const 
		{ 
			return m_VertexLayoutDX; 
		}

		inline bool IsReady() const
		{
			return m_IsReady;
		}
};

#endif