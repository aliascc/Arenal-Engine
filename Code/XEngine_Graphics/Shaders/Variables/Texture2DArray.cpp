/********************************************************
*
* Author: Carlos Chacón N.
*
* Created: 2/22/2012
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
#include "Texture2DArray.h"
#include "Textures\Texture.h"
#include "Base\BaseFunctions.h"
#include "Textures\ITexture2D.h"

//Always include last
#include "Memory\MemLeaks.h"

/********************
*   Function Defs   *
*********************/
Texture2DArray::Texture2DArray(GraphicDevice* graphicDevice, const std::wstring& name, uint32_t bindIndex)
	: TextureArray(graphicDevice, name, TextureType::Texture2D, bindIndex)
{
}

Texture2DArray::~Texture2DArray()
{
	CleanUp();
}

void Texture2DArray::CleanUp()
{
	ReleaseCOM(m_TextureArrayDX);

	for (size_t i = 0; i < m_RenderTargetViewsVector.size(); i++)
	{
		ReleaseCOM(m_RenderTargetViewsVector[i]);
	}

	m_RenderTargetViewsVector.clear();

	ReleaseCOM(m_TextureArraySRV);
}

XEResult Texture2DArray::Deinitialize()
{
	/////////////////////////////////////////////////////////////
	//Clean up memory
	m_IsReady			= false;
	m_ArraySize			= 0;
	m_Width				= 0;
	m_Height			= 0;
	m_MipLevels			= 0;
	m_Format			= DXGI_FORMAT_R32G32B32A32_FLOAT;
	m_TextureBindOption	= TextureBindOption::Default;

	CleanUp();

	return XEResult::Ok;
}

XEResult Texture2DArray::Initialize(uint32_t arraySize, uint32_t width, uint32_t height, uint32_t mipLevels, DXGI_FORMAT format, TextureBindOption bindOption)
{
	/////////////////////////////////////////////////////////////
	//Pre-check
	XEAssert(m_GraphicDevice != nullptr);
	if (m_GraphicDevice == nullptr)
	{
		return XEResult::GraphicDeviceNull;
	}

	XEAssert(arraySize != 0);
	if (arraySize == 0)
	{
		return XEResult::ZeroSize;
	}

	/////////////////////////////////////////////////////////////
	//Deinitialize the array
	Deinitialize();

	HRESULT hr = S_OK;

	/////////////////////////////////////////////////////////////
	//Create Texture Array
	D3D11_TEXTURE2D_DESC textDesc;
	memset(&textDesc, 0, sizeof(D3D11_TEXTURE2D_DESC));

	UINT bindFlags = D3D11_BIND_SHADER_RESOURCE;

	switch (bindOption)
	{
		case TextureBindOption::RenderTarget:
			bindFlags |= D3D11_BIND_RENDER_TARGET;
			break;

		case TextureBindOption::DepthStencil:
			bindFlags |= D3D11_BIND_DEPTH_STENCIL;
			break;
	}

	textDesc.Width				= width;
	textDesc.Height				= height;
	textDesc.MipLevels			= mipLevels;
	textDesc.Format				= format;
	textDesc.ArraySize			= arraySize;

	XETODO("Make this options");
	textDesc.Usage				= D3D11_USAGE_DEFAULT;
	textDesc.BindFlags			= bindFlags;
	textDesc.CPUAccessFlags		= 0;
	textDesc.MiscFlags			= 0;

	XETODO("Look into this to see if we need to pass this as parameters at constructor");
	textDesc.SampleDesc.Count	= 1;
	textDesc.SampleDesc.Quality	= 0;

	hr = m_GraphicDevice->GetDeviceDX()->CreateTexture2D(&textDesc, nullptr, &m_TextureArrayDX);

	if (hr != S_OK)
	{
		CleanUp();

		DisplayError(hr);

		XETODO("Better return code");
		return XEResult::Fail;
	}

	XEGraphicHelpers::SetDebugObjectName<ID3D11Texture2D>(m_TextureArrayDX, XE_DEBUG_TEX_2D_ARRAY_NAME_PREFIX + XE_Base::WideStr2String(m_Name));

	/////////////////////////////////////////////////////////////
	//Create Texture Array Shader Resource View
	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	memset(&srvDesc, 0, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));

	srvDesc.Format						= format;
	srvDesc.ViewDimension				= D3D_SRV_DIMENSION_TEXTURE2DARRAY;

	srvDesc.Texture2DArray.ArraySize	= arraySize;
	srvDesc.Texture2DArray.MipLevels	= mipLevels;

	hr = m_GraphicDevice->GetDeviceDX()->CreateShaderResourceView(m_TextureArrayDX, &srvDesc, &m_TextureArraySRV);

	if (hr != S_OK)
	{
		CleanUp();

		DisplayError(hr);

		XETODO("Better return code");
		return XEResult::Fail;
	}

	XEGraphicHelpers::SetDebugObjectName<ID3D11ShaderResourceView>(m_TextureArraySRV, XE_DEBUG_TEX_2D_ARRAY_SRV_NAME_PREFIX + XE_Base::WideStr2String(m_Name));

	/////////////////////////////////////////////////////////////
	//Create Render Target Views
	if(bindOption == TextureBindOption::RenderTarget)
	{
		for (uint32_t i = 0; i < arraySize; i++)
		{
			ID3D11RenderTargetView* rtv = nullptr;

			D3D11_RENDER_TARGET_VIEW_DESC rtDesc;
			memset(&rtDesc, 0, sizeof(D3D11_RENDER_TARGET_VIEW_DESC));
		
			rtDesc.Format						= format;
			rtDesc.ViewDimension				= D3D11_RTV_DIMENSION_TEXTURE2DARRAY;

			rtDesc.Texture2DArray.ArraySize			= 1;
			rtDesc.Texture2DArray.FirstArraySlice	= i;
			rtDesc.Texture2DArray.MipSlice			= 0;

			hr = m_GraphicDevice->GetDeviceDX()->CreateRenderTargetView(m_TextureArrayDX, &rtDesc, &rtv);

			if (hr != S_OK)
			{
				CleanUp();

				DisplayError(hr);

				XETODO("Better return code");
				return XEResult::Fail;
			}

			m_RenderTargetViewsVector.push_back(rtv);

			XEGraphicHelpers::SetDebugObjectName<ID3D11RenderTargetView>(rtv, XE_DEBUG_TEX_2D_ARRAY_RTVS_NAME_PREFIX + XE_Base::WideStr2String(m_Name));
		}
	}

	/////////////////////////////////////////////////////////////
	//Finish
	m_ArraySize			= arraySize;
	m_Width				= width;
	m_Height			= height;
	m_MipLevels			= mipLevels;
	m_Format			= format;
	m_TextureBindOption	= bindOption;

	m_IsReady = true;

	return XEResult::Ok;
}

