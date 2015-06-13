/********************************************************
*
* Author: Carlos Chacón N.
*
* Copyright 2012-2015
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
struct BoundingBox sealed : public AEObject
{
	glm::vec3 m_MinPos = AEMathHelpers::Vec3fMin;
	glm::vec3 m_MaxPos = AEMathHelpers::Vec3fMax;

	BoundingBox();

	BoundingBox(const glm::vec3& minPos, const glm::vec3& maxPos);

	virtual ~BoundingBox();

	void PushPositionPoint(const glm::vec3& pos);

	glm::vec3 GetSize() const;
};

#endif