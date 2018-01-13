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

#ifndef _CAMERA_UPDATER_H
#define _CAMERA_UPDATER_H

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
#include "GameUtils\GameComponent.h"

/************
*   Using   *
*************/

/********************
*   Forward Decls   *
*********************/
class Camera;
class GameApp;

/*****************
*   Class Decl   *
******************/
class CameraUpdater sealed : public GameComponent
{
	private:

		/************************
		*   Private Variables   *
		*************************/
#pragma region Private Variables

		std::wstring m_ServiceName = L"";

		bool m_IsReady = false;

#pragma endregion

		/**********************
		*   Private Methods   *
		***********************/
#pragma region Private Methods

#pragma endregion

	public:

		/***************************************
		*   Constructor & Destructor Methods   *
		****************************************/
#pragma region Constructor & Destructor Methods

		/// <summary>
		/// CameraManager Constructor
		/// </summary>
		/// <param name="gameApp">Game Application associated with this Camera Manager</param>
		/// <param name="name">Component Name of the Camera Manager</param>
		/// <param name="serviceName">Service Name of the Camera Manager</param>
		/// <param name="callOrder">Call order of the Camera Manager</param>
		CameraUpdater(GameApp* gameApp, const std::wstring& name = AE_CAMERA_UPDATER_DEF_COMPONENT_NAME, const std::wstring& serviceName = AE_CAMERA_UPDATER_DEF_SERVICE_NAME, uint32_t callOrder = AEGameComponentCallOrder::_AE_GCCO_CameraUpdater);

		/// <summary>
		/// Default CameraManager Destructor
		/// </summary>
		virtual ~CameraUpdater();

#pragma endregion

		/*******************
		 *   Get Methods   *
		 *******************/
#pragma region Get Methods

		/// <summary>
		/// Gets the number of Cameras
		/// </summary>
		/// <returns>Returns the number of Cameras</returns>
		uint32_t GetNumberOfCameras() const;

		Camera* GetMainCamera() const;

#pragma endregion

		/*************************
		 *   Framework Methods   *
		 *************************/
#pragma region Framework Methods

		AEResult SetMainCamera(uint64_t cameraID);

		bool CameraExists(uint64_t cameraID);

		void Update(const TimerParams& timerParams) override;

		void OnResetDevice() override;

#pragma endregion

};

#endif