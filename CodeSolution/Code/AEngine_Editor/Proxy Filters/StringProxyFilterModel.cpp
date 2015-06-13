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