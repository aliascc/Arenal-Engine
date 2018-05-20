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
#include "Texture2D.h"
#include "Color\Color.h"
#include "TextureDefs.h"
#include "Logger\Logger.h"
#include "GraphicDevice.h"
#include "Base\BaseFunctions.h"
#include "Localization\LocalizationManager.h"

//Always include last
#include "Memory\MemLeaks.h"

/********************
*   Function Defs   *
*********************/
Texture2D::Texture2D(GraphicDevice* graphicDevice, const std::string& textureName)
    : ITexture2D(graphicDevice, textureName)
{
}

Texture2D::~Texture2D()
{
}

void Texture2D::CleanUp()
{
    ReleaseCOM(m_TextureDX);
    ReleaseCOM(m_ShaderResourceView);
}

AEResult Texture2D::CreateTexture(uint32_t width, uint32_t height, DXGI_FORMAT format, GraphicBufferUsage usage, GraphicBufferAccess cpuAccess)
{
    std::lock_guard<std::mutex> lock(m_GameResourceMutex);

    /////////////////////////////////////////////////////////////
    //Pre-check
    AEAssert(m_GraphicDevice != nullptr);
    if (m_GraphicDevice == nullptr)
    {
        return AEResult::GraphicDeviceNull;
    }

    if (width == 0 || height == 0)
    {
        return AEResult::ZeroSize;
    }

    /////////////////////////////////////////////////////////////
    //Clean up memory for new load
    CleanUp();

    m_Filename = "";
    m_FromFile = false;

    m_Height = 0;
    m_Width = 0;

    /////////////////////////////////////////////////////////////
    //Variables
    HRESULT hr = S_OK;

    /////////////////////////////////////////////////////////////
    //Create Texture Array
    D3D11_TEXTURE2D_DESC textDesc;
    memset(&textDesc, 0, sizeof(D3D11_TEXTURE2D_DESC));

    textDesc.Width                = width;
    textDesc.Height               = height;
    textDesc.MipLevels            = 1;
    textDesc.Format               = format;
    textDesc.ArraySize            = 1;
    textDesc.Usage                = AEGraphicHelpers::GetDXUsage(usage);
    textDesc.BindFlags            = D3D11_BIND_SHADER_RESOURCE;
    textDesc.CPUAccessFlags       = AEGraphicHelpers::GetDXBufferAccess(cpuAccess);
    textDesc.MiscFlags            = 0;

    AETODO("Look into this to see if we need to pass this as parameters");
    textDesc.SampleDesc.Count     = 1;
    textDesc.SampleDesc.Quality   = 0;

    hr = m_GraphicDevice->GetDeviceDX()->CreateTexture2D(&textDesc, nullptr, &m_TextureDX);
    if (hr != S_OK)
    {
        DisplayError(hr);

        AETODO("Better return code");
        return AEResult::Fail;
    }

    AEGraphicHelpers::SetDebugObjectName<ID3D11Texture2D>(m_TextureDX, AE_DEBUG_TEX_2D_NAME_PREFIX + m_Name);

    /////////////////////////////////////////////////////////////
    //Create Shader Resource View
    D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
    memset(&srvDesc, 0, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));

    srvDesc.Format                       = format;
    srvDesc.ViewDimension                = D3D_SRV_DIMENSION_TEXTURE2D;

    srvDesc.Texture2D.MipLevels          = 1;

    hr = m_GraphicDevice->GetDeviceDX()->CreateShaderResourceView(m_TextureDX, &srvDesc, &m_ShaderResourceView);
    if (hr != S_OK)
    {
        ReleaseCOM(m_TextureDX);

        DisplayError(hr);

        AETODO("Better return code");
        return AEResult::Fail;
    }

    AEGraphicHelpers::SetDebugObjectName<ID3D11ShaderResourceView>(m_ShaderResourceView, AE_DEBUG_TEX_2D_SRV_NAME_PREFIX + m_Name);

    /////////////////////////////////////////////////////////////
    //Finish
    m_Height = height;
    m_Width = width;

    return AEResult::Ok;
}

