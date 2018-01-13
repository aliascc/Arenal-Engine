/** \file 
 * Contains definitions for handling Game Objects
 */

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

#ifndef _GAME_ASSET_DEFS_H
#define _GAME_ASSET_DEFS_H

/**********************
*   System Includes   *
***********************/
#include <list>
#include <stdint.h>
#include <functional>
#include <unordered_map>

/*************************
*   3rd Party Includes   *
**************************/

/***************************
*   Game Engine Includes   *
****************************/
#include "Base\Base.h"
#include "Base\AEObject.h"

/********************
*   Forward Decls   *
*********************/
class CubeMesh;
class GameAsset;
class MeshAsset;
class IcoSphereMesh;
class DiffuseTextureVS;
class DiffuseTexturePS;

/**************
*   Defines   *
***************/

/// <summary>
/// Defines the First Unique ID for User Game Assets
/// </summary>
#define AE_GAME_ASSET_UNIQUE_ID_USER_START			10000

/// <summary>
/// Cube Mesh Resource Name
/// </summary>
#define AE_CUBE_MESH_RESOURCE_NAME					L"AE Cube Mesh Resource"

/// <summary>
/// Cube Mesh Fake File Path
/// </summary>
#define AE_CUBE_MESH_FAKE_FILE_PATH					L"_AE_CubeMesh_FakeFilePath"

/// <summary>
/// Ico Sphere Mesh Resource Name
/// </summary>
#define AE_ICO_SPHERE_MESH_RESOURCE_NAME			L"AE IcoSphere Mesh Resource"

/// <summary>
/// Cube Mesh Fake File Path
/// </summary>
#define AE_ICO_SPHERE_MESH_FAKE_FILE_PATH			L"_AE_IcoSphereMesh_FakeFilePath"

/// <summary>
/// Diffuse Texture VS Fake File Path
/// </summary>
#define AE_DIFFUSE_TEXTURE_VS_FAKE_FILE_PATH		L"_AE_DiffuseTextureVS_FakeFilePath"

/// <summary>
/// Diffuse Texture PS Fake File Path
/// </summary>
#define AE_DIFFUSE_TEXTURE_PS_FAKE_FILE_PATH		L"_AE_DiffuseTexturePS_FakeFilePath"

#define AE_ASSET_MANAGER_NODE_NAME					L"AssetManager"

#define AE_ASSET_CONFIG_NODE_NAME					L"Config"
#define AE_ASSET_PERSISTANTUNIQUEID_PROP_NAME		L"PersistantUniqueID"

#define AE_ASSETS_NODE_NAME							L"Assets"
#define AE_ASSET_NODE_NAME							L"Asset"

#define AE_RAW_FILES_NODE_NAME						L"RawFiles"
#define AE_RAW_FILE_NODE_NAME						L"RawFile"

#define AE_RAW_FILE_CONTENTTYPE_PROP				L"ContentType"
#define AE_RAW_FILE_CONTENTSUBTYPE_PROP				L"ContentSubtype"
#define AE_RAW_FILE_GAMECONTEXTFILEEXT_PROP			L"GameContextFileExt"
#define AE_RAW_FILE_LASTMODIFIEDTIMESTAMP_PROP		L"LastModifiedTimeStamp"
#define AE_RAW_FILE_FILEPATH_PROP					L"FilePath"
#define AE_RAW_FILE_OUTPUTFILENAME_PROP				L"OutputFileName"
#define AE_RAW_FILE_RELOADNEEDED_PROP				L"ReloadNeeded"
#define AE_RAW_FILE_CONTENTSUBTYPECHANGED_PROP		L"ContentSubtypeChanged"
#define AE_RAW_FILE_ASSOCIATED_ASSET_ID_PROP		L"AssociatedAssetID"
#define AE_RAW_FILE_NAME_PROP						L"Name"
#define AE_RAW_FILE_CUSTOM_NAME_PROP				L"CustomName"

#define AE_ASSET_NAME_PROP							L"Name"
#define AE_ASSET_CUSTOM_NAME_PROP					L"CustomName"
#define AE_ASSET_UNIQUEASSETID_PROP					L"UniqueAssetID"
#define AE_ASSET_PARENTASSETID_PROP					L"ParentAssetID"
#define AE_ASSET_GAMECONTENTTYPE_PROP				L"GameContentType"
#define AE_ASSET_ISLOADED_PROP						L"IsLoaded"
#define AE_ASSET_FILEPATH_PROP						L"FilePath"

/************
*   Enums   *
*************/

enum class GameAssetDefaultIDs : uint64_t
{
	CubeMesh = 1,
	SphereMesh,
	DiffuseTextureVS,
	DiffuseTexturePS,

	///////////////////
	__Max_GameAssetDefaultIDs
};

/**************
*   Typedef   *
***************/

typedef std::unordered_map<std::wstring, uint64_t> AssetIDMap;
typedef std::function<void (uint64_t)> OnGameAssetDeletionNotifyManagerEvent;
typedef std::function<AEResult (uint64_t)> OnListenerObjDeletionEvent;
typedef std::function<void (GameAsset*)> OnGameAssetDeletionEvent;
typedef std::function<void (GameAsset*)> OnGameAssetReloadEvent;
typedef std::function<void (GameAsset*)> OnGameAssetPreReloadEvent;

/******************
*   Struct Decl   *
*******************/

struct GameAssetBuiltIns sealed : public AEObject
{
	CubeMesh* m_CubeMesh = nullptr;
	IcoSphereMesh* m_IcoSphereMesh = nullptr;
	DiffuseTextureVS* m_DiffuseTextureVS = nullptr;
	DiffuseTexturePS* m_DiffuseTexturePS = nullptr;

	GameAssetBuiltIns();
	~GameAssetBuiltIns();
};

struct GameAssetEventHandlers sealed : public AEObject
{
	uint64_t m_ObjID = 0;
	OnGameAssetDeletionEvent m_OnGameAssetDeletionEvent = nullptr;
	OnGameAssetReloadEvent m_OnGameAssetReloadEvent = nullptr;
	OnGameAssetPreReloadEvent m_OnGameAssetPreReloadEvent = nullptr;

	GameAssetEventHandlers();

	GameAssetEventHandlers(uint64_t objID, OnGameAssetDeletionEvent onGameAssetDeletionEvent, OnGameAssetReloadEvent onGameAssetReloadEvent, OnGameAssetPreReloadEvent onGameAssetPreReloadEvent);
};

template <class T>
struct GameAssetLoadStatus sealed : public AEObject
{
	bool m_IsLoaded = false;
	T* m_Asset = nullptr;

	GameAssetLoadStatus()
	{
	}

	GameAssetLoadStatus(T* asset, bool isLoaded)
		: m_Asset(asset)
		, m_IsLoaded(isLoaded)
	{
	}
};

struct GameAssetLoadingDetails sealed : public AEObject
{
	uint64_t m_AssetID = 0;
	uint64_t m_ParentAssetID = 0;
	std::wstring m_Name = L"";
	std::wstring m_CustomName = L"";
	std::wstring m_Filepath = L"";
	bool m_IsLoaded = false;
};

#endif
