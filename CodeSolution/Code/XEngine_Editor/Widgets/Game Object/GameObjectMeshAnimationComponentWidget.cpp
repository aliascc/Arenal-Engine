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
#include "qmessagebox.h"

/***************************
*   Game Engine Includes   *
****************************/
#include "Utils\XEQTDefs.h"
#include "GameApp\GameApp.h"
#include "Base\BaseFunctions.h"
#include "GameAssets\GameAsset.h"
#include "GameObject\GameObject.h"
#include "Models\Animation\Animation.h"
#include "GameAssets\GameAssetManager.h"
#include "GameAssets\Assets\SkeletonAsset.h"
#include "GameAssets\Assets\AnimationAsset.h"
#include "Models\Animation\AnimationPlayer.h"
#include "GameObject\Components\MeshAnimationGOC.h"
#include "GameObjectMeshAnimationComponentWidget.h"
#include "Dialogs\Game Objects\GameObjectSelectAssetDialog.h"

//Always include last
#include "Memory\MemLeaks.h"

/********************
*   Function Defs   *
*********************/
GameObjectMeshAnimationComponentWidget::GameObjectMeshAnimationComponentWidget(GameObject* gameObject, GameApp* gameApp, QWidget *parent)
	: QWidget(parent)
	, m_GameObject(gameObject)
	, m_GameApp(gameApp)
{
	m_GameObjectMeshComponentWidgetQtUI.setupUi(this);

	XEAssert(m_GameApp != nullptr);
	XEAssert(m_GameObject != nullptr);

	if (m_GameApp != nullptr && m_GameObject != nullptr)
	{
		m_HasInitObjs = true;
	}

	InitFields();
}

GameObjectMeshAnimationComponentWidget::~GameObjectMeshAnimationComponentWidget()
{
}

void GameObjectMeshAnimationComponentWidget::InitFields()
{
	if (!m_HasInitObjs)
	{
		return;
	}

	XEAssert(m_GameObject->HasMeshAnimationGOC());
	if (!m_GameObject->HasMeshAnimationGOC())
	{
		return;
	}

	////////////////////////////////////////////////////////////
	//Get Mesh Animation GOC
	MeshAnimationGOC* meshAnimGOC = m_GameObject->GetMeshAnimationGOC();

	////////////////////////////////////////////////////////////
	//Set Skeleton Name
	m_GameObjectMeshComponentWidgetQtUI.m_SkeletonAssetTextBox->setText(QString::fromStdWString(meshAnimGOC->GetSkeletonName()));

	/////////////////////////////////////////////////////////////
	//Fill Animation Data
	RefreshAnimationAssetTree();

	/////////////////////////////////////////////////////////////
	//Set Play Animation Options
	m_GameObjectMeshComponentWidgetQtUI.m_BlendTimeSB->setValue(meshAnimGOC->GetBlendTime());
	m_GameObjectMeshComponentWidgetQtUI.m_BlendAnimCB->setChecked(meshAnimGOC->GetBlendAnimation());
	m_GameObjectMeshComponentWidgetQtUI.m_LoopAnim->setChecked(meshAnimGOC->GetOnLoop());

	m_IsReady = true;
}

XEResult GameObjectMeshAnimationComponentWidget::DropAsset(QObject* object)
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

	return AddGameAssetFromID(gameAssetID);
}

XEResult GameObjectMeshAnimationComponentWidget::AddGameAssetFromID(uint64_t gameAssetID)
{
	if (!m_IsReady)
	{
		return XEResult::NotReady;
	}

	GameAsset* gameAsset = m_GameApp->GetGameAssetManager()->GetGameAsset(gameAssetID);

	if (gameAsset == nullptr)
	{
		return XEResult::NotFound;
	}

	switch (gameAsset->GetGameContentType())
	{
		case GameContentType::Animation:
			return AddAnimationAsset(dynamic_cast<AnimationAsset*>(gameAsset));

		case GameContentType::Skeleton:
			return AddSkeletonAsset(dynamic_cast<SkeletonAsset*>(gameAsset));

		default:
			return XEResult::InvalidObjType;
	}

	return XEResult::Ok;
}

XEResult GameObjectMeshAnimationComponentWidget::AddAnimationAsset(AnimationAsset* animAsset)
{
	////////////////////////////////////////////////////////////
	//Pre-Checks
	if (!m_IsReady)
	{
		return XEResult::NotReady;
	}

	XEAssert(animAsset != nullptr);
	if (animAsset == nullptr)
	{
		return XEResult::NullParameter;
	}

	////////////////////////////////////////////////////////////
	//Get Mesh Animation GOC
	MeshAnimationGOC* meshAnimGOC = m_GameObject->GetMeshAnimationGOC();

	////////////////////////////////////////////////////////////
	//Set Animation Asset
	XEResult ret = meshAnimGOC->AddAnimationAsset(animAsset);
	if (ret != XEResult::Ok)
	{
		return ret;
	}

	////////////////////////////////////////////////////////////
	//Set Refresh Animation Asset Tree
	RefreshAnimationAssetTree();

	return XEResult::Ok;
}

