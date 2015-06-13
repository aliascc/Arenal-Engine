/********************************************************
*
* Author: Carlos Chacón N.
*
* Copyright 2012-2015
*
*********************************************************/

#pragma once

#ifndef _LOAD_PROJECT_DIALOG_H
#define _LOAD_PROJECT_DIALOG_H

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
#include "GameContentDefs.h"
#include "ui_LoadProjectDialogQt.h"

/********************
*   Forward Decls   *
*********************/
class EngineViewer;

/*****************
*   Class Decl   *
******************/
class LoadProjectDialog sealed : public QDialog
{
	Q_OBJECT

	private:

		Ui::LoadProjectDialogQt m_LoadProjectDialogQtUI;

		EngineViewer* m_EngineViewer = nullptr;

		QString m_Directory = "";

		std::wstring m_ConfigFile;

	private slots:

		void on_m_SelectFileBtn_clicked();

	public:
		LoadProjectDialog(EngineViewer* engineViewer, QWidget *parent = 0);
		virtual ~LoadProjectDialog();

		inline const std::wstring& GetConfigFile() const
		{
			return m_ConfigFile;
		}
};

#endif