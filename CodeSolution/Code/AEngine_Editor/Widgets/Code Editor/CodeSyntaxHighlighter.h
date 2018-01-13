/*
* Copyright (c) 2018 <Carlos Chac�n>
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

#ifndef _CODE_SYNTAX_HIGHLIGHTER_H
#define _CODE_SYNTAX_HIGHLIGHTER_H

/**********************
*   System Includes   *
***********************/
#include <string>
#include <vector>
#include <stdint.h>

/*************************
*   3rd Party Includes   *
**************************/

/***************************
*   Game Engine Includes   *
****************************/
#include "qsyntaxhighlighter.h"

/********************
*   Forward Decls   *
*********************/

/*****************
*   Class Decl   *
******************/
class CodeSyntaxHighlither sealed : public QSyntaxHighlighter
{
	Q_OBJECT

	private:
		struct CodeSyntaxHighlightRule
		{
			QRegExp m_Pattern;
			QTextCharFormat m_Format;
		};

		std::vector<CodeSyntaxHighlightRule> m_CodeHighlightingRules;

		QRegExp m_CommentStartExpression;
		QRegExp m_CommentEndExpression;

		QTextCharFormat m_KeywordFormat;
		QTextCharFormat m_PreprocessorsFormat;
		QTextCharFormat m_SingleLineCommentFormat;
		QTextCharFormat m_MultiLineCommentFormat;
		QTextCharFormat m_QuotationFormat;
		QTextCharFormat m_FunctionFormat;

	protected:
		void highlightBlock(const QString& text) override;

	public:
		CodeSyntaxHighlither(QTextDocument* parent = nullptr);
		~CodeSyntaxHighlither();
};

#endif