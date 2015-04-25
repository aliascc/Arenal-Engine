/********************************************************
*
* Author: Carlos Chacón N.
*
* Copyright 2012-2015
*
*********************************************************/

#pragma once

#ifndef _SKINNING_DEFS_H
#define _SKINNING_DEFS_H

/**********************
*   System Includes   *
***********************/
#include <list>
#include <stdint.h>

/*************************
*   3rd Party Includes   *
**************************/
#include "glm\glm.hpp"
#include "glm\gtc\quaternion.hpp"

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
class Bone;

/**************
*   Defines   *
***************/

//Has to match shader
#define XE_MAX_BONES		96

/****************
*   Constants   *
*****************/

/************
*   Enums   *
*************/

/*************
*   Struct   *
**************/
struct BonePose sealed : public XEObject
{
	private:
		glm::mat4 m_TransformMatrix = XEMathHelpers::Mat4Identity;

	public:
		glm::quat m_RotationQ = XEMathHelpers::QuaternionIdentity;
		glm::vec3 m_Scale = XEMathHelpers::Vec3fOne;
		glm::vec3 m_Translation = XEMathHelpers::Vec3fZero;

		BonePose();

		void CalculateTransformMatrix();

		const glm::mat4& GetTransformMatrix() const;
};

#endif