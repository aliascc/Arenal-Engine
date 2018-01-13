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

/**********************
*   System Includes   *
***********************/

/*************************
*   3rd Party Includes   *
**************************/

/***************************
*   Game Engine Includes   *
****************************/
#include "StringProxyFilterModel.h"

//Always include last
#include "Memory\MemLeaks.h"

/********************
*   Function Defs   *
*********************/
StringProxyFilterModel::StringProxyFilterModel(QObject *parent, uint32_t searchColumn, QRegExp::PatternSyntax patternSyntax, Qt::CaseSensitivity caseSensitivity)
	: QSortFilterProxyModel(parent)
	, m_SearchColumn(searchColumn)
	, m_PatternSyntax(patternSyntax)
	, m_CaseSensitivity(caseSensitivity)
{
}

StringProxyFilterModel::~StringProxyFilterModel(void)
{
}

bool StringProxyFilterModel::filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const
{
	QModelIndex index0 = sourceModel()->index(sourceRow, m_SearchColumn, sourceParent);

	return (sourceModel()->data(index0).toString().contains(filterRegExp()));
}

bool StringProxyFilterModel::lessThan(const QModelIndex &left, const QModelIndex &right) const
{
	QVariant leftData = sourceModel()->data(left);
	QVariant rightData = sourceModel()->data(right);

	QString leftString = leftData.toString();
	QString rightString = rightData.toString();

	return QString::localeAwareCompare(leftString, rightString) < 0;
}

void StringProxyFilterModel::SetStringFilter(const QString& string)
{
	QRegExp regExp(string, m_CaseSensitivity, m_PatternSyntax);

	this->setFilterRegExp(regExp);
}