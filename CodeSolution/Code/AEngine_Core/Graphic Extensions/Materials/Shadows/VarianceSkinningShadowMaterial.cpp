/*
* Copyright (c) 2018 <Carlos Chac�n>
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
#include "GraphicDevice.h"
#include "Shaders\PixelShader.h"
#include "Shaders\VertexShader.h"
#include "Resource\GameResourceManager.h"
#include "Shaders\Buffers\ConstantBuffer.h"
#include "VarianceSkinningShadowMaterial.h"
#include "Shaders\Variables\ShaderCustomVariable.h"
#include "Graphic Extensions\Shader Extensions\Properties\ShaderProperties.h"

#if defined(DEBUG) | defined(_DEBUG)
#include "Compiled Materials\HLSL\VarianceSkinningShadowVS_x64_d.h"
#include "Compiled Materials\HLSL\VarianceShadowPS_x64_d.h"
#else
#include "Compiled Materials\HLSL\VarianceSkinningShadowVS_x64.h"
#include "Compiled Materials\HLSL\VarianceShadowPS_x64.h"
#endif

//Always include last
#include "Memory\MemLeaks.h"

/********************
*   Function Defs   *
*********************/
VarianceSkinningShadowMaterial::VarianceSkinningShadowMaterial(GraphicDevice& graphicDevice, GameResourceManager& gameResourceManager, const std::string& name)
    : Material(graphicDevice, gameResourceManager, name)
{
}

VarianceSkinningShadowMaterial::~VarianceSkinningShadowMaterial()
{
}

AEResult VarianceSkinningShadowMaterial::CreateVertexShader()
{
    AEResult ret = AEResult::Ok;

    /////////////////////////////////////////////////////
    //Get Vertex Shader from Game Resources
    m_VertexShader = (VertexShader*)m_GameResourceManager.AcquireGameResourceByStringID(AE_VARIANCE_SKINNING_SHADOW_MAT_VS_NAME, GameResourceType::VertexShader);

    if(m_VertexShader == nullptr)
    {
        m_VertexShader = new VertexShader(m_GraphicDevice, AE_VARIANCE_SKINNING_SHADOW_MAT_VS_NAME);
        ret = m_VertexShader->LoadShader(VarianceSkinningShadowVS, sizeof(VarianceSkinningShadowVS));
        if(ret != AEResult::Ok)
        {
            return AEResult::VertexShaderLoadFailed;
        }

        ret = m_GameResourceManager.ManageGameResource(m_VertexShader, AE_VARIANCE_SKINNING_SHADOW_MAT_VS_NAME);
        if(ret != AEResult::Ok)
        {
            return AEResult::ResourceManagedFailed;
        }
    }

    /////////////////////
    //Create Properties
    m_VSProps = new ShaderProperties(ShaderType::VertexShader, m_GraphicDevice);

    ret = CreateVertexShaderConstantBuffer();

    if(ret != AEResult::Ok)
    {
        return AEResult::ConstantBufferInitFailed;
    }

    return AEResult::Ok;
}

AEResult VarianceSkinningShadowMaterial::CreateVertexShaderConstantBuffer()
{
    AEResult ret = AEResult::Ok;

    /****************************************************************************
    *Constant Buffer #1: _AE_CB_World_View_Proj
    ****************************************************************************/
    ConstantBuffer* cbWVP = nullptr;

    ret = AEBuiltInMaterialsHelpers::BuildCBWVP(m_GraphicDevice, &cbWVP);

    if(ret != AEResult::Ok)
    {
        return AEResult::ConstantBufferInitFailed;
    }

    ret = m_VSProps->AddConstantBuffer(cbWVP);

    if(ret != AEResult::Ok)
    {
        DeleteMem(cbWVP);

        return AEResult::ConstantBufferInitFailed;
    }

    /****************************************************************************
    *Constant Buffer #2: _AE_CB_Bones
    ****************************************************************************/
    ConstantBuffer* cbBones = nullptr;

    ret = AEBuiltInMaterialsHelpers::BuildCBBones(m_GraphicDevice, &cbBones);

    if (ret != AEResult::Ok)
    {
        return AEResult::ConstantBufferInitFailed;
    }

    ret = m_VSProps->AddConstantBuffer(cbBones);

    if (ret != AEResult::Ok)
    {
        DeleteMem(cbBones);

        return AEResult::ConstantBufferInitFailed;
    }

    /****************************************************************************
    *Finish
    ****************************************************************************/
    return AEResult::Ok;
}

AEResult VarianceSkinningShadowMaterial::CreatePixelShader()
{
    AEResult ret = AEResult::Ok;

    /////////////////////////////////////////////////////
    //Get Pixel Shader from Game Resources
    m_PixelShader = (PixelShader*)m_GameResourceManager.AcquireGameResourceByStringID(AE_VARIANCE_SHADOW_MAT_PS_NAME, GameResourceType::PixelShader);

    if (m_PixelShader == nullptr)
    {
        m_PixelShader = new PixelShader(m_GraphicDevice, AE_VARIANCE_SHADOW_MAT_PS_NAME);
        ret = m_PixelShader->LoadShader(VarianceShadowPS, sizeof(VarianceShadowPS));
        if (ret != AEResult::Ok)
        {
            return AEResult::VertexShaderLoadFailed;
        }

        ret = m_GameResourceManager.ManageGameResource(m_PixelShader, AE_VARIANCE_SHADOW_MAT_PS_NAME);
        if (ret != AEResult::Ok)
        {
            return AEResult::ResourceManagedFailed;
        }
    }

    /////////////////////
    //Create Properties
    m_PSProps = new ShaderProperties(ShaderType::PixelShader, m_GraphicDevice);

    return AEResult::Ok;
}

AEResult VarianceSkinningShadowMaterial::LoadContent()
{
    if(m_IsReady)
    {
        return AEResult::Ok;
    }

    ////////////////////////////////////
    //Clean up memory
    CleanUp();

    AEResult ret = AEResult::Ok;

    /***************************
    *Vertex Shader
    ***************************/
    ret = CreateVertexShader();

    if(ret != AEResult::Ok)
    {
        CleanUp();

        return ret;
    }

    /***************************
    *Pixel Shader
    ***************************/
    ret = CreatePixelShader();

    if (ret != AEResult::Ok)
    {
        CleanUp();

        return ret;
    }

    /***************************
    *Finish
    ***************************/
    m_IsReady = true;

    return AEResult::Ok;
}

