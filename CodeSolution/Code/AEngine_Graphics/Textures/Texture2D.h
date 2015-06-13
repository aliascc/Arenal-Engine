/********************************************************
*
* Author: Carlos Chacón N.
*
* Copyright 2012-2015
*
*********************************************************/

#pragma once

#ifndef _TEXTURE2D_H
#define _TEXTURE2D_H

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
#include "ITexture2D.h"
#include "GraphicsDefs.h"

/************
*   Using   *
*************/

/********************
*   Forward Decls   *
*********************/
struct Color;
class GraphicDevice;

/*****************
*   Class Decl   *
******************/
class Texture2D sealed : public ITexture2D
{
	private:

		/************************
		*   Private Variables   *
		*************************/
#pragma region Private Variables

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
		/// Default Texture2D Destructor
		/// Private so no one can call the destructor
		/// Only Game Resource can
		/// </summary>
		virtual ~Texture2D();

#pragma endregion

	public:

		/**************************
		*   Constructor Methods   *
		***************************/
#pragma region Constructor Methods

		/// <summary>
		/// Default Texture2D Constructor
		/// </summary>
		/// <param name="graphicDevice">Graphic Device to be associated with this Texture</param>
		/// <param name="textureName">Name of the Texture</param>
		Texture2D(GraphicDevice* graphicDevice, const std::wstring& textureName);

#pragma endregion

		/******************
		*   Get Methods   *
		*******************/
#pragma region Get Methods

#pragma endregion

		/************************
		*   Framework Methods   *
		*************************/
#pragma region Framework Methods

		/// <summary>
		/// Creates a 2D Texture
		/// </summary>
		/// <param name="width">Width of the Texture</param>
		/// <param name="height">Height of the Texture</param>
		/// <param name="format">Format of the Texture</param>
		/// <param name="usage">Graphic Buffer Usage</param>
		/// <param name="cpuAccess">Graphic Buffer Access</param>
		/// <returns>AEResult::Ok if successful</returns>
		AEResult CreateTexture(uint32_t width, uint32_t height, DXGI_FORMAT format, GraphicBufferUsage usage, GraphicBufferAccess cpuAccess);

		/// <summary>
		/// Creates a 2D Color Texture
		/// </summary>
		/// <param name="width">Width of the Texture</param>
		/// <param name="height">Height of the Texture</param>
		/// <param name="color">Color of Texture</param>
		/// <returns>AEResult::Ok if successful</returns>
		AEResult CreateColorTexture(uint32_t width, uint32_t height, const Color& color);

		/// <summary>
		/// Load a Texture2D File to memory
		/// </summary>
		/// <param name="textureFile">File path of the Texture2D</param>
		/// <returns>AEResult::Ok if Texture2D was created from the file</returns>
		AEResult CreateFromFile(const std::wstring& textureFile);

		/// <summary>
		/// Loads or Reloads the Texture File to memory if it was created from a file
		/// </summary>
		/// <returns>AEResult::Ok if Texture File reload succeeded</returns>
		AEResult Load() override;

#pragma endregion

};

#endif