/********************************************************
*
* Author: Carlos Chacón N.
*
* Copyright 2012-2015
*
*********************************************************/

#pragma once

#ifndef _FPR_PRE_Z_RENDERING_H
#define _FPR_PRE_Z_RENDERING_H

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
#include "XEGameComponentsDefs.h"
#include "GameUtils\DrawableGameComponent.h"

/************
*   Using   *
*************/

/********************
*   Forward Decls   *
*********************/
class Camera;
class GameApp;
class GameObject;
struct TimerParams;
class CameraUpdater;
class GameObjectManager;
class ForwardPlusRendering;
class ForwardPlusZPrePassMaterial;
class ForwardPlusZPrePassSkinningMaterial;

/*****************
*   Class Decl   *
******************/
class FPRPreZ sealed : public DrawableGameComponent
{
	private:
		//Variables
		ForwardPlusRendering* m_ForwardPlusRendering = nullptr;

		ForwardPlusZPrePassMaterial* m_ForwardPlusZPrePassMaterial = nullptr;

		ForwardPlusZPrePassSkinningMaterial* m_ForwardPlusZPrePassSkinningMaterial = nullptr;

		bool m_SkinningMatSet = false;

		GameObjectManager* m_GameObjectManager = nullptr;

		CameraUpdater* m_CameraUpdater = nullptr;

		XEResult RenderPreZ();

		void DrawGameObjects();

		void DrawGameObject(GameObject* gameObject, const Camera* camera);

	public:
		//Constructor Destructor.
		FPRPreZ(GameApp* gameApp, const std::wstring& gameComponentName = XE_FORWARD_PLUS_Z_PRE_PASS_DEF_COMPONENT_NAME, const std::wstring& fprServiceName = XE_FORWARD_PLUS_MAIN_DEF_SERVICE_NAME, const std::wstring& cameraServiceName = XE_CAMERA_UPDATER_DEF_SERVICE_NAME, uint32_t callOrder = XEGameComponentCallOrder::_XE_GCCO_ForwardPlusPreZ);
		virtual ~FPRPreZ();

		//Gets

		//Framework Methods

		//Drawable Game Component Override methods
		void					Initialize					() override;
		void					LoadContent					() override;
		void					Render						(const TimerParams& timerParams) override;
};

#endif