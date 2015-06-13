/********************************************************
*
* Author: Carlos Chacón N.
*
* Copyright 2012-2015
*
*********************************************************/

#pragma once

#ifndef _STRUCTURED_BUFFER_H
#define _STRUCTURED_BUFFER_H

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
class StructuredBuffer sealed : public ShaderBuffer
{
	private:

		/*************************
		 *   Private Variables   *
		 *************************/
#pragma region Private Variables

		/// <summary>
		/// Size of the Structured that this buffer will hold
		/// </summary>
		uint32_t m_StructSize = 0;

#pragma endregion

	public:

		/****************************************
		 *   Constructor & Destructor Methods   *
		 ****************************************/
#pragma region Constructor & Destructor Methods

		/// <summary>
		/// StructuredBuffer Constructor
		/// </summary>
		/// <param name="name">Name of the Binding</param>
		/// <param name="bindIndex">Index where the Texture is going to be bound to the shader</param>
		/// <param name="createAsRW">If true, enables Read and Write usage in the shader for the Buffer</param>
		/// <param name="graphicDevice">Graphic Device to create structured buffer</param>
		StructuredBuffer(const std::wstring& name, uint32_t bindIndex, bool createAsRW, GraphicDevice* graphicDevice);

		/// <summary>
		/// Default StructuredBuffer Destructor
		/// </summary>
		virtual ~StructuredBuffer();

#pragma endregion

		/********************
		 *   Get Methods    *
		 ********************/
#pragma region Get Methods

		/// <summary>
		/// Gets the Size in bytes of the Struct this buffer holds
		/// </summary>
		/// <returns>Size in bytes of the Struct this buffer holds</returns>
		inline uint32_t GetStructSize() const
		{
			return m_StructSize;
		}

#pragma endregion

		/********************
		 *   Set Methods    *
		 ********************/
#pragma region Set Methods

#pragma endregion

		/*************************
		 *   Framework Methods   *
		 *************************/
#pragma region Framework Methods

		/// <summary>
		/// Initialize the Structured Buffer
		/// </summary>
		/// <param name="structSize">Size in bytes of the Struct this buffer will hold</param>
		/// <param name="numElements">Number of elements the Buffer will contain</param>
		/// <param name="bufferUsage">Buffer Usage for the Buffer, lets know how is going to be bound to the pipeline</param>
		/// <param name="bufferAccess">Buffer Access for the Buffer, lets know if the CPU needs to read or write or none</param>
		/// <returns>AEResult::OK if successful</returns>
		AEResult InitializeBuffer(uint32_t structSize, uint32_t numElements, GraphicBufferUsage bufferUsage, GraphicBufferAccess bufferAccess);

		/// <summary>
		/// Copies the data to the structure buffer
		/// </summary>
		/// <param name="data">Data to copy</param>
		/// <param name="elementCount">Number of elements to copy</param>
		/// <param name="startIndex">Start Index in Structure buffer to begin the copy</param>
		/// <param name="offset">Offset in Elements in the Structure buffer where to begin copying to</param>
		/// <param name="resourceMap">Tells how the Structured Buffer will be access (Valid only: Write, WriteDiscard)</param>
		/// <returns>AEResult::Ok if copy succeeded</returns>
		AEResult UpdateBuffer(const void* data, uint32_t elementCount, uint32_t startIndex, uint32_t offset, GraphicResourceMap resourceMap);

		/// <summary>
		/// Deinitializies the Buffer
		/// </summary>
		/// <returns>Returns AEResult::Ok if successful</returns>
		AEResult Deinitialize();

#pragma endregion

};

#endif