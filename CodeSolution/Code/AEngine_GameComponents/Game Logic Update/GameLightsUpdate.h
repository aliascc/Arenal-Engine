/********************************************************
*
* Author: Carlos Chacón N.
*
* Copyright 2012-2015
*
*********************************************************/

#pragma once

#ifndef _GAME_LIGHTS_UPDATE_H
#define _GAME_LIGHTS_UPDATE_H

/**********************
*   System Includes   *
***********************/
#include <string>
#include <vector>
#include <stdint.h>

/*************************
*   3rd Party Includes   *
**************************/

/***************************
*   Game Engine Includes   *
****************************/
#include "Base\Base.h"
#include "AEGameComponentsDefs.h"
#include "GameUtils\DrawableGameComponent.h"

/********************
*   Forward Decls   *
*********************/
class Light;
class GameApp;
class Viewport;
class GameObject;
struct TimerParams;
class CameraUpdater;
class ConstantBuffer;
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

		void UpdateGameObjectLight(GameObject* gameObject);

		AEResult ShadowSpotLightRenderGameObject();

		AEResult ShadowDirLightRenderGameObject();

		AEResult ShadowLightRenderGameObject(GameObject* gameObject, const glm::mat4& lightView, const glm::mat4& lightProj);

	public:
		//Constructor Destructor.
		GameLightsUpdate(GameApp* gameApp, const std::wstring& gameComponentName = AE_GAME_LIGHTS_UPDATE_DEF_COMPONENT_NAME, const std::wstring& cameraServiceName = AE_CAMERA_UPDATER_DEF_SERVICE_NAME, uint32_t callOrder = AEGameComponentCallOrder::_AE_GCCO_GameLightsUpdate);
		virtual ~GameLightsUpdate();

		//Game Component Override methods
		void Initialize() override;

		void LoadContent() override;

		void Update(const TimerParams& timerParams) override;
		
		void Render(const TimerParams& timerParams) override;
};

#endif