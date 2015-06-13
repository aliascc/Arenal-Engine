/********************************************************
*
* Author: Carlos Chacón N.
*
* Copyright 2012-2015
*
*********************************************************/

#pragma once

#ifndef _COMPUTE_SHADER_H
#define _COMPUTE_SHADER_H

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

/*****************
*   Class Decl   *
******************/
class ComputeShader sealed : public Shader
{
	private:

		/************************
		*   Private Variables   *
		*************************/
#pragma region Private Variables
				
		///<summary>
		///DirectX11 Compute Shader
		///</summary>
		ID3D11ComputeShader* m_CSDX = nullptr;

#pragma endregion

		/************************
		 *   Private Methods    *
		 ************************/
#pragma region Private Methods

		AEResult LoadShaderWithoutLock(const BYTE* shaderByteCode, uint32_t length) override;

#pragma endregion

		/**********************************
		 *   Private Destructor Methods   *
		 **********************************/
#pragma region Private Destructor Methods

		///<summary>
		///Default ComputeShader Destructor
		///</summary>
		virtual ~ComputeShader();

#pragma endregion

	public:

		/***************************
		 *   Constructor Methods   *
		 ***************************/
#pragma region Constructor Methods

		///<summary>
		///Default ComputeShader Constructor
		///</summary>
		///<param name="graphicDevice">Graphic Device to be associated with this Shader</param>
		///<param name="name">Name of Shader</param>
		ComputeShader(GraphicDevice* graphicDevice, const std::wstring& name = L"");

#pragma endregion
		
		/******************
		*   Get Methods   *
		*******************/
#pragma region Get Methods

		///<summary>
		///Returns DirectX11 Compute Shader
		///</summary>
		///<returns>DirectX11 Compute Shader</returns>
		inline ID3D11ComputeShader* GetComputeShaderDX() const
		{ 
			return m_CSDX;
		}

#pragma endregion

		/************************
		*   Framework Methods   *
		*************************/
#pragma region Framework Methods

#pragma endregion

};

#endif