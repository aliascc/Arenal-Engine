/********************************************************
*
* Author: Carlos Chacón N.
*
* Copyright 2012-2015
*
*********************************************************/

#pragma once

#ifndef _DEPTH_STENCIL_SURFACE_H
#define _DEPTH_STENCIL_SURFACE_H

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

/********************
*   Forward Decls   *
*********************/
class GraphicDevice;

/*****************
*   Class Decl   *
******************/
class DepthStencilSurface sealed : public ITexture2D
{
	private:

		/************************
		*   Private Variables   *
		*************************/
#pragma region Private Variables

		/// <summary>
		/// DirectX Depth Stencil
		/// </summary>
		ID3D11DepthStencilView* m_DepthStencilDX = nullptr;

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
		/// Default Depth Stencil Destructor
		/// </summary>
		virtual ~DepthStencilSurface();

#pragma endregion

	public:

		/**************************
		*   Constructor Methods   *
		***************************/
#pragma region Constructor Methods

		/// <summary>
		/// Default Render Target Constructor
		/// </summary>
		/// <param name="graphicDevice">Graphic Device to be associated with this Depth Stencil</param>
		/// <param name="depthStencilName">Name of Depth Stencil</param>
		DepthStencilSurface(GraphicDevice* graphicDevice, const std::wstring& depthStencilName);

#pragma endregion

		/*******************
		 *   Get Methods   *
		 *******************/
#pragma region Get Methods

		/// <summary>
		/// DirectX 11 Depth Stencil View use to bind the Depth Stencil to the Graphic Device
		/// </summary>
		/// <returns>Returns DirectX 11 Depth Stencil View</returns>
		inline ID3D11DepthStencilView* GetDXDepthStencilView() const
		{
			return m_DepthStencilDX;
		}

#pragma endregion

		/***********************************
		 *   Override Framework Methods    *
		 ***********************************/
#pragma region Override Framework Methods

		/// <summary>
		/// Invalid Option for Depth Stencil Surface
		/// </summary>
		/// <returns>XEResult::Fail as method is not use</returns>
		XEResult Load() override;

#pragma endregion

		/************************
		*   Framework Methods   *
		*************************/
#pragma region Framework Methods

		/// <summary>
		/// Reinitializes the Depth Stencil
		/// </summary>
		/// <param name="width">Width in pixels of the Depth Stencil</param>
		/// <param name="height">Height in pixels of the Depth Stencil</param>
		/// <param name="formatDST">Pixel format of the Depth Stencil Texture</param>
		/// <param name="formatDSV">Pixel format of the Depth Stencil View</param>
		/// <param name="formatDSSRV">Pixel format of the Depth Stencil Shader Resource View</param>
		/// <returns>XEResult::Ok if Depth Stencil creation succeeded</returns>
		XEResult InitializeDepthStencilSurface(uint32_t width, uint32_t height, DXGI_FORMAT formatDST, DXGI_FORMAT formatDSV, DXGI_FORMAT formatDSSRV);

#pragma endregion

};

#endif