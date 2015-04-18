
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

#ifndef _GAME_OBJECT_BASE_INFORMATION_WIDGET_H
#define _GAME_OBJECT_BASE_INFORMATION_WIDGET_H

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
#include "ui_GameObjectBaseInformationWidgetQt.h"

/********************
*   Forward Decls   *
*********************/
class GameObject;
class EngineViewer;

/*****************
*   Class Decl   *
******************/
class GameObjectBaseInformationWidget sealed : public QWidget
{
	Q_OBJECT

	private:

		Ui::GameObjectBaseInformationWidgetQt m_GameObjectBaseInformationWidgetQtUI;

		EngineViewer* m_EngineViewer = nullptr;

		GameObject* m_GameObject;

		void InitFields();

	private slots:

		void on_m_GameObjectNameTextBox_editingFinished();

	public:
		GameObjectBaseInformationWidget(GameObject* gameObject, EngineViewer* engineViewer, QWidget *parent = nullptr);
		~GameObjectBaseInformationWidget();

	signals:

		void NameChanged(uint64_t gameObjectID);
};


#endif