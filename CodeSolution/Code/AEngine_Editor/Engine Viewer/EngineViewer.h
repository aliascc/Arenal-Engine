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

#pragma once

#ifndef _ENGINE_VIEWER_H
#define _ENGINE_VIEWER_H

/**********************
*   System Includes   *
***********************/
#include <string>
#include <Windows.h>

/*************************
*   3rd Party Includes   *
**************************/

/***************************
*   Game Engine Includes   *
****************************/
#include "Base\Base.h"
#include "GameApp\GameApp.h"

#include "PhysicsDefs.h"

/********************
*   Forward Decls   *
*********************/
class Console;
class DebugStats;
class InputHandler;
struct TimerParams;
class CameraUpdater;
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
class EngineViewer sealed : public GameApp
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

        //To be removed
        bool m_AutoTestLoaded = false;
        void AutoLoadTest();
        void AutoLoadTestAddGO(const glm::vec3& pos, MeshAsset* model, ShaderAsset* vs, ShaderAsset* ps, TextureAsset* text, CollisionShape collisionShape);

    public:
        //Constructor and Destroyer
        EngineViewer(HINSTANCE hInstance);
        virtual ~EngineViewer();

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

#endif
