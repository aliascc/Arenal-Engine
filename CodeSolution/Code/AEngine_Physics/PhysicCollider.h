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

#ifndef _PHYSIC_COLLIDER_H
#define _PHYSIC_COLLIDER_H

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

/********************
*   Forward Decls   *
*********************/
class Object3D;
class PhysicsManager;

/*****************
*   Class Decl   *
******************/

class PhysicCollider abstract : public AEObject
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
