/********************************************************
*
* Author: Carlos Chacón N.
*
* Copyright 2012-2015
*
*********************************************************/

#pragma once

#ifndef _VERTEX_SHADER_H
#define _VERTEX_SHADER_H

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
class VertexShader : public Shader
{
	private:

		/************************
		*   Private Variables   *
		*************************/
#pragma region Private Variables

		///<summary>
		///DirectX11 Vertex Shader
		///</summary>
		ID3D11VertexShader* m_VSDX = nullptr;

#pragma endregion

	protected:

		/************************
		*   Protected Methods   *
		*************************/
#pragma region Private Methods

		XEResult LoadShaderWithoutLock(const BYTE* shaderByteCode, uint32_t length) override;

#pragma endregion

		/************************************
		 *   Protected Destructor Methods   *
		 ************************************/
#pragma region Private Destructor Methods

		///<summary>
		///Default VertexShader Destructor
		///</summary>
		virtual ~VertexShader();

#pragma endregion

	public:

		/***************************
		 *   Constructor Methods   *
		 ***************************/
#pragma region Constructor Methods

		///<summary>
		///Default VertexShader Constructor
		///</summary>
		///<param name="graphicDevice">Graphic Device to be associated with this Shader</param>
		///<param name="name">Name of Shader</param>
		VertexShader(GraphicDevice* graphicDevice, const std::wstring& name = L"");

#pragma endregion
		
		/******************
		*   Get Methods   *
		*******************/
#pragma region Get Methods

		///<summary>
		///Returns DirectX11 Vertex Shader
		///</summary>
		///<returns>DirectX11 Vertex Shader</returns>
		inline ID3D11VertexShader* GetVertexShaderDX() const
		{ 
			return m_VSDX;
		}

#pragma endregion

		/************************
		*   Framework Methods   *
		*************************/
#pragma region Framework Methods

#pragma endregion

};

#endif