/*
* Copyright (c) 2018 <Carlos Chacón>
* All rights reserved
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
* http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*/

#pragma once

#ifndef _SHADER_H
#define _SHADER_H

/**********************
*   System Includes   *
***********************/
#include <list>
#include <string>
#include <fstream>
#include <stdint.h>
#include <d3d11_1.h>

/*************************
*   3rd Party Includes   *
**************************/

/***************************
*   Game Engine Includes   *
****************************/
#include "Base\Base.h"
#include "ShaderDefs.h"
#include "ShaderSignatures.h"
#include "Resource\GameResource.h"

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
class Shader abstract : public GameResource
{	
	private:

		/*************************
		 *   Private Variables   *
		 *************************/
#pragma region Private Variables

		/// <summary>
		/// Type of the Shader
		/// </summary>
		ShaderType m_ShaderType = ShaderType::VertexShader;
		
#pragma endregion

		/***********************
		 *   Private Methods   *
		 ***********************/
#pragma region Private Methods

		AEResult ReadShaderByteCode(std::ifstream& fileStream, BYTE** shaderByteCode, uint32_t& shaderByteCodeSize);

		AEResult ReadShaderConstantBuffers(std::ifstream& fileStream, ConstantBufferSignatureList& cbSigList);

		AEResult ReadShaderSimpleBuffers(std::ifstream& fileStream, SimpleBufferSignatureList& simpleBufferSignatureList);

		AEResult ReadShaderStructuredBuffers(std::ifstream& fileStream, StructuredBufferSignatureList& structuredBufferSigList);

		AEResult ReadShaderTextureInputs(std::ifstream& fileStream, TextureInputSignatureList& texInputSigList);

		AEResult ReadShaderTextureArrayInputs(std::ifstream& fileStream, TextureArrayInputSignatureList& texArrayInputSigList);

		AEResult ReadShaderSamplers(std::ifstream& fileStream, SamplerSignatureList& samplerSignatureList);

#pragma endregion

	protected:

		/***************************
		 *   Protected Variables   *
		 ***************************/
#pragma region Protected Variables

		/// <summary>
		/// Graphic Device associated with the Texture
		/// </summary>
		GraphicDevice* m_GraphicDevice = nullptr;

		/// <summary>
		/// Lists Constant Buffer Signature, these are use to create 
		/// the Constant Buffers that this shader needs
		/// </summary>
		ConstantBufferSignatureList m_ConstantBufferSignatureList;

		/// <summary>
		/// Lists Simple Buffer Signature, these are use to create 
		/// the Simple Buffers that this shader needs
		/// </summary>
		SimpleBufferSignatureList m_SimpleBufferSignatureList;

		/// <summary>
		/// Lists Structured Buffer Signature, these are use to create 
		/// the Structured Buffers that this shader needs
		/// </summary>
		StructuredBufferSignatureList m_StructuredBufferSignatureList;

		/// <summary>
		/// Lists Texture Inputs Signature, these are use to create 
		/// the Texture Inputs that this shader needs
		/// </summary>
		TextureInputSignatureList m_TextureInputSignatureList;

		/// <summary>
		/// Lists Texture Array Inputs Signature, these are use to create 
		/// the Texture Array Inputs that this shader needs
		/// </summary>
		TextureArrayInputSignatureList m_TextureArrayInputSignatureList;

		/// <summary>
		/// Lists Sampler Signature, these are use to create 
		/// the Samplers that this shader needs
		/// </summary>
		SamplerSignatureList m_SamplerSignatureList;

#pragma endregion

		/**************************
		 *   Protected Methods    *
		 **************************/
#pragma region Protected Methods
		
		virtual AEResult LoadShaderWithoutLock(const BYTE* shaderByteCode, uint32_t length) = 0;

#pragma endregion

		/***********************************
		*   Protected Destructor Methods   *
		************************************/
#pragma region Protected Destructor Methods

		/// <summary>
		/// Default Shader Destructor
		/// </summary>
		virtual ~Shader();

#pragma endregion

	public:

		/**************************
		*   Constructor Methods   *
		***************************/
#pragma region Constructor Methods

		/// <summary>
		/// Default Shader Constructor
		/// </summary>
		/// <param name="graphicDevice">Graphic Device to be associated with this Shader</param>
		/// <param name="name">Name of Shader</param>
		/// <param name="shaderType">Type of the Shader</param>
		Shader(GraphicDevice* graphicDevice, ShaderType shaderType, const std::wstring& name = L"");

#pragma endregion

		/**********************
		 *     Get Methods    *
		 **********************/
#pragma region Get Methods
		
		/// <summary>
		/// Gets the Shader Type
		/// </summary>
		/// <returns>Get the Type of the Shader</returns>
		inline ShaderType GetShaderType() const
		{
			return m_ShaderType;
		}

		inline const ConstantBufferSignatureList& GetConstantBufferSignatureList() const
		{
			return m_ConstantBufferSignatureList;
		}

		inline const SamplerSignatureList& GetSamplerSignatureList() const
		{
			return m_SamplerSignatureList;
		}

		inline const SimpleBufferSignatureList& GetSimpleBufferSignatureList() const
		{
			return m_SimpleBufferSignatureList;
		}

		inline const StructuredBufferSignatureList& GetStructuredBufferSignatureList() const
		{
			return m_StructuredBufferSignatureList;
		}

		inline const TextureInputSignatureList& GetTextureInputSignatureList() const
		{
			return m_TextureInputSignatureList;
		}

		inline const TextureArrayInputSignatureList& GetTextureArrayInputSignatureList() const
		{
			return m_TextureArrayInputSignatureList;
		}

		inline GraphicDevice* GetGraphicDevice()
		{
			return m_GraphicDevice;
		}

#pragma endregion
		
		/**********************
		 *     Set Methods    *
		 **********************/
#pragma region Set Methods

#pragma endregion

		/**************************
		 *   Framework Methods    *
		 **************************/
#pragma region Framework Methods
		
		virtual AEResult Load() override;

		AEResult LoadShader(const BYTE* shaderByteCode, uint32_t length);

#pragma endregion

};

#endif