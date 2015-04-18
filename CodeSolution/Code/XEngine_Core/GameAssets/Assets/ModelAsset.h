/********************************************************
*
* Author: Carlos Chacón N.
*
* Created: 6/16/2013
*
* Desc:
*
*
* Copyright 2013
*
*********************************************************/

#pragma once

#ifndef _MODEL_ASSET_H
#define _MODEL_ASSET_H

/**********************
*   System Includes   *
***********************/
#include <vector>
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
class MeshAsset;
class GraphicDevice;
class SkeletonAsset;
class AnimationAsset;
class GameResourceManager;

/**************
*   Typedef   *
***************/
typedef std::vector<MeshAsset*> MeshAssetVector;

typedef std::vector<AnimationAsset*> AnimationAssetVector;

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

		MeshAssetVector m_MeshAssetVector;

		AnimationAssetVector m_AnimationAssetVector;

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

		inline MeshAssetVector& GetMeshAssetVector()
		{
			return m_MeshAssetVector;
		}

		inline SkeletonAsset* GetSkeletonAsset()
		{
			return m_SkeletonAsset;
		}

		inline AnimationAssetVector& GetAnimationAssetVector()
		{
			return m_AnimationAssetVector;
		}

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