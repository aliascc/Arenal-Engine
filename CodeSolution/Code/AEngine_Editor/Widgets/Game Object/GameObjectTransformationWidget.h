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

#ifndef _GAME_OBJECT_TRANSFORMATION_WIDGET_H
#define _GAME_OBJECT_TRANSFORMATION_WIDGET_H

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
#include "Utils\GraphicUtilsDefs.h"
#include "ui_GameObjectTransformationWidgetQt.h"

/********************
*   Forward Decls   *
*********************/
class GameObject;

/*****************
*   Class Decl   *
******************/
class GameObjectTransformationWidget sealed : public QWidget
{
	Q_OBJECT

	private:

		Ui::GameObjectTransformationWidgetQt m_GameObjectTransformationWidgetQtUI;

		GameObject* m_GameObject;

		void InitFields();

		uint64_t m_UniqueCallbackID = 0;

		void GameObject3DChanged(Object3DChangeEventType changeType, Object3D* object3D);

	signals:

		void GameObject3DChangedSignal(Object3DChangeEventType changeType);

	private slots:

		void GameObject3DChangedSlot(Object3DChangeEventType changeType);

		void on_m_PosX_valueChanged(double newValue);
		void on_m_PosY_valueChanged(double newValue);
		void on_m_PosZ_valueChanged(double newValue);

		void on_m_RotX_valueChanged(double newValue);
		void on_m_RotY_valueChanged(double newValue);
		void on_m_RotZ_valueChanged(double newValue);

		void on_m_SclX_valueChanged(double newValue);
		void on_m_SclY_valueChanged(double newValue);
		void on_m_SclZ_valueChanged(double newValue);

	public:
		GameObjectTransformationWidget(GameObject* gameObject, QWidget *parent = nullptr);
		~GameObjectTransformationWidget();
};


#endif