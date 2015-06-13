/********************************************************
*
* Author: Carlos Chacón N.
*
* Copyright 2012-2015
*
*********************************************************/

/**********************
*   System Includes   *
***********************/

/*************************
*   3rd Party Includes   *
**************************/
#include "cppformat\format.h"
#include "Localization\LocalizationManager.h"

/***************************
*   Game Engine Includes   *
****************************/
#include "VertexLayout.h"
#include "GraphicDevice.h"
#include "Base\BaseFunctions.h"

//Always include last
#include "Memory\MemLeaks.h"

/********************
*   Function Defs   *
*********************/
VertexLayout::VertexLayout()
	: Named(L"")
{
}

VertexLayout::~VertexLayout()
{
	ReleaseCOM(m_VertexLayoutDX);
}

AEResult VertexLayout::BuildVertexLayout(GraphicDevice* graphicDevice, const BYTE shaderByteCode[], uint32_t shadeByteCodeSize, const D3D11_INPUT_ELEMENT_DESC vertexDesc[], uint32_t elementCount, const std::wstring& name)
{
	if(graphicDevice == nullptr || vertexDesc == nullptr || shaderByteCode == nullptr || elementCount == 0)
	{
		return AEResult::Fail;
	}

	// Create the input layout	
	HRESULT hr = graphicDevice->GetDeviceDX()->CreateInputLayout(vertexDesc, elementCount, (void*)shaderByteCode, shadeByteCodeSize, &m_VertexLayoutDX);

	if(hr != S_OK)
	{
		DisplayError(hr);

		return AEResult::Fail;
	}

	m_Name = name;

	AEGraphicHelpers::SetDebugObjectName<ID3D11InputLayout>(m_VertexLayoutDX, AE_DEBUG_VIL_NAME_PREFIX + AE_Base::WideStr2String(m_Name));
	
	m_IsReady = true;

	return AEResult::Ok;
}
