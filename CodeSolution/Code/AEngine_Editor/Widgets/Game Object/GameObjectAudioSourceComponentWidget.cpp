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
#include "AudioPlayer.h"
#include "Utils\AEQTDefs.h"
#include "Base\BaseFunctions.h"
#include "GameAssets\GameAsset.h"
#include "GameObject\GameObject.h"
#include "Engine Viewer\EngineViewer.h"
#include "GameAssets\GameAssetManager.h"
#include "AngelScript\AngelScriptManager.h"
#include "GameObject\Components\AudioSourceGOC.h"
#include "GameObjectAudioSourceComponentWidget.h"
#include "GameAssets\Assets\GameObjectScriptAsset.h"
#include "Dialogs\Game Objects\GameObjectSelectAssetDialog.h"

//Always include last
#include "Memory\MemLeaks.h"

/********************
*   Function Defs   *
*********************/

GameObjectAudioSourceComponentWidget::GameObjectAudioSourceComponentWidget(GameObject* gameObject, EngineViewer* engineViewer, QWidget *parent)
	: QWidget(parent)
	, m_GameObject(gameObject)
	, m_EngineViewer(engineViewer)
{
	m_GameObjectAudioSourceComponentWidgetQtUI.setupUi(this);

	AEAssert(m_GameObject != nullptr);
	AEAssert(m_EngineViewer != nullptr);

	InitFields();

	PopulateAudioSourceTree();
}

GameObjectAudioSourceComponentWidget::~GameObjectAudioSourceComponentWidget()
{
}

void GameObjectAudioSourceComponentWidget::InitFields()
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

	AEAssert(m_GameObject->HasAudioSourceGOCs());
	if (!m_GameObject->HasAudioSourceGOCs())
	{
		return;
	}

	////////////////////////////////
	//Widget is Ready to run
	m_IsReady = true;
}

AudioSourceGOC* GameObjectAudioSourceComponentWidget::GetAudioSourceGOC(QTreeWidgetItem* item)
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
		QList<QTreeWidgetItem*> selectedItems = m_GameObjectAudioSourceComponentWidgetQtUI.m_AudioSourceTree->selectedItems();

		if (selectedItems.count() == 0)
		{
			return nullptr;
		}

		currentItem = selectedItems[0];
	}

	uint64_t gosID = currentItem->data(0, AE_QT_ITEM_DATA_ROLE_UNIQUE_ID).toULongLong();

	AudioSourceGOC* audioSourceGOC = m_GameObject->GetAudioSourceGOC(gosID);

	return audioSourceGOC;
}

void GameObjectAudioSourceComponentWidget::PopulateAudioSourceTree(uint32_t selectedIndex)
{
	if (!m_IsReady)
	{
		return;
	}

	////////////////////////////////
	//Clear List
	m_GameObjectAudioSourceComponentWidgetQtUI.m_AudioSourceTree->clear();

	////////////////////////////////
	//Get Audio Source GOC List
	const AudioSourceGOCList& audioSourceGOCList = m_GameObject->GetAudioSourceGOCList();

	////////////////////////////////
	//Add Audio Source GOC to Tree
	for (auto audioSourceGOC : audioSourceGOCList)
	{
		////////////////////////////////
		//Create Branch and Add Audio Source
		QTreeWidgetItem* gosBranch = new QTreeWidgetItem();
		QString name = QString::fromStdWString(audioSourceGOC->GetName());
		gosBranch->setData(0, Qt::ItemDataRole::DisplayRole, name);
		gosBranch->setData(0, AE_QT_ITEM_DATA_ROLE_UNIQUE_ID, audioSourceGOC->GetUniqueID());
		gosBranch->setFlags(gosBranch->flags() | Qt::ItemFlag::ItemIsEditable);

		////////////////////////////////
		//Add to Tree
		m_GameObjectAudioSourceComponentWidgetQtUI.m_AudioSourceTree->addTopLevelItem(gosBranch);
	}

	if (m_GameObjectAudioSourceComponentWidgetQtUI.m_AudioSourceTree->topLevelItemCount() != 0 && selectedIndex < (uint32_t)m_GameObjectAudioSourceComponentWidgetQtUI.m_AudioSourceTree->topLevelItemCount())
	{
		m_GameObjectAudioSourceComponentWidgetQtUI.m_AudioSourceTree->setCurrentItem(m_GameObjectAudioSourceComponentWidgetQtUI.m_AudioSourceTree->topLevelItem(selectedIndex));
	}
}

