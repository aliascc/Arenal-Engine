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
#include "IndexBuffer.h"
#include "GraphicDevice.h"

//Always include last
#include "Memory\MemLeaks.h"

/********************
*   Function Defs   *
*********************/
IndexBuffer::IndexBuffer(GraphicDevice& graphicDevice, GraphicBufferUsage bufferUsage, GraphicBufferAccess bufferAccess)
    : Named("")
    , m_BufferAccess(bufferAccess)
    , m_BufferUsage(bufferUsage)
    , m_GraphicDevice(graphicDevice)
{
}

IndexBuffer::~IndexBuffer()
{
    CleanUpIB();

    ReleaseCOM(m_IndexBufferDX);
}

void IndexBuffer::CleanUpIB()
{
    DeleteMemArr(m_IndexBuffer_32);
    DeleteMemArr(m_IndexBuffer_16);
}

void IndexBuffer::CopyToIndexBuffer(uint32_t indexBuffer[], uint32_t size)
{
    AEAssert(indexBuffer != nullptr);

    if(indexBuffer == nullptr)
    {
        return;
    }

    //Delete Any Index Buffer 
    CleanUpIB();

    m_IndexBuffer_32 = new uint32_t[size];
    m_IBType = IndexBufferType::Index32;
    m_Size = size;

    memcpy(m_IndexBuffer_32, indexBuffer, sizeof(uint32_t) * size);

    m_IsReady = false;
}

void IndexBuffer::CopyToIndexBuffer(uint16_t indexBuffer[], uint32_t size)
{
    AEAssert(indexBuffer != nullptr);

    if(indexBuffer == nullptr)
    {
        return;
    }

    //Delete Any Index Buffer 
    CleanUpIB();

    m_IndexBuffer_16 = new uint16_t[size];
    m_IBType = IndexBufferType::Index16;
    m_Size = size;

    memcpy(m_IndexBuffer_16, indexBuffer, sizeof(uint16_t) * size);

    m_IsReady = false;
}

void IndexBuffer::CopyToIndexBuffer(const std::vector<uint32_t>& indexBuffer)
{
    //Delete Any Index Buffer 
    CleanUpIB();

    uint32_t ibSize = (uint32_t)indexBuffer.size();

    m_IndexBuffer_32 = new uint32_t[ibSize];
    m_IBType = IndexBufferType::Index32;
    m_Size = ibSize;

    for(uint32_t i = 0; i < ibSize; ++i)
    {
        m_IndexBuffer_32[i] = indexBuffer[i];
    }

    m_IsReady = false;
}

void IndexBuffer::CopyToIndexBuffer(const std::vector<uint16_t>& indexBuffer)
{
    //Delete Any Index Buffer 
    CleanUpIB();

    uint32_t ibSize = (uint32_t)indexBuffer.size();

    m_IndexBuffer_16 = new uint16_t[ibSize];
    m_IBType = IndexBufferType::Index16;
    m_Size = ibSize;

    for(uint32_t i = 0; i < ibSize; ++i)
    {
        m_IndexBuffer_16[i] = indexBuffer[i];
    }

    m_IsReady = false;
}

AEResult IndexBuffer::BuildIndexBuffer()
{
    if(m_IsReady)
    {
        return AEResult::Ok;
    }

    if(m_Size == 0)
    {
        return AEResult::ZeroSize;
    }

    if(m_IndexBuffer_16 == nullptr && m_IBType == IndexBufferType::Index16)
    {
        return AEResult::NullObj;
    }

    if(m_IndexBuffer_32 == nullptr && m_IBType == IndexBufferType::Index32)
    {
        return AEResult::NullObj;
    }

    //Release DirectX Vertex Buffer if it has been created previously
    ReleaseCOM(m_IndexBufferDX);
    m_IsReady = false;

    D3D11_BUFFER_DESC ibDesc = { 0 };

    ibDesc.Usage                = AEGraphicHelpers::GetDXUsage(m_BufferUsage);
    ibDesc.ByteWidth            = GetIndexSize() * m_Size;
    ibDesc.BindFlags            = D3D11_BIND_INDEX_BUFFER;
    ibDesc.CPUAccessFlags       = AEGraphicHelpers::GetDXBufferAccess(m_BufferAccess);
    ibDesc.MiscFlags            = 0;
    ibDesc.StructureByteStride  = 0;

    D3D11_SUBRESOURCE_DATA ibInitData = { 0 };

    switch (m_IBType)
    {
        case IndexBufferType::Index16:
            ibInitData.pSysMem = m_IndexBuffer_16;
            break;

        case IndexBufferType::Index32:
            ibInitData.pSysMem = m_IndexBuffer_32;
            break;

        default:
            //Should never get here
            AEAssert(false);
            break;
    }

    HRESULT hr = m_GraphicDevice.GetDeviceDX()->CreateBuffer(&ibDesc, &ibInitData, &m_IndexBufferDX);

    if(hr != S_OK)
    {
        DisplayError(hr);

        return AEResult::FailIndexBufferInit;
    }

    AEGraphicHelpers::SetDebugObjectName<ID3D11Buffer>(m_IndexBufferDX, AE_DEBUG_IB_NAME_PREFIX + m_Name);

    m_IsReady = true;

    return AEResult::Ok;
}

