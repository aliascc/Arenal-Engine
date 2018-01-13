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
#include "qtreewidget.h"

/***************************
*   Game Engine Includes   *
****************************/
#include "Utils\AEQTDefs.h"
#include "Base\BaseFunctions.h"
#include "GameAssets\GameAsset.h"
#include "GameObject\GameObject.h"
#include "Engine Viewer\EngineViewer.h"
#include "GameAssets\GameAssetManager.h"
#include "AngelScript\AngelScriptManager.h"
#include "GameObjectScriptComponentWidget.h"
#include "GameAssets\Assets\GameObjectScriptAsset.h"
#include "GameObject\Components\GameObjectScriptGOC.h"
#include "Dialogs\Game Objects\GameObjectSelectAssetDialog.h"

//Always include last
#include "Memory\MemLeaks.h"

/********************
*   Function Defs   *
*********************/
GameObjectScriptComponentWidget::GameObjectScriptComponentWidget(GameObject* gameObject, EngineViewer* engineViewer, QWidget *parent)
	: QWidget(parent)
	, m_GameObject(gameObject)
	, m_EngineViewer(engineViewer)
{
	m_GameObjectScriptComponentWidgetQtUI.setupUi(this);

	AEAssert(m_GameObject != nullptr);
	AEAssert(m_EngineViewer != nullptr);

	InitFields();

	PopulateGOSTree();
}

GameObjectScriptComponentWidget::~GameObjectScriptComponentWidget()
{
}

void GameObjectScriptComponentWidget::InitFields()
{
	AEAssert(m_EngineViewer != nullptr);
	if (m_EngineViewer == nullptr)
	{
		return;
	}

	AEAssert(m_GameObject != nullptr);
	if(m_GameObject == nullptr)
	{
		return;
	}

	AEAssert(m_GameObject->HasGameObjectScriptGOCs());
	if (!m_GameObject->HasGameObjectScriptGOCs())
	{
		return;
	}

	AETODO("Check return");
	m_GameObjectScriptComponentWidgetQtUI.m_GameObjectScriptProperties->Initialize();

	////////////////////////////////
	//Widget is Ready to run
	m_IsReady = true;
}

GameObjectScriptGOC* GameObjectScriptComponentWidget::GetGameObjectScriptGOC(QTreeWidgetItem* item)
{
	if (!m_IsReady)
	{
		return nullptr;
	}

	QTreeWidgetItem*  currentItem = nullptr;

	if (item != nullptr)
	{
		currentItem = item;
	}
	else
	{
		QList<QTreeWidgetItem*> selectedItems = m_GameObjectScriptComponentWidgetQtUI.m_GameObjectScriptTree->selectedItems();

		if (selectedItems.count() == 0)
		{
			return nullptr;
		}

		currentItem = selectedItems[0];
	}

	uint64_t gosID = currentItem->data(0, AE_QT_ITEM_DATA_ROLE_UNIQUE_ID).toULongLong();

	GameObjectScriptGOC* gameObjectScriptGOC = m_GameObject->GetGameObjectScriptGOC(gosID);

	return gameObjectScriptGOC;
}

void GameObjectScriptComponentWidget::PopulateGOSTree(uint32_t selectedIndex)
{
	if (!m_IsReady)
	{
		return;
	}

	////////////////////////////////
	//Clear List
	m_GameObjectScriptComponentWidgetQtUI.m_GameObjectScriptTree->clear();

	////////////////////////////////
	//Get Game Object Scripts GOC List
	const GameObjectScriptGOCList& gosList = m_GameObject->GetGameObjectScriptGOCList();

	////////////////////////////////
	//Add Game Object Scripts GOC to Tree
	for (auto gameObjectScriptGOC : gosList)
	{
		////////////////////////////////
		//Create Branch and Add Game Object Script
		QTreeWidgetItem* gosBranch = new QTreeWidgetItem();
		QString name = QString::fromStdWString(gameObjectScriptGOC->GetName());
		gosBranch->setData(0, Qt::ItemDataRole::DisplayRole, name);
		gosBranch->setData(0, AE_QT_ITEM_DATA_ROLE_UNIQUE_ID, gameObjectScriptGOC->GetUniqueID());
		gosBranch->setFlags(gosBranch->flags() | Qt::ItemFlag::ItemIsEditable);

		////////////////////////////////
		//Add to Tree
		m_GameObjectScriptComponentWidgetQtUI.m_GameObjectScriptTree->addTopLevelItem(gosBranch);
	}

	if (m_GameObjectScriptComponentWidgetQtUI.m_GameObjectScriptTree->topLevelItemCount() != 0 && selectedIndex < (uint32_t)m_GameObjectScriptComponentWidgetQtUI.m_GameObjectScriptTree->topLevelItemCount())
	{
		m_GameObjectScriptComponentWidgetQtUI.m_GameObjectScriptTree->setCurrentItem(m_GameObjectScriptComponentWidgetQtUI.m_GameObjectScriptTree->topLevelItem(selectedIndex));
	}
}

