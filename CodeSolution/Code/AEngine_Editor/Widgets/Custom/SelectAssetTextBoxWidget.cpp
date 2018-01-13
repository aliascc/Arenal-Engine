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

	AEAssert(gameAssetManager != nullptr);
}

SelectAssetTextBoxWidget::~SelectAssetTextBoxWidget()
{
}

void SelectAssetTextBoxWidget::on_m_AddAsset_clicked()
{
	AEAssert(m_GameAssetManager != nullptr);
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

	if(UpdateTextBox() != AEResult::Ok)
	{
		return;
	}

	emit SelectedIDChanged(m_SelectedID);
}

void SelectAssetTextBoxWidget::on_m_RemoveAsset_clicked()
{
	AEAssert(m_GameAssetManager != nullptr);
	if(m_GameAssetManager == nullptr)
	{
		return;
	}

	ClearSelectedID();

	emit SelectedIDChanged(m_SelectedID);
}

AEResult SelectAssetTextBoxWidget::UpdateTextBox()
{
	AEAssert(m_GameAssetManager != nullptr);
	if(m_GameAssetManager == nullptr)
	{
		return AEResult::GameAssetManagerNull;
	}

	if(m_SelectedID == 0)
	{
		m_SelectAssetTextBoxWidgetUI.m_AssetNameTextBox->setText("");
	}
	else
	{
		GameAsset* asset = m_GameAssetManager->GetGameAsset(m_SelectedID);

		AEAssert(asset != nullptr);
		if(asset == nullptr)
		{
			AETODO("log error");
			return AEResult::NotFound;
		}

		if(asset->GetGameContentType() != m_GameContentType)
		{
			return AEResult::InvalidObjType;
		}

		QString assetName = QString::fromStdWString(asset->GetName());

		if(assetName.isEmpty())
		{
			AETODO("localization");
			assetName = "<<Empty Name>>";
		}

		m_SelectAssetTextBoxWidgetUI.m_AssetNameTextBox->setText(assetName);
	}

	return AEResult::Ok;
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
		AETODO("localization");
		m_SelectAssetTextBoxWidgetUI.m_AssetNameTextBox->setText("<<Empty Name>>");
	}
	else
	{
		m_SelectAssetTextBoxWidgetUI.m_AssetNameTextBox->setText(textureName);
	}
}