AEResult IndexBuffer::UpdateIndexBuffer(uint32_t offset, uint16_t indexData[], uint32_t startIndex, uint32_t elementCount, GraphicResourceMap resourceMap)
{
    return UpdateIndexBufferPrivate(IndexBufferType::Index16, offset, indexData, startIndex, elementCount, resourceMap);
}

AEResult IndexBuffer::UpdateIndexBuffer(uint32_t offset, uint32_t indexData[], uint32_t startIndex, uint32_t elementCount, GraphicResourceMap resourceMap)
{
    return UpdateIndexBufferPrivate(IndexBufferType::Index32, offset, indexData, startIndex, elementCount, resourceMap);
}

AEResult IndexBuffer::UpdateIndexBufferPrivate(IndexBufferType IBType, uint32_t offset, void* indexData, uint32_t startIndex, uint32_t elementCount, GraphicResourceMap resourceMap)
{
    AEAssert(resourceMap == GraphicResourceMap::Write || resourceMap == GraphicResourceMap::WriteDiscard || resourceMap == GraphicResourceMap::WriteNoOvewrite);
    if(resourceMap != GraphicResourceMap::Write && resourceMap != GraphicResourceMap::WriteDiscard && resourceMap != GraphicResourceMap::WriteNoOvewrite)
    {
        return AEResult::InvalidResourceMapType;
    }

    AEAssert(indexData != nullptr);
    if(indexData == nullptr)
    {
        return AEResult::NullParameter;
    }

    AEAssert(IBType == m_IBType);
    if(IBType != m_IBType)
    {
        return AEResult::InvalidIndexBufferType;
    }

    if(!m_IsReady)
    {
        if(BuildIndexBuffer() != AEResult::Ok)
        {
            return AEResult::FailIndexBufferInit;
        }
    }

    if(startIndex >= m_Size || (startIndex + elementCount) > m_Size)
    {
        return AEResult::OutsideRange;
    }

    AEResult ret = AEResult::Ok;

    ret = MapBuffer(resourceMap);

    if(ret != AEResult::Ok)
    {
        return ret;
    }

    uint32_t copySize = GetIndexSize() * elementCount;

    switch(m_IBType)
    {
        case IndexBufferType::Index16:
            {
                uint16_t* indexData16 = reinterpret_cast<uint16_t*>(indexData);
                memcpy(m_IndexBuffer_16 + startIndex, indexData16 + offset, copySize);
            }
            break;

        case IndexBufferType::Index32:
            {
                uint32_t* indexData32 = reinterpret_cast<uint32_t*>(indexData);
                memcpy(m_IndexBuffer_32 + startIndex, indexData32 + offset, copySize);
            }
            break;

        default:
            //Should never get here
            AEAssert(false);
            break;
    }

    ret = CommitIndexData(startIndex, elementCount);

    UnMapBuffer();

    if(ret != AEResult::Ok)
    {
        return AEResult::FailIndexBufferUpdate;
    }

    return AEResult::Ok;
}

AEResult IndexBuffer::MapBuffer(GraphicResourceMap resourceMap)
{
    if(!m_IsReady)
    {
        return AEResult::NotReady;
    }

    D3D11_MAPPED_SUBRESOURCE mappedData = { 0 };

    D3D11_MAP dxMapType = AEGraphicHelpers::GetDXResourceMap(resourceMap);

    AETODO("Check Map flags");
    AETODO("Check Sub Resource");
    HRESULT hr = m_GraphicDevice.GetDeviceContextDX()->Map(m_IndexBufferDX, 0, dxMapType, 0, &mappedData);

    if(hr != S_OK)
    {
        DisplayError(hr);

        return AEResult::ResourceMapFailed;
    }

    switch (m_IBType)
    {
        case IndexBufferType::Index16:
            m_VideoIB_16 = static_cast<uint16_t*>(mappedData.pData);
            break;

        case IndexBufferType::Index32:
            m_VideoIB_32 = static_cast<uint32_t*>(mappedData.pData);
            break;

        default:
            //Should never get here
            AEAssert(false);
            break;
    }

    return AEResult::Ok;
}

AEResult IndexBuffer::UnMapBuffer()
{
    AETODO("Check subresource")
    m_GraphicDevice.GetDeviceContextDX()->Unmap(m_IndexBufferDX, 0);

    //Set VideoIB to nullptr
    m_VideoIB_32 = nullptr;
    m_VideoIB_16 = nullptr;

    return AEResult::Ok;
}

AEResult IndexBuffer::CommitIndexData(uint32_t offset, uint32_t elementCount)
{
    uint32_t copySize = GetIndexSize() * elementCount;

    if(m_VideoIB_16 == nullptr && m_VideoIB_32 == nullptr)
    {
        return AEResult::NullObj;
    }

    switch (m_IBType)
    {
        case IndexBufferType::Index16:
            memcpy(m_VideoIB_16 + offset, m_IndexBuffer_16 + offset, copySize);
            break;

        case IndexBufferType::Index32:
            memcpy(m_VideoIB_32 + offset, m_IndexBuffer_32 + offset, copySize);
            break;

        default:
            //Should never get here
            AEAssert(false);
            break;
    }

    return AEResult::Ok;
}