void GameObjectAudioSourceComponentWidget::on_m_AudioSourceTree_itemChanged(QTreeWidgetItem* item, int column)
{
	if (!m_IsReady)
	{
		return;
	}

	if (column != 0)
	{
		return;
	}

	AudioSourceGOC* audioSourceGOC = GetAudioSourceGOC(item);

	AEAssert(audioSourceGOC != nullptr);
	if (audioSourceGOC == nullptr)
	{
		return;
	}

	QString newNameQS = item->data(0, Qt::ItemDataRole::DisplayRole).toString();
	std::wstring newName = newNameQS.toStdWString();
	if (newName.empty())
	{
		QString name = QString::fromStdWString(audioSourceGOC->GetName());
		item->setData(0, Qt::ItemDataRole::DisplayRole, name);
		return;
	}

	if (newName.compare(audioSourceGOC->GetName()) == 0)
	{
		return;
	}

	audioSourceGOC->SetName(newName);

	m_GameObjectAudioSourceComponentWidgetQtUI.m_NameAudioSourceTextBox->setText(newNameQS);
}

void GameObjectAudioSourceComponentWidget::on_m_AudioSourceTree_itemSelectionChanged()
{
	if (!m_IsReady)
	{
		return;
	}

	AudioSourceGOC* audioSourceGOC = GetAudioSourceGOC();
	if (audioSourceGOC == nullptr)
	{
		return;
	}

	m_GameObjectAudioSourceComponentWidgetQtUI.m_NameAudioSourceTextBox->setText(QString::fromStdWString(audioSourceGOC->GetName()));
	m_GameObjectAudioSourceComponentWidgetQtUI.m_AudioAssetTextBox->setText(QString::fromStdWString(audioSourceGOC->GetAudioName()));

	AudioPlayer* player = audioSourceGOC->GetAudioPlayer();

	m_GameObjectAudioSourceComponentWidgetQtUI.m_VolumeSB->setValue((double)player->GetVolume());
	m_GameObjectAudioSourceComponentWidgetQtUI.m_MinimumDistanceSB->setValue((double)player->GetMinDistance());
	m_GameObjectAudioSourceComponentWidgetQtUI.m_PitchSB->setValue((double)player->GetPitch());
	m_GameObjectAudioSourceComponentWidgetQtUI.m_AttenuationSB->setValue((double)player->GetAttenuation());

	m_GameObjectAudioSourceComponentWidgetQtUI.m_Is3DCheckBox->setChecked(player->Is3D());
	m_GameObjectAudioSourceComponentWidgetQtUI.m_LoopCheckBox->setChecked(player->IsLooped());

	QString channelCount(std::to_string(player->GetChannelCount()).c_str());
	m_GameObjectAudioSourceComponentWidgetQtUI.m_ChannelCountLabelText->setText(channelCount);

	QString sampleRate(std::to_string(player->GetSampleRate()).c_str());
	m_GameObjectAudioSourceComponentWidgetQtUI.m_SampleRateLabelText->setText(sampleRate);

	QString duration(std::to_string(player->GetDuration()).c_str());
	m_GameObjectAudioSourceComponentWidgetQtUI.m_DurationLabelText->setText(duration);
}

AEResult GameObjectAudioSourceComponentWidget::DropAsset(QObject* object)
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

void GameObjectAudioSourceComponentWidget::on_m_SearchAudioAsset_clicked()
{
	if (!m_IsReady)
	{
		return;
	}

	GameObjectSelectAssetDialog selectAssetDialog(m_EngineViewer->GetGameAssetManager(), GameContentType::Audio, this);

	int result = selectAssetDialog.exec();
	if (result != QDialog::Accepted)
	{
		return;
	}

	uint64_t gameAssetID = selectAssetDialog.GetGameAssetIDSelected();

	SetGameAssetFromID(gameAssetID);
}

