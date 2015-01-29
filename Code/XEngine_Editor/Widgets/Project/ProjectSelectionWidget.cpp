
/********************************************************
*
* Author: Carlos Chacón N.
*
* Desc:
*
*
* Copyright 2012-2014
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
#include "Utils\XEQTDefs.h"
#include "Base\BaseFunctions.h"
#include "ProjectSelectionWidget.h"

//Always include last
#include "Memory\MemLeaks.h"

/********************
*   Function Defs   *
*********************/
ProjectSelectionWidget::ProjectSelectionWidget(QWidget *parent)
	: QWidget(parent)
{
	m_ProjectSelectionWidgetQtUI.setupUi(this);

	//InitFields();
}

ProjectSelectionWidget::~ProjectSelectionWidget()
{
}