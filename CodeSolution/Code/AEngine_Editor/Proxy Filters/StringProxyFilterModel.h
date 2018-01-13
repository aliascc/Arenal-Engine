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