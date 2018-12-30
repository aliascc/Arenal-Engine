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
#include "Models\Mesh.h"
#include "FPRObjectDraw.h"
#include "Camera\Camera.h"
#include "GraphicDevice.h"
#include "GameApp\GameApp.h"
#include "Models\MeshPart.h"
#include "Vertex\IndexBuffer.h"
#include "Lights\LightManager.h"
#include "ForwardPlusRendering.h"
#include "Camera\CameraUpdater.h"
#include "GameObject\GameObject.h"
#include "Shaders\Variables\Sampler.h"
#include "Shaders\Buffers\SimpleBuffer.h"
#include "GameObject\GameObjectManager.h"
#include "GameObject\Components\MeshGOC.h"
#include "Shaders\Buffers\ConstantBuffer.h"
#include "Shaders\Buffers\StructuredBuffer.h"
#include "Shaders\Variables\Texture2DArray.h"
#include "Models\Animation\AnimationPlayer.h"
#include "GameObject\Components\MeshMaterialGOC.h"
#include "GameObject\Components\MeshAnimationGOC.h"
#include "Graphic Extensions\Materials\BuiltInMaterialsDefs.h"
#include "Graphic Extensions\Shader Extensions\Properties\ShaderProperties.h"

//Always include last
#include "Memory\MemLeaks.h"

/********************
*   Function Defs   *
*********************/
AETODO("Check object instances and calls to where it is init");
FPRObjectDraw::FPRObjectDraw(GameApp& gameApp, GameResourceManager& gameResourceManager, GraphicDevice& graphicDevice, const std::string& gameComponentName, const std::string& fprServiceName, const std::string& cameraServiceName, uint32_t callOrder)
    : DrawableGameComponent(gameApp, gameResourceManager, graphicDevice, gameComponentName, callOrder)
{
    m_ForwardPlusRendering = m_GameApp.GetGameService<ForwardPlusRendering>(fprServiceName);
    AEAssert(m_ForwardPlusRendering != nullptr);

    m_CameraUpdater = m_GameApp.GetGameService<CameraUpdater>(cameraServiceName);
    AEAssert(m_CameraUpdater != nullptr);
}

FPRObjectDraw::~FPRObjectDraw()
{
}

void FPRObjectDraw::Initialize()
{
    m_GameObjectManager = m_GameApp.GetGameObjectManager();

    DrawableGameComponent::Initialize();
}

void FPRObjectDraw::LoadContent()
{
    DrawableGameComponent::LoadContent();
}

void FPRObjectDraw::Update(const TimerParams& timerParams)
{
    DrawableGameComponent::Update(timerParams);
}

void FPRObjectDraw::Render(const TimerParams& timerParams)
{
    m_GraphicDevice.BeginEvent("Forward+ Rendering Object Draw");

    AEResult ret = RenderWithFPR();

    if(ret != AEResult::Ok)
    {
        AETODO("Log error");
    }

    m_GraphicDevice.EndEvent();

    DrawableGameComponent::Render(timerParams);
}

AEResult FPRObjectDraw::RenderWithFPR()
{
    AEResult ret = AEResult::Ok;

    DrawGameObjects();

    return AEResult::Ok;
}

void FPRObjectDraw::DrawGameObjects()
{
    for(auto goIt : *m_GameObjectManager)
    {
        DrawGameObject(goIt.second);
    }
}

