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
#include "FPRPreZ.h"
#include "FPRDefs.h"
#include "FPRObjectDraw.h"
#include "FPRLightCulling.h"
#include "Lights\LightDefs.h"
#include "ForwardPlusRendering.h"
#include "Shaders\Variables\Sampler.h"
#include "Shaders\Buffers\SimpleBuffer.h"
#include "Textures\DepthStencilSurface.h"
#include "Shaders\Buffers\StructuredBuffer.h"
#include "Shaders\Variables\Texture2DArray.h"
#include "Graphic Extensions\Materials\BuiltInMaterialsDefs.h"

//Always include last
#include "Memory\MemLeaks.h"

/********************
*   Function Defs   *
*********************/
ForwardPlusRendering::ForwardPlusRendering(GameApp& gameApp, const std::string& gameComponentName, const std::string& serviceName, uint32_t callOrder)
    : GameComponent(gameApp, gameComponentName, callOrder)
    , m_LightManager(gameApp.GetLightManager())
    , m_ServiceName(serviceName)
{
    //Register Game Component as Service
    AETODO("Check ret better");
    AEResult ret = m_GameApp.RegisterGameService(m_ServiceName, this);
    AEAssert(ret == AEResult::Ok);

    //Create Forward+ Rendering Game Components
    m_FPRPreZ = new FPRPreZ(gameApp);
    m_FPRLightCulling = new FPRLightCulling(gameApp);
    m_FPRObjectDraw = new FPRObjectDraw(gameApp);

    AETODO("add check");
    m_GameApp.AddComponent(m_FPRPreZ);
    m_GameApp.AddComponent(m_FPRLightCulling);
    m_GameApp.AddComponent(m_FPRObjectDraw);
}

ForwardPlusRendering::~ForwardPlusRendering()
{
    m_GameApp.RemoveComponent(m_FPRPreZ);
    m_GameApp.RemoveComponent(m_FPRLightCulling);
    m_GameApp.RemoveComponent(m_FPRObjectDraw);

    DeleteMem(m_FPRPreZ);
    DeleteMem(m_FPRLightCulling);
    DeleteMem(m_FPRObjectDraw);

    AERelease(m_ForwardPlusDS);
    DeleteMem(m_LightStructuredBuffer);
    DeleteMem(m_ShadowSpotLightInfoStructuredBuffer);
    DeleteMem(m_ShadowDirLightInfoStructuredBuffer);
    DeleteMem(m_PerTileLightIndexBuffer);
    DeleteMem(m_ShadowTextureSampler);
}

void ForwardPlusRendering::Initialize()
{
    AEResult ret                    = AEResult::Ok;
    GraphicDevice& graphicDevice    = m_GameApp.GetGraphicsDevice();

    /////////////////////////////////////////////////////
    //Create Depth Stencil
    m_ForwardPlusDS = new DepthStencilSurface(graphicDevice, AE_FORWARD_PLUS_DEPTH_STENCIL_NAME);

    /////////////////////////////////////////////////////
    //Create Shadow Texture Sampler
    m_ShadowTextureSampler = new Sampler(graphicDevice, AE_SAM_SHADOW_TEXTURE_SAMPLER_NAME, AE_BI_SHADOW_TEXTURE_SAMPLER_BIND_IDX);

    /////////////////////////////////////////////////////
    //Create Structured Buffer for Lights
    ret = AEBuiltInMaterialsHelpers::BuildBufferLightBuffer(graphicDevice, &m_LightStructuredBuffer);
    if(ret != AEResult::Ok)
    {
        AETODO("log error");
    }

    /////////////////////////////////////////////////////
    //Create Simple Buffer for Tile Light Index Buffer
    ret = AEBuiltInMaterialsHelpers::BuildBufferPerTileLightIndexBuffer(graphicDevice, &m_PerTileLightIndexBuffer);
    if(ret != AEResult::Ok)
    {
        AETODO("log error");
    }

    /////////////////////////////////////////////////////
    //Create Structured Buffer for Shadow Lights Info
    m_ShadowSpotLightInfoStructuredBuffer = new StructuredBuffer(AE_BF_SHADOW_SPOT_LIGHT_INFO_BUFFER_NAME, AE_BI_SHADOW_SPOT_LIGHT_INFO_BUFFER_BIND_IDX, false, graphicDevice);

    m_ShadowDirLightInfoStructuredBuffer = new StructuredBuffer(AE_BF_SHADOW_DIR_LIGHT_INFO_BUFFER_NAME, AE_BI_SHADOW_DIR_LIGHT_INFO_BUFFER_BIND_IDX, false, graphicDevice);
}

