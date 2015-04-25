/********************************************************
*
* Author: Carlos Chacón N.
*
* Copyright 2012-2015
*
*********************************************************/

#pragma once

#ifndef _QUAD_SHAPE_2D_H
#define _QUAD_SHAPE_2D_H

/**********************
*   System Includes   *
***********************/

/*************************
*   3rd Party Includes   *
**************************/
#include "glm\glm.hpp"

/***************************
*   Game Engine Includes   *
****************************/
#include "QuadShape.h"
#include "Base\Base.h"
#include "Vertex\VertexBuffer.h"
#include "Vertex\Types\VertexPositionTexture.h"

/************
*   Using   *
*************/

/********************
*   Forward Decls   *
*********************/
class IndexBuffer;
class GraphicDevice;
class IVertexBuffer;

/*****************
*   Class Decl   *
******************/
class QuadShape2D sealed : public QuadShape
{
	private:
		//Variables
		bool m_Dynamic = false;

		glm::vec4 m_CurrentTexCoords = XEMathHelpers::Vec4fZero;

		RECT m_CurrentSize;

	public:
		//Constructor Destructor.
		QuadShape2D(GraphicDevice* graphicDevice, bool dynamic = false, bool clockWise = true);
		virtual ~QuadShape2D();

		//Framework Methods
		XEResult Resize(const RECT& size, const glm::vec4& textCoords);

		XEResult BuildQuad() override;
};

#endif

