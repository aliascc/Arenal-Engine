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
#include "PixelShader.h"
#include "GraphicDevice.h"
#include "Base\BaseFunctions.h"

//Always include last
#include "Memory\MemLeaks.h"

/********************
*   Function Defs   *
*********************/
PixelShader::PixelShader(GraphicDevice* graphicDevice, const std::wstring& name)
	: Shader(graphicDevice, ShaderType::PixelShader, name)
{
}

PixelShader::~PixelShader()
{
	ReleaseCOM(m_PSDX);
}

AEResult PixelShader::LoadShaderWithoutLock(const BYTE* shaderByteCode, uint32_t length)
{
	AEAssert(shaderByteCode != nullptr);

	if(shaderByteCode == nullptr)
	{
		return AEResult::NullParameter;
	}
	
	ID3D11PixelShader* tempDX = nullptr;

	AETODO("Check Dynamic Class Linckage");
	HRESULT hr = m_GraphicDevice->GetDeviceDX()->CreatePixelShader((void*)shaderByteCode, length, nullptr, &tempDX);

	if(hr != S_OK)
	{
		AETODO("Add log message here");

		DisplayError(hr);

		return AEResult::Fail;
	}
	
	//Release DirectX Object
	ReleaseCOM(m_PSDX);
	m_PSDX = tempDX;

	AEGraphicHelpers::SetDebugObjectName<ID3D11PixelShader>(m_PSDX, AE_DEBUG_PS_NAME_PREFIX + AE_Base::WideStr2String(m_Name));

	return AEResult::Ok;
}
