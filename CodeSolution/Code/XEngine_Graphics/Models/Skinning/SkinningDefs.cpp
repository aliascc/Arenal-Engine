/********************************************************
*
* Author: Carlos Chacón N.
*
* Created: 12/30/2012
*
* Desc:
*
*
* Copyright 2012
*
*********************************************************/

/**********************
*   System Includes   *
***********************/

/*************************
*   3rd Party Includes   *
**************************/

/***************************
*   Game Engine Includes   *
****************************/
#include "Bone.h"
#include "SkinningDefs.h"

//Always include last
#include "Memory\MemLeaks.h"

/********************
*   Function Defs   *
*********************/

/***************
*   BonePose   *
****************/
#pragma region Struct BonePose

BonePose::BonePose()
{
}

void BonePose::CalculateTransformMatrix()
{
	m_TransformMatrix = XEMathHelpers::AffineTransformationMatrix(m_Translation, XEMathHelpers::Vec3fZero, m_RotationQ, m_Scale);
}

const glm::mat4& BonePose::GetTransformMatrix() const
{
	return m_TransformMatrix;
}

#pragma endregion