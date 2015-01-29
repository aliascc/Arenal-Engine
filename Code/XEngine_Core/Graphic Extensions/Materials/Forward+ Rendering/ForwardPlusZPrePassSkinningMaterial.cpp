
/********************************************************
*
* Author: Carlos Chacón N.
*
* Created: 11/10/2012
*
* Last Major Update: 12/30/2013
* 
* Desc:
*
*
* Copyright 2013
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
#include "Shaders\VertexShader.h"
#include "Resource\GameResourceManager.h"
#include "Shaders\Buffers\ConstantBuffer.h"
#include "ForwardPlusZPrePassSkinningMaterial.h"
#include "Shaders\Variables\ShaderCustomVariable.h"
#include "Graphic Extensions\Shader Extensions\Properties\ShaderProperties.h"

#if defined(DEBUG) | defined(_DEBUG)
	#if defined(_WIN64) | defined (WIN64) 
#include "Compiled Materials\HLSL\ForwardPlusZPrePassSkinning_x64_d.h"
	#else
#include "Compiled Materials\HLSL\ForwardPlusZPrePassSkinning_x86_d.h"
	#endif
#else
	#if defined(_WIN64) | defined (WIN64) 
#include "Compiled Materials\HLSL\ForwardPlusZPrePassSkinning_x64.h"
	#else
#include "Compiled Materials\HLSL\ForwardPlusZPrePassSkinning_x86.h"
	#endif
#endif

//Always include last
#include "Memory\MemLeaks.h"

/********************
*   Function Defs   *
*********************/
ForwardPlusZPrePassSkinningMaterial::ForwardPlusZPrePassSkinningMaterial(GraphicDevice* graphicDevice, GameResourceManager* gameResourceManager, const std::wstring& name)
	: Material(graphicDevice, gameResourceManager, name)
{
}

ForwardPlusZPrePassSkinningMaterial::~ForwardPlusZPrePassSkinningMaterial()
{
}

XEResult ForwardPlusZPrePassSkinningMaterial::CreateVertexShader()
{
	XEResult ret = XEResult::Ok;

	/////////////////////////////////////////////////////
	//Get Vertex Shader from Game Resources
	m_VertexShader = (VertexShader*)m_GameResourceManager->AcquireGameResourceByStringID(XE_FORWARD_PLUS_Z_PRE_PASS_SKINNING_MAT_VS_NAME, GameResourceType::VertexShader);

	if(m_VertexShader == nullptr)
	{
		m_VertexShader = new VertexShader(m_GraphicDevice, XE_FORWARD_PLUS_Z_PRE_PASS_SKINNING_MAT_VS_NAME);
		ret = m_VertexShader->LoadShader(ForwardPlusZPrePassSkinning, sizeof(ForwardPlusZPrePassSkinning));
		if(ret != XEResult::Ok)
		{
			return XEResult::VertexShaderLoadFailed;
		}

		ret = m_GameResourceManager->ManageGameResource(m_VertexShader, XE_FORWARD_PLUS_Z_PRE_PASS_SKINNING_MAT_VS_NAME);
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

XEResult ForwardPlusZPrePassSkinningMaterial::CreateVertexShaderConstantBuffer()
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
	*Constant Buffer #2: _XE_CB_Bones
	****************************************************************************/
	ConstantBuffer* cbBones = nullptr;

	ret = XEBuiltInMaterialsHelpers::BuildCBBones(m_GraphicDevice, &cbBones);

	if (ret != XEResult::Ok)
	{
		return XEResult::ConstantBufferInitFailed;
	}

	ret = m_VSProps->AddConstantBuffer(cbBones);

	if (ret != XEResult::Ok)
	{
		DeleteMem(cbBones);

		return XEResult::ConstantBufferInitFailed;
	}

	/****************************************************************************
	*Finish
	****************************************************************************/
	return XEResult::Ok;
}

XEResult ForwardPlusZPrePassSkinningMaterial::LoadContent()
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
	*Finish
	***************************/
	m_IsReady = true;

	return XEResult::Ok;
}

