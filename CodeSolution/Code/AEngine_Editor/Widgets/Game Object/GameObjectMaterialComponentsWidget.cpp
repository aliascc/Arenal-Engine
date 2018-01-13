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
#include "qmessagebox.h"

/***************************
*   Game Engine Includes   *
****************************/
#include "Utils\AEQTDefs.h"
#include "GameApp\GameApp.h"
#include "Base\BaseFunctions.h"
#include "Shaders\HullShader.h"
#include "Shaders\PixelShader.h"
#include "Shaders\DomainShader.h"
#include "Shaders\VertexShader.h"
#include "GameAssets\GameAsset.h"
#include "GameObject\GameObject.h"
#include "Shaders\ComputeShader.h"
#include "Shaders\GeometryShader.h"
#include "GameAssets\GameAssetManager.h"
#include "GameAssets\Assets\ShaderAsset.h"
#include "GameObjectMaterialComponentsWidget.h"
#include "GameObject\Components\MeshMaterialGOC.h"
#include "Dialogs\Game Objects\GameObjectSelectAssetDialog.h"
#include "Dialogs\Game Objects\GameObjectShaderPropsDialog.h"

//Always include last
#include "Memory\MemLeaks.h"

/********************
*   Function Defs   *
*********************/
GameObjectMaterialComponentsWidget::GameObjectMaterialComponentsWidget(GameObject* gameObject, GameApp* gameApp, QWidget *parent)
	: QWidget(parent)
	, m_GameObject(gameObject)
	, m_GameApp(gameApp)
{
	m_GameObjectMaterialComponentsWidgetQtUI.setupUi(this);
	
	AEAssert(gameObject != nullptr);
	AEAssert(m_GameApp != nullptr);

	InitFields();

	PopulateMaterialsTree();
}

GameObjectMaterialComponentsWidget::~GameObjectMaterialComponentsWidget()
{
}

void GameObjectMaterialComponentsWidget::InitFields()
{
	////////////////////////////////
	//Check that Game Object is not null
	AEAssert(m_GameObject != nullptr);
	if(m_GameObject == nullptr)
	{
		return;
	}
	
	////////////////////////////////
	//Check that Game Asset Manager is not null
	AEAssert(m_GameApp != nullptr);
	if (m_GameApp == nullptr)
	{
		return;
	}

	////////////////////////////////
	//Widget is Ready to run
	m_IsReady = true;
}

void GameObjectMaterialComponentsWidget::PopulateMaterialsTree(uint32_t selectedIndex)
{
	if(!m_IsReady)
	{
		return;
	}

	////////////////////////////////
	//Clear List
	m_GameObjectMaterialComponentsWidgetQtUI.m_MaterialsTree->clear();

	////////////////////////////////
	//Get Mesh Material GOC List
	const MeshMaterialsGOCList& matList = m_GameObject->GetMeshMaterialsGOCList();

	////////////////////////////////
	//Add Materials to Tree
	for(auto meshMaterialGOC : matList)
	{
		////////////////////////////////
		//Create Branch and Add Material
		QTreeWidgetItem* materialBranch = new QTreeWidgetItem();
		QString name = QString::fromStdWString(meshMaterialGOC->GetName());
		materialBranch->setData(0, Qt::ItemDataRole::DisplayRole, name);
		materialBranch->setData(0, AE_QT_ITEM_DATA_ROLE_UNIQUE_ID, meshMaterialGOC->GetUniqueID());
		materialBranch->setFlags(materialBranch->flags() | Qt::ItemFlag::ItemIsEditable);

		////////////////////////////////
		//Add to Tree
		m_GameObjectMaterialComponentsWidgetQtUI.m_MaterialsTree->addTopLevelItem(materialBranch);
	}

	if(m_GameObjectMaterialComponentsWidgetQtUI.m_MaterialsTree->topLevelItemCount() != 0 && selectedIndex < (uint32_t)m_GameObjectMaterialComponentsWidgetQtUI.m_MaterialsTree->topLevelItemCount())
	{
		m_GameObjectMaterialComponentsWidgetQtUI.m_MaterialsTree->setCurrentItem(m_GameObjectMaterialComponentsWidgetQtUI.m_MaterialsTree->topLevelItem(selectedIndex));
	}
}

