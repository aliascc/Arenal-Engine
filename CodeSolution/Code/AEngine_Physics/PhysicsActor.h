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

#pragma once

#ifndef _PHYSIC_ACTOR_H
#define _PHYSIC_ACTOR_H

/**********************
*   System Includes   *
***********************/

/*************************
*   3rd Party Includes   *
**************************/

/***************************
*   Game Engine Includes   *
****************************/
#include "PhysicsDefs.h"
#include "Utils\GraphicUtilsDefs.h"

/********************
*   Forward Decls   *
*********************/
class Object3D;
class PhysicsManager;
class PhysicCollider;

/**************
*   Typedef   *
***************/
typedef std::map<uint64_t, PhysicCollider*> PhysicColliderMap;

/*****************
*   Class Decl   *
******************/

class PhysicsActor sealed : public UniqueAEObject
{
    private:

        /************************
        *   Private Variables   *
        *************************/
#pragma region Private Variables

        float m_Mass = 1.0f;

        bool m_GravityEnable = true;

        bool m_Kinematic = false;

        float m_LinearDamping = AE_PHYSIC_LINEAR_DAMPING_DEFAULT;

        float m_AngularDamping = AE_PHYSIC_ANGULAR_DAMPING_DEFAULT;

        Object3D* m_Object3D = nullptr;

        physx::PxRigidActor* m_PxRigidActor = nullptr;

        PhysicsManager* m_PhysicsManager = nullptr;

        PhysicsActorType m_PhysicsActorType = PhysicsActorType::Static;

        bool m_IsReady = false;

        PhysicColliderMap m_PhysicColliderMap;

        Object3DChangedEvent m_Object3DChangedEvent;

        void GetPXTransformFromObject3D(physx::PxTransform& pxTransform);

#pragma endregion

        /**********************
        *   Private Methods   *
        ***********************/
#pragma region Private Methods

        void CleanUp();

        void Object3DChanged(Object3DChangeEventType changeType, Object3D* object3D);

        AEResult InitDynamicObject(physx::PxRigidDynamic* rigidDynamic);

#pragma endregion

    public:

        /***************************************
        *   Constructor & Destructor Methods   *
        ****************************************/
#pragma region Constructor & Destructor Methods

        /// <summary>
        /// Default PhysicsActor Constructor
        /// </summary>
        /// <param name="object3D">Object 3D to associate to the Physics Actor</param>
        PhysicsActor(Object3D* object3D);

        /// <summary>
        /// Default PhysicsActor Destructor
        /// </summary>
        virtual ~PhysicsActor();

#pragma endregion

        /******************
        *   Get Methods   *
        *******************/
#pragma region Get Methods

        inline bool IsReady() const
        {
            return m_IsReady;
        }

        inline const PhysicColliderMap& GetPhysicColliderMap()
        {
            return m_PhysicColliderMap;
        }

        inline physx::PxRigidActor* GetPxRigidActor() const
        {
            return m_PxRigidActor;
        }

        inline float GetMass() const
        {
            return m_Mass;
        }

        inline bool GetGravityEnable() const
        {
            return m_GravityEnable;
        }

        inline bool GetKinematic() const
        {
            return m_Kinematic;
        }

        inline float GetLinearDamping() const
        {
            return m_LinearDamping;
        }

        inline float GetAngularDamping() const
        {
            return m_AngularDamping;
        }

        inline PhysicsActorType GetPhysicsActorType() const
        {
            return m_PhysicsActorType;
        }

        inline uint32_t GetNumberOfColliders() const
        {
            return (uint32_t)m_PhysicColliderMap.size();
        }

#pragma endregion

        /******************
        *   Set Methods   *
        *******************/
#pragma region Set Methods

        void SetMass(float mass);

        void SetGravityEnable(bool gravityEnable);

        void SetKinematic(bool kinematic);

        void SetLinearDamping(float linearDamping);

        void SetAngularDamping(float angularDamping);

#pragma endregion

        /************************
        *   Framework Methods   *
        *************************/
#pragma region Framework Methods

        AEResult Initialize(PhysicsManager* physicsManager, PhysicsActorType PhysicsActorType);

        AEResult ChangePhysicsActorType(PhysicsActorType physicsActorType);

        AEResult AddPhysicCollider(PhysicCollider* physicCollider);

        AEResult RemovePhysicCollider(uint64_t id, bool deleteObj = true);

        bool ExistsPhysicCollider(uint64_t id) const;

        AEResult UpdateObject3D();

        PhysicCollider* GetPhysicCollider(uint64_t id);

#pragma endregion

};

#endif
