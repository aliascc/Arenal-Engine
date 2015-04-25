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
#include "GameContentDefs.h"
#include "Base\BaseFunctions.h"
#include "GameObjectTreeWidget.h"
#include "GameObject\GameObject.h"
#include "Engine Viewer\EngineViewer.h"
#include "GameObject\GameObjectManager.h"

//Always include last
#include "Memory\MemLeaks.h"

/********************
*   Function Defs   *
*********************/
GameObjectTreeWidget::GameObjectTreeWidget(QWidget* parent)
	: QTreeWidget(parent)
{
}

GameObjectTreeWidget::~GameObjectTreeWidget()
{
}

void GameObjectTreeWidget::InitFields()
{
	////////////////////////////////////////
	//Game Objects Tree Type
	XEQTUserTemplateData<XEQTObjType>* GameObjectsTreeType = new XEQTUserTemplateData<XEQTObjType>(XEQTObjType::GameObjectsTree);
	this->setUserData(XE_QT_USER_DATA_OBJ_TYPE_SLOT, GameObjectsTreeType);

	///////////////////////////////////////////
	//Set preferred indentation
	this->setIndentation(XE_QT_TREE_INDENTATION);

	///////////////////////////////////////////
	//Set Drag Mode
	this->setDragDropMode(DragDropMode::InternalMove);

	///////////////////////////////////////////
	//Set Animation
	this->setAnimated(true);

	///////////////////////////////////////////
	//Set Alternating Row Colors
	this->setAlternatingRowColors(false);

	///////////////////////////////////////////
	//Set number of columns and their names
	this->setColumnCount(1);

	QAbstractItemModel* itemModel = this->model();

	itemModel->setHeaderData(0, Qt::Orientation::Horizontal, "Game Objects", Qt::ItemDataRole::DisplayRole);

	///////////////////////////////////////////
	//Connect to Signals
	connect(this, SIGNAL(itemSelectionChanged()), this, SLOT(GameObjectSelectionChanged()));

	///////////////////////////////////////////
	//Finish
	m_IsReady = true;
}

void GameObjectTreeWidget::GameObjectSelectionChanged()
{
	///////////////////////////////////////
	//No need to lock as it is only a 
	//reading operation
	///////////////////////////////////////

	if(!m_IsReady)
	{
		return;
	}

	XEAssert(m_EngineViewer != nullptr);
	if (m_EngineViewer == nullptr)
	{
		return;
	}

	QList<QTreeWidgetItem*> selectedItems = this->selectedItems();
	if(selectedItems.count() == 0)
	{
		return;
	}

	QTreeWidgetItem* item = selectedItems[0];

	uint64_t gameObjectID = static_cast<uint64_t>(item->data(0, XE_QT_ITEM_DATA_ROLE_UNIQUE_ID).toULongLong());

	GameObject* gameObject = m_EngineViewer->GetGameObjectManager()->GetGameObject(gameObjectID);

	if(gameObject == nullptr)
	{
		XETODO("add error");
		return;
	}

	////////////////////////////////////////////
	//Get Scope Lock and lock Game App Loop
	GameAppScopedLock appLock = m_EngineViewer->GetGameAppScopedLock();
	appLock.StartLock();

	XETODO("Check return");
	m_EngineViewer->GetGameObjectManager()->SetSelectedGameObject(gameObject);
	
	////////////////////////////////////////////
	//End Scope Lock before emitting msg so we do not create a dead lock
	appLock.EndLock();

	emit GameObjectChanged(gameObject);
}

XEResult GameObjectTreeWidget::RefreshGameObjectsTree()
{
	///////////////////////////////////////
	//No need to lock as it is only a 
	//reading operation
	///////////////////////////////////////

	if(!m_IsReady)
	{
		return XEResult::NotReady;
	}

	XEAssert(m_EngineViewer != nullptr);
	if (m_EngineViewer == nullptr)
	{
		XETODO("Add new error GameAppNull");
		return XEResult::NullObj;
	}

	this->clear();

	for (auto gameObjectIt : *m_EngineViewer->GetGameObjectManager())
	{
		XETODO("Check return");
		CreateGameObjectBranch(gameObjectIt.second);
	}

	return XEResult::Ok;
}

XEResult GameObjectTreeWidget::CreateGameObjectBranch(GameObject* gameObject, QTreeWidgetItem* parentBranch)
{
	////////////////////////////////////////////
	//No need to lock this function as it is internal
	//and calling function should lock it
	////////////////////////////////////////////

	if(!m_IsReady)
	{
		return XEResult::NotReady;
	}

	XEAssert(m_EngineViewer != nullptr);
	if (m_EngineViewer == nullptr)
	{
		XETODO("Add new error GameAppNull");
		return XEResult::NullObj;
	}

	XEAssert(gameObject != nullptr);
	if(gameObject == nullptr)
	{
		return XEResult::NullParameter;
	}

	////////////////////////////////////////
	//Create Game Object Branch
	QTreeWidgetItem* branch = new QTreeWidgetItem();
	branch->setData(0, Qt::ItemDataRole::DisplayRole, QString::fromStdWString(gameObject->GetName()));
	branch->setData(0, XE_QT_ITEM_DATA_ROLE_UNIQUE_ID, gameObject->GetUniqueID());
	
	////////////////////////////////////////
	//Create Game Object Child Branches
	for(auto gameObjectChildIt : *gameObject)
	{
		XETODO("Check return");
		CreateGameObjectBranch(gameObjectChildIt.second, branch);
	}

	if(parentBranch != nullptr)
	{
		parentBranch->addChild(branch);
	}
	else
	{
		this->addTopLevelItem(branch);
	}

	return XEResult::Ok;
}

