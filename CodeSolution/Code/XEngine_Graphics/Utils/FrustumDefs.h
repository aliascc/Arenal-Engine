/********************************************************
*
* Author: Carlos Chacón N.
*
* Copyright 2012-2015
*
*********************************************************/

#pragma once

#ifndef _FRUSTUM_DEFS_H
#define _FRUSTUM_DEFS_H

/**********************
*   System Includes   *
***********************/
#include <stdint.h>

/*************************
*   3rd Party Includes   *
**************************/

/***************************
*   Game Engine Includes   *
****************************/

/********************
*   Forward Decls   *
*********************/

/**************
*   Defines   *
***************/

/// <summary>
/// Number of Frustum Planes
/// </summary>
#define XE_FRUSTUM_PLANE_NUM			6

/// <summary>
/// Number of Frustum Corners
/// </summary>
#define XE_FRUSTUM_CORNER_NUM			8

/// <summary>
/// Near Plane of the Frustum
/// </summary>
#define XE_FRUSTUM_NEAR_PLANE			0

/// <summary>
/// Far Plane of the Frustum
/// </summary>
#define XE_FRUSTUM_FAR_PLANE			1

/// <summary>
/// Left Plane of the Frustum
/// </summary>
#define XE_FRUSTUM_LEFT_PLANE			2

/// <summary>
/// Right Plane of the Frustum
/// </summary>
#define XE_FRUSTUM_RIGHT_PLANE			3

/// <summary>
/// Top Plane of the Frustum
/// </summary>
#define XE_FRUSTUM_TOP_PLANE			4

/// <summary>
/// Bottom Plane of the Frustum
/// </summary>
#define XE_FRUSTUM_BOTTOM_PLANE			5

/// <summary>
/// Near Frustum Corner Bottom Right
/// </summary>
#define XE_FRUSTUM_NEAR_BOTTOM_RIGHT	0

/// <summary>
/// Near Frustum Corner Top Right
/// </summary>
#define XE_FRUSTUM_NEAR_TOP_RIGHT		1

/// <summary>
/// Near Frustum Corner Top Left
/// </summary>
#define XE_FRUSTUM_NEAR_TOP_LEFT		2

/// <summary>
/// Near Frustum Corner Bottom Left
/// </summary>
#define XE_FRUSTUM_NEAR_BOTTOM_LEFT		3

/// <summary>
/// Far Frustum Corner Bottom Right
/// </summary>
#define XE_FRUSTUM_FAR_BOTTOM_RIGHT		4

/// <summary>
/// Far Frustum Corner Top Right
/// </summary>
#define XE_FRUSTUM_FAR_TOP_RIGHT		5
/// <summary>
/// Far Frustum Corner Top Left
/// </summary>
#define XE_FRUSTUM_FAR_TOP_LEFT			6

/// <summary>
/// Far Frustum Corner Bottom Right
/// </summary>
#define XE_FRUSTUM_FAR_BOTTOM_LEFT		7

/****************
*   Constants   *
*****************/

/************
*   Enums   *
*************/

/******************
*   Struct Decl   *
*******************/

#endif