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

#ifndef _PHYSICS_GOC_H
#define _PHYSICS_GOC_H

/**********************
*   System Includes   *
***********************/

/*************************
*   3rd Party Includes   *
**************************/

/***************************
*   Game Engine Includes   *
****************************/
#include "Base\Base.h"
#include "PhysicsDefs.h"
#include "GameObject\GameObjectComponent.h"

/********************
*   Forward Decls   *
*********************/
class PhysicsActor;
class PhysicsManager;

/*****************
*   Class Decl   *
******************/

class PhysicsGOC sealed : public GameObjectComponent
{
	private:

		/************************
		*   Private Variables   *
		*************************/
#pragma region Private Variables

		PhysicsManager* m_PhysicsManager = nullptr;

		PhysicsActor* m_PhysicsActor = nullptr;

#pragma endregion

		/**********************
		*   Private Methods   *
		***********************/
#pragma region Private Methods;

#pragma endregion

	public:

		/***************************************
		*   Constructor & Destructor Methods   *
		****************************************/
#pragma region Constructor & Destructor Methods

		/// <summary>
		/// PhysicsGOC Constructor
		/// </summary>
		/// <param name="gameObject">Game Object that this Component is attached too</param>
		/// <param name="physcisManager">Physics Manager to add the actor</param>
		PhysicsGOC(GameObject* gameObject, PhysicsManager* physcisManager);

		/// <summary>
		/// Default PhysicsGOC Destructor
		/// </summary>
		virtual ~PhysicsGOC();

#pragma endregion

		/******************
		*   Get Methods   *
		*******************/
#pragma region Get Methods

		/// <summary>
		/// Gets the Light Object
		/// </summary>
		/// <returns>Returns the Light Object</returns>
		inline PhysicsActor* GetPhysicsActor() const
		{ 
			return m_PhysicsActor;
		}

#pragma endregion

		/******************
		*   Set Methods   *
		*******************/
#pragma region Set Methods

#pragma endregion

		/************************
		*   Framework Methods   *
		*************************/
#pragma region Framework Methods

		AEResult AddRigidBody();

		AEResult RemoveRigidBody();

		AEResult AddCollider(CollisionShape collisionShape, uint64_t& colliderID);

		bool IsRigidBody();

#pragma endregion

};

#endif