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
#include "Base\XEObject.h"
#include "Math\XEMathDefs.h"

/************
*   Using   *
*************/

/*******************
*   Forward Decl   *
********************/

/**************
*   Defines   *
***************/

#define XE_PHYSIC_MASS_DEFAULT					0.0f

#define XE_PHYSIC_LINEAR_DAMPING_DEFAULT		0.0f

#define XE_PHYSIC_ANGULAR_DAMPING_DEFAULT		0.05f

#define XE_PHYSIC_RESTITUTION_DEFAULT			0.5f

#define XE_PHYSIC_STATIC_FRICTION_DEFAULT		0.5f

#define XE_PHYSIC_DYNAMIC_FRICTION_DEFAULT		0.5f

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