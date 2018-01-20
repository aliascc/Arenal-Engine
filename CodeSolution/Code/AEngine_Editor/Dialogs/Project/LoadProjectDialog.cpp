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
#include "qfile.h"
#include "qfiledialog.h"
#include "qmessagebox.h"

/***************************
*   Game Engine Includes   *
****************************/
#include "Utils\AEQTDefs.h"
#include "LoadProjectDialog.h"
#include "Base\BaseFunctions.h"
#include "Base\BaseLocations.h"
#include "Engine Viewer\EngineViewer.h"

//Always include last
#include "Memory\MemLeaks.h"

/********************
*   Function Defs   *
*********************/
LoadProjectDialog::LoadProjectDialog(QWidget *parent)
    : QDialog(parent)
{
    m_LoadProjectDialogQtUI.setupUi(this);
}

LoadProjectDialog::~LoadProjectDialog()
{
}

void LoadProjectDialog::on_m_SelectFileBtn_clicked()
{
    QString filter = "Project Configuration File (*.xml)";
    QString configFile = QFileDialog::getOpenFileName(this, tr("Select Project Config"), QString(), filter);

    m_ConfigFile = configFile.toStdWString();

    m_LoadProjectDialogQtUI.m_ProjectConfigTB->setText(configFile);
}
