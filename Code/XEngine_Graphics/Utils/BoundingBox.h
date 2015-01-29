
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

#ifndef _BOUNDING_BOX_H
#define _BOUNDING_BOX_H

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
struct BoundingBox sealed : public XEObject
{
	glm::vec3 m_MinPos = XEMathHelpers::Vec3fMin;
	glm::vec3 m_MaxPos = XEMathHelpers::Vec3fMax;

	BoundingBox();

	BoundingBox(const glm::vec3& minPos, const glm::vec3& maxPos);

	virtual ~BoundingBox();

	void PushPositionPoint(const glm::vec3& pos);

	glm::vec3 GetSize() const;
};

#endif