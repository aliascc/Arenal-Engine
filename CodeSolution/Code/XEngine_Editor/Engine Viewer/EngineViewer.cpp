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

/*************************
*   3rd Party Includes   *
**************************/
#include "glm\glm.hpp"

/***************************
*   Game Engine Includes   *
****************************/
#include "Keyboard.h"
#include "InputDefs.h"
#include "Lights\Light.h"
#include "EngineViewer.h"
#include "GraphicDevice.h"
#include "Stats\Console.h"
#include "Time\XETimeDefs.h"
#include "Stats\DebugStats.h"
#include "Lights\SpotLight.h"
#include "Input\InputHandler.h"
#include "Camera\EditorCamera.h"
#include "Camera\CameraManager.h"
#include "Camera\CameraUpdater.h"
#include "XEGameComponentsDefs.h"
#include "GameObject\GameObject.h"
#include "GameAssets\RawGameAsset.h"
#include "GameAssets\Assets\MeshAsset.h"
#include "GameAssets\GameAssetManager.h"
#include "GameAssets\Assets\ModelAsset.h"
#include "GameObject\GameObjectManager.h"
#include "GameAssets\Assets\ShaderAsset.h"
#include "GameObject\Components\MeshGOC.h"
#include "GameAssets\Assets\TextureAsset.h"
#include "GameObject\Components\LightGOC.h"
#include "GameAssets\Assets\SkeletonAsset.h"
#include "GameObject\Components\PhysicsGOC.h"
#include "GameAssets\Assets\AnimationAsset.h"
#include "Game Logic Update\GameAudioUpdate.h"
#include "Game Logic Update\GameLightsUpdate.h"
#include "Game Logic Update\GamePhysicsUpdate.h"
#include "GameObject\Components\MeshMaterialGOC.h"
#include "Shaders\Bindings\ShaderTextureBinding.h"
#include "GameObject\Components\MeshAnimationGOC.h"
#include "Game Logic Update\GameAnimationsUpdate.h"
#include "Forward+ Rendering\ForwardPlusRendering.h"
#include "Game Logic Update\GameObjectScriptCaller.h"
#include "Game Logic Update\GameObjectCameraUpdate.h"
#include "Game Objects Debug\GameObjectsDebugVisualizer.h"
#include "Graphic Extensions\Shader Extensions\Properties\ShaderProperties.h"

#include "Models\Mesh.h"
#include "PhysicsActor.h"
#include "PhysicColliderBox.h"
#include "PhysicColliderSphere.h"
#include "Lights\LightManager.h"
#include "Utils\XERandomDefs.h"

//Always include last
#include "Memory\MemLeaks.h"

/********************
*   Function Defs   *
*********************/
EngineViewer::EngineViewer(HINSTANCE hInstance)
	: GameApp(hInstance, GameAppRunOpt::EditorMode)
{
}

EngineViewer::~EngineViewer()
{
}

void EngineViewer::Initialize()
{
	m_InputHandler = new InputHandler(this);
	this->AddComponent(m_InputHandler);

	glm::ivec2 dimension(m_GraphicDevice->GetGraphicPP().m_BackBufferWidth, m_GraphicDevice->GetGraphicPP().m_BackBufferHeight);
	EditorCamera* editorCamera = new EditorCamera(m_InputManager, XE_CAMERA_EDITOR_ENG_DEFAULT_NAME, glm::vec3(0.0f, 0.0f, -5.0f), glm::vec3(0.0f, 0.0f, 0.0f), XEMathHelpers::Vec3fUp, dimension, 45.0f, 1.0f, 1000.0f);

	m_CameraManager->AddCamera(editorCamera);
	m_CameraManager->SetEditorCamera(editorCamera->GetUniqueID());
	m_CameraManager->SetMainCamera(editorCamera->GetUniqueID());

	m_CameraUpdater = new CameraUpdater(this);
	this->AddComponent(m_CameraUpdater);

	m_GameAnimationsUpdate = new GameAnimationsUpdate(this);
	this->AddComponent(m_GameAnimationsUpdate);

	m_GameLightsUpdate = new GameLightsUpdate(this);
	this->AddComponent(m_GameLightsUpdate);

	m_ForwardPlusRendering = new ForwardPlusRendering(this);
	this->AddComponent(m_ForwardPlusRendering);

	m_GameObjectScriptCaller = new GameObjectScriptCaller(this);
	this->AddComponent(m_GameObjectScriptCaller);

	m_GameObjectDebugVisualizer = new GameObjectsDebugVisualizer(this);
	this->AddComponent(m_GameObjectDebugVisualizer);

	m_GameObjectCameraUpdate = new GameObjectCameraUpdate(this);
	this->AddComponent(m_GameObjectCameraUpdate);

	m_GameAudioUpdate = new GameAudioUpdate(this);
	this->AddComponent(m_GameAudioUpdate);

	m_GamePhysicsUpdate = new GamePhysicsUpdate(this);
	this->AddComponent(m_GamePhysicsUpdate);

	DebugStatsConfig dbConf;
	dbConf.m_SpriteFontFile = XE_BASE_FS_PATH L"Data\\Fonts\\arial.spritefont";
	dbConf.m_GridEnabled = false;
	dbConf.m_GridSize = 200.0f;
	dbConf.m_GridUnits = 5.0f;
	m_DebugStats = new DebugStats(this, dbConf);
	this->AddComponent(m_DebugStats);

	m_Console = new Console(this);
	this->AddComponent(m_Console);

	GameApp::Initialize();
}

