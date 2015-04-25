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
#include "qtreewidget.h"

/***************************
*   Game Engine Includes   *
****************************/
#include "Utils\XEQTDefs.h"
#include "Base\BaseFunctions.h"
#include "GameObject\GameObject.h"
#include "GameAssets\Assets\MeshAsset.h"
#include "GameObjectAudioListenerComponentWidget.h"
#include "GameObject\Components\AudioListenerGOC.h"

//Always include last
#include "Memory\MemLeaks.h"

/********************
*   Function Defs   *
*********************/
GameObjectAudioListenerComponentWidget::GameObjectAudioListenerComponentWidget(GameObject* gameObject, QWidget *parent)
	: QWidget(parent)
	, m_GameObject(gameObject)
{
	m_GameObjectAudioListenerComponentWidgetQtUI.setupUi(this);

	XEAssert(m_GameObject != nullptr);
}

GameObjectAudioListenerComponentWidget::~GameObjectAudioListenerComponentWidget()
{
}

void GameObjectAudioListenerComponentWidget::InitFields()
{
	XEAssert(m_GameObject != nullptr);
	if (m_GameObject == nullptr)
	{
		return;
	}

	XEAssert(m_GameObject->HasAudioListenerGOC());
	if (!m_GameObject->HasAudioListenerGOC())
	{
		return;
	}

	m_IsReady = true;
}