XEResult GameObjectTreeWidget::GameObjectNameChanged(uint64_t gameObjectID)
{
	////////////////////////////////////////////
	//No need to lock this function as it is internal
	//and only a name been reflected
	////////////////////////////////////////////

	if(!m_IsReady)
	{
		return XEResult::NotReady;
	}

	XEAssert(m_EngineViewer != nullptr);
	if (m_EngineViewer == nullptr)
	{
		XETODO("Add new error GameAppNull");
		return XEResult::NullObj;
	}

	GameObject* gameObject = m_EngineViewer->GetGameObjectManager()->GetGameObject(gameObjectID);
	if(gameObject == nullptr)
	{
		return XEResult::NotFound;
	}

	QTreeWidgetItem* item = FindBranch(gameObjectID);

	XEAssert(item != nullptr);
	if(item == nullptr)
	{
		return XEResult::NotFound;
	}

	QString name = QString::fromStdWString(gameObject->GetName());
	item->setData(0, Qt::ItemDataRole::DisplayRole, name);

	return XEResult::Ok;
}

QTreeWidgetItem* GameObjectTreeWidget::FindBranch(uint64_t gameObjectID)
{
	int32_t countNum = this->topLevelItemCount();

	for(int32_t i = 0; i < countNum; i++)
	{
		QTreeWidgetItem* child = this->topLevelItem(i);

		QTreeWidgetItem* found = FindBranchChild(gameObjectID, child);

		if(found != nullptr)
		{
			return found;
		}
	}

	return nullptr;
}

QTreeWidgetItem* GameObjectTreeWidget::FindBranchChild(uint64_t gameObjectID, QTreeWidgetItem* parent)
{
	if(parent == nullptr)
	{
		return nullptr;
	}

	uint64_t id = static_cast<uint64_t>(parent->data(0, XE_QT_ITEM_DATA_ROLE_UNIQUE_ID).toULongLong());
	if(gameObjectID == id)
	{
		return parent;
	}

	int32_t childCount = parent->childCount();
	for(int32_t i = 0; i < childCount; i++)
	{
		QTreeWidgetItem* child = parent->child(i);

		QTreeWidgetItem* found = FindBranchChild(gameObjectID, child);

		if(found != nullptr)
		{
			return found;
		}
	}

	return nullptr;
}

uint64_t GameObjectTreeWidget::GetCurrentGameObjectID()
{
	if(!m_IsReady)
	{
		return 0;
	}

	XEAssert(m_EngineViewer != nullptr);
	if (m_EngineViewer == nullptr)
	{
		return 0;
	}

	QList<QTreeWidgetItem*> selectedItems = this->selectedItems();

	if(selectedItems.count() == 0)
	{
		return 0;
	}

	QTreeWidgetItem* item = selectedItems[0];

	uint64_t gameObjectID = static_cast<uint64_t>(item->data(0, XE_QT_ITEM_DATA_ROLE_UNIQUE_ID).toULongLong());

	return gameObjectID;
}

void GameObjectTreeWidget::dropEvent(QDropEvent* dEvent)
{
	if(!m_IsReady)
	{
		dEvent->ignore();
		return;
	}

	XEAssert(m_EngineViewer != nullptr);
	if (m_EngineViewer == nullptr)
	{
		dEvent->ignore();
		return;
	}

	////////////////////////////////
	//Get Current Index
	QList<QTreeWidgetItem*> selectedItems = this->selectedItems();

	if(selectedItems.count() == 0)
	{
		dEvent->ignore();
		return;
	}

	////////////////////////////////
	//Accept Drops continue with internal move
	QTreeWidget::dropEvent(dEvent);

	////////////////////////////////////////////
	//Get Scope Lock and lock Game App Loop
	GameAppScopedLock appLock = m_EngineViewer->GetGameAppScopedLock();
	appLock.StartLock();

	QTreeWidgetItem* childItem = selectedItems[0];
	uint64_t gameObjectID = static_cast<uint64_t>(childItem->data(0, XE_QT_ITEM_DATA_ROLE_UNIQUE_ID).toULongLong());

	GameObject* childGameObject = m_EngineViewer->GetGameObjectManager()->GetGameObject(gameObjectID);

	QTreeWidgetItem* parentItem = childItem->parent();

	if(parentItem != nullptr)
	{
		uint64_t parentGameObjectID = static_cast<uint64_t>(parentItem->data(0, XE_QT_ITEM_DATA_ROLE_UNIQUE_ID).toULongLong());

		GameObject* parentGameObject = m_EngineViewer->GetGameObjectManager()->GetGameObject(parentGameObjectID);

		XETODO("Check return");
		parentGameObject->AddChild(childGameObject);
	}
	else
	{
		XETODO("Check return");
		m_EngineViewer->GetGameObjectManager()->AddGameObject(childGameObject);
	}
}