MeshMaterialGOC* GameObjectMaterialComponentsWidget::GetMeshMaterialGOC(QTreeWidgetItem* item)
{
	if(!m_IsReady)
	{
		return nullptr;
	}

	QTreeWidgetItem*  currentItem = nullptr;

	if(item != nullptr)
	{
		currentItem = item;
	}
	else
	{
		QList<QTreeWidgetItem*> selectedItems = m_GameObjectMaterialComponentsWidgetQtUI.m_MaterialsTree->selectedItems();

		if(selectedItems.count() == 0)
		{
			return nullptr;
		}

		currentItem = selectedItems[0];
	}

	uint64_t meshMatID = currentItem->data(0, AE_QT_ITEM_DATA_ROLE_UNIQUE_ID).toULongLong();

	MeshMaterialGOC* meshMatGOC = m_GameObject->GetMeshMaterialGOC(meshMatID);

	return meshMatGOC;
}

void GameObjectMaterialComponentsWidget::on_m_MaterialsTree_itemChanged(QTreeWidgetItem* item, int column)
{
	if(!m_IsReady)
	{
		return;
	}

	if(column != 0)
	{
		return;
	}

	MeshMaterialGOC* meshMatGOC = GetMeshMaterialGOC(item);

	AEAssert(meshMatGOC != nullptr);
	if(meshMatGOC == nullptr)
	{
		return;
	}

	std::wstring newName = item->data(0, Qt::ItemDataRole::DisplayRole).toString().toStdWString();

	if(newName.empty())
	{
		QString name = QString::fromStdWString(meshMatGOC->GetName());
		item->setData(0, Qt::ItemDataRole::DisplayRole, name);
		return;
	}

	if(newName.compare(meshMatGOC->GetName()) == 0)
	{
		return;
	}

	meshMatGOC->SetName(newName);
}

void GameObjectMaterialComponentsWidget::on_m_AddMaterialButton_clicked()
{
	if(!m_IsReady)
	{
		return;
	}

	////////////////////////////////
	//Create New Mesh Material GOC
	AETODO("Add literal from Localization");
	MeshMaterialGOC* meshMaterialGOC = new MeshMaterialGOC(m_GameObject, L"Empty Mesh Material");

	////////////////////////////////
	//Add it to Game Object
	AEResult ret = m_GameObject->AddMeshMaterialGOC(meshMaterialGOC);

	if(ret != AEResult::Ok)
	{
		AETODO("Add log and window message");

		DeleteMem(meshMaterialGOC);

		return;
	}

	////////////////////////////////
	//New Index will be the Top Level Count
	uint32_t selectedIndex = m_GameObjectMaterialComponentsWidgetQtUI.m_MaterialsTree->topLevelItemCount();

	////////////////////////////////
	//Repopulate tree with new data
	PopulateMaterialsTree(selectedIndex);
}

void GameObjectMaterialComponentsWidget::on_m_RemoveMaterialButton_clicked()
{
	if(!m_IsReady)
	{
		return;
	}

	QList<QTreeWidgetItem*> selectedItems = m_GameObjectMaterialComponentsWidgetQtUI.m_MaterialsTree->selectedItems();

	if(selectedItems.count() == 0)
	{
		return;
	}

	////////////////////////////////
	//Ask if User is sure to delete item
	AETODO("Add Localization string");		
	QMessageBox::StandardButton result = QMessageBox::information(this, "Remove Mesh Material GOC", "Are you sure you want to remove this Mesh Material GOC?", QMessageBox::Yes, QMessageBox::No);

	if(result != QMessageBox::Yes)
	{
		return;
	}

	////////////////////////////////
	//Get Mesh Material GOC ID
	QTreeWidgetItem* item = selectedItems[0];
	uint64_t meshMatID = item->data(0, AE_QT_ITEM_DATA_ROLE_UNIQUE_ID).toULongLong();

	////////////////////////////////
	//Tell Game Object to remove it
	AEResult ret = m_GameObject->RemoveMeshMaterialGOC(meshMatID);

	if(ret != AEResult::Ok)
	{
		AETODO("Add log and window message");

		return;
	}

	////////////////////////////////
	//Repopulate tree with new data
	PopulateMaterialsTree();
}

AEResult GameObjectMaterialComponentsWidget::DropAsset(QObject* object, ShaderType shaderType)
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
		return AEResult::ZeroSize;
	}

	QTreeWidgetItem* item = selectedItems[0];

	uint64_t gameAssetID = item->data(0, AE_QT_ITEM_DATA_ROLE_GAME_ASSET_UNIQUE_ID).toULongLong();

	return SetGameAssetFromID(shaderType, gameAssetID);
}

