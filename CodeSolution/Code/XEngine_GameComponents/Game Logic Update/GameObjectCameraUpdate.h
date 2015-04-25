/********************************************************
*
* Author: Carlos Chacón N.
*
* Copyright 2012-2015
*
*********************************************************/

#pragma once

#ifndef _GAME_OBJECT_CAMERA_UPDATE_H
#define _GAME_OBJECT_CAMERA_UPDATE_H

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
#include "XEGameComponentsDefs.h"
#include "GameUtils\GameComponent.h"

/********************
*   Forward Decls   *
*********************/
class GameApp;
class GameObject;
struct TimerParams;

/*****************
*   Class Decl   *
******************/
class GameObjectCameraUpdate sealed : public GameComponent
{
	private:

		/************************
		*   Private Variables   *
		*************************/
#pragma region Private Variables

#pragma endregion

		/**********************
		*   Private Methods   *
		***********************/
#pragma region Private Methods

		void UpdateCamera(GameObject* gameObject);

#pragma endregion

	public:

		/***************************************
		*   Constructor & Destructor Methods   *
		****************************************/
#pragma region Constructor & Destructor Methods

		/// <summary>
		/// GameObjectCameraUpdate Constructor
		/// </summary>
		GameObjectCameraUpdate(GameApp* gameApp, const std::wstring& gameComponentName = XE_GAME_GAME_OBJECT_CAMERA_UPDATE_COMPONENT_NAME, uint32_t callOrder = XEGameComponentCallOrder::_XE_GCCO_GameObjectCameraUpdate);

		/// <summary>
		/// Default GameObjectCameraUpdate Destructor
		/// </summary>
		virtual ~GameObjectCameraUpdate();

#pragma endregion

		/************************
		*   Framework Methods   *
		*************************/
#pragma region Framework Methods

		void Update(const TimerParams& timerParams) override;

#pragma endregion

};

#endif