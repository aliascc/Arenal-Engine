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
#include "qfile.h"
#include "qfiledialog.h"
#include "qmessagebox.h"

/***************************
*   Game Engine Includes   *
****************************/
#include "Utils\AEQTDefs.h"
#include "NewProjectDialog.h"
#include "Base\BaseFunctions.h"
#include "Base\BaseLocations.h"
#include "Engine Viewer\EngineViewer.h"

//Always include last
#include "Memory\MemLeaks.h"

/********************
*   Function Defs   *
*********************/
NewProjectDialog::NewProjectDialog(EngineViewer* engineViewer, QWidget *parent)
	: QDialog(parent)
	, m_EngineViewer(engineViewer)
{
	AEAssert(m_EngineViewer != nullptr);

	m_NewProjectDialogQtUI.setupUi(this);
}

NewProjectDialog::~NewProjectDialog()
{
}

void NewProjectDialog::on_m_SelectDirBtn_clicked()
{
	if (m_EngineViewer == nullptr)
	{
		return;
	}

	m_Directory = QFileDialog::getExistingDirectory(this, tr("Select Project Folder"));

	if (m_NewProjectDialogQtUI.m_CreateProjectFolderCB->isChecked())
	{
		m_NewProjectDialogQtUI.m_ProjectFolderTB->setText( (m_Directory + "/" + m_NewProjectDialogQtUI.m_ProjectNameTB->text()) );
	}
	else
	{
		m_NewProjectDialogQtUI.m_ProjectFolderTB->setText(m_Directory);
	}
}

void NewProjectDialog::on_m_OkButton_clicked()
{
	std::wstring projectDir = L"";
	std::wstring projectName = L"";
	m_ConfigFile = L"";
	bool createProjectDir = m_NewProjectDialogQtUI.m_CreateProjectFolderCB->isChecked();

	if (m_Directory.isEmpty())
	{
		AETODO("Error message box");
		return;
	}

	if (m_NewProjectDialogQtUI.m_ProjectNameTB->text().isEmpty())
	{
		AETODO("Error message box");
		return;
	}

	if (createProjectDir)
	{
		projectDir = ((m_Directory + "/" + m_NewProjectDialogQtUI.m_ProjectNameTB->text())).toStdWString();
	}
	else
	{
		projectDir = m_Directory.toStdWString();
	}

	projectName = m_NewProjectDialogQtUI.m_ProjectNameTB->text().toStdWString();

	if (m_EngineViewer->CreateProjectFolder(projectDir, projectName, createProjectDir) != AEResult::Ok)
	{
		AETODO("Add message");
		return;
	}

	m_ConfigFile = projectDir + L"/" + AE_PROJ_CONFIG_PROJ_FILE;

	accept();
}

void NewProjectDialog::on_m_ProjectNameTB_textChanged(QString str)
{
	if (m_Directory.isEmpty() || !m_NewProjectDialogQtUI.m_CreateProjectFolderCB->isChecked())
	{
		return;
	}

	m_NewProjectDialogQtUI.m_ProjectFolderTB->setText((m_Directory + "/" + m_NewProjectDialogQtUI.m_ProjectNameTB->text()));
}

void NewProjectDialog::on_m_CreateProjectFolderCB_toggled(bool value)
{
	if (value)
	{
		m_NewProjectDialogQtUI.m_ProjectFolderTB->setText( (m_Directory + "/" + m_NewProjectDialogQtUI.m_ProjectNameTB->text()) );
	}
	else
	{
		m_NewProjectDialogQtUI.m_ProjectFolderTB->setText(m_Directory);
	}
}
