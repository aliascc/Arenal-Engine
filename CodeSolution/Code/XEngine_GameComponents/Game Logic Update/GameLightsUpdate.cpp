/********************************************************
*
* Author: Carlos Chacón N.
*
* Copyright 2012-2015
*
*********************************************************/


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
#include "Time\XETimeDefs.h"
#include "GameLightsUpdate.h"
#include "Math\XEMathDefs.h"
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
	XEAssert(m_CameraUpdater != nullptr);
}

GameLightsUpdate::~GameLightsUpdate()
{
	DeleteMem(m_SpotLightShadowViewport);
	DeleteMem(m_VarianceShadowMaterial);
	DeleteMem(m_VarianceSkinningShadowMaterial);
	XERelease(m_SpotLightShadowTexturesDS);
	XERelease(m_DirLightShadowTexturesDS);

	for (size_t i = 0; i < m_DirLightShadowViewports.size(); i++)
	{
		DeleteMem(m_DirLightShadowViewports[i]);
	}
}

void GameLightsUpdate::Initialize()
{
	m_VarianceShadowMaterial = new VarianceShadowMaterial(m_GraphicDevice, m_GameResourceManager);

	m_VarianceSkinningShadowMaterial = new VarianceSkinningShadowMaterial(m_GraphicDevice, m_GameResourceManager);

	XETODO("Set name in macro");
	m_SpotLightShadowTexturesDS = new DepthStencilSurface(m_GraphicDevice, L"XE Spot Light Shadow Textures DS");

	m_SpotLightShadowViewport = new Viewport();

	XETODO("Set name in macro");
	m_DirLightShadowTexturesDS = new DepthStencilSurface(m_GraphicDevice, L"XE Dir Light Shadow Textures DS");

	for (uint32_t i = 0; i < XE_LIGHT_NUM_CASCADE_MAPS; i++)
	{
		Viewport* cascadeViewPort = new Viewport();

		m_DirLightShadowViewports.push_back(cascadeViewPort);
	}

	DrawableGameComponent::Initialize();
}

void GameLightsUpdate::LoadContent()
{
	XETODO("Check return");
	m_VarianceShadowMaterial->LoadContent();

	XETODO("Check return");
	m_VarianceSkinningShadowMaterial->LoadContent();

	XETODO("Check return");
	m_SpotLightShadowTexturesDS->InitializeDepthStencilSurface(XE_LIGHT_SPOT_SHADOW_TEXTURE_WIDTH, XE_LIGHT_SPOT_SHADOW_TEXTURE_HEIGHT, DXGI_FORMAT_R24G8_TYPELESS, DXGI_FORMAT_D24_UNORM_S8_UINT, DXGI_FORMAT_R24_UNORM_X8_TYPELESS);

	XETODO("Check return");
	m_SpotLightShadowViewport->Initialize(0.0f, 0.0f, XE_LIGHT_SPOT_SHADOW_TEXTURE_WIDTH, XE_LIGHT_SPOT_SHADOW_TEXTURE_HEIGHT, 0.0f, 1.0f);

	XETODO("Check return");
	m_DirLightShadowTexturesDS->InitializeDepthStencilSurface(XE_LIGHT_DIR_SHADOW_TEXTURE_WIDTH * XE_LIGHT_NUM_CASCADE_MAPS, XE_LIGHT_DIR_SHADOW_TEXTURE_HEIGHT, DXGI_FORMAT_R24G8_TYPELESS, DXGI_FORMAT_D24_UNORM_S8_UINT, DXGI_FORMAT_R24_UNORM_X8_TYPELESS);

	for (uint32_t i = 0; i < XE_LIGHT_NUM_CASCADE_MAPS; i++)
	{
		XETODO("Check return");
		m_DirLightShadowViewports[i]->Initialize((float)(i * XE_LIGHT_DIR_SHADOW_TEXTURE_WIDTH), 0.0f, (float)XE_LIGHT_DIR_SHADOW_TEXTURE_WIDTH, (float)XE_LIGHT_DIR_SHADOW_TEXTURE_HEIGHT, 0.0f, 1.0f);
	}

	DrawableGameComponent::LoadContent();
}

