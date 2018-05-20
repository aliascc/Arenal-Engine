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
#include "DiffuseTexturePS.h"
#include "Base\BaseFunctions.h"
#include "Shaders\ShaderDefs.h"
#include "Shaders\ShaderSignatures.h"

#if defined(DEBUG) | defined(_DEBUG)
#include "Compiled Materials\HLSL\DiffuseTextureNormalBasicMaterialPS_x64_d.h"
#else
#include "Compiled Materials\HLSL\DiffuseTextureNormalBasicMaterialPS_x64.h"
#endif

//Always include last
#include "Memory\MemLeaks.h"

/********************
*   Function Defs   *
*********************/
DiffuseTexturePS::DiffuseTexturePS(GraphicDevice* graphicDevice, const std::string& name)
    : PixelShader(graphicDevice, name)
{
}

DiffuseTexturePS::~DiffuseTexturePS()
{
}

AEResult DiffuseTexturePS::Load()
{
    /////////////////////////////////////////////
    //Pre-check
    AEAssert(m_GraphicDevice != nullptr);
    if (m_GraphicDevice == nullptr)
    {
        return AEResult::GraphicDeviceNull;
    }

    std::lock_guard<std::mutex> lock(m_GameResourceMutex);

    /////////////////////////////////////////////
    //Variables
    AEResult ret = AEResult::Ok;

    /////////////////////////////////////////////
    //Load Shader Into Memory
    ret = this->LoadShaderWithoutLock(DiffuseTextureNormalBasicMaterialPS, sizeof(DiffuseTextureNormalBasicMaterialPS));;
    if (ret != AEResult::Ok)
    {
        return ret;
    }

    /////////////////////////////////////////////
    //Create Signature Lists

    SamplerSignature diffuseSamplerSig;
    diffuseSamplerSig.m_BindIndex = 0;
    diffuseSamplerSig.m_Name = AE_SAM_DIFFUSE_TEXTURE_SAMPLER_NAME;

    TextureInputSignature diffuseTISig;
    diffuseTISig.m_BindIndex = 0;
    diffuseTISig.m_TextureType = TextureType::Texture2D;
    diffuseTISig.m_Name = AE_TX_DIFFUSE_TEXTURE_NAME;

    /////////////////////////////////////////////
    //Add to Signature Lists

    m_TextureInputSignatureList.push_back(diffuseTISig);

    m_SamplerSignatureList.push_back(diffuseSamplerSig);

    /////////////////////////////////////////////
    //Finish
    return AEResult::Ok;
}
