/********************************************************
*
* Author: Carlos Chacón N.
*
* Copyright 2012-2015
*
*********************************************************/

#pragma once

#ifndef _PHYSIC_COLLIDER_H
#define _PHYSIC_COLLIDER_H

/**********************
*   System Includes   *
***********************/
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
#include "Math\AEMathDefs.h"
#include "Base\UniqueAEObject.h"

/********************
*   Forward Decls   *
*********************/
class Object3D;
class PhysicsManager;

/*****************
*   Class Decl   *
******************/

class PhysicCollider abstract : public UniqueAEObject
{
	private:

		/************************
		*   Private Variables   *
		*************************/
#pragma region Private Variables

		CollisionShape m_CollisionShape = CollisionShape::Box;

		physx::PxMaterial* m_PxMaterial = nullptr;

#pragma endregion

	protected:

		/**************************
		*   Protected Variables   *
		***************************/
#pragma region Protected Variables

		bool m_IsReady = false;

		glm::vec3 m_Scale = AEMathHelpers::Vec3fOne;

		physx::PxShape* m_PxShape = nullptr;

#pragma endregion

		/************************
		*   Protected Methods   *
		*************************/
#pragma region Protected Methods

		void CleanUp();

		virtual physx::PxGeometry& CreateGeomtry(physx::PxPhysics* pxPhysics) = 0;

#pragma endregion

	public:

		/***************************************
		*   Constructor & Destructor Methods   *
		****************************************/
#pragma region Constructor & Destructor Methods

		/// <summary>
		/// Default PhysicCollider Constructor
		/// </summary>
		PhysicCollider(CollisionShape m_CollisionShape);

		/// <summary>
		/// Default PhysicCollider Destructor
		/// </summary>
		virtual ~PhysicCollider();

#pragma endregion

		/******************
		*   Get Methods   *
		*******************/
#pragma region Get Methods

		inline bool IsReady() const
		{
			return m_IsReady;
		}

		inline physx::PxShape* GetPxShape() const
		{
			return m_PxShape;
		}

		inline CollisionShape GetCollisionShape() const
		{
			return m_CollisionShape;
		}

		inline const glm::vec3& GetScale() const
		{
			return m_Scale;
		}

#pragma endregion

		/******************
		*   Set Methods   *
		*******************/
#pragma region Set Methods

		virtual void SetScale(const glm::vec3& scale) = 0;

#pragma endregion

		/************************
		*   Framework Methods   *
		*************************/
#pragma region Framework Methods

		AEResult Initialize(PhysicsManager* physicsManager);

#pragma endregion

};

#endif