/********************************************************
*
* Author: Carlos Chacón N.
*
* Created: 2/13/2012
*
* Desc:
*
*
* Copyright 2013
*
*********************************************************/

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
#include "XEGameComponentsDefs.h"
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

		XEResult RenderWithFPR();

		void DrawGameObjects();

		void DrawGameObject(GameObject* gameObject);

	public:
		//Constructor Destructor.
		FPRObjectDraw(GameApp* gameApp, const std::wstring& gameComponentName = XE_FORWARD_PLUS_OBJECT_DRAW_DEF_COMPONENT_NAME, const std::wstring& fprServiceName = XE_FORWARD_PLUS_MAIN_DEF_SERVICE_NAME, const std::wstring& cameraServiceName = XE_CAMERA_UPDATER_DEF_SERVICE_NAME, uint32_t callOrder = XEGameComponentCallOrder::_XE_GCCO_ForwardPlusLightObjectDraw);
		virtual ~FPRObjectDraw();

		//Gets

		//Framework Methods

		//Drawable Game Component Override methods
		void					Initialize					() override;
		void					LoadContent					() override;
		void					Update						(const TimerParams& timerParams) override;
		void					Render						(const TimerParams& timerParams) override;
		void					OnLostDevice				() override;
		void					OnResetDevice				() override;
};

#endif