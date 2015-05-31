/********************************************************
*
* Author: Carlos Chacón N.
*
* Copyright 2012-2015
*
*********************************************************/

#pragma once

#ifndef _VERTEX_BUFFER_H
#define _VERTEX_BUFFER_H

/**********************
*   System Includes   *
***********************/
#include <assert.h>
#include <vector>
#include <stdint.h>
#include <d3d11_1.h>

/*************************
*   3rd Party Includes   *
**************************/

/***************************
*   Game Engine Includes   *
****************************/
#include "Base\Base.h"
#include "VertexDefs.h"
#include "GraphicsDefs.h"
#include "VertexLayout.h"
#include "IVertexBuffer.h"
#include "cppformat\format.h"
#include "Base\BaseFunctions.h"
#include "Localization\LocalizationManager.h"

/********************
*   Forward Decls   *
*********************/
class GraphicDevice;

/*****************
*   Class Decl   *
******************/
template<class T>
class VertexBuffer sealed : public IVertexBuffer
{
	private:
		
		/************************
		*   Private Variables   *
		*************************/
#pragma region Private Variables
		
		/// <summary>
		/// Vertex Buffer in RAM
		/// </summary>
		T* m_VertexBuffer = nullptr;
		
		/// <summary>
		/// When Map, this is the Vertex Buffer(VRAM)
		/// </summary>
		T* m_VideoVB = nullptr;

#pragma endregion

		/********************************
		*   Private Framework Methods   *
		*********************************/
#pragma region Private Framework Methods
		
		/// <summary>
		/// Commit new Data from Vertex Buffer in RAM to VRAM
		/// </summary>
		/// <param name="offset">Offset in Vertex Buffer(RAM) to where to begin copying from</param>
		/// <param name="elementCount">Number of elements to copy</param>
		/// <returns>XEResult::Ok commit succeeded</returns>	
		XEResult CommitVertexData(uint32_t offset, uint32_t elementCount)
		{
			if(m_VideoVB == nullptr)
			{
				return XEResult::Fail;
			}

			uint32_t copySize = VertexSize() * elementCount;
			memcpy(m_VideoVB + offset, m_VertexBuffer + offset, copySize);

			return XEResult::Ok;
		}
		
		/// <summary>
		/// Map the VRAM Vertex Buffer
		/// </summary>
		/// <param name="mapType">Tells how the Vertex Buffer will be access</param>
		/// <returns>XEResult::Ok Map succeeded</returns>
		XEResult MapBuffer(D3D11_MAP mapType)
		{
			D3D11_MAPPED_SUBRESOURCE mappedData = { 0 };

			XETODO("Check Map flags");
			XETODO("Check Sub Resource");
			HRESULT hr = m_GraphicDevice->GetDeviceContextDX()->Map(m_VertexBufferDX, 0, mapType, 0, &mappedData);

			if(hr != S_OK)
			{
				DisplayError(hr);

				return XEResult::Fail;
			}

			m_VideoVB = static_cast<T*>(mappedData.pData);

			return XEResult::Ok;
		}

		/// <summary>
		/// UnMap the Vertex Buffer from VRAM
		/// </summary>
		/// <returns>XEResult::Ok UnMap succeeded</returns>
		XEResult UnMapBuffer()
		{
			XETODO("Check Subresource");
			m_GraphicDevice->GetDeviceContextDX()->Unmap(m_VertexBufferDX, 0);

			m_VideoVB = nullptr;

			return XEResult::Ok;
		}

#pragma endregion

	public:

		/***************************************
		*   Constructor & Destructor Methods   *
		****************************************/
#pragma region Constructor & Destructor Methods

		/// <summary>
		/// VertexBuffer Constructor
		/// </summary>
		/// <param name="graphicDevice">Graphic Device to be associated with this Vertex Buffer</param>
		/// <param name="bufferUsage">How will the buffer be use</param>
		/// <param name="bufferAccess">How will the CPU Access the memory of the buffer</param>
		VertexBuffer(GraphicDevice* graphicDevice, GraphicBufferUsage bufferUsage = GraphicBufferUsage::Static, GraphicBufferAccess bufferAccess = GraphicBufferAccess::None)
			: IVertexBuffer(graphicDevice, bufferUsage, bufferAccess)
			, m_VertexBuffer(nullptr)
			, m_VideoVB(nullptr)
		{
		}
		
		/// <summary>
		/// Default VertexBuffer Destructor
		/// </summary>
		virtual ~VertexBuffer()
		{
			DeleteMemArr(m_VertexBuffer);
		}

#pragma endregion

		/***************************
		*   Override Get Methods   *
		****************************/
#pragma region Override Get Methods

		/// <summary>
		/// Returns Vertex Type for this Buffer
		/// </summary>
		/// <returns>Vertex Type for Buffer</returns>
		virtual VertexType GetVertexType() const
		{
			return T::GetVertexType();
		}

#pragma endregion

		/************************
		*   Framework Methods   *
		*************************/
#pragma region Framework Methods

