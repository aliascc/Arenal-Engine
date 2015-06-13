/********************************************************
*
* Author: Carlos Chacón N.
*
* Copyright 2012-2015
*
*********************************************************/

#pragma once

#ifndef _PHYSIC_COLLIDER_SPHERE_H
#define _PHYSIC_COLLIDER_SPHERE_H

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
#include "PhysicCollider.h"
#include "Math\AEMathDefs.h"

/********************
*   Forward Decls   *
*********************/
class Object3D;
class PhysicsManager;

/*****************
*   Class Decl   *
******************/

class PhysicColliderSphere sealed : public PhysicCollider
{
	private:

		/************************
		*   Private Variables   *
		*************************/
#pragma region Private Variables

		physx::PxSphereGeometry m_PxSphereGeometry;

		float m_Radius = 1.0f;

#pragma endregion

		/**********************
		*   Private Methods   *
		***********************/
#pragma region Private Methods

		physx::PxGeometry& CreateGeomtry(physx::PxPhysics* pxPhysics);

#pragma endregion

	public:

		/***************************************
		*   Constructor & Destructor Methods   *
		****************************************/
#pragma region Constructor & Destructor Methods

		/// <summary>
		/// Default PhysicColliderSphere Constructor
		/// </summary>
		PhysicColliderSphere();

		/// <summary>
		/// Default PhysicColliderSphere Destructor
		/// </summary>
		virtual ~PhysicColliderSphere();

#pragma endregion

		/******************
		*   Get Methods   *
		*******************/
#pragma region Get Methods

		inline float GetRadius() const
		{
			return m_Radius;
		}

#pragma endregion

		/******************
		*   Set Methods   *
		*******************/
#pragma region Set Methods

		void SetRadius(float radius);

		void SetScale(const glm::vec3& scale) override;

#pragma endregion

		/************************
		*   Framework Methods   *
		*************************/
#pragma region Framework Methods

#pragma endregion

};

#endif