void ForwardPlusRendering::LoadContent()
{
    AEResult ret = AEResult::Ok;
    GraphicDevice& graphicDevice = m_GameApp.GetGraphicsDevice();

    m_NumTiles = ForwardPlusHelper::GetNumTiles(graphicDevice.GetGraphicPP().m_GameBackBufferWidth, graphicDevice.GetGraphicPP().m_GameBackBufferHeight);

    AETODO("Set is ready when ds and buffers are created");

    /////////////////////////////////////////////////////
    //Init Depth Stencil
    AETODO("Check return");
    InitForwardPlusDS();

    /////////////////////////////////////////////////////
    //Init Per Light Light Index Buffer
    AETODO("Check return");
    InitPerTileLightIndexBuffer();

    /////////////////////////////////////////////////////
    //Init Light Structure Buffer
    ret = m_LightStructuredBuffer->InitializeBuffer(sizeof(LightFX), AE_MAX_LIGHTS, GraphicBufferUsage::Dynamic, GraphicBufferAccess::Write);
    if (ret != AEResult::Ok)
    {
        AETODO("log error");
    }

    /////////////////////////////////////////////////////
    //Init Shadow Texture Sampler
    //
    // MinFilter        = ANISOTROPIC;
    // MagFilter        = ANISOTROPIC;
    // MipFilter        = ANISOTROPIC;
    // AddressU         = BORDER;
    // AddressV         = BORDER;
    // MaxAnisotropy    = 8;
    // BorderColor      = float4(0.0f, 0.0f, 0.0f, 0.0f);
    //
    ret = m_ShadowTextureSampler->Initialize(ShaderFilter::Anisotropic, ShaderTextureAddressMode::Border, ShaderTextureAddressMode::Border, ShaderTextureAddressMode::Clamp, -std::numeric_limits<float>::max(), std::numeric_limits<float>::max(), 0.0f, 8, ShaderCompFunct::Never, AEColors::White);
    if (ret != AEResult::Ok)
    {
        AETODO("log error");
    }
}

void ForwardPlusRendering::Update(const TimerParams& timerParams)
{
    AEResult ret = AEResult::Ok;

    ////////////////////////////////////////////////
    //Update Light FX Vector if needed
    if(m_LightManager.HasFXVectorChanged())
    {
        const LightFX* lighFXVector = m_LightManager.GetLightFXVector();

        AETODO("Check return");
        m_LightStructuredBuffer->UpdateBuffer(lighFXVector, AE_MAX_LIGHTS, 0, 0, GraphicResourceMap::WriteDiscard);

        m_LightManager.ClearFXVectorChangeFlag();
    }

    ////////////////////////////////////////////////
    //Update Shadow Spot Light Info FX Vector if needed
    if (m_LightManager.HasFXSpotShadowVectorChanged())
    {
        uint32_t numSpotInfoShadows = m_LightManager.GetNumSpotLightsWithShadows();

        if (m_ShadowSpotLightInfoStructuredBuffer->GetNumElements() != numSpotInfoShadows)
        {
            if (numSpotInfoShadows != 0)
            {
                /////////////////////////////////////////////////////
                //Init Shadow Light Info Structure Buffer with new size
                ret = m_ShadowSpotLightInfoStructuredBuffer->InitializeBuffer(sizeof(SpotLightShadowInfoFX), numSpotInfoShadows, GraphicBufferUsage::Dynamic, GraphicBufferAccess::Write);
                if (ret != AEResult::Ok)
                {
                    AETODO("log error");
                }
            }
            else
            {
                /////////////////////////////////////////////////////
                //De-initialize Array
                ret = m_ShadowSpotLightInfoStructuredBuffer->Deinitialize();
                if (ret != AEResult::Ok)
                {
                    AETODO("log error");
                }
            }
        }

        if (numSpotInfoShadows != 0)
        {
            const SpotLightShadowInfoFX* shadowLightInfoFXVec = m_LightManager.GetSpotLightShadowInfoFXVector();

            AETODO("Check return");
            m_ShadowSpotLightInfoStructuredBuffer->UpdateBuffer(shadowLightInfoFXVec, numSpotInfoShadows, 0, 0, GraphicResourceMap::WriteDiscard);
        }

        m_LightManager.ClearFXSpotShadowVectorChangedFlag();
    }

    ////////////////////////////////////////////////
    //Update Shadow Directional Light Info FX Vector if needed
    if (m_LightManager.HasFXDirShadowVectorChanged())
    {
        uint32_t numDirInfoShadows = m_LightManager.GetNumDirLightsWithShadows();

        if (m_ShadowDirLightInfoStructuredBuffer->GetNumElements() != numDirInfoShadows)
        {
            if (numDirInfoShadows != 0)
            {
                /////////////////////////////////////////////////////
                //Init Shadow Light Info Structure Buffer with new size
                ret = m_ShadowDirLightInfoStructuredBuffer->InitializeBuffer(sizeof(LightCascadeInfoFX), numDirInfoShadows, GraphicBufferUsage::Dynamic, GraphicBufferAccess::Write);
                if (ret != AEResult::Ok)
                {
                    AETODO("log error");
                }
            }
            else
            {
                /////////////////////////////////////////////////////
                //De-initialize Array
                ret = m_ShadowDirLightInfoStructuredBuffer->Deinitialize();
                if (ret != AEResult::Ok)
                {
                    AETODO("log error");
                }
            }
        }

        if (numDirInfoShadows != 0)
        {
            const LightCascadeInfoFX* shadowLightInfoFXVec = m_LightManager.GetDirLightShadowInfoFXVector();

            AETODO("Check return");
            m_ShadowDirLightInfoStructuredBuffer->UpdateBuffer(shadowLightInfoFXVec, numDirInfoShadows, 0, 0, GraphicResourceMap::WriteDiscard);
        }

        m_LightManager.ClearFXDirShadowVectorChangedFlag();
    }
}

