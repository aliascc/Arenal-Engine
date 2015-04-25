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
#include "GraphicDevice.h"
#include "SkinningMaterial.h"
#include "Math\XEMathDefs.h"
#include "Base\BaseFunctions.h"
#include "Textures\Texture2D.h"
#include "Shaders\PixelShader.h"
#include "Shaders\VertexShader.h"
#include "Models\Skinning\SkinningDefs.h"
#include "Resource\GameResourceManager.h"
#include "Shaders\Buffers\ConstantBuffer.h"
#include "Shaders\Bindings\ShaderTextureBinding.h"
#include "Shaders\Variables\ShaderCustomVariable.h"
#include "Graphic Extensions\Shader Extensions\Properties\ShaderProperties.h"

#if defined(DEBUG) | defined(_DEBUG)
	#if defined(_WIN64) | defined (WIN64) 
#include "Compiled Materials\HLSL\SkinningMaterialVS_x64_d.h"
#include "Compiled Materials\HLSL\SkinningMaterialPS_x64_d.h"
	#else
#include "Compiled Materials\HLSL\SkinningMaterialVS_x86_d.h"
#include "Compiled Materials\HLSL\SkinningMaterialPS_x86_d.h"
	#endif
#else
	#if defined(_WIN64) | defined (WIN64) 
#include "Compiled Materials\HLSL\SkinningMaterialVS_x64.h"
#include "Compiled Materials\HLSL\SkinningMaterialPS_x64.h"
	#else
#include "Compiled Materials\HLSL\SkinningMaterialVS_x86.h"
#include "Compiled Materials\HLSL\SkinningMaterialPS_x86.h"
	#endif
#endif

//Always include last
#include "Memory\MemLeaks.h"

/********************
*   Function Defs   *
*********************/
SkinningMaterial::SkinningMaterial(GraphicDevice* graphicDevice, GameResourceManager* gameResourceManager, const std::wstring& name)
	: Material(graphicDevice, gameResourceManager, name)
{
}

SkinningMaterial::~SkinningMaterial()
{
}

XEResult SkinningMaterial::CreateVertexShader()
{
	XEResult ret = XEResult::Ok;

	/////////////////////////////////////////////////////
	//Get Vertex Shader from Game Resources
	m_VertexShader = (VertexShader*)m_GameResourceManager->AcquireGameResourceByStringID(XE_SKINNING_MAT_VS_NAME, GameResourceType::VertexShader);

	if(m_VertexShader == nullptr)
	{
		m_VertexShader = new VertexShader(m_GraphicDevice, XE_SKINNING_MAT_VS_NAME);
		ret = m_VertexShader->LoadShader(SkinningMaterialVS, sizeof(SkinningMaterialVS));
		if(ret != XEResult::Ok)
		{
			return XEResult::VertexShaderLoadFailed;
		}

		ret = m_GameResourceManager->ManageGameResource(m_VertexShader, XE_SKINNING_MAT_VS_NAME);
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

XEResult SkinningMaterial::CreateVertexShaderConstantBuffer()
{
	XEResult ret = XEResult::Ok;
	
	/////////////////////////////////////////////////////
	//Create Constant Buffer 
	//	- _XE_CB_World_View_Proj
	//	- _XE_CB_Bones
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
	*Constant Buffer #2: _XE_CB_Bones
	****************************************************************************/
	ConstantBuffer* cbBones = nullptr;

	ret = XEBuiltInMaterialsHelpers::BuildCBBones(m_GraphicDevice, &cbBones);

	ret = m_VSProps->AddConstantBuffer(cbBones);

	if(ret != XEResult::Ok)
	{
		DeleteMem(cbBones);

		return XEResult::ConstantBufferInitFailed;
	}
	
	/****************************************************************************
	*Finish
	****************************************************************************/
	return XEResult::Ok;
}

XEResult SkinningMaterial::CreatePixelShader()
{
	XEResult ret = XEResult::Ok;
	
	/////////////////////////////////////////////////////
	//Get Pixel Shader from Game Resources
	m_PixelShader = (PixelShader*)m_GameResourceManager->AcquireGameResourceByStringID(XE_SKINNING_MAT_PS_NAME, GameResourceType::PixelShader);
	
	if(m_PixelShader == nullptr)
	{
		m_PixelShader = new PixelShader(m_GraphicDevice, XE_SKINNING_MAT_PS_NAME);
		ret = m_PixelShader->LoadShader(SkinningMaterialPS, sizeof(SkinningMaterialPS));
		if(ret != XEResult::Ok)
		{
			return XEResult::PixelShaderLoadFailed;
		}

		ret = m_GameResourceManager->ManageGameResource(m_PixelShader, XE_SKINNING_MAT_PS_NAME);
		if(ret != XEResult::Ok)
		{
			return XEResult::ResourceManagedFailed;
		}
	}

	/////////////////////
	//Create Properties
	m_PSProps = new ShaderProperties(ShaderType::PixelShader, m_GraphicDevice);

	ret = CreatePixelShaderTextureBinding();

	if(ret != XEResult::Ok)
	{
		return XEResult::ShaderTextureBindingInitFailed;
	}

	return XEResult::Ok;
}
					   
XEResult SkinningMaterial::CreatePixelShaderTextureBinding()
{
	XEResult ret = XEResult::Ok;
	
	/////////////////////////////////////////////////////
	//Create Texture Binding
	//as in Shader:
	// Texture2D _XE_DiffuseTexture : register(t0);
	//

	ShaderTextureBinding* stb = new ShaderTextureBinding(L"_XE_DiffuseTexture", 0, TextureType::Texture2D, nullptr);

	ret = m_PSProps->AddShaderTextureBinding(stb);

	if(ret != XEResult::Ok)
	{
		DeleteMem(stb);

		return XEResult::ShaderTextureBindingInitFailed;
	}

	return XEResult::Ok;
}

XEResult SkinningMaterial::LoadContent()
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
