/********************************************************
*
* Author: Carlos Chac�n N.
*
* Copyright 2012-2015
*
*********************************************************/

#pragma once

#ifndef _TEXTURE_2D_ARRAY_H
#define _TEXTURE_2D_ARRAY_H

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
#include "TextureArray.h"

/************
*   Using   *
*************/

/********************
*   Forward Decls   *
*********************/
class Texture;
class GraphicDevice;

/*****************
*   Class Decl   *
******************/
class Texture2DArray sealed : public TextureArray
{
	private:

		/************************
		*   Private Variables   *
		*************************/
#pragma region Private Variables

		/// <summary>
		/// DirectX Texture
		/// </summary>
		ID3D11Texture2D* m_TextureArrayDX = nullptr;

		/// <summary>
		/// DirectX Render Target Views Vector
		/// </summary>
		std::vector<ID3D11RenderTargetView*> m_RenderTargetViewsVector;

#pragma endregion

		/**********************
		*   Private Methods   *
		***********************/
#pragma region Private Variables

		/// <summary>
		/// Clean up the memory use by the class
		/// </summary>
		void CleanUp();

#pragma endregion

	public:

		/***************************************
		*   Constructor & Destructor Methods   *
		****************************************/
#pragma region Constructor & Destructor Methods

		/// <summary>
		/// Default Texture2DArray Constructor
		/// </summary>
		/// <param name="graphicDevice">Graphic Device to be associated with this Texture</param>
		/// <param name="name">Name of the Texture Array</param>
		/// <param name="bindIndex">Index where to bind</param>
		Texture2DArray(GraphicDevice* graphicDevice, const std::wstring& name, uint32_t bindIndex);

		/// <summary>
		/// Default Texture2DArray Destructor
		/// </summary>
		virtual ~Texture2DArray();

#pragma endregion

		/******************
		*   Get Methods   *
		*******************/
#pragma region Get Methods

		/// <summary>
		/// Gets the DirectX Render Target if Bind Option was set to Render Target
		/// </summary>
		/// <param name="index">Index of the Array</param>
		/// <returns>Returns DirectX Render Target if found, null otherwise</returns>
		ID3D11RenderTargetView* GetRenderTargetDX(uint32_t index);

#pragma endregion

		/************************
		*   Framework Methods   *
		*************************/
#pragma region Framework Methods

		/// <summary>
		/// Initializes the Texture Array
		/// </summary>
		/// <param name="arraySize">Size of the Array</param>
		/// <param name="width">Width of the Textures in the Array</param>
		/// <param name="height">Height of the Textures in the Array</param>
		/// <param name="mipLevels">Mip Levels of the Textures in the Array</param>
		/// <param name="format">Format of the Textures in the Array</param>
		/// <param name="binfOption">Texture bind Option</param>
		/// <returns>Returns XEResult::Ok if successful</returns>
		XEResult Initialize(uint32_t arraySize, uint32_t width, uint32_t height, uint32_t mipLevels, DXGI_FORMAT format, TextureBindOption bindOption = TextureBindOption::Default);

		/// <summary>
		/// Copies the Texture to the array at an specific index
		/// </summary>
		/// <param name="index">Index of the Array</param>
		/// <param name="texture">Texture to copy</param>
		/// <returns>Returns XEResult::Ok if successful</returns>
		XEResult SetTexture(uint32_t index, Texture* texture) override;

		/// <summary>
		/// Deinitializies the Texture Array
		/// </summary>
		/// <returns>Returns XEResult::Ok if successful</returns>
		XEResult Deinitialize() override;

#pragma endregion

};

#endif