
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
#include "HullShader.h"
#include "GraphicDevice.h"
#include "Base\BaseFunctions.h"

//Always include last
#include "Memory\MemLeaks.h"

/********************
*   Function Defs   *
*********************/
HullShader::HullShader(GraphicDevice* graphicDevice, const std::wstring& name)
	: Shader(graphicDevice, ShaderType::HullShader, name)
{
}

HullShader::~HullShader()
{
	ReleaseCOM(m_HSDX);
}

XEResult HullShader::LoadShaderWithoutLock(const BYTE* shaderByteCode, uint32_t length)
{
	XEAssert(shaderByteCode != nullptr);

	if(shaderByteCode == nullptr)
	{
		return XEResult::NullParameter;
	}
	
	ID3D11HullShader* tempDX = nullptr;

	XETODO("Check Dynamic Class Linckage");
	HRESULT hr = m_GraphicDevice->GetDeviceDX()->CreateHullShader((void*)shaderByteCode, length, nullptr, &tempDX);

	if(hr != S_OK)
	{
		XETODO("Add log message here");

		DisplayError(hr);

		return XEResult::Fail;
	}
	
	//Release DirectX Object
	ReleaseCOM(m_HSDX);
	m_HSDX = tempDX;

	XEGraphicHelpers::SetDebugObjectName<ID3D11HullShader>(m_HSDX, XE_DEBUG_HS_NAME_PREFIX + XE_Base::WideStr2String(m_Name));
	
	return XEResult::Ok;
}