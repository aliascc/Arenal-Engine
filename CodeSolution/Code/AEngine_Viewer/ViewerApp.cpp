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
#include <DirectXMath.h>

/*************************
*   3rd Party Includes   *
**************************/
#include "glm\glm.hpp"

/***************************
*   Game Engine Includes   *
****************************/
#include "Base\Base.h"
#include "Logger\Logger.h"
#include "ViewerApp.h"
#include "GraphicDevice.h"
#include "Time\AETimeDefs.h"
#include "Vertex\IndexBuffer.h"
#include "Input\InputHandler.h"
#include "Vertex\VertexBuffer.h"
#include "Vertex\Types\VertexPositionColor.h"
#include "Keyboard.h"
#include "InputDefs.h"
#include "Stats\DebugStats.h"
#include "Stats\Console.h"
#include "Wrappers\SpriteBatchAE.h"
#include "Wrappers\SpriteFontAE.h"
#include "Vertex\Types\VertexPositionTexture.h"
#include "Textures\Texture2D.h"
#include "Models\Mesh.h"
#include "Models\MeshPart.h"
#include "Models\Animation\AnimationPlayer.h"

#include "Graphic Extensions\Materials\Skinning\SkinningMaterial.h"
#include "Graphic Extensions\Materials\Basic\DiffuseTextureMaterial.h"

#include "Shaders\Bindings\ShaderTextureBinding.h"
#include "Shaders\Buffers\ConstantBuffer.h"
#include "Graphic Extensions\Shader Extensions\Properties\ShaderProperties.h"

#include "GameAssets\GameAssetManager.h"
#include "GameObject\GameObject.h"
#include "GameObject\Components\MeshGOC.h"
#include "GameObject\GameObjectManager.h"
#include "Models\Mesh.h"
#include "Models\MeshPart.h"

//Always include last
#include "Memory\MemLeaks.h"

/************
*   Using   *
*************/

/********************
*   Function Defs   *
*********************/
ViewerApp::ViewerApp(HINSTANCE hInstance)
	: GameApp(hInstance, GameAppRunOpt::GameMode)
	, m_InputHandler(nullptr)
	, m_SpriteBatchAE(nullptr)
	, m_SpriteFontAE(nullptr)
	, m_DebugStats(nullptr)
	, m_Console(nullptr)
	, m_AnimationPlayer(nullptr)
	, m_DiffuseTextureMaterial(nullptr)
	, m_SkinningMaterial(nullptr)
	, m_GameObjectLobo(nullptr)
	, m_LoboTexture(nullptr)
{
}

ViewerApp::~ViewerApp()
{
	DeleteMem(m_Console);
	DeleteMem(m_DebugStats);
	DeleteMem(m_SpriteFontAE);
	DeleteMem(m_SpriteBatchAE);
	DeleteMem(m_InputHandler);
	DeleteMem(m_AnimationPlayer);
	
	DeleteMem(m_DiffuseTextureMaterial);
	DeleteMem(m_SkinningMaterial);

	AERelease(m_LoboTexture);
}

void ViewerApp::Initialize()
{
	//m_InputHandler = new InputHandler(this);
	//this->AddComponent(m_InputHandler);

	//glm::ivec2 dimesions(m_GraphicDevice->GetGraphicPP().m_BackBufferWidth, m_GraphicDevice->GetGraphicPP().m_BackBufferHeight);
	//m_ViewerCamera = new ViewerCamera(m_InputManager, AE_CAMERA_EDITOR_ENG_DEFAULT_NAME, glm::vec3(0.0f, 0.0f, 5.0f), glm::vec3(0.0f, 0.0f, 0.0f), AEMathHelpers::Vec3fUp, dimesions, 45.0f/*glm::pi<float>() * .25f*/, 1.0f, 1000.0f);

	//DebugStatsConfig dbConf;
	//dbConf.m_SpriteFontFile = AE_Base_FS_PATH L".\\Data\\Fonts\\arial.spritefont";
	//dbConf.m_GridEnabled = true;
	//dbConf.m_GridSize = 200.0f;
	//dbConf.m_GridUnits = 5.0f;
	//m_DebugStats = new DebugStats(this, dbConf);
	//this->AddComponent(m_DebugStats);

	//m_Console = new Console(this);
	//this->AddComponent(m_Console);
	//
	//m_SpriteBatchAE = new SpriteBatchAE(m_GraphicDevice);

	//m_SpriteFontAE = new SpriteFontAE(m_GraphicDevice, AE_Base_FS_PATH L".\\Data\\Fonts\\arial.spritefont");
	//
	//m_DiffuseTextureMaterial = new DiffuseTextureMaterial(m_GraphicDevice, m_GameResourceManager);

	//m_SkinningMaterial = new SkinningMaterial(m_GraphicDevice, m_GameResourceManager);
	//	
	//m_AnimationPlayer = new AnimationPlayer();

	//m_GameAssetManager->CreateNewRawGameAsset(AE_Base_FS_PATH L".\\Data\\Raw Files\\Lobo.dae", AE_Base_FS_PATH L".\\Data\\Assets\\", GameContentSubtype::None);

	//m_LoboTexture = new Texture2D(m_GraphicDevice, L"LoboTexture");

	GameApp::Initialize();
}

