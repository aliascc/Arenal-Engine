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

#ifndef _ANIMATION_ASSET_H
#define _ANIMATION_ASSET_H

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
class Animation;
class GraphicDevice;
class GameResourceManager;

/*****************
*   Class Decl   *
******************/
class AnimationAsset sealed : public GameAsset
{	
	private:

		/************************
		*   Private Variables   *
		*************************/
#pragma region Private Variables

		Animation* m_Animation = nullptr;

#pragma endregion

		/**********************
		*   Private Methods   *
		***********************/
#pragma region Private Methods

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
		/// AnimationAsset Constructor
		/// </summary>
		/// <param name="filePath">File Path of Game Asset to Load</param>
		/// <param name="gameResourceManager">Resource Manager to handle the Asset Resource</param>
		AnimationAsset(const std::wstring& filePath, GameResourceManager* gameResourceManager);

		/// <summary>
		/// Default AnimationAsset Destructor
		/// </summary>
		virtual ~AnimationAsset();

#pragma endregion

		/******************
		*   Get Methods   *
		*******************/
#pragma region Get Methods

		Animation* GetAnimationReference();

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