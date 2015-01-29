/********************************************************
*
* Author: Carlos Chacón N.
*
* Created: 2/6/2014
*
* Desc:
*
*
* Copyright 2012
*
*********************************************************/

#pragma once

#ifndef _XE_GAME_COMPONENTS_DEFS_H
#define _XE_GAME_COMPONENTS_DEFS_H

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
#define XE_CAMERA_UPDATER_DEF_SERVICE_NAME					L"XE Camera Updater Service"

/// <summary>
/// Default Service Name for Input Handler
/// </summary>
#define XE_INPUT_HANDLER_DEF_SERVICE_NAME					L"XE Input Handler Service"

/// <summary>
/// Default Service Name for Forward Plus Rendering Main
/// </summary>
#define XE_FORWARD_PLUS_MAIN_DEF_SERVICE_NAME				L"XE Forward Plus Rendering Main Service"

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////

/// <summary>
/// Default Component Name for Camera Updater
/// </summary>
#define XE_CAMERA_UPDATER_DEF_COMPONENT_NAME				L"XE Camera Updater Component"

/// <summary>
/// Default Game Component Name for Input Handler
/// </summary>
#define XE_INPUT_HANDLER_DEF_COMPONENT_NAME					L"XE Input Handler Component"

/// <summary>
/// Default Game Component Name for Forward Plus Rendering Main
/// </summary>
#define XE_FORWARD_PLUS_MAIN_DEF_COMPONENT_NAME				L"XE Forward Plus Rendering Main Component"

/// <summary>
/// Default Game Component Name for Forward Plus Rendering Z Pre Pass
/// </summary>
#define XE_FORWARD_PLUS_Z_PRE_PASS_DEF_COMPONENT_NAME		L"XE Forward Plus Rendering Pre Z Pass Component"

/// <summary>
/// Default Game Component Name for Forward Plus Rendering Object Draw
/// </summary>
#define XE_FORWARD_PLUS_OBJECT_DRAW_DEF_COMPONENT_NAME		L"XE Forward Plus Rendering Object Draw Component"

/// <summary>
/// Default Game Component Name for Forward Plus Rendering Light Culling
/// </summary>
#define XE_FORWARD_PLUS_LIGHT_CULLING_DEF_COMPONENT_NAME	L"XE Forward Plus Light Culling Component"

/// <summary>
/// Default Game Component Name for Game Lights Update
/// </summary>
#define XE_GAME_LIGHTS_UPDATE_DEF_COMPONENT_NAME			L"XE Game Lights Update Component"

/// <summary>
/// Default Game Component Name for Game Object Script Caller
/// </summary>
#define XE_GAME_GAME_OBJECT_SCRIPT_CALLER_COMPONENT_NAME	L"XE Game Object Script Caller Component"

/// <summary>
/// Default Game Component Name for Game Animations Update
/// </summary>
#define XE_GAME_ANIMATIONS_UPDATE_DEF_COMPONENT_NAME		L"XE Game Animations Update Component"

/// <summary>
/// Default Game Component Name for Game Audio Update
/// </summary>
#define XE_GAME_AUDIO_UPDATE_DEF_COMPONENT_NAME				L"XE Game Audio Update Component"

/// <summary>
/// Default Game Component Name for Game Physics Update
/// </summary>
#define XE_GAME_PHYSICS_UPDATE_DEF_COMPONENT_NAME			L"XE Game Physics Update Component"

/// <summary>
/// Default Game Component Name for Game Objects Debug Visualizer
/// </summary>
#define XE_GAME_OBJECTS_DEBUG_VISUALIZER_DEF_COMPONENT_NAME	L"XE Game Objects Debug Visualizer Component"

/// <summary>
/// Default Game Component Name for Game Object Camera Update
/// </summary>
#define XE_GAME_GAME_OBJECT_CAMERA_UPDATE_COMPONENT_NAME	L"XE Game Object Camera Update Component"

/// <summary>
/// Default Game Component Name for Debug Stats
/// </summary>
#define XE_DEBUG_STATS_DEF_COMPONENT_NAME					L"XE Debug Stats Component"

/// <summary>
/// Default Game Component Name for Debug Stats
/// </summary>
#define XE_CONSOLE_DEF_COMPONENT_NAME						L"XE Console Component"

/// <summary>
/// Default Name for Engine Editor Viewer Camera
/// </summary>
#define XE_CAMERA_EDITOR_ENG_DEFAULT_NAME					L"XE Editor Main Camera"

/****************
*   Constants   *
*****************/

/************
*   Enums   *
*************/
enum XEGameComponentCallOrder : uint32_t
{
	_XE_GCCO_GamePhysicsUpdate				= 100,
	_XE_GCCO_InputHandler					= 200,
	_XE_GCCO_GameObjectScriptCaller			= 300,
	_XE_GCCO_GameObjectCameraUpdate			= 400,
	_XE_GCCO_CameraUpdater					= 500,
	_XE_GCCO_AnimationsUpdate				= 600,
	_XE_GCCO_GameLightsUpdate				= 700,
	_XE_GCCO_GameAudioUpdate				= 800,
	_XE_GCCO_ForwardPlusMain				= 900,
	_XE_GCCO_ForwardPlusPreZ				= 1000,
	_XE_GCCO_ForwardPlusLightCulling		= 1100,
	_XE_GCCO_ForwardPlusLightObjectDraw		= 1200,

	_XE_GCCO_GameObjectsDebugVisualizer		= 10000,
	_XE_GCCO_DebugStats						= 10100,
	_XE_GCCO_Console						= 10200
};

/******************
*   Struct Decl   *
*******************/

#endif