
/********************************************************
*
* Author: Carlos Chacón N.
*
* Created: 11/7/2012
*
* Last Major Update: 11/7/2012
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

/***************************
*   Game Engine Includes   *
****************************/
#include "GraphicDevice.h"
#include "ConstantBuffer.h"
#include "Base\BaseFunctions.h"
#include "Shaders\Variables\ShaderCustomVariable.h"

//Always include last
#include "Memory\MemLeaks.h"

/********************
*   Function Defs   *
*********************/
ConstantBuffer::ConstantBuffer(GraphicDevice* graphicDevice, uint32_t bindIndex, const std::wstring& name)
	: Named(name)
	, m_GraphicDevice(graphicDevice)
	, m_BindIndex(bindIndex)
{
	XEAssert(m_GraphicDevice != nullptr);
}

ConstantBuffer::~ConstantBuffer()
{
	ReleaseCOM(m_ConstantBufferDX);

	DeleteMemArr(m_ConstantBuffer);

	uint32_t numVars = (uint32_t)m_VariableVector.size();
	for (uint32_t i = 0; i < numVars; i++)
	{
		DeleteMem(m_VariableVector[i]);
	}
	m_VariableVector.clear();
	m_VariableMap.clear();
}

uint32_t ConstantBuffer::CalculateSize()
{
	uint32_t sizeBuffer = 0;

	uint32_t numVariables = (uint32_t)m_VariableVector.size();
		
	for (uint32_t i = 0; i < numVariables; ++i)
	{
		sizeBuffer += (m_VariableVector[i]->m_Size + m_VariableVector[i]->m_Offset);
	}

	return sizeBuffer;
}

XEResult ConstantBuffer::Initialize()
{
	if(m_IsReady)
	{
		return XEResult::Ok;
	}

	/******************************************
	*1. Initialize RAM Constant Buffer Memory
	/******************************************/
	uint32_t sizeBuffer = CalculateSize();

	if(sizeBuffer == 0)
	{
		return XEResult::ZeroSize;
	}

	//Constant Buffer must be multiple of 16
	uint32_t mod16 = sizeBuffer % 16;

	if(mod16 != 0)
	{
		uint32_t addToSize = 16 - mod16;

		sizeBuffer += addToSize;
	}

	BYTE* constantBufferMemory = new BYTE[sizeBuffer];
	memset(constantBufferMemory, 0, sizeBuffer);
	
	uint32_t numVariables = (uint32_t)m_VariableVector.size();		
	for (uint32_t i = 0; i < numVariables; ++i)
	{
		XEAssert(m_VariableVector[i]->m_StartOffset < sizeBuffer);
		XEAssert((m_VariableVector[i]->m_StartOffset + m_VariableVector[i]->m_Size) <= sizeBuffer);

		uint32_t offset = (m_VariableVector[i]->m_StartOffset + m_VariableVector[i]->m_Size);

		if(offset > sizeBuffer)
		{
			DeleteMemArr(constantBufferMemory);

			return XEResult::OutsideRange;
		}

		m_VariableVector[i]->m_Memory = reinterpret_cast<void*>(constantBufferMemory + m_VariableVector[i]->m_StartOffset);
	}

	/******************************************
	*2. Create VRAM Constant Buffer
	/******************************************/
	ReleaseCOM(m_ConstantBufferDX);

	D3D11_BUFFER_DESC cbDesc = { 0 };

	cbDesc.ByteWidth		= sizeBuffer;
	cbDesc.Usage			= D3D11_USAGE_DYNAMIC;
	cbDesc.BindFlags		= D3D11_BIND_CONSTANT_BUFFER;
	cbDesc.CPUAccessFlags	= D3D11_CPU_ACCESS_WRITE;

	HRESULT hr = m_GraphicDevice->GetDeviceDX()->CreateBuffer(&cbDesc, nullptr, &m_ConstantBufferDX);

	if(hr != S_OK)
	{
		DisplayError(hr);

		DeleteMemArr(constantBufferMemory);

		return XEResult::ConstantBufferInitFailed;
	}

	XEGraphicHelpers::SetDebugObjectName<ID3D11Buffer>(m_ConstantBufferDX, XE_DEBUG_CB_NAME_PREFIX + XE_Base::WideStr2String(m_Name));

	/******************************************
	*3. Finish
	/******************************************/
	m_ConstantBuffer = reinterpret_cast<void*>(constantBufferMemory);

	m_ConstantBufferDataSize = sizeBuffer;

	m_IsReady = true;

	return XEResult::Ok;
}

