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
#include "Utils\XEQTDefs.h"
#include "Base\BaseFunctions.h"
#include "GameObjectAddComponentDialog.h"

//Always include last
#include "Memory\MemLeaks.h"

/********************
*   Function Defs   *
*********************/
GameObjectAddComponentDialog::GameObjectAddComponentDialog(QWidget *parent)
	: QDialog(parent)
{
	m_GameObjectAddComponentDialogQtUI.setupUi(this);

	XEQTHelpers::SetComboBoxGameObjectComponentTypeOption(m_GameObjectAddComponentDialogQtUI.m_GameComponentTypeComboBox);
	m_GameObjectAddComponentDialogQtUI.m_GameComponentTypeComboBox->setCurrentIndex(0);
}

GameObjectAddComponentDialog::~GameObjectAddComponentDialog()
{
}

GameObjectComponentTypeOption GameObjectAddComponentDialog::GetGameObjectComponentTypeOption() const
{
	int currentIndex = m_GameObjectAddComponentDialogQtUI.m_GameComponentTypeComboBox->currentIndex();

	XEAssert(currentIndex != -1);
	if(currentIndex == -1)
	{
		return GameObjectComponentTypeOption::Mesh;
	}

	GameObjectComponentTypeOption gameObjectComponentTypeOption = static_cast<GameObjectComponentTypeOption>(m_GameObjectAddComponentDialogQtUI.m_GameComponentTypeComboBox->itemData(currentIndex).toUInt());
	
	return gameObjectComponentTypeOption;
}