void ViewerApp::LoadContent()
{
	//m_SpriteBatchAE->Initialize();

	//m_SpriteFontAE->Initialize();

	//m_SkinningMaterial->LoadContent();

	//m_DiffuseTextureMaterial->LoadContent();

	//m_GameAssetManager->CheckForLatestRawGameAssetsAndImport();

	//m_GameObjectLobo = new GameObject(L"Lobo");

	//MeshGOC* meshGOC = new MeshGOC();

	//m_GameObjectLobo->SetMeshGOC(meshGOC);

	//MeshAsset* meshaset = (MeshAsset*)m_GameAssetManager->GetGameAsset(2);

	//meshGOC->SetMeshAsset(meshaset);

	//m_GameObjectManager->AddGameObject(m_GameObjectLobo);

	//m_LoboTexture->CreateFromFile(L".\\Data\\Textures\\LoboTexture.dds");

	GameApp::LoadContent();
}

void ViewerApp::CallBackAnim1()
{
	AE_Base::ConsolePrintLine(L"Enter callback");
}

void ViewerApp::UnLoadContent()
{
	GameApp::UnLoadContent();
}

void ViewerApp::OnLostDevice()
{
	GameApp::OnLostDevice();
}

void ViewerApp::OnResetDevice()
{
	GameApp::OnResetDevice();
}

void ViewerApp::Update(const TimerParams& timerParams)
{
	/*
	if(m_InputHandler->KeyboardWasKeyPressed(AEKeys::O))
	{
		m_DebugStats->SetGridEnabled(!m_DebugStats->GetGridEnabled());
	}
	if(m_InputHandler->KeyboardWasKeyPressed(AEKeys::P))
	{
		m_DebugStats->SetAxisEnabled(!m_DebugStats->GetAxisEnabled());
	}
	if(m_InputHandler->KeyboardWasKeyPressed(AEKeys::I))
	{
		m_DebugStats->SetFPSEnabled(!m_DebugStats->GetFPSEnabled());
	}
	
	static glm::vec3 rotate = AEMathHelpers::Vec3fZero;
	static float scale = 1.0f;

	glm::mat4 w = AEMathHelpers::Mat4Identity;
	glm::mat4 wvp = AEMathHelpers::Mat4Identity;
	
	m_DiffuseTextureMaterial->GetVSProps()->GetConstantBuffer(L"_AE_CB_World_View_Proj")->SetValueT<glm::mat4>(L"_AE_World", w);
	m_DiffuseTextureMaterial->GetVSProps()->GetConstantBuffer(L"_AE_CB_World_View_Proj")->SetValueT<glm::mat4>(L"_AE_View", m_ViewerCamera->GetViewMatrix());
	m_DiffuseTextureMaterial->GetVSProps()->GetConstantBuffer(L"_AE_CB_World_View_Proj")->SetValueT<glm::mat4>(L"_AE_PROJection", m_ViewerCamera->GetProjectionMatrix());

	m_SkinningMaterial->GetVSProps()->GetConstantBuffer(L"_AE_CB_World_View_Proj")->SetValueT<glm::mat4>(L"_AE_World", w);
	m_SkinningMaterial->GetVSProps()->GetConstantBuffer(L"_AE_CB_World_View_Proj")->SetValueT<glm::mat4>(L"_AE_View", m_ViewerCamera->GetViewMatrix());
	m_SkinningMaterial->GetVSProps()->GetConstantBuffer(L"_AE_CB_World_View_Proj")->SetValueT<glm::mat4>(L"_AE_PROJection", m_ViewerCamera->GetProjectionMatrix());

	m_SkinningMaterial->GetVSProps()->GetConstantBuffer(L"_AE_CB_Bones")->SetValue(L"_AE_BoneTransforms", m_AnimationPlayer->GetBoneTransforms(), AE_MAX_BONES * sizeof(glm::mat4));*/

	GameApp::Update(timerParams);
}