XEResult ConstantBuffer::Apply()
{
	if(!m_IsReady)
	{
		return XEResult::NotReady;
	}

	if(!m_NeedUpdate)
	{
		return XEResult::Ok;
	}

	D3D11_MAPPED_SUBRESOURCE mappedResource = { 0 };

	HRESULT hr = m_GraphicDevice->GetDeviceContextDX()->Map(m_ConstantBufferDX, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);

	if(hr != S_OK)
	{
		DisplayError(hr);

		return XEResult::ConstantBufferSetDataFailed;
	}

	memcpy(mappedResource.pData, m_ConstantBuffer, m_ConstantBufferDataSize);

	m_GraphicDevice->GetDeviceContextDX()->Unmap(m_ConstantBufferDX, 0);

	m_NeedUpdate = false;

	return XEResult::Ok;
}

const ShaderCustomVariable* ConstantBuffer::GetVariable(uint32_t index) const
{
	if(index >= static_cast<uint32_t>(m_VariableVector.size()))
	{
		return nullptr;
	}

	return m_VariableVector[index];
}

const ShaderCustomVariable* ConstantBuffer::GetVariable(const std::wstring& name) const
{
	if(!Exists(name))
	{
		return nullptr;
	}

	return m_VariableMap.find(name)->second;
}

bool ConstantBuffer::Exists(const std::wstring& name) const
{
	return (m_VariableMap.find(name) != m_VariableMap.end());
}

XEResult ConstantBuffer::AddVariable(ShaderCustomVariable* scv)
{
	if(m_IsReady)
	{
		return XEResult::Fail;
	}

	XEAssert(scv != nullptr);
	if(scv == nullptr)
	{
		return XEResult::NullParameter;
	}

	XEAssert(scv->m_Size != 0);
	if(scv->m_Size == 0)
	{
		return XEResult::ZeroSize;
	}

	XEAssert(!scv->m_Name.empty());
	if(scv->m_Name.empty())
	{
		return XEResult::EmptyName;
	}

	if(Exists(scv->m_Name))
	{
		return XEResult::ObjExists;
	}

	m_VariableMap[scv->m_Name] = scv;

	m_VariableVector.push_back(scv);

	return XEResult::Ok;
}

XEResult ConstantBuffer::SetValue(const std::wstring& varName, const void* value, uint32_t varSize, bool user)
{
	if(!m_IsReady)
	{
		return XEResult::NotReady;
	}

	XEAssert(value != nullptr);
	if(value == nullptr)
	{
		return XEResult::NullParameter;
	}

	XEAssert(varSize != 0);
	if(varSize == 0)
	{
		return XEResult::ZeroSize;
	}

	if(!Exists(varName))
	{
		return XEResult::NotFound;
	}

	ShaderCustomVariable* cv = m_VariableMap[varName];

	if(user && !cv->m_UserVariable)
	{
		return XEResult::NotFound;
	}

	XETODO("return value if requested size is more than variable size");
	uint32_t maxSize = (varSize < cv->m_Size) ? varSize : cv->m_Size;

	if(memcmp(cv->m_Memory, value, maxSize) != 0)
	{
		memcpy(cv->m_Memory, value, maxSize);

		m_NeedUpdate = true;
	}

	return XEResult::Ok;
}

XEResult ConstantBuffer::GetValue(const std::wstring& varName, void* value, uint32_t varSize, bool user)
{
	if(!m_IsReady)
	{
		return XEResult::NotReady;
	}

	XEAssert(value != nullptr);

	if(value == nullptr)
	{
		return XEResult::NullParameter;
	}

	XEAssert(varSize != 0);

	if(varSize == 0)
	{
		return XEResult::ZeroSize;
	}

	if(!Exists(varName))
	{
		return XEResult::NotFound;
	}

	ShaderCustomVariable* cv = m_VariableMap[varName];

	if(user && !cv->m_UserVariable)
	{
		return XEResult::NotFound;
	}

	XETODO("return value if requested size is more than variable size");
	uint32_t maxSize = (varSize < cv->m_Size) ? varSize : cv->m_Size;

	memcpy(value, cv->m_Memory, maxSize);

	return XEResult::Ok;
}

bool ConstantBuffer::ContainsUserVariables()
{
	uint32_t numVars = (uint32_t)m_VariableVector.size();
	for (uint32_t i = 0; i < numVars; i++)
	{
		if(m_VariableVector[i]->m_UserVariable)
		{
			return true;
		}
	}

	return false;
}