XEResult GameObjectMeshAnimationComponentWidget::RemoveAnimationAsset(uint64_t assetID)
{
	////////////////////////////////////////////////////////////
	//Pre-Checks
	if (!m_IsReady)
	{
		return XEResult::NotReady;
	}

	////////////////////////////////
	//Get Animation Asset
	GameAsset* gameAsset = m_GameApp->GetGameAssetManager()->GetGameAsset(assetID);
	if (gameAsset == nullptr)
	{
		return XEResult::NotFound;
	}

	////////////////////////////////////////////////////////////
	//Get Mesh Animation GOC
	MeshAnimationGOC* meshAnimGOC = m_GameObject->GetMeshAnimationGOC();

	XEResult ret = meshAnimGOC->RemoveAnimationAsset(gameAsset->GetUniqueAssetID());
	if (ret != XEResult::Ok)
	{
		XETODO("Add log and window message");
		return XEResult::Fail;
	}

	////////////////////////////////
	//Repopulate tree with new data
	RefreshAnimationAssetTree();

	return XEResult::Ok;
}

void GameObjectMeshAnimationComponentWidget::on_m_AddAnimationlButton_clicked()
{
	////////////////////////////////////////////////////////////
	//Pre-Checks
	if (!m_IsReady)
	{
		return;
	}

	////////////////////////////////////////////////////////////
	//Set Select Asset Dialog
	GameObjectSelectAssetDialog selectAssetDialog(m_GameApp->GetGameAssetManager(), GameContentType::Animation, this);

	int result = selectAssetDialog.exec();

	if (result != QDialog::Accepted)
	{
		return;
	}

	////////////////////////////////////////////////////////////
	//Get Selected Asset ID
	uint64_t gameAssetID = selectAssetDialog.GetGameAssetIDSelected();

	////////////////////////////////////////////////////////////
	//Add Asset
	AddGameAssetFromID(gameAssetID);
}

void GameObjectMeshAnimationComponentWidget::on_m_RemoveAnimationButton_clicked()
{
	if (!m_IsReady)
	{
		return;
	}

	QList<QTreeWidgetItem*> selectedItems = m_GameObjectMeshComponentWidgetQtUI.m_AnimationsTree->selectedItems();

	if (selectedItems.count() == 0)
	{
		return;
	}

	////////////////////////////////
	//Ask if User is sure to delete item
	XETODO("Add Localization string");
	QMessageBox::StandardButton result = QMessageBox::information(this, "Remove Animation", "Are you sure you want to remove this Animation?", QMessageBox::Yes, QMessageBox::No);

	if (result != QMessageBox::Yes)
	{
		return;
	}

	////////////////////////////////
	//Get Animation Asset ID
	QTreeWidgetItem* item = selectedItems[0];
	uint64_t meshAnimID = item->data(0, XE_QT_ITEM_DATA_ROLE_UNIQUE_ID).toULongLong();

	////////////////////////////////
	//Remove Animation Asset
	RemoveAnimationAsset(meshAnimID);
}

void GameObjectMeshAnimationComponentWidget::RefreshAnimationAssetTree()
{
	////////////////////////////////
	//Pre-checks
	if (!m_HasInitObjs)
	{
		return;
	}

	////////////////////////////////
	//Clear List
	m_GameObjectMeshComponentWidgetQtUI.m_AnimationsTree->clear();

	////////////////////////////////////////////////////////////
	//Get Mesh Animation GOC
	MeshAnimationGOC* meshAnimGOC = m_GameObject->GetMeshAnimationGOC();
	
	////////////////////////////////
	//Get Animation Asset Map
	const AnimationAssetPairMap& animAssetMap = meshAnimGOC->GetAnimationAssetMap();

	////////////////////////////////
	//Add Animations to Tree
	for (auto animAssetPairIt : animAssetMap)
	{
		////////////////////////////////
		//Create Branch and Add Animation
		QTreeWidgetItem* animationBranch = new QTreeWidgetItem();
		QString name = QString::fromStdWString(animAssetPairIt.second.m_GameAsset->GetName());
		animationBranch->setData(0, Qt::ItemDataRole::DisplayRole, name);
		animationBranch->setData(0, XE_QT_ITEM_DATA_ROLE_UNIQUE_ID, animAssetPairIt.second.m_GameAsset->GetUniqueAssetID());
		animationBranch->setFlags(animationBranch->flags() | Qt::ItemFlag::ItemIsEditable);

		////////////////////////////////
		//Add to Tree
		m_GameObjectMeshComponentWidgetQtUI.m_AnimationsTree->addTopLevelItem(animationBranch);
	}
}

