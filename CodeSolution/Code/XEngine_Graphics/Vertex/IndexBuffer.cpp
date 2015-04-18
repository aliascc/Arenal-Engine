/********************************************************
*
* Author: Carlos Chacón N.
*
* Created: 2/22/2012
*
* Desc:
*
*
* Copyright 2012
*
*********************************************************/

/**********************
*   System Includes   *
***********************/

/*************************
*   3rd Party Includes   *
**************************/
#include "fastformat\fastformat.hpp"
#include "fastformat\sinks\ostream.hpp"

/***************************
*   Game Engine Includes   *
****************************/
#include "IndexBuffer.h"
#include "GraphicDevice.h"
#include "Logger\Logger.h"
#include "Base\BaseFunctions.h"
#include "Localization\LocalizationManager.h"

//Always include last
#include "Memory\MemLeaks.h"

/********************
*   Function Defs   *
*********************/
IndexBuffer::IndexBuffer(GraphicDevice* graphicDevice, GraphicBufferUsage bufferUsage, GraphicBufferAccess bufferAccess)
	: Named(L"")
	, m_BufferAccess(bufferAccess)
	, m_BufferUsage(bufferUsage)
	, m_GraphicDevice(graphicDevice)
{
	XEAssert(m_GraphicDevice != nullptr);
}

IndexBuffer::~IndexBuffer()
{
	CleanUpIB();

	ReleaseCOM(m_IndexBufferDX);
}

void IndexBuffer::CleanUpIB()
{
	DeleteMemArr(m_IndexBuffer_32);
	DeleteMemArr(m_IndexBuffer_16);
}

void IndexBuffer::CopyToIndexBuffer(uint32_t indexBuffer[], uint32_t size)
{
	XEAssert(indexBuffer != nullptr);

	if(indexBuffer == nullptr)
	{
		return;
	}

	//Delete Any Index Buffer 
	CleanUpIB();

	m_IndexBuffer_32 = new uint32_t[size];
	m_IBType = IndexBufferType::Index32;
	m_Size = size;

	memcpy(m_IndexBuffer_32, indexBuffer, sizeof(uint32_t) * size);

	m_IsReady = false;
}

void IndexBuffer::CopyToIndexBuffer(uint16_t indexBuffer[], uint32_t size)
{
	XEAssert(indexBuffer != nullptr);

	if(indexBuffer == nullptr)
	{
		return;
	}

	//Delete Any Index Buffer 
	CleanUpIB();

	m_IndexBuffer_16 = new uint16_t[size];
	m_IBType = IndexBufferType::Index16;
	m_Size = size;

	memcpy(m_IndexBuffer_16, indexBuffer, sizeof(uint16_t) * size);

	m_IsReady = false;
}

void IndexBuffer::CopyToIndexBuffer(const std::vector<uint32_t>& indexBuffer)
{
	//Delete Any Index Buffer 
	CleanUpIB();

	uint32_t ibSize = (uint32_t)indexBuffer.size();

	m_IndexBuffer_32 = new uint32_t[ibSize];
	m_IBType = IndexBufferType::Index32;
	m_Size = ibSize;

	for(uint32_t i = 0; i < ibSize; ++i)
	{
		m_IndexBuffer_32[i] = indexBuffer[i];
	}

	m_IsReady = false;
}

void IndexBuffer::CopyToIndexBuffer(const std::vector<uint16_t>& indexBuffer)
{
	//Delete Any Index Buffer 
	CleanUpIB();

	uint32_t ibSize = (uint32_t)indexBuffer.size();

	m_IndexBuffer_16 = new uint16_t[ibSize];
	m_IBType = IndexBufferType::Index16;
	m_Size = ibSize;

	for(uint32_t i = 0; i < ibSize; ++i)
	{
		m_IndexBuffer_16[i] = indexBuffer[i];
	}

	m_IsReady = false;
}

XEResult IndexBuffer::BuildIndexBuffer()
{
	if(m_IsReady)
	{
		return XEResult::Ok;
	}

	if(m_Size == 0)
	{
		return XEResult::ZeroSize;
	}

	if(m_IndexBuffer_16 == nullptr && m_IBType == IndexBufferType::Index16)
	{
		return XEResult::NullObj;
	}

	if(m_IndexBuffer_32 == nullptr && m_IBType == IndexBufferType::Index32)
	{
		return XEResult::NullObj;
	}

	//Release DirectX Vertex Buffer if it has been created previously
	ReleaseCOM(m_IndexBufferDX);
	m_IsReady = false;

	D3D11_BUFFER_DESC ibDesc = { 0 };

	ibDesc.Usage				= XEGraphicHelpers::GetDXUsage(m_BufferUsage);
	ibDesc.ByteWidth			= GetIndexSize() * m_Size;
	ibDesc.BindFlags			= D3D11_BIND_INDEX_BUFFER;
	ibDesc.CPUAccessFlags		= XEGraphicHelpers::GetDXBufferAccess(m_BufferAccess);
	ibDesc.MiscFlags			= 0;
	ibDesc.StructureByteStride	= 0;

	D3D11_SUBRESOURCE_DATA ibInitData = { 0 };

	switch (m_IBType)
	{
		case IndexBufferType::Index16:
			ibInitData.pSysMem = m_IndexBuffer_16;
			break;

		case IndexBufferType::Index32:
			ibInitData.pSysMem = m_IndexBuffer_32;
			break;

		default:
			//Should never get here
			XEAssert(false);
			break;
	}

	HRESULT hr = m_GraphicDevice->GetDeviceDX()->CreateBuffer(&ibDesc, &ibInitData, &m_IndexBufferDX);

	if(hr != S_OK)
	{
		DisplayError(hr);

		return XEResult::FailIndexBufferInit;
	}

	XEGraphicHelpers::SetDebugObjectName<ID3D11Buffer>(m_IndexBufferDX, XE_DEBUG_IB_NAME_PREFIX + XE_Base::WideStr2String(m_Name));

	m_IsReady = true;

	return XEResult::Ok;
}

