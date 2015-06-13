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
#include "VertexShader.h"
#include "GraphicDevice.h"
#include "Base\BaseFunctions.h"

//Always include last
#include "Memory\MemLeaks.h"

/********************
*   Function Defs   *
*********************/
VertexShader::VertexShader(GraphicDevice* graphicDevice, const std::wstring& name)
	: Shader(graphicDevice, ShaderType::VertexShader, name)
{
}

VertexShader::~VertexShader()
{
	ReleaseCOM(m_VSDX);
}

AEResult VertexShader::LoadShaderWithoutLock(const BYTE* shaderByteCode, uint32_t length)
{
	AEAssert(shaderByteCode != nullptr);

	if(shaderByteCode == nullptr)
	{
		return AEResult::NullParameter;
	}

	ID3D11VertexShader* tempDX = nullptr;

	AETODO("Check Dynamic Class Linkage");
	HRESULT hr = m_GraphicDevice->GetDeviceDX()->CreateVertexShader((void*)shaderByteCode, length, nullptr, &tempDX);
	if(hr != S_OK)
	{
		AETODO("Add log message here");

		DisplayError(hr);

		return AEResult::Fail;
	}
	
	//Release DirectX Object
	ReleaseCOM(m_VSDX);
	m_VSDX = tempDX;

	AEGraphicHelpers::SetDebugObjectName<ID3D11VertexShader>(m_VSDX, AE_DEBUG_VS_NAME_PREFIX + AE_Base::WideStr2String(m_Name));
	
	return AEResult::Ok;
}