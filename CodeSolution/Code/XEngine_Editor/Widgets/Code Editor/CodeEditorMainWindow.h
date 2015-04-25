/********************************************************
*
* Author: Carlos Chacón N.
*
* Copyright 2012-2015
*
*********************************************************/

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

		XEResult NewCodeTab(const QString& codeFilepath, const QString& fileName);

		TabFileMap m_TabFileMap;

		XEResult SaveCurrent(); 

		XEResult SaveAll(); 

		XEResult CloseCurrent();

		XEResult CloseAll();

		void closeEvent(QCloseEvent* cEvent) override;

	private slots:

		void on_m_SaveTBAction_triggered();

		void on_m_SaveFileAction_triggered();

		void on_m_SaveAllFileAction_triggered();

		void on_m_CloseFileAction_triggered();

	public:

		CodeEditorMainWindow(QWidget* parent = nullptr);
		~CodeEditorMainWindow();

		XEResult AddCodeTab(const QString& codeFilepath);

		XEResult AddCodeTab(const std::wstring& codeFilepath);
};

#endif // XENGINE_EDITOR_H
