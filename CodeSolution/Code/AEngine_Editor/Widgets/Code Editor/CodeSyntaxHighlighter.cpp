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
#include "Utils\AEQTDefs.h"
#include "Color\AEColorDefs.h"
#include "CodeSyntaxHighlighter.h"

//Always include last
#include "Memory\MemLeaks.h"

/********************
*   Function Defs   *
*********************/
CodeSyntaxHighlither::CodeSyntaxHighlither(QTextDocument *parent)
	: QSyntaxHighlighter(parent)
{
	/////////////////////////////////////////////////
	//Set Keywords
	m_KeywordFormat.setForeground(AEQTHelpers::GetQColorFromColor(AEColors::CornflowerBlue));
	m_KeywordFormat.setFontItalic(true);

	QStringList keywordPatterns;

	keywordPatterns << "\\buint\\b" << "\\int\\b" << "\\bmatrix\\b"
					<< "\\bfloat\\b" << "\\bfloat2\\b" << "\\bfloat3\\b"
					<< "\\bfloat4\\b" << "\\bint2\\b" << "\\bint3\\b"
					<< "\\bint4\\b" << "\\bregister\\b" << "\\bSamplerState\\b"
					<< "\\breturn\\b" << "\\bpackoffset\\b" << "\\bTexture2DArray\\b"
					<< "\\bTexture2D\\b" << "\\bStructuredBuffer\\b" << "\\bBuffer\\b"
					<< "\\bwhile\\b" << "\\bfor\\b" << "\\bstruct\\b"
					<< "\\bdefault\\b" << "\\bcase\\b" << "\\bcbuffer\\b";

	uint32_t sizePattens = (uint32_t)keywordPatterns.size();
	for(uint32_t i = 0; i < sizePattens; i++)
	{
		CodeSyntaxHighlightRule keywordRule;

		keywordRule.m_Pattern = QRegExp(keywordPatterns[i]);
		keywordRule.m_Format = m_KeywordFormat;

		m_CodeHighlightingRules.push_back(keywordRule);
	}

	/////////////////////////////////////////////////
	//Preprocessor
	CodeSyntaxHighlightRule preprocessorCommentRule;

	m_PreprocessorsFormat.setForeground(AEQTHelpers::GetQColorFromColor(AEColors::IndianRed));
	preprocessorCommentRule.m_Pattern = QRegExp("#[^\n]*");
	preprocessorCommentRule.m_Format = m_PreprocessorsFormat;
	m_CodeHighlightingRules.push_back(preprocessorCommentRule);

	/////////////////////////////////////////////////
	//Set Comments
	CodeSyntaxHighlightRule singleLineCommentRule;
	m_SingleLineCommentFormat.setForeground(AEQTHelpers::GetQColorFromColor(AEColors::ForestGreen));
	singleLineCommentRule.m_Pattern = QRegExp("//[^\n]*");
	singleLineCommentRule.m_Format = m_SingleLineCommentFormat;
	m_CodeHighlightingRules.push_back(singleLineCommentRule);
	m_SingleLineCommentFormat.setFontItalic(true);
	m_SingleLineCommentFormat.setFontWeight(QFont::Weight::DemiBold);

	m_MultiLineCommentFormat.setForeground(AEQTHelpers::GetQColorFromColor(AEColors::ForestGreen));

	m_CommentStartExpression = QRegExp("/\\*");
	m_CommentEndExpression = QRegExp("\\*/");

	/////////////////////////////////////////////////
	//Strings
	CodeSyntaxHighlightRule quotationRule;
	m_QuotationFormat.setForeground(AEQTHelpers::GetQColorFromColor(AEColors::OrangeRed));
	quotationRule.m_Pattern = QRegExp("\".*\"");
	quotationRule.m_Format = m_QuotationFormat;
	m_CodeHighlightingRules.push_back(quotationRule);

	/////////////////////////////////////////////////
	//Set Functions
	CodeSyntaxHighlightRule functionRule;
	m_FunctionFormat.setForeground(AEQTHelpers::GetQColorFromColor(AEColors::Orange));
	functionRule.m_Pattern = QRegExp("\\b[A-Za-z0-9_]+(?=\\()");
	functionRule.m_Format = m_FunctionFormat;
	m_CodeHighlightingRules.push_back(functionRule);
}

CodeSyntaxHighlither::~CodeSyntaxHighlither()
{
}

void CodeSyntaxHighlither::highlightBlock(const QString& text)
{
	for(const CodeSyntaxHighlightRule& rule : m_CodeHighlightingRules)
	{
		QRegExp expression(rule.m_Pattern);

		int32_t index = expression.indexIn(text);

		while(index >= 0)
		{
			int32_t length = expression.matchedLength();

			setFormat(index, length, rule.m_Format);

			index = expression.indexIn(text, index + length);
		}
	}

	setCurrentBlockState(0);

	int32_t startIndex = 0;
	if(previousBlockState() != 1)
	{
		startIndex = m_CommentStartExpression.indexIn(text);
	}

	while(startIndex >= 0) 
	{
		int32_t endIndex = m_CommentEndExpression.indexIn(text, startIndex);
		int32_t commentLength = 0;

		if(endIndex == -1) 
		{
			setCurrentBlockState(1);
			commentLength = text.length() - startIndex;
		}
		else
		{
			commentLength = endIndex - startIndex + m_CommentEndExpression.matchedLength();
		}

		setFormat(startIndex, commentLength, m_MultiLineCommentFormat);
		startIndex = m_CommentStartExpression.indexIn(text, startIndex + commentLength);
	}
}