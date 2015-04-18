
/********************************************************
*
* Author: Carlos Chacón N.
* 
* Desc:
*
*
* Copyright 2012-2014
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

XEResult VertexShader::LoadShaderWithoutLock(const BYTE* shaderByteCode, uint32_t length)
{
	XEAssert(shaderByteCode != nullptr);

	if(shaderByteCode == nullptr)
	{
		return XEResult::NullParameter;
	}

	ID3D11VertexShader* tempDX = nullptr;

	XETODO("Check Dynamic Class Linkage");
	HRESULT hr = m_GraphicDevice->GetDeviceDX()->CreateVertexShader((void*)shaderByteCode, length, nullptr, &tempDX);
	if(hr != S_OK)
	{
		XETODO("Add log message here");

		DisplayError(hr);

		return XEResult::Fail;
	}
	
	//Release DirectX Object
	ReleaseCOM(m_VSDX);
	m_VSDX = tempDX;

	XEGraphicHelpers::SetDebugObjectName<ID3D11VertexShader>(m_VSDX, XE_DEBUG_VS_NAME_PREFIX + XE_Base::WideStr2String(m_Name));
	
	return XEResult::Ok;
}