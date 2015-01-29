/** \file 
 * Contains definitions for handling Game Objects
 */

/********************************************************
*
* Author: Carlos Chacón N.
*
* Desc:
*
*
* Copyright 2012-2014
*
*********************************************************/

#pragma once

#ifndef _GAME_ASSET_DEFS_H
#define _GAME_ASSET_DEFS_H

/**********************
*   System Includes   *
***********************/
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
#define XE_GAME_ASSET_UNIQUE_ID_USER_START			10000

/// <summary>
/// Cube Mesh Resource Name
/// </summary>
#define XE_CUBE_MESH_RESOURCE_NAME					L"XE Cube Mesh Resource"

/// <summary>
/// Cube Mesh Fake File Path
/// </summary>
#define XE_CUBE_MESH_FAKE_FILE_PATH					L"_XE_CubeMesh_FakeFilePath"

/// <summary>
/// Ico Sphere Mesh Resource Name
/// </summary>
#define XE_ICO_SPHERE_MESH_RESOURCE_NAME			L"XE IcoSphere Mesh Resource"

/// <summary>
/// Cube Mesh Fake File Path
/// </summary>
#define XE_ICO_SPHERE_MESH_FAKE_FILE_PATH			L"_XE_IcoSphereMesh_FakeFilePath"

/// <summary>
/// Diffuse Texture VS Fake File Path
/// </summary>
#define XE_DIFFUSE_TEXTURE_VS_FAKE_FILE_PATH		L"_XE_DiffuseTextureVS_FakeFilePath"

/// <summary>
/// Diffuse Texture PS Fake File Path
/// </summary>
#define XE_DIFFUSE_TEXTURE_PS_FAKE_FILE_PATH		L"_XE_DiffuseTexturePS_FakeFilePath"

#define XE_ASSET_MANAGER_NODE_NAME					L"AssetManager"

#define XE_ASSET_CONFIG_NODE_NAME					L"Config"
#define XE_ASSET_PERSISTANTUNIQUEID_PROP_NAME		L"PersistantUniqueID"

#define XE_ASSETS_NODE_NAME							L"Assets"
#define XE_ASSET_NODE_NAME							L"Asset"

#define XE_RAW_FILES_NODE_NAME						L"RawFiles"
#define XE_RAW_FILE_NODE_NAME						L"RawFile"

#define XE_RAW_FILE_CONTENTTYPE_PROP				L"ContentType"
#define XE_RAW_FILE_CONTENTSUBTYPE_PROP				L"ContentSubtype"
#define XE_RAW_FILE_GAMECONTEXTFILEEXT_PROP			L"GameContextFileExt"
#define XE_RAW_FILE_LASTMODIFIEDTIMESTAMP_PROP		L"LastModifiedTimeStamp"
#define XE_RAW_FILE_FILEPATH_PROP					L"FilePath"
#define XE_RAW_FILE_OUTPUTDIRECTORY_PROP			L"OutputDirectory"
#define XE_RAW_FILE_OUTPUTFILENAME_PROP				L"OutputFileName"
#define XE_RAW_FILE_RELOADNEEDED_PROP				L"ReloadNeeded"
#define XE_RAW_FILE_OUTPUTDIRCHANGED_PROP			L"OutputDirChanged"
#define XE_RAW_FILE_CONTENTSUBTYPECHANGED_PROP		L"ContentSubtypeChanged"
#define XE_RAW_FILE_ASSOCIATED_ASSET_ID_PROP		L"AssociatedAssetID"
#define XE_RAW_FILE_NAME_PROP						L"Name"

#define XE_ASSET_NAME_PROP							L"Name"
#define XE_ASSET_UNIQUEASSETID_PROP					L"UniqueAssetID"
#define XE_ASSET_PARENTASSETID_PROP					L"ParentAssetID"
#define XE_ASSET_GAMECONTENTTYPE_PROP				L"GameContentType"
#define XE_ASSET_ISLOADED_PROP						L"IsLoaded"

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

typedef std::function<void (uint64_t)> OnGameAssetDeletionNotifyManagerEvent;
typedef std::function<XEResult (uint64_t)> OnListenerObjDeletionEvent;
typedef std::function<void (GameAsset*)> OnGameAssetDeletionEvent;
typedef std::function<void (GameAsset*)> OnGameAssetReloadEvent;
typedef std::function<void (GameAsset*)> OnGameAssetPreReloadEvent;

/******************
*   Struct Decl   *
*******************/

struct GameAssetBuiltIns sealed : public XEObject
{
	CubeMesh* m_CubeMesh = nullptr;
	IcoSphereMesh* m_IcoSphereMesh = nullptr;
	DiffuseTextureVS* m_DiffuseTextureVS = nullptr;
	DiffuseTexturePS* m_DiffuseTexturePS = nullptr;

	GameAssetBuiltIns();
	~GameAssetBuiltIns();
};

struct GameAssetEventHandlers : public XEObject
{
	uint64_t m_ObjID = 0;
	OnGameAssetDeletionEvent m_OnGameAssetDeletionEvent = nullptr;
	OnGameAssetReloadEvent m_OnGameAssetReloadEvent = nullptr;
	OnGameAssetPreReloadEvent m_OnGameAssetPreReloadEvent = nullptr;

	GameAssetEventHandlers();

	GameAssetEventHandlers(uint64_t objID, OnGameAssetDeletionEvent onGameAssetDeletionEvent, OnGameAssetReloadEvent onGameAssetReloadEvent, OnGameAssetPreReloadEvent onGameAssetPreReloadEvent);
};

#endif