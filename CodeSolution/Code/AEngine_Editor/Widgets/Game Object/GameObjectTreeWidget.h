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

#ifndef _GAME_OBJECT_TREE_WIDGET_H
#define _GAME_OBJECT_TREE_WIDGET_H

/**********************
*   System Includes   *
***********************/
#include <string>
#include <stdint.h>

/*************************
*   3rd Party Includes   *
**************************/
#include "qevent.h"
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
class GameObjectTreeWidget sealed : public QTreeWidget
{
	Q_OBJECT

	private:

		EngineViewer* m_EngineViewer = nullptr;

		bool m_IsReady = false;

		AEResult CreateGameObjectBranch(GameObject* gameObject, QTreeWidgetItem* parentBranch = nullptr);

		QTreeWidgetItem* FindBranch(uint64_t gameObjectID);

		QTreeWidgetItem* FindBranchChild(uint64_t gameObjectID, QTreeWidgetItem* parent);

		void dropEvent(QDropEvent* dEvent) override;

	private slots:

		void GameObjectSelectionChanged();

	public:
		GameObjectTreeWidget(QWidget* parent = nullptr);
		~GameObjectTreeWidget();

		inline void SetEngineViewer(EngineViewer* engineViewer)
		{
			m_EngineViewer = engineViewer;
		}

		void InitFields();

		AEResult RefreshGameObjectsTree();

		AEResult GameObjectNameChanged(uint64_t gameObjectID);

		uint64_t GetCurrentGameObjectID();

	signals:
		void GameObjectChanged(GameObject* gameObject);

};

#endif