void FPRObjectDraw::DrawGameObject(GameObject* gameObject)
{
    AEAssert(gameObject != nullptr);
    if(gameObject == nullptr)
    {
        return;
    }

    if(gameObject->HasMeshGOC() && gameObject->HasMaterialGOCs())
    {
        Mesh* mesh = gameObject->GetMeshGOC()->GetMeshResource();

        Camera* currentCamera = m_CameraUpdater->GetMainCamera();

        if (mesh != nullptr && currentCamera != nullptr)
        {
            const MeshMaterialsGOCList& matList = gameObject->GetMeshMaterialsGOCList();
            auto matListIt = matList.begin();

            for(uint32_t i = 0; i < mesh->GetNumberMeshParts() && i < matList.size(); i++, matListIt++)
            {
                MeshPart* meshPart = mesh->GetMeshPart(i);

                MeshMaterialGOC* meshMat = *matListIt;

                ShaderProperties* vsProps = meshMat->GetVertexShaderProperties();
                ShaderProperties* psProps = meshMat->GetPixelShaderProperties();

                if (vsProps != nullptr && psProps != nullptr)
                {
                    ConstantBuffer* cbVS = vsProps->GetConstantBuffer(AE_CB_WORLD_VIEW_PROJ_NAME);
                    if (cbVS == nullptr)
                    {
                        cbVS = vsProps->GetConstantBuffer(AE_CB_VIEW_PROJ_NAME);
                    }
                    else
                    {
                        cbVS->SetValueT<glm::mat4>(AE_CB_WORLD_VAR_NAME, gameObject->GetWorldTransform());
                    }

                    if (cbVS != nullptr)
                    {
                        cbVS->SetValueT<glm::mat4>(AE_CB_VIEW_VAR_NAME, currentCamera->GetViewMatrix());
                        cbVS->SetValueT<glm::mat4>(AE_CB_PROJECTION_VAR_NAME, currentCamera->GetProjectionMatrix());
                    }

                    if (gameObject->HasMeshAnimationGOC())
                    {
                        ConstantBuffer* cbBones = vsProps->GetConstantBuffer(AE_CB_BONES_NAME);

                        if (cbBones != nullptr)
                        {
                            AnimationPlayer* player = gameObject->GetMeshAnimationGOC()->GetAnimationPlayer();
                            cbBones->SetValue(AE_CB_BONE_TRANSFORMS_VAR_NAME, player->GetBoneTransforms(), sizeof(glm::mat4) * AE_MAX_BONES);
                        }
                    }

                    ConstantBuffer* cbPS = psProps->GetConstantBuffer(AE_CB_FPR_LIGHTS_NAME);
                    if (cbPS != nullptr)
                    {
                        cbPS->SetValueT<glm::vec3>(AE_CB_CAMERA_POS_VAR_NAME, currentCamera->GetPosition());
                        cbPS->SetValueT<uint32_t>(AE_CB_WINDOW_WIDTH_VAR_NAME, m_GraphicDevice.GetGraphicPP().m_BackBufferWidth);
                    }

                    /////////////////////////////////////////////////////////
                    //Get Current Shader Variables
                    SimpleBuffer* currentPerTileLightIndexBuffer        = psProps->GetSimpleBuffer(AE_BF_PER_TILE_LIGHT_INDEX_BUFFER_NAME);
                    StructuredBuffer* currentLightBuffer                = psProps->GetStructuredBuffer(AE_BF_LIGHT_BUFFER_NAME);
                    StructuredBuffer* currentShadowSpotLightInfoBuffer  = psProps->GetStructuredBuffer(AE_BF_SHADOW_SPOT_LIGHT_INFO_BUFFER_NAME);
                    StructuredBuffer* currentShadowDirLightInfoBuffer   = psProps->GetStructuredBuffer(AE_BF_SHADOW_DIR_LIGHT_INFO_BUFFER_NAME);
                    TextureArray* currentSpotLightShadowTextureArray    = psProps->GetTextureArray(AE_TXA_SPOT_LIGHT_SHADOW_TEXTURE_ARRAY_NAME);
                    TextureArray* currentDirLightShadowTextureArray     = psProps->GetTextureArray(AE_TXA_DIR_LIGHT_SHADOW_TEXTURE_ARRAY_NAME);
                    Sampler* currentShadowTextureSample                 = psProps->GetSampler(AE_SAM_SHADOW_TEXTURE_SAMPLER_NAME);

                    /////////////////////////////////////////////////////////
                    //Override Light Buffer to use a single one
                    StructuredBuffer* forwardPlusLightBuffer = m_ForwardPlusRendering->GetLightStructuredBuffer();
                    if (currentLightBuffer != nullptr && currentLightBuffer->GetUniqueID() != forwardPlusLightBuffer->GetUniqueID())
                    {
                        AETODO("Check Return");
                        psProps->OverrideStructuredBuffer(AE_BF_LIGHT_BUFFER_NAME, forwardPlusLightBuffer);
                    }

                    /////////////////////////////////////////////////////////
                    //Override Per Tile Light Index to use a single one
                    SimpleBuffer* forwardPlusPerTileLightIndexBuffer = m_ForwardPlusRendering->GetPerTileLightIndexBuffer();
                    if (currentPerTileLightIndexBuffer != nullptr && currentPerTileLightIndexBuffer->GetUniqueID() != forwardPlusPerTileLightIndexBuffer->GetUniqueID())
                    {
                        AETODO("Check Return");
                        psProps->OverrideSimpleBuffer(AE_BF_PER_TILE_LIGHT_INDEX_BUFFER_NAME, forwardPlusPerTileLightIndexBuffer, true, false, true, AE_BI_PER_TILE_LIGHT_INDEX_BUFFER_DRAW_BIND_IDX);
                    }

                    /////////////////////////////////////////////////////////
                    //Override Shadow Spot Light Info Buffer to use a single one
                    StructuredBuffer* forwardShadowSpotLightInfo = m_ForwardPlusRendering->GetShadowSpotLightInfoStructuredBuffer();
                    if (currentShadowSpotLightInfoBuffer != nullptr && currentShadowSpotLightInfoBuffer->GetUniqueID() != forwardShadowSpotLightInfo->GetUniqueID())
                    {
                        AETODO("Check Return");
                        psProps->OverrideStructuredBuffer(AE_BF_SHADOW_SPOT_LIGHT_INFO_BUFFER_NAME, forwardShadowSpotLightInfo);
                    }

                    /////////////////////////////////////////////////////////
                    //Override Shadow Spot Light Texture Array to use a single one
                    Texture2DArray* forwardSpotLightShadowTextureArray = m_GameApp.GetLightManager()->GetSpotLightShadowTextureArray();
                    if (currentSpotLightShadowTextureArray != nullptr && currentSpotLightShadowTextureArray->GetUniqueID() != forwardSpotLightShadowTextureArray->GetUniqueID())
                    {
                        AETODO("Check Return");
                        psProps->OverrideTextureArray(AE_TXA_SPOT_LIGHT_SHADOW_TEXTURE_ARRAY_NAME, forwardSpotLightShadowTextureArray);
                    }

                    /////////////////////////////////////////////////////////
                    //Override Shadow Spot Light Info Buffer to use a single one
                    StructuredBuffer* forwardShadowDirLightInfo = m_ForwardPlusRendering->GetShadowDirLightInfoStructuredBuffer();
                    if (currentShadowDirLightInfoBuffer != nullptr && currentShadowDirLightInfoBuffer->GetUniqueID() != forwardShadowDirLightInfo->GetUniqueID())
                    {
                        AETODO("Check Return");
                        psProps->OverrideStructuredBuffer(AE_BF_SHADOW_DIR_LIGHT_INFO_BUFFER_NAME, forwardShadowDirLightInfo);
                    }

                    /////////////////////////////////////////////////////////
                    //Override Shadow Directional Light Texture Array to use a single one
                    Texture2DArray* forwardDirLightShadowTextureArray = m_GameApp.GetLightManager()->GetDirLightShadowTextureArray();
                    if (currentDirLightShadowTextureArray != nullptr && currentDirLightShadowTextureArray->GetUniqueID() != forwardDirLightShadowTextureArray->GetUniqueID())
                    {
                        AETODO("Check Return");
                        psProps->OverrideTextureArray(AE_TXA_DIR_LIGHT_SHADOW_TEXTURE_ARRAY_NAME, forwardDirLightShadowTextureArray);
                    }

                    /////////////////////////////////////////////////////////
                    //Override Shadow Texture Sampler to use a single one
                    Sampler* forwardShadowTextureSampler = m_ForwardPlusRendering->GetShadowTextureSampler();
                    if (currentShadowTextureSample != nullptr && currentShadowTextureSample->GetUniqueID() != forwardShadowTextureSampler->GetUniqueID())
                    {
                        AETODO("Check Return");
                        psProps->OverrideSampler(AE_SAM_SHADOW_TEXTURE_SAMPLER_NAME, forwardShadowTextureSampler);
                    }

                    meshMat->ApplyShaders(m_GraphicDevice);

                    m_GraphicDevice.SetVertexBuffer(meshPart->GetVertexBuffer());
                    m_GraphicDevice.SetIndexBuffer(meshPart->GetIndexBuffer());

                    m_GraphicDevice.SetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

                    m_GraphicDevice.DrawIndexed(0, 0, meshPart->GetIndexBuffer()->GetSize());

                    meshMat->UnApplyShaders(m_GraphicDevice);
                }
            }
        }
    }

    for(auto goIt : *gameObject)
    {
        DrawGameObject(goIt.second);
    }
}

void FPRObjectDraw::OnLostDevice()
{
    DrawableGameComponent::OnLostDevice();
}

void FPRObjectDraw::OnResetDevice()
{
    DrawableGameComponent::OnResetDevice();
}