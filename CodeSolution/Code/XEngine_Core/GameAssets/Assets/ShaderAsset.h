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

#ifndef _SHADER_ASSET_H
#define _SHADER_ASSET_H

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
class Shader;

/*****************
*   Class Decl   *
******************/
class ShaderAsset sealed : public GameAsset
{	
	private:

		/************************
		*   Private Variables   *
		*************************/
#pragma region Private Variables

		Shader* m_Shader = nullptr;

		ShaderType m_ShaderType = ShaderType::VertexShader;

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
		/// <returns>XEResult::OK if successful</returns>
		XEResult LoadAssetResource() override;

#pragma endregion

	public:

		/***************************************
		*   Constructor & Destructor Methods   *
		****************************************/
#pragma region Constructor & Destructor Methods

		/// <summary>
		/// ShaderAsset Constructor
		/// </summary>
		/// <param name="filePath">File Path of Game Asset to Load</param>
		/// <param name="gameResourceManager">Resource Manager to handle the Asset Resource</param>
		/// <param name="shaderType">Type of the Shader that this Asset will hold</param>
		/// <param name="graphicDevice">Graphic Device to create Asset Resources in video memory</param>
		ShaderAsset(const std::wstring& filePath, GameResourceManager* gameResourceManager, ShaderType shaderType, GraphicDevice* graphicDevice);

		/// <summary>
		/// Default MaterialAsset Destructor
		/// </summary>
		virtual ~ShaderAsset();

#pragma endregion

		/******************
		*   Get Methods   *
		*******************/
#pragma region Get Methods

		inline ShaderType GetShaderType() const
		{
			return m_ShaderType;
		}

		Shader* GetShaderReference();

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