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
#include "precomp_gamecomponents.h"

/**********************
*   System Includes   *
***********************/

/*************************
*   3rd Party Includes   *
**************************/

/***************************
*   Game Engine Includes   *
****************************/
#include "Time\AETime.h"
#include "Camera\Camera.h"
#include "GraphicDevice.h"
#include "FPRLightCulling.h"
#include "GameApp\GameApp.h"
#include "Textures\Texture.h"
#include "Lights\LightManager.h"
#include "ForwardPlusRendering.h"
#include "Camera\CameraUpdater.h"
#include "Textures\DepthStencilSurface.h"
#include "Shaders\Buffers\SimpleBuffer.h"
#include "Shaders\Buffers\ConstantBuffer.h"
#include "Shaders\Buffers\StructuredBuffer.h"
#include "Shaders\Bindings\ShaderTextureBinding.h"
#include "Graphic Extensions\Materials\BuiltInMaterialsDefs.h"
#include "Graphic Extensions\Shader Extensions\Properties\ShaderProperties.h"
#include "Graphic Extensions\Materials\Forward+ Rendering\ForwardPlusLightCullingMaterial.h"

//Always include last
#include "Memory\MemLeaks.h"

/********************
*   Function Defs   *
*********************/
AETODO("Check object instances and calls to where it is init");
FPRLightCulling::FPRLightCulling(GameApp* gameApp, const std::string& gameComponentName, const std::string& fprServiceName, const std::string& cameraServiceName, uint32_t callOrder)
    : DrawableGameComponent(gameApp, gameComponentName, callOrder)
{
    m_ForwardPlusRendering = m_GameApp->GetGameService<ForwardPlusRendering>(fprServiceName);
    AEAssert(m_ForwardPlusRendering != nullptr);

    m_CameraUpdater = m_GameApp->GetGameService<CameraUpdater>(cameraServiceName);
    AEAssert(m_CameraUpdater != nullptr);
}

FPRLightCulling::~FPRLightCulling()
{
    DeleteMem(m_ForwardPlusLightCullingMaterial);
}

void FPRLightCulling::Initialize()
{
    m_ForwardPlusLightCullingMaterial = new ForwardPlusLightCullingMaterial(m_GraphicDevice, m_GameResourceManager);

    DrawableGameComponent::Initialize();
}

void FPRLightCulling::LoadContent()
{
    AEResult ret = AEResult::Ok;

    ret = m_ForwardPlusLightCullingMaterial->LoadContent();

    if(ret != AEResult::Ok)
    {
        AETODO("Add log");
    }

    DrawableGameComponent::LoadContent();
}

void FPRLightCulling::Update(const TimerParams& timerParams)
{
    DrawableGameComponent::Update(timerParams);
}

