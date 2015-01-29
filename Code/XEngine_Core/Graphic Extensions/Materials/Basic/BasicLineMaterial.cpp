
/********************************************************
*
* Author: Carlos Chacón N.
*
* Created: 11/19/2012
*
* Last Major Update: 11/19/2012
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
#include "GraphicDevice.h"
#include "BasicLineMaterial.h"
#include "Base\BaseFunctions.h"
#include "Shaders\PixelShader.h"
#include "Shaders\VertexShader.h"
#include "Resource\GameResourceManager.h"
#include "Shaders\Buffers\ConstantBuffer.h"
#include "Shaders\Variables\ShaderCustomVariable.h"
#include "Graphic Extensions\Shader Extensions\Properties\ShaderProperties.h"

#if defined(DEBUG) | defined(_DEBUG)
	#if defined(_WIN64) | defined (WIN64) 
#include "Compiled Materials\HLSL\BasicLineMaterialPS_x64_d.h"
#include "Compiled Materials\HLSL\BasicLineMaterialVS_x64_d.h"
	#else
#include "Compiled Materials\HLSL\BasicLineMaterialPS_x86_d.h"
#include "Compiled Materials\HLSL\BasicLineMaterialVS_x86_d.h"
	#endif
#else
	#if defined(_WIN64) | defined (WIN64) 
#include "Compiled Materials\HLSL\BasicLineMaterialPS_x64.h"
#include "Compiled Materials\HLSL\BasicLineMaterialVS_x64.h"
	#else
#include "Compiled Materials\HLSL\BasicLineMaterialPS_x86.h"
#include "Compiled Materials\HLSL\BasicLineMaterialVS_x86.h"
	#endif
#endif

//Always include last
#include "Memory\MemLeaks.h"

/********************
*   Function Defs   *
*********************/
BasicLineMaterial::BasicLineMaterial(GraphicDevice* graphicDevice, GameResourceManager* gameResourceManager, const std::wstring& name)
	: Material(graphicDevice, gameResourceManager, name)
{
}

BasicLineMaterial::~BasicLineMaterial()
{
}

XEResult BasicLineMaterial::CreateVertexShader()
{
	XEResult ret = XEResult::Ok;

	/////////////////////////////////////////////////////
	//Get Vertex Shader from Game Resources
	m_VertexShader = (VertexShader*)m_GameResourceManager->AcquireGameResourceByStringID(XE_BASIC_LINE_MAT_VS_NAME, GameResourceType::VertexShader);

	if(m_VertexShader == nullptr)
	{
		m_VertexShader = new VertexShader(m_GraphicDevice, XE_BASIC_LINE_MAT_VS_NAME);
		ret = m_VertexShader->LoadShader(BasicLineMaterialVS, sizeof(BasicLineMaterialVS));
		if(ret != XEResult::Ok)
		{
			return XEResult::VertexShaderLoadFailed;
		}

		ret = m_GameResourceManager->ManageGameResource(m_VertexShader, XE_BASIC_LINE_MAT_VS_NAME);
		if(ret != XEResult::Ok)
		{
			return XEResult::ResourceManagedFailed;
		}
	}

	/////////////////////
	//Create Properties
	m_VSProps = new ShaderProperties(ShaderType::VertexShader, m_GraphicDevice);

	ret = CreateVertexShaderConstantBuffer();

	if(ret != XEResult::Ok)
	{
		return XEResult::ConstantBufferInitFailed;
	}

	return XEResult::Ok;
}

