
/********************************************************
*
* Author: Carlos Chacón N.
*
* Created: 6/7/2013
* 
* Desc:
*
*
* Copyright 2013
*
*********************************************************/


/**********************
*   System Includes   *
***********************/

/*************************
*   3rd Party Includes   *
**************************/

/***************************
*   Game Engine Includes   *
****************************/
#include "SimpleBuffer.h"
#include "Logger\Logger.h"
#include "GraphicDevice.h"
#include "Base\BaseFunctions.h"
#include "Localization\LocalizationManager.h"

//Always include last
#include "Memory\MemLeaks.h"

/********************
*   Function Defs   *
*********************/
SimpleBuffer::SimpleBuffer(const std::wstring& name, uint32_t bindIndex, bool createAsRW, ShaderVariableClass variableClass,  ShaderVariableType variableType, uint32_t elementCount, GraphicDevice* graphicDevice)
	: ShaderBuffer(name, bindIndex, createAsRW, graphicDevice)
	, m_VariableClass(variableClass)
	, m_VariableType(variableType)
	, m_ElementCount(elementCount)
{
	XEResult ret = XEResult::Ok;

	ret = CheckShaderVariableInfo(variableClass, variableType, elementCount);
	if(ret != XEResult::Ok)
	{
		XEAssert(false);
		XETODO("Log Error");
	}
	else
	{
		m_IsStartReady = true;
	}
}

SimpleBuffer::~SimpleBuffer()
{
}

XEResult SimpleBuffer::CheckShaderVariableInfo(ShaderVariableClass variableClass,  ShaderVariableType variableType, uint32_t elementCount)
{
	XETODO("Add check if double has more than 2 elements we need to cancel");
	XEAssert(elementCount > 0 && elementCount < 5);
	if(elementCount <= 0 || elementCount >= 5)
	{
		return XEResult::OutsideRange;
	}

	switch (variableClass)
	{
		case ShaderVariableClass::Scalar:
		case ShaderVariableClass::Vector:
			//We accept these Variable Classes
			break;
		default:
			XEAssert(false);
			return XEResult::InvalidObjType;
	}

	switch (variableType)
	{
		case ShaderVariableType::Int:
		case ShaderVariableType::UInt:
		case ShaderVariableType::Float:
		case ShaderVariableType::Double:
			//We accept these Variable Types
			break;
		default:
			XEAssert(false);
			return XEResult::InvalidObjType;
	}

	if(elementCount > 1 && variableClass != ShaderVariableClass::Vector)
	{
		XETODO("Better return");
		return XEResult::InvalidObjType;
	}

	return XEResult::Ok;
}

DXGI_FORMAT SimpleBuffer::GetDXGIFormat()
{
	XEAssert(m_IsStartReady)
	if(!m_IsStartReady)
	{
		XETODO("Log error");
		return DXGI_FORMAT_UNKNOWN;
	}

	bool err = false;

	switch (m_VariableType)
	{
		case ShaderVariableType::Int:
			{
				switch (m_ElementCount)
				{
					case 1:
						return DXGI_FORMAT_R32_SINT;

					case 2:
						return DXGI_FORMAT_R32G32_SINT;

					case 3:
						return DXGI_FORMAT_R32G32B32_SINT;

					case 4:
						return DXGI_FORMAT_R32G32B32A32_SINT;

					default:
						break;
				}
			}
			break;

		case ShaderVariableType::UInt:
			{
				switch (m_ElementCount)
				{
					case 1:
						return DXGI_FORMAT_R32_UINT;

					case 2:
						return DXGI_FORMAT_R32G32_UINT;

					case 3:
						return DXGI_FORMAT_R32G32B32_UINT;

					case 4:
						return DXGI_FORMAT_R32G32B32A32_UINT;

					default:
						break;
				}
			}
			break;

		case ShaderVariableType::Float:
			{
				switch (m_ElementCount)
				{
					case 1:
						return DXGI_FORMAT_R32_FLOAT;

					case 2:
						return DXGI_FORMAT_R32G32_FLOAT;

					case 3:
						return DXGI_FORMAT_R32G32B32_FLOAT;

					case 4:
						return DXGI_FORMAT_R32G32B32A32_FLOAT;

					default:
						break;
				}
			}
			break;

		case ShaderVariableType::Double:
			{
				switch (m_ElementCount)
				{
					case 1:
						return DXGI_FORMAT_R32G32_FLOAT;

					case 2:
						return DXGI_FORMAT_R32G32B32A32_FLOAT;

					default:
						break;
				}
			}
			break;

		default:
			break;
	}

	XETODO("Log error");
	return DXGI_FORMAT_UNKNOWN;
}

