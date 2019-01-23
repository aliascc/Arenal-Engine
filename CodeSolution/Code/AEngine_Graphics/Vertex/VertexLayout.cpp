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
#include "VertexLayout.h"
#include "GraphicDevice.h"

//Always include last
#include "Memory\MemLeaks.h"

/********************
*   Function Defs   *
*********************/
VertexLayout::VertexLayout()
    : Named("")
{
}

VertexLayout::~VertexLayout()
{
    ReleaseCOM(m_VertexLayoutDX);
}

AEResult VertexLayout::BuildVertexLayout(GraphicDevice& graphicDevice, const BYTE shaderByteCode[], uint32_t shadeByteCodeSize, const D3D11_INPUT_ELEMENT_DESC vertexDesc[], uint32_t elementCount, const std::string& name)
{
    if(vertexDesc == nullptr || shaderByteCode == nullptr || elementCount == 0)
    {
        return AEResult::Fail;
    }

    // Create the input layout    
    HRESULT hr = graphicDevice.GetDeviceDX()->CreateInputLayout(vertexDesc, elementCount, (void*)shaderByteCode, shadeByteCodeSize, &m_VertexLayoutDX);

    if(hr != S_OK)
    {
        DisplayError(hr);

        return AEResult::Fail;
    }

    m_Name = name;

    AEGraphicHelpers::SetDebugObjectName(m_VertexLayoutDX, AE_DEBUG_VIL_NAME_PREFIX + m_Name);
    
    m_IsReady = true;

    return AEResult::Ok;
}

