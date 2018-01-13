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

#ifndef _TEXTURE_H
#define _TEXTURE_H

/**********************
*   System Includes   *
***********************/
#include <string>
#include <stdint.h>
#include <d3d11_1.h>

/*************************
*   3rd Party Includes   *
**************************/

/***************************
*   Game Engine Includes   *
****************************/
#include "Base\Base.h"
#include "TextureDefs.h"
#include "Resource\GameResource.h"

/********************
*   Forward Decls   *
*********************/
class GraphicDevice;

/*****************
*   Class Decl   *
******************/
class Texture abstract : public GameResource
{
	private:

		/************************
		*   Private Variables   *
		*************************/
#pragma region Private Variables

		/// <summary>
		/// Type of the Texture
		/// </summary>
		TextureType m_TextureType = TextureType::Texture2D;

#pragma endregion

	protected:

		/**************************
		*   Protected Variables   *
		***************************/
#pragma region Protected Variables

		/// <summary>
		/// Filename of the Texture
		/// </summary>
		std::wstring m_Filename = L"";
		
		/// <summary>
		/// True is the Texture was loaded from a file
		/// </summary>
		bool m_FromFile = false;

		/// <summary>
		/// Width of the Texture
		/// </summary>
		uint32_t m_Width = 0;

		/// <summary>
		/// Height of the Texture
		/// </summary>
		uint32_t m_Height = 0;

		/// <summary>
		/// Graphic Device associated with the Texture
		/// </summary>
		GraphicDevice* m_GraphicDevice = nullptr;

		/// <summary>
		/// DirectX Shader Resource use to bind the Texture to a Shader
		/// </summary>
		ID3D11ShaderResourceView* m_ShaderResourceView = nullptr;

#pragma endregion

		/************************
		*   Protected Methods   *
		*************************/
#pragma region Protected Methods

#pragma endregion

		/***********************************
		*   Protected Destructor Methods   *
		************************************/
#pragma region Protected Destructor Methods

		/// <summary>
		/// Default Texture Destructor
		/// Protected so only Childs can access it
		/// </summary>
		virtual ~Texture();

#pragma endregion

	public:

		/**************************
		*   Constructor Methods   *
		***************************/
#pragma region Constructor Methods

		/// <summary>
		/// Default Texture Constructor
		/// </summary>
		/// <param name="graphicDevice">Graphic Device to be associated with this Texture</param>
		/// <param name="textureType">Type of the Texture</param>
		/// <param name="textureName">Name of the Texture</param>
		Texture(GraphicDevice* graphicDevice, TextureType textureType, const std::wstring& textureName);

#pragma endregion

		/*******************
		 *   Get Methods   *
		 *******************/
#pragma region Get Methods

		/// <summary>
		/// Get the Texture's Width
		/// </summary>
		/// <returns>Width of Depth Stencil</returns>
		inline uint32_t GetWidth() const
		{
			return m_Width;
		}

		/// <summary>
		/// Get the Texture's Height
		/// </summary>
		/// <returns>Height of Depth Stencil</returns>
		inline uint32_t GetHeight() const
		{
			return m_Height;
		}

		/// <summary>
		/// DirectX Shader Resource use to bind the Texture to a Shader
		/// </summary>
		/// <returns>Returns DirectX Shader Texture Resource</returns>
		inline ID3D11ShaderResourceView* GetDXShaderResourceView() const
		{
			return m_ShaderResourceView;
		}

		/// <summary>
		/// Gets the Texture Type
		/// </summary>
		/// <returns>Get the Type of the Texture</returns>
		inline TextureType GetTextureType() const
		{
			return m_TextureType;
		}

		/// <summary>
		/// Returns the Filename of the Texture
		/// </summary>
		/// <returns>Filename of the Texture</returns>
		inline const std::wstring& GetFilename() const
		{ 
			return m_Filename; 
		}

		/// <summary>
		/// Returns if the Texture was loaded from a file
		/// </summary>
		/// <returns>True if the Texture was loaded from a file</returns>
		inline bool IsFromFile() const
		{ 
			return m_FromFile; 
		}

#pragma endregion

};

#endif