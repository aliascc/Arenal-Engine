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

/***************************
*   Game Engine Includes   *
****************************/
#include "Utils\AEQTDefs.h"
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
	AEQTUserTemplateData<AEQTObjType>* GameObjectsTreeType = new AEQTUserTemplateData<AEQTObjType>(AEQTObjType::GameObjectsTree);
	this->setUserData(AE_QT_USER_DATA_OBJ_TYPE_SLOT, GameObjectsTreeType);

	///////////////////////////////////////////
	//Set preferred indentation
	this->setIndentation(AE_QT_TREE_INDENTATION);

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

	AEAssert(m_EngineViewer != nullptr);
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

	uint64_t gameObjectID = static_cast<uint64_t>(item->data(0, AE_QT_ITEM_DATA_ROLE_UNIQUE_ID).toULongLong());

	GameObject* gameObject = m_EngineViewer->GetGameObjectManager()->GetGameObject(gameObjectID);

	if(gameObject == nullptr)
	{
		AETODO("add error");
		return;
	}

	////////////////////////////////////////////
	//Get Scope Lock and lock Game App Loop
	GameAppScopedLock appLock = m_EngineViewer->GetGameAppScopedLock();
	appLock.StartLock();

	AETODO("Check return");
	m_EngineViewer->GetGameObjectManager()->SetSelectedGameObject(gameObject);
	
	////////////////////////////////////////////
	//End Scope Lock before emitting msg so we do not create a dead lock
	appLock.EndLock();

	emit GameObjectChanged(gameObject);
}

AEResult GameObjectTreeWidget::RefreshGameObjectsTree()
{
	///////////////////////////////////////
	//No need to lock as it is only a 
	//reading operation
	///////////////////////////////////////

	if(!m_IsReady)
	{
		return AEResult::NotReady;
	}

	AEAssert(m_EngineViewer != nullptr);
	if (m_EngineViewer == nullptr)
	{
		AETODO("Add new error GameAppNull");
		return AEResult::NullObj;
	}

	this->clear();

	for (auto gameObjectIt : *m_EngineViewer->GetGameObjectManager())
	{
		AETODO("Check return");
		CreateGameObjectBranch(gameObjectIt.second);
	}

	return AEResult::Ok;
}

AEResult GameObjectTreeWidget::CreateGameObjectBranch(GameObject* gameObject, QTreeWidgetItem* parentBranch)
{
	////////////////////////////////////////////
	//No need to lock this function as it is internal
	//and calling function should lock it
	////////////////////////////////////////////

	if(!m_IsReady)
	{
		return AEResult::NotReady;
	}

	AEAssert(m_EngineViewer != nullptr);
	if (m_EngineViewer == nullptr)
	{
		AETODO("Add new error GameAppNull");
		return AEResult::NullObj;
	}

	AEAssert(gameObject != nullptr);
	if(gameObject == nullptr)
	{
		return AEResult::NullParameter;
	}

	////////////////////////////////////////
	//Create Game Object Branch
	QTreeWidgetItem* branch = new QTreeWidgetItem();
	branch->setData(0, Qt::ItemDataRole::DisplayRole, QString::fromStdWString(gameObject->GetName()));
	branch->setData(0, AE_QT_ITEM_DATA_ROLE_UNIQUE_ID, gameObject->GetUniqueID());
	
	////////////////////////////////////////
	//Create Game Object Child Branches
	for(auto gameObjectChildIt : *gameObject)
	{
		AETODO("Check return");
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

	return AEResult::Ok;
}

AEResult GameObjectTreeWidget::GameObjectNameChanged(uint64_t gameObjectID)
{
	////////////////////////////////////////////
	//No need to lock this function as it is internal
	//and only a name been reflected
	////////////////////////////////////////////

	if(!m_IsReady)
	{
		return AEResult::NotReady;
	}

	AEAssert(m_EngineViewer != nullptr);
	if (m_EngineViewer == nullptr)
	{
		AETODO("Add new error GameAppNull");
		return AEResult::NullObj;
	}

	GameObject* gameObject = m_EngineViewer->GetGameObjectManager()->GetGameObject(gameObjectID);
	if(gameObject == nullptr)
	{
		return AEResult::NotFound;
	}

	QTreeWidgetItem* item = FindBranch(gameObjectID);

	AEAssert(item != nullptr);
	if(item == nullptr)
	{
		return AEResult::NotFound;
	}

	QString name = QString::fromStdWString(gameObject->GetName());
	item->setData(0, Qt::ItemDataRole::DisplayRole, name);

	return AEResult::Ok;
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

	uint64_t id = static_cast<uint64_t>(parent->data(0, AE_QT_ITEM_DATA_ROLE_UNIQUE_ID).toULongLong());
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

	AEAssert(m_EngineViewer != nullptr);
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

	uint64_t gameObjectID = static_cast<uint64_t>(item->data(0, AE_QT_ITEM_DATA_ROLE_UNIQUE_ID).toULongLong());

	return gameObjectID;
}

void GameObjectTreeWidget::dropEvent(QDropEvent* dEvent)
{
	if(!m_IsReady)
	{
		dEvent->ignore();
		return;
	}

	AEAssert(m_EngineViewer != nullptr);
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
	uint64_t gameObjectID = static_cast<uint64_t>(childItem->data(0, AE_QT_ITEM_DATA_ROLE_UNIQUE_ID).toULongLong());

	GameObject* childGameObject = m_EngineViewer->GetGameObjectManager()->GetGameObject(gameObjectID);

	QTreeWidgetItem* parentItem = childItem->parent();

	if(parentItem != nullptr)
	{
		uint64_t parentGameObjectID = static_cast<uint64_t>(parentItem->data(0, AE_QT_ITEM_DATA_ROLE_UNIQUE_ID).toULongLong());

		GameObject* parentGameObject = m_EngineViewer->GetGameObjectManager()->GetGameObject(parentGameObjectID);

		AETODO("Check return");
		parentGameObject->AddChild(childGameObject);
	}
	else
	{
		AETODO("Check return");
		m_EngineViewer->GetGameObjectManager()->AddGameObject(childGameObject);
	}
}
