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
#include "SimpleBuffer.h"
#include "GraphicDevice.h"

//Always include last
#include "Memory\MemLeaks.h"

/********************
*   Function Defs   *
*********************/
SimpleBuffer::SimpleBuffer(const std::string& name, uint32_t bindIndex, bool createAsRW, ShaderVariableClass variableClass,  ShaderVariableType variableType, uint32_t elementCount, GraphicDevice* graphicDevice)
    : ShaderBuffer(name, bindIndex, createAsRW, graphicDevice)
    , m_VariableClass(variableClass)
    , m_VariableType(variableType)
    , m_ElementCount(elementCount)
{
    AEResult ret = AEResult::Ok;

    ret = CheckShaderVariableInfo(variableClass, variableType, elementCount);
    if(ret != AEResult::Ok)
    {
        AEAssert(false);
        AETODO("Log Error");
    }
    else
    {
        m_IsStartReady = true;
    }
}

SimpleBuffer::~SimpleBuffer()
{
}

AEResult SimpleBuffer::CheckShaderVariableInfo(ShaderVariableClass variableClass,  ShaderVariableType variableType, uint32_t elementCount)
{
    AETODO("Add check if double has more than 2 elements we need to cancel");
    AEAssert(elementCount > 0 && elementCount < 5);
    if(elementCount <= 0 || elementCount >= 5)
    {
        return AEResult::OutsideRange;
    }

    switch (variableClass)
    {
        case ShaderVariableClass::Scalar:
        case ShaderVariableClass::Vector:
            //We accept these Variable Classes
            break;
        default:
            AEAssert(false);
            return AEResult::InvalidObjType;
    }

    switch (variableType)
    {
        case ShaderVariableType::Int:
        case ShaderVariableType::UInt:
        case ShaderVariableType::Float:
        case ShaderVariableType::Double:
            //We accept these Variable Types
            break;
        default:
            AEAssert(false);
            return AEResult::InvalidObjType;
    }

    if(elementCount > 1 && variableClass != ShaderVariableClass::Vector)
    {
        AETODO("Better return");
        return AEResult::InvalidObjType;
    }

    return AEResult::Ok;
}

DXGI_FORMAT SimpleBuffer::GetDXGIFormat()
{
    AEAssert(m_IsStartReady)
    if(!m_IsStartReady)
    {
        AETODO("Log error");
        return DXGI_FORMAT_UNKNOWN;
    }

    bool err = false;

    switch (m_VariableType)
    {
        case ShaderVariableType::Int:
            {
                switch (m_ElementCount)
                {
                    case 1:
                        return DXGI_FORMAT_R32_SINT;

                    case 2:
                        return DXGI_FORMAT_R32G32_SINT;

                    case 3:
                        return DXGI_FORMAT_R32G32B32_SINT;

                    case 4:
                        return DXGI_FORMAT_R32G32B32A32_SINT;

                    default:
                        break;
                }
            }
            break;

        case ShaderVariableType::UInt:
            {
                switch (m_ElementCount)
                {
                    case 1:
                        return DXGI_FORMAT_R32_UINT;

                    case 2:
                        return DXGI_FORMAT_R32G32_UINT;

                    case 3:
                        return DXGI_FORMAT_R32G32B32_UINT;

                    case 4:
                        return DXGI_FORMAT_R32G32B32A32_UINT;

                    default:
                        break;
                }
            }
            break;

        case ShaderVariableType::Float:
            {
                switch (m_ElementCount)
                {
                    case 1:
                        return DXGI_FORMAT_R32_FLOAT;

                    case 2:
                        return DXGI_FORMAT_R32G32_FLOAT;

                    case 3:
                        return DXGI_FORMAT_R32G32B32_FLOAT;

                    case 4:
                        return DXGI_FORMAT_R32G32B32A32_FLOAT;

                    default:
                        break;
                }
            }
            break;

        case ShaderVariableType::Double:
            {
                switch (m_ElementCount)
                {
                    case 1:
                        return DXGI_FORMAT_R32G32_FLOAT;

                    case 2:
                        return DXGI_FORMAT_R32G32B32A32_FLOAT;

                    default:
                        break;
                }
            }
            break;

        default:
            break;
    }

    AETODO("Log error");
    return DXGI_FORMAT_UNKNOWN;
}

AEResult SimpleBuffer::Deinitialize()
{
    m_IsReady        = false;
    m_NumElements    = 0;
    m_BufferUsage    = GraphicBufferUsage::Default;
    m_BufferAccess   = GraphicBufferAccess::None;

    CleanUp();

    return AEResult::Ok;
}

