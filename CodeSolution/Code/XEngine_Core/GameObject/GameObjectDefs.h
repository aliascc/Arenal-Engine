/** \file 
 * Contains definitions for handling Game Objects
 */

/********************************************************
*
* Author: Carlos Chacón N.
*
* Copyright 2012-2015
*
*********************************************************/

#pragma once

#ifndef _GAME_OBJECT_DEFS_H
#define _GAME_OBJECT_DEFS_H

/**********************
*   System Includes   *
***********************/
#include <map>
#include <stdint.h>
#include <functional>

/*************************
*   3rd Party Includes   *
**************************/

/***************************
*   Game Engine Includes   *
****************************/
#include "Base\Base.h"
#include "Base\XEObject.h"
#include "GameAssets\GameAssetDefs.h"

/********************
*   Forward Decls   *
*********************/
class Texture;
class GameAsset;

/**************
*   Defines   *
***************/

#define XE_GAME_OBJ_MANAGER_NODE_NAME					L"GameObjectManager"
#define XE_GAME_OBJ_NODE_NAME							L"GameObject"
#define XE_GAME_OBJ_NAME_PROP							L"Name"
#define XE_GAME_OBJ_POSITION_PROP						L"Position"
#define XE_GAME_OBJ_ROTATION_PROP						L"Rotation"
#define XE_GAME_OBJ_SCALE_PROP							L"Scale"
#define XE_GAME_OBJ_COMPONENTS_NODE_NAME				L"Components"
#define XE_GAME_OBJ_CHILDS_NODE_NAME					L"Childs"
#define XE_GAME_OBJ_COMPONENT_ASSETID_PROP				L"AssetID"
#define XE_GAME_OBJ_GOC_MESH_NODE_NAME					L"MeshComponent"
#define XE_GAME_OBJ_GOC_MAT_NODE_NAME					L"MaterialComponent"
#define XE_GAME_OBJ_GOC_MAT_SHADER_NODE_NAME			L"Shader"
#define XE_GAME_OBJ_GOC_MAT_SHADER_TYPE_PROP			L"ShaderType"
#define XE_GAME_OBJ_GOC_MAT_SHADER_PROPS_NODE_NAME		L"ShaderProperties"
#define XE_GAME_OBJ_GOC_MAT_SHADER_VAR_NODE_NAME		L"ShaderVariable"
#define XE_GAME_OBJ_GOC_MAT_SHADER_CB_NODE_NAME			L"ConstantBuffer"
#define XE_GAME_OBJ_GOC_MAT_SHADER_CB_NAME_PROP			L"ConstantBufferName"
#define XE_GAME_OBJ_GOC_MAT_SHADER_VAR_NAME_PROP		L"ShaderVariableName"
#define XE_GAME_OBJ_GOC_MAT_SHADER_VAR_CLASS_PROP		L"ShaderVariableClass"
#define XE_GAME_OBJ_GOC_MAT_SHADER_VAR_TYPE_PROP		L"ShaderVariableType"
#define XE_GAME_OBJ_GOC_MAT_SHADER_VAR_VALUE_PROP		L"ShaderVariableValue"
#define XE_GAME_OBJ_GOC_MAT_TEXTURE_NODE_NAME			L"ShaderTextureBinding"
#define XE_GAME_OBJ_GOC_SCRIPT_NODE_NAME				L"GameObjectScriptComponent"
#define XE_GAME_OBJ_GOC_SCRIPT_HAS_INSTANCE_PROP		L"HasInstance"
#define XE_GAME_OBJ_GOC_SCRIPT_INSTANCE_NAME_PROP		L"InstanceName"
#define XE_GAME_OBJ_GOC_SCRIPT_PROPERTY_NODE_NAME		L"GOSProperty"
#define XE_GAME_OBJ_GOC_SCRIPT_PROPERTY_TYPE_PROP		L"PropertyType"
#define XE_GAME_OBJ_GOC_SCRIPT_PROPERTY_VALUE_PROP		L"PropertyValue"
#define XE_GAME_OBJ_GOC_LIGHT_NODE_NAME					L"LightComponent"
#define XE_GAME_OBJ_GOC_LIGHT_FRUSTUM_CASCADE_PROP		L"FrustumCascadeEnabled"
#define XE_GAME_OBJ_GOC_LIGHT_DEBUG_DRAW_PROP			L"DebugDrawEnabled"
#define XE_GAME_OBJ_GOC_LIGHT_DRAW_FRUSTUM_SIMPLE_PROP	L"DrawFrustumSimple"
#define XE_GAME_OBJ_GOC_LIGHT_DRAW_FRUSTUM_ENABLE_PROP	L"DrawFrustumEnabled"
#define XE_GAME_OBJ_GOC_LIGHT_LIGHT_TYPE_PROP			L"Type"
#define XE_GAME_OBJ_GOC_LIGHT_ENABLED_PROP				L"Enabled"
#define XE_GAME_OBJ_GOC_LIGHT_SHADOW_ENABLED_PROP		L"ShadowEnabled"
#define XE_GAME_OBJ_GOC_LIGHT_COLOR_PROP				L"Color"
#define XE_GAME_OBJ_GOC_LIGHT_FAR_ATTENUATION_PROP		L"FarAttenuation"
#define XE_GAME_OBJ_GOC_LIGHT_NEAR_ATTENUATION_PROP		L"NearAttenuation"
#define XE_GAME_OBJ_GOC_LIGHT_FALL_OFF_ANGLE_PROP		L"FallOffAngle"
#define XE_GAME_OBJ_GOC_LIGHT_ANGLE_PROP				L"Angle"
#define XE_GAME_OBJ_GOC_ANIM_NODE_NAME					L"MeshAnimationComponent"
#define XE_GAME_OBJ_GOC_ANIM_BLEND_ANIM_PROP			L"BlendAnimation"
#define XE_GAME_OBJ_GOC_ANIM_BLEND_TIME_PROP			L"BlendTime"
#define XE_GAME_OBJ_GOC_ANIM_ON_LOOP_ANIM_PROP			L"OnLoop"
#define XE_GAME_OBJ_GOC_ANIM_SKELETON_ASSET_ID_PROP		L"SkeletonAssetID"
#define XE_GAME_OBJ_GOC_ANIM_ASSET_NODE_NAME			L"AnimationAsset"
#define XE_GAME_OBJ_GOC_CAM_NODE_NAME					L"CameraComponent"
#define XE_GAME_OBJ_GOC_CAM_DEBUG_DRAW_ENABLED_PROP		L"DebugDrawEnabled"
#define XE_GAME_OBJ_GOC_CAM_IS_DEFAULT_CAM_PROP			L"IsDefaultCamera"
#define XE_GAME_OBJ_GOC_CAM_DRAW_FRUSTUM_ENABLED_PROP	L"DrawFrustumEnabled"
#define XE_GAME_OBJ_GOC_CAM_DRAW_FRUSTUM_SIMPLE_PROP	L"DrawFrustumSimple"
#define XE_GAME_OBJ_GOC_AUDIO_LISTENER_NODE_NAME		L"AudioListenerComponent"
#define XE_GAME_OBJ_GOC_AUDIO_SOURCE_NODE_NAME			L"AudioSourceComponent"
#define XE_GAME_OBJ_GOC_PHYSICS_NODE_NAME				L"PhysicsComponent"
#define XE_GAME_OBJ_GOC_PHYSICS_IS_RIGID_BODY_PROP		L"IsRigidBody"
#define XE_GAME_OBJ_GOC_PHYSICS_COLLIDER_NODE_NAME		L"Collider"
#define XE_GAME_OBJ_GOC_PHYSICS_COLLIDER_TYPE_PROP		L"Type"
#define XE_GAME_OBJ_GOC_PHYSICS_COLLIDER_SCALE_PROP		L"Scale"
#define XE_GAME_OBJ_GOC_PHYSICS_COLLIDER_SIZE_PROP		L"Size"
#define XE_GAME_OBJ_GOC_PHYSICS_COLLIDER_RADIUS_PROP	L"Radius"