AEResult GameObjectMaterialComponentsWidget::SetGameAssetFromID(ShaderType shaderType, uint64_t gameAssetID)
{
	if(!m_IsReady)
	{
		return AEResult::NotReady;
	}

	GameAsset* gameAsset = m_GameApp->GetGameAssetManager()->GetGameAsset(gameAssetID);

	if(gameAsset == nullptr)
	{
		AETODO("log error");

		return AEResult::NotFound;
	}

	if(gameAsset->GetGameContentType() != GameContentType::Shader)
	{
		AETODO("log error and window message");

		return AEResult::InvalidObjType;
	}

	ShaderAsset* shaderAsset = reinterpret_cast<ShaderAsset*>(gameAsset);

	if(shaderAsset->GetShaderType() != shaderType)
	{
		AETODO("log error and window message");

		return AEResult::ShaderTypeMismatch;
	}

	return SetShaderGameAssetToCurrent(shaderAsset);
}

AEResult GameObjectMaterialComponentsWidget::SetShaderGameAssetToCurrent(ShaderAsset* shaderAsset)
{
	AEAssert(shaderAsset != nullptr);
	if(shaderAsset == nullptr)
	{
		return AEResult::NullParameter;
	}

	MeshMaterialGOC* meshMatGOC = GetMeshMaterialGOC();

	if(meshMatGOC == nullptr)
	{
		AETODO("Log error and window error");

		return AEResult::NotFound;
	}

	AEResult ret = AEResult::Ok;

	GameAppScopedLock lock = m_GameApp->GetGameAppScopedLock();
	lock.StartLock();

	switch (shaderAsset->GetShaderType())
	{
		case ShaderType::VertexShader:
			ret = meshMatGOC->SetVertexShaderAsset(shaderAsset);
			break;

		case ShaderType::PixelShader:
			ret = meshMatGOC->SetPixelShaderAsset(shaderAsset);
			break;

		case ShaderType::GeometryShader:
			ret = meshMatGOC->SetGeometryShaderAsset(shaderAsset);
			break;

		case ShaderType::ComputeShader:
			ret = meshMatGOC->SetComputeShaderAsset(shaderAsset);
			break;

		case ShaderType::DomainShader:
			ret = meshMatGOC->SetDomainShaderAsset(shaderAsset);
			break;

		case ShaderType::HullShader:
			ret = meshMatGOC->SetHullShaderAsset(shaderAsset);
			break;

		default:
			AEAssert(false);
			ret = AEResult::Fail;
			break;
	}

	if(ret != AEResult::Ok)
	{
		AETODO("Log error and window error");

		return ret;
	}

	lock.EndLock();

	RefreshShaderNames();

	return AEResult::Ok;
}

void GameObjectMaterialComponentsWidget::RefreshShaderNames()
{
	QString vsName = "";
	QString psName = "";
	QString gsName = "";
	QString csName = "";
	QString hsName = "";
	QString dsName = "";

	MeshMaterialGOC* meshMatGOC = GetMeshMaterialGOC();

	if(meshMatGOC != nullptr)
	{
		vsName = QString::fromStdWString(meshMatGOC->GetVertexShaderName());
		psName = QString::fromStdWString(meshMatGOC->GetPixelShaderName());
		gsName = QString::fromStdWString(meshMatGOC->GetGeometryShaderName());
		csName = QString::fromStdWString(meshMatGOC->GetComputeShaderName());
		hsName = QString::fromStdWString(meshMatGOC->GetHullShaderName());
		dsName = QString::fromStdWString(meshMatGOC->GetDomainShaderName());
	}

	m_GameObjectMaterialComponentsWidgetQtUI.m_VSAssetTextBox->setText(vsName);

	m_GameObjectMaterialComponentsWidgetQtUI.m_PSAssetTextBox->setText(psName);

	m_GameObjectMaterialComponentsWidgetQtUI.m_GSAssetTextBox->setText(gsName);

	m_GameObjectMaterialComponentsWidgetQtUI.m_CSAssetTextBox->setText(csName);

	m_GameObjectMaterialComponentsWidgetQtUI.m_HSAssetTextBox->setText(hsName);

	m_GameObjectMaterialComponentsWidgetQtUI.m_DSAssetTextBox->setText(dsName);
}

void GameObjectMaterialComponentsWidget::on_m_MaterialsTree_itemSelectionChanged()
{
	if(!m_IsReady)
	{
		return;
	}

	RefreshShaderNames();
}