ID3D11RenderTargetView* Texture2DArray::GetRenderTargetDX(uint32_t index)
{
	if (index >= m_RenderTargetViewsVector.size())
	{
		return nullptr;
	}

	return m_RenderTargetViewsVector[index];
}

XEResult Texture2DArray::SetTexture(uint32_t index, Texture* texture)
{
	if (!m_IsReady)
	{
		return XEResult::NotReady;
	}

	XEAssert(texture != nullptr);
	if (texture == nullptr)
	{
		return XEResult::NullParameter;
	}

	XEAssert(index < m_ArraySize);
	if (index >= m_ArraySize)
	{
		return XEResult::OutsideRange;
	}

	XEAssert(texture->GetTextureType() == TextureType::Texture2D);
	if (texture->GetTextureType() != TextureType::Texture2D)
	{
		return XEResult::InvalidTextureType;
	}

	bool errorFound = false;
	XEResult ret = XEResult::Ok;
	HRESULT hr = S_OK;
	ID3D11DeviceContext* dxContext = m_GraphicDevice->GetDeviceContextDX();

	D3D11_TEXTURE2D_DESC textDesc;
	memset(&textDesc, 0, sizeof(D3D11_TEXTURE2D_DESC));

	ITexture2D* texture2D = reinterpret_cast<ITexture2D*>(texture);

	ID3D11Texture2D* textureDX = texture2D->GetDXTexture();

	textureDX->GetDesc(&textDesc);

	if (textDesc.Format != m_Format || textDesc.MipLevels != m_MipLevels || textDesc.Height != m_Height || textDesc.Width != m_Width)
	{
		XETODO("Better return code");
		return XEResult::Fail;
	}

	for (uint32_t i = 0; i < m_MipLevels; i++)
	{
		D3D11_MAPPED_SUBRESOURCE mappedText2D;

		hr = dxContext->Map(textureDX, i, D3D11_MAP_READ, 0, &mappedText2D);

		if (hr == S_OK)
		{
			dxContext->UpdateSubresource(m_TextureArrayDX, D3D11CalcSubresource(i, index, m_MipLevels), 0, mappedText2D.pData, mappedText2D.RowPitch, mappedText2D.RowPitch);

			dxContext->Unmap(textureDX, i);
		}
		else
		{
			errorFound = true;
		}
	}

	if (errorFound)
	{
		XETODO("Assign to ret ErrorsEncountered");
		XETODO("Log error");
	}

	return ret;
}