AEResult SimpleBuffer::InitializeBuffer(uint32_t numElements, GraphicBufferUsage bufferUsage, GraphicBufferAccess bufferAccess)
{
    AEAssert(m_IsStartReady)
    if(!m_IsStartReady)
    {
        AETODO("better return error");
        return AEResult::Fail;
    }

    AEAssert(m_GraphicDevice != nullptr);
    if(m_GraphicDevice == nullptr)
    {
        return AEResult::GraphicDeviceNull;
    }

    AEAssert(numElements != 0);
    if(numElements == 0)
    {
        return AEResult::ZeroSize;
    }

    /******************************************
    *1. Verify if buffer has been initialize 
    *   and remove it is
    /******************************************/
    Deinitialize();

    /******************************************
    *2. Create VRAM Buffer
    /******************************************/
    D3D11_BUFFER_DESC cbDesc = { 0 };

    DXGI_FORMAT dxFormat = GetDXGIFormat();

    UINT bindFlagsDX = D3D11_BIND_SHADER_RESOURCE;

    if(m_IsRWEnabled)
    {
        bindFlagsDX |= D3D11_BIND_UNORDERED_ACCESS;
    }

    cbDesc.ByteWidth            = (ELEMENT_SIZE_SIMPLE_BUFFER * m_ElementCount) * numElements;
    cbDesc.Usage                = AEGraphicHelpers::GetDXUsage(bufferUsage);
    cbDesc.BindFlags            = bindFlagsDX;
    cbDesc.CPUAccessFlags       = AEGraphicHelpers::GetDXBufferAccess(bufferAccess);
    cbDesc.StructureByteStride  = (ELEMENT_SIZE_SIMPLE_BUFFER * m_ElementCount);
    cbDesc.MiscFlags            = 0;

    HRESULT hr = m_GraphicDevice->GetDeviceDX()->CreateBuffer(&cbDesc, nullptr, &m_BufferDX);

    if(hr != S_OK)
    {
        DisplayError(hr);

        return AEResult::SimpleBufferInitFailed;
    }

    AEGraphicHelpers::SetDebugObjectName<ID3D11Buffer>(m_BufferDX, AE_DEBUG_B_NAME_PREFIX + m_Name);

    /******************************************
    *3. Create SRV for Buffer
    /******************************************/
    D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
    memset(&srvDesc, 0, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));

    srvDesc.Format                   = dxFormat;
    srvDesc.ViewDimension            = D3D11_SRV_DIMENSION_BUFFER;

    srvDesc.Buffer.FirstElement      = 0;
    srvDesc.Buffer.NumElements       = numElements;

    hr = m_GraphicDevice->GetDeviceDX()->CreateShaderResourceView(m_BufferDX, &srvDesc, &m_BufferSRVDX);

    if(hr != S_OK)
    {
        DisplayError(hr);

        ReleaseCOM(m_BufferDX);

        return AEResult::SimpleBufferInitFailed;
    }

    AEGraphicHelpers::SetDebugObjectName<ID3D11ShaderResourceView>(m_BufferSRVDX, AE_DEBUG_B_SRV_NAME_PREFIX + m_Name);

    /******************************************
    *4. Create UAV for Buffer if flag is set
    /******************************************/
    if(m_IsRWEnabled)
    {
        D3D11_UNORDERED_ACCESS_VIEW_DESC uavDesc;
        memset(&uavDesc, 0, sizeof(D3D11_UNORDERED_ACCESS_VIEW_DESC));

        uavDesc.Format                   = dxFormat;
        uavDesc.ViewDimension            = D3D11_UAV_DIMENSION_BUFFER;

        uavDesc.Buffer.FirstElement      = 0;
        uavDesc.Buffer.NumElements       = numElements;
        uavDesc.Buffer.Flags             = 0;

        hr = m_GraphicDevice->GetDeviceDX()->CreateUnorderedAccessView(m_BufferDX, &uavDesc, &m_BufferUAVDX);

        if(hr != S_OK)
        {
            DisplayError(hr);

            ReleaseCOM(m_BufferSRVDX);
            ReleaseCOM(m_BufferDX);

            return AEResult::SimpleBufferInitFailed;
        }

        AEGraphicHelpers::SetDebugObjectName<ID3D11ShaderResourceView>(m_BufferSRVDX, AE_DEBUG_B_UAV_NAME_PREFIX + m_Name);
    }

    /******************************************
    *5. Finish
    /******************************************/
    m_IsReady        = true;
    m_NumElements    = numElements;
    m_BufferUsage    = bufferUsage;
    m_BufferAccess   = bufferAccess;

    return AEResult::Ok;
}

AEResult SimpleBuffer::UpdateBuffer(void* data, uint32_t numElements, uint32_t startIndex, uint32_t offset, GraphicResourceMap resourceMap)
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
    if(startIndex >= m_NumElements || (startIndex + numElements) > m_NumElements)
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
    uint32_t copySize = (ELEMENT_SIZE_SIMPLE_BUFFER * m_ElementCount) * numElements;
    uint32_t offsetSize =  (ELEMENT_SIZE_SIMPLE_BUFFER * m_ElementCount) * offset;

    memcpy(mappedMemory + offsetSize, data, copySize);

    /******************************************
    *4. Unmap Resource
    /******************************************/
    m_GraphicDevice->GetDeviceContextDX()->Unmap(m_BufferDX, 0);

    return AEResult::Ok;
}
