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
#include "precomp_core.h"

/**********************
*   System Includes   *
***********************/

/*************************
*   3rd Party Includes   *
**************************/

/***************************
*   Game Engine Includes   *
****************************/
#include "Material.h"
#include "GraphicDevice.h"
#include "Shaders\HullShader.h"
#include "Shaders\PixelShader.h"
#include "Shaders\DomainShader.h"
#include "Shaders\VertexShader.h"
#include "Shaders\ComputeShader.h"
#include "Shaders\GeometryShader.h"
#include "Resource\GameResourceManager.h"
#include "Graphic Extensions\Shader Extensions\Properties\ShaderProperties.h"

//Always include last
#include "Memory\MemLeaks.h"

/********************
*   Function Defs   *
*********************/
Material::Material(GraphicDevice& graphicDevice, GameResourceManager& gameResourceManager, const std::string& name)
    : Named(name)
    , m_GraphicDevice(graphicDevice)
    , m_GameResourceManager(gameResourceManager)
{
}

Material::~Material()
{
    CleanUp();
}

void Material::CleanUp()
{
    AERelease(m_VertexShader);
    DeleteMem(m_VSProps);

    AERelease(m_PixelShader);
    DeleteMem(m_PSProps);

    AERelease(m_GeometryShader);
    DeleteMem(m_GSProps);

    AERelease(m_DomainShader);
    DeleteMem(m_DSProps);

    AERelease(m_HullShader);
    DeleteMem(m_HSProps);

    AERelease(m_ComputeShader);
    DeleteMem(m_CSProps);
}

void Material::SetVertexShader(VertexShader* shader, bool releaseOld)
{
    if(releaseOld)
    {
        AERelease(m_VertexShader);
    }

    m_VertexShader = shader;
}

void Material::SetPixelShader(PixelShader* shader, bool releaseOld)
{
    if(releaseOld)
    {
        AERelease(m_PixelShader);
    }

    m_PixelShader = shader;
}

void Material::SetGeometryShader(GeometryShader* shader, bool releaseOld)
{
    if(releaseOld)
    {
        AERelease(m_GeometryShader);
    }

    m_GeometryShader = shader;
}

void Material::SetDomainShader(DomainShader* shader, bool releaseOld)
{
    if(releaseOld)
    {
        AERelease(m_DomainShader);
    }

    m_DomainShader = shader;
}

void Material::SetHullShader(HullShader* shader, bool releaseOld)
{
    if(releaseOld)
    {
        AERelease(m_HullShader);
    }

    m_HullShader = shader;
}

void Material::SetComputeShader(ComputeShader* shader, bool releaseOld)
{
    if(releaseOld)
    {
        AERelease(m_ComputeShader);
    }

    m_ComputeShader = shader;
}

AEResult Material::Apply()
{
    if (!m_IsReady)
    {
        return AEResult::NotReady;
    }

    AEResult ret = AEResult::Ok;
    
    /***********************
    *Vertex Shader
    ***********************/
    m_GraphicDevice.SetVertexShader(m_VertexShader);

    if(m_VertexShader != nullptr && m_VSProps != nullptr)
    {
        ret = m_VSProps->ApplyAll();

        if(ret != AEResult::Ok)
        {
            return AEResult::SetVertexShaderPropsFailed;
        }
    }

    /***********************
    *Pixel Shader
    ***********************/
    m_GraphicDevice.SetPixelShader(m_PixelShader);

    if(m_PixelShader != nullptr && m_PSProps != nullptr)
    {
        ret = m_PSProps->ApplyAll();

        if(ret != AEResult::Ok)
        {
            return AEResult::SetPixelShaderPropsFailed;
        }
    }
    
    /***********************
    *Geometry Shader
    ***********************/
    m_GraphicDevice.SetGeometryShader(m_GeometryShader);

    if(m_GeometryShader != nullptr && m_GSProps != nullptr)
    {
        ret = m_GSProps->ApplyAll();

        if(ret != AEResult::Ok)
        {
            return AEResult::SetGeometryShaderPropsFailed;
        }
    }
    
    /***********************
    *Domain Shader
    ***********************/
    m_GraphicDevice.SetDomainShader(m_DomainShader);

    if(m_DomainShader != nullptr && m_DSProps != nullptr)
    {
        ret = m_DSProps->ApplyAll();

        if(ret != AEResult::Ok)
        {
            return AEResult::SetDomainShaderPropsFailed;
        }
    }
    
    /***********************
    *Hull Shader
    ***********************/
    m_GraphicDevice.SetHullShader(m_HullShader);

    if(m_HullShader != nullptr && m_HSProps != nullptr)
    {
        ret = m_HSProps->ApplyAll();

        if(ret != AEResult::Ok)
        {
            return AEResult::SetHullShaderPropsFailed;
        }
    }
    
    /***********************
    *Compute Shader
    ***********************/
    m_GraphicDevice.SetComputeShader(m_ComputeShader);

    if(m_ComputeShader != nullptr && m_CSProps != nullptr)
    {
        ret = m_CSProps->ApplyAll();

        if(ret != AEResult::Ok)
        {
            return AEResult::SetComputeShaderPropsFailed;
        }
    }
    
    /***********************
    *Finish
    ***********************/
    return AEResult::Ok;
}

AEResult Material::UnApply()
{
    if (!m_IsReady)
    {
        return AEResult::NotReady;
    }

    AEResult ret = AEResult::Ok;
    
    /***********************
    *Vertex Shader
    ***********************/
    if(m_VertexShader != nullptr)
    {
        m_GraphicDevice.SetVertexShader(nullptr);

        if(m_VSProps != nullptr)
        {
            m_VSProps->UnApplyAll();
        }
    }
    
    /***********************
    *Pixel Shader
    ***********************/
    if(m_PixelShader != nullptr)
    {
        m_GraphicDevice.SetPixelShader(nullptr);

        if(m_PSProps != nullptr)
        {
            m_PSProps->UnApplyAll();
        }
    }
    
    /***********************
    *Geometry Shader
    ***********************/
    if(m_GeometryShader != nullptr)
    {
        m_GraphicDevice.SetGeometryShader(nullptr);

        if(m_GSProps != nullptr)
        {
            m_GSProps->UnApplyAll();
        }
    }
    
    /***********************
    *Domain Shader
    ***********************/
    if(m_DomainShader != nullptr)
    {
        m_GraphicDevice.SetDomainShader(nullptr);

        if(m_DSProps != nullptr)
        {
            m_DSProps->UnApplyAll();
        }
    }
    
    /***********************
    *Hull Shader
    ***********************/
    if(m_HullShader != nullptr)
    {
        m_GraphicDevice.SetHullShader(nullptr);

        if(m_HSProps != nullptr)
        {
            m_HSProps->UnApplyAll();
        }
    }
    
    /***********************
    *Compute Shader
    ***********************/
    if(m_ComputeShader != nullptr)
    {
        m_GraphicDevice.SetComputeShader(nullptr);

        if(m_CSProps != nullptr)
        {
            m_CSProps->UnApplyAll();
        }
    }
    
    /***********************
    *Finish
    ***********************/
    return AEResult::Ok;
}
