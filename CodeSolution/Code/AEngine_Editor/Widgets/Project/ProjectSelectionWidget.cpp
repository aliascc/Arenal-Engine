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
#include "Base\BaseFunctions.h"
#include "ProjectSelectionWidget.h"
#include "Dialogs\Project\NewProjectDialog.h"
#include "Dialogs\Project\LoadProjectDialog.h"

//Always include last
#include "Memory\MemLeaks.h"

/********************
*   Function Defs   *
*********************/
ProjectSelectionWidget::ProjectSelectionWidget(QWidget *parent)
	: QWidget(parent)
{
	m_ProjectSelectionWidgetQtUI.setupUi(this);

	InitFields();
}

ProjectSelectionWidget::~ProjectSelectionWidget()
{
}

void ProjectSelectionWidget::InitFields()
{
}
