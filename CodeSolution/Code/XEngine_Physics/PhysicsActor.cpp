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
#include "PhysicsActor.h"
#include "PhysicsManager.h"
#include "Utils\Object3D.h"
#include "PhysicCollider.h"
#include "Base\BaseFunctions.h"

#if defined(_DEBUG)
#include "Memory\MemLeaks.h"
#endif

/********************
*   Function Defs   *
*********************/

PhysicsActor::PhysicsActor()
{
}

PhysicsActor::~PhysicsActor()
{
	CleanUp();
}

void PhysicsActor::CleanUp()
{
	for each (auto collider in m_PhysicColliderMap)
	{
		PhysicCollider* physicCollider = collider.second;

		m_PxRigidActor->detachShape(*physicCollider->GetPxShape());

		DeleteMem(physicCollider);
	}
	m_PhysicColliderMap.clear();

	ReleasePhysX(m_PxRigidActor);
}

XEResult PhysicsActor::Initialize(PhysicsManager* physicsManager, PhysicsActorType physicsActorType)
{
	////////////////////////////
	//Pre-checks
	if (m_IsReady && m_PhysicsActorType == physicsActorType)
	{
		return XEResult::Ok;
	}
	else if (m_IsReady)
	{
		XETODO("Better retun code");

		return XEResult::Fail;
	}

	XEAssert(physicsManager != nullptr);
	if (physicsManager == nullptr)
	{
		return XEResult::NullParameter;
	}

	if (m_Object3D == nullptr)
	{
		return XEResult::NullObj;
	}

	if (!physicsManager->IsReady())
	{
		XETODO("Better return code");
		return XEResult::Fail;
	}

	////////////////////////////
	//Get PhysX Managers
	physx::PxPhysics* pxPhysics = physicsManager->GetPhysX();
	physx::PxScene* pxScene = physicsManager->GetPxScene();

	////////////////////////////
	//Get Objs Position and Rotation
	glm::quat glmRotQuat = m_Object3D->GetWorldRotationQuat();
	glm::vec3 glmPos = m_Object3D->GetWorldPosition();

	physx::PxQuat pxRotQuat(glmRotQuat.x, glmRotQuat.y, glmRotQuat.z, glmRotQuat.w);
	physx::PxVec3 pxPos(glmPos.x, glmPos.y, glmPos.z);

	physx::PxTransform pxTransform(pxPos, pxRotQuat);

	////////////////////////////
	//Create PhysX Actor
	switch (physicsActorType)
	{
		case PhysicsActorType::Dynamic:
			{
				physx::PxRigidDynamic* rigidDynamic = pxPhysics->createRigidDynamic(pxTransform);

				if (InitDynamicObject(rigidDynamic) != XEResult::Ok)
				{
					ReleasePhysX(rigidDynamic);

					XETODO("Better ret code");
					return XEResult::Fail;
				}

				m_PxRigidActor = rigidDynamic;
			}
			break;

		case PhysicsActorType::Static:
			{
				m_PxRigidActor = pxPhysics->createRigidStatic(pxTransform);
			}
			break;

		default:
			XEAssert(false);
			return XEResult::InvalidObjType;
	}

	////////////////////////////////
	//Finish
	m_PhysicsActorType = physicsActorType;
	m_PhysicsManager = physicsManager;

	m_IsReady = true;

	return XEResult::Ok;
}

XEResult PhysicsActor::InitDynamicObject(physx::PxRigidDynamic* rigidDynamic)
{
	XEAssert(rigidDynamic != nullptr);
	if (rigidDynamic == nullptr)
	{
		return XEResult::NullParameter;
	}

	rigidDynamic->setMass(m_Mass);
	rigidDynamic->setAngularDamping(m_AngularDamping);
	rigidDynamic->setLinearDamping(m_LinearDamping);
	rigidDynamic->setRigidBodyFlag(physx::PxRigidBodyFlag::eKINEMATIC, m_Kinematic);
	rigidDynamic->setActorFlag(physx::PxActorFlag::eDISABLE_GRAVITY, !m_GravityEnable);

	return XEResult::Ok;
}

