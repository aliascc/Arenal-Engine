/********************************************************
*
* Author: Carlos Chacón N.
*
* Copyright 2012-2015
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
	m_TransformMatrix = AEMathHelpers::AffineTransformationMatrix(m_Translation, AEMathHelpers::Vec3fZero, m_RotationQ, m_Scale);
}

const glm::mat4& BonePose::GetTransformMatrix() const
{
	return m_TransformMatrix;
}

#pragma endregion