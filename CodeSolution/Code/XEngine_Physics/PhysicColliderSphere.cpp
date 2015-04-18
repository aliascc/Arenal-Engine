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
#include "PhysicsManager.h"
#include "Utils\Object3D.h"
#include "Base\BaseFunctions.h"
#include "PhysicColliderSphere.h"

#include "Memory\MemLeaks.h"

/********************
*   Function Defs   *
*********************/

PhysicColliderSphere::PhysicColliderSphere()
	: PhysicCollider(CollisionShape::Sphere)
{
}

PhysicColliderSphere::~PhysicColliderSphere()
{
}

physx::PxGeometry& PhysicColliderSphere::CreateGeomtry(physx::PxPhysics* pxPhysics)
{
	m_PxSphereGeometry.radius = m_Radius;

	return m_PxSphereGeometry;
}

void PhysicColliderSphere::SetRadius(float radius)
{
	if (radius < 0.0f)
	{
		radius = 0.0f;
	}

	m_Radius = radius;
	if (!m_IsReady)
	{
		return;
	}

	m_PxSphereGeometry.radius = m_Radius;

	m_PxShape->setGeometry(m_PxSphereGeometry);
}

void PhysicColliderSphere::SetScale(const glm::vec3& scale)
{
	m_Scale = scale;

	if (m_Scale.x < 0.0f)
	{
		m_Scale.x = 0.0f;
	}

	if (m_Scale.y < 0.0f)
	{
		m_Scale.y = 0.0f;
	}

	if (m_Scale.z < 0.0f)
	{
		m_Scale.z = 0.0f;
	}

	if (!m_IsReady)
	{
		return;
	}

	float maxScale = 0.0f;
	if (maxScale < m_Scale.x)
	{
		maxScale = m_Scale.x;
	}

	if (maxScale < m_Scale.y)
	{
		maxScale = m_Scale.y;
	}

	if (maxScale < m_Scale.z)
	{
		maxScale = m_Scale.z;
	}

	m_PxSphereGeometry.radius = m_Radius * maxScale;

	m_PxShape->setGeometry(m_PxSphereGeometry);
}
