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
#include "StructuredBuffer.h"
#include "Base\BaseFunctions.h"
#include "Localization\LocalizationManager.h"

//Always include last
#include "Memory\MemLeaks.h"

/********************
*   Function Defs   *
*********************/
StructuredBuffer::StructuredBuffer(const std::wstring& name, uint32_t bindIndex, bool createAsRW, GraphicDevice* graphicDevice)
    : ShaderBuffer(name, bindIndex, createAsRW, graphicDevice)
{
}

StructuredBuffer::~StructuredBuffer()
{
}

AEResult StructuredBuffer::Deinitialize()
{
    m_IsReady       = false;
    m_NumElements   = 0;
    m_StructSize    = 0;
    m_BufferUsage   = GraphicBufferUsage::Default;
    m_BufferAccess  = GraphicBufferAccess::None;

    CleanUp();

    return AEResult::Ok;
}

AEResult StructuredBuffer::InitializeBuffer(uint32_t structSize, uint32_t numElements, GraphicBufferUsage bufferUsage, GraphicBufferAccess bufferAccess)
{
    AEAssert(m_GraphicDevice != nullptr);
    if(m_GraphicDevice == nullptr)
    {
        return AEResult::GraphicDeviceNull;
    }

    AEAssert(structSize != 0);
    AEAssert(numElements != 0);
    if(structSize == 0 || numElements == 0)
    {
        return AEResult::ZeroSize;
    }

    /******************************************
    *1. Verify if buffer has been initialize 
    *   and remove it is
    /******************************************/
    Deinitialize();

    /******************************************
    *2. Create VRAM Structured Buffer
    /******************************************/
    D3D11_BUFFER_DESC cbDesc = { 0 };

    UINT bindFlagsDX = D3D11_BIND_SHADER_RESOURCE;

    if(m_IsRWEnabled)
    {
        bindFlagsDX |= D3D11_BIND_UNORDERED_ACCESS;
    }

    cbDesc.ByteWidth            = structSize * numElements;
    cbDesc.Usage                = AEGraphicHelpers::GetDXUsage(bufferUsage);
    cbDesc.BindFlags            = bindFlagsDX;
    cbDesc.CPUAccessFlags       = AEGraphicHelpers::GetDXBufferAccess(bufferAccess);
    cbDesc.StructureByteStride  = structSize;
    cbDesc.MiscFlags            = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;

    HRESULT hr = m_GraphicDevice->GetDeviceDX()->CreateBuffer(&cbDesc, nullptr, &m_BufferDX);

    if(hr != S_OK)
    {
        DisplayError(hr);

        return AEResult::StructuredBufferInitFailed;
    }

    AEGraphicHelpers::SetDebugObjectName<ID3D11Buffer>(m_BufferDX, AE_DEBUG_SB_NAME_PREFIX + AE_Base::WideStr2String(m_Name));

    /******************************************
    *3. Create SRV for Structured Buffer
    /******************************************/
    D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
    memset(&srvDesc, 0, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));

    srvDesc.Format                   = DXGI_FORMAT_UNKNOWN;
    srvDesc.ViewDimension            = D3D11_SRV_DIMENSION_BUFFEREX;
    srvDesc.BufferEx.FirstElement    = 0;
    srvDesc.BufferEx.Flags           = 0;
    srvDesc.BufferEx.NumElements     = numElements;

    hr = m_GraphicDevice->GetDeviceDX()->CreateShaderResourceView(m_BufferDX, &srvDesc, &m_BufferSRVDX);

    if(hr != S_OK)
    {
        DisplayError(hr);

        ReleaseCOM(m_BufferDX);

        return AEResult::StructuredBufferInitFailed;
    }

    AEGraphicHelpers::SetDebugObjectName<ID3D11ShaderResourceView>(m_BufferSRVDX, AE_DEBUG_SB_SRV_NAME_PREFIX + AE_Base::WideStr2String(m_Name));

    /******************************************
    *4. Create UAV for Buffer if flag is set
    /******************************************/
    if(m_IsRWEnabled)
    {
        D3D11_UNORDERED_ACCESS_VIEW_DESC uavDesc;
        memset(&uavDesc, 0, sizeof(D3D11_UNORDERED_ACCESS_VIEW_DESC));

        uavDesc.Buffer.FirstElement     = 0;
        uavDesc.Buffer.NumElements      = numElements;
        uavDesc.Buffer.Flags            = 0;

        uavDesc.Format                  = DXGI_FORMAT_UNKNOWN;
        uavDesc.ViewDimension           = D3D11_UAV_DIMENSION_BUFFER;

        hr = m_GraphicDevice->GetDeviceDX()->CreateUnorderedAccessView(m_BufferDX, &uavDesc, &m_BufferUAVDX);

        if(hr != S_OK)
        {
            DisplayError(hr);

            ReleaseCOM(m_BufferSRVDX);
            ReleaseCOM(m_BufferDX);

            return AEResult::StructuredBufferInitFailed;
        }

        AEGraphicHelpers::SetDebugObjectName<ID3D11ShaderResourceView>(m_BufferSRVDX, AE_DEBUG_SB_UAV_NAME_PREFIX + AE_Base::WideStr2String(m_Name));
    }

    /******************************************
    *5. Finish
    /******************************************/
    m_IsReady       = true;
    m_NumElements   = numElements;
    m_StructSize    = structSize;
    m_BufferUsage   = bufferUsage;
    m_BufferAccess  = bufferAccess;

    return AEResult::Ok;
}

AEResult StructuredBuffer::UpdateBuffer(const void* data, uint32_t elementCount, uint32_t startIndex, uint32_t offset, GraphicResourceMap resourceMap)
{
    if(!m_IsReady)
    {
        return AEResult::NotReady;
    }

    AEAssert(data != nullptr);
    if(data == nullptr)
    {
        return AEResult::NullParameter;
    }

    AEAssert(resourceMap == GraphicResourceMap::Write || resourceMap == GraphicResourceMap::WriteDiscard);
    if(resourceMap != GraphicResourceMap::Write && resourceMap != GraphicResourceMap::WriteDiscard)
    {
        return AEResult::InvalidResourceMapType;
    }

    BYTE* mappedMemory = nullptr;
    AEResult ret = AEResult::Ok;

    /******************************************
    *1. Verify Constraints
    /******************************************/
    if(startIndex >= m_NumElements || (startIndex + elementCount) > m_NumElements)
    {
        return AEResult::OutsideRange;
    }

    /******************************************
    *2. Map Resource to RAM
    /******************************************/
    D3D11_MAPPED_SUBRESOURCE mappedData = { 0 };

    D3D11_MAP dxMapType = AEGraphicHelpers::GetDXResourceMap(resourceMap);

    HRESULT hr = m_GraphicDevice->GetDeviceContextDX()->Map(m_BufferDX, 0, dxMapType, 0, &mappedData);

    if(hr != S_OK)
    {
        DisplayError(hr);

        return AEResult::ResourceMapFailed;
    }

    mappedMemory = reinterpret_cast<BYTE*>(mappedData.pData);

    /******************************************
    *3. Copy to VRAM
    /******************************************/
    uint32_t copySize = m_StructSize * elementCount;
    uint32_t offsetSize = m_StructSize * offset;

    memcpy(mappedMemory + offsetSize, data, copySize);

    /******************************************
    *4. Unmap Resource
    /******************************************/
    m_GraphicDevice->GetDeviceContextDX()->Unmap(m_BufferDX, 0);

    return AEResult::Ok;
}
