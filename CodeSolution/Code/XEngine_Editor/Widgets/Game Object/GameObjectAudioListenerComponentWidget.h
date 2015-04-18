
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

#ifndef _GAME_OBJECT_AUDIO_LISTENER_COMPONENT_WIDGET_H
#define _GAME_OBJECT_AUDIO_LISTENER_COMPONENT_WIDGET_H

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
#include "ui_GameObjectAudioListenerComponentWidgetQt.h"

/********************
*   Forward Decls   *
*********************/
class GameObject;

/*****************
*   Class Decl   *
******************/
class GameObjectAudioListenerComponentWidget sealed : public QWidget
{
	Q_OBJECT

	private:

		Ui::GameObjectAudioListenerComponentWidgetQt m_GameObjectAudioListenerComponentWidgetQtUI;

		GameObject* m_GameObject = nullptr;

		bool m_IsReady = false;

		void InitFields();;

	private slots:

	public:
		GameObjectAudioListenerComponentWidget(GameObject* gameObject, QWidget *parent = nullptr);
		~GameObjectAudioListenerComponentWidget();
};


#endif