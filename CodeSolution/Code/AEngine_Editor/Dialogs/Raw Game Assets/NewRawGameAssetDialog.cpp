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
#include "Base\BaseFunctions.h"
#include "NewRawGameAssetDialog.h"

//Always include last
#include "Memory\MemLeaks.h"

/********************
*   Function Defs   *
*********************/
NewRawGameAssetDialog::NewRawGameAssetDialog(QWidget *parent)
	: QDialog(parent)
{
	m_NewRawGameAssetDialogQtUI.setupUi(this);

	AEQTHelpers::SetComboBoxGameContentSubType(m_NewRawGameAssetDialogQtUI.m_GameContentSubTypeComboBox);
	m_NewRawGameAssetDialogQtUI.m_GameContentSubTypeComboBox->setCurrentIndex(0);
}

NewRawGameAssetDialog::~NewRawGameAssetDialog()
{
}

void NewRawGameAssetDialog::on_m_OpenFile_clicked()
{
	QString fileName = QFileDialog::getOpenFileName(this, tr("Open Raw Game Asset"));

	m_NewRawGameAssetDialogQtUI.m_RawAssetFileTextBox->setText(fileName);
}

void NewRawGameAssetDialog::SetFilename(const QString& filename)
{
	m_NewRawGameAssetDialogQtUI.m_RawAssetFileTextBox->setText(filename);
}

std::wstring NewRawGameAssetDialog::GetFilename() const
{
	return m_NewRawGameAssetDialogQtUI.m_RawAssetFileTextBox->text().toStdWString();
}

GameContentSubtype NewRawGameAssetDialog::GetGameContentSubtype() const
{
	int currentIndex = m_NewRawGameAssetDialogQtUI.m_GameContentSubTypeComboBox->currentIndex();

	AEAssert(currentIndex != -1);
	if(currentIndex == -1)
	{
		return GameContentSubtype::None;
	}

	GameContentSubtype gameContentSubtype = static_cast<GameContentSubtype>(m_NewRawGameAssetDialogQtUI.m_GameContentSubTypeComboBox->itemData(currentIndex).toUInt());
	
	return gameContentSubtype;
}