XEResult SimpleBuffer::Deinitialize()
{
	m_IsReady		= false;
	m_NumElements	= 0;
	m_BufferUsage	= GraphicBufferUsage::Default;
	m_BufferAccess	= GraphicBufferAccess::None;

	CleanUp();

	return XEResult::Ok;
}

XEResult SimpleBuffer::InitializeBuffer(uint32_t numElements, GraphicBufferUsage bufferUsage, GraphicBufferAccess bufferAccess)
{
	XEAssert(m_IsStartReady)
	if(!m_IsStartReady)
	{
		XETODO("better return error");
		return XEResult::Fail;
	}

	XEAssert(m_GraphicDevice != nullptr);
	if(m_GraphicDevice == nullptr)
	{
		return XEResult::GraphicDeviceNull;
	}

	XEAssert(numElements != 0);
	if(numElements == 0)
	{
		return XEResult::ZeroSize;
	}

	/******************************************
	*1. Verify if buffer has been initialize 
	*   and remove it is
	/******************************************/
	Deinitialize();

	/******************************************
	*2. Create VRAM Buffer
	/******************************************/
	D3D11_BUFFER_DESC cbDesc = { 0 };

	DXGI_FORMAT dxFormat = GetDXGIFormat();

	UINT bindFlagsDX = D3D11_BIND_SHADER_RESOURCE;

	if(m_IsRWEnabled)
	{
		bindFlagsDX |= D3D11_BIND_UNORDERED_ACCESS;
	}

	cbDesc.ByteWidth			= (ELEMENT_SIZE_SIMPLE_BUFFER * m_ElementCount) * numElements;
	cbDesc.Usage				= XEGraphicHelpers::GetDXUsage(bufferUsage);
	cbDesc.BindFlags			= bindFlagsDX;
	cbDesc.CPUAccessFlags		= XEGraphicHelpers::GetDXBufferAccess(bufferAccess);
	cbDesc.StructureByteStride	= (ELEMENT_SIZE_SIMPLE_BUFFER * m_ElementCount);
	cbDesc.MiscFlags			= 0;

	HRESULT hr = m_GraphicDevice->GetDeviceDX()->CreateBuffer(&cbDesc, nullptr, &m_BufferDX);

	if(hr != S_OK)
	{
		DisplayError(hr);

		return XEResult::SimpleBufferInitFailed;
	}

	XEGraphicHelpers::SetDebugObjectName<ID3D11Buffer>(m_BufferDX, XE_DEBUG_B_NAME_PREFIX + XE_Base::WideStr2String(m_Name));

	/******************************************
	*3. Create SRV for Buffer
	/******************************************/
	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	memset(&srvDesc, 0, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));

	srvDesc.Format					= dxFormat;
	srvDesc.ViewDimension			= D3D11_SRV_DIMENSION_BUFFER;

	srvDesc.Buffer.FirstElement		= 0;
	srvDesc.Buffer.NumElements		= numElements;

	hr = m_GraphicDevice->GetDeviceDX()->CreateShaderResourceView(m_BufferDX, &srvDesc, &m_BufferSRVDX);

	if(hr != S_OK)
	{
		DisplayError(hr);

		ReleaseCOM(m_BufferDX);

		return XEResult::SimpleBufferInitFailed;
	}

	XEGraphicHelpers::SetDebugObjectName<ID3D11ShaderResourceView>(m_BufferSRVDX, XE_DEBUG_B_SRV_NAME_PREFIX + XE_Base::WideStr2String(m_Name));

	/******************************************
	*4. Create UAV for Buffer if flag is set
	/******************************************/
	if(m_IsRWEnabled)
	{
		D3D11_UNORDERED_ACCESS_VIEW_DESC uavDesc;
		memset(&uavDesc, 0, sizeof(D3D11_UNORDERED_ACCESS_VIEW_DESC));

		uavDesc.Format					= dxFormat;
		uavDesc.ViewDimension			= D3D11_UAV_DIMENSION_BUFFER;

		uavDesc.Buffer.FirstElement		= 0;
		uavDesc.Buffer.NumElements		= numElements;
		uavDesc.Buffer.Flags			= 0;

		hr = m_GraphicDevice->GetDeviceDX()->CreateUnorderedAccessView(m_BufferDX, &uavDesc, &m_BufferUAVDX);

		if(hr != S_OK)
		{
			DisplayError(hr);

			ReleaseCOM(m_BufferSRVDX);
			ReleaseCOM(m_BufferDX);

			return XEResult::SimpleBufferInitFailed;
		}

		XEGraphicHelpers::SetDebugObjectName<ID3D11ShaderResourceView>(m_BufferSRVDX, XE_DEBUG_B_UAV_NAME_PREFIX + XE_Base::WideStr2String(m_Name));
	}

	/******************************************
	*5. Finish
	/******************************************/
	m_IsReady		= true;
	m_NumElements	= numElements;
	m_BufferUsage	= bufferUsage;
	m_BufferAccess	= bufferAccess;

	return XEResult::Ok;
}

