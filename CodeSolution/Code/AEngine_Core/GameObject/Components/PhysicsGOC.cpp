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
#include "precomp_core.h"

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
AETODO("Add mutex");
PhysicsGOC::PhysicsGOC(GameObject* gameObject, PhysicsManager* physicsManager)
    : GameObjectComponent(gameObject, GameObjectComponentType::Physics)
    , m_PhysicsManager(physicsManager)
{
    AEAssert(m_PhysicsManager != nullptr);
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
        AETODO("Check return");
        m_PhysicsManager->RemovePhysicsActor(m_PhysicsActor->GetUniqueID(), false);
    }
    DeleteMem(m_PhysicsActor);
}

AEResult PhysicsGOC::AddRigidBody()
{
    if (!m_IsReady)
    {
        return AEResult::NotReady;
    }

    AEResult ret = AEResult::Ok;

    if (m_PhysicsActor == nullptr)
    {
        m_PhysicsActor = new PhysicsActor(m_GameObject);

        ret = m_PhysicsActor->Initialize(m_PhysicsManager, PhysicsActorType::Dynamic);
        if (ret != AEResult::Ok)
        {
            DeleteMem(m_PhysicsActor);

            return ret;
        }

        //////////////////////////////
        //Add to Physics Manager
        ret = m_PhysicsManager->AddPhysicsActor(m_PhysicsActor);
        if (ret != AEResult::Ok)
        {
            DeleteMem(m_PhysicsActor);

            return ret;
        }
    }
    else if (m_PhysicsActor->GetPhysicsActorType() != PhysicsActorType::Dynamic)
    {
        ret = m_PhysicsActor->ChangePhysicsActorType(PhysicsActorType::Dynamic);
        if (ret != AEResult::Ok)
        {
            m_PhysicsManager->RemovePhysicsActor(m_PhysicsActor->GetUniqueID(), false);

            DeleteMem(m_PhysicsActor);

            return ret;
        }
    }

    return AEResult::Ok;
}

AEResult PhysicsGOC::RemoveRigidBody()
{
    if (!m_IsReady)
    {
        return AEResult::NotReady;
    }

    if (m_PhysicsActor == nullptr)
    {
        return AEResult::Ok;
    }

    if (m_PhysicsActor->GetPhysicsActorType() != PhysicsActorType::Dynamic)
    {
        return AEResult::Ok;
    }

    if (m_PhysicsActor->GetNumberOfColliders() == 0)
    {
        DeleteMem(m_PhysicsActor);
    }
    else
    {
        AEResult ret = m_PhysicsActor->ChangePhysicsActorType(PhysicsActorType::Static);
        if (ret != AEResult::Ok)
        {
            DeleteMem(m_PhysicsActor);

            return ret;
        }
    }

    return AEResult::Ok;
}

AEResult PhysicsGOC::AddCollider(CollisionShape collisionShape, uint64_t& colliderID)
{
    if (!m_IsReady)
    {
        return AEResult::NotReady;
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
            AEAssert(false);
            return AEResult::InvalidObjType;
    }

    AEResult ret = AEResult::Ok;

    ///////////////////////////////
    //Initialize
    ret = physicCollider->Initialize(m_PhysicsManager);
    if (ret != AEResult::Ok)
    {
        DeleteMem(physicCollider);

        return ret;
    }

    ///////////////////////////////
    //Initialize Physics Actor
    if (m_PhysicsActor == nullptr)
    {
        m_PhysicsActor = new PhysicsActor(m_GameObject);

        ret = m_PhysicsActor->Initialize(m_PhysicsManager, PhysicsActorType::Static);
        if (ret != AEResult::Ok)
        {
            DeleteMem(physicCollider);

            DeleteMem(m_PhysicsActor);

            return ret;
        }

        //////////////////////////////
        //Add to Physics Manager
        ret = m_PhysicsManager->AddPhysicsActor(m_PhysicsActor);
        if (ret != AEResult::Ok)
        {
            DeleteMem(physicCollider);

            DeleteMem(m_PhysicsActor);

            return ret;
        }
    }

    ///////////////////////////////
    //Add
    ret = m_PhysicsActor->AddPhysicCollider(physicCollider);
    if (ret != AEResult::Ok)
    {
        DeleteMem(physicCollider);

        return ret;
    }

    colliderID = physicCollider->GetUniqueID();

    return AEResult::Ok;
}

bool PhysicsGOC::IsRigidBody()
{
    if (!m_IsReady || m_PhysicsActor == nullptr)
    {
        return false;
    }

    return (m_PhysicsActor->GetPhysicsActorType() == PhysicsActorType::Dynamic);
}
