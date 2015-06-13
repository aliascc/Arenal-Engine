/********************************************************
*
* Author: Carlos Chacón N.
*
* Copyright 2012-2015
*
*********************************************************/

#pragma once

#ifndef _GAME_OBJECT_BOX_COLLIDER_COMPONENT_WIDGET_H
#define _GAME_OBJECT_BOX_COLLIDER_COMPONENT_WIDGET_H

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
#include "ui_GameObjectBoxColliderComponentWidgetQt.h"

/********************
*   Forward Decls   *
*********************/
class GameObject;
class EngineViewer;

/*****************
*   Class Decl   *
******************/
class GameObjectBoxColliderComponentWidget sealed : public QWidget
{
	Q_OBJECT

	private:

		Ui::GameObjectBoxColliderComponentWidgetQt m_GameObjectBoxColliderComponentWidgetQtUI;

		GameObject* m_GameObject = nullptr;

		bool m_IsReady = false;

		uint64_t m_PhysicColliderID = 0;

		void InitFields();

	private slots:

		void on_m_SizeXSB_valueChanged(double newValue);
		void on_m_SizeYSB_valueChanged(double newValue);
		void on_m_SizeZSB_valueChanged(double newValue);

	public:
		GameObjectBoxColliderComponentWidget(GameObject* gameObject, uint64_t physicColliderID, QWidget *parent = nullptr);
		~GameObjectBoxColliderComponentWidget();
};


#endif