void ForwardPlusRendering::OnResetDevice()
{
    GraphicDevice& graphicDevice = m_GameApp.GetGraphicsDevice();

    m_NumTiles = ForwardPlusHelper::GetNumTiles(graphicDevice.GetGraphicPP().m_GameBackBufferWidth, graphicDevice.GetGraphicPP().m_GameBackBufferHeight);

    AETODO("Check returns");

    InitForwardPlusDS();

    InitPerTileLightIndexBuffer();
}

AEResult ForwardPlusRendering::InitForwardPlusDS()
{
    AEAssert(m_ForwardPlusDS != nullptr);
    if(m_ForwardPlusDS == nullptr)
    {
        return AEResult::NullObj;
    }

    GraphicDevice& graphicDevice = m_GameApp.GetGraphicsDevice();

    AEResult ret = AEResult::Ok;

    uint32_t screenWidth = graphicDevice.GetGraphicPP().m_GameBackBufferWidth;
    uint32_t screenHeight = graphicDevice.GetGraphicPP().m_GameBackBufferHeight;

    ret = m_ForwardPlusDS->InitializeDepthStencilSurface(screenWidth, screenHeight, DXGI_FORMAT_R24G8_TYPELESS, DXGI_FORMAT_D24_UNORM_S8_UINT, DXGI_FORMAT_R24_UNORM_X8_TYPELESS);
    if(ret != AEResult::Ok)
    {
        AETODO("Add log");
        return ret;
    }

    return AEResult::Ok;
}

AEResult ForwardPlusRendering::InitPerTileLightIndexBuffer()
{
    AEAssert(m_PerTileLightIndexBuffer != nullptr);
    if(m_PerTileLightIndexBuffer == nullptr)
    {
        return AEResult::NullObj;
    }

    AEResult ret = AEResult::Ok;

    //Use +1 for MAX_LIGHTS, that way we can add a sentry at the end of the Tile Light Buffer telling us
    //it has ended
    uint32_t sizeOfTileLightBuffer = (AE_MAX_LIGHTS + 1) * (m_NumTiles.x * m_NumTiles.y);

    ret = m_PerTileLightIndexBuffer->InitializeBuffer(sizeOfTileLightBuffer, GraphicBufferUsage::Default, GraphicBufferAccess::None);
    if(ret != AEResult::Ok)
    {
        AETODO("log error");
        return ret;
    }

    return AEResult::Ok;
}
