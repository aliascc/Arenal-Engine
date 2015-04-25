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

/***************************
*   Game Engine Includes   *
****************************/
#include "ShaderDefs.h"
#include "DomainShader.h"
#include "GraphicDevice.h"
#include "Base\BaseFunctions.h"

//Always include last
#include "Memory\MemLeaks.h"

/********************
*   Function Defs   *
*********************/
DomainShader::DomainShader(GraphicDevice* graphicDevice, const std::wstring& name)
	: Shader(graphicDevice, ShaderType::DomainShader, name)
{
}

DomainShader::~DomainShader()
{
	ReleaseCOM(m_DSDX);
}

XEResult DomainShader::LoadShaderWithoutLock(const BYTE* shaderByteCode, uint32_t length)
{
	XEAssert(shaderByteCode != nullptr);

	if(shaderByteCode == nullptr)
	{
		return XEResult::NullParameter;
	}
	
	ID3D11DomainShader* tempDX = nullptr;

	XETODO("Check Dynamic Class Linckage");
	HRESULT hr = m_GraphicDevice->GetDeviceDX()->CreateDomainShader((void*)shaderByteCode, length, nullptr, &tempDX);

	if(hr != S_OK)
	{
		XETODO("Add log message here");

		DisplayError(hr);

		return XEResult::Fail;
	}
	
	//Release DirectX Object
	ReleaseCOM(m_DSDX);
	m_DSDX = tempDX;

	XEGraphicHelpers::SetDebugObjectName<ID3D11DomainShader>(m_DSDX, XE_DEBUG_DS_NAME_PREFIX + XE_Base::WideStr2String(m_Name));
	
	return XEResult::Ok;
}