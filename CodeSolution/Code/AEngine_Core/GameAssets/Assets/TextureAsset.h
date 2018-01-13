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

#ifndef _TEXTURE_ASSET_H
#define _TEXTURE_ASSET_H

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
#include "Textures\TextureDefs.h"

/********************
*   Forward Decls   *
*********************/
class Texture;

/*****************
*   Class Decl   *
******************/
class TextureAsset sealed : public GameAsset
{	
	private:

		/*************************
		*   Private Variables   *
		**************************/
#pragma region Private Variables

		/// <summary>
		/// Graphic Device to create Asset Resources in video memory
		/// </summary>
		GraphicDevice* m_GraphicDevice = nullptr;

		Texture* m_Texture = nullptr;

		TextureType m_TextureType = TextureType::Texture2D;

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
		/// TextureAsset Constructor
		/// </summary>
		/// <param name="filePath">File Path of Game Asset to Load</param>
		/// <param name="gameResourceManager">Resource Manager to handle the Asset Resource</param>
		/// <param name="textureType">Type of the Texture that this Asset will hold</param>
		/// <param name="graphicDevice">Graphic Device to create Asset Resources in video memory</param>
		TextureAsset(const std::wstring& filePath, GameResourceManager* gameResourceManager, TextureType textureType, GraphicDevice* graphicDevice);

		/// <summary>
		/// Default TextureAsset Destructor
		/// </summary>
		virtual ~TextureAsset();

#pragma endregion

		/*******************
		*   Get Methods    *
		********************/
#pragma region Get Methods

		inline TextureType GetTextureType() const
		{
			return m_TextureType;
		}

		Texture* GetTextureReference();

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