
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
#pragma once

#ifndef _PHYSIC_ACTOR_H
#define _PHYSIC_ACTOR_H

/**********************
*   System Includes   *
***********************/
#include <map>
#include <stdint.h>

/*************************
*   3rd Party Includes   *
**************************/
#include "glm\glm.hpp"
#include "PxPhysicsAPI.h"

/***************************
*   Game Engine Includes   *
****************************/
#include "Base\Base.h"
#include "PhysicsDefs.h"
#include "Math\XEMathDefs.h"
#include "Base\UniqueXEObject.h"

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

class PhysicsActor sealed : public UniqueXEObject
{
	private:

		/************************
		*   Private Variables   *
		*************************/
#pragma region Private Variables

		float m_Mass = 1.0f;

		bool m_GravityEnable = true;

		bool m_Kinematic = false;

		float m_LinearDamping = XE_PHYSIC_LINEAR_DAMPING_DEFAULT;

		float m_AngularDamping = XE_PHYSIC_ANGULAR_DAMPING_DEFAULT;

		Object3D* m_Object3D = nullptr;

		physx::PxRigidActor* m_PxRigidActor = nullptr;

		PhysicsManager* m_PhysicsManager = nullptr;

		PhysicsActorType m_PhysicsActorType = PhysicsActorType::Static;

		bool m_IsReady = false;

		PhysicColliderMap m_PhysicColliderMap;

#pragma endregion

		/**********************
		*   Private Methods   *
		***********************/
#pragma region Private Methods

		void CleanUp();

		XEResult InitDynamicObject(physx::PxRigidDynamic* rigidDynamic);

#pragma endregion

	public:

		/***************************************
		*   Constructor & Destructor Methods   *
		****************************************/
#pragma region Constructor & Destructor Methods

		/// <summary>
		/// Default PhysicsActor Constructor
		/// </summary>
		PhysicsActor();

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

		XEResult SetObject3D(Object3D* object3D);

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

		XEResult Initialize(PhysicsManager* physicsManager, PhysicsActorType PhysicsActorType);

		XEResult ChangePhysicsActorType(PhysicsActorType physicsActorType);

		XEResult AddPhysicCollider(PhysicCollider* physicCollider);

		XEResult RemovePhysicCollider(uint64_t id, bool deleteObj = true);

		bool ExistsPhysicCollider(uint64_t id) const;

		XEResult UpdateObject3D();

		PhysicCollider* GetPhysicCollider(uint64_t id);

#pragma endregion

};

#endif