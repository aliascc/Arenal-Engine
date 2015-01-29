
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

#ifndef _PROJECT_SELECTION_WIDGET_H
#define _PROJECT_SELECTION_WIDGET_H

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
#include "ui_ProjectSelectionWidgetQt.h"

/********************
*   Forward Decls   *
*********************/
class GameObject;
class GameAssetManager;

/*****************
*   Class Decl   *
******************/

class ProjectSelectionWidget sealed : public QWidget
{
	Q_OBJECT

	private:

		Ui::ProjectSelectionWidgetQt m_ProjectSelectionWidgetQtUI;

	private slots:

	public:
		ProjectSelectionWidget(QWidget *parent = nullptr);
		~ProjectSelectionWidget();

};


#endif