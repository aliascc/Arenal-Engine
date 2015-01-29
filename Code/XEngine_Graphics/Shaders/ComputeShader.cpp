
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

XEResult ComputeShader::LoadShaderWithoutLock(const BYTE* shaderByteCode, uint32_t length)
{
	XEAssert(shaderByteCode != nullptr);

	if(shaderByteCode == nullptr)
	{
		return XEResult::NullParameter;
	}
	
	ID3D11ComputeShader* tempDX = nullptr;

	XETODO("Check Dynamic Class Linkage");
	HRESULT hr = m_GraphicDevice->GetDeviceDX()->CreateComputeShader((void*)shaderByteCode, length, nullptr, &tempDX);

	if(hr != S_OK)
	{
		XETODO("Add log message here");

		DisplayError(hr);

		return XEResult::Fail;
	}
	
	//Release DirectX Object
	ReleaseCOM(m_CSDX);
	m_CSDX = tempDX;

	XEGraphicHelpers::SetDebugObjectName<ID3D11ComputeShader>(m_CSDX, XE_DEBUG_CS_NAME_PREFIX + XE_Base::WideStr2String(m_Name));
	
	return XEResult::Ok;
}