XEResult GameObjectMeshAnimationComponentWidget::AddSkeletonAsset(SkeletonAsset* skeletonAsset)
{
	////////////////////////////////////////////////////////////
	//Pre-Checks
	if (!m_IsReady)
	{
		return XEResult::NotReady;
	}

	XEAssert(skeletonAsset != nullptr);
	if (skeletonAsset == nullptr)
	{
		return XEResult::NullParameter;
	}

	////////////////////////////////////////////////////////////
	//Get Mesh Animation GOC
	MeshAnimationGOC* meshAnimGOC = m_GameObject->GetMeshAnimationGOC();

	////////////////////////////////////////////////////////////
	//Set Skeleton Asset;
	XEResult ret = meshAnimGOC->SetSkeletonAsset(skeletonAsset);
	if (ret != XEResult::Ok)
	{
		return ret;
	}

	////////////////////////////////////////////////////////////
	//Set Skeleton Name
	m_GameObjectMeshComponentWidgetQtUI.m_SkeletonAssetTextBox->setText(QString::fromStdWString(skeletonAsset->GetName()));

	return XEResult::Ok;
}

XEResult GameObjectMeshAnimationComponentWidget::RemoveSkeletonAsset()
{
	////////////////////////////////////////////////////////////
	//Pre-Checks
	if (!m_IsReady)
	{
		return XEResult::NotReady;
	}

	////////////////////////////////////////////////////////////
	//Get Mesh Animation GOC
	MeshAnimationGOC* meshAnimGOC = m_GameObject->GetMeshAnimationGOC();

	////////////////////////////////////////////////////////////
	//Remove Skeleton
	XEResult ret = meshAnimGOC->RemoveSkeletonAsset();
	if (ret != XEResult::Ok)
	{
		return ret;
	}

	////////////////////////////////////////////////////////////
	//Clear name
	m_GameObjectMeshComponentWidgetQtUI.m_SkeletonAssetTextBox->setText("");

	////////////////////////////////////////////////////////////
	//Finish
	return XEResult::Ok;
}

void GameObjectMeshAnimationComponentWidget::on_m_AddSkeletonAssetButton_clicked()
{
	////////////////////////////////////////////////////////////
	//Pre-Checks
	if(!m_IsReady)
	{
		return;
	}

	////////////////////////////////////////////////////////////
	//Set Select Asset Dialog
	GameObjectSelectAssetDialog selectAssetDialog(m_GameApp->GetGameAssetManager(), GameContentType::Skeleton, this);

	int result = selectAssetDialog.exec();

	if(result != QDialog::Accepted)
	{
		return;
	}

	////////////////////////////////////////////////////////////
	//Get Selected Asset ID
	uint64_t gameAssetID = selectAssetDialog.GetGameAssetIDSelected();

	////////////////////////////////////////////////////////////
	//Add Asset
	AddGameAssetFromID(gameAssetID);
}

void GameObjectMeshAnimationComponentWidget::on_m_ClearSkeletonAssetButton_clicked()
{
	////////////////////////////////////////////////////////////
	//Pre-Checks
	if(!m_IsReady)
	{
		return;
	}

	////////////////////////////////////////////////////////////
	//Remove Asset
	RemoveSkeletonAsset();
}

void GameObjectMeshAnimationComponentWidget::on_m_PlayTestAnimationButton_clicked()
{
	////////////////////////////////
	//Pre-checks
	if (!m_IsReady)
	{
		return;
	}

	QList<QTreeWidgetItem*> selectedItems = m_GameObjectMeshComponentWidgetQtUI.m_AnimationsTree->selectedItems();

	if (selectedItems.count() == 0)
	{
		return;
	}

	////////////////////////////////
	//Get Animation Asset ID
	QTreeWidgetItem* item = selectedItems[0];
	uint64_t meshAnimID = item->data(0, XE_QT_ITEM_DATA_ROLE_UNIQUE_ID).toULongLong();

	////////////////////////////////
	//Get Animation Asset
	GameAsset* gameAsset = m_GameApp->GetGameAssetManager()->GetGameAsset(meshAnimID);
	if (gameAsset == nullptr)
	{
		return;
	}

	////////////////////////////////////////////////////////////
	//Get Mesh Animation GOC
	MeshAnimationGOC* meshAnimGOC = m_GameObject->GetMeshAnimationGOC();

	////////////////////////////////////////////////////////////
	//Get Mesh Animation GOC
	meshAnimGOC->PlayTestAnimation(gameAsset->GetUniqueAssetID());
}