XEResult PhysicsActor::ChangePhysicsActorType(PhysicsActorType physicsActorType)
{
	////////////////////////////
	//Pre-checks
	if (!m_IsReady)
	{
		return XEResult::NotReady;
	}

	if (m_IsReady && m_PhysicsActorType == physicsActorType)
	{
		return XEResult::Ok;
	}

	////////////////////////////
	//Get PhysX Managers
	physx::PxPhysics* pxPhysics = m_PhysicsManager->GetPhysX();
	physx::PxScene* pxScene = m_PhysicsManager->GetPxScene();

	////////////////////////////
	//Get Transform
	physx::PxTransform pxTransform = m_PxRigidActor->getGlobalPose();

	////////////////////////////////
	//Remove Actor
	bool actorIsInManager = false;
	if (m_PhysicsManager->ExistsPhysicsActor(this->GetUniqueID()))
	{
		actorIsInManager = true;
		pxScene->removeActor(*m_PxRigidActor);
	}

	////////////////////////////
	//Remove Actor
	ReleasePhysX(m_PxRigidActor);

	////////////////////////////
	//Create PhysX Actor
	switch (physicsActorType)
	{
		case PhysicsActorType::Dynamic:
		{
			physx::PxRigidDynamic* rigidDynamic = pxPhysics->createRigidDynamic(pxTransform);

			if (InitDynamicObject(rigidDynamic) != XEResult::Ok)
			{
				ReleasePhysX(rigidDynamic);

				XETODO("Better ret code");
				return XEResult::Fail;
			}

			m_PxRigidActor = rigidDynamic;
		}
		break;

		case PhysicsActorType::Static:
		{
			m_PxRigidActor = pxPhysics->createRigidStatic(pxTransform);
		}
		break;

		default:
			XEAssert(false);
			return XEResult::InvalidObjType;
	}

	////////////////////////////////
	//Add Actor
	if (actorIsInManager)
	{
		pxScene->addActor(*m_PxRigidActor);
	}

	////////////////////////////////
	//Re-insert colliders
	for each (auto collider in m_PhysicColliderMap)
	{
		PhysicCollider* physicCollider = collider.second;

		m_PxRigidActor->attachShape(*physicCollider->GetPxShape());
	}

	////////////////////////////////
	//Finish
	m_PhysicsActorType = physicsActorType;

	return XEResult::Ok;
}

bool PhysicsActor::ExistsPhysicCollider(uint64_t id) const
{
	return (m_PhysicColliderMap.find(id) != m_PhysicColliderMap.end());
}

PhysicCollider* PhysicsActor::GetPhysicCollider(uint64_t id)
{
	if (!ExistsPhysicCollider(id))
	{
		return nullptr;
	}

	return m_PhysicColliderMap[id];
}

XEResult PhysicsActor::AddPhysicCollider(PhysicCollider* physicCollider)
{
	if (!m_IsReady)
	{
		return XEResult::NotReady;
	}

	XEAssert(physicCollider != nullptr);
	if (physicCollider == nullptr)
	{
		return XEResult::NullParameter;
	}

	if (!physicCollider->IsReady())
	{
		XETODO("Add better return code");
		return XEResult::Fail;
	}

	if (ExistsPhysicCollider(physicCollider->GetUniqueID()))
	{
		return XEResult::ObjExists;
	}

	m_PxRigidActor->attachShape(*physicCollider->GetPxShape());

	m_PhysicColliderMap[physicCollider->GetUniqueID()] = physicCollider;

	return XEResult::Ok;
}

XEResult PhysicsActor::RemovePhysicCollider(uint64_t id, bool deleteObj)
{
	if (!ExistsPhysicCollider(id))
	{
		return XEResult::NotFound;
	}

	PhysicCollider* physicCollider = m_PhysicColliderMap[id];

	m_PxRigidActor->detachShape(*physicCollider->GetPxShape());

	m_PhysicColliderMap.erase(id);

	if (deleteObj)
	{
		DeleteMem(physicCollider);
	}

	return XEResult::Ok;
}