void GameObjectMaterialComponentsWidget::on_m_VSAddAssetButton_clicked()
{
	if(!m_IsReady)
	{
		return;
	}

	uint64_t gameAssetID = SelectShaderAsset();

	if(gameAssetID == 0)
	{
		return;
	}

	SetGameAssetFromID(ShaderType::VertexShader, gameAssetID);
}

void GameObjectMaterialComponentsWidget::on_m_PSAddAssetButton_clicked()
{
	if(!m_IsReady)
	{
		return;
	}

	uint64_t gameAssetID = SelectShaderAsset();

	if(gameAssetID == 0)
	{
		return;
	}

	SetGameAssetFromID(ShaderType::PixelShader, gameAssetID);
}

void GameObjectMaterialComponentsWidget::on_m_GSAddAssetButton_clicked()
{
	if(!m_IsReady)
	{
		return;
	}

	uint64_t gameAssetID = SelectShaderAsset();

	if(gameAssetID == 0)
	{
		return;
	}

	SetGameAssetFromID(ShaderType::GeometryShader, gameAssetID);
}

void GameObjectMaterialComponentsWidget::on_m_CSAddAssetButton_clicked()
{
	if(!m_IsReady)
	{
		return;
	}

	uint64_t gameAssetID = SelectShaderAsset();

	if(gameAssetID == 0)
	{
		return;
	}

	SetGameAssetFromID(ShaderType::ComputeShader, gameAssetID);
}

void GameObjectMaterialComponentsWidget::on_m_DSAddAssetButton_clicked()
{
	if(!m_IsReady)
	{
		return;
	}

	uint64_t gameAssetID = SelectShaderAsset();

	if(gameAssetID == 0)
	{
		return;
	}

	SetGameAssetFromID(ShaderType::DomainShader, gameAssetID);
}

void GameObjectMaterialComponentsWidget::on_m_HSAddAssetButton_clicked()
{
	if(!m_IsReady)
	{
		return;
	}

	uint64_t gameAssetID = SelectShaderAsset();

	if(gameAssetID == 0)
	{
		return;
	}

	SetGameAssetFromID(ShaderType::HullShader, gameAssetID);
}

uint64_t GameObjectMaterialComponentsWidget::SelectShaderAsset()
{
	if(!m_IsReady)
	{
		return 0;
	}

	GameObjectSelectAssetDialog selectAssetDialog(m_GameApp->GetGameAssetManager(), GameContentType::Shader, this);

	int result = selectAssetDialog.exec();

	if(result != QDialog::Accepted)
	{
		return 0;
	}

	uint64_t gameAssetID = selectAssetDialog.GetGameAssetIDSelected();

	return gameAssetID;
}

void GameObjectMaterialComponentsWidget::on_m_VSClearAssetButton_clicked()
{
	if(!m_IsReady)
	{
		return;
	}

	MeshMaterialGOC* meshMat = GetMeshMaterialGOC();

	if(meshMat == nullptr)
	{
		return;
	}

	AEResult ret = meshMat->RemoveVertexShaderAsset();

	if(ret != AEResult::Ok)
	{
		AETODO("log error and window warning");

		return;
	}

	RefreshShaderNames();
}

void GameObjectMaterialComponentsWidget::on_m_PSClearAssetButton_clicked()
{
	if(!m_IsReady)
	{
		return;
	}

	MeshMaterialGOC* meshMat = GetMeshMaterialGOC();

	if(meshMat == nullptr)
	{
		return;
	}

	AEResult ret = meshMat->RemovePixelShaderAsset();

	if(ret != AEResult::Ok)
	{
		AETODO("log error and window warning");

		return;
	}

	RefreshShaderNames();
}

void GameObjectMaterialComponentsWidget::on_m_GSClearAssetButton_clicked()
{
	if(!m_IsReady)
	{
		return;
	}

	MeshMaterialGOC* meshMat = GetMeshMaterialGOC();

	if(meshMat == nullptr)
	{
		return;
	}

	AEResult ret = meshMat->RemoveGeometryShaderAsset();

	if(ret != AEResult::Ok)
	{
		AETODO("log error and window warning");

		return;
	}

	RefreshShaderNames();
}

