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
#include "GameAssets\GameAsset.h"
#include "GameObject\GameObject.h"
#include "GameAssets\Assets\MeshAsset.h"
#include "GameAssets\GameAssetManager.h"
#include "GameObjectMeshComponentWidget.h"
#include "GameObject\Components\MeshGOC.h"
#include "Dialogs\Game Objects\GameObjectSelectAssetDialog.h"

//Always include last
#include "Memory\MemLeaks.h"

/********************
*   Function Defs   *
*********************/
GameObjectMeshComponentWidget::GameObjectMeshComponentWidget(GameObject* gameObject, GameAssetManager* gameAssetManager, QWidget *parent)
	: QWidget(parent)
	, m_GameObject(gameObject)
	, m_GameAssetManager(gameAssetManager)
{
	m_GameObjectMeshComponentWidgetQtUI.setupUi(this);

	XEAssert(m_GameObject != nullptr);

	XEAssert(m_GameAssetManager != nullptr);

	InitFields();
}

GameObjectMeshComponentWidget::~GameObjectMeshComponentWidget()
{
}

void GameObjectMeshComponentWidget::InitFields()
{
	XEAssert(m_GameAssetManager != nullptr);
	if(m_GameAssetManager == nullptr)
	{
		return;
	}

	XEAssert(m_GameObject != nullptr);
	if(m_GameObject == nullptr)
	{
		return;
	}

	XEAssert(m_GameObject->HasMeshGOC());
	if(!m_GameObject->HasMeshGOC())
	{
		return;
	}

	MeshGOC* meshGOC = m_GameObject->GetMeshGOC();
	
	m_GameObjectMeshComponentWidgetQtUI.m_MeshAssetTextBox->setText(QString::fromStdWString(meshGOC->GetMeshName()));

	m_IsReady = true;
}

XEResult GameObjectMeshComponentWidget::DropAsset(QObject* object)
{
	if(!m_IsReady)
	{
		return XEResult::NotReady;
	}

	XEAssert(object != nullptr);
	if(object == nullptr)
	{
		return XEResult::NullParameter;
	}

	QTreeWidget* gameAssetTree = reinterpret_cast<QTreeWidget*>(object);

	QList<QTreeWidgetItem*> selectedItems = gameAssetTree->selectedItems();

	if(selectedItems.count() == 0)
	{
		return XEResult::Ok;
	}

	QTreeWidgetItem* item = selectedItems[0];
	
	uint64_t gameAssetID = item->data(0, XE_QT_ITEM_DATA_ROLE_GAME_ASSET_UNIQUE_ID).toULongLong();

	return SetGameAssetFromID(gameAssetID);
}

void GameObjectMeshComponentWidget::on_m_AddMeshAssetButton_clicked()
{
	if(!m_IsReady)
	{
		return;
	}

	GameObjectSelectAssetDialog selectAssetDialog(m_GameAssetManager, GameContentType::Mesh, this);

	int result = selectAssetDialog.exec();

	if(result != QDialog::Accepted)
	{
		return;
	}

	uint64_t gameAssetID = selectAssetDialog.GetGameAssetIDSelected();

	SetGameAssetFromID(gameAssetID);
}

XEResult GameObjectMeshComponentWidget::SetGameAssetFromID(uint64_t gameAssetID)
{
	if(!m_IsReady)
	{
		return XEResult::NotReady;
	}

	GameAsset* gameAsset = m_GameAssetManager->GetGameAsset(gameAssetID);

	if(gameAsset == nullptr)
	{
		return XEResult::NotFound;
	}

	if(gameAsset->GetGameContentType() != GameContentType::Mesh)
	{
		return XEResult::InvalidObjType;
	}

	MeshGOC* meshGOC = m_GameObject->GetMeshGOC();

	XEResult ret = meshGOC->SetMeshAsset(reinterpret_cast<MeshAsset*>(gameAsset));

	if(ret != XEResult::Ok)
	{
		return ret;
	}

	m_GameObjectMeshComponentWidgetQtUI.m_MeshAssetTextBox->setText(QString::fromStdWString(meshGOC->GetMeshName()));

	return XEResult::Ok;
}

void GameObjectMeshComponentWidget::on_m_ClearMeshAssetButton_clicked()
{
	if(!m_IsReady)
	{
		return;
	}

	RemoveGameAsset();
}

XEResult GameObjectMeshComponentWidget::RemoveGameAsset()
{
	if(!m_IsReady)
	{
		return XEResult::NotReady;
	}

	MeshGOC* meshGOC = m_GameObject->GetMeshGOC();

	XEResult ret = meshGOC->RemoveMeshAsset();

	if(ret != XEResult::Ok)
	{
		return ret;
	}

	m_GameObjectMeshComponentWidgetQtUI.m_MeshAssetTextBox->setText("");

	return XEResult::Ok;
}

void GameObjectMeshComponentWidget::dragEnterEvent(QDragEnterEvent* enterEvent)
{
	DragDropType type = XEQTHelpers::GetDragDropType(enterEvent);

	if(type == DragDropType::GameAsset)
	{
		enterEvent->accept();
		return;
	}

	enterEvent->ignore();
}

void GameObjectMeshComponentWidget::dragLeaveEvent(QDragLeaveEvent* leaveEvent)
{
	leaveEvent->accept();
}

void GameObjectMeshComponentWidget::dragMoveEvent(QDragMoveEvent* moveEvent)
{
	DragDropType type = XEQTHelpers::GetDragDropType(moveEvent);

	if(m_GameObjectMeshComponentWidgetQtUI.m_MeshAssetTextBox->rect().contains(m_GameObjectMeshComponentWidgetQtUI.m_MeshAssetTextBox->mapFromParent(moveEvent->pos()))) 
	{
		if(type == DragDropType::GameAsset)
		{
			moveEvent->accept();
			return;
		}
	}

	moveEvent->ignore();
}

void GameObjectMeshComponentWidget::dropEvent(QDropEvent* dropEvent)
{
	DragDropType type = XEQTHelpers::GetDragDropType(dropEvent);
	
	if(m_GameObjectMeshComponentWidgetQtUI.m_MeshAssetTextBox->rect().contains(m_GameObjectMeshComponentWidgetQtUI.m_MeshAssetTextBox->mapFromParent(dropEvent->pos()))) 
	{
		if(type == DragDropType::GameAsset)
		{
			if(DropAsset(dropEvent->source()) == XEResult::Ok)
			{
				dropEvent->accept();
				return;
			}
		}
	}

	dropEvent->ignore();
}