void GameObjectScriptComponentWidget::on_m_GameObjectScriptTree_itemChanged(QTreeWidgetItem* item, int column)
{
	if (!m_IsReady)
	{
		return;
	}

	if (column != 0)
	{
		return;
	}

	GameObjectScriptGOC* gameObjectScriptGOC = GetGameObjectScriptGOC(item);

	AEAssert(gameObjectScriptGOC != nullptr);
	if (gameObjectScriptGOC == nullptr)
	{
		return;
	}

	std::wstring newName = item->data(0, Qt::ItemDataRole::DisplayRole).toString().toStdWString();
	if (newName.empty())
	{
		QString name = QString::fromStdWString(gameObjectScriptGOC->GetName());
		item->setData(0, Qt::ItemDataRole::DisplayRole, name);
		return;
	}

	if (newName.compare(gameObjectScriptGOC->GetName()) == 0)
	{
		return;
	}

	gameObjectScriptGOC->SetName(newName);
}

void GameObjectScriptComponentWidget::on_m_GameObjectScriptTree_itemSelectionChanged()
{
	if (!m_IsReady)
	{
		return;
	}

	GameObjectScriptGOC* gameObjectScriptGOC = GetGameObjectScriptGOC();
	if (gameObjectScriptGOC == nullptr)
	{
		return;
	}

	m_GameObjectScriptComponentWidgetQtUI.m_InstanceNameTextBox->setText(QString::fromStdWString(gameObjectScriptGOC->GetGameObjectScriptInstanceName()));
	m_GameObjectScriptComponentWidgetQtUI.m_ScriptAssetTextBox->setText(QString::fromStdWString(gameObjectScriptGOC->GetGameObjectScriptName()));

	if (gameObjectScriptGOC->HasScriptInstance())
	{
		m_GameObjectScriptComponentWidgetQtUI.m_InstanceNameTextBox->setReadOnly(true);
		m_GameObjectScriptComponentWidgetQtUI.m_CreateRemoveInstance->setText("Remove");

		/////////////////////////////////////
		//Init Parameters for Property Tree
		AETODO("Check return");
		m_GameObjectScriptComponentWidgetQtUI.m_GameObjectScriptProperties->Populate(gameObjectScriptGOC->GetGameObjectScriptProperties());
	}
	else
	{
		m_GameObjectScriptComponentWidgetQtUI.m_GameObjectScriptProperties->clear();

		m_GameObjectScriptComponentWidgetQtUI.m_InstanceNameTextBox->setReadOnly(false);
		m_GameObjectScriptComponentWidgetQtUI.m_CreateRemoveInstance->setText("Create");
	}
}

AEResult GameObjectScriptComponentWidget::DropAsset(QObject* object)
{
	if(!m_IsReady)
	{
		return AEResult::NotReady;
	}

	AEAssert(object != nullptr);
	if(object == nullptr)
	{
		return AEResult::NullParameter;
	}

	QTreeWidget* gameAssetTree = reinterpret_cast<QTreeWidget*>(object);

	QList<QTreeWidgetItem*> selectedItems = gameAssetTree->selectedItems();

	if(selectedItems.count() == 0)
	{
		return AEResult::Ok;
	}

	QTreeWidgetItem* item = selectedItems[0];

	uint64_t gameAssetID = item->data(0, AE_QT_ITEM_DATA_ROLE_GAME_ASSET_UNIQUE_ID).toULongLong();

	return SetGameAssetFromID(gameAssetID);
}

void GameObjectScriptComponentWidget::on_m_SearchScriptAsset_clicked()
{
	if (!m_IsReady)
	{
		return;
	}

	GameObjectSelectAssetDialog selectAssetDialog(m_EngineViewer->GetGameAssetManager(), GameContentType::GameObjectScript, this);

	int result = selectAssetDialog.exec();
	if (result != QDialog::Accepted)
	{
		return;
	}

	uint64_t gameAssetID = selectAssetDialog.GetGameAssetIDSelected();

	SetGameAssetFromID(gameAssetID);
}

void GameObjectScriptComponentWidget::on_m_AddGOScript_clicked()
{
	if (!m_IsReady)
	{
		return;
	}

	GameObjectScriptGOC* gosGOC = new GameObjectScriptGOC(m_GameObject, L"Empty Script", m_EngineViewer->GetAngelScriptManager(), m_EngineViewer->GetGameObjectScriptManager());

	if (m_GameObject->AddGameObjectScriptGOC(gosGOC) != AEResult::Ok)
	{
		AETODO("Add message exit");
		return;
	}

	uint32_t pos = (uint32_t)m_GameObject->GetGameObjectScriptGOCList().size() - 1;

	PopulateGOSTree(pos);
}

