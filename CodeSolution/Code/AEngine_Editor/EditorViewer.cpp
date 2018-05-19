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
#include "precomp_editor.h"

/**********************
*   System Includes   *
***********************/

/*************************
*   3rd Party Includes   *
**************************/

/***************************
*   Game Engine Includes   *
****************************/
#include "Keyboard.h"
#include "InputDefs.h"
#include "Lights\Light.h"
#include "EditorViewer.h"
#include "GraphicDevice.h"
#include "Stats\Console.h"
#include "Stats\DebugStats.h"
#include "Lights\SpotLight.h"
#include "Input\InputHandler.h"
#include "Camera\EditorCamera.h"
#include "Camera\CameraManager.h"
#include "Camera\CameraUpdater.h"
#include "AEGameComponentsDefs.h"
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

//Always include last
#include "Memory\MemLeaks.h"

/********************
*   Function Defs   *
*********************/
EditorViewer::EditorViewer(HINSTANCE hInstance)
    : GameApp(hInstance, GameAppRunOpt::EditorMode)
{
}

EditorViewer::~EditorViewer()
{
}

void EditorViewer::Initialize()
{
    m_InputHandler = new InputHandler(this);
    this->AddComponent(m_InputHandler);

    glm::ivec2 dimension(m_GraphicDevice->GetGraphicPP().m_BackBufferWidth, m_GraphicDevice->GetGraphicPP().m_BackBufferHeight);
    EditorCamera* editorCamera = new EditorCamera(m_InputManager, AE_CAMERA_EDITOR_ENG_DEFAULT_NAME, glm::vec3(0.0f, 0.0f, -5.0f), glm::vec3(0.0f, 0.0f, 0.0f), AEMathHelpers::Vec3fUp, dimension, 45.0f, 1.0f, 1000.0f);

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
    dbConf.m_SpriteFontFile = AE_Base_FS_PATH L"Data\\Fonts\\arial.spritefont";
    dbConf.m_GridEnabled = false;
    dbConf.m_GridSize = 200.0f;
    dbConf.m_GridUnits = 5.0f;
    m_DebugStats = new DebugStats(this, dbConf);
    this->AddComponent(m_DebugStats);

    m_Console = new Console(this);
    this->AddComponent(m_Console);

    GameApp::Initialize();
}

void EditorViewer::LoadContent()
{    
    GameApp::LoadContent();
}

void EditorViewer::UnLoadContent()
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

void EditorViewer::OnLostDevice()
{
    GameApp::OnLostDevice();
}

void EditorViewer::OnResetDevice()
{
    GameApp::OnResetDevice();
}

void EditorViewer::ConstantUpdate(const TimerParams& timerParams)
{
    GameApp::ConstantUpdate(timerParams);
}

void EditorViewer::Update(const TimerParams& timerParams)
{
    AETODO("Remove this from here");
    Keyboard* keyboard = m_InputHandler->GetKeyboard();
    if (keyboard->WasKeyPressed(AEKeys::O))
    {
        m_DebugStats->SetGridEnabled(!m_DebugStats->GetGridEnabled());
    }
    if (keyboard->WasKeyPressed(AEKeys::P))
    {
        m_DebugStats->SetAxisEnabled(!m_DebugStats->GetAxisEnabled());
    }
    if (keyboard->WasKeyPressed(AEKeys::I))
    {
        m_DebugStats->SetFPSEnabled(!m_DebugStats->GetFPSEnabled());
    }

    GameApp::Update(timerParams);
}

void EditorViewer::PostUpdate(const TimerParams& timerParams)
{
    GameApp::PostUpdate(timerParams);
}

void EditorViewer::Render(const TimerParams& timerParams)
{
    m_GraphicDevice->Clear();

    GameApp::Render(timerParams);
}
