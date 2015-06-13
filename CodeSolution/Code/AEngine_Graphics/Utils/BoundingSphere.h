/********************************************************
*
* Author: Carlos Chacón N.
*
* Copyright 2012-2015
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
#include "Base\AEObject.h"
#include "Math\AEMathDefs.h"

/************
*   Using   *
*************/

/********************
*   Forward Decls   *
*********************/

/******************
*   Struct Decl   *
*******************/
struct BoundingSphere sealed : public AEObject
{
	float m_Radius = 0.0f;

	BoundingSphere();

	BoundingSphere(float radius);

	virtual ~BoundingSphere();

	void PushPositionPoint(const glm::vec3& pos);
};

#endif