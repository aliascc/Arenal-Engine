/********************************************************
*
* Author: Carlos Chacón N.
*
* Copyright 2012-2015
*
*********************************************************/

#pragma once

#ifndef _SIMPLE_BUFFER_H
#define _SIMPLE_BUFFER_H

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
#include "GraphicsDefs.h"
#include "Textures\TextureDefs.h"
#include "Shaders\Buffers\ShaderBuffer.h"

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
class SimpleBuffer sealed : public ShaderBuffer 
{
	private:

		/*************************
		 *   Private Variables   *
		 *************************/
#pragma region Private Variables

		ShaderVariableClass m_VariableClass = ShaderVariableClass::Scalar;

		ShaderVariableType m_VariableType = ShaderVariableType::Int;

		uint32_t m_ElementCount = 0;

#pragma endregion

		/***********************
		 *   Private Methods   *
		 ***********************/
#pragma region Private Methods

		XEResult CheckShaderVariableInfo(ShaderVariableClass variableClass,  ShaderVariableType variableType, uint32_t elementCount);

		DXGI_FORMAT GetDXGIFormat();

#pragma endregion

	public:

		/****************************************
		 *   Constructor & Destructor Methods   *
		 ****************************************/
#pragma region Constructor & Destructor Methods

		/// <summary>
		/// SimpleBuffer Constructor
		/// </summary>
		/// <param name="name">Name of the Binding</param>
		/// <param name="bindIndex">Index where the Texture is going to be bound to the shader</param>
		/// <param name="createAsRW">If true, enables Read and Write usage in the shader for the Buffer</param>
		/// <param name="variableClass">Variable Class for the Simple Buffer</param>
		/// <param name="variableType">Variable Type for the Simple Buffer</param>
		/// <param name="elementCount">Number of elements per variable (That is a vector2 will be 2, vector3 will be 3, a normal scalar would be 1).</param>
		/// <param name="graphicDevice">Graphic Device to create buffer</param>
		SimpleBuffer(const std::wstring& name, uint32_t bindIndex, bool createAsRW, ShaderVariableClass variableClass,  ShaderVariableType variableType, uint32_t elementCount, GraphicDevice* graphicDevice);

		/// <summary>
		/// Default SimpleBuffer Destructor
		/// </summary>
		virtual ~SimpleBuffer();

#pragma endregion

		/********************
		 *   Get Methods    *
		 ********************/
#pragma region Get Methods

#pragma endregion

		/*******************
		 *   Set Methods   *
		 *******************/
#pragma region Set Methods

#pragma endregion

		/*************************
		 *   Framework Methods   *
		 *************************/
#pragma region Framework Methods

		/// <summary>
		/// Initialize the Buffer
		/// </summary>
		/// <param name="numElements">Number of elements the Simple Buffer will contain</param>
		/// <param name="bufferUsage">Buffer Usage for the Buffer, lets know how is going to be bound to the pipeline</param>
		/// <param name="bufferAccess">Buffer Access for the Buffer, lets know if the CPU needs to read or write or none</param>
		/// <returns>XEResult::OK if successful</returns>
		XEResult InitializeBuffer(uint32_t numElements, GraphicBufferUsage bufferUsage, GraphicBufferAccess bufferAccess);

		/// <summary>
		/// Copies the data to the buffer
		/// </summary>
		/// <param name="data">Data to copy</param>
		/// <param name="numElements">Number of elements to copy</param>
		/// <param name="startIndex">Start Index in buffer to begin the copy</param>
		/// <param name="offset">Offset in Elements in the buffer where to begin copying to</param>
		/// <param name="resourceMap">Tells how the Buffer will be access (Valid only: Write, WriteDiscard)</param>
		/// <returns>XEResult::Ok if copy succeeded</returns>
		XEResult UpdateBuffer(void* data, uint32_t numElements, uint32_t startIndex, uint32_t offset, GraphicResourceMap resourceMap);

		/// <summary>
		/// Deinitializies the Buffer
		/// </summary>
		/// <returns>Returns XEResult::Ok if successful</returns>
		XEResult Deinitialize();

#pragma endregion

};

#endif