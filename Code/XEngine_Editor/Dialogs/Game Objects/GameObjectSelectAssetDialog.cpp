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
#include "qstandarditemmodel.h"

/***************************
*   Game Engine Includes   *
****************************/
#include "Utils\XEQTDefs.h"
#include "Base\BaseFunctions.h"
#include "GameAssets\GameAsset.h"
#include "GameAssets\GameAssetManager.h"
#include "GameObjectSelectAssetDialog.h"
#include "Proxy Filters\StringProxyFilterModel.h"

//Always include last
#include "Memory\MemLeaks.h"

/********************
*   Function Defs   *
*********************/
GameObjectSelectAssetDialog::GameObjectSelectAssetDialog(GameAssetManager* gameAssetManager, GameContentType gameContentType, QWidget *parent)
	: QDialog(parent)
	, m_StringProxyFilterModel(nullptr)
	, m_GameAssetManager(gameAssetManager)
	, m_IsReady(false)
{
	m_GameObjectSelectAssetDialogQtUI.setupUi(this);

	XEAssert(gameAssetManager != nullptr);

	InitFields(gameContentType);
}

GameObjectSelectAssetDialog::~GameObjectSelectAssetDialog()
{
}

void GameObjectSelectAssetDialog::InitFields(GameContentType gameContentType)
{
	///////////////////////////////////
	//Initialize Combo Box with Game Component Types
	XEQTHelpers::SetComboBoxGameContentType(m_GameObjectSelectAssetDialogQtUI.m_GameContentTypeComboBox);
	m_GameObjectSelectAssetDialogQtUI.m_GameContentTypeComboBox->setCurrentIndex(0);

	///////////////////////////////////
	//Initialize String Proxy Filter
	m_StringProxyFilterModel = new StringProxyFilterModel(this);
	
	///////////////////////////////////
	//Initialize Tree Model
	QStandardItemModel* standardModel = new QStandardItemModel(0, 1, this);
	standardModel->setHeaderData(0, Qt::Orientation::Horizontal, QString("Game Assets"), Qt::ItemDataRole::DisplayRole);
	
	///////////////////////////////////
	//Set Model to Proxy Filter and Proxy Filter to Tree
	m_StringProxyFilterModel->setSourceModel(standardModel);
	m_GameObjectSelectAssetDialogQtUI.m_GameAssetsSearchTree->setModel(m_StringProxyFilterModel);
	
	///////////////////////////////////
	//Set Combo Box and Populate Tree
	SetGameContentType(gameContentType);
	
	///////////////////////////////////
	//Set Ready to true
	m_IsReady = true;
}

void GameObjectSelectAssetDialog::PopulateGameAssetsTree(GameContentType gameContentType)
{
	XEAssert(m_GameAssetManager != nullptr);

	if(m_GameAssetManager == nullptr)
	{
		return;
	}

	QStandardItemModel* standardModel = reinterpret_cast<QStandardItemModel*>(m_StringProxyFilterModel->sourceModel());

	standardModel->removeRows(0, standardModel->rowCount());

	const GameAssetMap& gameAssetMap = m_GameAssetManager->GetGameAssetMap();

	for (auto gameAssetIt : gameAssetMap)
	{
		GameAsset* gameAsset = gameAssetIt.second;

		if(gameAsset->GetGameContentType() == gameContentType)
		{
			standardModel->insertRow(0);
			standardModel->setData(standardModel->index(0, 0), QString::fromStdWString(gameAsset->GetName()), Qt::ItemDataRole::DisplayRole);
			standardModel->setData(standardModel->index(0, 0), gameAsset->GetUniqueAssetID(), XE_QT_ITEM_DATA_ROLE_GAME_ASSET_UNIQUE_ID);
		}
	}
}

void GameObjectSelectAssetDialog::on_m_FilterAssetTextBox_textChanged(QString string)
{
	if(!m_IsReady)
	{
		return;
	}

	m_StringProxyFilterModel->SetStringFilter(string);
}

void GameObjectSelectAssetDialog::on_m_GameContentTypeComboBox_currentIndexChanged(int index)
{
	if(!m_IsReady)
	{
		return;
	}

	///////////////////////////////////
	//Get Game Content Type
	GameContentType gameContentType = static_cast<GameContentType>(m_GameObjectSelectAssetDialogQtUI.m_GameContentTypeComboBox->itemData(index).toUInt());

	///////////////////////////////////
	//Populate Tree
	PopulateGameAssetsTree(gameContentType);
}

void GameObjectSelectAssetDialog::SetGameContentType(GameContentType gameContentType)
{
	///////////////////////////////////
	//Set Game Content Type on Combo Box
	QComboBox* comboBox = m_GameObjectSelectAssetDialogQtUI.m_GameContentTypeComboBox;

	uint32_t size = (uint32_t)comboBox->count();
	for(uint32_t i = 0; i < size; i++)
	{
		GameContentType itemGameContentType = static_cast<GameContentType>(comboBox->itemData(i).toUInt());

		if(gameContentType == itemGameContentType)
		{
			comboBox->setCurrentIndex(i);

			break;
		}
	}

	///////////////////////////////////
	//Populate Tree
	PopulateGameAssetsTree(gameContentType);
}

uint64_t GameObjectSelectAssetDialog::GetGameAssetIDSelected()
{
	if(!m_IsReady)
	{
		return 0;
	}

	QModelIndex currentIndex = m_GameObjectSelectAssetDialogQtUI.m_GameAssetsSearchTree->currentIndex();

	uint64_t gameAssetID = currentIndex.data(XE_QT_ITEM_DATA_ROLE_GAME_ASSET_UNIQUE_ID).toULongLong();

	return gameAssetID;
}