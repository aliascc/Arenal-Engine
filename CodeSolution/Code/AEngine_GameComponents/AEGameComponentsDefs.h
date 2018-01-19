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

#ifndef _AE_GAME_COMPONENTS_DEFS_H
#define _AE_GAME_COMPONENTS_DEFS_H

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
/// Default Service Name for Camera Updater
/// </summary>
#define AE_CAMERA_UPDATER_DEF_SERVICE_NAME                  L"AE Camera Updater Service"

/// <summary>
/// Default Service Name for Input Handler
/// </summary>
#define AE_INPUT_HANDLER_DEF_SERVICE_NAME                   L"AE Input Handler Service"

/// <summary>
/// Default Service Name for Forward Plus Rendering Main
/// </summary>
#define AE_FORWARD_PLUS_MAIN_DEF_SERVICE_NAME               L"AE Forward Plus Rendering Main Service"

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////

/// <summary>
/// Default Component Name for Camera Updater
/// </summary>
#define AE_CAMERA_UPDATER_DEF_COMPONENT_NAME                L"AE Camera Updater Component"

/// <summary>
/// Default Game Component Name for Input Handler
/// </summary>
#define AE_INPUT_HANDLER_DEF_COMPONENT_NAME                 L"AE Input Handler Component"

/// <summary>
/// Default Game Component Name for Forward Plus Rendering Main
/// </summary>
#define AE_FORWARD_PLUS_MAIN_DEF_COMPONENT_NAME             L"AE Forward Plus Rendering Main Component"

/// <summary>
/// Default Game Component Name for Forward Plus Rendering Z Pre Pass
/// </summary>
#define AE_FORWARD_PLUS_Z_PRE_PASS_DEF_COMPONENT_NAME       L"AE Forward Plus Rendering Pre Z Pass Component"

/// <summary>
/// Default Game Component Name for Forward Plus Rendering Object Draw
/// </summary>
#define AE_FORWARD_PLUS_OBJECT_DRAW_DEF_COMPONENT_NAME      L"AE Forward Plus Rendering Object Draw Component"

/// <summary>
/// Default Game Component Name for Forward Plus Rendering Light Culling
/// </summary>
#define AE_FORWARD_PLUS_LIGHT_CULLING_DEF_COMPONENT_NAME    L"AE Forward Plus Light Culling Component"

/// <summary>
/// Default Game Component Name for Game Lights Update
/// </summary>
#define AE_GAME_LIGHTS_UPDATE_DEF_COMPONENT_NAME            L"AE Game Lights Update Component"

/// <summary>
/// Default Game Component Name for Game Object Script Caller
/// </summary>
#define AE_GAME_GAME_OBJECT_SCRIPT_CALLER_COMPONENT_NAME    L"AE Game Object Script Caller Component"

/// <summary>
/// Default Game Component Name for Game Animations Update
/// </summary>
#define AE_GAME_ANIMATIONS_UPDATE_DEF_COMPONENT_NAME        L"AE Game Animations Update Component"

/// <summary>
/// Default Game Component Name for Game Audio Update
/// </summary>
#define AE_GAME_AUDIO_UPDATE_DEF_COMPONENT_NAME             L"AE Game Audio Update Component"

/// <summary>
/// Default Game Component Name for Game Physics Update
/// </summary>
#define AE_GAME_PHYSICS_UPDATE_DEF_COMPONENT_NAME           L"AE Game Physics Update Component"

/// <summary>
/// Default Game Component Name for Game Objects Debug Visualizer
/// </summary>
#define AE_GAME_OBJECTS_DEBUG_VISUALIZER_DEF_COMPONENT_NAME L"AE Game Objects Debug Visualizer Component"

/// <summary>
/// Default Game Component Name for Game Object Camera Update
/// </summary>
#define AE_GAME_GAME_OBJECT_CAMERA_UPDATE_COMPONENT_NAME    L"AE Game Object Camera Update Component"

/// <summary>
/// Default Game Component Name for Debug Stats
/// </summary>
#define AE_DEBUG_STATS_DEF_COMPONENT_NAME                   L"AE Debug Stats Component"

/// <summary>
/// Default Game Component Name for Debug Stats
/// </summary>
#define AE_CONSOLE_DEF_COMPONENT_NAME                       L"AE Console Component"

/// <summary>
/// Default Name for Engine Editor Viewer Camera
/// </summary>
#define AE_CAMERA_EDITOR_ENG_DEFAULT_NAME                   L"AE Editor Main Camera"

/****************
*   Constants   *
*****************/

/************
*   Enums   *
*************/
enum AEGameComponentCallOrder : uint32_t
{
    _AE_GCCO_GamePhysicsUpdate              = 100,
    _AE_GCCO_InputHandler                   = 200,
    _AE_GCCO_GameObjectScriptCaller         = 300,
    _AE_GCCO_GameObjectCameraUpdate         = 400,
    _AE_GCCO_CameraUpdater                  = 500,
    _AE_GCCO_AnimationsUpdate               = 600,
    _AE_GCCO_GameLightsUpdate               = 700,
    _AE_GCCO_GameAudioUpdate                = 800,
    _AE_GCCO_ForwardPlusMain                = 900,
    _AE_GCCO_ForwardPlusPreZ                = 1000,
    _AE_GCCO_ForwardPlusLightCulling        = 1100,
    _AE_GCCO_ForwardPlusLightObjectDraw     = 1200,

    _AE_GCCO_GameObjectsDebugVisualizer     = 10000,
    _AE_GCCO_DebugStats                     = 10100,
    _AE_GCCO_Console                        = 10200
};

/******************
*   Struct Decl   *
*******************/

#endif