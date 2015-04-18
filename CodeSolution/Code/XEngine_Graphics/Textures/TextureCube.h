
/********************************************************
*
* Author: Carlos Chacón N.
*
* Created: 2/21/2012
*
* Desc:
*
*
* Copyright 2012
*
*********************************************************/
#pragma once

#ifndef _TEXTURECUBE_H
#define _TEXTURECUBE_H

/**********************
*   System Includes   *
***********************/
#include <string>
#include <stdint.h>
#include <d3d11_1.h>

/*************************
*   3rd Party Includes   *
**************************/
#include "DirectXTex\DirectXTex.h"

/***************************
*   Game Engine Includes   *
****************************/
#include "Base\Base.h"
#include "ITexture2D.h"

/************
*   Using   *
*************/
using namespace DirectX;

/********************
*   Forward Decls   *
*********************/
class GraphicDevice;

/*****************
*   Class Decl   *
******************/
class TextureCube sealed : public ITexture2D
{

	private:

		/************************
		*   Private Variables   *
		*************************/
#pragma region Private Variables

		uint32_t m_Depth = 0;

#pragma endregion

		/**********************
		*   Private Methods   *
		***********************/
#pragma region Private Methods

		/// <summary>
		/// Cleans up memory use by the class
		/// </summary>
		void CleanUp();

#pragma endregion

		/*********************************
		*   Private Destructor Methods   *
		**********************************/
#pragma region Private Destructor Methods

		/// <summary>
		/// Default TextureCube Destructor
		/// Private so no one can call the destructor
		/// Only Game Resource can
		/// </summary>
		virtual ~TextureCube();

#pragma endregion

	public:

		/**************************
		*   Constructor Methods   *
		***************************/
#pragma region Constructor Methods

		/// <summary>
		/// Default TextureCube Constructor
		/// </summary>
		/// <param name="graphicDevice">Graphic Device to be associated with this Texture</param>
		/// <param name="textureName">Name of the Texture</param>
		TextureCube(GraphicDevice* graphicDevice, const std::wstring& textureName);

#pragma endregion

		/******************
		*   Get Methods   *
		*******************/
#pragma region Get Methods

		/// <summary>
		/// Gets Depth of the Texture
		/// </summary>
		/// <returns>Depth of the Texture</returns>
		inline uint32_t GetDepth() const 
		{ 
			return m_Depth;
		}

#pragma endregion

		/************************
		*   Framework Methods   *
		*************************/
#pragma region Override Framework Methods

		/// <summary>
		/// Load a TextureCube File to memory
		/// </summary>
		/// <param name="textureFile">File path of the Texture2D</param>
		/// <returns>XEResult::Ok if Texture2D was created from the file</returns>
		XEResult CreateFromFile(const std::wstring& textureFile);

		/// <summary>
		/// Loads or Reloads the Texture File to memory if it was created from a file
		/// </summary>
		/// <returns>XEResult::Ok if if Texture File reload succeeded</returns>
		XEResult Load() override;

#pragma endregion

};

#endif