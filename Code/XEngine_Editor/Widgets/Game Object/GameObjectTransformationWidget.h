
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