void GameObjectMeshAnimationComponentWidget::on_m_LoopAnim_stateChanged(int newState)
{
	////////////////////////////////
	//Pre-checks
	if (!m_IsReady)
	{
		return;
	}

	////////////////////////////////////////////////////////////
	//Get Mesh Animation GOC
	MeshAnimationGOC* meshAnimGOC = m_GameObject->GetMeshAnimationGOC();

	////////////////////////////////////////////////////////////
	//Set value
	meshAnimGOC->SetOnLoop(m_GameObjectMeshComponentWidgetQtUI.m_LoopAnim->isChecked());
}

void GameObjectMeshAnimationComponentWidget::on_m_BlendAnimCB_stateChanged(int newState)
{
	////////////////////////////////
	//Pre-checks
	if (!m_IsReady)
	{
		return;
	}

	////////////////////////////////////////////////////////////
	//Get Mesh Animation GOC
	MeshAnimationGOC* meshAnimGOC = m_GameObject->GetMeshAnimationGOC();

	////////////////////////////////////////////////////////////
	//Set value
	meshAnimGOC->SetBlendAnimation(m_GameObjectMeshComponentWidgetQtUI.m_BlendAnimCB->isChecked());
}

void GameObjectMeshAnimationComponentWidget::on_m_BlendTimeSB_editingFinished()
{
	////////////////////////////////
	//Pre-checks
	if (!m_IsReady)
	{
		return;
	}

	////////////////////////////////////////////////////////////
	//Get Blend Time Value
	float blendTime = static_cast<float>(m_GameObjectMeshComponentWidgetQtUI.m_BlendTimeSB->value());

	////////////////////////////////////////////////////////////
	//Get Mesh Animation GOC
	MeshAnimationGOC* meshAnimGOC = m_GameObject->GetMeshAnimationGOC();

	////////////////////////////////////////////////////////////
	//Set value
	meshAnimGOC->SetBlendTime(blendTime);
}

///////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////

void GameObjectMeshAnimationComponentWidget::dragEnterEvent(QDragEnterEvent* enterEvent)
{
	DragDropType type = XEQTHelpers::GetDragDropType(enterEvent);

	if(type == DragDropType::GameAsset)
	{
		enterEvent->accept();
		return;
	}

	enterEvent->ignore();
}

void GameObjectMeshAnimationComponentWidget::dragLeaveEvent(QDragLeaveEvent* leaveEvent)
{
	leaveEvent->accept();
}

void GameObjectMeshAnimationComponentWidget::dragMoveEvent(QDragMoveEvent* moveEvent)
{
	DragDropType type = XEQTHelpers::GetDragDropType(moveEvent);

	if (m_GameObjectMeshComponentWidgetQtUI.m_SkeletonAssetTextBox->rect().contains(m_GameObjectMeshComponentWidgetQtUI.m_SkeletonAssetTextBox->mapFromParent(moveEvent->pos())))
	{
		if(type == DragDropType::GameAsset)
		{
			moveEvent->accept();
			return;
		}
	}

	if (m_GameObjectMeshComponentWidgetQtUI.m_AnimationsTree->rect().contains(m_GameObjectMeshComponentWidgetQtUI.m_AnimationsTree->mapFromParent(moveEvent->pos())))
	{
		if (type == DragDropType::GameAsset)
		{
			moveEvent->accept();
			return;
		}
	}

	moveEvent->ignore();
}

void GameObjectMeshAnimationComponentWidget::dropEvent(QDropEvent* dropEvent)
{
	DragDropType type = XEQTHelpers::GetDragDropType(dropEvent);

	if (m_GameObjectMeshComponentWidgetQtUI.m_SkeletonAssetTextBox->rect().contains(m_GameObjectMeshComponentWidgetQtUI.m_SkeletonAssetTextBox->mapFromParent(dropEvent->pos())))
	{
		if(type == DragDropType::GameAsset)
		{
			if (DropAsset(dropEvent->source()) == XEResult::Ok)
			{
				dropEvent->accept();
				return;
			}
		}
	}

	if (m_GameObjectMeshComponentWidgetQtUI.m_AnimationsTree->rect().contains(m_GameObjectMeshComponentWidgetQtUI.m_AnimationsTree->mapFromParent(dropEvent->pos())))
	{
		if (type == DragDropType::GameAsset)
		{
			if (DropAsset(dropEvent->source()) == XEResult::Ok)
			{
				dropEvent->accept();
				return;
			}
		}
	}

	dropEvent->ignore();
}