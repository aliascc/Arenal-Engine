/********************************************************
*
* Author: Carlos Chacón N.
*
* Copyright 2012-2015
*
*********************************************************/

#pragma once

#ifndef _SHAPES_DEFS_H
#define _SHAPES_DEFS_H

/**********************
*   System Includes   *
***********************/

/*************************
*   3rd Party Includes   *
**************************/

/***************************
*   Game Engine Includes   *
****************************/
#include "Color\AEColorDefs.h"

/**************
*   Defines   *
***************/

/// <summary>
/// Default Size for Directional Light Arrow
/// </summary>
#define AE_LIGHT_SHAPE_DIR_DEFAULT_ARROW_SIZE				1.0f

/// <summary>
/// Default Size for Directional Light Arrow Tips
/// </summary>
#define AE_LIGHT_SHAPE_DIR_DEFAULT_ARROW_TIP_SIZE			(AE_LIGHT_SHAPE_DIR_DEFAULT_ARROW_SIZE * 0.25f)

/// <summary>
/// Default Size for Directional Light Arrow Tips Height
/// </summary>
#define AE_LIGHT_SHAPE_DIR_DEFAULT_ARROW_TIP_HEIGHT_SIZE	(AE_LIGHT_SHAPE_DIR_DEFAULT_ARROW_SIZE * 0.1f)

/// <summary>
/// Default Number of Edges for Light
/// </summary>
#define AE_LIGHT_SHAPE_DEFAULT_NUM_EDGES					48

/// <summary>
/// Default Size for Omni Light Ring
/// </summary>
#define AE_LIGHT_SHAPE_OMNI_DEFAULT_RING_SIZE				1.0f

/// <summary>
/// Default Color for Debug Shape Lights Near Attenuation
/// </summary>
#define AE_LIGHT_SHAPE_DEBUG_DEFAULT_NEAR_COLOR				AEColors::LightYellow

/// <summary>
/// Default Color for Debug Shape Lights Far Attenuation
/// </summary>
#define AE_LIGHT_SHAPE_DEBUG_DEFAULT_FAR_COLOR				AEColors::Yellow

/// <summary>
/// Default Color for Debug Shape Spot Lights Inner Circle
/// </summary>
#define AE_LIGHT_SHAPE_DEBUG_DEFAULT_INNER_CIRCLE_COLOR		AEColors::Blue

/// <summary>
/// Default Color for Debug Shape Spot Lights Far Lines
/// </summary>
#define AE_LIGHT_SHAPE_DEBUG_DEFAULT_LINE_FAR_COLOR			AE_LIGHT_SHAPE_DEBUG_DEFAULT_FAR_COLOR

/// <summary>
/// Default Color for Debug Shape Spot Lights Near Lines
/// </summary>
#define AE_LIGHT_SHAPE_DEBUG_DEFAULT_LINE_NEAR_COLOR		AE_LIGHT_SHAPE_DEBUG_DEFAULT_NEAR_COLOR

/// <summary>
/// Default Color for Debug Shape Spot Lights Lines Inner Circle
/// </summary>
#define AE_LIGHT_SHAPE_DEBUG_DEFAULT_LINE_INNER_COLOR		AE_LIGHT_SHAPE_DEBUG_DEFAULT_INNER_CIRCLE_COLOR

/************
*   Using   *
*************/

/********************
*   Forward Decls   *
*********************/

/****************
*   Constants   *
*****************/

/******************
*   Struct Decl   *
*******************/

#endif