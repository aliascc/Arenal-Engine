/********************************************************
*
* Author: Carlos Chacón N.
*
* Copyright 2012-2015
*
*********************************************************/

#pragma once

#ifndef _GRAPHIC_UTILS_DEFS_H
#define _GRAPHIC_UTILS_DEFS_H

/**********************
*   System Includes   *
***********************/
#include <stdint.h>
#include <functional>

/***************************
*   Game Engine Includes   *
****************************/

/********************
*   Forward Decls   *
*********************/
class Object3D;

/************
*   Using   *
*************/

/**************
*   Defines   *
***************/

/****************
*   Constants   *
*****************/

/************
*   Enums   *
*************/

/// <summary>
/// Type of Object 3D Change Event
/// </summary>
enum class Object3DChangeEventType : uint32_t
{
	PositionChange = 0,
	RotationChange,
	ScaleChange,
	ParentChange
};

/*************
*   Struct   *
**************/

/**************
*   Typedef   *
***************/
typedef std::function<void(Object3DChangeEventType, Object3D*)> Object3DChangedEvent;


#endif