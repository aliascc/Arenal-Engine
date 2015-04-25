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
#include "PhysicsGOC.h"
#include "PhysicsActor.h"
#include "PhysicsManager.h"
#include "PhysicColliderBox.h"
#include "Base\BaseFunctions.h"
#include "PhysicColliderSphere.h"
#include "GameObject\GameObject.h"

//Always include last
#include "Memory\MemLeaks.h"

/********************
*   Function Defs   *
*********************/
XETODO("Add mutex");
PhysicsGOC::PhysicsGOC(GameObject* gameObject, PhysicsManager* physicsManager)
	: GameObjectComponent(gameObject, GameObjectComponentType::Physics)
	, m_PhysicsManager(physicsManager)
{
	XEAssert(m_PhysicsManager != nullptr);
	if (m_PhysicsManager == nullptr)
	{
		m_IsReady = false;

		return;
	}

}

PhysicsGOC::~PhysicsGOC()
{
	if (m_PhysicsActor != nullptr)
	{
		XETODO("Check return");
		m_PhysicsManager->RemovePhysicsActor(m_PhysicsActor->GetUniqueID(), false);
	}
	DeleteMem(m_PhysicsActor);
}

XEResult PhysicsGOC::AddRigidBody()
{
	if (!m_IsReady)
	{
		return XEResult::NotReady;
	}

	XEResult ret = XEResult::Ok;

	if (m_PhysicsActor == nullptr)
	{
		m_PhysicsActor = new PhysicsActor();

		m_PhysicsActor->SetObject3D(m_GameObject);

		ret = m_PhysicsActor->Initialize(m_PhysicsManager, PhysicsActorType::Dynamic);
		if (ret != XEResult::Ok)
		{
			DeleteMem(m_PhysicsActor);

			return ret;
		}

		//////////////////////////////
		//Add to Physics Manager
		ret = m_PhysicsManager->AddPhysicsActor(m_PhysicsActor);
		if (ret != XEResult::Ok)
		{
			DeleteMem(m_PhysicsActor);

			return ret;
		}
	}
	else if (m_PhysicsActor->GetPhysicsActorType() != PhysicsActorType::Dynamic)
	{
		ret = m_PhysicsActor->ChangePhysicsActorType(PhysicsActorType::Dynamic);
		if (ret != XEResult::Ok)
		{
			m_PhysicsManager->RemovePhysicsActor(m_PhysicsActor->GetUniqueID(), false);

			DeleteMem(m_PhysicsActor);

			return ret;
		}
	}

	return XEResult::Ok;
}

XEResult PhysicsGOC::RemoveRigidBody()
{
	if (!m_IsReady)
	{
		return XEResult::NotReady;
	}

	if (m_PhysicsActor == nullptr)
	{
		return XEResult::Ok;
	}

	if (m_PhysicsActor->GetPhysicsActorType() != PhysicsActorType::Dynamic)
	{
		return XEResult::Ok;
	}

	if (m_PhysicsActor->GetNumberOfColliders() == 0)
	{
		DeleteMem(m_PhysicsActor);
	}
	else
	{
		XEResult ret = m_PhysicsActor->ChangePhysicsActorType(PhysicsActorType::Static);
		if (ret != XEResult::Ok)
		{
			DeleteMem(m_PhysicsActor);

			return ret;
		}
	}

	return XEResult::Ok;
}

XEResult PhysicsGOC::AddCollider(CollisionShape collisionShape, uint64_t& colliderID)
{
	if (!m_IsReady)
	{
		return XEResult::NotReady;
	}

	PhysicCollider* physicCollider = nullptr;

	switch (collisionShape)
	{
		case CollisionShape::Box:
			physicCollider = new PhysicColliderBox();
			break;

		case CollisionShape::Sphere:
			physicCollider = new PhysicColliderSphere();
			break;

		default:
			XEAssert(false);
			return XEResult::InvalidObjType;
	}

	XEResult ret = XEResult::Ok;

	///////////////////////////////
	//Initialize
	ret = physicCollider->Initialize(m_PhysicsManager);
	if (ret != XEResult::Ok)
	{
		DeleteMem(physicCollider);

		return ret;
	}

	///////////////////////////////
	//Initialize Physics Actor
	if (m_PhysicsActor == nullptr)
	{
		m_PhysicsActor = new PhysicsActor();

		m_PhysicsActor->SetObject3D(m_GameObject);

		ret = m_PhysicsActor->Initialize(m_PhysicsManager, PhysicsActorType::Static);
		if (ret != XEResult::Ok)
		{
			DeleteMem(physicCollider);

			DeleteMem(m_PhysicsActor);

			return ret;
		}

		//////////////////////////////
		//Add to Physics Manager
		ret = m_PhysicsManager->AddPhysicsActor(m_PhysicsActor);
		if (ret != XEResult::Ok)
		{
			DeleteMem(physicCollider);

			DeleteMem(m_PhysicsActor);

			return ret;
		}
	}

	///////////////////////////////
	//Add
	ret = m_PhysicsActor->AddPhysicCollider(physicCollider);
	if (ret != XEResult::Ok)
	{
		DeleteMem(physicCollider);

		return ret;
	}

	colliderID = physicCollider->GetUniqueID();

	return XEResult::Ok;
}

bool PhysicsGOC::IsRigidBody()
{
	if (!m_IsReady || m_PhysicsActor == nullptr)
	{
		return false;
	}

	return (m_PhysicsActor->GetPhysicsActorType() == PhysicsActorType::Dynamic);
}