XEResult SimpleBuffer::UpdateBuffer(void* data, uint32_t numElements, uint32_t startIndex, uint32_t offset, GraphicResourceMap resourceMap)
{
	if(!m_IsReady)
	{
		return XEResult::NotReady;
	}

	XEAssert(data != nullptr);
	if(data == nullptr)
	{
		return XEResult::NullParameter;
	}

	XEAssert(resourceMap == GraphicResourceMap::Write || resourceMap == GraphicResourceMap::WriteDiscard);
	if(resourceMap != GraphicResourceMap::Write && resourceMap != GraphicResourceMap::WriteDiscard)
	{
		return XEResult::InvalidResourceMapType;
	}

	BYTE* mappedMemory = nullptr;
	XEResult ret = XEResult::Ok;

	/******************************************
	*1. Verify Constraints
	/******************************************/
	if(startIndex >= m_NumElements || (startIndex + numElements) > m_NumElements)
	{
		return XEResult::OutsideRange;
	}

	/******************************************
	*2. Map Resource to RAM
	/******************************************/
	D3D11_MAPPED_SUBRESOURCE mappedData = { 0 };

	D3D11_MAP dxMapType = XEGraphicHelpers::GetDXResourceMap(resourceMap);

	HRESULT hr = m_GraphicDevice->GetDeviceContextDX()->Map(m_BufferDX, 0, dxMapType, 0, &mappedData);

	if(hr != S_OK)
	{
		DisplayError(hr);

		return XEResult::ResourceMapFailed;
	}

	mappedMemory = reinterpret_cast<BYTE*>(mappedData.pData);

	/******************************************
	*3. Copy to VRAM
	/******************************************/
	uint32_t copySize = (ELEMENT_SIZE_SIMPLE_BUFFER * m_ElementCount) * numElements;
	uint32_t offsetSize =  (ELEMENT_SIZE_SIMPLE_BUFFER * m_ElementCount) * offset;

	memcpy(mappedMemory + offsetSize, data, copySize);

	/******************************************
	*4. Unmap Resource
	/******************************************/
	m_GraphicDevice->GetDeviceContextDX()->Unmap(m_BufferDX, 0);

	return XEResult::Ok;
}