void ViewerApp::Render(const TimerParams& timerParams)
{
	//m_GraphicDevice->Clear();

	//ID3D11RasterizerState* defaultRSState = nullptr;
	//ID3D11RasterizerState* rsState = nullptr;

	//D3D11_RASTERIZER_DESC rsD;
	//memset(&rsD, 0, sizeof(rsD));

	//rsD.AntialiasedLineEnable = TRUE;
	//rsD.CullMode = D3D11_CULL_FRONT;
	//rsD.DepthBias = 0;
	//rsD.DepthBiasClamp = 0;
	//rsD.DepthClipEnable = FALSE;
	//rsD.FillMode = D3D11_FILL_SOLID;
	//rsD.FrontCounterClockwise = TRUE;
	//rsD.MultisampleEnable = TRUE;
	//rsD.ScissorEnable = FALSE;
	//rsD.SlopeScaledDepthBias = 0.0f;

	//m_GraphicDevice->GetDeviceDX()->CreateRasterizerState(&rsD, &rsState);
	//m_GraphicDevice->GetDeviceContextDX()->RSGetState(&defaultRSState);
	//m_GraphicDevice->GetDeviceContextDX()->RSSetState(rsState);

	////DrawSkinModels(m_Model);
	//DrawGameObject(m_GameObjectLobo);
	//
	//m_GraphicDevice->GetDeviceContextDX()->RSSetState(defaultRSState);
	//ReleaseCOM(rsState);

	GameApp::Render(timerParams);
}

//void ViewerApp::DrawSkinModels(Model* model)
//{
//	const MeshCollection* mc = model->GetMeshCollection();
//	
//	for(auto mesh : *mc)
//	{
//		MeshPartCollection& mpc = mesh.second->GetMeshPartCollection();
//			
//		for(auto meshpart : mpc)
//		{
//			const TextureMaterials* txm = model->GetTextureMaterial(meshpart->GetTexMaterialID());
//
//			m_SkinningMaterial->GetPSProps()->GetShaderTextureBinding(L"_AE_DiffuseTexture")->SetTexture(txm->m_DiffuseTexture);
//
//			m_GraphicDevice->SetVertexBuffer(meshpart->GetVertexBuffer());
//			m_GraphicDevice->SetIndexBuffer(meshpart->GetIndexBuffer());
//			
//			m_SkinningMaterial->Apply();
//
//			m_GraphicDevice->SetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
//
//			m_GraphicDevice->DrawIndexed(0, 0, meshpart->GetIndexBuffer()->GetSize());
//		}
//	}
//}

void ViewerApp::DrawGameObject(GameObject* gameObject)
{
	//if(gameObject == nullptr)
	//{
	//	return;
	//}

	//Mesh* mesh = gameObject->GetMeshGOC()->GetMeshResource();

	//for(uint32_t i = 0; i < mesh->GetNumberMeshParts(); i++)
	//{
	//	MeshPart* meshPart = mesh->GetMeshPart(i);
	//	
	//	m_DiffuseTextureMaterial->GetPSProps()->GetShaderTextureBinding(L"DiffuseTexture")->SetTexture((Texture*)m_LoboTexture->AddRef());

	//	m_GraphicDevice->SetVertexBuffer(meshPart->GetVertexBuffer());
	//	m_GraphicDevice->SetIndexBuffer(meshPart->GetIndexBuffer());
	//		
	//	m_DiffuseTextureMaterial->Apply();

	//	m_GraphicDevice->SetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	//	m_GraphicDevice->DrawIndexed(0, 0, meshPart->GetIndexBuffer()->GetSize());
	//}
}


