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