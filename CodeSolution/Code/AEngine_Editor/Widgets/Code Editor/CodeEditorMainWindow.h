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

#ifndef _CODE_EDITOR_MAIN_WINDOW_H
#define _CODE_EDITOR_MAIN_WINDOW_H

/**********************
*   System Includes   *
***********************/
#include <string>
#include <stdint.h>

/*************************
*   3rd Party Includes   *
**************************/
#include "QtWidgets\qmainwindow.h"

/***************************
*   Game Engine Includes   *
****************************/
#include "Base\Base.h"
#include "ui_CodeEditorMainWindowQt.h"

/********************
*   Forward Decls   *
*********************/

/*****************
*   Class Decl   *
******************/
class CodeEditorMainWindow sealed : public QMainWindow
{
	Q_OBJECT

	typedef std::map<QString, int32_t> TabFileMap;

	private:

		Ui::CodeEditorMainWindowQt m_CodeEditorMainWindowQtUI;

		AEResult NewCodeTab(const QString& codeFilepath, const QString& fileName);

		TabFileMap m_TabFileMap;

		AEResult SaveCurrent(); 

		AEResult SaveAll(); 

		AEResult CloseCurrent();

		AEResult CloseAll();

		void closeEvent(QCloseEvent* cEvent) override;

	private slots:

		void on_m_SaveTBAction_triggered();

		void on_m_SaveFileAction_triggered();

		void on_m_SaveAllFileAction_triggered();

		void on_m_CloseFileAction_triggered();

	public:

		CodeEditorMainWindow(QWidget* parent = nullptr);
		~CodeEditorMainWindow();

		AEResult AddCodeTab(const QString& codeFilepath);

		AEResult AddCodeTab(const std::wstring& codeFilepath);
};

#endif // AEngine_EDITOR_H
