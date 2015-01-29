
/********************************************************
*
* Author: Carlos Chacón N.
*
* Created: 8/10/2013
*
* Desc:
*
*
* Copyright 2013
*
*********************************************************/

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

		XEResult CreateGameObjectBranch(GameObject* gameObject, QTreeWidgetItem* parentBranch = nullptr);

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

		XEResult RefreshGameObjectsTree();

		XEResult GameObjectNameChanged(uint64_t gameObjectID);

		uint64_t GetCurrentGameObjectID();

	signals:
		void GameObjectChanged(GameObject* gameObject);

};

#endif