/********************************************************
*
* Author: Carlos Chacón N.
*
* Copyright 2012-2015
*
*********************************************************/

#pragma once

#ifndef _SHADER_TEXTURE_BINDING_H
#define _SHADER_TEXTURE_BINDING_H

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
#include "ShaderBinding.h"
#include "Textures\TextureDefs.h"

/************
*   Using   *
*************/

/********************
*   Forward Decls   *
*********************/
class Texture;

/*****************
*   Class Decl   *
******************/
class ShaderTextureBinding sealed : public ShaderBinding
{
	private:

		/************************
		*   Private Variables   *
		*************************/
#pragma region Private Variables

		/// <summary>
		/// Texture to bind to the Shader
		/// </summary>
		Texture* m_Texture = nullptr;

		/// <summary>
		/// Texture Type that the Shader Expects to be bound
		/// </summary>
		TextureType m_TextureType = TextureType::Texture2D;

#pragma endregion

	public:

		/***************************************
		*   Constructor & Destructor Methods   *
		****************************************/
#pragma region Constructor & Destructor Methods

		/// <summary>
		/// ShaderTextureBinding Constructor
		/// </summary>
		/// <param name="name">Name of the Binding</param>
		/// <param name="bindIndex">Index where the Texture is going to be bound to the shader</param>
		/// <param name="textureType">Type of Texture</param>
		/// <param name="texture">Texture to bind to the Shader</param>
		ShaderTextureBinding(const std::wstring& name, uint32_t bindIndex, TextureType textureType, Texture* texture);

		/// <summary>
		/// Default ShaderTextureBinding Destructor
		/// </summary>
		virtual ~ShaderTextureBinding();

#pragma endregion

		/******************
		*   Get Methods   *
		*******************/
#pragma region Get Methods

		/// <summary>
		/// Gets the Texture
		/// </summary>
		/// <returns>Texture that is going to be bind to the Shader</returns>
		inline Texture* GetTexture() const 
		{ 
			return m_Texture; 
		}

		/// <summary>
		/// Gets the Texture Type
		/// </summary>
		/// <returns>Texture Type of the Texture</returns>
		inline TextureType GetTextureType() const
		{
			return m_TextureType;
		}

		/// <summary>
		/// Gets the Texture's name
		/// </summary>
		/// <returns>Name of the Texture</returns>
		std::wstring GetTextureName() const;

#pragma endregion

		/******************
		*   Set Methods   *
		*******************/
#pragma region Set Methods

		/// <summary>
		/// Sets the Texture to bind to the shader
		/// </summary>
		/// <param name="texture">Texture to bind</param>
		/// <returns>AEResult::OK if successful</returns>
		AEResult SetTexture(Texture* texture);

#pragma endregion

		/************************
		*   Framework Methods   *
		*************************/
#pragma region Framework Methods

		inline bool HasTexture() const
		{
			return (m_Texture != nullptr);
		}

		bool Compare(const ShaderTextureBinding& other) const;

		bool Compare(const ShaderTextureBinding* other) const;

		bool operator==(const ShaderTextureBinding& other) const;

#pragma endregion

};

#endif