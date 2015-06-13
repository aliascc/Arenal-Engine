/********************************************************
*
* Author: Carlos Chacón N.
*
* Copyright 2012-2015
*
*********************************************************/

#pragma once

#ifndef _SHADER_BUFFER_H
#define _SHADER_BUFFER_H

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
#include "Shaders\Bindings\ShaderBinding.h"

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
class ShaderBuffer abstract : public ShaderBinding
{
	protected:

		/***************************
		 *   Protected Variables   *
		 ***************************/
#pragma region Protected Variables

		/// <summary>
		/// DirectX 11 Buffer to hold Buffer of the Shader
		/// </summary>
		ID3D11Buffer* m_BufferDX = nullptr;

		/// <summary>
		/// DirectX 11 Shader Resource View to Bind the Buffer as an input to the shader
		/// </summary>
		ID3D11ShaderResourceView* m_BufferSRVDX = nullptr;

		/// <summary>
		/// DirectX 11 Unordered Access View to Bind the Buffer as an input/output to the shader
		/// </summary>
		ID3D11UnorderedAccessView* m_BufferUAVDX = nullptr;

		/// <summary>
		/// Buffer Usage Flag for the Buffer, to let know how it is going to be bound to the pipeline
		/// </summary>
		GraphicBufferUsage m_BufferUsage = GraphicBufferUsage::Default;

		/// <summary>
		/// Buffer Access Flags for the Buffer, lets know if CPU is going to read or write or none
		/// </summary>
		GraphicBufferAccess m_BufferAccess = GraphicBufferAccess::None;

		/// <summary>
		/// Number of Elements the Buffer will contain
		/// </summary>
		uint32_t m_NumElements = 0;

		/// <summary>
		/// Is Read and Write Enabled (Creates UAV, allows read and write access in the shader)
		/// </summary>
		bool m_IsRWEnabled = false;

		/// <summary>
		/// Is Class Ready to start, all parameters in constructor are ok
		/// </summary>
		bool m_IsStartReady = false;

		/// <summary>
		/// Is Buffer Ready
		/// </summary>
		bool m_IsReady = false;

		/// <summary>
		/// Graphic Device to create and work Buffer
		/// </summary>
		GraphicDevice* m_GraphicDevice = nullptr;

#pragma endregion

		/*************************
		 *   Protected Methods   *
		 *************************/
#pragma region Protected Methods

		void CleanUp();

#pragma endregion

	public:

		/****************************************
		 *   Constructor & Destructor Methods   *
		 ****************************************/
#pragma region Constructor & Destructor Methods

		/// <summary>
		/// ShaderBuffer Constructor
		/// </summary>
		/// <param name="name">Name of the Binding</param>
		/// <param name="bindIndex">Index where the Texture is going to be bound to the shader</param>
		/// <param name="createAsRW">If true, enables Read and Write usage in the shader for the Buffer</param>
		/// <param name="graphicDevice">Graphic Device to create buffer</param>
		ShaderBuffer(const std::wstring& name, uint32_t bindIndex, bool createAsRW, GraphicDevice* graphicDevice);

		/// <summary>
		/// Default ShaderBuffer Destructor
		/// </summary>
		virtual ~ShaderBuffer();

#pragma endregion

		/********************
		 *   Get Methods    *
		 ********************/
#pragma region Get Methods

		/// <summary>
		/// Number of Elements the Buffer contains
		/// </summary>
		/// <returns>Number of Elements the Buffer will contain</returns>
		inline uint32_t GetNumElements() const
		{
			return m_NumElements;
		}

		/// <summary>
		/// Gets if Buffer has been initialized to read and write in the shader
		/// </summary>
		/// <returns>True if Buffer is set to read and write, false if only to read</returns>
		inline bool IsRWEnabled() const
		{
			return m_IsRWEnabled;
		}

		/// <summary>
		/// Gets if Buffer has been initialized and ready to use
		/// </summary>
		/// <returns>True if Buffer is ready to use</returns>
		inline bool IsReady() const
		{
			return m_IsReady;
		}

		/// <summary>
		/// Gets the Buffer Usage for the Buffer
		/// </summary>
		/// <returns>Buffer Usage for the Buffer</returns>
		inline GraphicBufferUsage GetBufferUsage() const
		{
			return m_BufferUsage;
		}

		/// <summary>
		/// Gets Buffer Access Flags for the Buffer
		/// </summary>
		/// <returns>Buffer Access for the Buffer</returns>
		inline GraphicBufferAccess GetBufferAccess() const
		{
			return m_BufferAccess;
		}

		/// <summary>
		/// Gets the DirectX Shader Resource View of the Buffer
		/// </summary>
		/// <returns>SRV of Buffer</returns>
		inline ID3D11ShaderResourceView* GetBufferSRVDX() const
		{
			return m_BufferSRVDX;
		}

		/// <summary>
		/// Gets the DirectX Unordered Access View of the Buffer
		/// </summary>
		/// <returns>SRV of Buffer</returns>
		inline ID3D11UnorderedAccessView* GetBufferUAVDX() const
		{
			return m_BufferUAVDX;
		}

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
		/// Deinitializies the Buffer
		/// </summary>
		/// <returns>Returns AEResult::Ok if successful</returns>
		virtual AEResult Deinitialize() = 0;

#pragma endregion

};

#endif