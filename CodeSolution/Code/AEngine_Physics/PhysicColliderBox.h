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

#ifndef _PHYSIC_COLLIDER_BOX_H
#define _PHYSIC_COLLIDER_BOX_H

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
#include "PhysicCollider.h"

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

        glm::vec3 m_Size = AEMathHelpers::Vec3fOne;

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