void GameObjectMaterialComponentsWidget::on_m_CSClearAssetButton_clicked()
{
	if(!m_IsReady)
	{
		return;
	}

	MeshMaterialGOC* meshMat = GetMeshMaterialGOC();

	if(meshMat == nullptr)
	{
		return;
	}

	AEResult ret = meshMat->RemoveComputeShaderAsset();

	if(ret != AEResult::Ok)
	{
		AETODO("log error and window warning");

		return;
	}

	RefreshShaderNames();
}

void GameObjectMaterialComponentsWidget::on_m_HSClearAssetButton_clicked()
{
	if(!m_IsReady)
	{
		return;
	}

	MeshMaterialGOC* meshMat = GetMeshMaterialGOC();

	if(meshMat == nullptr)
	{
		return;
	}

	AEResult ret = meshMat->RemoveHullShaderAsset();

	if(ret != AEResult::Ok)
	{
		AETODO("log error and window warning");

		return;
	}

	RefreshShaderNames();
}

void GameObjectMaterialComponentsWidget::on_m_DSClearAssetButton_clicked()
{
	if(!m_IsReady)
	{
		return;
	}

	MeshMaterialGOC* meshMat = GetMeshMaterialGOC();

	if(meshMat == nullptr)
	{
		return;
	}

	AEResult ret = meshMat->RemoveDomainShaderAsset();

	if(ret != AEResult::Ok)
	{
		AETODO("log error and window warning");

		return;
	}

	RefreshShaderNames();
}

void GameObjectMaterialComponentsWidget::on_m_VSPropsButton_clicked()
{
	if(!m_IsReady)
	{
		return;
	}

	MeshMaterialGOC* meshMat = GetMeshMaterialGOC();

	if(meshMat == nullptr)
	{
		return;
	}

	VertexShader* vs = meshMat->GetVertexShaderResource();
	if(vs == nullptr)
	{
		return;
	}

	ShaderProperties* vsProps = meshMat->GetVertexShaderProperties();

	GameObjectShaderPropsDialog shaderPropsDiag(meshMat->GetName(), vs->GetName(), m_GameApp->GetGameAssetManager(), vsProps);

	shaderPropsDiag.exec();
}

void GameObjectMaterialComponentsWidget::on_m_PSPropsButton_clicked()
{
	if(!m_IsReady)
	{
		return;
	}

	MeshMaterialGOC* meshMat = GetMeshMaterialGOC();

	if(meshMat == nullptr)
	{
		return;
	}

	PixelShader* ps = meshMat->GetPixelShaderResource();
	if(ps == nullptr)
	{
		return;
	}

	ShaderProperties* psProps = meshMat->GetPixelShaderProperties();

	GameObjectShaderPropsDialog shaderPropsDiag(meshMat->GetName(), ps->GetName(), m_GameApp->GetGameAssetManager(), psProps);

	shaderPropsDiag.exec();
}

void GameObjectMaterialComponentsWidget::on_m_GSPropsButton_clicked()
{
	if(!m_IsReady)
	{
		return;
	}

	MeshMaterialGOC* meshMat = GetMeshMaterialGOC();

	if(meshMat == nullptr)
	{
		return;
	}
	AETODO("finish");
}

void GameObjectMaterialComponentsWidget::on_m_CSPropsButton_clicked()
{
	if(!m_IsReady)
	{
		return;
	}

	MeshMaterialGOC* meshMat = GetMeshMaterialGOC();

	if(meshMat == nullptr)
	{
		return;
	}
	AETODO("finish");
}

void GameObjectMaterialComponentsWidget::on_m_DSPropsButton_clicked()
{
	if(!m_IsReady)
	{
		return;
	}

	MeshMaterialGOC* meshMat = GetMeshMaterialGOC();

	if(meshMat == nullptr)
	{
		return;
	}
	AETODO("finish");
}

void GameObjectMaterialComponentsWidget::on_m_HSPropsButton_clicked()
{
	if(!m_IsReady)
	{
		return;
	}

	MeshMaterialGOC* meshMat = GetMeshMaterialGOC();

	if(meshMat == nullptr)
	{
		return;
	}
	AETODO("finish");
}

void GameObjectMaterialComponentsWidget::dragEnterEvent(QDragEnterEvent* enterEvent)
{
	DragDropType type = AEQTHelpers::GetDragDropType(enterEvent);

	if(type == DragDropType::GameAsset)
	{
		enterEvent->accept();
		return;
	}

	enterEvent->ignore();
}

void GameObjectMaterialComponentsWidget::dragLeaveEvent(QDragLeaveEvent* leaveEvent)
{
	leaveEvent->accept();
}

