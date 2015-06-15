/********************************************************
*
* Author: Carlos Chac�n N.
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
#include "Math\AEMathDefs.h"
#include "Base\BaseFunctions.h"
#include "Textures\Texture2D.h"
#include "Shaders\PixelShader.h"
#include "Shaders\VertexShader.h"
#include "DiffuseTextureMaterial.h"
#include "Resource\GameResourceManager.h"
#include "Shaders\Buffers\ConstantBuffer.h"
#include "Shaders\Bindings\ShaderTextureBinding.h"
#include "Shaders\Variables\ShaderCustomVariable.h"
#include "Graphic Extensions\Shader Extensions\Properties\ShaderProperties.h"

#if defined(DEBUG) | defined(_DEBUG)
	#if defined(_WIN64) | defined (WIN64) 
#include "Compiled Materials\HLSL\DiffuseTextureMaterialVS_x64_d.h"
#include "Compiled Materials\HLSL\DiffuseTextureMaterialPS_x64_d.h"
	#else
#include "Compiled Materials\HLSL\DiffuseTextureMaterialVS_x86_d.h"
#include "Compiled Materials\HLSL\DiffuseTextureMaterialPS_x86_d.h"
	#endif
#else
	#if defined(_WIN64) | defined (WIN64) 
#include "Compiled Materials\HLSL\DiffuseTextureMaterialVS_x64.h"
#include "Compiled Materials\HLSL\DiffuseTextureMaterialPS_x64.h"
	#else
#include "Compiled Materials\HLSL\DiffuseTextureMaterialVS_x86.h"
#include "Compiled Materials\HLSL\DiffuseTextureMaterialPS_x86.h"
	#endif
#endif

//Always include last
#include "Memory\MemLeaks.h"

/********************
*   Function Defs   *
*********************/
DiffuseTextureMaterial::DiffuseTextureMaterial(GraphicDevice* graphicDevice, GameResourceManager* gameResourceManager, const std::wstring& name)
	: Material(graphicDevice, gameResourceManager, name)
{
}

DiffuseTextureMaterial::~DiffuseTextureMaterial()
{
}

AEResult DiffuseTextureMaterial::CreateVertexShader()
{
	AEResult ret = AEResult::Ok;

	/////////////////////////////////////////////////////
	//Get Vertex Shader from Game Resources
	m_VertexShader = (VertexShader*)m_GameResourceManager->AcquireGameResourceByStringID(AE_DIFFUSE_TEXTURE_MAT_VS_NAME, GameResourceType::VertexShader);

	if(m_VertexShader == nullptr)
	{
		m_VertexShader = new VertexShader(m_GraphicDevice, AE_DIFFUSE_TEXTURE_MAT_VS_NAME);
		ret = m_VertexShader->LoadShader(DiffuseTextureMaterialVS, sizeof(DiffuseTextureMaterialVS));
		if(ret != AEResult::Ok)
		{
			return AEResult::VertexShaderLoadFailed;
		}

		ret = m_GameResourceManager->ManageGameResource(m_VertexShader, AE_DIFFUSE_TEXTURE_MAT_VS_NAME);
		if(ret != AEResult::Ok)
		{
			return AEResult::ResourceManagedFailed;
		}
	}

	/////////////////////
	//Create Properties
	m_VSProps = new ShaderProperties(ShaderType::VertexShader, m_GraphicDevice);

	ret = CreateVertexShaderConstantBuffer();

	if(ret != AEResult::Ok)
	{
		return AEResult::ConstantBufferInitFailed;
	}

	return AEResult::Ok;
}

