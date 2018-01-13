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

#ifndef _GAME_OBJECT_CAMERA_COMPONENT_WIDGET_H
#define _GAME_OBJECT_CAMERA_COMPONENT_WIDGET_H

/**********************
*   System Includes   *
***********************/
#include <stdint.h>

/*************************
*   3rd Party Includes   *
**************************/
#include "QtWidgets\qwidget.h"

/***************************
*   Game Engine Includes   *
****************************/
#include "Lights\LightDefs.h"
#include "ui_GameObjectCameraComponentWidgetQt.h"

/********************
*   Forward Decls   *
*********************/
class Camera;
class CameraGOC;
class GameObject;
class EngineViewer;

/*****************
*   Class Decl   *
******************/

class GameObjectCameraComponentWidget sealed : public QWidget
{
	Q_OBJECT

	private:

		Ui::GameObjectCameraComponentWidgetQt m_GameObjectCameraComponentWidgetQtUI;

		EngineViewer* m_EngineViewer = nullptr;

		GameObject* m_GameObject = nullptr;

		bool m_IsReady = false;

		void InitFields();

		CameraGOC* GetCameraGOC();

		Camera* GetCamera();

	private slots:

		void on_m_MakeDefaultButton_clicked();

		void on_m_DrawFrustumEnabled_stateChanged(int newState);

	public:
		GameObjectCameraComponentWidget(GameObject* gameObject, EngineViewer* engineViewer, QWidget *parent = nullptr);
		~GameObjectCameraComponentWidget();
};


#endif