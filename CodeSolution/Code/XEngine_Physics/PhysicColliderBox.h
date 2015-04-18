
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

#ifndef _PHYSIC_COLLIDER_BOX_H
#define _PHYSIC_COLLIDER_BOX_H

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
#include "Math\XEMathDefs.h"

/********************
*   Forward Decls   *
*********************/
class Object3D;
class PhysicsManager;

/*****************
*   Class Decl   *
******************/

class PhysicColliderBox sealed : public PhysicCollider
{
	private:

		/************************
		*   Private Variables   *
		*************************/
#pragma region Private Variables

		physx::PxBoxGeometry m_PxBoxGeometry;

#pragma endregion

		/**********************
		*   Private Methods   *
		***********************/
#pragma region Private Methods

		glm::vec3 m_Size = XEMathHelpers::Vec3fOne;

		physx::PxGeometry& CreateGeomtry(physx::PxPhysics* pxPhysics);

#pragma endregion

	public:

		/***************************************
		*   Constructor & Destructor Methods   *
		****************************************/
#pragma region Constructor & Destructor Methods

		/// <summary>
		/// Default PhysicColliderBox Constructor
		/// </summary>
		PhysicColliderBox();

		/// <summary>
		/// Default PhysicColliderBox Destructor
		/// </summary>
		virtual ~PhysicColliderBox();

#pragma endregion

		/******************
		*   Get Methods   *
		*******************/
#pragma region Get Methods

		inline const glm::vec3& GetSize() const
		{
			return m_Size;
		}

#pragma endregion

		/******************
		*   Set Methods   *
		*******************/
#pragma region Set Methods

		void SetSize(const glm::vec3& size);

		void SetScale(const glm::vec3& scale) override;

#pragma endregion

		/************************
		*   Framework Methods   *
		*************************/
#pragma region Framework Methods

#pragma endregion

};

#endif