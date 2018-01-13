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

PhysicsActor::PhysicsActor(Object3D* object3D)
	: m_Object3D(object3D)
{
	AEAssert(m_Object3D != nullptr);
	if (m_Object3D != nullptr)
	{
		m_Object3DChangedEvent = std::bind(&PhysicsActor::Object3DChanged, this, std::placeholders::_1, std::placeholders::_2);

		AEResult ret = m_Object3D->AddObject3DChangeEventCallback(this->GetUniqueID(), m_Object3DChangedEvent);
		AEAssert(ret == AEResult::Ok);
	}
}

PhysicsActor::~PhysicsActor()
{
	if (m_Object3D != nullptr)
	{
		m_Object3D->RemoveObject3DChangeEventCallback(this->GetUniqueID());
	}

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

void PhysicsActor::Object3DChanged(Object3DChangeEventType changeType, Object3D* object3D)
{
	if (m_Object3D == nullptr || m_PxRigidActor == nullptr)
	{
		return;
	}

	switch (changeType)
	{
		////////////////////////////////////////
		// Set Position and Rotation
		case Object3DChangeEventType::PositionChange:
		case Object3DChangeEventType::RotationChange:
			{
				////////////////////////////
				//Get Objs Position and Rotation
				physx::PxTransform pxTransform;
				GetPXTransformFromObject3D(pxTransform);

				m_PxRigidActor->setGlobalPose(pxTransform);
			}
			break;

		case Object3DChangeEventType::ScaleChange:
		default:
			return;
	}
}

void PhysicsActor::GetPXTransformFromObject3D(physx::PxTransform& pxTransform)
{
	if (m_Object3D == nullptr)
	{
		return;
	}

	glm::quat glmRotQuat = m_Object3D->GetWorldRotationQuat();
	glm::vec3 glmPos = m_Object3D->GetWorldPosition();

	physx::PxQuat pxRotQuat(glmRotQuat.x, glmRotQuat.y, glmRotQuat.z, glmRotQuat.w);
	physx::PxVec3 pxPos(glmPos.x, glmPos.y, glmPos.z);

	pxTransform = physx::PxTransform(pxPos, pxRotQuat);
}

AEResult PhysicsActor::Initialize(PhysicsManager* physicsManager, PhysicsActorType physicsActorType)
{
	////////////////////////////
	//Pre-checks
	if (m_IsReady && m_PhysicsActorType == physicsActorType)
	{
		return AEResult::Ok;
	}
	else if (m_IsReady)
	{
		AETODO("Better retun code");

		return AEResult::Fail;
	}

	AEAssert(physicsManager != nullptr);
	if (physicsManager == nullptr)
	{
		return AEResult::NullParameter;
	}

	if (m_Object3D == nullptr)
	{
		return AEResult::NullObj;
	}

	if (!physicsManager->IsReady())
	{
		AETODO("Better return code");
		return AEResult::Fail;
	}

	////////////////////////////
	//Get PhysX Managers
	physx::PxPhysics* pxPhysics = physicsManager->GetPhysX();
	physx::PxScene* pxScene = physicsManager->GetPxScene();

	////////////////////////////
	//Get Objs Position and Rotation
	physx::PxTransform pxTransform;
	GetPXTransformFromObject3D(pxTransform);

	////////////////////////////
	//Create PhysX Actor
	switch (physicsActorType)
	{
		case PhysicsActorType::Dynamic:
			{
				physx::PxRigidDynamic* rigidDynamic = pxPhysics->createRigidDynamic(pxTransform);

				if (InitDynamicObject(rigidDynamic) != AEResult::Ok)
				{
					ReleasePhysX(rigidDynamic);

					AETODO("Better ret code");
					return AEResult::Fail;
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
			AEAssert(false);
			return AEResult::InvalidObjType;
	}

	////////////////////////////////
	//Finish
	m_PhysicsActorType = physicsActorType;
	m_PhysicsManager = physicsManager;

	m_IsReady = true;

	return AEResult::Ok;
}

AEResult PhysicsActor::InitDynamicObject(physx::PxRigidDynamic* rigidDynamic)
{
	AEAssert(rigidDynamic != nullptr);
	if (rigidDynamic == nullptr)
	{
		return AEResult::NullParameter;
	}

	rigidDynamic->setMass(m_Mass);
	rigidDynamic->setAngularDamping(m_AngularDamping);
	rigidDynamic->setLinearDamping(m_LinearDamping);
	rigidDynamic->setRigidBodyFlag(physx::PxRigidBodyFlag::eKINEMATIC, m_Kinematic);
	rigidDynamic->setActorFlag(physx::PxActorFlag::eDISABLE_GRAVITY, !m_GravityEnable);

	return AEResult::Ok;
}

AEResult PhysicsActor::ChangePhysicsActorType(PhysicsActorType physicsActorType)
{
	////////////////////////////
	//Pre-checks
	if (!m_IsReady)
	{
		return AEResult::NotReady;
	}

	if (m_IsReady && m_PhysicsActorType == physicsActorType)
	{
		return AEResult::Ok;
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

			if (InitDynamicObject(rigidDynamic) != AEResult::Ok)
			{
				ReleasePhysX(rigidDynamic);

				AETODO("Better ret code");
				return AEResult::Fail;
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
			AEAssert(false);
			return AEResult::InvalidObjType;
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

	return AEResult::Ok;
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

AEResult PhysicsActor::AddPhysicCollider(PhysicCollider* physicCollider)
{
	if (!m_IsReady)
	{
		return AEResult::NotReady;
	}

	AEAssert(physicCollider != nullptr);
	if (physicCollider == nullptr)
	{
		return AEResult::NullParameter;
	}

	if (!physicCollider->IsReady())
	{
		AETODO("Add better return code");
		return AEResult::Fail;
	}

	if (ExistsPhysicCollider(physicCollider->GetUniqueID()))
	{
		return AEResult::ObjExists;
	}

	m_PxRigidActor->attachShape(*physicCollider->GetPxShape());

	m_PhysicColliderMap[physicCollider->GetUniqueID()] = physicCollider;

	return AEResult::Ok;
}

AEResult PhysicsActor::RemovePhysicCollider(uint64_t id, bool deleteObj)
{
	if (!ExistsPhysicCollider(id))
	{
		return AEResult::NotFound;
	}

	PhysicCollider* physicCollider = m_PhysicColliderMap[id];

	m_PxRigidActor->detachShape(*physicCollider->GetPxShape());

	m_PhysicColliderMap.erase(id);

	if (deleteObj)
	{
		DeleteMem(physicCollider);
	}

	return AEResult::Ok;
}

AEResult PhysicsActor::UpdateObject3D()
{
	if (!m_IsReady)
	{
		return AEResult::NotReady;
	}

	/////////////////////////////////////////////
	//Get Actor Pose
	physx::PxTransform pxTransform = m_PxRigidActor->getGlobalPose();

	/////////////////////////////////////////////
	//Get Parent Transforms
	glm::mat4 indentityScale		= AEMathHelpers::Mat4Identity;
	glm::mat4 parentWorldTransform	= AEMathHelpers::Mat4Identity;
	glm::quat parentRotQuat			= AEMathHelpers::QuaternionIdentity;

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
	glm::vec3 currentRot		= glm::degrees(AEMathHelpers::GetEulerAnglesFromQuaternionRotation(currentRotQuat));
	glm::vec3 currentScale		= AEMathHelpers::Vec3fOne;

	glm::mat4 currentTransform	= AEMathHelpers::CreateTransform(currentPos, currentRot, currentScale);

	/////////////////////////////////////////////
	//New Transforms
	glm::vec3 newPosition	= AEMathHelpers::Vec3fZero;
	glm::vec3 newRotation	= AEMathHelpers::Vec3fZero;
	glm::vec3 newScale		= AEMathHelpers::Vec3fZero;

	/////////////////////////////////////////////
	//Get Positions
	AEMathHelpers::GetTransformationFromRelativeToParent(parentWorldTransform, parentRotQuat, indentityScale, currentRotQuat, currentTransform, indentityScale, newPosition, newRotation, newScale);

	/////////////////////////////////////////////
	//Set to Object 3D
	m_Object3D->SetPosition(newPosition, this->GetUniqueID());
	m_Object3D->SetRotation(newRotation, this->GetUniqueID());

	/////////////////////////////////////////////
	//Finish
	return AEResult::Ok;
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
