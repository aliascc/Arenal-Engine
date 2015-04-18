
/********************************************************
*
* Author: Carlos Chacón N.
*
* Created: 3/4/2014
*
* Desc:
*
*
* Copyright 2012
*
*********************************************************/

#pragma once

#ifndef _FRUSTUM_SHAPE_H
#define _FRUSTUM_SHAPE_H

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
#include "Vertex\Types\VertexPositionColor.h"

/************
*   Using   *
*************/

/********************
*   Forward Decls   *
*********************/
class Frustum;
class GraphicDevice;
class IVertexBuffer;

/**************
*   Defines   *
***************/

#define XE_FRUSTUM_SIMPLE_SHAPE_SIZE	24
#define XE_FRUSTUM_SHAPE_SIZE			252

/*****************
*   Class Decl   *
******************/
class FrustumShape sealed : public XEObject
{
	private:
		//Variables
		bool m_IsReady = false;

		VertexPositionColor m_Vtx[XE_FRUSTUM_SHAPE_SIZE];

		VertexBuffer<VertexPositionColor>* m_VB = nullptr;

		GraphicDevice* m_GraphicDevice = nullptr;

		XEResult UpdateFrustumShapeVtx(const Frustum* frustum);

	public:
		//Constructor Destructor.
		FrustumShape(GraphicDevice* graphicDevice);
		virtual ~FrustumShape();

		//Get Methods
		inline IVertexBuffer* GetVertexBuffer()
		{
			return m_VB;
		}

		//Framework Methods
		XEResult BuildFrustumShape();

		XEResult UpdateFrustumShape(const Frustum* frustum);

		XEResult DrawFrustumShape(bool simple = false);
};

#endif

