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
#include "qpainter.h"
#include "glm\glm.hpp"
#include "qtextobject.h"
#include "qmessagebox.h"
#include "qtextstream.h"
#include "qapplication.h"
#include "qcryptographichash.h"

/***************************
*   Game Engine Includes   *
****************************/
#include "Base\BaseFunctions.h"
#include "CodeEditorTextEdit.h"
#include "LineNumberCodeEditor.h"
#include "CodeSyntaxHighlighter.h"

//Always include last
#include "Memory\MemLeaks.h"

/****************
*   Un Define   *
*****************/
#undef max
#undef min

/********************
*   Function Defs   *
*********************/
CodeEditorTextEdit::CodeEditorTextEdit(const QString& filename, QWidget* parent)
	: QPlainTextEdit(parent)
	, m_Filename(filename)
	, m_IsReady(false)
{
	const int32_t tabStop = 4;
	this->setTabStopWidth(tabStop * fontMetrics().width(QLatin1Char(' ')));

	m_LineNumberArea = new LineNumberCodeEditor(this);

	connect(this, SIGNAL(blockCountChanged(int)), this, SLOT(UpdateLineNumberAreaWidth(int)));
	connect(this, SIGNAL(updateRequest(QRect, int)), this, SLOT(UpdateLineNumberArea(QRect, int)));
	connect(this, SIGNAL(textChanged()), this, SLOT(TextChangedEvent()));

	UpdateLineNumberAreaWidth(0);

	OpenCodeFile();

	m_CodeSyntaxHighlither = new CodeSyntaxHighlither(this->document());
}

CodeEditorTextEdit::~CodeEditorTextEdit()
{
	DeleteMem(m_CodeSyntaxHighlither);
}

XEResult CodeEditorTextEdit::ReloadFile()
{
	this->clear();

	OpenCodeFile();

	return XEResult::Ok;
}

void CodeEditorTextEdit::OpenCodeFile()
{
	QFile file(m_Filename);

	if(!file.open(QFile::ReadOnly | QFile::Text))
	{
		QMessageBox::warning(this, "XE Code Editor",
								tr("Unable to open File: %1:\n%2.")
								.arg(m_Filename)
								.arg(file.errorString()));

		return;
	}

	QTextStream codeTextFile(&file);

	QApplication::setOverrideCursor(Qt::WaitCursor);
	this->setPlainText(codeTextFile.readAll());
	QApplication::restoreOverrideCursor();

	file.close();

	m_ModifiedHash = QCryptographicHash::hash(this->toPlainText().toStdString().c_str(),QCryptographicHash::Md5);

	UpdateLineNumberAreaWidth(0);

	if(XE_Base::GetFileModifiedTime(m_Filename.toStdWString(), m_LastModifiedTimeStamp) != XEResult::Ok)
	{
		XEAssert(false);
	}

	m_IsReady = true;
}

bool CodeEditorTextEdit::CheckIfModifiedOutside()
{
	TimeStamp modifiedTime;

	if(XE_Base::GetFileModifiedTime(m_Filename.toStdWString(), modifiedTime) != XEResult::Ok)
	{
		XETODO("return better codes in case file is not existent");
		return false;
	}

	if(m_LastModifiedTimeStamp < modifiedTime)
	{
		return true;
	}

	return false;
}

int32_t CodeEditorTextEdit::GetLineNumberAreaWidth()
{
	int32_t digits = 1;

	int32_t max = glm::max(1, blockCount());

	while (max >= 10)
	{
		max /= 10;
		++digits;
	}

	int32_t space = 3 + fontMetrics().width(QLatin1Char('9')) * digits;

	return space;
}

void CodeEditorTextEdit::UpdateLineNumberAreaWidth(int newBlockCount)
{
	setViewportMargins(GetLineNumberAreaWidth(), 0, 0, 0);
}