/**************
*   Typedef   *
***************/
typedef std::function<void (uint64_t)> GameObjectMoveToChildCallback;

/************
*   Enums   *
*************/

enum class GameObjectComponentTypeOption : uint32_t
{
	GameObjectScript = 0,

	Mesh,

	MeshMaterial,

	MeshAnimation,

	Light,

	RigidBody,

	BoxCollider,

	SphereCollider,

	Camera,

	AudioSource,

	AudioListener
};

/// <summary>	
/// Types for Game Object Components
/// </summary>
enum class GameObjectComponentType : uint32_t
{
	/// <summary>
	/// Game Object Component type that handles the Game Object Scripts for the Game Object
	/// </summary>
	GameObjectScript = 0,
	
	/// <summary>
	/// Game Object Component type that handles the Mesh for the Game Object
	/// </summary>
	Mesh,

	/// <summary>
	/// Game Object Component type that handles the Mesh Materials for the Game Object
	/// </summary>
	MeshMaterial,

	/// <summary>
	/// Game Object Component type that handles the Mesh Animations for the Game Object
	/// </summary>
	MeshAnimation,
	
	/// <summary>
	/// Game Object Component type that handles a Light Source that the Game Object emits
	/// </summary>
	Light,

	/// <summary>
	/// Game Object Component type that handles the Physics for the Game Object
	/// </summary>
	Physics,

	/// <summary>
	/// Game Object Component type that handles the a camera that a Game Object owns
	/// </summary>
	Camera,

	/// <summary>
	/// Game Object Component type that handles an audio source that a Game Object emits
	/// </summary>
	AudioSource,

	/// <summary>
	/// Game Object Component type that handles if a Game Object is listening to audio
	/// </summary>
	AudioListener
};

/******************
*   Struct Decl   *
*******************/

template <class T>
struct GameObjectAssetPair sealed : public XEObject
{
	uint64_t m_AssetID = 0;
	uint64_t m_CallerID = 0;
	T* m_ResourceAsset = nullptr;
	OnListenerObjDeletionEvent m_OnListenerObjDeletionEvent = nullptr;
	GameAsset* m_GameAsset = nullptr;

	GameObjectAssetPair()
	{
	}
};

#endif