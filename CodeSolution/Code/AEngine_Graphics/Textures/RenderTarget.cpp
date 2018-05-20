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
RenderTarget::RenderTarget(GraphicDevice* graphicDevice, const std::string& renderTargetName)
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

AEResult RenderTarget::Load()
{
    return AEResult::Fail;
}

AEResult RenderTarget::InitializeRenderTarget(uint32_t width, uint32_t height, DXGI_FORMAT format, GraphicBufferUsage graphicBufferUsage, GraphicBufferAccess graphicBufferAccess)
{
    AEAssert(m_GraphicDevice != nullptr)

    if(m_GraphicDevice == nullptr)
    {
        return AEResult::GraphicDeviceNull;
    }

    CleanUp();

    D3D11_TEXTURE2D_DESC dxDesc = { 0 };
    dxDesc.Width                = width;
    dxDesc.Height               = height;
    AETODO("Check Mip Map Levels in RT");

    dxDesc.MipLevels            = 1;
    dxDesc.ArraySize            = 1;
    dxDesc.Format               = format;

    AETODO("Check Multisampling in RT");
    //dxDesc.SampleDesc;
    dxDesc.Usage                = AEGraphicHelpers::GetDXUsage(graphicBufferUsage);
    dxDesc.BindFlags            = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
    dxDesc.CPUAccessFlags       = AEGraphicHelpers::GetDXBufferAccess(graphicBufferAccess);

    AETODO("Check other flags for RT and if Mip Map is needed");
    dxDesc.MiscFlags            = 0;

    dxDesc.SampleDesc.Count     = 1;
    dxDesc.SampleDesc.Quality   = 0;

    AETODO("Check if we need this RT");
    /*uint32_t pixelSize = AEGraphicHelpers::GetSizeOfDXFormat(format);
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

        return AEResult::CreateTextureFailed;
    }

    AEGraphicHelpers::SetDebugObjectName<ID3D11Texture2D>(m_TextureDX, AE_DEBUG_RT_T_NAME_PREFIX + m_Name);

    AETODO("Check Shader Resource Desc");
    hr = m_GraphicDevice->GetDeviceDX()->CreateShaderResourceView(m_TextureDX, nullptr, &m_ShaderResourceView);

    if(hr != S_OK)
    {
        DisplayError(hr);

        AETODO("Add Log here");
        ReleaseCOM(m_TextureDX);

        return AEResult::CreateSRViewFailed;
    }

    AEGraphicHelpers::SetDebugObjectName<ID3D11ShaderResourceView>(m_ShaderResourceView, AE_DEBUG_RT_SRV_NAME_PREFIX + m_Name);

    AETODO("Check Render Target Desc");
    hr = m_GraphicDevice->GetDeviceDX()->CreateRenderTargetView(m_TextureDX, nullptr, &m_RenderTargetDX);

    if(hr != S_OK)
    {
        DisplayError(hr);

        AETODO("Add Log here");
        ReleaseCOM(m_ShaderResourceView);
        ReleaseCOM(m_TextureDX);

        return AEResult::CreateRTViewFailed;
    }

    AEGraphicHelpers::SetDebugObjectName<ID3D11RenderTargetView>(m_RenderTargetDX, AE_DEBUG_RT_NAME_PREFIX + m_Name);

    m_Width = width;
    m_Height = height;

    return AEResult::Ok;
}
