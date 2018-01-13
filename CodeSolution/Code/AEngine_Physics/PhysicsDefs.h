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

#ifndef _PHYSICS_DEFS_H
#define _PHYSICS_DEFS_H

/**********************
*   System Includes   *
***********************/

/*************************
*   3rd Party Includes   *
**************************/
#include "glm\glm.hpp"

/***************************
*   Game Engine Includes   *
****************************/
#include "Base\AEObject.h"
#include "Math\AEMathDefs.h"

/************
*   Using   *
*************/

/*******************
*   Forward Decl   *
********************/

/**************
*   Defines   *
***************/

#define AE_PHYSIC_MASS_DEFAULT					0.0f

#define AE_PHYSIC_LINEAR_DAMPING_DEFAULT		0.0f

#define AE_PHYSIC_ANGULAR_DAMPING_DEFAULT		0.05f

#define AE_PHYSIC_RESTITUTION_DEFAULT			0.5f

#define AE_PHYSIC_STATIC_FRICTION_DEFAULT		0.5f

#define AE_PHYSIC_DYNAMIC_FRICTION_DEFAULT		0.5f

#define AE_PHYSIC_GET_SIZE_ALIGN(x)				(x + PX_SERIAL_FILE_ALIGN)

#define AE_PHYSIC_GET_MEM_ALIGN(x)				( (void*)((size_t(x) + PX_SERIAL_FILE_ALIGN) & ~(PX_SERIAL_FILE_ALIGN - 1)) )

/****************
*   Constants   *
*****************/

/************
*   Enums   *
*************/

enum class PhysicsActorType : uint8_t
{
	Dynamic = 0,
	Static
};

enum class CollisionShape : uint8_t
{
	Box = 0,
	Sphere,
	Mesh
};

/*************
*   Struct   *
**************/

/**************
*   Typedef   *
***************/


#endif