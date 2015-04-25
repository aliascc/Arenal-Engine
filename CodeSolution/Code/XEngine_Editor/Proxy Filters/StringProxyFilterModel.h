/********************************************************
*
* Author: Carlos Chacón N.
*
* Copyright 2012-2015
*
*********************************************************/

#pragma once

#ifndef _STRING_PROXY_FILTER_MODEL_H
#define _STRING_PROXY_FILTER_MODEL_H

/**********************
*   System Includes   *
***********************/
#include <stdint.h>

/*************************
*   3rd Party Includes   *
**************************/
#include "QSortFilterProxyModel"

/***************************
*   Game Engine Includes   *
****************************/

/********************
*   Forward Decls   *
*********************/

/*****************
*   Class Decl   *
******************/
class StringProxyFilterModel sealed : public QSortFilterProxyModel
{
	private:

		uint32_t m_SearchColumn;

		QRegExp::PatternSyntax m_PatternSyntax;

		Qt::CaseSensitivity m_CaseSensitivity;

		bool filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const override;

		bool lessThan(const QModelIndex &left, const QModelIndex &right) const override;

	public:
		StringProxyFilterModel(QObject *parent = 0, uint32_t searchColumn = 0, QRegExp::PatternSyntax patternSyntax = QRegExp::PatternSyntax::FixedString, Qt::CaseSensitivity caseSensitivity = Qt::CaseSensitivity::CaseInsensitive);
		virtual ~StringProxyFilterModel(void);

		inline Qt::CaseSensitivity GetCaseSensitive() const
		{
			return m_CaseSensitivity;
		}

		inline uint32_t GetSearchColumn() const
		{
			return m_SearchColumn;
		}

		inline QRegExp::PatternSyntax GetPatternSyntax() const
		{
			return m_PatternSyntax;
		}

		inline void SetCaseSensitive(Qt::CaseSensitivity caseSensitivity)
		{
			m_CaseSensitivity = caseSensitivity;
		}

		inline void SetSearchColumn(uint32_t searchColumn)
		{
			m_SearchColumn = searchColumn;
		}

		inline void SetPatternSyntax(QRegExp::PatternSyntax patternSyntax)
		{
			m_PatternSyntax = patternSyntax;
		}

		void SetStringFilter(const QString& string);
};

#endif