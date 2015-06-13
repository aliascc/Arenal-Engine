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
#include "ComputeShader.h"
#include "GraphicDevice.h"
#include "Base\BaseFunctions.h"

//Always include last
#include "Memory\MemLeaks.h"

/********************
*   Function Defs   *
*********************/
ComputeShader::ComputeShader(GraphicDevice* graphicDevice, const std::wstring& name)
	: Shader(graphicDevice, ShaderType::ComputeShader, name)
{
}

ComputeShader::~ComputeShader()
{
	ReleaseCOM(m_CSDX);
}

AEResult ComputeShader::LoadShaderWithoutLock(const BYTE* shaderByteCode, uint32_t length)
{
	AEAssert(shaderByteCode != nullptr);

	if(shaderByteCode == nullptr)
	{
		return AEResult::NullParameter;
	}
	
	ID3D11ComputeShader* tempDX = nullptr;

	AETODO("Check Dynamic Class Linkage");
	HRESULT hr = m_GraphicDevice->GetDeviceDX()->CreateComputeShader((void*)shaderByteCode, length, nullptr, &tempDX);

	if(hr != S_OK)
	{
		AETODO("Add log message here");

		DisplayError(hr);

		return AEResult::Fail;
	}
	
	//Release DirectX Object
	ReleaseCOM(m_CSDX);
	m_CSDX = tempDX;

	AEGraphicHelpers::SetDebugObjectName<ID3D11ComputeShader>(m_CSDX, AE_DEBUG_CS_NAME_PREFIX + AE_Base::WideStr2String(m_Name));
	
	return AEResult::Ok;
}