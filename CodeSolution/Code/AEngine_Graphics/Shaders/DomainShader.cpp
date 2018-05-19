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
#include "DomainShader.h"
#include "GraphicDevice.h"
#include "Base\BaseFunctions.h"

//Always include last
#include "Memory\MemLeaks.h"

/********************
*   Function Defs   *
*********************/
DomainShader::DomainShader(GraphicDevice* graphicDevice, const std::string& name)
    : Shader(graphicDevice, ShaderType::DomainShader, name)
{
}

DomainShader::~DomainShader()
{
    ReleaseCOM(m_DSDX);
}

AEResult DomainShader::LoadShaderWithoutLock(const BYTE* shaderByteCode, uint32_t length)
{
    AEAssert(shaderByteCode != nullptr);

    if(shaderByteCode == nullptr)
    {
        return AEResult::NullParameter;
    }
    
    ID3D11DomainShader* tempDX = nullptr;

    AETODO("Check Dynamic Class Linckage");
    HRESULT hr = m_GraphicDevice->GetDeviceDX()->CreateDomainShader((void*)shaderByteCode, length, nullptr, &tempDX);

    if(hr != S_OK)
    {
        AETODO("Add log message here");

        DisplayError(hr);

        return AEResult::Fail;
    }
    
    //Release DirectX Object
    ReleaseCOM(m_DSDX);
    m_DSDX = tempDX;

    AEGraphicHelpers::SetDebugObjectName<ID3D11DomainShader>(m_DSDX, AE_DEBUG_DS_NAME_PREFIX + m_Name);
    
    return AEResult::Ok;
}
