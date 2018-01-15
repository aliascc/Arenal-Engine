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

#ifndef _PROJECT_SELECTION_dialog_H
#define _PROJECT_SELECTION_dialog_H

/**********************
*   System Includes   *
***********************/
#include <stdint.h>

/*************************
*   3rd Party Includes   *
**************************/
#include "qevent.h"
#include "QtWidgets\qdialog.h"
#include "QtWidgets\qwidget.h"

/***************************
*   Game Engine Includes   *
****************************/
#include "Base\Base.h"
#include "ui_ProjectSelectionDialogQt.h"

/********************
*   Forward Decls   *
*********************/
class GameObject;
class EngineViewer;
class GameAssetManager;

/*****************
*   Class Decl   *
******************/

class ProjectSelectionDialog sealed : public QDialog
{
	Q_OBJECT

	private:

		Ui::ProjectSelectionDialogQt m_ProjectSelectionDialogQtUI;

        EngineViewer* m_EngineViewer = nullptr;

        std::wstring m_ConfigFile = L"";

		void InitFields();

	private slots:

        void on_m_OpenProjBtn_clicked();

        void on_m_CreateProjBtn_clicked();

        void on_m_LoadProjBtn_clicked();

	public:
        ProjectSelectionDialog(EngineViewer* engineViewer, QWidget *parent = nullptr);
		~ProjectSelectionDialog();
};


#endif