XEResult BasicLineMaterial::CreateVertexShaderConstantBuffer()
{
	XEResult ret = XEResult::Ok;
	
	/////////////////////////////////////////////////////
	//Create Constant Buffer 
	//	- _XE_CB_World_View_Proj
	//
	
	/****************************************************************************
	*Constant Buffer #1: _XE_CB_World_View_Proj
	****************************************************************************/
	ConstantBuffer* cbWVP = nullptr;

	ret = XEBuiltInMaterialsHelpers::BuildCBWVP(m_GraphicDevice, &cbWVP);

	if(ret != XEResult::Ok)
	{
		return XEResult::ConstantBufferInitFailed;
	}

	ret = m_VSProps->AddConstantBuffer(cbWVP);

	if(ret != XEResult::Ok)
	{
		DeleteMem(cbWVP);

		return XEResult::ConstantBufferInitFailed;
	}
	
	/****************************************************************************
	*Finish
	****************************************************************************/
	return XEResult::Ok;
}
		
XEResult BasicLineMaterial::CreatePixelShader()
{
	XEResult ret = XEResult::Ok;
	
	/////////////////////////////////////////////////////
	//Get Pixel Shader from Game Resources
	m_PixelShader = (PixelShader*)m_GameResourceManager->AcquireGameResourceByStringID(XE_BASIC_LINE_MAT_PS_NAME, GameResourceType::PixelShader);
	
	if(m_PixelShader == nullptr)
	{
		m_PixelShader = new PixelShader(m_GraphicDevice, XE_BASIC_LINE_MAT_PS_NAME);
		ret = m_PixelShader->LoadShader(BasicLineMaterialPS, sizeof(BasicLineMaterialPS));
		if(ret != XEResult::Ok)
		{
			return XEResult::PixelShaderLoadFailed;
		}

		ret = m_GameResourceManager->ManageGameResource(m_PixelShader, XE_BASIC_LINE_MAT_PS_NAME);
		if(ret != XEResult::Ok)
		{
			return XEResult::ResourceManagedFailed;
		}
	}

	/////////////////////
	//Create Properties
	m_PSProps = new ShaderProperties(ShaderType::PixelShader, m_GraphicDevice);

	ret = CreatePixelShaderConstantBuffer();

	if(ret != XEResult::Ok)
	{
		return XEResult::ConstantBufferInitFailed;
	}

	return XEResult::Ok;
}
					   
XEResult BasicLineMaterial::CreatePixelShaderConstantBuffer()
{
	XEResult ret = XEResult::Ok;
	
	/////////////////////////////////////////////////////
	//Create Constant Buffer 
	//	- _XE_CB_Color
	//
	
	/****************************************************************************
	*Constant Buffer #1: _XE_CB_Color
	****************************************************************************/
	ConstantBuffer* cb = nullptr;

	ret = XEBuiltInMaterialsHelpers::BuildCBColor(m_GraphicDevice, &cb);

	if(ret != XEResult::Ok)
	{
		return XEResult::ConstantBufferInitFailed;
	}

	ret = m_PSProps->AddConstantBuffer(cb);

	if(ret != XEResult::Ok)
	{
		DeleteMem(cb);

		return XEResult::ConstantBufferInitFailed;
	}
	
	/****************************************************************************
	*Finish
	****************************************************************************/
	return XEResult::Ok;
}

XEResult BasicLineMaterial::LoadContent()
{
	XEAssert(m_GraphicDevice != nullptr);

	if(m_GraphicDevice == nullptr)
	{
		return XEResult::GraphicDeviceNull;
	}

	XEAssert(m_GameResourceManager != nullptr);

	if(m_GameResourceManager == nullptr)
	{
		return XEResult::GameResourceManagerNull;
	}

	if(m_IsReady)
	{
		return XEResult::Ok;
	}

	////////////////////////////////////
	//Clean up memory
	CleanUp();

	XEResult ret = XEResult::Ok;

	/***************************
	*Vertex Shader
	***************************/
	ret = CreateVertexShader();

	if(ret != XEResult::Ok)
	{
		CleanUp();

		return ret;
	}

	/***************************
	*Pixel Shader
	***************************/
	ret = CreatePixelShader();

	if(ret != XEResult::Ok)
	{
		CleanUp();

		return ret;
	}
	
	/***************************
	*Finish
	***************************/
	m_IsReady = true;

	return XEResult::Ok;
}

