
/********************************************************
*
* Author: Carlos Chacón N.
*
* Desc:
*
*
* Copyright 2012-2014
*
*********************************************************/

#pragma once

#ifndef _BOUNDING_SPHERE_H
#define _BOUNDING_SPHERE_H

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
#include "Math\XEMathDefs.h"

/************
*   Using   *
*************/

/********************
*   Forward Decls   *
*********************/

/******************
*   Struct Decl   *
*******************/
struct BoundingSphere sealed : public XEObject
{
	float m_Radius = 0.0f;

	BoundingSphere();

	BoundingSphere(float radius);

	virtual ~BoundingSphere();

	void PushPositionPoint(const glm::vec3& pos);
};

#endif