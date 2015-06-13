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
LoadProjectDialog::LoadProjectDialog(EngineViewer* engineViewer, QWidget *parent)
	: QDialog(parent)
	, m_EngineViewer(engineViewer)
{
	AEAssert(m_EngineViewer != nullptr);

	m_LoadProjectDialogQtUI.setupUi(this);
}

LoadProjectDialog::~LoadProjectDialog()
{
}

void LoadProjectDialog::on_m_SelectFileBtn_clicked()
{
	if (m_EngineViewer == nullptr)
	{
		return;
	}

	QString filter = "Project Configuration File (*.xml)";
	QString configFile = QFileDialog::getOpenFileName(this, tr("Select Project Config"), QString(), filter);

	m_ConfigFile = configFile.toStdWString();

	m_LoadProjectDialogQtUI.m_ProjectConfigTB->setText(configFile);
}
