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
#include "Base\AEObject.h"
#include "Math\AEMathDefs.h"

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
#define AE_MAX_BONES		96

/****************
*   Constants   *
*****************/

/************
*   Enums   *
*************/

/*************
*   Struct   *
**************/
struct BonePose sealed : public AEObject
{
	private:
		glm::mat4 m_TransformMatrix = AEMathHelpers::Mat4Identity;

	public:
		glm::quat m_RotationQ = AEMathHelpers::QuaternionIdentity;
		glm::vec3 m_Scale = AEMathHelpers::Vec3fOne;
		glm::vec3 m_Translation = AEMathHelpers::Vec3fZero;

		BonePose();

		void CalculateTransformMatrix();

		const glm::mat4& GetTransformMatrix() const;
};

#endif