		/// <summary>
		/// Copies the array to the Vertex Buffer and makes it ready for use
		/// </summary>
		/// <param name="vertexBuff">Array to be copied</param>
		/// <param name="size">Size of the Array</param>
		XEResult CopyVertexBuffer(T vertexBuff[], uint32_t size)
		{
			XEAssert(vertexBuff != nullptr);

			if(vertexBuff == nullptr)
			{
				return XEResult::Fail;
			}

			//Delete Any Vertex Buffer 
			DeleteMemArr(m_VertexBuffer);

			m_VertexBuffer = new T[size];
			m_Size = size;

			memcpy(m_VertexBuffer, vertexBuff, VertexSize() * size);

			m_IsReady = false;

			return XEResult::Ok;
		}

		/// <summary>
		/// Copies the array to the Vertex Buffer and makes it ready for use
		/// </summary>
		/// <param name="vertexBuff">Array to be copied</param>
		void CopyVertexBuffer(const std::vector<T>& vertexBuff)
		{
			//Delete Any Vertex Buffer 
			DeleteMemArr(m_VertexBuffer);
			
			uint32_t vbSize = (uint32_t)vertexBuff.size();

			m_VertexBuffer = new T[vbSize];
			m_Size = vbSize;

			for(uint32_t i = 0; i < vbSize; ++i)
			{
				m_VertexBuffer[i] = vertexBuff[i];
			}

			m_IsReady = false;
		}
		
		/// <summary>
		/// Copies the data from the array to the Vertex Buffer
		/// </summary>
		/// <param name="offset">Offset in array to where to begin copying from</param>
		/// <param name="vertexData">Vertex Array</param>
		/// <param name="startIndex">Start Index in Vertex Buffer to begin the copy</param>
		/// <param name="elementCount">Number of elements to copy</param>
		/// <param name="mapType">Tells how the Vertex Buffer will be access</param>
		/// <returns>XEResult::Ok if copy succeeded</returns>
		XEResult SetVertexData(uint32_t offset, T vertexData[], uint32_t startIndex, uint32_t elementCount, D3D11_MAP mapType)
		{
			XEAssert(vertexData != nullptr);

			if(!m_IsReady)
			{
				if(BuildVertexBuffer() != XEResult::Ok)
				{
					return XEResult::Fail;
				}
			} 

			if(startIndex >= m_Size || (startIndex + elementCount) > m_Size)
			{
				return XEResult::Fail;
			}

			if(MapBuffer(mapType) != XEResult::Ok)
			{
				return XEResult::Fail;
			}
				
			uint32_t copySize = VertexSize() * elementCount;
			memcpy(m_VertexBuffer + startIndex, vertexData + offset, copySize);

			if(CommitVertexData(startIndex, elementCount) != XEResult::Ok)
			{
				return XEResult::Fail;
			}

			UnMapBuffer();
			
			return XEResult::Ok;
		}

#pragma endregion
		
		/***********************************
		 *   Override Framework Methods    *
		 **********************************/
#pragma region Override Framework Methods

		/// <summary>
		/// Builds the Vertex Buffer.
		/// </summary>
		/// <returns>XEResult::Ok if Build Vertex Buffer succeeded</returns>
		XEResult BuildVertexBuffer() override
		{
			XEAssert(m_GraphicDevice != nullptr);

			if(m_VertexBuffer == nullptr || m_IsReady || m_GraphicDevice == nullptr)
			{
				return XEResult::Fail;
			}

			D3D11_BUFFER_DESC vbDesc = { 0 };
			
			vbDesc.Usage = XEGraphicHelpers::GetDXUsage(m_BufferUsage);
			vbDesc.ByteWidth = sizeof(T) * m_Size;
			vbDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
			vbDesc.CPUAccessFlags = XEGraphicHelpers::GetDXBufferAccess(m_BufferAccess);
			vbDesc.MiscFlags = 0;
			vbDesc.StructureByteStride = 0;
			
			D3D11_SUBRESOURCE_DATA vbInitData = { 0 };
			vbInitData.pSysMem = m_VertexBuffer;
			
			HRESULT hr = m_GraphicDevice->GetDeviceDX()->CreateBuffer(&vbDesc, &vbInitData, &m_VertexBufferDX);

			if(hr != S_OK)
			{
				DisplayError(hr);

				return XEResult::Fail;
			}

			XEGraphicHelpers::SetDebugObjectName<ID3D11Buffer>(m_VertexBufferDX, XE_DEBUG_VB_NAME_PREFIX + XE_Base::WideStr2String(m_Name));

			m_IsReady = true;

			return XEResult::Ok;
		}

		/// <summary>
		/// Size of the Vertex that the Vertex Buffer has
		/// </summary>
		/// <returns>Vertex Size</returns>
		inline uint32_t VertexSize() const override		
		{
			return sizeof(T);
		}
		
		/// <summary>
		/// Vertex Layout of the Vertex that the Vertex Buffer has
		/// </summary>
		/// <returns>Vertex Layout of the Vertex</returns>
		const VertexLayout* GetVertexLayout() override
		{
			if(m_VertexBuffer == nullptr || !m_IsReady)
			{
				return nullptr;
			}

			return T::GetVertexLayout(m_GraphicDevice);
		}

#pragma endregion
		
};

#endif