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
#include "DiffuseTextureVS.h"
#include "Base\BaseFunctions.h"
#include "Shaders\ShaderDefs.h"
#include "Shaders\ShaderSignatures.h"

#if defined(DEBUG) | defined(_DEBUG)
#if defined(_WIN64) | defined (WIN64) 
#include "Compiled Materials\HLSL\DiffuseTextureNormalBasicMaterialVS_x64_d.h"
#else
#include "Compiled Materials\HLSL\DiffuseTextureNormalBasicMaterialVS_x86_d.h"
#endif
#else
#if defined(_WIN64) | defined (WIN64) 
#include "Compiled Materials\HLSL\DiffuseTextureNormalBasicMaterialVS_x64.h"
#else
#include "Compiled Materials\HLSL\DiffuseTextureNormalBasicMaterialVS_x86.h"
#endif
#endif

//Always include last
#include "Memory\MemLeaks.h"

/********************
*   Function Defs   *
*********************/
DiffuseTextureVS::DiffuseTextureVS(GraphicDevice* graphicDevice, const std::wstring& name)
	: VertexShader(graphicDevice, name)
{
}

DiffuseTextureVS::~DiffuseTextureVS()
{
}

AEResult DiffuseTextureVS::Load()
{
	/////////////////////////////////////////////
	//Pre-check
	AEAssert(m_GraphicDevice != nullptr);
	if (m_GraphicDevice == nullptr)
	{
		return AEResult::GraphicDeviceNull;
	}

	std::lock_guard<std::mutex> lock(m_GameResourceMutex);

	/////////////////////////////////////////////
	//Variables
	AEResult ret = AEResult::Ok;

	/////////////////////////////////////////////
	//Load Shader Into Memory
	ret = this->LoadShaderWithoutLock(DiffuseTextureNormalBasicMaterialVS, sizeof(DiffuseTextureNormalBasicMaterialVS));;
	if (ret != AEResult::Ok)
	{
		return ret;
	}

	/////////////////////////////////////////////
	//Create Signature Lists

	ConstantBufferSignature wvpSig = AEShaderSignatureHelpers::CreateWorldViewProjCBSig(0);

	/////////////////////////////////////////////
	//Add to Signature Lists

	m_ConstantBufferSignatureList.push_back(wvpSig);

	/////////////////////////////////////////////
	//Finish
	return AEResult::Ok;
}
