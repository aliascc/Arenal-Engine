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

#pragma once

/**********************
*   System Includes   *
***********************/

/*************************
*   3rd Party Includes   *
**************************/

/***************************
*   Game Engine Includes   *
****************************/
#include "GameApp\GameApp.h"

/********************
*   Forward Decls   *
*********************/
class Console;
class UIManager;
class DebugStats;
class InputHandler;
struct TimerParams;
class CameraUpdater;
class ImGuiComponent;
class GameAudioUpdate;
class GameLightsUpdate;
class GamePhysicsUpdate;
class GameAnimationsUpdate;
class ForwardPlusRendering;
class GameObjectCameraUpdate;
class GameObjectScriptCaller;
class GameObjectsDebugVisualizer;

class MeshAsset;
class TextureAsset;
class ShaderAsset;

/*****************
*   Class Decl   *
******************/
class EditorViewer sealed : public GameApp
{
    private:
        InputHandler* m_InputHandler = nullptr;
        DebugStats* m_DebugStats = nullptr;
        Console* m_Console = nullptr;
        GameLightsUpdate* m_GameLightsUpdate = nullptr;
        ForwardPlusRendering* m_ForwardPlusRendering = nullptr;
        CameraUpdater* m_CameraUpdater = nullptr;
        GameObjectsDebugVisualizer* m_GameObjectDebugVisualizer = nullptr;
        GameAnimationsUpdate* m_GameAnimationsUpdate = nullptr;
        GameObjectScriptCaller* m_GameObjectScriptCaller = nullptr;
        GameObjectCameraUpdate* m_GameObjectCameraUpdate = nullptr;
        GameAudioUpdate* m_GameAudioUpdate = nullptr;
        GamePhysicsUpdate* m_GamePhysicsUpdate = nullptr;
        ImGuiComponent* m_ImGuiComponent = nullptr;
        UIManager* m_UIManager = nullptr;

    public:
        //Constructor and Destroyer
        EditorViewer(HINSTANCE hInstance);
        virtual ~EditorViewer();

        //Override methods from parent class
        void Initialize() override;
        void LoadContent() override;
        void UnLoadContent() override;
        void OnLostDevice() override;
        void OnResetDevice() override;
        void ConstantUpdate(const TimerParams& timerParams) override;
        void Update(const TimerParams& timerParams) override;
        void PostUpdate(const TimerParams& timerParams) override;
        void Render(const TimerParams& timerParams) override;
};