XEResult IndexBuffer::UpdateIndexBuffer(uint32_t offset, uint16_t indexData[], uint32_t startIndex, uint32_t elementCount, GraphicResourceMap resourceMap)
{
	return UpdateIndexBufferPrivate(IndexBufferType::Index16, offset, indexData, startIndex, elementCount, resourceMap);
}

XEResult IndexBuffer::UpdateIndexBuffer(uint32_t offset, uint32_t indexData[], uint32_t startIndex, uint32_t elementCount, GraphicResourceMap resourceMap)
{
	return UpdateIndexBufferPrivate(IndexBufferType::Index32, offset, indexData, startIndex, elementCount, resourceMap);
}

XEResult IndexBuffer::UpdateIndexBufferPrivate(IndexBufferType IBType, uint32_t offset, void* indexData, uint32_t startIndex, uint32_t elementCount, GraphicResourceMap resourceMap)
{
	XEAssert(resourceMap == GraphicResourceMap::Write || resourceMap == GraphicResourceMap::WriteDiscard || resourceMap == GraphicResourceMap::WriteNoOvewrite);
	if(resourceMap != GraphicResourceMap::Write && resourceMap != GraphicResourceMap::WriteDiscard && resourceMap != GraphicResourceMap::WriteNoOvewrite)
	{
		return XEResult::InvalidResourceMapType;
	}

	XEAssert(indexData != nullptr);
	if(indexData == nullptr)
	{
		return XEResult::NullParameter;
	}

	XEAssert(IBType == m_IBType);
	if(IBType != m_IBType)
	{
		return XEResult::InvalidIndexBufferType;
	}

	if(!m_IsReady)
	{
		if(BuildIndexBuffer() != XEResult::Ok)
		{
			return XEResult::FailIndexBufferInit;
		}
	}

	if(startIndex >= m_Size || (startIndex + elementCount) > m_Size)
	{
		return XEResult::OutsideRange;
	}

	XEResult ret = XEResult::Ok;

	ret = MapBuffer(resourceMap);

	if(ret != XEResult::Ok)
	{
		return ret;
	}

	uint32_t copySize = GetIndexSize() * elementCount;

	switch(m_IBType)
	{
		case IndexBufferType::Index16:
			{
				uint16_t* indexData16 = reinterpret_cast<uint16_t*>(indexData);
				memcpy(m_IndexBuffer_16 + startIndex, indexData16 + offset, copySize);
			}
			break;

		case IndexBufferType::Index32:
			{
				uint32_t* indexData32 = reinterpret_cast<uint32_t*>(indexData);
				memcpy(m_IndexBuffer_32 + startIndex, indexData32 + offset, copySize);
			}
			break;

		default:
			//Should never get here
			XEAssert(false);
			break;
	}

	ret = CommitIndexData(startIndex, elementCount);

	UnMapBuffer();

	if(ret != XEResult::Ok)
	{
		return XEResult::FailIndexBufferUpdate;
	}

	return XEResult::Ok;
}

XEResult IndexBuffer::MapBuffer(GraphicResourceMap resourceMap)
{
	if(!m_IsReady)
	{
		return XEResult::NotReady;
	}

	D3D11_MAPPED_SUBRESOURCE mappedData = { 0 };

	D3D11_MAP dxMapType = XEGraphicHelpers::GetDXResourceMap(resourceMap);

	XETODO("Check Map flags");
	XETODO("Check Sub Resource");
	HRESULT hr = m_GraphicDevice->GetDeviceContextDX()->Map(m_IndexBufferDX, 0, dxMapType, 0, &mappedData);

	if(hr != S_OK)
	{
		DisplayError(hr);

		return XEResult::ResourceMapFailed;
	}

	switch (m_IBType)
	{
		case IndexBufferType::Index16:
			m_VideoIB_16 = static_cast<uint16_t*>(mappedData.pData);
			break;

		case IndexBufferType::Index32:
			m_VideoIB_32 = static_cast<uint32_t*>(mappedData.pData);
			break;

		default:
			//Should never get here
			XEAssert(false);
			break;
	}

	return XEResult::Ok;
}

XEResult IndexBuffer::UnMapBuffer()
{
	XETODO("Check subresource")
	m_GraphicDevice->GetDeviceContextDX()->Unmap(m_IndexBufferDX, 0);

	//Set VideoIB to nullptr
	m_VideoIB_32 = nullptr;
	m_VideoIB_16 = nullptr;

	return XEResult::Ok;
}

XEResult IndexBuffer::CommitIndexData(uint32_t offset, uint32_t elementCount)
{
	uint32_t copySize = GetIndexSize() * elementCount;

	if(m_VideoIB_16 == nullptr && m_VideoIB_32 == nullptr)
	{
		return XEResult::NullObj;
	}

	switch (m_IBType)
	{
		case IndexBufferType::Index16:
			memcpy(m_VideoIB_16 + offset, m_IndexBuffer_16 + offset, copySize);
			break;

		case IndexBufferType::Index32:
			memcpy(m_VideoIB_32 + offset, m_IndexBuffer_32 + offset, copySize);
			break;

		default:
			//Should never get here
			XEAssert(false);
			break;
	}

	return XEResult::Ok;
}