/********************************************************
*
* Author: Carlos Chacón N.
*
* Copyright 2012-2015
*
*********************************************************/

#pragma once

#ifndef _GAME_OBJECT_DEBUG_DEFS_H
#define _GAME_OBJECT_DEBUG_DEFS_H

/**********************
*   System Includes   *
***********************/

/*************************
*   3rd Party Includes   *
**************************/
#include "Base\AEObject.h"
#include "Color\AEColorDefs.h"

/***************************
*   Game Engine Includes   *
****************************/

/**************
*   Defines   *
***************/

/// <summary>
/// Name for Basic Colro Material in Visualizer
/// </summary>
#define AE_GOD_V_BASIC_COLOR_MAT_NAME					L"AE GOD V Basic Color Material"

/// <summary>
/// Name for Basic Line Material in Visualizer
/// </summary>
#define AE_GOD_V_BASIC_LINE_MAT_NAME					L"AE GOD V Basic Line Material"

/// <summary>
/// Name for Diffuse Texture Basic Material in Visualizer
/// </summary>
#define AE_GOD_V_DIFFUSE_TEXTURE_BASIC_MAT_NAME			L"AE GOD V Diffuse Texture Basic Material"

/// <summary>
/// Name for Directional Light Icon Texture
/// </summary>
#define AE_GOD_DIRECTIONAL_LIGHT_ICON_TEXTURE_NAME		L"AE Directional Light Icon"

/// <summary>
/// Name for Omni Light Icon Texture
/// </summary>
#define AE_GOD_OMNI_LIGHT_ICON_TEXTURE_NAME				L"AE Omni Light Icon"

/// <summary>
/// Name for Spot Light Icon Texture
/// </summary>
#define AE_GOD_SPOT_LIGHT_ICON_TEXTURE_NAME				L"AE Spot Light Icon"

/// <summary>
/// Name for Camera Icon Texture
/// </summary>
#define AE_GOD_CAMERA_ICON_TEXTURE_NAME					L"AE Camera Icon"

/// <summary>
/// File path for Directional Light Icon Texture
/// </summary>
#define AE_GOD_DIRECTIONAL_LIGHT_ICON_PATH				AE_Base_FS_PATH L"Data\\Textures\\DirLightIcon.dds"

/// <summary>
/// File path for Omni Light Icon Texture
/// </summary>
#define AE_GOD_OMNI_LIGHT_ICON_PATH						AE_Base_FS_PATH L"Data\\Textures\\OmniLightIcon.dds"

/// <summary>
/// File path for Spot Light Icon Texture
/// </summary>
#define AE_GOD_SPOT_LIGHT_ICON_PATH						AE_Base_FS_PATH L"Data\\Textures\\SpotLightIcon.dds"

/// <summary>
/// File path for Camera Icon Texture
/// </summary>
#define AE_GOD_CAMERA_ICON_PATH							AE_Base_FS_PATH L"Data\\Textures\\VideoCameraIcon.dds"

/// <summary>
/// Scale Amount for Light Icons
/// </summary>
#define AE_LIGHT_ICONS_DEFAULT_SCALE_AMOUNT				0.5f

/// <summary>
/// Scale Amount for Directional Light Shape
/// </summary>
#define AE_LIGHT_SHAPE_DIR_DEFAULT_SCALE_AMOUNT			2.0f

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
struct GameObjectsDebugVisualizerConfig sealed : AEObject
{
	bool m_CameraIconDrawEnabled = true;
	bool m_LightIconDrawEnabled = true;
	bool m_GameObjectDebugRenderEnabled = true;
	float m_LightIconScale = AE_LIGHT_ICONS_DEFAULT_SCALE_AMOUNT;
	float m_DirectionalLightShapeScale = AE_LIGHT_SHAPE_DIR_DEFAULT_SCALE_AMOUNT;

	GameObjectsDebugVisualizerConfig()
	{
	}
};

#endif