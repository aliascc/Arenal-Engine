/********************************************************
*
* Author: Carlos Chacón N.
*
* Copyright 2012-2015
*
*********************************************************/

#pragma once

#ifndef _GAME_OBJECT_AUDIO_SOURCE_COMPONENT_WIDGET_H
#define _GAME_OBJECT_AUDIO_SOURCE_COMPONENT_WIDGET_H

/**********************
*   System Includes   *
***********************/
#include <stdint.h>

/*************************
*   3rd Party Includes   *
**************************/
#include "qevent.h"
#include "QtWidgets\qwidget.h"

/***************************
*   Game Engine Includes   *
****************************/
#include "Base\Base.h"
#include "ui_GameObjectAudioSourceComponentWidgetQt.h"

/********************
*   Forward Decls   *
*********************/
class GameObject;
class EngineViewer;
class AudioSourceGOC;
class GameAssetManager;
class AngelScriptManager;

/*****************
*   Class Decl   *
******************/
class GameObjectAudioSourceComponentWidget sealed : public QWidget
{
	Q_OBJECT

	private:

		Ui::GameObjectAudioSourceComponentWidgetQt m_GameObjectAudioSourceComponentWidgetQtUI;

		GameObject* m_GameObject = nullptr;

		EngineViewer* m_EngineViewer = nullptr;

		bool m_IsReady = false;

		void InitFields();

		void PopulateAudioSourceTree(uint32_t selectedIndex = 0);

		void dragEnterEvent(QDragEnterEvent* enterEvent) override;

		void dragLeaveEvent(QDragLeaveEvent* leaveEvent) override;

		void dragMoveEvent(QDragMoveEvent* moveEvent) override;

		void dropEvent(QDropEvent* dropEvent) override;

		XEResult DropAsset(QObject* object);

		XEResult SetGameAssetFromID(uint64_t gameAssetID);

		XEResult RemoveGameAsset();

		AudioSourceGOC* GetAudioSourceGOC(QTreeWidgetItem* item = nullptr);

	private slots:

		void on_m_AddAudioSource_clicked();

		void on_m_RemoveAudioSource_clicked();

		void on_m_SearchAudioAsset_clicked();

		void on_m_AudioSourceTree_itemChanged(QTreeWidgetItem* item, int column);

		void on_m_AudioSourceTree_itemSelectionChanged();

		void on_m_IsSound_toggled(bool toggled);

		void on_m_VolumeSB_valueChanged(double value);

		void on_m_MinimumDistanceSB_valueChanged(double value);

		void on_m_PitchSB_valueChanged(double value);

		void on_m_AttenuationSB_valueChanged(double value);

		void on_m_Is3DCheckBox_toggled(bool toggled);

		void on_m_LoopCheckBox_toggled(bool toggled);

		void on_m_NameAudioSourceTextBox_editingFinished();

	public:
		GameObjectAudioSourceComponentWidget(GameObject* gameObject, EngineViewer* engineViewer, QWidget *parent = nullptr);
		~GameObjectAudioSourceComponentWidget();
};


#endif