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

#ifndef _GAME_OBJECT_DEBUG_DEFS_H
#define _GAME_OBJECT_DEBUG_DEFS_H

/**********************
*   System Includes   *
***********************/

/*************************
*   3rd Party Includes   *
**************************/

/***************************
*   Game Engine Includes   *
****************************/

/**************
*   Defines   *
***************/

/// <summary>
/// Name for Basic Colro Material in Visualizer
/// </summary>
#define AE_GOD_V_BASIC_COLOR_MAT_NAME                    "AE GOD V Basic Color Material"

/// <summary>
/// Name for Basic Line Material in Visualizer
/// </summary>
#define AE_GOD_V_BASIC_LINE_MAT_NAME                    "AE GOD V Basic Line Material"

/// <summary>
/// Name for Diffuse Texture Basic Material in Visualizer
/// </summary>
#define AE_GOD_V_DIFFUSE_TEXTURE_BASIC_MAT_NAME         "AE GOD V Diffuse Texture Basic Material"

/// <summary>
/// Name for Directional Light Icon Texture
/// </summary>
#define AE_GOD_DIRECTIONAL_LIGHT_ICON_TEXTURE_NAME      "AE Directional Light Icon"

/// <summary>
/// Name for Omni Light Icon Texture
/// </summary>
#define AE_GOD_OMNI_LIGHT_ICON_TEXTURE_NAME             "AE Omni Light Icon"

/// <summary>
/// Name for Spot Light Icon Texture
/// </summary>
#define AE_GOD_SPOT_LIGHT_ICON_TEXTURE_NAME             "AE Spot Light Icon"

/// <summary>
/// Name for Camera Icon Texture
/// </summary>
#define AE_GOD_CAMERA_ICON_TEXTURE_NAME                 "AE Camera Icon"

/// <summary>
/// File path for Directional Light Icon Texture
/// </summary>
#define AE_GOD_DIRECTIONAL_LIGHT_ICON_PATH              AE_Base_FS_PATH "Data\\Textures\\DirLightIcon.dds"

/// <summary>
/// File path for Omni Light Icon Texture
/// </summary>
#define AE_GOD_OMNI_LIGHT_ICON_PATH                     AE_Base_FS_PATH "Data\\Textures\\OmniLightIcon.dds"

/// <summary>
/// File path for Spot Light Icon Texture
/// </summary>
#define AE_GOD_SPOT_LIGHT_ICON_PATH                     AE_Base_FS_PATH "Data\\Textures\\SpotLightIcon.dds"

/// <summary>
/// File path for Camera Icon Texture
/// </summary>
#define AE_GOD_CAMERA_ICON_PATH                         AE_Base_FS_PATH "Data\\Textures\\VideoCameraIcon.dds"

/// <summary>
/// Scale Amount for Light Icons
/// </summary>
#define AE_LIGHT_ICONS_DEFAULT_SCALE_AMOUNT             0.5f

/// <summary>
/// Scale Amount for Directional Light Shape
/// </summary>
#define AE_LIGHT_SHAPE_DIR_DEFAULT_SCALE_AMOUNT         2.0f

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
