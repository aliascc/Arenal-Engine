/*
* Copyright (c) 2018 <Carlos Chac�n>
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

#ifndef _GAME_OBJECT_RIGID_BODY_COMPONENT_WIDGET_H
#define _GAME_OBJECT_RIGID_BODY_COMPONENT_WIDGET_H

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
#include "ui_GameObjectRigidBodyComponentWidgetQt.h"

/********************
*   Forward Decls   *
*********************/
class GameObject;
class EngineViewer;

/*****************
*   Class Decl   *
******************/
class GameObjectRigidBodyComponentWidget sealed : public QWidget
{
	Q_OBJECT

	private:

		Ui::GameObjectRigidBodyComponentWidgetQt m_GameObjectRigidBodyComponentWidgetQtUI;

		GameObject* m_GameObject = nullptr;

		bool m_IsReady = false;

		void InitFields();

	private slots:

		void on_m_MassSB_valueChanged(double newValue);
		void on_m_LinearDampingSB_valueChanged(double newValue);
		void on_m_AngularDampingSB_valueChanged(double newValue);

		void on_m_GravityEnabledCB_stateChanged(int newState);
		void on_m_IsKinematicCB_stateChanged(int newState);

	public:
		GameObjectRigidBodyComponentWidget(GameObject* gameObject, QWidget *parent = nullptr);
		~GameObjectRigidBodyComponentWidget();
};


#endif