void EngineViewer::LoadContent()
{	
	GameApp::LoadContent();
}

void EngineViewer::UnLoadContent()
{
	if (m_GamePhysicsUpdate != nullptr)
	{
		RemoveComponent(m_GamePhysicsUpdate);
	}

	if (m_GameObjectScriptCaller != nullptr)
	{
		RemoveComponent(m_GameObjectScriptCaller);
	}

	if (m_CameraUpdater != nullptr)
	{
		RemoveComponent(m_CameraUpdater);
	}

	if (m_InputHandler != nullptr)
	{
		RemoveComponent(m_InputHandler);
	}

	if (m_DebugStats != nullptr)
	{
		RemoveComponent(m_DebugStats);
	}

	if (m_Console != nullptr)
	{
		RemoveComponent(m_Console);
	}

	if (m_GameLightsUpdate != nullptr)
	{
		RemoveComponent(m_GameLightsUpdate);
	}

	if (m_ForwardPlusRendering != nullptr)
	{
		RemoveComponent(m_ForwardPlusRendering);
	}

	if (m_GameAnimationsUpdate != nullptr)
	{
		RemoveComponent(m_GameAnimationsUpdate);
	}

	if (m_GameObjectDebugVisualizer != nullptr)
	{
		RemoveComponent(m_GameObjectDebugVisualizer);
	}

	if (m_GameObjectCameraUpdate != nullptr)
	{
		RemoveComponent(m_GameObjectCameraUpdate);
	}

	if (m_GameAudioUpdate != nullptr)
	{
		RemoveComponent(m_GameAudioUpdate);
	}

	DeleteMem(m_GamePhysicsUpdate);
	DeleteMem(m_GameObjectDebugVisualizer);
	DeleteMem(m_GameObjectScriptCaller);
	DeleteMem(m_CameraUpdater);
	DeleteMem(m_InputHandler);
	DeleteMem(m_DebugStats);
	DeleteMem(m_Console);
	DeleteMem(m_GameLightsUpdate);
	DeleteMem(m_ForwardPlusRendering);
	DeleteMem(m_GameAnimationsUpdate);
	DeleteMem(m_GameObjectCameraUpdate);
	DeleteMem(m_GameAudioUpdate);

	GameApp::UnLoadContent();
}

void EngineViewer::OnLostDevice()
{
	GameApp::OnLostDevice();
}

void EngineViewer::OnResetDevice()
{
	GameApp::OnResetDevice();
}

void EngineViewer::ConstantUpdate(const TimerParams& timerParams)
{
	GameApp::ConstantUpdate(timerParams);
}

void EngineViewer::Update(const TimerParams& timerParams)
{
	XETODO("Remove this from here");
	Keyboard* keyboard = m_InputHandler->GetKeyboard();
	if (keyboard->WasKeyPressed(XEKeys::L))
	{
		AutoLoadTest();
	}

	GameApp::Update(timerParams);
}

void EngineViewer::PostUpdate(const TimerParams& timerParams)
{
	GameApp::PostUpdate(timerParams);
}

void EngineViewer::Render(const TimerParams& timerParams)
{
	m_GraphicDevice->Clear();

	XETODO("Remove this from here");
	Keyboard* keyboard = m_InputHandler->GetKeyboard();
	if (keyboard->WasKeyPressed(XEKeys::O))
	{
		m_DebugStats->SetGridEnabled(!m_DebugStats->GetGridEnabled());
	}
	if (keyboard->WasKeyPressed(XEKeys::P))
	{
		m_DebugStats->SetAxisEnabled(!m_DebugStats->GetAxisEnabled());
	}
	if (keyboard->WasKeyPressed(XEKeys::I))
	{
		m_DebugStats->SetFPSEnabled(!m_DebugStats->GetFPSEnabled());
	}

	GameApp::Render(timerParams);
}