AEResult DiffuseTextureMaterial::CreateVertexShaderConstantBuffer()
{
	AEResult ret = AEResult::Ok;
	
	/////////////////////////////////////////////////////
	//Create Constant Buffer 
	//	- _AE_CB_World_View_Proj
	//
	
	/****************************************************************************
	*Constant Buffer #1: _AE_CB_World_View_Proj
	****************************************************************************/
	ConstantBuffer* cbWVP = nullptr;

	ret = AEBuiltInMaterialsHelpers::BuildCBWVP(m_GraphicDevice, &cbWVP);

	if(ret != AEResult::Ok)
	{
		return AEResult::ConstantBufferInitFailed;
	}

	ret = m_VSProps->AddConstantBuffer(cbWVP);

	if(ret != AEResult::Ok)
	{
		DeleteMem(cbWVP);

		return AEResult::ConstantBufferInitFailed;
	}
	
	/****************************************************************************
	*Finish
	****************************************************************************/
	return AEResult::Ok;
}

AEResult DiffuseTextureMaterial::CreatePixelShader()
{
	AEResult ret = AEResult::Ok;
	
	/////////////////////////////////////////////////////
	//Get Pixel Shader from Game Resources
	m_PixelShader = (PixelShader*)m_GameResourceManager->AcquireGameResourceByStringID(AE_DIFFUSE_TEXTURE_MAT_PS_NAME, GameResourceType::PixelShader);
	
	if(m_PixelShader == nullptr)
	{
		m_PixelShader = new PixelShader(m_GraphicDevice, AE_DIFFUSE_TEXTURE_MAT_PS_NAME);
		ret = m_PixelShader->LoadShader(DiffuseTextureMaterialPS, sizeof(DiffuseTextureMaterialPS));
		if(ret != AEResult::Ok)
		{
			return AEResult::PixelShaderLoadFailed;
		}

		ret = m_GameResourceManager->ManageGameResource(m_PixelShader, AE_DIFFUSE_TEXTURE_MAT_PS_NAME);
		if(ret != AEResult::Ok)
		{
			return AEResult::ResourceManagedFailed;
		}
	}

	/////////////////////
	//Create Properties
	m_PSProps = new ShaderProperties(ShaderType::PixelShader, m_GraphicDevice);

	ret = CreatePixelShaderTextureBinding();

	if(ret != AEResult::Ok)
	{
		return AEResult::ShaderTextureBindingInitFailed;
	}

	return AEResult::Ok;
}
					   
AEResult DiffuseTextureMaterial::CreatePixelShaderTextureBinding()
{
	AEResult ret = AEResult::Ok;
	
	/////////////////////////////////////////////////////
	//Create Texture Binding
	//as in Shader:
	// Texture2D _AE_DiffuseTexture : register(t2);
	//

	ShaderTextureBinding* stb = new ShaderTextureBinding(AE_TX_DIFFUSE_TEXTURE_NAME, 2, TextureType::Texture2D, nullptr);

	ret = m_PSProps->AddShaderTextureBinding(stb);

	if(ret != AEResult::Ok)
	{
		DeleteMem(stb);

		return AEResult::ShaderTextureBindingInitFailed;
	}

	return AEResult::Ok;
}

AEResult DiffuseTextureMaterial::LoadContent()
{
	AEAssert(m_GraphicDevice != nullptr);

	if(m_GraphicDevice == nullptr)
	{
		return AEResult::GraphicDeviceNull;
	}

	AEAssert(m_GameResourceManager != nullptr);

	if(m_GameResourceManager == nullptr)
	{
		return AEResult::GameResourceManagerNull;
	}

	if(m_IsReady)
	{
		return AEResult::Ok;
	}

	////////////////////////////////////
	//Clean up memory
	CleanUp();

	AEResult ret = AEResult::Ok;
	
	/***************************
	*Vertex Shader
	***************************/
	ret = CreateVertexShader();

	if(ret != AEResult::Ok)
	{
		CleanUp();

		return ret;
	}

	/***************************
	*Pixel Shader
	***************************/
	ret = CreatePixelShader();

	if(ret != AEResult::Ok)
	{
		CleanUp();

		return ret;
	}
	
	/***************************
	*Finish
	***************************/
	m_IsReady = true;

	return AEResult::Ok;
}
