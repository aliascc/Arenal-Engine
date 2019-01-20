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
#include "TextureCube.h"
#include "TextureDefs.h"
#include "GraphicDevice.h"

//Always include last
#include "Memory\MemLeaks.h"

/********************
*   Function Defs   *
*********************/
TextureCube::TextureCube(GraphicDevice& graphicDevice, const std::string& textureName)
    : ITexture2D(graphicDevice, textureName, true)
{
}

TextureCube::~TextureCube()
{
}

void TextureCube::CleanUp()
{
    ReleaseCOM(m_TextureDX);
    ReleaseCOM(m_ShaderResourceView);
}

AEResult TextureCube::Load()
{
    //This has to be an atomic operation
    std::lock_guard<std::mutex> lock(m_GameResourceMutex);

    /////////////////////////////////////////////////////////////
    //Verify we are loading a file
    if (!m_FromFile)
    {
        AETODO("Better return code");
        return AEResult::Fail;
    }

    /////////////////////////////////////////////////////////////
    //Clean up memory for new load
    CleanUp();

    m_Height = 0;
    m_Width = 0;
    m_Depth = 0;

    /////////////////////////////////////////////////////////////
    //Variables
    HRESULT hr = S_OK;
    DirectX::TexMetadata texMetadata;
    DirectX::ScratchImage scratchImage;
    std::wstring filenameW = AE_Base::String2WideStr(m_Filename);

    /////////////////////////////////////////////////////////////
    //Use DirectX Tex to get meta data from file
    hr = GetMetadataFromDDSFile(filenameW.c_str(), DirectX::DDS_FLAGS_NONE, texMetadata);

    if (hr != S_OK)
    {
        DisplayError(hr);

        return AEResult::Fail;
    }

    /////////////////////////////////////////////////////////////
    //Verify that this is a 2D Texture
    if (texMetadata.dimension != DirectX::TEX_DIMENSION_TEXTURE2D && texMetadata.IsCubemap() && texMetadata.arraySize != 6)
    {
        return AEResult::InvalidTextureType;
    }

    /////////////////////////////////////////////////////////////
    //Load file with DirectX Tex
    hr = LoadFromDDSFile(filenameW.c_str(), DirectX::DDS_FLAGS_NONE, &texMetadata, scratchImage);

    if (hr != S_OK)
    {
        DisplayError(hr);

        return AEResult::Fail;
    }

    /////////////////////////////////////////////////////////////
    //Get DirectX Texture2D Resource from DirectX Tex Objects
    ID3D11Resource* resource = nullptr;
    hr = CreateTextureEx(m_GraphicDevice.GetDeviceDX(), scratchImage.GetImages(), scratchImage.GetImageCount(), texMetadata, D3D11_USAGE_DEFAULT, D3D11_BIND_SHADER_RESOURCE, 0, 0, false, &resource);

    if (hr != S_OK)
    {
        DisplayError(hr);

        AETODO("Better return code");
        return AEResult::Fail;
    }

    m_TextureDX = reinterpret_cast<ID3D11Texture2D*>(resource);

    AEGraphicHelpers::SetDebugObjectName(m_TextureDX, AE_DEBUG_TEX_CUBE_NAME_PREFIX + m_Name);

    /////////////////////////////////////////////////////////////
    //Create Shader Resource View
    D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
    memset(&srvDesc, 0, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));

    srvDesc.Format = texMetadata.format;
    srvDesc.ViewDimension = D3D_SRV_DIMENSION_TEXTURECUBE;
    srvDesc.TextureCube.MipLevels = static_cast<UINT>(texMetadata.mipLevels);

    hr = m_GraphicDevice.GetDeviceDX()->CreateShaderResourceView(m_TextureDX, &srvDesc, &m_ShaderResourceView);

    if (hr != S_OK)
    {
        ReleaseCOM(m_TextureDX);

        DisplayError(hr);

        AETODO("Better return code");
        return AEResult::Fail;
    }

    AEGraphicHelpers::SetDebugObjectName(m_ShaderResourceView, AE_DEBUG_TEX_CUBE_SRV_NAME_PREFIX + m_Name);

    /////////////////////////////////////////////////////////////
    //Set Width and Height
    m_Height = static_cast<uint32_t>(texMetadata.height);
    m_Width = static_cast<uint32_t>(texMetadata.width);
    m_Depth = static_cast<uint32_t>(texMetadata.depth);

    return AEResult::Ok;
}

AEResult TextureCube::CreateFromFile(const std::string& textureFile)
{
    m_Filename = textureFile;
    m_FromFile = true;

    return Load();
}
