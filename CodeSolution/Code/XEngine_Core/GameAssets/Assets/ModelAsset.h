/********************************************************
*
* Author: Carlos Chacón N.
*
* Copyright 2012-2015
*
*********************************************************/

#pragma once

#ifndef _MODEL_ASSET_H
#define _MODEL_ASSET_H

/**********************
*   System Includes   *
***********************/
#include <string>
#include <stdint.h>
#include <unordered_map>

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
class MeshAsset;
class GraphicDevice;
class SkeletonAsset;
class AnimationAsset;
class GameResourceManager;

/**************
*   Typedef   *
***************/
typedef std::unordered_map<std::wstring, GameAssetLoadStatus<MeshAsset>> MeshAssetMap;

typedef std::unordered_map<std::wstring, GameAssetLoadStatus<AnimationAsset>> AnimationAssetMap;

/*****************
*   Class Decl   *
******************/
class ModelAsset sealed : public GameAsset
{	
	private:

		/************************
		*   Private Variables   *
		*************************/
#pragma region Private Variables

		MeshAssetMap m_MeshAssetMap;

		AnimationAssetMap m_AnimationAssetMap;

		SkeletonAsset* m_SkeletonAsset = nullptr;

		/// <summary>
		/// Graphic Device to create Asset Resources in video memory
		/// </summary>
		GraphicDevice* m_GraphicDevice = nullptr;

#pragma endregion

		/**********************
		*   Private Methods   *
		***********************/
#pragma region Private Methods

		void CleanUp();

		XEResult LoadFile();

		XEResult ReadModelMeshFiles(std::ifstream& fileStream);
		
		XEResult ReadModelSkeletonFile(std::ifstream& fileStream);
		
		XEResult ReadModelAnimationFiles(std::ifstream& fileStream);
		
		/// <summary>
		/// Loads an Asset Resource to Memory
		/// </summary>
		/// <returns>XEResult::OK if successful</returns>
		XEResult LoadAssetResource() override;

#pragma endregion

	public:

		/***************************************
		*   Constructor & Destructor Methods   *
		****************************************/
#pragma region Constructor & Destructor Methods

		/// <summary>
		/// ModelAsset Constructor
		/// </summary>
		/// <param name="filePath">File Path of Game Asset to Load</param>
		/// <param name="gameResourceManager">Resource Manager to handle the Asset Resource</param>
		/// <param name="graphicDevice">Graphic Device to create Asset Resources in video memory</param>
		ModelAsset(const std::wstring& filePath, GameResourceManager* gameResourceManager, GraphicDevice* graphicDevice);

		/// <summary>
		/// Default ModelAsset Destructor
		/// </summary>
		virtual ~ModelAsset();

#pragma endregion

		/******************
		*   Get Methods   *
		*******************/
#pragma region Get Methods

		inline MeshAssetMap& GetMeshAssetMap()
		{
			return m_MeshAssetMap;
		}

		inline SkeletonAsset* GetSkeletonAsset()
		{
			return m_SkeletonAsset;
		}

		inline AnimationAssetMap& GetAnimationAssetMap()
		{
			return m_AnimationAssetMap;
		}

#pragma endregion

		/******************
		*   Set Methods   *
		*******************/
#pragma region Set Methods

		inline void SetSkeletonAsset(SkeletonAsset* skeletonAsset)
		{
			m_SkeletonAsset = skeletonAsset;
		}

#pragma endregion

		/************************
		*   Framework Methods   *
		*************************/
#pragma region Framework Methods

#pragma endregion

};

#endif