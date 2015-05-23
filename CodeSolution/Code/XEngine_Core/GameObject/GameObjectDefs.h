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