void FPRLightCulling::Render(const TimerParams& timerParams)
{
    ///////////////////////////////////////////
    //Begin Event for Diagnostic View
    m_GraphicDevice->BeginEvent("Forward+ Rendering Light Culling");

    ///////////////////////////////////////////////////
    //Get Current Main Camera
    Camera* currentCamera = m_CameraUpdater->GetMainCamera();
    if (currentCamera == nullptr)
    {
        AETODO("Add log");
        return;
    }

    ///////////////////////////////////////////
    //Check that the Depth Stencil has been
    //applied and that is its the correct one
    const ShaderTextureBinding* stb = m_ForwardPlusLightCullingMaterial->GetCSProps()->GetShaderTextureBinding(AE_TX_DEPTH_TEXTURE_CULL_NAME);
    Texture* texture = stb->GetTexture();
    DepthStencilSurface* dsFPR = m_ForwardPlusRendering->GetForwardPlusDS();

    if( !stb->HasTexture() || ( stb->HasTexture() && texture->GetUniqueID() != dsFPR->GetUniqueID() ) )
    {
        AETODO("Check Return");
        m_ForwardPlusLightCullingMaterial->GetCSProps()->SetTexture(AE_TX_DEPTH_TEXTURE_CULL_NAME, dsFPR);
    }

    ///////////////////////////////////////////
    //Override Structure Buffer to use a single 
    //one
    StructuredBuffer* currentStructuredBuffer = m_ForwardPlusLightCullingMaterial->GetCSProps()->GetStructuredBuffer(AE_BF_LIGHT_BUFFER_NAME);
    StructuredBuffer* structuredBuffer = m_ForwardPlusRendering->GetLightStructuredBuffer();

    if( currentStructuredBuffer == nullptr || (currentStructuredBuffer != nullptr && currentStructuredBuffer->GetUniqueID() != structuredBuffer->GetUniqueID() ) )
    {
        AETODO("Check Return");
        m_ForwardPlusLightCullingMaterial->GetCSProps()->OverrideStructuredBuffer(AE_BF_LIGHT_BUFFER_NAME, structuredBuffer);
    }

    ///////////////////////////////////////////
    //Override Simple Buffer to use a single 
    //one
    SimpleBuffer* currentSimpleBuffer = m_ForwardPlusLightCullingMaterial->GetCSProps()->GetSimpleBuffer(AE_BF_PER_TILE_LIGHT_INDEX_BUFFER_NAME);
    SimpleBuffer* simpleBuffer = m_ForwardPlusRendering->GetPerTileLightIndexBuffer();

    if( currentSimpleBuffer == nullptr || (currentSimpleBuffer != nullptr && currentSimpleBuffer->GetUniqueID() != simpleBuffer->GetUniqueID() ) )
    {
        AETODO("Check Return");
        m_ForwardPlusLightCullingMaterial->GetCSProps()->OverrideSimpleBuffer(AE_BF_PER_TILE_LIGHT_INDEX_BUFFER_NAME, simpleBuffer);
    }

    ///////////////////////////////////////////
    //Set Constant Buffer Variables
    ConstantBuffer* cb = m_ForwardPlusLightCullingMaterial->GetCSProps()->GetConstantBuffer(AE_CB_FPR_LIGHT_CULLING_NAME);
    AEAssert(cb != nullptr);
    if(cb != nullptr)
    {
        glm::mat4 invProj   = glm::inverse(currentCamera->GetProjectionMatrix());
        uint32_t numLights  = m_GameApp->GetLightManager()->GetNumberOfLights();

        cb->SetValueT<glm::mat4>(AE_CB_VIEW_VAR_NAME, currentCamera->GetViewMatrix());
        cb->SetValueT<glm::mat4>(AE_CB_INV_PROJECTION_VAR_NAME, invProj);

        cb->SetValueT<uint32_t>(AE_CB_WINDOW_HEIGHT_VAR_NAME, m_GraphicDevice->GetGraphicPP().m_BackBufferHeight);
        cb->SetValueT<uint32_t>(AE_CB_WINDOW_WIDTH_VAR_NAME, m_GraphicDevice->GetGraphicPP().m_BackBufferWidth);
        cb->SetValueT<uint32_t>(AE_CB_NUM_LIGHTS_VAR_NAME, numLights);
    }

    ///////////////////////////////////////////
    //Apply Shader Settings
    AETODO("Check Return");
    m_ForwardPlusLightCullingMaterial->Apply();

    ///////////////////////////////////////////
    //Dispatch CS
    const glm::uvec2& numTiles = m_ForwardPlusRendering->GetNumTiles();

    AETODO("Check Return");
    m_GraphicDevice->DispatchComputeShader(numTiles.x, numTiles.y, 1);

    ///////////////////////////////////////////
    //Un-apply Shader Settings
    AETODO("Check Return");
    m_ForwardPlusLightCullingMaterial->UnApply();

    ///////////////////////////////////////////
    //End Event for Diagnostic View
    m_GraphicDevice->EndEvent();

    ///////////////////////////////////////////
    //End
    DrawableGameComponent::Render(timerParams);
}

void FPRLightCulling::OnLostDevice()
{    
    DrawableGameComponent::OnLostDevice();
}

void FPRLightCulling::OnResetDevice()
{
    DrawableGameComponent::OnResetDevice();
}