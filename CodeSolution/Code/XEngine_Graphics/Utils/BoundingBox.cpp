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

/**********************
*   System Includes   *
***********************/

/*************************
*   3rd Party Includes   *
**************************/

/***************************
*   Game Engine Includes   *
****************************/
#include "BoundingBox.h"

//Always include last
#include "Memory\MemLeaks.h"

/********************
*   Function Defs   *
*********************/
BoundingBox::BoundingBox()
{
}

BoundingBox::BoundingBox(const glm::vec3& minPos, const glm::vec3& maxPos)
	: m_MinPos(minPos)
	, m_MaxPos(maxPos)
{
}

BoundingBox::~BoundingBox()
{
}

void BoundingBox::PushPositionPoint(const glm::vec3& pos)
{
	if (m_MinPos.x > pos.x)
	{
		m_MinPos.x = pos.x;
	}
	if (m_MinPos.y > pos.y)
	{
		m_MinPos.y = pos.y;
	}
	if (m_MinPos.z > pos.z)
	{
		m_MinPos.z = pos.z;
	}

	if (m_MaxPos.x < pos.x)
	{
		m_MaxPos.x = pos.x;
	}
	if (m_MaxPos.y < pos.y)
	{
		m_MaxPos.y = pos.y;
	}
	if (m_MaxPos.z < pos.z)
	{
		m_MaxPos.z = pos.z;
	}
}

glm::vec3 BoundingBox::GetSize() const
{
	return (m_MaxPos - m_MinPos);
}
