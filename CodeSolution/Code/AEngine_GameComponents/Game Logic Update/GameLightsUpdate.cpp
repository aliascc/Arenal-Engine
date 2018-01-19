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


/**********************
*   System Includes   *
***********************/

/***************************
*   Game Engine Includes   *
****************************/
#include "Base\Base.h"
#include "Models\Mesh.h"
#include "Lights\Light.h"
#include "Camera\Camera.h"
#include "GraphicDevice.h"
#include "Utils\Viewport.h"
#include "Models\MeshPart.h"
#include "GameApp\GameApp.h"
#include "Time\AETimeDefs.h"
#include "GameLightsUpdate.h"
#include "Math\AEMathDefs.h"
#include "Vertex\IndexBuffer.h"
#include "Lights\LightManager.h"
#include "Camera\CameraUpdater.h"
#include "GameObject\GameObject.h"
#include "Textures\RenderTarget.h"
#include "Lights\DirectionalLight.h"
#include "Textures\DepthStencilSurface.h"
#include "GameObject\GameObjectManager.h"
#include "GameObject\Components\MeshGOC.h"
#include "GameObject\Components\LightGOC.h"
#include "Shaders\Buffers\ConstantBuffer.h"
#include "Models\Animation\AnimationPlayer.h"
#include "GameObject\Components\MeshAnimationGOC.h"
#include "Graphic Extensions\Materials\Shadows\VarianceShadowMaterial.h"
#include "Graphic Extensions\Shader Extensions\Properties\ShaderProperties.h"
#include "Graphic Extensions\Materials\Shadows\VarianceSkinningShadowMaterial.h"

//Always include last
#include "Memory\MemLeaks.h"

/********************
*   Function Defs   *
*********************/
GameLightsUpdate::GameLightsUpdate(GameApp* gameApp, const std::wstring& gameComponentName, const std::wstring& cameraServiceName, uint32_t callOrder)
    : DrawableGameComponent(gameApp, gameComponentName, callOrder)
{
    m_CameraUpdater = m_GameApp->GetGameService<CameraUpdater>(cameraServiceName);
    AEAssert(m_CameraUpdater != nullptr);
}

GameLightsUpdate::~GameLightsUpdate()
{
    DeleteMem(m_SpotLightShadowViewport);
    DeleteMem(m_VarianceShadowMaterial);
    DeleteMem(m_VarianceSkinningShadowMaterial);
    AERelease(m_SpotLightShadowTexturesDS);
    AERelease(m_DirLightShadowTexturesDS);

    for (size_t i = 0; i < m_DirLightShadowViewports.size(); i++)
    {
        DeleteMem(m_DirLightShadowViewports[i]);
    }
}

void GameLightsUpdate::Initialize()
{
    m_VarianceShadowMaterial = new VarianceShadowMaterial(m_GraphicDevice, m_GameResourceManager);

    m_VarianceSkinningShadowMaterial = new VarianceSkinningShadowMaterial(m_GraphicDevice, m_GameResourceManager);

    AETODO("Set name in macro");
    m_SpotLightShadowTexturesDS = new DepthStencilSurface(m_GraphicDevice, L"AE Spot Light Shadow Textures DS");

    m_SpotLightShadowViewport = new Viewport();

    AETODO("Set name in macro");
    m_DirLightShadowTexturesDS = new DepthStencilSurface(m_GraphicDevice, L"AE Dir Light Shadow Textures DS");

    for (uint32_t i = 0; i < AE_LIGHT_NUM_CASCADE_MAPS; i++)
    {
        Viewport* cascadeViewPort = new Viewport();

        m_DirLightShadowViewports.push_back(cascadeViewPort);
    }

    DrawableGameComponent::Initialize();
}

void GameLightsUpdate::LoadContent()
{
    AETODO("Check return");
    m_VarianceShadowMaterial->LoadContent();

    AETODO("Check return");
    m_VarianceSkinningShadowMaterial->LoadContent();

    AETODO("Check return");
    m_SpotLightShadowTexturesDS->InitializeDepthStencilSurface(AE_LIGHT_SPOT_SHADOW_TEXTURE_WIDTH, AE_LIGHT_SPOT_SHADOW_TEXTURE_HEIGHT, DXGI_FORMAT_R24G8_TYPELESS, DXGI_FORMAT_D24_UNORM_S8_UINT, DXGI_FORMAT_R24_UNORM_X8_TYPELESS);

    AETODO("Check return");
    m_SpotLightShadowViewport->Initialize(0.0f, 0.0f, AE_LIGHT_SPOT_SHADOW_TEXTURE_WIDTH, AE_LIGHT_SPOT_SHADOW_TEXTURE_HEIGHT, 0.0f, 1.0f);

    AETODO("Check return");
    m_DirLightShadowTexturesDS->InitializeDepthStencilSurface(AE_LIGHT_DIR_SHADOW_TEXTURE_WIDTH * AE_LIGHT_NUM_CASCADE_MAPS, AE_LIGHT_DIR_SHADOW_TEXTURE_HEIGHT, DXGI_FORMAT_R24G8_TYPELESS, DXGI_FORMAT_D24_UNORM_S8_UINT, DXGI_FORMAT_R24_UNORM_X8_TYPELESS);

    for (uint32_t i = 0; i < AE_LIGHT_NUM_CASCADE_MAPS; i++)
    {
        AETODO("Check return");
        m_DirLightShadowViewports[i]->Initialize((float)(i * AE_LIGHT_DIR_SHADOW_TEXTURE_WIDTH), 0.0f, (float)AE_LIGHT_DIR_SHADOW_TEXTURE_WIDTH, (float)AE_LIGHT_DIR_SHADOW_TEXTURE_HEIGHT, 0.0f, 1.0f);
    }

    DrawableGameComponent::LoadContent();
}

