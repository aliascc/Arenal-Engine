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

#ifndef _GAME_OBJECTS_DEBUG_VISUALIZER_H
#define _GAME_OBJECTS_DEBUG_VISUALIZER_H

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
#include "GameObjectsDebugDefs.h"

/************
*   Using   *
*************/

/********************
*   Forward Decls   *
*********************/
class Light;
class Camera;
class GameApp;
class Frustum;
class LightGOC;
class OmniLight;
class SpotLight;
class Texture2D;
class GameObject;
class QuadShape3D;
struct TimerParams;
class FrustumShape;
class CameraUpdater;
class OmniLightShape;
class SpotLightShape;
class DirectionalLight;
class BasicLineMaterial;
class BasicColorMaterial;
class DirectionalLightShape;
class DiffuseTextureBasicMaterial;

/*****************
*   Class Decl   *
******************/
class GameObjectsDebugVisualizer sealed : public DrawableGameComponent
{
    private:

        GameObjectsDebugVisualizerConfig m_GameObjectsDebugVisualizerConfig;

        glm::mat4 m_LightIconsScale = AEMathHelpers::Mat4Identity;

        CameraUpdater* m_CameraUpdater;

        BasicLineMaterial* m_BasicLineMaterial = nullptr;

        BasicColorMaterial* m_BasicColorMaterial = nullptr;

        DiffuseTextureBasicMaterial* m_DiffuseTextureBasicMaterial = nullptr;

        QuadShape3D* m_QuadShape3D = nullptr;

        OmniLightShape* m_OmniLightShape = nullptr;

        SpotLightShape* m_SpotLightShape = nullptr;

        DirectionalLightShape* m_DirectionalLightShape = nullptr;

        Texture2D* m_OmniLightIconTex = nullptr;

        Texture2D* m_DirLightIconTex = nullptr;

        Texture2D* m_SpotLightIconTex = nullptr;

        Texture2D* m_CameraIconTex = nullptr;

        FrustumShape* m_FrustumShape = nullptr;

        AEResult SelectedGameObjectDebugRender();

        AEResult DrawFrustum(const Frustum* frustum, bool simple, const Camera* camera);

        AEResult GameObjectCameraRender(const GameObject* gameObject, const Camera* camera);

        AEResult GameObjectLightRender(const GameObject* gameObject, const Camera* camera);

        AEResult GameObjectOmniLightRender(const GameObject* gameObject, const OmniLight* omnilight, const Camera* camera, bool drawDebug, bool renderFrustum, bool frustumSimple);

        AEResult GameObjectSpotLightRender(const GameObject* gameObject, const SpotLight* spotlight, const Camera* camera, bool drawDebug, bool renderFrustum, bool frustumSimple);

        AEResult GameObjectDirectionalLightRender(const GameObject* gameObject, const DirectionalLight* dirlight, const Camera* camera, bool drawDebug, bool renderFrustum, bool frustumSimple, bool drawFustumCascades);

        AEResult LightIconDraw();

        AEResult CameraIconDraw();

    public:
        //Constructor Destructor.
        GameObjectsDebugVisualizer(GameApp& gameApp, GameResourceManager& gameResourceManager, GraphicDevice& graphicDevice, const std::string& gameComponentName = AE_GAME_OBJECTS_DEBUG_VISUALIZER_DEF_COMPONENT_NAME, const std::string& cameraServiceName = AE_CAMERA_UPDATER_DEF_SERVICE_NAME, uint32_t callOrder = AEGameComponentCallOrder::_AE_GCCO_GameObjectsDebugVisualizer);
        virtual ~GameObjectsDebugVisualizer();

        void Initialize() override;
        void LoadContent() override;
        void Render(const TimerParams& timerParams) override;
};

#endif
