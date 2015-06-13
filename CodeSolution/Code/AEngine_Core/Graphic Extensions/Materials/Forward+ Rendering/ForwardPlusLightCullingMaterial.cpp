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
#include "Base\BaseFunctions.h"
#include "Shaders\ComputeShader.h"
#include "Shaders\Buffers\SimpleBuffer.h"
#include "Resource\GameResourceManager.h"
#include "Shaders\Buffers\ConstantBuffer.h"
#include "ForwardPlusLightCullingMaterial.h"
#include "Shaders\Buffers\StructuredBuffer.h"
#include "Shaders\Bindings\ShaderTextureBinding.h"
#include "Shaders\Variables\ShaderCustomVariable.h"
#include "Graphic Extensions\Shader Extensions\Properties\ShaderProperties.h"

#if defined(DEBUG) | defined(_DEBUG)
	#if defined(_WIN64) | defined (WIN64) 
#include "Compiled Materials\HLSL\ForwardPlusLightCulling_x64_d.h"
	#else
#include "Compiled Materials\HLSL\ForwardPlusLightCulling_x86_d.h"
	#endif
#else
	#if defined(_WIN64) | defined (WIN64) 
#include "Compiled Materials\HLSL\ForwardPlusLightCulling_x64.h"
	#else
#include "Compiled Materials\HLSL\ForwardPlusLightCulling_x86.h"
	#endif
#endif

//Always include last
#include "Memory\MemLeaks.h"

/********************
*   Function Defs   *
*********************/
ForwardPlusLightCullingMaterial::ForwardPlusLightCullingMaterial(GraphicDevice* graphicDevice, GameResourceManager* gameResourceManager, const std::wstring& name)
	: Material(graphicDevice, gameResourceManager, name)
{
}

ForwardPlusLightCullingMaterial::~ForwardPlusLightCullingMaterial()
{
}

AEResult ForwardPlusLightCullingMaterial::CreateComputeShader()
{
	AEResult ret = AEResult::Ok;

	/////////////////////////////////////////////////////
	//Get Compute Shader from Game Resources
	m_ComputeShader = (ComputeShader*)m_GameResourceManager->AcquireGameResourceByStringID(AE_FORWARD_PLUS_LIGHT_CULLING_MAT_CS_NAME, GameResourceType::ComputeShader);

	if(m_ComputeShader == nullptr)
	{
		m_ComputeShader = new ComputeShader(m_GraphicDevice, AE_FORWARD_PLUS_LIGHT_CULLING_MAT_CS_NAME);
		ret = m_ComputeShader->LoadShader(ForwardPlusLightCulling, sizeof(ForwardPlusLightCulling));
		if(ret != AEResult::Ok)
		{
			return AEResult::ComputeShaderLoadFailed;
		}

		ret = m_GameResourceManager->ManageGameResource(m_ComputeShader, AE_FORWARD_PLUS_LIGHT_CULLING_MAT_CS_NAME);
		if(ret != AEResult::Ok)
		{
			return AEResult::ResourceManagedFailed;
		}
	}

	/////////////////////
	//Create Properties
	m_CSProps = new ShaderProperties(ShaderType::ComputeShader, m_GraphicDevice);

	ret = CreateComputeShaderConstantBuffer();
	if(ret != AEResult::Ok)
	{
		return AEResult::ConstantBufferInitFailed;
	}

	ret = CreateComputeShaderTextureBinding();
	if(ret != AEResult::Ok)
	{
		return AEResult::ShaderTextureBindingInitFailed;
	}

	ret = CreateComputeShaderStructuredBuffers();
	if(ret != AEResult::Ok)
	{
		return AEResult::ShaderStructuredBufferInitFailed;
	}

	ret = CreateComputeShaderSimpleBuffers();
	if(ret != AEResult::Ok)
	{
		return AEResult::ShaderSimpleBufferInitFailed;
	}

	return AEResult::Ok;
}

AEResult ForwardPlusLightCullingMaterial::CreateComputeShaderConstantBuffer()
{
	AEResult ret = AEResult::Ok;
	
	/****************************************************************************
	*Constant Buffer #1: _AE_CB_FPR_LightCulling
	****************************************************************************/
	ConstantBuffer* cbFPRPerFrame = nullptr;

	ret = AEBuiltInMaterialsHelpers::BuildCBFPRLightCulling(m_GraphicDevice, &cbFPRPerFrame);
	if(ret != AEResult::Ok)
	{
		return AEResult::ConstantBufferInitFailed;
	}

	ret = m_CSProps->AddConstantBuffer(cbFPRPerFrame);
	if(ret != AEResult::Ok)
	{
		DeleteMem(cbFPRPerFrame);

		return AEResult::ConstantBufferInitFailed;
	}

	/****************************************************************************
	*Finish
	****************************************************************************/
	return AEResult::Ok;
}

AEResult ForwardPlusLightCullingMaterial::CreateComputeShaderTextureBinding()
{
	AEResult ret = AEResult::Ok;
	
	/////////////////////////////////////////////////////
	//Create Texture Binding
	//as in Shader:
	// Texture2D<float> _AE_DepthTexture : register(t1);
	//

	ShaderTextureBinding* stb = new ShaderTextureBinding(AE_TX_DEPTH_TEXTURE_CULL_NAME, 1, TextureType::Texture2D, nullptr);

	ret = m_CSProps->AddShaderTextureBinding(stb);

	if(ret != AEResult::Ok)
	{
		DeleteMem(stb);

		return AEResult::ShaderTextureBindingInitFailed;
	}

	return AEResult::Ok;
}

AEResult ForwardPlusLightCullingMaterial::CreateComputeShaderStructuredBuffers()
{
	AEResult ret = AEResult::Ok;
	
	/****************************************************************************
	*Structured Buffer #1: _AE_LightBuffer
	****************************************************************************/
	StructuredBuffer* sbLightBuffer = nullptr;

	ret = AEBuiltInMaterialsHelpers::BuildBufferLightBuffer(m_GraphicDevice, &sbLightBuffer);
	if(ret != AEResult::Ok)
	{
		return AEResult::ShaderStructuredBufferInitFailed;
	}

	ret = m_CSProps->AddStructuredBuffer(sbLightBuffer);
	if(ret != AEResult::Ok)
	{
		DeleteMem(sbLightBuffer);

		return AEResult::ShaderStructuredBufferInitFailed;
	}

	/****************************************************************************
	*Finish
	****************************************************************************/
	return AEResult::Ok;
}

AEResult ForwardPlusLightCullingMaterial::CreateComputeShaderSimpleBuffers()
{
	AEResult ret = AEResult::Ok;
	
	/****************************************************************************
	*Simple Buffer #1: _AE_PerTileLightIndexBuffer
	****************************************************************************/
	SimpleBuffer* sbPerTileLightIndexBuffer = nullptr;

	ret = AEBuiltInMaterialsHelpers::BuildBufferPerTileLightIndexBuffer(m_GraphicDevice, &sbPerTileLightIndexBuffer);
	if(ret != AEResult::Ok)
	{
		return AEResult::ShaderSimpleBufferInitFailed;
	}

	ret = m_CSProps->AddSimpleBuffer(sbPerTileLightIndexBuffer);
	if(ret != AEResult::Ok)
	{
		DeleteMem(sbPerTileLightIndexBuffer);

		return AEResult::ShaderSimpleBufferInitFailed;
	}

	/****************************************************************************
	*Finish
	****************************************************************************/
	return AEResult::Ok;
}

AEResult ForwardPlusLightCullingMaterial::LoadContent()
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
	*Compute Shader
	***************************/
	ret = CreateComputeShader();

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