void GameLightsUpdate::Update(const TimerParams& timerParams)
{
    ///////////////////////////////////////////
    //Get Game Object Manager & Light Manager
    GameObjectManager* gameObjectManager = m_GameApp->GetGameObjectManager();

    LightManager* lightManager = m_GameApp->GetLightManager();

    AEAssert(gameObjectManager != nullptr);
    AEAssert(lightManager != nullptr);
    if(gameObjectManager == nullptr || lightManager == nullptr)
    {
        return;
    }

    ///////////////////////////////////////////
    //Update all Light Object information
    for(auto goIt : *gameObjectManager)
    {
        UpdateGameObjectLight(goIt.second);
    }

    ///////////////////////////////////////////
    //Update Light Manager
    Camera* camera = m_CameraUpdater->GetMainCamera();

    //glm::ivec2 dimension(m_GraphicDevice->GetGraphicPP().m_BackBufferWidth, m_GraphicDevice->GetGraphicPP().m_BackBufferHeight);
    //Camera camera(L"Dummy Test", glm::vec3(0.0f, 0.0f, -5.0f), glm::vec3(0.0f, 0.0f, 0.0f), AEMathHelpers::Vec3fUp, dimension, 45.0f, 1.0f, 1000.0f);
    //camera.Initialize();
    AEResult ret = lightManager->Update(camera);
    if(ret != AEResult::Ok)
    {
        AETODO("Log info");
    }

    DrawableGameComponent::Update(timerParams);
}

void GameLightsUpdate::UpdateGameObjectLight(GameObject* gameObject)
{
    AEAssert(gameObject != nullptr);
    if (gameObject == nullptr)
    {
        return;
    }

    if (gameObject->HasLightGOC())
    {
        Light* light = gameObject->GetLightGOC()->GetLight();

        if (light != nullptr && light->IsEnabled())
        {
            light->SetPosition(gameObject->GetWorldPosition());

            if (light->GetLightType() == LightType::Spot || light->GetLightType() == LightType::Directional)
            {
                glm::vec3 direction = gameObject->GetDirection();

                light->SetDirection(direction);
            }
        }
    }

    for (auto goIt : *gameObject)
    {
        UpdateGameObjectLight(goIt.second);
    }
}

void GameLightsUpdate::Render(const TimerParams& timerParams)
{
    m_GraphicDevice->BeginEvent(L"Game Lights Update");

    m_GraphicDevice->BeginEvent(L"Spot Light Shadow Render");

    AETODO("Check return");
    ShadowSpotLightRenderGameObject();

    m_GraphicDevice->EndEvent(); //Spot Light Shadow Render

    m_GraphicDevice->BeginEvent(L"Directional Light Shadow Render");

    AETODO("Check return");
    ShadowDirLightRenderGameObject();

    m_GraphicDevice->EndEvent(); //Spot Light Shadow Render

    m_GraphicDevice->EndEvent(); //Game Lights Update

    DrawableGameComponent::Render(timerParams);
}

AEResult GameLightsUpdate::ShadowDirLightRenderGameObject()
{
    LightManager* lightManager = m_GameApp->GetLightManager();
    GameObjectManager* gameObjectManager = m_GameApp->GetGameObjectManager();

    if (lightManager->GetNumDirLightsWithShadows() == 0)
    {
        return AEResult::Ok;
    }

    Texture2DArray* shadowTextureArray = lightManager->GetDirLightShadowTextureArray();

    AETODO("Check return");
    RenderTarget* rtsDS[1] = { nullptr };
    m_GraphicDevice->SetRenderTargetsAndDepthStencil(1, rtsDS, m_DirLightShadowTexturesDS);

    for (auto lightIt : *lightManager)
    {
        Light* light = lightIt.second;

        if (light->GetLightType() == LightType::Directional && light->IsShadowEnabled())
        {
            DirectionalLight* dirLight = reinterpret_cast<DirectionalLight*>(light);

            uint32_t idxs[1] = { light->GetShadowTextureIndex() };
            AETODO("Check return");
            m_GraphicDevice->SetRenderTargets(1, idxs, shadowTextureArray);
            m_GraphicDevice->Clear(true, 0, true, true, AEColors::Transparent);

            for (uint32_t i = 0; i < AE_LIGHT_NUM_CASCADE_MAPS; i++)
            {
                AETODO("Check return");
                m_GraphicDevice->SetViewport(m_DirLightShadowViewports[i]);

                const LightCascadeInfo& lightCascadeInfo = dirLight->GetLightCascadeInfo();

                for (auto goIt : *gameObjectManager)
                {
                    AETODO("Check return");
                    ShadowLightRenderGameObject(goIt.second, lightCascadeInfo.m_CascadeViewMatrix[i], lightCascadeInfo.m_CascadeProjectionMatrix[i]);
                }
            }
        }
    }

    AETODO("Check return");
    m_GraphicDevice->ResetViewport();

    AETODO("Check return");
    m_GraphicDevice->ResetRenderTargetAndSetDepthStencil();

    return AEResult::Ok;
}

