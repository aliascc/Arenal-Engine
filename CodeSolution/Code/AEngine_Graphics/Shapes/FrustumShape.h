/********************************************************
*
* Author: Carlos Chacón N.
*
* Copyright 2012-2015
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
#include "Base\AEObject.h"
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

#define AE_FRUSTUM_SIMPLE_SHAPE_SIZE	24
#define AE_FRUSTUM_SHAPE_SIZE			252

/*****************
*   Class Decl   *
******************/
class FrustumShape sealed : public AEObject
{
	private:
		//Variables
		bool m_IsReady = false;

		VertexPositionColor m_Vtx[AE_FRUSTUM_SHAPE_SIZE];

		VertexBuffer<VertexPositionColor>* m_VB = nullptr;

		GraphicDevice* m_GraphicDevice = nullptr;

		AEResult UpdateFrustumShapeVtx(const Frustum* frustum);

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
		AEResult BuildFrustumShape();

		AEResult UpdateFrustumShape(const Frustum* frustum);

		AEResult DrawFrustumShape(bool simple = false);
};

#endif

