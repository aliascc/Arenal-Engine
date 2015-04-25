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
#include "qdialog.h"

/***************************
*   Game Engine Includes   *
****************************/
#include "Base\BaseFunctions.h"
#include "GameAssets\GameAsset.h"
#include "SelectAssetTextBoxWidget.h"
#include "GameAssets\GameAssetManager.h"
#include "Dialogs\Game Objects\GameObjectSelectAssetDialog.h"

//Always include last
#include "Memory\MemLeaks.h"

/********************
*   Function Defs   *
*********************/
SelectAssetTextBoxWidget::SelectAssetTextBoxWidget(GameAssetManager* gameAssetManager, GameContentType gameContentType, QWidget *parent)
	: QWidget(parent)
	, m_GameAssetManager(gameAssetManager)
	, m_GameContentType(gameContentType)
	, m_SelectedID(0)
{
	m_SelectAssetTextBoxWidgetUI.setupUi(this);

	XEAssert(gameAssetManager != nullptr);
}

SelectAssetTextBoxWidget::~SelectAssetTextBoxWidget()
{
}

void SelectAssetTextBoxWidget::on_m_AddAsset_clicked()
{
	XEAssert(m_GameAssetManager != nullptr);
	if(m_GameAssetManager == nullptr)
	{
		return;
	}

	GameObjectSelectAssetDialog selectAssetDiag(m_GameAssetManager, m_GameContentType, this);

	int result = selectAssetDiag.exec();

	if(result != QDialog::Accepted)
	{
		return;
	}

	m_SelectedID = selectAssetDiag.GetGameAssetIDSelected();

	if(UpdateTextBox() != XEResult::Ok)
	{
		return;
	}

	emit SelectedIDChanged(m_SelectedID);
}

void SelectAssetTextBoxWidget::on_m_RemoveAsset_clicked()
{
	XEAssert(m_GameAssetManager != nullptr);
	if(m_GameAssetManager == nullptr)
	{
		return;
	}

	ClearSelectedID();

	emit SelectedIDChanged(m_SelectedID);
}

XEResult SelectAssetTextBoxWidget::UpdateTextBox()
{
	XEAssert(m_GameAssetManager != nullptr);
	if(m_GameAssetManager == nullptr)
	{
		return XEResult::GameAssetManagerNull;
	}

	if(m_SelectedID == 0)
	{
		m_SelectAssetTextBoxWidgetUI.m_AssetNameTextBox->setText("");
	}
	else
	{
		GameAsset* asset = m_GameAssetManager->GetGameAsset(m_SelectedID);

		XEAssert(asset != nullptr);
		if(asset == nullptr)
		{
			XETODO("log error");
			return XEResult::NotFound;
		}

		if(asset->GetGameContentType() != m_GameContentType)
		{
			return XEResult::InvalidObjType;
		}

		QString assetName = QString::fromStdWString(asset->GetName());

		if(assetName.isEmpty())
		{
			XETODO("localization");
			assetName = "<<Empty Name>>";
		}

		m_SelectAssetTextBoxWidgetUI.m_AssetNameTextBox->setText(assetName);
	}

	return XEResult::Ok;
}

void SelectAssetTextBoxWidget::ClearSelectedID()
{
	m_SelectedID = 0;

	UpdateTextBox();
}

void SelectAssetTextBoxWidget::SetTextureName(const QString& textureName)
{
	if(textureName.isEmpty())
	{
		XETODO("localization");
		m_SelectAssetTextBoxWidgetUI.m_AssetNameTextBox->setText("<<Empty Name>>");
	}
	else
	{
		m_SelectAssetTextBoxWidgetUI.m_AssetNameTextBox->setText(textureName);
	}
}