AEResult Texture2D::CreateColorTexture(uint32_t width, uint32_t height, const Color& color)
{
    std::lock_guard<std::mutex> lock(m_GameResourceMutex);

    /////////////////////////////////////////////////////////////
    //Pre-check
    AEAssert(m_GraphicDevice != nullptr);
    if (m_GraphicDevice == nullptr)
    {
        return AEResult::GraphicDeviceNull;
    }

    if (width == 0 || height == 0)
    {
        return AEResult::ZeroSize;
    }

    /////////////////////////////////////////////////////////////
    //Clean up memory for new load
    CleanUp();

    m_Filename = "";
    m_FromFile = false;

    m_Height = 0;
    m_Width = 0;

    /////////////////////////////////////////////////////////////
    //Variables
    HRESULT hr = S_OK;

    /////////////////////////////////////////////////////////////
    //Create Texture Array
    D3D11_TEXTURE2D_DESC textDesc;
    memset(&textDesc, 0, sizeof(D3D11_TEXTURE2D_DESC));

    textDesc.Width            = width;
    textDesc.Height           = height;
    textDesc.MipLevels        = 1;
    textDesc.Format           = DXGI_FORMAT_R8G8B8A8_UNORM;
    textDesc.ArraySize        = 1;
    textDesc.Usage            = AEGraphicHelpers::GetDXUsage(GraphicBufferUsage::Default);
    textDesc.BindFlags        = D3D11_BIND_SHADER_RESOURCE;
    textDesc.CPUAccessFlags   = AEGraphicHelpers::GetDXBufferAccess(GraphicBufferAccess::None);
    textDesc.MiscFlags        = 0;

    AETODO("Look into this to see if we need to pass this as parameters");
    textDesc.SampleDesc.Count    = 1;
    textDesc.SampleDesc.Quality    = 0;

    uint32_t totalSize        = width * height;
    uint32_t* pixelData        = new uint32_t[totalSize];

    for (uint32_t i = 0; i < totalSize; i++)
    {
        pixelData[i] = color.GetColorRGBA();
    }

    D3D11_SUBRESOURCE_DATA subResourceData;
    subResourceData.pSysMem             = pixelData;
    subResourceData.SysMemSlicePitch    = sizeof(uint32_t) * width * height;
    subResourceData.SysMemPitch         = sizeof(uint32_t) * width;

    hr = m_GraphicDevice->GetDeviceDX()->CreateTexture2D(&textDesc, &subResourceData, &m_TextureDX);

    DeleteMemArr(pixelData);

    if (hr != S_OK)
    {
        DisplayError(hr);

        AETODO("Better return code");
        return AEResult::Fail;
    }

    AEGraphicHelpers::SetDebugObjectName<ID3D11Texture2D>(m_TextureDX, AE_DEBUG_TEX_2D_NAME_PREFIX + m_Name);

    /////////////////////////////////////////////////////////////
    //Create Shader Resource View
    D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
    memset(&srvDesc, 0, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));

    srvDesc.Format           = DXGI_FORMAT_R8G8B8A8_UNORM;
    srvDesc.ViewDimension    = D3D_SRV_DIMENSION_TEXTURE2D;

    srvDesc.Texture2D.MipLevels = 1;

    hr = m_GraphicDevice->GetDeviceDX()->CreateShaderResourceView(m_TextureDX, &srvDesc, &m_ShaderResourceView);
    if (hr != S_OK)
    {
        ReleaseCOM(m_TextureDX);

        DisplayError(hr);

        AETODO("Better return code");
        return AEResult::Fail;
    }

    AEGraphicHelpers::SetDebugObjectName<ID3D11ShaderResourceView>(m_ShaderResourceView, AE_DEBUG_TEX_2D_SRV_NAME_PREFIX + m_Name);

    /////////////////////////////////////////////////////////////
    //Finish
    m_Height = height;
    m_Width = width;

    return AEResult::Ok;
}

AEResult Texture2D::CreateFromFile(const std::string& textureFile)
{
    m_Filename = textureFile;
    m_FromFile = true;

    return Load();
}

