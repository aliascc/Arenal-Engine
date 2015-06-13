/********************************************************
*
* Author: Carlos Chacón N.
*
* Copyright 2012-2015
*
*********************************************************/

#pragma once

#ifndef _DOMAIN_SHADER_H
#define _DOMAIN_SHADER_H

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
class DomainShader sealed : public Shader
{
	private:

		/************************
		*   Private Variables   *
		*************************/
#pragma region Private Variables
				
		///<summary>
		///DirectX11 Domain Shader
		///</summary>
		ID3D11DomainShader* m_DSDX = nullptr;

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
		///Default DomainShader Destructor
		///</summary>
		virtual ~DomainShader();

#pragma endregion

	public:

		/***************************
		 *   Constructor Methods   *
		 ***************************/
#pragma region Constructor Methods

		///<summary>
		///Default DomainShader Constructor
		///</summary>
		///<param name="graphicDevice">Graphic Device to be associated with this Shader</param>
		///<param name="name">Name of Shader</param>
		DomainShader(GraphicDevice* graphicDevice, const std::wstring& name = L"");

#pragma endregion
		
		/******************
		*   Get Methods   *
		*******************/
#pragma region Get Methods

		///<summary>
		///Returns DirectX11 Domain Shader
		///</summary>
		///<returns>DirectX11 Domain Shader</returns>
		inline ID3D11DomainShader* GetDomainShaderDX() const
		{ 
			return m_DSDX;
		}

#pragma endregion

		/************************
		*   Framework Methods   *
		*************************/
#pragma region Framework Methods

#pragma endregion

};

#endif