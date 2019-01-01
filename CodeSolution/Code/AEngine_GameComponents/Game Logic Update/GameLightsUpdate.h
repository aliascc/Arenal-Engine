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

#ifndef _GAME_LIGHTS_UPDATE_H
#define _GAME_LIGHTS_UPDATE_H

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
#include "GameUtils\DrawableGameComponent.h"

/********************
*   Forward Decls   *
*********************/
class Light;
class GameApp;
class Viewport;
class GameObject;
class LightManager;
struct TimerParams;
class CameraUpdater;
class ConstantBuffer;
class GameObjectManager;
class DepthStencilSurface;
class VarianceShadowMaterial;
class VarianceSkinningShadowMaterial;

/*****************
*   Class Decl   *
******************/
class GameLightsUpdate sealed : public DrawableGameComponent
{
    private:

        VarianceShadowMaterial* m_VarianceShadowMaterial = nullptr;

        VarianceSkinningShadowMaterial* m_VarianceSkinningShadowMaterial = nullptr;

        DepthStencilSurface* m_SpotLightShadowTexturesDS = nullptr;

        DepthStencilSurface* m_DirLightShadowTexturesDS = nullptr;

        Viewport* m_SpotLightShadowViewport = nullptr;

        CameraUpdater* m_CameraUpdater = nullptr;

        std::vector<Viewport*> m_DirLightShadowViewports;

        LightManager& m_LightManager;

        GameObjectManager& m_GameObjectManager;

        void UpdateGameObjectLight(GameObject* gameObject);

        AEResult ShadowSpotLightRenderGameObject();

        AEResult ShadowDirLightRenderGameObject();

        AEResult ShadowLightRenderGameObject(GameObject* gameObject, const glm::mat4& lightView, const glm::mat4& lightProj);

    public:
        //Constructor Destructor.
        GameLightsUpdate(GameApp& gameApp, const std::string& gameComponentName = AE_GAME_LIGHTS_UPDATE_DEF_COMPONENT_NAME, const std::string& cameraServiceName = AE_CAMERA_UPDATER_DEF_SERVICE_NAME, uint32_t callOrder = AEGameComponentCallOrder::_AE_GCCO_GameLightsUpdate);
        virtual ~GameLightsUpdate();

        //Game Component Override methods
        void Initialize() override;

        void LoadContent() override;

        void Update(const TimerParams& timerParams) override;
        
        void Render(const TimerParams& timerParams) override;
};

#endif
