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

#ifndef _GAME_OBJECT_SCRIPT_PROPERTY_TREE_WIDGET_H
#define _GAME_OBJECT_SCRIPT_PROPERTY_TREE_WIDGET_H

/**********************
*   System Includes   *
***********************/
#include <string>
#include <stdint.h>

/*************************
*   3rd Party Includes   *
**************************/
#include "qspinbox.h"
#include "qcheckbox.h"
#include "qtreewidget.h"

/***************************
*   Game Engine Includes   *
****************************/
#include "Base\Base.h"

/********************
*   Forward Decls   *
*********************/
struct GameObjectScriptProperty;
struct GameObjectScriptProperties;

/*****************
*   Class Decl   *
******************/
class GameObjectScriptPropertyWidget sealed : public QTreeWidget
{
	Q_OBJECT

	private:

		bool m_IsReady = false;

		AEResult FillPropertiesRows(GameObjectScriptProperties* gameObjectScriptProperties);

		AEResult AddScalarWidget(QTreeWidgetItem* item, GameObjectScriptProperty* gosProp);

		QSpinBox* CreateSpinBox(GameObjectScriptProperty* gosProp);

		QDoubleSpinBox* CreateDoubleSpinBox(GameObjectScriptProperty* gosProp);

		QCheckBox* CreateCheckBox(GameObjectScriptProperty* gosProp);

	private slots:

		void SpinBoxChanged();

		void CheckBoxChanged(int newState);

	public:
		GameObjectScriptPropertyWidget(QWidget *parent = nullptr);
		~GameObjectScriptPropertyWidget();

		AEResult Initialize();

		AEResult Populate(GameObjectScriptProperties* gameObjectScriptProperties);
};

#endif