void GameObjectMaterialComponentsWidget::dragMoveEvent(QDragMoveEvent* moveEvent)
{
	DragDropType type = AEQTHelpers::GetDragDropType(moveEvent);

	if(	m_GameObjectMaterialComponentsWidgetQtUI.m_VSAssetTextBox->rect().contains(m_GameObjectMaterialComponentsWidgetQtUI.m_VSAssetTextBox->mapFromParent(moveEvent->pos())) ||
		m_GameObjectMaterialComponentsWidgetQtUI.m_PSAssetTextBox->rect().contains(m_GameObjectMaterialComponentsWidgetQtUI.m_PSAssetTextBox->mapFromParent(moveEvent->pos())) ||
		m_GameObjectMaterialComponentsWidgetQtUI.m_GSAssetTextBox->rect().contains(m_GameObjectMaterialComponentsWidgetQtUI.m_GSAssetTextBox->mapFromParent(moveEvent->pos())) ||
		m_GameObjectMaterialComponentsWidgetQtUI.m_CSAssetTextBox->rect().contains(m_GameObjectMaterialComponentsWidgetQtUI.m_CSAssetTextBox->mapFromParent(moveEvent->pos())) ||
		m_GameObjectMaterialComponentsWidgetQtUI.m_HSAssetTextBox->rect().contains(m_GameObjectMaterialComponentsWidgetQtUI.m_HSAssetTextBox->mapFromParent(moveEvent->pos())) ||
		m_GameObjectMaterialComponentsWidgetQtUI.m_DSAssetTextBox->rect().contains(m_GameObjectMaterialComponentsWidgetQtUI.m_DSAssetTextBox->mapFromParent(moveEvent->pos()))
		) 
	{
		if(type == DragDropType::GameAsset)
		{
			moveEvent->accept();
			return;
		}
	}

	moveEvent->ignore();
}

void GameObjectMaterialComponentsWidget::dropEvent(QDropEvent* dropEvent)
{
	DragDropType type = AEQTHelpers::GetDragDropType(dropEvent);

	bool isInsideTextBox = false;
	ShaderType shaderType = ShaderType::VertexShader;

	if(m_GameObjectMaterialComponentsWidgetQtUI.m_VSAssetTextBox->rect().contains(m_GameObjectMaterialComponentsWidgetQtUI.m_VSAssetTextBox->mapFromParent(dropEvent->pos())))
	{
		isInsideTextBox = true;
		shaderType = ShaderType::VertexShader;
	}
	else if(m_GameObjectMaterialComponentsWidgetQtUI.m_PSAssetTextBox->rect().contains(m_GameObjectMaterialComponentsWidgetQtUI.m_PSAssetTextBox->mapFromParent(dropEvent->pos())))
	{
		isInsideTextBox = true;
		shaderType = ShaderType::PixelShader;
	}
	else if(m_GameObjectMaterialComponentsWidgetQtUI.m_GSAssetTextBox->rect().contains(m_GameObjectMaterialComponentsWidgetQtUI.m_GSAssetTextBox->mapFromParent(dropEvent->pos())))
	{
		isInsideTextBox = true;
		shaderType = ShaderType::GeometryShader;
	}
	else if(m_GameObjectMaterialComponentsWidgetQtUI.m_CSAssetTextBox->rect().contains(m_GameObjectMaterialComponentsWidgetQtUI.m_CSAssetTextBox->mapFromParent(dropEvent->pos())))
	{
		isInsideTextBox = true;
		shaderType = ShaderType::ComputeShader;
	}
	else if(m_GameObjectMaterialComponentsWidgetQtUI.m_HSAssetTextBox->rect().contains(m_GameObjectMaterialComponentsWidgetQtUI.m_HSAssetTextBox->mapFromParent(dropEvent->pos())))
	{
		isInsideTextBox = true;
		shaderType = ShaderType::HullShader;
	}
	else if(m_GameObjectMaterialComponentsWidgetQtUI.m_DSAssetTextBox->rect().contains(m_GameObjectMaterialComponentsWidgetQtUI.m_DSAssetTextBox->mapFromParent(dropEvent->pos())))
	{
		isInsideTextBox = true;
		shaderType = ShaderType::DomainShader;
	}

	if(isInsideTextBox)
	{
		if(type == DragDropType::GameAsset)
		{
			if(DropAsset(dropEvent->source(), shaderType) == AEResult::Ok)
			{
				dropEvent->accept();
				return;
			}
		}
	}

	dropEvent->ignore();
}