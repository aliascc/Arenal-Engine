/********************************************************
*
* Author: Carlos Chacón N.
*
* Copyright 2012-2015
*
*********************************************************/

#pragma once

#ifndef _OMNI_LIGHT_SHAPE_H
#define _OMNI_LIGHT_SHAPE_H

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
#include "LightShape.h"

/************
*   Using   *
*************/

/********************
*   Forward Decls   *
*********************/

/*****************
*   Class Decl   *
******************/
class OmniLightShape sealed : public LightShape
{
	private:

		uint32_t m_NumEdges = 0;

		float m_Size = XE_LIGHT_SHAPE_OMNI_DEFAULT_RING_SIZE;

	public:
		//Constructor Destructor.
		OmniLightShape(GraphicDevice* graphicDevice, uint32_t numEdges = XE_LIGHT_SHAPE_DEFAULT_NUM_EDGES);
		virtual ~OmniLightShape();

		XEResult BuildLightShape() override;
};

#endif

