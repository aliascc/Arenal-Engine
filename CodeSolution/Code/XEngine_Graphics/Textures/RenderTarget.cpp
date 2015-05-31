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
#include "cppformat\format.h"

/***************************
*   Game Engine Includes   *
****************************/
#include "RenderTarget.h"
#include "Logger\Logger.h"
#include "GraphicDevice.h"
#include "Base\BaseFunctions.h"
#include "Localization\LocalizationManager.h"

//Always include last
#include "Memory\MemLeaks.h"

/********************
*   Function Defs   *
*********************/
RenderTarget::RenderTarget(GraphicDevice* graphicDevice, const std::wstring& renderTargetName)
	: ITexture2D(graphicDevice, renderTargetName)
{
}

RenderTarget::~RenderTarget()
{
	ReleaseCOM(m_RenderTargetDX);
}

void RenderTarget::CleanUp()
{
	ReleaseCOM(m_TextureDX);
	ReleaseCOM(m_RenderTargetDX);
	ReleaseCOM(m_ShaderResourceView);
}

XEResult RenderTarget::Load()
{
	return XEResult::Fail;
}

XEResult RenderTarget::InitializeRenderTarget(uint32_t width, uint32_t height, DXGI_FORMAT format, GraphicBufferUsage graphicBufferUsage, GraphicBufferAccess graphicBufferAccess)
{
	XEAssert(m_GraphicDevice != nullptr)

	if(m_GraphicDevice == nullptr)
	{
		return XEResult::GraphicDeviceNull;
	}

	CleanUp();

	D3D11_TEXTURE2D_DESC dxDesc = { 0 };
	dxDesc.Width				= width;
	dxDesc.Height				= height;
	XETODO("Check Mip Map Levels in RT");

	dxDesc.MipLevels			= 1;
	dxDesc.ArraySize			= 1;
	dxDesc.Format				= format;

	XETODO("Check Multisampling in RT");
	//dxDesc.SampleDesc;
	dxDesc.Usage				= XEGraphicHelpers::GetDXUsage(graphicBufferUsage);
	dxDesc.BindFlags			= D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
	dxDesc.CPUAccessFlags		= XEGraphicHelpers::GetDXBufferAccess(graphicBufferAccess);

	XETODO("Check other flags for RT and if Mip Map is needed");
	dxDesc.MiscFlags			= 0;

	dxDesc.SampleDesc.Count = 1;
	dxDesc.SampleDesc.Quality = 0;

	XETODO("Check if we need this RT");
	/*uint32_t pixelSize = XEGraphicHelpers::GetSizeOfDXFormat(format);
	char* pixelData = new char[pixelSize * width * height];
	ZeroMemory(pixelData, pixelSize * width * height);

	D3D11_SUBRESOURCE_DATA dxData;
	dxData.pSysMem = pixelData;
	dxData.SysMemPitch = pixelSize * width;*/
	//dxData.SysMemSlicePitch = 0;

	HRESULT hr = m_GraphicDevice->GetDeviceDX()->CreateTexture2D(&dxDesc, nullptr/*&dxData*/, &m_TextureDX);

	//DeleteMemArr(pixelData);

	if(hr != S_OK)
	{
		DisplayError(hr);

		return XEResult::CreateTextureFailed;
	}

	XEGraphicHelpers::SetDebugObjectName<ID3D11Texture2D>(m_TextureDX, XE_DEBUG_RT_T_NAME_PREFIX + XE_Base::WideStr2String(m_Name));

	XETODO("Check Shader Resource Desc");
	hr = m_GraphicDevice->GetDeviceDX()->CreateShaderResourceView(m_TextureDX, nullptr, &m_ShaderResourceView);

	if(hr != S_OK)
	{
		DisplayError(hr);

		XETODO("Add Log here");
		ReleaseCOM(m_TextureDX);

		return XEResult::CreateSRViewFailed;
	}

	XEGraphicHelpers::SetDebugObjectName<ID3D11ShaderResourceView>(m_ShaderResourceView, XE_DEBUG_RT_SRV_NAME_PREFIX + XE_Base::WideStr2String(m_Name));

	XETODO("Check Render Target Desc");
	hr = m_GraphicDevice->GetDeviceDX()->CreateRenderTargetView(m_TextureDX, nullptr, &m_RenderTargetDX);

	if(hr != S_OK)
	{
		DisplayError(hr);

		XETODO("Add Log here");
		ReleaseCOM(m_ShaderResourceView);
		ReleaseCOM(m_TextureDX);

		return XEResult::CreateRTViewFailed;
	}

	XEGraphicHelpers::SetDebugObjectName<ID3D11RenderTargetView>(m_RenderTargetDX, XE_DEBUG_RT_NAME_PREFIX + XE_Base::WideStr2String(m_Name));

	m_Width = width;
	m_Height = height;

	return XEResult::Ok;
}