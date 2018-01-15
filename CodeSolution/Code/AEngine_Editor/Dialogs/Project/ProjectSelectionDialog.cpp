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
#include "Utils\AEQTDefs.h"
#include "Utils\GlobalConfig.h"
#include "ProjectSelectionDialog.h"
#include "Engine Viewer\EngineViewer.h"
#include "Dialogs\Project\NewProjectDialog.h"
#include "Dialogs\Project\LoadProjectDialog.h"

//Always include last
#include "Memory\MemLeaks.h"

/********************
*   Function Defs   *
*********************/
ProjectSelectionDialog::ProjectSelectionDialog(EngineViewer* engineViewer, QWidget *parent)
	: QDialog(parent)
	, m_EngineViewer(engineViewer)
{
	assert(m_EngineViewer != nullptr);

	m_ProjectSelectionDialogQtUI.setupUi(this);

	InitFields();
}

ProjectSelectionDialog::~ProjectSelectionDialog()
{
}

void ProjectSelectionDialog::InitFields()
{
	//this->setWindowFlags(Qt::CustomizeWindowHint | Qt::WindowTitleHint);

    std::vector<std::wstring> projects;
    AE_GlobalConfig::GetRecentProjects(projects);

    for (const std::wstring& proj : projects)
    {
        QListWidgetItem* qtListItem = new QListWidgetItem();
        qtListItem->setData(0, QString::fromStdWString(proj));
        qtListItem->setData(AE_QT_ITEM_DATA_ROLE_PROJECT_DIR_LOCATION, QString::fromStdWString(proj));
        this->m_ProjectSelectionDialogQtUI.m_RecentProjectList->addItem(qtListItem);
    }
}

void ProjectSelectionDialog::on_m_OpenProjBtn_clicked()
{
}

void ProjectSelectionDialog::on_m_CreateProjBtn_clicked()
{
    NewProjectDialog newProject(m_EngineViewer);

    int result = newProject.exec();
    if (result != QDialog::Accepted)
    {
        return;
    }

    m_ConfigFile = newProject.GetConfigFile();
}

void ProjectSelectionDialog::on_m_LoadProjBtn_clicked()
{
    LoadProjectDialog loadProject(nullptr);

    int result = loadProject.exec();
    if (result != QDialog::Accepted)
    {
        return;
    }

    m_ConfigFile = loadProject.GetConfigFile();
}