
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

#ifndef _QUAD_SHAPE_H
#define _QUAD_SHAPE_H

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
class QuadShape abstract : public XEObject
{
	protected:
		//Variables
		bool m_IsReady = false;

		bool m_ClockWise = false;

		VertexPositionTexture m_Vtx[4];

		VertexBuffer<VertexPositionTexture>* m_VB = nullptr;

		IndexBuffer* m_IB = nullptr;

		GraphicDevice* m_GraphicDevice = nullptr;

	public:
		//Constructor Destructor.
		QuadShape(GraphicDevice* graphicDevice, bool clockWise = true);
		virtual ~QuadShape();

		//Get Methods
		inline IVertexBuffer*	GetVertexBuffer		()									{ return m_VB; }
		inline IndexBuffer*		GetIndexBuffer		()									{ return m_IB; }

		//Framework Methods
		virtual XEResult BuildQuad() = 0;

		XEResult			DrawQuad			();
};

#endif