XEResult PhysicsActor::UpdateObject3D()
{
	if (!m_IsReady)
	{
		return XEResult::NotReady;
	}

	/////////////////////////////////////////////
	//Get Actor Pose
	physx::PxTransform pxTransform = m_PxRigidActor->getGlobalPose();

	/////////////////////////////////////////////
	//Get Parent Transforms
	glm::mat4 indentityScale		= XEMathHelpers::Mat4Identity;
	glm::mat4 parentWorldTransform	= XEMathHelpers::Mat4Identity;
	glm::quat parentRotQuat			= XEMathHelpers::QuaternionIdentity;

	if (m_Object3D->GetParentObject3D() != nullptr)
	{
		Object3D* parent3D = m_Object3D->GetParentObject3D();

		parentWorldTransform	= parent3D->GetWorldTransform();
		parentRotQuat			= parent3D->GetWorldRotationQuat();
	}

	/////////////////////////////////////////////
	//Get Current Transforms
	glm::vec3 currentPos		= glm::vec3(pxTransform.p.x, pxTransform.p.y, pxTransform.p.z);
	glm::quat currentRotQuat	= glm::quat(pxTransform.q.w, pxTransform.q.x, pxTransform.q.y, pxTransform.q.z);
	glm::vec3 currentRot		= glm::degrees(XEMathHelpers::GetEulerAnglesFromQuaternionRotation(currentRotQuat));
	glm::vec3 currentScale		= XEMathHelpers::Vec3fOne;

	glm::mat4 currentTransform	= XEMathHelpers::CreateTransform(currentPos, currentRot, currentScale);

	/////////////////////////////////////////////
	//New Transforms
	glm::vec3 newPosition	= XEMathHelpers::Vec3fZero;
	glm::vec3 newRotation	= XEMathHelpers::Vec3fZero;
	glm::vec3 newScale		= XEMathHelpers::Vec3fZero;

	/////////////////////////////////////////////
	//Get Positions
	XEMathHelpers::GetTransformationFromRelativeToParent(parentWorldTransform, parentRotQuat, indentityScale, currentRotQuat, currentTransform, indentityScale, newPosition, newRotation, newScale);

	/////////////////////////////////////////////
	//Set to Object 3D
	m_Object3D->SetPosition(newPosition);
	m_Object3D->SetRotation(newRotation);

	/////////////////////////////////////////////
	//Finish
	return XEResult::Ok;
}

XEResult PhysicsActor::SetObject3D(Object3D* object3D)
{
	if (m_IsReady)
	{
		return XEResult::Fail;
	}

	m_Object3D = object3D;

	return XEResult::Ok;
}

void PhysicsActor::SetMass(float mass)
{
	if (mass < 0.0f)
	{
		mass = 0;
	}

	m_Mass = mass;
	if (!m_IsReady && m_PhysicsActorType != PhysicsActorType::Dynamic)
	{
		return;
	}

	physx::PxRigidDynamic* rigidDynamic = reinterpret_cast<physx::PxRigidDynamic*>(m_PxRigidActor);

	rigidDynamic->setMass(m_Mass);
}

void PhysicsActor::SetGravityEnable(bool gravityEnable)
{
	m_GravityEnable = gravityEnable;
	if (!m_IsReady && m_PhysicsActorType != PhysicsActorType::Dynamic)
	{
		return;
	}

	physx::PxRigidDynamic* rigidDynamic = reinterpret_cast<physx::PxRigidDynamic*>(m_PxRigidActor);

	rigidDynamic->setActorFlag(physx::PxActorFlag::eDISABLE_GRAVITY, !m_GravityEnable);
}

void PhysicsActor::SetKinematic(bool kinematic)
{
	m_Kinematic = kinematic;
	if (!m_IsReady && m_PhysicsActorType != PhysicsActorType::Dynamic)
	{
		return;
	}

	physx::PxRigidDynamic* rigidDynamic = reinterpret_cast<physx::PxRigidDynamic*>(m_PxRigidActor);

	rigidDynamic->setRigidBodyFlag(physx::PxRigidBodyFlag::eKINEMATIC, m_Kinematic);
}

void PhysicsActor::SetLinearDamping(float linearDamping)
{
	if (linearDamping < 0.0f)
	{
		linearDamping = 0.0f;
	}

	m_LinearDamping = linearDamping;
	if (!m_IsReady && m_PhysicsActorType != PhysicsActorType::Dynamic)
	{
		return;
	}

	physx::PxRigidDynamic* rigidDynamic = reinterpret_cast<physx::PxRigidDynamic*>(m_PxRigidActor);

	rigidDynamic->setLinearDamping(m_LinearDamping);
}

void PhysicsActor::SetAngularDamping(float angularDamping)
{
	if (angularDamping < 0.0f)
	{
		angularDamping = 0.0f;
	}

	m_AngularDamping = angularDamping;
	if (!m_IsReady && m_PhysicsActorType != PhysicsActorType::Dynamic)
	{
		return;
	}

	physx::PxRigidDynamic* rigidDynamic = reinterpret_cast<physx::PxRigidDynamic*>(m_PxRigidActor);

	rigidDynamic->setAngularDamping(m_AngularDamping);
}