void GameObjectAudioSourceComponentWidget::on_m_AddAudioSource_clicked()
{
	if (!m_IsReady)
	{
		return;
	}

	AudioSourceGOC* audioSourceGOC = new AudioSourceGOC(m_GameObject, L"Empty Audio Source");
	if (m_GameObject->AddAudioSourceGOC(audioSourceGOC) != AEResult::Ok)
	{
		AETODO("Add message exit");
		return;
	}

	uint32_t pos = (uint32_t)m_GameObject->GetAudioSourceGOCList().size() - 1;

	PopulateAudioSourceTree(pos);
}

AEResult GameObjectAudioSourceComponentWidget::SetGameAssetFromID(uint64_t gameAssetID)
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

	if(gameAsset->GetGameContentType() != GameContentType::Audio)
	{
		return AEResult::InvalidObjType;
	}

	AudioSourceGOC* audioSourceGOC = GetAudioSourceGOC();
	if (audioSourceGOC == nullptr)
	{
		return AEResult::NullObj;
	}

	AEResult ret = audioSourceGOC->SetAudioAsset(reinterpret_cast<AudioAsset*>(gameAsset));
	if(ret != AEResult::Ok)
	{
		return ret;
	}

	m_GameObjectAudioSourceComponentWidgetQtUI.m_AudioAssetTextBox->setText(QString::fromStdWString(audioSourceGOC->GetAudioName()));

	return AEResult::Ok;
}

void GameObjectAudioSourceComponentWidget::on_m_RemoveAudioSource_clicked()
{
	if(!m_IsReady)
	{
		return;
	}

	RemoveGameAsset();
}

AEResult GameObjectAudioSourceComponentWidget::RemoveGameAsset()
{
	if(!m_IsReady)
	{
		return AEResult::NotReady;
	}

	AETODO("Implement");
	AEAssert(false);

	return AEResult::Ok;
}

void GameObjectAudioSourceComponentWidget::on_m_IsSound_toggled(bool toggled)
{
	if (!m_IsReady)
	{
		return;
	}

	AudioSourceGOC* audioSourceGOC = GetAudioSourceGOC();
	if (audioSourceGOC == nullptr)
	{
		return;
	}

	AETODO("Check return and add log");
	audioSourceGOC->GetAudioPlayer()->SetAsSoundOrMusicProcessing(toggled);
}

void GameObjectAudioSourceComponentWidget::on_m_VolumeSB_valueChanged(double value)
{
	if (!m_IsReady)
	{
		return;
	}

	AudioSourceGOC* audioSourceGOC = GetAudioSourceGOC();
	if (audioSourceGOC == nullptr)
	{
		return;
	}

	float fValue = (float)value;

	audioSourceGOC->GetAudioPlayer()->SetVolume(fValue);

	if (audioSourceGOC->GetAudioPlayer()->GetVolume() != fValue)
	{
		double dValue = (double)audioSourceGOC->GetAudioPlayer()->GetVolume();

		m_GameObjectAudioSourceComponentWidgetQtUI.m_VolumeSB->setValue(dValue);
	}
}

void GameObjectAudioSourceComponentWidget::on_m_MinimumDistanceSB_valueChanged(double value)
{
	if (!m_IsReady)
	{
		return;
	}

	AudioSourceGOC* audioSourceGOC = GetAudioSourceGOC();
	if (audioSourceGOC == nullptr)
	{
		return;
	}

	float fValue = (float)value;

	audioSourceGOC->GetAudioPlayer()->SetMinDistance(fValue);

	if (audioSourceGOC->GetAudioPlayer()->GetMinDistance() != fValue)
	{
		double dValue = (double)audioSourceGOC->GetAudioPlayer()->GetMinDistance();

		m_GameObjectAudioSourceComponentWidgetQtUI.m_MinimumDistanceSB->setValue(dValue);
	}
}

void GameObjectAudioSourceComponentWidget::on_m_PitchSB_valueChanged(double value)
{
	if (!m_IsReady)
	{
		return;
	}

	AudioSourceGOC* audioSourceGOC = GetAudioSourceGOC();
	if (audioSourceGOC == nullptr)
	{
		return;
	}

	float fValue = (float)value;

	audioSourceGOC->GetAudioPlayer()->SetPitch(fValue);

	if (audioSourceGOC->GetAudioPlayer()->GetPitch() != fValue)
	{
		double dValue = (double)audioSourceGOC->GetAudioPlayer()->GetPitch();

		m_GameObjectAudioSourceComponentWidgetQtUI.m_PitchSB->setValue(dValue);
	}
}

