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
#include "GraphicDevice.h"
#include "ConstantBuffer.h"
#include "Shaders\Variables\ShaderCustomVariable.h"

//Always include last
#include "Memory\MemLeaks.h"

/********************
*   Function Defs   *
*********************/
ConstantBuffer::ConstantBuffer(GraphicDevice* graphicDevice, uint32_t bindIndex, const std::string& name)
    : Named(name)
    , m_GraphicDevice(graphicDevice)
    , m_BindIndex(bindIndex)
{
    AEAssert(m_GraphicDevice != nullptr);
}

ConstantBuffer::~ConstantBuffer()
{
    ReleaseCOM(m_ConstantBufferDX);

    DeleteMemArr(m_ConstantBuffer);

    uint32_t numVars = (uint32_t)m_VariableVector.size();
    for (uint32_t i = 0; i < numVars; i++)
    {
        DeleteMem(m_VariableVector[i]);
    }
    m_VariableVector.clear();
    m_VariableMap.clear();
}

uint32_t ConstantBuffer::CalculateSize()
{
    uint32_t sizeBuffer = 0;

    uint32_t numVariables = (uint32_t)m_VariableVector.size();
        
    for (uint32_t i = 0; i < numVariables; ++i)
    {
        sizeBuffer += (m_VariableVector[i]->m_Size + m_VariableVector[i]->m_Offset);
    }

    return sizeBuffer;
}

AEResult ConstantBuffer::Initialize()
{
    if(m_IsReady)
    {
        return AEResult::Ok;
    }

    /******************************************
    *1. Initialize RAM Constant Buffer Memory
    /******************************************/
    uint32_t sizeBuffer = CalculateSize();

    if(sizeBuffer == 0)
    {
        return AEResult::ZeroSize;
    }

    //Constant Buffer must be multiple of 16
    uint32_t mod16 = sizeBuffer % 16;

    if(mod16 != 0)
    {
        uint32_t addToSize = 16 - mod16;

        sizeBuffer += addToSize;
    }

    BYTE* constantBufferMemory = new BYTE[sizeBuffer];
    memset(constantBufferMemory, 0, sizeBuffer);
    
    uint32_t numVariables = (uint32_t)m_VariableVector.size();        
    for (uint32_t i = 0; i < numVariables; ++i)
    {
        AEAssert(m_VariableVector[i]->m_StartOffset < sizeBuffer);
        AEAssert((m_VariableVector[i]->m_StartOffset + m_VariableVector[i]->m_Size) <= sizeBuffer);

        uint32_t offset = (m_VariableVector[i]->m_StartOffset + m_VariableVector[i]->m_Size);

        if(offset > sizeBuffer)
        {
            DeleteMemArr(constantBufferMemory);

            return AEResult::OutsideRange;
        }

        m_VariableVector[i]->m_Memory = reinterpret_cast<void*>(constantBufferMemory + m_VariableVector[i]->m_StartOffset);
    }

    /******************************************
    *2. Create VRAM Constant Buffer
    /******************************************/
    ReleaseCOM(m_ConstantBufferDX);

    D3D11_BUFFER_DESC cbDesc = { 0 };

    cbDesc.ByteWidth        = sizeBuffer;
    cbDesc.Usage            = D3D11_USAGE_DYNAMIC;
    cbDesc.BindFlags        = D3D11_BIND_CONSTANT_BUFFER;
    cbDesc.CPUAccessFlags    = D3D11_CPU_ACCESS_WRITE;

    HRESULT hr = m_GraphicDevice->GetDeviceDX()->CreateBuffer(&cbDesc, nullptr, &m_ConstantBufferDX);

    if(hr != S_OK)
    {
        DisplayError(hr);

        DeleteMemArr(constantBufferMemory);

        return AEResult::ConstantBufferInitFailed;
    }

    AEGraphicHelpers::SetDebugObjectName<ID3D11Buffer>(m_ConstantBufferDX, AE_DEBUG_CB_NAME_PREFIX + m_Name);

    /******************************************
    *3. Finish
    /******************************************/
    m_ConstantBuffer = reinterpret_cast<void*>(constantBufferMemory);

    m_ConstantBufferDataSize = sizeBuffer;

    m_IsReady = true;

    return AEResult::Ok;
}