void GameLightsUpdate::Update(const TimerParams& timerParams)
{
	///////////////////////////////////////////
	//Get Game Object Manager & Light Manager
	GameObjectManager* gameObjectManager = m_GameApp->GetGameObjectManager();

	LightManager* lightManager = m_GameApp->GetLightManager();

	XEAssert(gameObjectManager != nullptr);
	XEAssert(lightManager != nullptr);
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
	//Camera camera(L"Dummy Test", glm::vec3(0.0f, 0.0f, -5.0f), glm::vec3(0.0f, 0.0f, 0.0f), XEMathHelpers::Vec3fUp, dimension, 45.0f, 1.0f, 1000.0f);
	//camera.Initialize();
	XEResult ret = lightManager->Update(camera);
	if(ret != XEResult::Ok)
	{
		XETODO("Log info");
	}

	DrawableGameComponent::Update(timerParams);
}

void GameLightsUpdate::UpdateGameObjectLight(GameObject* gameObject)
{
	XEAssert(gameObject != nullptr);
	if (gameObject == nullptr)
	{
		return;
	}

	if (gameObject->HasLightGOC())
	{
		Light* light = gameObject->GetLightGOC()->GetLight();

		XEAssert(light != nullptr)
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

	XETODO("Check return");
	ShadowSpotLightRenderGameObject();

	m_GraphicDevice->EndEvent(); //Spot Light Shadow Render

	m_GraphicDevice->BeginEvent(L"Directional Light Shadow Render");

	XETODO("Check return");
	ShadowDirLightRenderGameObject();

	m_GraphicDevice->EndEvent(); //Spot Light Shadow Render

	m_GraphicDevice->EndEvent(); //Game Lights Update

	DrawableGameComponent::Render(timerParams);
}

XEResult GameLightsUpdate::ShadowDirLightRenderGameObject()
{
	LightManager* lightManager = m_GameApp->GetLightManager();
	GameObjectManager* gameObjectManager = m_GameApp->GetGameObjectManager();

	if (lightManager->GetNumDirLightsWithShadows() == 0)
	{
		return XEResult::Ok;
	}

	Texture2DArray* shadowTextureArray = lightManager->GetDirLightShadowTextureArray();

	XETODO("Check return");
	RenderTarget* rtsDS[1] = { nullptr };
	m_GraphicDevice->SetRenderTargetsAndDepthStencil(1, rtsDS, m_DirLightShadowTexturesDS);

	for (auto lightIt : *lightManager)
	{
		Light* light = lightIt.second;

		if (light->GetLightType() == LightType::Directional && light->IsShadowEnabled())
		{
			DirectionalLight* dirLight = reinterpret_cast<DirectionalLight*>(light);

			uint32_t idxs[1] = { light->GetShadowTextureIndex() };
			XETODO("Check return");
			m_GraphicDevice->SetRenderTargets(1, idxs, shadowTextureArray);
			m_GraphicDevice->Clear(true, 0, true, true, XEColors::Transparent);

			for (uint32_t i = 0; i < XE_LIGHT_NUM_CASCADE_MAPS; i++)
			{
				XETODO("Check return");
				m_GraphicDevice->SetViewport(m_DirLightShadowViewports[i]);

				const LightCascadeInfo& lightCascadeInfo = dirLight->GetLightCascadeInfo();

				for (auto goIt : *gameObjectManager)
				{
					XETODO("Check return");
					ShadowLightRenderGameObject(goIt.second, lightCascadeInfo.m_CascadeViewMatrix[i], lightCascadeInfo.m_CascadeProjectionMatrix[i]);
				}
			}
		}
	}

	XETODO("Check return");
	m_GraphicDevice->ResetViewport();

	XETODO("Check return");
	m_GraphicDevice->ResetRenderTargetAndSetDepthStencil();

	return XEResult::Ok;
}

XEResult GameLightsUpdate::ShadowSpotLightRenderGameObject()
{
	LightManager* lightManager = m_GameApp->GetLightManager();
	GameObjectManager* gameObjectManager = m_GameApp->GetGameObjectManager();

	if (lightManager->GetNumSpotLightsWithShadows() == 0)
	{
		return XEResult::Ok;
	}

	Texture2DArray* shadowTextureArray = lightManager->GetSpotLightShadowTextureArray();
	
	XETODO("Check return");
	RenderTarget* rtsDS[1] = { nullptr };
	m_GraphicDevice->SetRenderTargetsAndDepthStencil(1, rtsDS, m_SpotLightShadowTexturesDS);

	XETODO("Check return");
	m_GraphicDevice->SetViewport(m_SpotLightShadowViewport);

	for (auto lightIt : *lightManager)
	{
		Light* light = lightIt.second;

		if (light->GetLightType() == LightType::Spot && light->IsShadowEnabled())
		{
			uint32_t idxs[1] = { light->GetShadowTextureIndex() };
			XETODO("Check return");
			m_GraphicDevice->SetRenderTargets(1, idxs, shadowTextureArray);
			m_GraphicDevice->Clear(true, 0, true, true, XEColors::Transparent);

			for (auto goIt : *gameObjectManager)
			{
				XETODO("Check return");
				ShadowLightRenderGameObject(goIt.second, light->GetViewMatrix(), light->GetProjectionMatrix());
			}
		}
	}

	XETODO("Check return");
	m_GraphicDevice->ResetViewport();

	XETODO("Check return");
	m_GraphicDevice->ResetRenderTargetAndSetDepthStencil();

	return XEResult::Ok;
}

XEResult GameLightsUpdate::ShadowLightRenderGameObject(GameObject* gameObject, const glm::mat4& lightView, const glm::mat4& lightProj)
{
	XEAssert(gameObject != nullptr);
	if (gameObject == nullptr)
	{
		return XEResult::NullParameter;
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
				cb = vsProps->GetConstantBuffer(XE_CB_WORLD_VIEW_PROJ_NAME);

				ConstantBuffer* cbBones = vsProps->GetConstantBuffer(XE_CB_BONES_NAME);
				if (cbBones != nullptr)
				{
					AnimationPlayer* player = gameObject->GetMeshAnimationGOC()->GetAnimationPlayer();
					cbBones->SetValue(XE_CB_BONE_TRANSFORMS_VAR_NAME, player->GetBoneTransforms(), sizeof(glm::mat4) * XE_MAX_BONES);
				}
			}
			else
			{
				mat = m_VarianceShadowMaterial;
				ShaderProperties* vsProps = m_VarianceShadowMaterial->GetVSProps();
				cb = vsProps->GetConstantBuffer(XE_CB_WORLD_VIEW_PROJ_NAME);
			}

			cb->SetValueT<glm::mat4>(XE_CB_WORLD_VAR_NAME, gameObject->GetWorldTransform());
			cb->SetValueT<glm::mat4>(XE_CB_VIEW_VAR_NAME, lightView);
			cb->SetValueT<glm::mat4>(XE_CB_PROJECTION_VAR_NAME, lightProj);

			XETODO("check return");
			mat->Apply();

			for (uint32_t i = 0; i < mesh->GetNumberMeshParts(); i++)
			{
				MeshPart* meshPart = mesh->GetMeshPart(i);

				m_GraphicDevice->SetVertexBuffer(meshPart->GetVertexBuffer());
				m_GraphicDevice->SetIndexBuffer(meshPart->GetIndexBuffer());

				m_GraphicDevice->SetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

				m_GraphicDevice->DrawIndexed(0, 0, meshPart->GetIndexBuffer()->GetSize());
			}

			XETODO("check return");
			mat->UnApply();
		}
	}

	for (auto goIt : *gameObject)
	{
		ShadowLightRenderGameObject(goIt.second, lightView, lightProj);
	}

	return XEResult::Ok;
}