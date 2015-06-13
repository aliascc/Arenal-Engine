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
#include <iostream>

/*************************
*   3rd Party Includes   *
**************************/

/***************************
*   Game Engine Includes   *
****************************/
#include "Sampler.h"
#include "Logger\Logger.h"
#include "GraphicDevice.h"
#include "Base\BaseFunctions.h"
#include "Localization\LocalizationManager.h"

//Always include last
#include "Memory\MemLeaks.h"

/********************
*   Function Defs   *
*********************/
Sampler::Sampler(GraphicDevice* graphicDevice, const std::wstring& name, uint32_t bindIndex)
	: ShaderBinding(name, bindIndex)
	, m_GraphicDevice(graphicDevice)
{
	AEAssert(m_GraphicDevice != nullptr);
}

Sampler::~Sampler()
{
	ReleaseCOM(m_SamplerDX);
}

AEResult Sampler::Initialize(ShaderFilter shaderFilter, ShaderTextureAddressMode textureAddressModeU, ShaderTextureAddressMode textureAddressModeV, ShaderTextureAddressMode textureAddressModeW, float minLOD, float maxLOD, float mipLODBias, uint32_t maxAnisotropy, ShaderCompFunct compFunction, const Color& borderColor)
{
	///////////////////////////////////////////////
	//Pre-checks
	AEAssert(m_GraphicDevice != nullptr);
	if (m_GraphicDevice == nullptr)
	{
		return AEResult::GraphicDeviceNull;
	}

	///////////////////////////////////////////////
	//Clean Memory
	ReleaseCOM(m_SamplerDX);
	m_IsReady = false;
	m_NeedsReinit = false;

	///////////////////////////////////////////////
	//Create DX Sampler Desc
	D3D11_SAMPLER_DESC dxSamplerDesc;
	memset(&dxSamplerDesc, 0, sizeof(D3D11_SAMPLER_DESC));
	
	dxSamplerDesc.Filter			= AEShaderHelper::GetDXFilter(shaderFilter);
	dxSamplerDesc.AddressU			= AEShaderHelper::GetDXTextureAddressMode(textureAddressModeU);
	dxSamplerDesc.AddressV			= AEShaderHelper::GetDXTextureAddressMode(textureAddressModeV);
	dxSamplerDesc.AddressW			= AEShaderHelper::GetDXTextureAddressMode(textureAddressModeW);
	dxSamplerDesc.MinLOD			= minLOD;
	dxSamplerDesc.MaxLOD			= maxLOD;
	dxSamplerDesc.MipLODBias		= mipLODBias;
	dxSamplerDesc.MaxAnisotropy		= maxAnisotropy;
	dxSamplerDesc.ComparisonFunc	= AEShaderHelper::GetDXCompFunct(compFunction);

	borderColor.FillFloatArray(dxSamplerDesc.BorderColor);

	///////////////////////////////////////////////
	//Create DX Sampler
	HRESULT hr = m_GraphicDevice->GetDeviceDX()->CreateSamplerState(&dxSamplerDesc, &m_SamplerDX);

	if (hr != S_OK)
	{
		DisplayError(hr);

		AETODO("Better return code");
		return AEResult::Fail;
	}

	///////////////////////////////////////////////
	//Set New Variables
	m_ShaderFilter			= shaderFilter;
	m_TextureAddressModeU	= textureAddressModeU;
	m_TextureAddressModeV	= textureAddressModeU;
	m_TextureAddressModeW	= textureAddressModeU;
	m_MinLOD				= minLOD;
	m_MaxLOD				= maxLOD;
	m_MipLODBias			= mipLODBias;
	m_MaxAnisotropy			= maxAnisotropy;
	m_CompFunction			= compFunction;
	m_BorderColor			= borderColor;

	///////////////////////////////////////////////
	//Set Ready to true
	m_IsReady = true;

	///////////////////////////////////////////////
	//Finish
	return AEResult::Ok;
}

AEResult Sampler::Reinitialize()
{
	return Initialize(m_ShaderFilter, m_TextureAddressModeU, m_TextureAddressModeV, m_TextureAddressModeW, m_MinLOD, m_MaxLOD, m_MipLODBias, m_MaxAnisotropy, m_CompFunction, m_BorderColor);
}
