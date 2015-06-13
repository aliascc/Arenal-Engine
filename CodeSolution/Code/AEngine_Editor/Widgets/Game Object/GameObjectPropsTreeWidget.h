/********************************************************
*
* Author: Carlos Chacón N.
*
* Copyright 2012-2015
*
*********************************************************/

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