AEResult ConstantBuffer::Apply()
{
    if(!m_IsReady)
    {
        return AEResult::NotReady;
    }

    if(!m_NeedUpdate)
    {
        return AEResult::Ok;
    }

    D3D11_MAPPED_SUBRESOURCE mappedResource = { 0 };

    HRESULT hr = m_GraphicDevice->GetDeviceContextDX()->Map(m_ConstantBufferDX, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);

    if(hr != S_OK)
    {
        DisplayError(hr);

        return AEResult::ConstantBufferSetDataFailed;
    }

    memcpy(mappedResource.pData, m_ConstantBuffer, m_ConstantBufferDataSize);

    m_GraphicDevice->GetDeviceContextDX()->Unmap(m_ConstantBufferDX, 0);

    m_NeedUpdate = false;

    return AEResult::Ok;
}

const ShaderCustomVariable* ConstantBuffer::GetVariable(uint32_t index) const
{
    if(index >= static_cast<uint32_t>(m_VariableVector.size()))
    {
        return nullptr;
    }

    return m_VariableVector[index];
}

const ShaderCustomVariable* ConstantBuffer::GetVariable(const std::string& name) const
{
    if(!Exists(name))
    {
        return nullptr;
    }

    return m_VariableMap.find(name)->second;
}

bool ConstantBuffer::Exists(const std::string& name) const
{
    return (m_VariableMap.find(name) != m_VariableMap.end());
}

AEResult ConstantBuffer::AddVariable(ShaderCustomVariable* scv)
{
    if(m_IsReady)
    {
        return AEResult::Fail;
    }

    AEAssert(scv != nullptr);
    if(scv == nullptr)
    {
        return AEResult::NullParameter;
    }

    AEAssert(scv->m_Size != 0);
    if(scv->m_Size == 0)
    {
        return AEResult::ZeroSize;
    }

    AEAssert(!scv->m_Name.empty());
    if(scv->m_Name.empty())
    {
        return AEResult::EmptyName;
    }

    if(Exists(scv->m_Name))
    {
        return AEResult::ObjExists;
    }

    m_VariableMap[scv->m_Name] = scv;

    m_VariableVector.push_back(scv);

    return AEResult::Ok;
}

AEResult ConstantBuffer::SetValue(const std::string& varName, const void* value, uint32_t varSize, bool user)
{
    if(!m_IsReady)
    {
        return AEResult::NotReady;
    }

    AEAssert(value != nullptr);
    if(value == nullptr)
    {
        return AEResult::NullParameter;
    }

    AEAssert(varSize != 0);
    if(varSize == 0)
    {
        return AEResult::ZeroSize;
    }

    if(!Exists(varName))
    {
        return AEResult::NotFound;
    }

    ShaderCustomVariable* cv = m_VariableMap[varName];

    if(user && !cv->m_UserVariable)
    {
        return AEResult::NotFound;
    }

    AETODO("return value if requested size is more than variable size");
    uint32_t maxSize = (varSize < cv->m_Size) ? varSize : cv->m_Size;

    if(memcmp(cv->m_Memory, value, maxSize) != 0)
    {
        memcpy(cv->m_Memory, value, maxSize);

        m_NeedUpdate = true;
    }

    return AEResult::Ok;
}

AEResult ConstantBuffer::GetValue(const std::string& varName, void* value, uint32_t varSize, bool user)
{
    if(!m_IsReady)
    {
        return AEResult::NotReady;
    }

    AEAssert(value != nullptr);

    if(value == nullptr)
    {
        return AEResult::NullParameter;
    }

    AEAssert(varSize != 0);

    if(varSize == 0)
    {
        return AEResult::ZeroSize;
    }

    if(!Exists(varName))
    {
        return AEResult::NotFound;
    }

    ShaderCustomVariable* cv = m_VariableMap[varName];

    if(user && !cv->m_UserVariable)
    {
        return AEResult::NotFound;
    }

    AETODO("return value if requested size is more than variable size");
    uint32_t maxSize = (varSize < cv->m_Size) ? varSize : cv->m_Size;

    memcpy(value, cv->m_Memory, maxSize);

    return AEResult::Ok;
}

bool ConstantBuffer::ContainsUserVariables()
{
    uint32_t numVars = (uint32_t)m_VariableVector.size();
    for (uint32_t i = 0; i < numVars; i++)
    {
        if(m_VariableVector[i]->m_UserVariable)
        {
            return true;
        }
    }

    return false;
}
