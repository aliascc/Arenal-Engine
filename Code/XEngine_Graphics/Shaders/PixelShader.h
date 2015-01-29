
/********************************************************
*
* Author: Carlos Chacón N.
*
* Created: 11/7/2012
*
* Last Major Update: 11/7/2012
* 
* Desc:
*
*
* Copyright 2012
*
*********************************************************/

#pragma once

#ifndef _PIXEL_SHADER_H
#define _PIXEL_SHADER_H

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
#include "Shader.h"
#include "Base\Base.h"

/************
*   Using   *
*************/

/********************
*   Forward Decls   *
*********************/
class GraphicDevice;
class IConstantBuffer;

/*****************
*   Class Decl   *
******************/

class PixelShader : public Shader
{
	private:

		/************************
		*   Private Variables   *
		*************************/
#pragma region Private Variables

		/// <summary>
		/// DirectX11 Pixel Shader
		/// </summary>
		ID3D11PixelShader* m_PSDX = nullptr;

#pragma endregion

	protected:

		/************************
		*   Protected Methods   *
		*************************/
#pragma region Private Methods

		XEResult LoadShaderWithoutLock(const BYTE* shaderByteCode, uint32_t length) override;

#pragma endregion

		/***********************************
		*   Protected Destructor Methods   *
		************************************/
#pragma region Private Destructor Methods

		/// <summary>
		/// Default PixelShader Destructor
		/// </summary>
		virtual ~PixelShader();

#pragma endregion

	public:

		/**************************
		*   Constructor Methods   *
		***************************/
#pragma region Constructor Methods

		/// <summary>
		/// Default PixelShader Constructor
		/// </summary>
		/// <param name="graphicDevice">Graphic Device to be associated with this Shader</param>
		/// <param name="name">Name of Shader</param>
		PixelShader(GraphicDevice* graphicDevice, const std::wstring& name = L"");

#pragma endregion
		
		/******************
		*   Get Methods   *
		*******************/
#pragma region Get Methods

		/// <summary>
		/// Returns DirectX11 Pixel Shader
		/// </summary>
		/// <returns>DirectX11 Pixel Shader</returns>
		inline ID3D11PixelShader* GetPixelShaderDX() const
		{ 
			return m_PSDX;
		}

#pragma endregion

		/************************
		*   Framework Methods   *
		*************************/
#pragma region Framework Methods

#pragma endregion

};

#endif