void EngineViewer::AutoLoadTest()
{
	if(m_AutoTestLoaded)
	{
		return;
	}

	m_AutoTestLoaded = true;

	uint64_t raGrassTexID = 0;
	uint64_t raSnowTexID = 0;
	uint64_t raRockTexID = 0;
	uint64_t raboxMeshID = 0;
	uint64_t raVSID = 0;
	uint64_t raPSID = 0;
	uint64_t raSphereID = 0;

	

	m_GameAssetManager->CreateNewRawGameAsset(boost::filesystem::current_path().wstring() + L"/../" L"Data\\Raw Files\\Grass_Terrain_04.dds", GameContentSubtype::Texture2D, &raGrassTexID);
	m_GameAssetManager->CreateNewRawGameAsset(boost::filesystem::current_path().wstring() + L"/../" L"Data\\Raw Files\\Road_Terrain_03.dds", GameContentSubtype::Texture2D, &raSnowTexID);
	m_GameAssetManager->CreateNewRawGameAsset(boost::filesystem::current_path().wstring() + L"/../" L"Data\\Raw Files\\Piedra_Nieve.dds", GameContentSubtype::Texture2D, &raRockTexID);
	m_GameAssetManager->CreateNewRawGameAsset(boost::filesystem::current_path().wstring() + L"/../" L"Data\\Raw Files\\box.DAE", GameContentSubtype::None, &raboxMeshID);
	m_GameAssetManager->CreateNewRawGameAsset(boost::filesystem::current_path().wstring() + L"/../" L"Data\\Raw Files\\ModelStaticVS.fx", GameContentSubtype::VertexShaderHLSL, &raVSID);
	m_GameAssetManager->CreateNewRawGameAsset(boost::filesystem::current_path().wstring() + L"/../" L"Data\\Raw Files\\ModelPS.fx", GameContentSubtype::PixelShaderHLSL, &raPSID);
	m_GameAssetManager->CreateNewRawGameAsset(boost::filesystem::current_path().wstring() + L"/../" L"Data\\Raw Files\\sphere.DAE", GameContentSubtype::None, &raSphereID);

	m_GameAssetManager->CheckForLatestRawGameAssetsAndImport();

	////////////////////////////////////////



	uint64_t gaGrassTexID	= m_GameAssetManager->GetRawGameAsset(raGrassTexID)->GetUniqueAssociatedAssetID();
	uint64_t gaSnowTexID	= m_GameAssetManager->GetRawGameAsset(raSnowTexID)->GetUniqueAssociatedAssetID();
	uint64_t gaRockTexID	= m_GameAssetManager->GetRawGameAsset(raRockTexID)->GetUniqueAssociatedAssetID();
	uint64_t gaboxMeshID	= m_GameAssetManager->GetRawGameAsset(raboxMeshID)->GetUniqueAssociatedAssetID();
	uint64_t gaVSID			= m_GameAssetManager->GetRawGameAsset(raVSID)->GetUniqueAssociatedAssetID();
	uint64_t gaPSID			= m_GameAssetManager->GetRawGameAsset(raPSID)->GetUniqueAssociatedAssetID();
	uint64_t gaSphereID		= m_GameAssetManager->GetRawGameAsset(raSphereID)->GetUniqueAssociatedAssetID();

	///////////////////////////////////////////////////////////////////////////////////

	//MeshAsset* boxMeshAsset = (MeshAsset*)m_GameAssetManager->GetGameAsset(1); //Box
	//MeshAsset* sphereMeshAsset = (MeshAsset*)m_GameAssetManager->GetGameAsset(2); //Sphere
	ShaderAsset* vsAsset		= (ShaderAsset*)m_GameAssetManager->GetGameAsset(gaVSID); //VS
	ShaderAsset* psAsset		= (ShaderAsset*)m_GameAssetManager->GetGameAsset(gaPSID); //PS
	TextureAsset* grassTexAsset	= (TextureAsset*)m_GameAssetManager->GetGameAsset(gaGrassTexID);
	TextureAsset* snowTexAsset	= (TextureAsset*)m_GameAssetManager->GetGameAsset(gaSnowTexID);
	TextureAsset* rockTexAsset	= (TextureAsset*)m_GameAssetManager->GetGameAsset(gaRockTexID);
	MeshAsset* boxMeshAsset		= ((ModelAsset*)m_GameAssetManager->GetGameAsset(gaboxMeshID))->GetMeshAssetMap().begin()->second.m_Asset;
	MeshAsset* sphereMeshAsset	= ((ModelAsset*)m_GameAssetManager->GetGameAsset(gaSphereID))->GetMeshAssetMap().begin()->second.m_Asset;

	////////////////////////////////////////

	GameObject* goFloor = new GameObject(L"Floor");
	m_GameObjectManager->AddGameObject(goFloor);
	goFloor->SetScale(glm::vec3(50.0f, 0.5f, 50.0f));

	MeshGOC* meshFloor = new MeshGOC(goFloor);
	goFloor->SetMeshGOC(meshFloor);
	meshFloor->SetMeshAsset(boxMeshAsset);

	MeshMaterialGOC* matFloor = new MeshMaterialGOC(goFloor, L"Floor Mat");
	goFloor->AddMeshMaterialGOC(matFloor);
	matFloor->SetVertexShaderAsset(vsAsset);
	matFloor->SetPixelShaderAsset(psAsset);
	matFloor->GetPixelShaderProperties()->SetTexture(L"DiffuseTexture", grassTexAsset);

	PhysicsGOC* physicsFloor = new PhysicsGOC(goFloor, this->GetPhysicsManager());
	goFloor->SetPhysicsGOC(physicsFloor);
	uint64_t physicsFloorColliderID = 0;
	physicsFloor->AddCollider(CollisionShape::Box, physicsFloorColliderID);
	PhysicColliderBox* physicColliderBoxFloor = (PhysicColliderBox*)physicsFloor->GetPhysicsActor()->GetPhysicCollider(physicsFloorColliderID);
	//physicColliderBoxFloor->SetSize(boxMeshAsset->GetMeshReference()->GetBoundingBox().GetSize());
	physicColliderBoxFloor->SetScale(goFloor->GetScale());

	for (int32_t i = 0; i < 5; i++)
	{
		for (int32_t j = 0; j < 5; j++)
		{
			for (int32_t h = 0; h < 5; h++)
			{
				glm::vec3 pos(15 - (i * 3), 30 - (j * 3), 15 - (h * 3));
				glm::vec3 rand(XERandomHelpers::GetFloat(-0.9f, 0.9f), XERandomHelpers::GetFloat(-0.9f, 0.9f), XERandomHelpers::GetFloat(-0.9f, 0.9f));
				AutoLoadTestAddGO(pos + rand, boxMeshAsset, vsAsset, psAsset, snowTexAsset, CollisionShape::Box);
			}
		}
	}

	for (int32_t i = 0; i < 5; i++)
	{
		for (int32_t j = 0; j < 5; j++)
		{
			for (int32_t h = 0; h < 5; h++)
			{
				glm::vec3 rand(XERandomHelpers::GetFloat(-0.9f, 0.9f), XERandomHelpers::GetFloat(-0.9f, 0.9f), XERandomHelpers::GetFloat(-0.9f, 0.9f));
				glm::vec3 pos(15 - (i * 4), 15 - (j * 4), 15 - (h * 4));
				AutoLoadTestAddGO(pos + rand, sphereMeshAsset, vsAsset, psAsset, rockTexAsset, CollisionShape::Sphere);
			}
		}
	}

	//////////////////////////////////////
}

