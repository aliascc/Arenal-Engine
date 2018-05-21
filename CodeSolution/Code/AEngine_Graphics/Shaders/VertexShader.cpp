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
#include "ShaderDefs.h"
#include "VertexShader.h"
#include "GraphicDevice.h"

//Always include last
#include "Memory\MemLeaks.h"

/********************
*   Function Defs   *
*********************/
VertexShader::VertexShader(GraphicDevice* graphicDevice, const std::string& name)
    : Shader(graphicDevice, ShaderType::VertexShader, name)
{
}

VertexShader::~VertexShader()
{
    ReleaseCOM(m_VSDX);
}

AEResult VertexShader::LoadShaderWithoutLock(const BYTE* shaderByteCode, uint32_t length)
{
    AEAssert(shaderByteCode != nullptr);

    if(shaderByteCode == nullptr)
    {
        return AEResult::NullParameter;
    }

    ID3D11VertexShader* tempDX = nullptr;

    AETODO("Check Dynamic Class Linkage");
    HRESULT hr = m_GraphicDevice->GetDeviceDX()->CreateVertexShader((void*)shaderByteCode, length, nullptr, &tempDX);
    if(hr != S_OK)
    {
        AETODO("Add log message here");

        DisplayError(hr);

        return AEResult::Fail;
    }
    
    //Release DirectX Object
    ReleaseCOM(m_VSDX);
    m_VSDX = tempDX;

    AEGraphicHelpers::SetDebugObjectName<ID3D11VertexShader>(m_VSDX, AE_DEBUG_VS_NAME_PREFIX + m_Name);
    
    return AEResult::Ok;
}
