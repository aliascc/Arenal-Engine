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

AEResult GeometryShader::LoadShaderWithoutLock(const BYTE* shaderByteCode, uint32_t length)
{
	AEAssert(shaderByteCode != nullptr);

	if(shaderByteCode == nullptr)
	{
		return AEResult::NullParameter;
	}
	
	ID3D11GeometryShader* tempDX = nullptr;

	AETODO("Check Dynamic Class Linckage");
	HRESULT hr = m_GraphicDevice->GetDeviceDX()->CreateGeometryShader((void*)shaderByteCode, length, nullptr, &tempDX);

	if(hr != S_OK)
	{
		AETODO("Add log message here");

		DisplayError(hr);

		return AEResult::Fail;
	}
	
	//Release DirectX Object
	ReleaseCOM(m_GSDX);
	m_GSDX = tempDX;

	AEGraphicHelpers::SetDebugObjectName<ID3D11GeometryShader>(m_GSDX, AE_DEBUG_GS_NAME_PREFIX + AE_Base::WideStr2String(m_Name));
	
	return AEResult::Ok;
}