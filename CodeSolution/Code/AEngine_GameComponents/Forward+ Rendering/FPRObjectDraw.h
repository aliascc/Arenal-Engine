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

#ifndef _FPR_OBJECT_DRAW_H
#define _FPR_OBJECT_DRAW_H

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
#include "Base\Base.h"
#include "AEGameComponentsDefs.h"
#include "GameUtils\DrawableGameComponent.h"

/************
*   Using   *
*************/

/********************
*   Forward Decls   *
*********************/
class GameApp;
class GameObject;
struct TimerParams;
class CameraUpdater;
class GameObjectManager;
class ForwardPlusRendering;
class ForwardPlusZPrePassMaterial;

/*****************
*   Class Decl   *
******************/
class FPRObjectDraw sealed : public DrawableGameComponent
{
    private:
        //Variables
        ForwardPlusRendering* m_ForwardPlusRendering = nullptr;

        GameObjectManager* m_GameObjectManager = nullptr;

        CameraUpdater* m_CameraUpdater = nullptr;

        AEResult RenderWithFPR();

        void DrawGameObjects();

        void DrawGameObject(GameObject* gameObject);

    public:
        //Constructor Destructor.
        FPRObjectDraw(GameApp* gameApp, const std::wstring& gameComponentName = AE_FORWARD_PLUS_OBJECT_DRAW_DEF_COMPONENT_NAME, const std::wstring& fprServiceName = AE_FORWARD_PLUS_MAIN_DEF_SERVICE_NAME, const std::wstring& cameraServiceName = AE_CAMERA_UPDATER_DEF_SERVICE_NAME, uint32_t callOrder = AEGameComponentCallOrder::_AE_GCCO_ForwardPlusLightObjectDraw);
        virtual ~FPRObjectDraw();

        //Gets

        //Framework Methods

        //Drawable Game Component Override methods
        void Initialize() override;
        void LoadContent() override;
        void Update(const TimerParams& timerParams) override;
        void Render(const TimerParams& timerParams) override;
        void OnLostDevice() override;
        void OnResetDevice() override;
};

#endif