void EngineViewer::AutoLoadTestAddGO(const glm::vec3& pos, MeshAsset* model, ShaderAsset* vs, ShaderAsset* ps, TextureAsset* text, CollisionShape collisionShape)
{
	GameObject* goFloor = new GameObject(L"Obj");
	m_GameObjectManager->AddGameObject(goFloor);
	goFloor->SetPosition(pos);

	MeshGOC* meshFloor = new MeshGOC(goFloor);
	goFloor->SetMeshGOC(meshFloor);
	meshFloor->SetMeshAsset(model);

	MeshMaterialGOC* matFloor = new MeshMaterialGOC(goFloor, L"Obj Mat");
	goFloor->AddMeshMaterialGOC(matFloor);
	matFloor->SetVertexShaderAsset(vs);
	matFloor->SetPixelShaderAsset(ps);
	matFloor->GetPixelShaderProperties()->SetTexture(L"DiffuseTexture", text);

	PhysicsGOC* physicsFloor = new PhysicsGOC(goFloor, this->GetPhysicsManager());
	goFloor->SetPhysicsGOC(physicsFloor);
	physicsFloor->AddRigidBody();
	uint64_t physicsFloorColliderID = 0;
	physicsFloor->AddCollider(collisionShape, physicsFloorColliderID);

	if (!m_LightManager->IsContainerFull())
	{
		LightGOC* lightGOC = new LightGOC(goFloor, m_LightManager);
		goFloor->SetLightGOC(lightGOC);
		lightGOC->ChangeLightType(LightType::Omni);

		lightGOC->GetLight()->SetNearAttenuation(1);
		lightGOC->GetLight()->SetFarAttenuation(6);
		lightGOC->GetLight()->SetIntensity(XERandomHelpers::GetFloat(0.1f, 1.0f));
		lightGOC->GetLight()->SetColor(Color(XERandomHelpers::GetUInt(0, 255), XERandomHelpers::GetUInt(0, 255), XERandomHelpers::GetUInt(0, 255), 1));
	}
}