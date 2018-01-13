/*
* Copyright (c) 2018 <Carlos Chacón>
* All rights reserved
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
* http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*/

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