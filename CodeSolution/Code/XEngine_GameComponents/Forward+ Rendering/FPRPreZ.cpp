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
#include <string>

/*************************
*   3rd Party Includes   *
**************************/

/***************************
*   Game Engine Includes   *
****************************/
#include "FPRPreZ.h"
#include "Models\Mesh.h"
#include "Time\XETime.h"
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
XETODO("Check object instances and calls to where it is init");
FPRPreZ::FPRPreZ(GameApp* gameApp, const std::wstring& gameComponentName, const std::wstring& fprServiceName, const std::wstring& cameraServiceName, uint32_t callOrder)
	: DrawableGameComponent(gameApp, gameComponentName, callOrder)
{
	m_ForwardPlusRendering = m_GameApp->GetGameService<ForwardPlusRendering>(fprServiceName);
	XEAssert(m_ForwardPlusRendering != nullptr);

	m_CameraUpdater = m_GameApp->GetGameService<CameraUpdater>(cameraServiceName);
	XEAssert(m_CameraUpdater != nullptr);
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
	XETODO("Set is ready when material is created");

	XEResult ret = XEResult::Ok;

	ret = m_ForwardPlusZPrePassMaterial->LoadContent();
	if(ret != XEResult::Ok)
	{
		XETODO("Add log");
	}

	ret = m_ForwardPlusZPrePassSkinningMaterial->LoadContent();
	if (ret != XEResult::Ok)
	{
		XETODO("Add log");
	}

	DrawableGameComponent::LoadContent();
}

void FPRPreZ::Render(const TimerParams& timerParams)
{
	m_GraphicDevice->BeginEvent(L"Forward+ Rendering Z Pre Pass");

	XEResult ret = RenderPreZ();

	if(ret != XEResult::Ok)
	{
		XETODO("Log error");
	}

	m_GraphicDevice->EndEvent();

	DrawableGameComponent::Render(timerParams);
}

XEResult FPRPreZ::RenderPreZ()
{
	XEResult ret = XEResult::Ok;

	RenderTarget* rts[1] = { nullptr };

	ret = m_GraphicDevice->SetRenderTargetsAndDepthStencil(1, rts, m_ForwardPlusRendering->GetForwardPlusDS());

	if(ret != XEResult::Ok)
	{
		XETODO("Add log");

		return ret;
	}

	XETODO("check return");
	ret = m_GraphicDevice->Clear(false);

	ret = m_ForwardPlusZPrePassMaterial->Apply();

	if(ret != XEResult::Ok)
	{
		XETODO("check return");
		m_GraphicDevice->ResetRenderTargetAndSetDepthStencil();

		XETODO("Add log");

		return ret;
	}

	DrawGameObjects();

	ret = m_GraphicDevice->ResetRenderTargetAndSetDepthStencil();

	if(ret != XEResult::Ok)
	{
		XETODO("Add log");

		return ret;
	}

	return XEResult::Ok;
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
	XEAssert(gameObject != nullptr);
	XEAssert(camera != nullptr);
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
				cb = vsProps->GetConstantBuffer(XE_CB_WORLD_VIEW_PROJ_NAME);
			}
			else
			{
				ShaderProperties* vsProps = m_ForwardPlusZPrePassSkinningMaterial->GetVSProps();
				cb = vsProps->GetConstantBuffer(XE_CB_WORLD_VIEW_PROJ_NAME);

				ConstantBuffer* cbBones = vsProps->GetConstantBuffer(XE_CB_BONES_NAME);
				if (cbBones != nullptr)
				{
					AnimationPlayer* player = gameObject->GetMeshAnimationGOC()->GetAnimationPlayer();
					cbBones->SetValue(XE_CB_BONE_TRANSFORMS_VAR_NAME, player->GetBoneTransforms(), sizeof(glm::mat4) * XE_MAX_BONES);
				}
			}

			cb->SetValueT<glm::mat4>(XE_CB_VIEW_VAR_NAME, camera->GetViewMatrix());
			cb->SetValueT<glm::mat4>(XE_CB_PROJECTION_VAR_NAME, camera->GetProjectionMatrix());
			cb->SetValueT<glm::mat4>(XE_CB_WORLD_VAR_NAME, gameObject->GetWorldTransform());

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