AEResult Texture2D::Load()
{
    //This has to be an atomic operation
    std::lock_guard<std::mutex> lock(m_GameResourceMutex);

    /////////////////////////////////////////////////////////////
    //Pre-check
    AEAssert(m_GraphicDevice != nullptr);
    if (m_GraphicDevice == nullptr)
    {
        return AEResult::GraphicDeviceNull;
    }

    AEAssert(!m_Filename.empty());
    if (m_Filename.empty())
    {
        return AEResult::EmptyFilename;
    }

    /////////////////////////////////////////////////////////////
    //Verify we are loading a file
    if(!m_FromFile)
    {
        AETODO("Better return code");
        return AEResult::Fail;
    }

    /////////////////////////////////////////////////////////////
    //Clean up memory for new load
    CleanUp();

    m_Height = 0;
    m_Width = 0;

    /////////////////////////////////////////////////////////////
    //Variables
    HRESULT hr = S_OK;
    DirectX::TexMetadata texMetadata;
    DirectX::ScratchImage scratchImage;
    std::wstring fileNameW = AE_Base::String2WideStr(m_Filename);

    /////////////////////////////////////////////////////////////
    //Use DirectX Tex to get meta data from file
    hr = GetMetadataFromDDSFile(fileNameW.c_str(), DirectX::DDS_FLAGS_NONE, texMetadata);

    if(hr != S_OK)
    {
        DisplayError(hr);

        return AEResult::Fail;
    }

    /////////////////////////////////////////////////////////////
    //Verify that this is a 2D Texture
    if (texMetadata.dimension != DirectX::TEX_DIMENSION_TEXTURE2D && !texMetadata.IsCubemap() && texMetadata.arraySize != 1)
    {
        return AEResult::InvalidTextureType;
    }

    /////////////////////////////////////////////////////////////
    //Load file with DirectX Tex
    hr = LoadFromDDSFile(fileNameW.c_str(), DirectX::DDS_FLAGS_NONE, &texMetadata, scratchImage);

    if(hr != S_OK)
    {
        DisplayError(hr);

        return AEResult::Fail;
    }

    /////////////////////////////////////////////////////////////
    //Get DirectX Texture2D Resource from DirectX Tex Objects
    ID3D11Resource* resource = nullptr;
    hr = CreateTextureEx(m_GraphicDevice->GetDeviceDX(), scratchImage.GetImages(), scratchImage.GetImageCount(), texMetadata, D3D11_USAGE_DEFAULT, D3D11_BIND_SHADER_RESOURCE, 0, 0, false, &resource);
    
    if(hr != S_OK)
    {
        DisplayError(hr);

        AETODO("Better return code");
        return AEResult::Fail;
    }

    m_TextureDX = reinterpret_cast<ID3D11Texture2D*>(resource);

    AEGraphicHelpers::SetDebugObjectName<ID3D11Texture2D>(m_TextureDX, AE_DEBUG_TEX_2D_NAME_PREFIX + m_Name);

    /////////////////////////////////////////////////////////////
    //Create Shader Resource View
    D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
    memset(&srvDesc, 0, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));

    srvDesc.Format                    = texMetadata.format;
    srvDesc.ViewDimension            = D3D_SRV_DIMENSION_TEXTURE2D;
    srvDesc.Texture2D.MipLevels        = static_cast<UINT>(texMetadata.mipLevels);

    hr = m_GraphicDevice->GetDeviceDX()->CreateShaderResourceView(m_TextureDX, &srvDesc, &m_ShaderResourceView);

    if (hr != S_OK)
    {
        ReleaseCOM(m_TextureDX);

        DisplayError(hr);

        AETODO("Better return code");
        return AEResult::Fail;
    }

    AEGraphicHelpers::SetDebugObjectName<ID3D11ShaderResourceView>(m_ShaderResourceView, AE_DEBUG_TEX_2D_SRV_NAME_PREFIX + m_Name);

    /////////////////////////////////////////////////////////////
    //Set Width and Height
    m_Height = static_cast<uint32_t>(texMetadata.height);
    m_Width = static_cast<uint32_t>(texMetadata.width);

    return AEResult::Ok;
}
