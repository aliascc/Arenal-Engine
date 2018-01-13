/*
* Copyright (c) 2018 <Carlos Chacón>
* All rights reserved
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
* http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*/

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
#include "PhysicColliderBox.h"
#include "Base\BaseFunctions.h"

#include "Memory\MemLeaks.h"

/********************
*   Function Defs   *
*********************/

PhysicColliderBox::PhysicColliderBox()
	: PhysicCollider(CollisionShape::Box)
{
}

PhysicColliderBox::~PhysicColliderBox()
{
}

physx::PxGeometry& PhysicColliderBox::CreateGeomtry(physx::PxPhysics* pxPhysics)
{
	m_PxBoxGeometry.halfExtents.x = (m_Size.x * m_Scale.x) / 2.0f;
	m_PxBoxGeometry.halfExtents.y = (m_Size.y * m_Scale.y) / 2.0f;
	m_PxBoxGeometry.halfExtents.z = (m_Size.z * m_Scale.z) / 2.0f;

	return m_PxBoxGeometry;
}

void PhysicColliderBox::SetSize(const glm::vec3& size)
{
	m_Size = size;

	if (m_Size.x < 0.0f)
	{
		m_Size.x = 0.0f;
	}

	if (m_Size.y < 0.0f)
	{
		m_Size.y = 0.0f;
	}

	if (m_Size.z < 0.0f)
	{
		m_Size.z = 0.0f;
	}

	if (!m_IsReady)
	{
		return;
	}

	m_PxBoxGeometry.halfExtents.x = (m_Size.x * m_Scale.x) / 2.0f;
	m_PxBoxGeometry.halfExtents.y = (m_Size.y * m_Scale.y) / 2.0f;
	m_PxBoxGeometry.halfExtents.z = (m_Size.z * m_Scale.z) / 2.0f;

	m_PxShape->setGeometry(m_PxBoxGeometry);
}

void PhysicColliderBox::SetScale(const glm::vec3& scale)
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

	m_PxBoxGeometry.halfExtents.x = (m_Size.x * m_Scale.x) / 2.0f;
	m_PxBoxGeometry.halfExtents.y = (m_Size.y * m_Scale.y) / 2.0f;
	m_PxBoxGeometry.halfExtents.z = (m_Size.z * m_Scale.z) / 2.0f;

	m_PxShape->setGeometry(m_PxBoxGeometry);
}
