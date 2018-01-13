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

#ifndef _CODE_EDITOR_TEXT_EDIT_H
#define _CODE_EDITOR_TEXT_EDIT_H

/**********************
*   System Includes   *
***********************/
#include <map>
#include <string>
#include <stdint.h>

/*************************
*   3rd Party Includes   *
**************************/
#include "qevent.h"
#include "QtWidgets\qplaintextedit.h"

/***************************
*   Game Engine Includes   *
****************************/
#include "Base\Base.h"
#include "Time\AETimeDefs.h"

/********************
*   Forward Decls   *
*********************/
class CodeSyntaxHighlither;

/*****************
*   Class Decl   *
******************/
class CodeEditorTextEdit sealed : public QPlainTextEdit
{
	Q_OBJECT

	private:

		QWidget* m_LineNumberArea;

		CodeSyntaxHighlither* m_CodeSyntaxHighlither;

		QString m_Filename;

		TimeStamp m_LastModifiedTimeStamp;

		QByteArray m_ModifiedHash;

		bool m_IsReady;

		void OpenCodeFile();

		bool CheckIfModifiedOutside();

		void resizeEvent(QResizeEvent* rEvent) override;

		void focusInEvent(QFocusEvent* focusEvent) override;

	private slots:

		void UpdateLineNumberAreaWidth(int newBlockCount);

		void UpdateLineNumberArea(const QRect& rectArea, int difference);

	public:
		CodeEditorTextEdit(const QString& filename, QWidget* parent = nullptr);
		~CodeEditorTextEdit();

		inline bool IsReady() const
		{
			return m_IsReady;
		}

		bool WasModified() const;

		inline const QString& GetFilename() const
		{
			return m_Filename;
		}

		int32_t GetLineNumberAreaWidth();

		void LineNumberAreaPaintEvent(QPaintEvent* pEvent);

		AEResult ReloadFile();

		AEResult SaveFile();
};

#endif // AEngine_EDITOR_H
