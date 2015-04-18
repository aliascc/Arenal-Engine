/** \file 
 * Contains definitions for handling Game Objects
 */

/********************************************************
*
* Author: Carlos Chacón N.
*
* Created: 5/17/2013
*
* Desc:
*
*
* Copyright 2013
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