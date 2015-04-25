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
#include "BoundingSphere.h"

//Always include last
#include "Memory\MemLeaks.h"

/********************
*   Function Defs   *
*********************/
BoundingSphere::BoundingSphere()
{
}

BoundingSphere::BoundingSphere(float radius)
	: m_Radius(radius)
{
}

BoundingSphere::~BoundingSphere()
{
}

void BoundingSphere::PushPositionPoint(const glm::vec3& pos)
{
	if (m_Radius < glm::abs(pos.x))
	{
		m_Radius = pos.x;
	}

	if (m_Radius < glm::abs(pos.y))
	{
		m_Radius = pos.y;
	}

	if (m_Radius < glm::abs(pos.z))
	{
		m_Radius = pos.z;
	}
}
