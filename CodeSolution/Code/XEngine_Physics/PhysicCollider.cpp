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
#include "PhysicCollider.h"
#include "PhysicsManager.h"
#include "Utils\Object3D.h"
#include "Base\BaseFunctions.h"

#if defined(_DEBUG)
#include "Memory\MemLeaks.h"
#endif

/********************
*   Function Defs   *
*********************/

PhysicCollider::PhysicCollider(CollisionShape collisionShape)
	: m_CollisionShape(collisionShape)
{
}

PhysicCollider::~PhysicCollider()
{
	CleanUp();
}

void PhysicCollider::CleanUp()
{
	ReleasePhysX(m_PxShape);

	ReleasePhysX(m_PxMaterial);
}

XEResult PhysicCollider::Initialize(PhysicsManager* physicsManager)
{
	////////////////////////////
	//Pre-checks
	if (m_IsReady)
	{
		return XEResult::Ok;
	}

	XEAssert(physicsManager != nullptr);
	if (physicsManager == nullptr)
	{
		return XEResult::NullParameter;
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
	//Clean Memory
	CleanUp();

	////////////////////////////
	//Create PhysX Actor
	physx::PxGeometry& pxGeometry = CreateGeomtry(pxPhysics);

	XETODO("Make material in a manager");
	m_PxMaterial = pxPhysics->createMaterial(XE_PHYSIC_STATIC_FRICTION_DEFAULT, XE_PHYSIC_DYNAMIC_FRICTION_DEFAULT, XE_PHYSIC_RESTITUTION_DEFAULT);

	m_PxShape = pxPhysics->createShape(pxGeometry, *m_PxMaterial, true);

	////////////////////////////////
	//Remove Memory

	////////////////////////////////
	//Finish
	m_IsReady = true;

	return XEResult::Ok;
}
