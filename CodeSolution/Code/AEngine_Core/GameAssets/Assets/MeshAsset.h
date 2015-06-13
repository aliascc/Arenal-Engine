/********************************************************
*
* Author: Carlos Chacón N.
*
* Copyright 2012-2015
*
*********************************************************/

#pragma once

#ifndef _MESH_ASSET_H
#define _MESH_ASSET_H

/**********************
*   System Includes   *
***********************/
#include <string>
#include <stdint.h>

/*************************
*   3rd Party Includes   *
**************************/

/***************************
*   Game Engine Includes   *
****************************/
#include "Base\Base.h"
#include "GameAssets\GameAsset.h"

/********************
*   Forward Decls   *
*********************/
class Mesh;

/*****************
*   Class Decl   *
******************/
class MeshAsset sealed : public GameAsset
{	
	private:

		/************************
		*   Private Variables   *
		*************************/
#pragma region Private Variables

		Mesh* m_Mesh = nullptr;

		/// <summary>
		/// Graphic Device to create Asset Resources in video memory
		/// </summary>
		GraphicDevice* m_GraphicDevice = nullptr;

#pragma endregion

		/**********************
		*   Private Methods   *
		***********************/
#pragma region Private Methods
		
		/// <summary>
		/// Loads an Asset Resource to Memory
		/// </summary>
		/// <returns>AEResult::OK if successful</returns>
		AEResult LoadAssetResource() override;

#pragma endregion

	public:

		/***************************************
		*   Constructor & Destructor Methods   *
		****************************************/
#pragma region Constructor & Destructor Methods

		/// <summary>
		/// MeshAsset Constructor
		/// </summary>
		/// <param name="filePath">File Path of Game Asset to Load</param>
		/// <param name="gameResourceManager">Resource Manager to handle the Asset Resource</param>
		/// <param name="graphicDevice">Graphic Device to create Asset Resources in video memory</param>
		MeshAsset(const std::wstring& filePath, GameResourceManager* gameResourceManager, GraphicDevice* graphicDevice);

		/// <summary>
		/// Default SkeletonAsset Destructor
		/// </summary>
		virtual ~MeshAsset();

#pragma endregion

		/******************
		*   Get Methods   *
		*******************/
#pragma region Get Methods

		Mesh* GetMeshReference();

#pragma endregion

		/******************
		*   Set Methods   *
		*******************/
#pragma region Set Methods

#pragma endregion
		
		/************************
		*   Framework Methods   *
		*************************/
#pragma region Framework Methods

#pragma endregion

};

#endif