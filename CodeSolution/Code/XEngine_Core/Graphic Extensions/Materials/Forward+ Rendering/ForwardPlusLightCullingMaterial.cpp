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

XEResult ForwardPlusLightCullingMaterial::CreateComputeShader()
{
	XEResult ret = XEResult::Ok;

	/////////////////////////////////////////////////////
	//Get Compute Shader from Game Resources
	m_ComputeShader = (ComputeShader*)m_GameResourceManager->AcquireGameResourceByStringID(XE_FORWARD_PLUS_LIGHT_CULLING_MAT_CS_NAME, GameResourceType::ComputeShader);

	if(m_ComputeShader == nullptr)
	{
		m_ComputeShader = new ComputeShader(m_GraphicDevice, XE_FORWARD_PLUS_LIGHT_CULLING_MAT_CS_NAME);
		ret = m_ComputeShader->LoadShader(ForwardPlusLightCulling, sizeof(ForwardPlusLightCulling));
		if(ret != XEResult::Ok)
		{
			return XEResult::ComputeShaderLoadFailed;
		}

		ret = m_GameResourceManager->ManageGameResource(m_ComputeShader, XE_FORWARD_PLUS_LIGHT_CULLING_MAT_CS_NAME);
		if(ret != XEResult::Ok)
		{
			return XEResult::ResourceManagedFailed;
		}
	}

	/////////////////////
	//Create Properties
	m_CSProps = new ShaderProperties(ShaderType::ComputeShader, m_GraphicDevice);

	ret = CreateComputeShaderConstantBuffer();
	if(ret != XEResult::Ok)
	{
		return XEResult::ConstantBufferInitFailed;
	}

	ret = CreateComputeShaderTextureBinding();
	if(ret != XEResult::Ok)
	{
		return XEResult::ShaderTextureBindingInitFailed;
	}

	ret = CreateComputeShaderStructuredBuffers();
	if(ret != XEResult::Ok)
	{
		return XEResult::ShaderStructuredBufferInitFailed;
	}

	ret = CreateComputeShaderSimpleBuffers();
	if(ret != XEResult::Ok)
	{
		return XEResult::ShaderSimpleBufferInitFailed;
	}

	return XEResult::Ok;
}

XEResult ForwardPlusLightCullingMaterial::CreateComputeShaderConstantBuffer()
{
	XEResult ret = XEResult::Ok;
	
	/****************************************************************************
	*Constant Buffer #1: _XE_CB_FPR_LightCulling
	****************************************************************************/
	ConstantBuffer* cbFPRPerFrame = nullptr;

	ret = XEBuiltInMaterialsHelpers::BuildCBFPRLightCulling(m_GraphicDevice, &cbFPRPerFrame);
	if(ret != XEResult::Ok)
	{
		return XEResult::ConstantBufferInitFailed;
	}

	ret = m_CSProps->AddConstantBuffer(cbFPRPerFrame);
	if(ret != XEResult::Ok)
	{
		DeleteMem(cbFPRPerFrame);

		return XEResult::ConstantBufferInitFailed;
	}

	/****************************************************************************
	*Finish
	****************************************************************************/
	return XEResult::Ok;
}

XEResult ForwardPlusLightCullingMaterial::CreateComputeShaderTextureBinding()
{
	XEResult ret = XEResult::Ok;
	
	/////////////////////////////////////////////////////
	//Create Texture Binding
	//as in Shader:
	// Texture2D<float> _XE_DepthTexture : register(t1);
	//

	ShaderTextureBinding* stb = new ShaderTextureBinding(XE_TX_DEPTH_TEXTURE_CULL_NAME, 1, TextureType::Texture2D, nullptr);

	ret = m_CSProps->AddShaderTextureBinding(stb);

	if(ret != XEResult::Ok)
	{
		DeleteMem(stb);

		return XEResult::ShaderTextureBindingInitFailed;
	}

	return XEResult::Ok;
}

XEResult ForwardPlusLightCullingMaterial::CreateComputeShaderStructuredBuffers()
{
	XEResult ret = XEResult::Ok;
	
	/****************************************************************************
	*Structured Buffer #1: _XE_LightBuffer
	****************************************************************************/
	StructuredBuffer* sbLightBuffer = nullptr;

	ret = XEBuiltInMaterialsHelpers::BuildBufferLightBuffer(m_GraphicDevice, &sbLightBuffer);
	if(ret != XEResult::Ok)
	{
		return XEResult::ShaderStructuredBufferInitFailed;
	}

	ret = m_CSProps->AddStructuredBuffer(sbLightBuffer);
	if(ret != XEResult::Ok)
	{
		DeleteMem(sbLightBuffer);

		return XEResult::ShaderStructuredBufferInitFailed;
	}

	/****************************************************************************
	*Finish
	****************************************************************************/
	return XEResult::Ok;
}

XEResult ForwardPlusLightCullingMaterial::CreateComputeShaderSimpleBuffers()
{
	XEResult ret = XEResult::Ok;
	
	/****************************************************************************
	*Simple Buffer #1: _XE_PerTileLightIndexBuffer
	****************************************************************************/
	SimpleBuffer* sbPerTileLightIndexBuffer = nullptr;

	ret = XEBuiltInMaterialsHelpers::BuildBufferPerTileLightIndexBuffer(m_GraphicDevice, &sbPerTileLightIndexBuffer);
	if(ret != XEResult::Ok)
	{
		return XEResult::ShaderSimpleBufferInitFailed;
	}

	ret = m_CSProps->AddSimpleBuffer(sbPerTileLightIndexBuffer);
	if(ret != XEResult::Ok)
	{
		DeleteMem(sbPerTileLightIndexBuffer);

		return XEResult::ShaderSimpleBufferInitFailed;
	}

	/****************************************************************************
	*Finish
	****************************************************************************/
	return XEResult::Ok;
}

XEResult ForwardPlusLightCullingMaterial::LoadContent()
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
	*Compute Shader
	***************************/
	ret = CreateComputeShader();

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

