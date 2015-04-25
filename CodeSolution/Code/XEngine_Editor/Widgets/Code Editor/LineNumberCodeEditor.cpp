/********************************************************
*
* Author: Carlos Chacón N.
*
* Copyright 2012-2015
*
*********************************************************/

/**********************
*   System Includes   *
***********************/

/*************************
*   3rd Party Includes   *
**************************/

/***************************
*   Game Engine Includes   *
****************************/
#include "Base\BaseFunctions.h"
#include "CodeEditorTextEdit.h"
#include "LineNumberCodeEditor.h"

//Always include last
#include "Memory\MemLeaks.h"

/********************
*   Function Defs   *
*********************/
LineNumberCodeEditor::LineNumberCodeEditor(CodeEditorTextEdit* codeEditorTextEdit)
	: QWidget(codeEditorTextEdit)
	, m_CodeEditorTextEdit(codeEditorTextEdit)
{
	XEAssert(codeEditorTextEdit != nullptr);
}

LineNumberCodeEditor::~LineNumberCodeEditor()
{
}

QSize LineNumberCodeEditor::sizeHint() const
{
	if(m_CodeEditorTextEdit != nullptr)
	{
		return QSize(m_CodeEditorTextEdit->GetLineNumberAreaWidth(), 0);
	}

	return QSize(0, 0);
}

void LineNumberCodeEditor::paintEvent(QPaintEvent* pEvent)
{
	if(m_CodeEditorTextEdit != nullptr)
	{
		m_CodeEditorTextEdit->LineNumberAreaPaintEvent(pEvent);
	}
}