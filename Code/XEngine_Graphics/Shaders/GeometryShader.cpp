
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
#include "GeometryShader.h"
#include "GraphicDevice.h"
#include "Base\BaseFunctions.h"

//Always include last
#include "Memory\MemLeaks.h"

/********************
*   Function Defs   *
*********************/
GeometryShader::GeometryShader(GraphicDevice* graphicDevice, const std::wstring& name)
	: Shader(graphicDevice, ShaderType::GeometryShader, name)
{
}

GeometryShader::~GeometryShader()
{
	ReleaseCOM(m_GSDX);
}

XEResult GeometryShader::LoadShaderWithoutLock(const BYTE* shaderByteCode, uint32_t length)
{
	XEAssert(shaderByteCode != nullptr);

	if(shaderByteCode == nullptr)
	{
		return XEResult::NullParameter;
	}
	
	ID3D11GeometryShader* tempDX = nullptr;

	XETODO("Check Dynamic Class Linckage");
	HRESULT hr = m_GraphicDevice->GetDeviceDX()->CreateGeometryShader((void*)shaderByteCode, length, nullptr, &tempDX);

	if(hr != S_OK)
	{
		XETODO("Add log message here");

		DisplayError(hr);

		return XEResult::Fail;
	}
	
	//Release DirectX Object
	ReleaseCOM(m_GSDX);
	m_GSDX = tempDX;

	XEGraphicHelpers::SetDebugObjectName<ID3D11GeometryShader>(m_GSDX, XE_DEBUG_GS_NAME_PREFIX + XE_Base::WideStr2String(m_Name));
	
	return XEResult::Ok;
}