
/********************************************************
*
* Author: Carlos Chacón N.
*
* Desc:
*
*
* Copyright 2012-2014
*
*********************************************************/

#pragma once

#ifndef _GAME_OBJECT_MESH_COMPONENT_WIDGET_H
#define _GAME_OBJECT_MESH_COMPONENT_WIDGET_H

/**********************
*   System Includes   *
***********************/
#include <stdint.h>

/*************************
*   3rd Party Includes   *
**************************/
#include "qevent.h"
#include "QtWidgets\qwidget.h"

/***************************
*   Game Engine Includes   *
****************************/
#include "Base\Base.h"
#include "ui_GameObjectMeshComponentWidgetQt.h"

/********************
*   Forward Decls   *
*********************/
class GameObject;
class GameAssetManager;

/*****************
*   Class Decl   *
******************/
class GameObjectMeshComponentWidget sealed : public QWidget
{
	Q_OBJECT

	private:

		Ui::GameObjectMeshComponentWidgetQt m_GameObjectMeshComponentWidgetQtUI;

		GameObject* m_GameObject = nullptr;

		GameAssetManager* m_GameAssetManager = nullptr;

		bool m_IsReady = false;

		void InitFields();

		void dragEnterEvent(QDragEnterEvent* enterEvent) override;

		void dragLeaveEvent(QDragLeaveEvent* leaveEvent) override;

		void dragMoveEvent(QDragMoveEvent* moveEvent) override;

		void dropEvent(QDropEvent* dropEvent) override;

		XEResult DropAsset(QObject* object);

		XEResult SetGameAssetFromID(uint64_t gameAssetID);

		XEResult RemoveGameAsset();

	private slots:

		void on_m_AddMeshAssetButton_clicked();

		void on_m_ClearMeshAssetButton_clicked();

	public:
		GameObjectMeshComponentWidget(GameObject* gameObject, GameAssetManager* gameAssetManager, QWidget *parent = nullptr);
		~GameObjectMeshComponentWidget();
};


#endif