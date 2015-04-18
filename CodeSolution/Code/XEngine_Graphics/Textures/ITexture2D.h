
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

#ifndef _I_TEXTURE_2D_H
#define _I_TEXTURE_2D_H

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
#include "Texture.h"
#include "Base\Base.h"
#include "TextureDefs.h"

/********************
*   Forward Decls   *
*********************/
class GraphicDevice;

/*****************
*   Class Decl   *
******************/
class ITexture2D abstract : public Texture
{
	protected:

		/**************************
		*   Protected Variables   *
		***************************/
#pragma region Protected Variables

		/// <summary>
		/// DirectX Texture
		/// </summary>
		ID3D11Texture2D* m_TextureDX = nullptr;

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
		/// Default ITexture2D Destructor
		/// Protected so only Childs can access it
		/// </summary>
		virtual ~ITexture2D();

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
		/// <param name="textureName">Name of the Texture</param>
		/// <param name="isCube">If true Texture 2D is a Texture Cube</param>
		ITexture2D(GraphicDevice* graphicDevice, const std::wstring& textureName, bool isCube = false);

#pragma endregion

		/******************
		*   Get Methods   *
		*******************/
#pragma region Get Methods

		/// <summary>
		/// DirectX Texture Resource
		/// </summary>
		/// <returns>Returns DirectX Texture Resource</returns>
		inline ID3D11Texture2D* GetDXTexture() const
		{
			return m_TextureDX;
		}

#pragma endregion

};

#endif