void GameObjectAudioSourceComponentWidget::on_m_AttenuationSB_valueChanged(double value)
{
	if (!m_IsReady)
	{
		return;
	}

	AudioSourceGOC* audioSourceGOC = GetAudioSourceGOC();
	if (audioSourceGOC == nullptr)
	{
		return;
	}

	float fValue = (float)value;

	audioSourceGOC->GetAudioPlayer()->SetAttenuation(fValue);

	if (audioSourceGOC->GetAudioPlayer()->GetAttenuation() != fValue)
	{
		double dValue = (double)audioSourceGOC->GetAudioPlayer()->GetAttenuation();

		m_GameObjectAudioSourceComponentWidgetQtUI.m_AttenuationSB->setValue(dValue);
	}
}

void GameObjectAudioSourceComponentWidget::on_m_Is3DCheckBox_toggled(bool toggled)
{
	if (!m_IsReady)
	{
		return;
	}

	AudioSourceGOC* audioSourceGOC = GetAudioSourceGOC();
	if (audioSourceGOC == nullptr)
	{
		return;
	}

	audioSourceGOC->GetAudioPlayer()->Set3D(toggled);
}

void GameObjectAudioSourceComponentWidget::on_m_LoopCheckBox_toggled(bool toggled)
{
	if (!m_IsReady)
	{
		return;
	}

	AudioSourceGOC* audioSourceGOC = GetAudioSourceGOC();
	if (audioSourceGOC == nullptr)
	{
		return;
	}

	audioSourceGOC->GetAudioPlayer()->SetLoop(toggled);
}

void GameObjectAudioSourceComponentWidget::on_m_NameAudioSourceTextBox_editingFinished()
{
	if (!m_IsReady)
	{
		return;
	}

	AudioSourceGOC* audioSourceGOC = GetAudioSourceGOC();
	if (audioSourceGOC == nullptr)
	{
		return;
	}

	QString newNameQS = m_GameObjectAudioSourceComponentWidgetQtUI.m_NameAudioSourceTextBox->text();
	std::wstring newName = newNameQS.toStdWString();
	if (newName.empty())
	{
		QString name = QString::fromStdWString(audioSourceGOC->GetName());
		m_GameObjectAudioSourceComponentWidgetQtUI.m_NameAudioSourceTextBox->setText(name);
		return;
	}

	if (newName.compare(audioSourceGOC->GetName()) == 0)
	{
		return;
	}

	audioSourceGOC->SetName(newName);

	QTreeWidgetItem* item = m_GameObjectAudioSourceComponentWidgetQtUI.m_AudioSourceTree->selectedItems().at(0);
	item->setData(0, Qt::ItemDataRole::DisplayRole, newNameQS);
}

void GameObjectAudioSourceComponentWidget::dragEnterEvent(QDragEnterEvent* enterEvent)
{
	DragDropType type = AEQTHelpers::GetDragDropType(enterEvent);

	if(type == DragDropType::GameAsset)
	{
		enterEvent->accept();
		return;
	}

	enterEvent->ignore();
}

void GameObjectAudioSourceComponentWidget::dragLeaveEvent(QDragLeaveEvent* leaveEvent)
{
	leaveEvent->accept();
}

void GameObjectAudioSourceComponentWidget::dragMoveEvent(QDragMoveEvent* moveEvent)
{
	DragDropType type = AEQTHelpers::GetDragDropType(moveEvent);

	if (m_GameObjectAudioSourceComponentWidgetQtUI.m_AudioAssetTextBox->rect().contains(m_GameObjectAudioSourceComponentWidgetQtUI.m_AudioAssetTextBox->mapFromParent(moveEvent->pos())))
	{
		if(type == DragDropType::GameAsset)
		{
			moveEvent->accept();
			return;
		}
	}

	moveEvent->ignore();
}

void GameObjectAudioSourceComponentWidget::dropEvent(QDropEvent* dropEvent)
{
	DragDropType type = AEQTHelpers::GetDragDropType(dropEvent);

	if (m_GameObjectAudioSourceComponentWidgetQtUI.m_AudioAssetTextBox->rect().contains(m_GameObjectAudioSourceComponentWidgetQtUI.m_AudioAssetTextBox->mapFromParent(dropEvent->pos())))
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
