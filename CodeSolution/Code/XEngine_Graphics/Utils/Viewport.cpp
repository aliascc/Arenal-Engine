/********************************************************
*
* Author: Carlos Chacón N.
*
* Created: 3/3/2014
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
#include "Viewport.h"

//Always include last
#include "Memory\MemLeaks.h"

/********************
*   Function Defs   *
*********************/
Viewport::Viewport()
{
	memset(&m_ViewportDX, 0, sizeof(D3D11_VIEWPORT));
}

Viewport::~Viewport()
{
}

XEResult Viewport::Initialize(float topLeftX, float topLeftY, float width, float height, float minDepth, float maxDepth)
{
	m_ViewportDX.TopLeftX	= topLeftX;
	m_ViewportDX.TopLeftY	= topLeftY;
	m_ViewportDX.Width		= width;
	m_ViewportDX.Height		= height;
	m_ViewportDX.MinDepth	= minDepth;
	m_ViewportDX.MaxDepth	= maxDepth;

	return XEResult::Ok;
}
