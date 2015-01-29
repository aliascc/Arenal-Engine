
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

#pragma once

#ifndef _QUAD_SHAPE_3D_H
#define _QUAD_SHAPE_3D_H

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
#include "Base\XEObject.h"
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
class QuadShape3D sealed : public QuadShape
{
	public:
		//Constructor Destructor.
		QuadShape3D(GraphicDevice* graphicDevice, bool clockWise = true);
		virtual ~QuadShape3D();

		//Framework Methods
		XEResult BuildQuad() override;
};

#endif

