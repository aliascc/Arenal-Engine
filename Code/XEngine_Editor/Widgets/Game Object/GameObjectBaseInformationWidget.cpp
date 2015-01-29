
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
#include "Base\BaseFunctions.h"
#include "GameObject\GameObject.h"
#include "Engine Viewer\EngineViewer.h"
#include "GameObjectBaseInformationWidget.h"

//Always include last
#include "Memory\MemLeaks.h"

/********************
*   Function Defs   *
*********************/
GameObjectBaseInformationWidget::GameObjectBaseInformationWidget(GameObject* gameObject, EngineViewer* engineViewer, QWidget *parent)
	: QWidget(parent)
	, m_GameObject(gameObject)
	, m_EngineViewer(engineViewer)
{
	m_GameObjectBaseInformationWidgetQtUI.setupUi(this);

	XEAssert(m_GameObject != nullptr);

	XEAssert(m_EngineViewer != nullptr);

	InitFields();
}

GameObjectBaseInformationWidget::~GameObjectBaseInformationWidget()
{
}

void GameObjectBaseInformationWidget::InitFields()
{
	XEAssert(m_GameObject != nullptr);
	XEAssert(m_EngineViewer != nullptr);
	if (m_GameObject == nullptr || m_EngineViewer == nullptr)
	{
		return;
	}

	QString id = QString::number(m_GameObject->GetUniqueID());
	m_GameObjectBaseInformationWidgetQtUI.m_GameObjectIDTextBox->setText(id);

	m_GameObjectBaseInformationWidgetQtUI.m_GameObjectNameTextBox->setText(QString::fromStdWString(m_GameObject->GetName()));
}

void GameObjectBaseInformationWidget::on_m_GameObjectNameTextBox_editingFinished()
{
	XEAssert(m_GameObject != nullptr);

	if(m_GameObject == nullptr)
	{
		return;
	}

	if(m_GameObjectBaseInformationWidgetQtUI.m_GameObjectNameTextBox->text().isEmpty())
	{
		m_GameObjectBaseInformationWidgetQtUI.m_GameObjectNameTextBox->setText(QString::fromStdWString(m_GameObject->GetName()));

		return;
	}

	QString newName = m_GameObjectBaseInformationWidgetQtUI.m_GameObjectNameTextBox->text();

	std::wstring newNameStd = newName.toStdWString();
	
	m_GameObject->SetName(newNameStd);

	emit (NameChanged(m_GameObject->GetUniqueID()));
}