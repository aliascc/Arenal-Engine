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

#ifndef _FPR_LIGHT_CULLING_H
#define _FPR_LIGHT_CULLING_H

/**********************
*   System Includes   *
***********************/

/*************************
*   3rd Party Includes   *
**************************/

/***************************
*   Game Engine Includes   *
****************************/
#include "AEGameComponentsDefs.h"

/************
*   Using   *
*************/

/********************
*   Forward Decls   *
*********************/
class GameApp;
class LightManager;
struct TimerParams;
class CameraUpdater;
class ForwardPlusRendering;
class ForwardPlusLightCullingMaterial;

/*****************
*   Class Decl   *
******************/
class FPRLightCulling sealed : public DrawableGameComponent
{
    private:

        LightManager& m_LightManager;

        ForwardPlusRendering* m_ForwardPlusRendering = nullptr;

        CameraUpdater* m_CameraUpdater = nullptr;

        ForwardPlusLightCullingMaterial* m_ForwardPlusLightCullingMaterial = nullptr;

    public:
        //Constructor Destructor.
        FPRLightCulling(GameApp& gameApp, const std::string& gameComponentName = AE_FORWARD_PLUS_LIGHT_CULLING_DEF_COMPONENT_NAME, const std::string& fprServiceName = AE_FORWARD_PLUS_MAIN_DEF_SERVICE_NAME, const std::string& cameraServiceName = AE_CAMERA_UPDATER_DEF_SERVICE_NAME, uint32_t callOrder = AEGameComponentCallOrder::_AE_GCCO_ForwardPlusLightCulling);
        virtual ~FPRLightCulling();

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