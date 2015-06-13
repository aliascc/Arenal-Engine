/********************************************************
*
* Author: Carlos Chacón N.
*
* Copyright 2012-2015
*
*********************************************************/

#pragma once

#ifndef _GAME_OBJECT_SPHERE_COLLIDER_COMPONENT_WIDGET_H
#define _GAME_OBJECT_SPHERE_COLLIDER_COMPONENT_WIDGET_H

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
#include "ui_GameObjectSphereColliderComponentWidgetQt.h"

/********************
*   Forward Decls   *
*********************/
class GameObject;
class EngineViewer;

/*****************
*   Class Decl   *
******************/
class GameObjectSphereColliderComponentWidget sealed : public QWidget
{
		Q_OBJECT

	private:

		Ui::GameObjectSphereColliderComponentWidgetQt m_GameObjectSphereColliderComponentWidgetUI;

		GameObject* m_GameObject = nullptr;

		bool m_IsReady = false;

		uint64_t m_PhysicColliderID = 0;

		void InitFields();

	private slots:

		void on_m_RadiusSB_valueChanged(double newValue);

	public:
		GameObjectSphereColliderComponentWidget(GameObject* gameObject, uint64_t physicColliderID, QWidget *parent = nullptr);
		~GameObjectSphereColliderComponentWidget();
};


#endif
