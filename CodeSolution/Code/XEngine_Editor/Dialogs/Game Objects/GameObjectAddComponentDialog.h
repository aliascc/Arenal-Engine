
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

#ifndef _GAME_OBJECT_ADD_COMPONENT_DIALOG_H
#define _GAME_OBJECT_ADD_COMPONENT_DIALOG_H

/**********************
*   System Includes   *
***********************/

/*************************
*   3rd Party Includes   *
**************************/
#include "QtWidgets\qdialog.h"

/***************************
*   Game Engine Includes   *
****************************/
#include "GameObject\GameObjectDefs.h"
#include "ui_GameObjectAddComponentDialogQt.h"

/********************
*   Forward Decls   *
*********************/

/*****************
*   Class Decl   *
******************/
class GameObjectAddComponentDialog sealed : public QDialog
{
	Q_OBJECT

	private:
		Ui::GameObjectAddComponentDialogQt m_GameObjectAddComponentDialogQtUI;

	private slots:

	public:
		GameObjectAddComponentDialog(QWidget *parent = 0);
		virtual ~GameObjectAddComponentDialog();

		GameObjectComponentTypeOption GetGameObjectComponentTypeOption() const;
};

#endif