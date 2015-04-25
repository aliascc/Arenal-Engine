/********************************************************
*
* Author: Carlos Chacón N.
*
* Copyright 2012-2015
*
*********************************************************/

#pragma once

#ifndef _NEW_PROJECT_DIALOG_H
#define _NEW_PROJECT_DIALOG_H

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
#include "ui_NewProjectDialogQt.h"

/********************
*   Forward Decls   *
*********************/
class EngineViewer;

/*****************
*   Class Decl   *
******************/
class NewProjectDialog sealed : public QDialog
{
	Q_OBJECT

	private:

		Ui::NewProjectDialogQt m_NewProjectDialogQtUI;

		EngineViewer* m_EngineViewer = nullptr;

		QString m_Directory = "";

		std::wstring m_ConfigFile;

	private slots:

		void on_m_SelectDirBtn_clicked();

		void on_m_ProjectNameTB_textChanged(QString str);

		void on_m_OkButton_clicked();

		void on_m_CreateProjectFolderCB_toggled(bool value);

	public:
		NewProjectDialog(EngineViewer* engineViewer, QWidget *parent = 0);
		virtual ~NewProjectDialog();

		inline const std::wstring& GetConfigFile() const
		{
			return m_ConfigFile;
		}
};

#endif