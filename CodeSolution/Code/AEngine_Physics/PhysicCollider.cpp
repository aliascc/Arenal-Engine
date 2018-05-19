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

/*************************
*   Precompiled Header   *
**************************/
#include "precomp_physics.h"

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

//Always include last
#include "Memory\MemLeaks.h"

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

AEResult PhysicCollider::Initialize(PhysicsManager* physicsManager)
{
    ////////////////////////////
    //Pre-checks
    if (m_IsReady)
    {
        return AEResult::Ok;
    }

    AEAssert(physicsManager != nullptr);
    if (physicsManager == nullptr)
    {
        return AEResult::NullParameter;
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
    //Clean Memory
    CleanUp();

    ////////////////////////////
    //Create PhysX Actor
    physx::PxGeometry& pxGeometry = CreateGeomtry(pxPhysics);

    AETODO("Make material in a manager");
    m_PxMaterial = pxPhysics->createMaterial(AE_PHYSIC_STATIC_FRICTION_DEFAULT, AE_PHYSIC_DYNAMIC_FRICTION_DEFAULT, AE_PHYSIC_RESTITUTION_DEFAULT);

    m_PxShape = pxPhysics->createShape(pxGeometry, *m_PxMaterial, true);

    ////////////////////////////////
    //Remove Memory

    ////////////////////////////////
    //Finish
    m_IsReady = true;

    return AEResult::Ok;
}
