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
#include <string>

/*************************
*   3rd Party Includes   *
**************************/

/***************************
*   Game Engine Includes   *
****************************/
#include "FPRPreZ.h"
#include "Models\Mesh.h"
#include "Time\AETime.h"
#include "Camera\Camera.h"
#include "GraphicDevice.h"
#include "GameApp\GameApp.h"
#include "Models\MeshPart.h"
#include "Base\BaseFunctions.h"
#include "Vertex\IndexBuffer.h"
#include "Camera\CameraUpdater.h"
#include "ForwardPlusRendering.h"
#include "GameObject\GameObject.h"
#include "Textures\RenderTarget.h"
#include "Textures\DepthStencilSurface.h"
#include "GameObject\GameObjectManager.h"
#include "GameObject\Components\MeshGOC.h"
#include "Shaders\Buffers\ConstantBuffer.h"
#include "Models\Animation\AnimationPlayer.h"
#include "GameObject\Components\MeshAnimationGOC.h"
#include "Graphic Extensions\Shader Extensions\Properties\ShaderProperties.h"
#include "Graphic Extensions\Materials\Forward+ Rendering\ForwardPlusZPrePassMaterial.h"
#include "Graphic Extensions\Materials\Forward+ Rendering\ForwardPlusZPrePassSkinningMaterial.h"

//Always include last
#include "Memory\MemLeaks.h"

/********************
*   Function Defs   *
*********************/
AETODO("Check object instances and calls to where it is init");
FPRPreZ::FPRPreZ(GameApp* gameApp, const std::wstring& gameComponentName, const std::wstring& fprServiceName, const std::wstring& cameraServiceName, uint32_t callOrder)
	: DrawableGameComponent(gameApp, gameComponentName, callOrder)
{
	m_ForwardPlusRendering = m_GameApp->GetGameService<ForwardPlusRendering>(fprServiceName);
	AEAssert(m_ForwardPlusRendering != nullptr);

	m_CameraUpdater = m_GameApp->GetGameService<CameraUpdater>(cameraServiceName);
	AEAssert(m_CameraUpdater != nullptr);
}

FPRPreZ::~FPRPreZ()
{
	DeleteMem(m_ForwardPlusZPrePassMaterial);
	DeleteMem(m_ForwardPlusZPrePassSkinningMaterial);
}

void FPRPreZ::Initialize()
{
	m_GameObjectManager = m_GameApp->GetGameObjectManager();

	m_ForwardPlusZPrePassMaterial = new ForwardPlusZPrePassMaterial(m_GraphicDevice, m_GameResourceManager);

	m_ForwardPlusZPrePassSkinningMaterial = new ForwardPlusZPrePassSkinningMaterial(m_GraphicDevice, m_GameResourceManager);

	DrawableGameComponent::Initialize();
}

void FPRPreZ::LoadContent()
{
	AETODO("Set is ready when material is created");

	AEResult ret = AEResult::Ok;

	ret = m_ForwardPlusZPrePassMaterial->LoadContent();
	if(ret != AEResult::Ok)
	{
		AETODO("Add log");
	}

	ret = m_ForwardPlusZPrePassSkinningMaterial->LoadContent();
	if (ret != AEResult::Ok)
	{
		AETODO("Add log");
	}

	DrawableGameComponent::LoadContent();
}

void FPRPreZ::Render(const TimerParams& timerParams)
{
	m_GraphicDevice->BeginEvent(L"Forward+ Rendering Z Pre Pass");

	AEResult ret = RenderPreZ();

	if(ret != AEResult::Ok)
	{
		AETODO("Log error");
	}

	m_GraphicDevice->EndEvent();

	DrawableGameComponent::Render(timerParams);
}

AEResult FPRPreZ::RenderPreZ()
{
	AEResult ret = AEResult::Ok;

	RenderTarget* rts[1] = { nullptr };

	ret = m_GraphicDevice->SetRenderTargetsAndDepthStencil(1, rts, m_ForwardPlusRendering->GetForwardPlusDS());

	if(ret != AEResult::Ok)
	{
		AETODO("Add log");

		return ret;
	}

	AETODO("check return");
	ret = m_GraphicDevice->Clear(false);

	ret = m_ForwardPlusZPrePassMaterial->Apply();

	if(ret != AEResult::Ok)
	{
		AETODO("check return");
		m_GraphicDevice->ResetRenderTargetAndSetDepthStencil();

		AETODO("Add log");

		return ret;
	}

	DrawGameObjects();

	ret = m_GraphicDevice->ResetRenderTargetAndSetDepthStencil();

	if(ret != AEResult::Ok)
	{
		AETODO("Add log");

		return ret;
	}

	return AEResult::Ok;
}

void FPRPreZ::DrawGameObjects()
{
	Camera* currentCamera = m_CameraUpdater->GetMainCamera();

	for(auto goIt : *m_GameObjectManager)
	{
		DrawGameObject(goIt.second, currentCamera);
	}
}

void FPRPreZ::DrawGameObject(GameObject* gameObject, const Camera* camera)
{
	AEAssert(gameObject != nullptr);
	AEAssert(camera != nullptr);
	if (gameObject == nullptr || camera == nullptr)
	{
		return;
	}

	if(gameObject->HasMeshGOC() && gameObject->HasMaterialGOCs())
	{
		Mesh* mesh = gameObject->GetMeshGOC()->GetMeshResource();

		if (mesh != nullptr)
		{
			ConstantBuffer* cb = nullptr;

			if (!gameObject->HasMeshAnimationGOC())
			{
				ShaderProperties* vsProps = m_ForwardPlusZPrePassMaterial->GetVSProps();
				cb = vsProps->GetConstantBuffer(AE_CB_WORLD_VIEW_PROJ_NAME);
			}
			else
			{
				ShaderProperties* vsProps = m_ForwardPlusZPrePassSkinningMaterial->GetVSProps();
				cb = vsProps->GetConstantBuffer(AE_CB_WORLD_VIEW_PROJ_NAME);

				ConstantBuffer* cbBones = vsProps->GetConstantBuffer(AE_CB_BONES_NAME);
				if (cbBones != nullptr)
				{
					AnimationPlayer* player = gameObject->GetMeshAnimationGOC()->GetAnimationPlayer();
					cbBones->SetValue(AE_CB_BONE_TRANSFORMS_VAR_NAME, player->GetBoneTransforms(), sizeof(glm::mat4) * AE_MAX_BONES);
				}
			}

			cb->SetValueT<glm::mat4>(AE_CB_VIEW_VAR_NAME, camera->GetViewMatrix());
			cb->SetValueT<glm::mat4>(AE_CB_PROJECTION_VAR_NAME, camera->GetProjectionMatrix());
			cb->SetValueT<glm::mat4>(AE_CB_WORLD_VAR_NAME, gameObject->GetWorldTransform());

			for (uint32_t i = 0; i < mesh->GetNumberMeshParts(); i++)
			{
				MeshPart* meshPart = mesh->GetMeshPart(i);

				m_GraphicDevice->SetVertexBuffer(meshPart->GetVertexBuffer());
				m_GraphicDevice->SetIndexBuffer(meshPart->GetIndexBuffer());

				m_GraphicDevice->SetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

				m_GraphicDevice->DrawIndexed(0, 0, meshPart->GetIndexBuffer()->GetSize());
			}
		}
	}

	for(auto goIt : *gameObject)
	{
		DrawGameObject(goIt.second, camera);
	}
}