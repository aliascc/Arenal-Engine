/*
* Copyright (c) 2018 <Carlos Chacón>
* All rights reserved
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
* http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*/

/*************************
*   Precompiled Header   *
**************************/
#include "precomp_graphics.h"

/**********************
*   System Includes   *
***********************/

/*************************
*   3rd Party Includes   *
**************************/

/***************************
*   Game Engine Includes   *
****************************/
#include "Logger\Logger.h"
#include "GraphicDevice.h"
#include "Base\BaseFunctions.h"
#include "DepthStencilSurface.h"
#include "Localization\LocalizationManager.h"

//Always include last
#include "Memory\MemLeaks.h"

/********************
*   Function Defs   *
*********************/
DepthStencilSurface::DepthStencilSurface(GraphicDevice* graphicDevice, const std::string& name)
    : ITexture2D(graphicDevice, name)
{
}

DepthStencilSurface::~DepthStencilSurface()
{
    ReleaseCOM(m_DepthStencilDX);
}

void DepthStencilSurface::CleanUp()
{
    ReleaseCOM(m_TextureDX);
    ReleaseCOM(m_DepthStencilDX);
    ReleaseCOM(m_ShaderResourceView);
}

AEResult DepthStencilSurface::Load()
{
    return AEResult::Fail;
}

AEResult DepthStencilSurface::InitializeDepthStencilSurface(uint32_t width, uint32_t height, DXGI_FORMAT formatDST, DXGI_FORMAT formatDSV, DXGI_FORMAT formatDSSRV)
{
    AEAssert(m_GraphicDevice != nullptr)

    if(m_GraphicDevice == nullptr)
    {
        return AEResult::GraphicDeviceNull;
    }

    //Clean if we had previously allocated resources
    CleanUp();

    D3D11_TEXTURE2D_DESC dxDesc = { 0 };

    dxDesc.Width                = width;
    dxDesc.Height               = height;
    dxDesc.MipLevels            = 1;//Depth Stencil for Default has 1 Mip Map
    dxDesc.ArraySize            = 1;//Depth Stencil for Default has 1 Array Size
    dxDesc.Format               = formatDST;

    AETODO("Check Multisampling in DS");
    dxDesc.SampleDesc.Count     = 1;
    dxDesc.SampleDesc.Quality   = 0;

    dxDesc.Usage                = D3D11_USAGE_DEFAULT;
    dxDesc.BindFlags            = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_DEPTH_STENCIL;
    dxDesc.CPUAccessFlags       = 0;

    AETODO("Check other flags for RT and if Mip Map is needed");
    dxDesc.MiscFlags            = 0;

    HRESULT hr = S_OK;

    AETODO("Check if we need Depth Stencil Init Data");
    hr = m_GraphicDevice->GetDeviceDX()->CreateTexture2D(&dxDesc, nullptr, &m_TextureDX);

    if(hr != S_OK)
    {
        DisplayError(hr);

        return AEResult::CreateTextureFailed;
    }

    AEGraphicHelpers::SetDebugObjectName<ID3D11Texture2D>(m_TextureDX, AE_DEBUG_DEPTH_T_NAME_PREFIX + m_Name);

    D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
    memset(&srvDesc, 0, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));

    AETODO(Check Buffer Variable);
    //srvDesc.Buffer;
    srvDesc.Format                       = formatDSSRV;
    srvDesc.ViewDimension                = D3D11_SRV_DIMENSION_TEXTURE2D;
    srvDesc.Texture2D.MipLevels          = dxDesc.MipLevels;
    srvDesc.Texture2D.MostDetailedMip    = 0;

    AETODO("Check Shader Resource Desc");
    hr = m_GraphicDevice->GetDeviceDX()->CreateShaderResourceView(m_TextureDX, &srvDesc, &m_ShaderResourceView);

    if(hr != S_OK)
    {
        DisplayError(hr);

        AETODO("Add Log here");
        ReleaseCOM(m_TextureDX);

        return AEResult::CreateSRViewFailed;
    }

    AEGraphicHelpers::SetDebugObjectName<ID3D11ShaderResourceView>(m_ShaderResourceView, AE_DEBUG_DEPTH_SRV_NAME_PREFIX + m_Name);

    D3D11_DEPTH_STENCIL_VIEW_DESC dsDesc;
    memset(&dsDesc, 0, sizeof(D3D11_DEPTH_STENCIL_VIEW_DESC));

    AETODO("Check for flags");
    dsDesc.Flags                = 0;
    dsDesc.Format               = formatDSV;

    AETODO("Check for multisample");
    dsDesc.ViewDimension        = D3D11_DSV_DIMENSION_TEXTURE2D;
    dsDesc.Texture2D.MipSlice   = 0;

    AETODO("Check Depth Stencil View Desc");
    hr = m_GraphicDevice->GetDeviceDX()->CreateDepthStencilView(m_TextureDX, &dsDesc, &m_DepthStencilDX);

    if(hr != S_OK)
    {
        DisplayError(hr);

        AETODO("Add Log here");
        ReleaseCOM(m_ShaderResourceView);
        ReleaseCOM(m_TextureDX);

        return AEResult::CreateDSViewFailed;
    }

    AEGraphicHelpers::SetDebugObjectName<ID3D11DepthStencilView>(m_DepthStencilDX, AE_DEBUG_DEPTH_S_NAME_PREFIX + m_Name);

    m_Width = width;
    m_Height = height;

    return AEResult::Ok;
}
