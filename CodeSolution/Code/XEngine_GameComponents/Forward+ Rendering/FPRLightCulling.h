/********************************************************
*
* Author: Carlos Chacón N.
*
* Copyright 2012-2015
*
*********************************************************/

#pragma once

#ifndef _FPR_LIGHT_CULLING_H
#define _FPR_LIGHT_CULLING_H

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
#include "XEGameComponentsDefs.h"
#include "GameUtils\DrawableGameComponent.h"

/************
*   Using   *
*************/

/********************
*   Forward Decls   *
*********************/
class GameApp;
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
		//Variables
		ForwardPlusRendering* m_ForwardPlusRendering = nullptr;

		CameraUpdater* m_CameraUpdater = nullptr;

		ForwardPlusLightCullingMaterial* m_ForwardPlusLightCullingMaterial = nullptr;

	public:
		//Constructor Destructor.
		FPRLightCulling(GameApp* gameApp, const std::wstring& gameComponentName = XE_FORWARD_PLUS_LIGHT_CULLING_DEF_COMPONENT_NAME, const std::wstring& fprServiceName = XE_FORWARD_PLUS_MAIN_DEF_SERVICE_NAME, const std::wstring& cameraServiceName = XE_CAMERA_UPDATER_DEF_SERVICE_NAME, uint32_t callOrder = XEGameComponentCallOrder::_XE_GCCO_ForwardPlusLightCulling);
		virtual ~FPRLightCulling();

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