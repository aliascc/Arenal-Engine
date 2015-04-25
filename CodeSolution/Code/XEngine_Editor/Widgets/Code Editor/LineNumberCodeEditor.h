/********************************************************
*
* Author: Carlos Chacón N.
*
* Copyright 2012-2015
*
*********************************************************/

#pragma once

#ifndef _LINE_NUMBER_CODE_EDITOR_H
#define _LINE_NUMBER_CODE_EDITOR_H

/**********************
*   System Includes   *
***********************/
#include <string>
#include <stdint.h>

/*************************
*   3rd Party Includes   *
**************************/
#include "qevent.h"
#include "QtWidgets\qwidget.h"

/***************************
*   Game Engine Includes   *
****************************/

/********************
*   Forward Decls   *
*********************/
class CodeEditorTextEdit;

/*****************
*   Class Decl   *
******************/
class LineNumberCodeEditor sealed : public QWidget
{
	private:
		CodeEditorTextEdit* m_CodeEditorTextEdit;

	protected:
		void paintEvent(QPaintEvent* pEvent) override;

	public:
		LineNumberCodeEditor(CodeEditorTextEdit* codeEditorTextEdit);
		~LineNumberCodeEditor();

		QSize sizeHint() const override;
};

#endif