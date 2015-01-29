
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

#ifndef _DIFFUSE_TEXTURE_PS_H
#define _DIFFUSE_TEXTURE_PS_H

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
#include "Shaders\PixelShader.h"

/************
*   Using   *
*************/

/********************
*   Forward Decls   *
*********************/
class GraphicDevice;
class IConstantBuffer;

/**************
*   Defines   *
***************/

/// <summary>
/// Pixel Shader PNT Name
/// </summary>
#define XE_DIFFUSE_TEXTURE_PS_NAME			L"Diffuse Texture Pixel Shader"

/*****************
*   Class Decl   *
******************/
class DiffuseTexturePS sealed : public PixelShader
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

	#pragma endregion

		/*********************************
		*   Private Destructor Methods   *
		**********************************/
	#pragma region Private Destructor Methods

		/// <summary>
		/// Default DiffuseTexturePS Destructor
		/// </summary>
		virtual ~DiffuseTexturePS();

	#pragma endregion

	public:

		/**************************
		*   Constructor Methods   *
		***************************/
	#pragma region Constructor Methods

		/// <summary>
		/// Default DiffuseTexturePS Constructor
		/// </summary>
		/// <param name="graphicDevice">Graphic Device to be associated with this Shader</param>
		/// <param name="name">Name of Shader</param>
		DiffuseTexturePS(GraphicDevice* graphicDevice, const std::wstring& resourceName);

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

		XEResult Load() override;

	#pragma endregion

};

#endif