AEResult GameLightsUpdate::ShadowSpotLightRenderGameObject()
{
    LightManager* lightManager = m_GameApp->GetLightManager();
    GameObjectManager* gameObjectManager = m_GameApp->GetGameObjectManager();

    if (lightManager->GetNumSpotLightsWithShadows() == 0)
    {
        return AEResult::Ok;
    }

    Texture2DArray* shadowTextureArray = lightManager->GetSpotLightShadowTextureArray();
    
    AETODO("Check return");
    RenderTarget* rtsDS[1] = { nullptr };
    m_GraphicDevice->SetRenderTargetsAndDepthStencil(1, rtsDS, m_SpotLightShadowTexturesDS);

    AETODO("Check return");
    m_GraphicDevice->SetViewport(m_SpotLightShadowViewport);

    for (auto lightIt : *lightManager)
    {
        Light* light = lightIt.second;

        if (light->GetLightType() == LightType::Spot && light->IsShadowEnabled())
        {
            uint32_t idxs[1] = { light->GetShadowTextureIndex() };
            AETODO("Check return");
            m_GraphicDevice->SetRenderTargets(1, idxs, shadowTextureArray);
            m_GraphicDevice->Clear(true, 0, true, true, AEColors::Transparent);

            for (auto goIt : *gameObjectManager)
            {
                AETODO("Check return");
                ShadowLightRenderGameObject(goIt.second, light->GetViewMatrix(), light->GetProjectionMatrix());
            }
        }
    }

    AETODO("Check return");
    m_GraphicDevice->ResetViewport();

    AETODO("Check return");
    m_GraphicDevice->ResetRenderTargetAndSetDepthStencil();

    return AEResult::Ok;
}

AEResult GameLightsUpdate::ShadowLightRenderGameObject(GameObject* gameObject, const glm::mat4& lightView, const glm::mat4& lightProj)
{
    AEAssert(gameObject != nullptr);
    if (gameObject == nullptr)
    {
        return AEResult::NullParameter;
    }

    if (gameObject->HasMeshGOC() && gameObject->HasMaterialGOCs())
    {
        Mesh* mesh = gameObject->GetMeshGOC()->GetMeshResource();

        if (mesh != nullptr)
        {
            ConstantBuffer* cb = nullptr;
            Material* mat = nullptr;
            bool hasAnimation = gameObject->HasMeshAnimationGOC();

            if (hasAnimation)
            {
                mat = m_VarianceSkinningShadowMaterial;
                ShaderProperties* vsProps = m_VarianceSkinningShadowMaterial->GetVSProps();
                cb = vsProps->GetConstantBuffer(AE_CB_WORLD_VIEW_PROJ_NAME);

                ConstantBuffer* cbBones = vsProps->GetConstantBuffer(AE_CB_BONES_NAME);
                if (cbBones != nullptr)
                {
                    AnimationPlayer* player = gameObject->GetMeshAnimationGOC()->GetAnimationPlayer();
                    cbBones->SetValue(AE_CB_BONE_TRANSFORMS_VAR_NAME, player->GetBoneTransforms(), sizeof(glm::mat4) * AE_MAX_BONES);
                }
            }
            else
            {
                mat = m_VarianceShadowMaterial;
                ShaderProperties* vsProps = m_VarianceShadowMaterial->GetVSProps();
                cb = vsProps->GetConstantBuffer(AE_CB_WORLD_VIEW_PROJ_NAME);
            }

            cb->SetValueT<glm::mat4>(AE_CB_WORLD_VAR_NAME, gameObject->GetWorldTransform());
            cb->SetValueT<glm::mat4>(AE_CB_VIEW_VAR_NAME, lightView);
            cb->SetValueT<glm::mat4>(AE_CB_PROJECTION_VAR_NAME, lightProj);

            AETODO("check return");
            mat->Apply();

            for (uint32_t i = 0; i < mesh->GetNumberMeshParts(); i++)
            {
                MeshPart* meshPart = mesh->GetMeshPart(i);

                m_GraphicDevice->SetVertexBuffer(meshPart->GetVertexBuffer());
                m_GraphicDevice->SetIndexBuffer(meshPart->GetIndexBuffer());

                m_GraphicDevice->SetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

                m_GraphicDevice->DrawIndexed(0, 0, meshPart->GetIndexBuffer()->GetSize());
            }

            AETODO("check return");
            mat->UnApply();
        }
    }

    for (auto goIt : *gameObject)
    {
        ShadowLightRenderGameObject(goIt.second, lightView, lightProj);
    }

    return AEResult::Ok;
}
