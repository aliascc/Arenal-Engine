
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
#include "qfile.h"
#include "qfiledialog.h"
#include "qmessagebox.h"

/***************************
*   Game Engine Includes   *
****************************/
#include "Utils\XEQTDefs.h"
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

	XEQTHelpers::SetComboBoxGameContentSubType(m_NewRawGameAssetDialogQtUI.m_GameContentSubTypeComboBox);
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

	XEAssert(currentIndex != -1);
	if(currentIndex == -1)
	{
		return GameContentSubtype::None;
	}

	GameContentSubtype gameContentSubtype = static_cast<GameContentSubtype>(m_NewRawGameAssetDialogQtUI.m_GameContentSubTypeComboBox->itemData(currentIndex).toUInt());
	
	return gameContentSubtype;
}