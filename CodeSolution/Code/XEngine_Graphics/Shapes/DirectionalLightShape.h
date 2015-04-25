/********************************************************
*
* Author: Carlos Chacón N.
*
* Copyright 2012-2015
*
*********************************************************/

#pragma once

#ifndef _DIRECTIONAL_LIGHT_SHAPE_H
#define _DIRECTIONAL_LIGHT_SHAPE_H

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
class DirectionalLightShape sealed : public LightShape
{
	public:
		//Constructor Destructor.
		DirectionalLightShape(GraphicDevice* graphicDevice);
		virtual ~DirectionalLightShape();

		XEResult BuildLightShape() override;
};

#endif

