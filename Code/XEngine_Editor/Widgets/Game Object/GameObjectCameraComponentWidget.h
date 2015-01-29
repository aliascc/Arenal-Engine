
/********************************************************
*
* Author: Carlos Chacón N.
*
* Created: 9/21/2014
*
* Desc:
*
*
* Copyright 2012-2014
*
*********************************************************/

#pragma once

#ifndef _GAME_OBJECT_CAMERA_COMPONENT_WIDGET_H
#define _GAME_OBJECT_CAMERA_COMPONENT_WIDGET_H

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
#include "Lights\LightDefs.h"
#include "ui_GameObjectCameraComponentWidgetQt.h"

/********************
*   Forward Decls   *
*********************/
class Camera;
class CameraGOC;
class GameObject;
class EngineViewer;

/*****************
*   Class Decl   *
******************/

class GameObjectCameraComponentWidget sealed : public QWidget
{
	Q_OBJECT

	private:

		Ui::GameObjectCameraComponentWidgetQt m_GameObjectCameraComponentWidgetQtUI;

		EngineViewer* m_EngineViewer = nullptr;

		GameObject* m_GameObject = nullptr;

		bool m_IsReady = false;

		void InitFields();

		CameraGOC* GetCameraGOC();

		Camera* GetCamera();

	private slots:

		void on_m_MakeDefaultButton_clicked();

		void on_m_DrawFrustumEnabled_stateChanged(int newState);

	public:
		GameObjectCameraComponentWidget(GameObject* gameObject, EngineViewer* engineViewer, QWidget *parent = nullptr);
		~GameObjectCameraComponentWidget();
};


#endif