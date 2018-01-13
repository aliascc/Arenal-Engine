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

#ifndef _GAME_OBJECT_PROPS_TREE_WIDGET_H
#define _GAME_OBJECT_PROPS_TREE_WIDGET_H

/**********************
*   System Includes   *
***********************/
#include <string>
#include <stdint.h>

/*************************
*   3rd Party Includes   *
**************************/
#include "qtreewidget.h"

/***************************
*   Game Engine Includes   *
****************************/
#include "Base\Base.h"

/********************
*   Forward Decls   *
*********************/
class GameObject;
class EngineViewer;

/*****************
*   Class Decl   *
******************/
class GameObjectPropsTreeWidget sealed : public QTreeWidget
{
	Q_OBJECT

	private:

		EngineViewer* m_EngineViewer = nullptr;

		GameObject* m_CurrentGameObject = nullptr;

		bool m_IsReady = false;

	private slots:

		void GameObjectChangedNameEvent(uint64_t gameObjectID);

	public:
		GameObjectPropsTreeWidget(QWidget* parent = nullptr);
		~GameObjectPropsTreeWidget();

		inline void SetEngineViewer(EngineViewer* engineViewer)
		{
			m_EngineViewer = engineViewer;
		}

		void InitFields();

		AEResult DisplayGameObjectInfo(GameObject* gameObject);

		AEResult ReloadGameObjectInfo();

	signals:

		void GameObjectChangedName(uint64_t gameObjectID);
};

#endif