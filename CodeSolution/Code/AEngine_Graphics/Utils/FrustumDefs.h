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
#define AE_FRUSTUM_PLANE_NUM            6

/// <summary>
/// Number of Frustum Corners
/// </summary>
#define AE_FRUSTUM_CORNER_NUM           8

/// <summary>
/// Near Plane of the Frustum
/// </summary>
#define AE_FRUSTUM_NEAR_PLANE           0

/// <summary>
/// Far Plane of the Frustum
/// </summary>
#define AE_FRUSTUM_FAR_PLANE            1

/// <summary>
/// Left Plane of the Frustum
/// </summary>
#define AE_FRUSTUM_LEFT_PLANE           2

/// <summary>
/// Right Plane of the Frustum
/// </summary>
#define AE_FRUSTUM_RIGHT_PLANE          3

/// <summary>
/// Top Plane of the Frustum
/// </summary>
#define AE_FRUSTUM_TOP_PLANE            4

/// <summary>
/// Bottom Plane of the Frustum
/// </summary>
#define AE_FRUSTUM_BOTTOM_PLANE         5

/// <summary>
/// Near Frustum Corner Bottom Right
/// </summary>
#define AE_FRUSTUM_NEAR_BOTTOM_RIGHT    0

/// <summary>
/// Near Frustum Corner Top Right
/// </summary>
#define AE_FRUSTUM_NEAR_TOP_RIGHT       1

/// <summary>
/// Near Frustum Corner Top Left
/// </summary>
#define AE_FRUSTUM_NEAR_TOP_LEFT        2

/// <summary>
/// Near Frustum Corner Bottom Left
/// </summary>
#define AE_FRUSTUM_NEAR_BOTTOM_LEFT     3

/// <summary>
/// Far Frustum Corner Bottom Right
/// </summary>
#define AE_FRUSTUM_FAR_BOTTOM_RIGHT     4

/// <summary>
/// Far Frustum Corner Top Right
/// </summary>
#define AE_FRUSTUM_FAR_TOP_RIGHT        5
/// <summary>
/// Far Frustum Corner Top Left
/// </summary>
#define AE_FRUSTUM_FAR_TOP_LEFT         6

/// <summary>
/// Far Frustum Corner Bottom Right
/// </summary>
#define AE_FRUSTUM_FAR_BOTTOM_LEFT      7

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
