/********************************************************
*
* Author: Carlos Chacón N.
*
* Copyright 2012-2015
*
*********************************************************/

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