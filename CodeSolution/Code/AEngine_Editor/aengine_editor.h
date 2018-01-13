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

#pragma once

#ifndef _AEngine_EDITOR_H
#define _AEngine_EDITOR_H

/**********************
*   System Includes   *
***********************/
#include <stdint.h>

/*************************
*   3rd Party Includes   *
**************************/
#include "qevent.h"
#include "QtWidgets\qmainwindow.h"

/***************************
*   Game Engine Includes   *
****************************/
#include "ui_AEngine_editor.h"

/********************
*   Forward Decls   *
*********************/
class GameAsset;
class GameObject;
class ModelAsset;
class ShaderAsset;
class RawGameAsset;
class TextureAsset;
class AEngineViewerWidget;
class CodeEditorMainWindow;
class ProjectSelectionWidget;

/*****************
*   Class Decl   *
******************/
class AEngine_Editor sealed : public QMainWindow
{
	Q_OBJECT

	private:

		Ui::AEngine_EditorClass m_EditorUI;

		AEngineViewerWidget* m_AEngineViewerWidget = nullptr;

		CodeEditorMainWindow* m_CodeEditorMainWindow = nullptr;

		ProjectSelectionWidget* m_ProjectSelectionWidget = nullptr;

		void Initialize();

		void SetNewEngineInstance(const std::wstring& configProjFile);

		void SetEngineInstanceToAssets();

		void SetObjTypeUserData();

		void closeEvent(QCloseEvent* cEvent) override;

		void ShowProjectSelectionWidget();

		void RefreshAllViews();

	private slots:

		void on_m_SaveLayoutAction_triggered();

		void on_m_LoadLayoutAction_triggered();

		void on_m_NewRawGameAssetAction_triggered();

		void on_m_ReloadOutdatedRawGameAssetsAction_triggered();

		void on_m_CreateEmptyGameObjectAction_triggered();

		void on_m_AddGOComponentButton_clicked();

		void on_m_RefreshAllViewsAction_triggered();

		/******************
		*   Project Bar   *
		*******************/

		void on_m_MenuFileCreateProjectAction_triggered();

		void on_m_MenuFileLoadProjectAction_triggered();

		void on_m_MenuFileSaveProjectAction_triggered();

		/******************
		*   Physics Bar   *
		*******************/

		void on_m_StartPhysXDebuggerAction_triggered();

		void on_m_StopPhysXDebuggerAction_triggered();

		/***************
		*   Tool bar   *
		****************/
		void on_m_PlayAction_triggered();

		void on_m_PauseAction_triggered();

		void on_m_StopAction_triggered();

		/***********************
		*   Game Object Tree   *
		************************/
		void GameObjectChangedEvent(GameObject* gameObject);

		/*****************************
		*   Game Object Props Tree   *
		******************************/
		void GameObjectChangedNameEvent(uint64_t gameObjectID);

		/**********************************
		*   Raw Game Assets Tree Widget   *
		***********************************/
		void NewDropRawAssetsFilesEvent(QStringList fileList);

		void EditCodeFileEvent(QString filepath);

		void RawGameAssetReloadEvent(uint64_t rawGAID);

	public:

		AEngine_Editor(QWidget *parent = nullptr);
		~AEngine_Editor();

		void SaveLayout(const QString& fileName);

		void LoadLayout(const QString& fileName);
};

#endif // AEngine_EDITOR_H
