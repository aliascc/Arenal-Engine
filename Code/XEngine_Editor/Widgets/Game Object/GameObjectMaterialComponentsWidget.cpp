/********************************************************
*
* Author: Carlos Chacón N.
*
* Created: 2/14/2012
*
* Desc:
*
*
* Copyright 2012
*
*********************************************************/

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
#include "Utils\XEQTDefs.h"
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
	
	XEAssert(gameObject != nullptr);
	XEAssert(m_GameApp != nullptr);

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
	XEAssert(m_GameObject != nullptr);
	if(m_GameObject == nullptr)
	{
		return;
	}
	
	////////////////////////////////
	//Check that Game Asset Manager is not null
	XEAssert(m_GameApp != nullptr);
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
		materialBranch->setData(0, XE_QT_ITEM_DATA_ROLE_UNIQUE_ID, meshMaterialGOC->GetUniqueID());
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

	uint64_t meshMatID = currentItem->data(0, XE_QT_ITEM_DATA_ROLE_UNIQUE_ID).toULongLong();

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

	XEAssert(meshMatGOC != nullptr);
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
	XETODO("Add literal from Localization");
	MeshMaterialGOC* meshMaterialGOC = new MeshMaterialGOC(m_GameObject, L"Empty Mesh Material");

	////////////////////////////////
	//Add it to Game Object
	XEResult ret = m_GameObject->AddMeshMaterialGOC(meshMaterialGOC);

	if(ret != XEResult::Ok)
	{
		XETODO("Add log and window message");

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
	XETODO("Add Localization string");		
	QMessageBox::StandardButton result = QMessageBox::information(this, "Remove Mesh Material GOC", "Are you sure you want to remove this Mesh Material GOC?", QMessageBox::Yes, QMessageBox::No);

	if(result != QMessageBox::Yes)
	{
		return;
	}

	////////////////////////////////
	//Get Mesh Material GOC ID
	QTreeWidgetItem* item = selectedItems[0];
	uint64_t meshMatID = item->data(0, XE_QT_ITEM_DATA_ROLE_UNIQUE_ID).toULongLong();

	////////////////////////////////
	//Tell Game Object to remove it
	XEResult ret = m_GameObject->RemoveMeshMaterialGOC(meshMatID);

	if(ret != XEResult::Ok)
	{
		XETODO("Add log and window message");

		return;
	}

	////////////////////////////////
	//Repopulate tree with new data
	PopulateMaterialsTree();
}

XEResult GameObjectMaterialComponentsWidget::DropAsset(QObject* object, ShaderType shaderType)
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
		return XEResult::ZeroSize;
	}

	QTreeWidgetItem* item = selectedItems[0];

	uint64_t gameAssetID = item->data(0, XE_QT_ITEM_DATA_ROLE_GAME_ASSET_UNIQUE_ID).toULongLong();

	return SetGameAssetFromID(shaderType, gameAssetID);
}

XEResult GameObjectMaterialComponentsWidget::SetGameAssetFromID(ShaderType shaderType, uint64_t gameAssetID)
{
	if(!m_IsReady)
	{
		return XEResult::NotReady;
	}

	GameAsset* gameAsset = m_GameApp->GetGameAssetManager()->GetGameAsset(gameAssetID);

	if(gameAsset == nullptr)
	{
		XETODO("log error");

		return XEResult::NotFound;
	}

	if(gameAsset->GetGameContentType() != GameContentType::Shader)
	{
		XETODO("log error and window message");

		return XEResult::InvalidObjType;
	}

	ShaderAsset* shaderAsset = reinterpret_cast<ShaderAsset*>(gameAsset);

	if(shaderAsset->GetShaderType() != shaderType)
	{
		XETODO("log error and window message");

		return XEResult::ShaderTypeMismatch;
	}

	return SetShaderGameAssetToCurrent(shaderAsset);
}

XEResult GameObjectMaterialComponentsWidget::SetShaderGameAssetToCurrent(ShaderAsset* shaderAsset)
{
	XEAssert(shaderAsset != nullptr);
	if(shaderAsset == nullptr)
	{
		return XEResult::NullParameter;
	}

	MeshMaterialGOC* meshMatGOC = GetMeshMaterialGOC();

	if(meshMatGOC == nullptr)
	{
		XETODO("Log error and window error");

		return XEResult::NotFound;
	}

	XEResult ret = XEResult::Ok;

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
			XEAssert(false);
			ret = XEResult::Fail;
			break;
	}

	if(ret != XEResult::Ok)
	{
		XETODO("Log error and window error");

		return ret;
	}

	lock.EndLock();

	RefreshShaderNames();

	return XEResult::Ok;
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

	XEResult ret = meshMat->RemoveVertexShaderAsset();

	if(ret != XEResult::Ok)
	{
		XETODO("log error and window warning");

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

	XEResult ret = meshMat->RemovePixelShaderAsset();

	if(ret != XEResult::Ok)
	{
		XETODO("log error and window warning");

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

	XEResult ret = meshMat->RemoveGeometryShaderAsset();

	if(ret != XEResult::Ok)
	{
		XETODO("log error and window warning");

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

	XEResult ret = meshMat->RemoveComputeShaderAsset();

	if(ret != XEResult::Ok)
	{
		XETODO("log error and window warning");

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

	XEResult ret = meshMat->RemoveHullShaderAsset();

	if(ret != XEResult::Ok)
	{
		XETODO("log error and window warning");

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

	XEResult ret = meshMat->RemoveDomainShaderAsset();

	if(ret != XEResult::Ok)
	{
		XETODO("log error and window warning");

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
	XETODO("finish");
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
	XETODO("finish");
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
	XETODO("finish");
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
	XETODO("finish");
}

void GameObjectMaterialComponentsWidget::dragEnterEvent(QDragEnterEvent* enterEvent)
{
	DragDropType type = XEQTHelpers::GetDragDropType(enterEvent);

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
	DragDropType type = XEQTHelpers::GetDragDropType(moveEvent);

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
	DragDropType type = XEQTHelpers::GetDragDropType(dropEvent);

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
			if(DropAsset(dropEvent->source(), shaderType) == XEResult::Ok)
			{
				dropEvent->accept();
				return;
			}
		}
	}

	dropEvent->ignore();
}