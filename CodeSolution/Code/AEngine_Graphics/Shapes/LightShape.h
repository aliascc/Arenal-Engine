/********************************************************
*
* Author: Carlos Chacón N.
*
* Copyright 2012-2015
*
*********************************************************/

#pragma once

#ifndef _LIGHT_SHAPE_H
#define _LIGHT_SHAPE_H

/**********************
*   System Includes   *
***********************/
#include <vector>

/*************************
*   3rd Party Includes   *
**************************/
#include "glm\glm.hpp"

/***************************
*   Game Engine Includes   *
****************************/
#include "Base\Base.h"
#include "ShapesDefs.h"
#include "Base\AEObject.h"

/************
*   Using   *
*************/

/********************
*   Forward Decls   *
*********************/
class GraphicDevice;
class IVertexBuffer;

/*****************
*   Class Decl   *
******************/
class LightShape abstract : public AEObject
{
	protected:
		//Variables
		bool m_IsReady = false;

		std::vector<IVertexBuffer*> m_VertexBufferVector;

		GraphicDevice* m_GraphicDevice = nullptr;

		void CleanUp();

	public:
		//Constructor Destructor.
		LightShape(GraphicDevice* graphicDevice);
		virtual ~LightShape();

		//Framework Methods
		virtual AEResult BuildLightShape() = 0;

		AEResult DrawLightShape();
};

#endif