void CodeEditorTextEdit::UpdateLineNumberArea(const QRect& rectArea, int difference)
{
	if(difference != 0)
	{
		m_LineNumberArea->scroll(0, difference);
	}
	else
	{
		m_LineNumberArea->update(0, rectArea.y(), m_LineNumberArea->width(), rectArea.height());
	}

	if(rectArea.contains(viewport()->rect()))
	{
		UpdateLineNumberAreaWidth(0);
	}
}

void CodeEditorTextEdit::resizeEvent(QResizeEvent* rEvent)
{
	QPlainTextEdit::resizeEvent(rEvent);

	QRect cr = contentsRect();
	m_LineNumberArea->setGeometry(QRect(cr.left(), cr.top(), GetLineNumberAreaWidth(), cr.height()));
}

void CodeEditorTextEdit::LineNumberAreaPaintEvent(QPaintEvent* pEvent)
{
	QPainter painter(m_LineNumberArea);

	painter.fillRect(pEvent->rect(), Qt::lightGray);

	QTextBlock block = firstVisibleBlock();
	int32_t blockNumber = block.blockNumber();
	int32_t top = (int32_t)blockBoundingGeometry(block).translated(contentOffset()).top();
	int32_t bottom = top + (int32_t)blockBoundingRect(block).height();

	while(block.isValid() && top <= pEvent->rect().bottom())
	{
		if(block.isVisible() && bottom >= pEvent->rect().top())
		{
			QString number = QString::number(blockNumber + 1);
			painter.setPen(Qt::black);
			painter.drawText(0, top, m_LineNumberArea->width(), fontMetrics().height(), Qt::AlignRight, number);
		}

		block = block.next();
		top = bottom;
		bottom = top + (int32_t)blockBoundingRect(block).height();
		++blockNumber;
	}
}

bool CodeEditorTextEdit::WasModified() const
{
	QByteArray encodedPass = QCryptographicHash::hash(this->toPlainText().toStdString().c_str(),QCryptographicHash::Md5);

	return (m_ModifiedHash != encodedPass);
}

XEResult CodeEditorTextEdit::SaveFile()
{
	if(!m_IsReady)
	{
		return XEResult::NotReady;
	}

	QFile file(m_Filename);

	if(!file.open(QFile::WriteOnly | QFile::Text))
	{
		QMessageBox::warning(this, "XE Code Editor",
								tr("Unable to write to File: %1:\n%2.")
								.arg(m_Filename)
								.arg(file.errorString()));

		return XEResult::OpenFileFail;
	}

	QTextStream codeTextFile(&file);

	QApplication::setOverrideCursor(Qt::WaitCursor);
	codeTextFile << this->toPlainText();
	QApplication::restoreOverrideCursor();

	file.close();

	m_ModifiedHash = QCryptographicHash::hash(this->toPlainText().toStdString().c_str(),QCryptographicHash::Md5);

	if(XE_Base::GetFileModifiedTime(m_Filename.toStdWString(), m_LastModifiedTimeStamp) != XEResult::Ok)
	{
		XEAssert(false);
	}

	return XEResult::Ok;
}

void CodeEditorTextEdit::focusInEvent(QFocusEvent* focusEvent)
{
	QPlainTextEdit::focusInEvent(focusEvent);

	if(CheckIfModifiedOutside())
	{
		if(XE_Base::GetFileModifiedTime(m_Filename.toStdWString(), m_LastModifiedTimeStamp) != XEResult::Ok)
		{
			XEAssert(false);
		}

		QMessageBox::StandardButton ret =  QMessageBox::information(	this,
																		"File Modified",
																		tr("File: %1 has been modified outside the editor, do you want to reload the file?")
																			.arg(m_Filename),
																		QMessageBox::StandardButton::Yes,
																		QMessageBox::StandardButton::No);

		if(ret == QMessageBox::StandardButton::Yes)
		{
			ReloadFile();
		}
	}
}