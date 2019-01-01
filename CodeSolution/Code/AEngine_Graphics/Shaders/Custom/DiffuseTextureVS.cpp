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
#include "DiffuseTextureVS.h"
#include "Shaders\ShaderDefs.h"
#include "Shaders\ShaderSignatures.h"

#if defined(DEBUG) | defined(_DEBUG)
#include "Compiled Materials\HLSL\DiffuseTextureNormalBasicMaterialVS_x64_d.h"
#else
#include "Compiled Materials\HLSL\DiffuseTextureNormalBasicMaterialVS_x64.h"
#endif

//Always include last
#include "Memory\MemLeaks.h"

/********************
*   Function Defs   *
*********************/
DiffuseTextureVS::DiffuseTextureVS(GraphicDevice& graphicDevice, const std::string& name)
    : VertexShader(graphicDevice, name)
{
}

DiffuseTextureVS::~DiffuseTextureVS()
{
}

AEResult DiffuseTextureVS::Load()
{
    std::lock_guard<std::mutex> lock(m_GameResourceMutex);

    /////////////////////////////////////////////
    //Variables
    AEResult ret = AEResult::Ok;

    /////////////////////////////////////////////
    //Load Shader Into Memory
    ret = this->LoadShaderWithoutLock(DiffuseTextureNormalBasicMaterialVS, sizeof(DiffuseTextureNormalBasicMaterialVS));;
    if (ret != AEResult::Ok)
    {
        return ret;
    }

    /////////////////////////////////////////////
    //Create Signature Lists

    ConstantBufferSignature wvpSig = AEShaderSignatureHelpers::CreateWorldViewProjCBSig(0);

    /////////////////////////////////////////////
    //Add to Signature Lists

    m_ConstantBufferSignatureList.push_back(wvpSig);

    /////////////////////////////////////////////
    //Finish
    return AEResult::Ok;
}