AEResult GameObjectScriptComponentWidget::SetGameAssetFromID(uint64_t gameAssetID)
{
	if(!m_IsReady)
	{
		return AEResult::NotReady;
	}

	GameAsset* gameAsset = m_EngineViewer->GetGameAssetManager()->GetGameAsset(gameAssetID);
	if(gameAsset == nullptr)
	{
		return AEResult::NotFound;
	}

	if(gameAsset->GetGameContentType() != GameContentType::GameObjectScript)
	{
		return AEResult::InvalidObjType;
	}

	GameObjectScriptGOC* gameObjectScriptGOC = GetGameObjectScriptGOC();
	if (gameObjectScriptGOC == nullptr)
	{
		return AEResult::NullObj;
	}

	AEResult ret = gameObjectScriptGOC->SetGameObjectScriptAsset(reinterpret_cast<GameObjectScriptAsset*>(gameAsset));
	if(ret != AEResult::Ok)
	{
		return ret;
	}

	m_GameObjectScriptComponentWidgetQtUI.m_ScriptAssetTextBox->setText(QString::fromStdWString(gameObjectScriptGOC->GetGameObjectScriptName()));

	return AEResult::Ok;
}

void GameObjectScriptComponentWidget::on_m_RemoveGOScript_clicked()
{
	if(!m_IsReady)
	{
		return;
	}

	RemoveGameAsset();
}

AEResult GameObjectScriptComponentWidget::RemoveGameAsset()
{
	if(!m_IsReady)
	{
		return AEResult::NotReady;
	}

	AETODO("Implement");
	AEAssert(false);

	return AEResult::Ok;
}

void GameObjectScriptComponentWidget::on_m_CreateRemoveInstance_clicked()
{
	if (!m_IsReady)
	{
		return;
	}

	GameObjectScriptGOC* gameObjectScriptGOC = GetGameObjectScriptGOC();
	if (gameObjectScriptGOC == nullptr)
	{
		return;
	}

	if (gameObjectScriptGOC->HasScriptInstance())
	{
		AEResult ret = gameObjectScriptGOC->RemoveGameObjectScriptInstance();
		if (ret != AEResult::Ok)
		{
			AETODO("Add error");
			return;
		}

		m_GameObjectScriptComponentWidgetQtUI.m_InstanceNameTextBox->setText("");
		m_GameObjectScriptComponentWidgetQtUI.m_InstanceNameTextBox->setReadOnly(false);
		m_GameObjectScriptComponentWidgetQtUI.m_CreateRemoveInstance->setText("Create");

		m_GameObjectScriptComponentWidgetQtUI.m_GameObjectScriptProperties->clear();
	}
	else
	{
		std::wstring instanceName = m_GameObjectScriptComponentWidgetQtUI.m_InstanceNameTextBox->text().toStdWString();
		AEResult ret = gameObjectScriptGOC->CreateGameObjectScriptInstance(instanceName);
		if (ret != AEResult::Ok)
		{
			AETODO("Add error");
			return;
		}

		m_GameObjectScriptComponentWidgetQtUI.m_InstanceNameTextBox->setReadOnly(true);
		m_GameObjectScriptComponentWidgetQtUI.m_CreateRemoveInstance->setText("Remove");

		/////////////////////////////////////
		//Init Parameters for Property Tree
		AETODO("Check return");
		m_GameObjectScriptComponentWidgetQtUI.m_GameObjectScriptProperties->Populate(gameObjectScriptGOC->GetGameObjectScriptProperties());
	}
}

void GameObjectScriptComponentWidget::dragEnterEvent(QDragEnterEvent* enterEvent)
{
	DragDropType type = AEQTHelpers::GetDragDropType(enterEvent);

	if(type == DragDropType::GameAsset)
	{
		enterEvent->accept();
		return;
	}

	enterEvent->ignore();
}

void GameObjectScriptComponentWidget::dragLeaveEvent(QDragLeaveEvent* leaveEvent)
{
	leaveEvent->accept();
}

void GameObjectScriptComponentWidget::dragMoveEvent(QDragMoveEvent* moveEvent)
{
	DragDropType type = AEQTHelpers::GetDragDropType(moveEvent);

	if (m_GameObjectScriptComponentWidgetQtUI.m_ScriptAssetTextBox->rect().contains(m_GameObjectScriptComponentWidgetQtUI.m_ScriptAssetTextBox->mapFromParent(moveEvent->pos())))
	{
		if(type == DragDropType::GameAsset)
		{
			moveEvent->accept();
			return;
		}
	}

	moveEvent->ignore();
}

void GameObjectScriptComponentWidget::dropEvent(QDropEvent* dropEvent)
{
	DragDropType type = AEQTHelpers::GetDragDropType(dropEvent);

	if (m_GameObjectScriptComponentWidgetQtUI.m_ScriptAssetTextBox->rect().contains(m_GameObjectScriptComponentWidgetQtUI.m_ScriptAssetTextBox->mapFromParent(dropEvent->pos())))
	{
		if(type == DragDropType::GameAsset)
		{
			if(DropAsset(dropEvent->source()) == AEResult::Ok)
			{
				dropEvent->accept();
				return;
			}
		}
	}

	dropEvent->ignore();
}