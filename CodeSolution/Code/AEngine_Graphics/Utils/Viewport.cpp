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

AEResult Viewport::Initialize(float topLeftX, float topLeftY, float width, float height, float minDepth, float maxDepth)
{
	m_ViewportDX.TopLeftX	= topLeftX;
	m_ViewportDX.TopLeftY	= topLeftY;
	m_ViewportDX.Width		= width;
	m_ViewportDX.Height		= height;
	m_ViewportDX.MinDepth	= minDepth;
	m_ViewportDX.MaxDepth	= maxDepth;

	return AEResult::Ok;
}
