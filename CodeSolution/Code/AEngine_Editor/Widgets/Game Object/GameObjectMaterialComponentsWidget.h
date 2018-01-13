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

#ifndef _GAME_OBJECT_MATERIAL_COMPONENTS_WIDGET_H
#define _GAME_OBJECT_MATERIAL_COMPONENTS_WIDGET_H

/**********************
*   System Includes   *
***********************/
#include <stdint.h>

/*************************
*   3rd Party Includes   *
**************************/
#include "qevent.h"
#include "qobject.h"
#include "QtWidgets\qwidget.h"

/***************************
*   Game Engine Includes   *
****************************/
#include "Base\Base.h"
#include "Shaders\ShaderDefs.h"
#include "ui_GameObjectMaterialComponentsWidgetQt.h"

/********************
*   Forward Decls   *
*********************/
class GameApp;
class GameObject;
class ShaderAsset;
class MeshMaterialGOC;

/*****************
*   Class Decl   *
******************/
class GameObjectMaterialComponentsWidget sealed : public QWidget
{
	Q_OBJECT

	private:

		Ui::GameObjectMaterialComponentsWidgetQt m_GameObjectMaterialComponentsWidgetQtUI;

		GameObject* m_GameObject = nullptr;

		GameApp* m_GameApp = nullptr;

		bool m_IsReady = false;

		void InitFields();

		MeshMaterialGOC* GetMeshMaterialGOC(QTreeWidgetItem* item = nullptr);

		void PopulateMaterialsTree(uint32_t selectedIndex = 0);

		void dragEnterEvent(QDragEnterEvent* enterEvent) override;

		void dragLeaveEvent(QDragLeaveEvent* leaveEvent) override;

		void dragMoveEvent(QDragMoveEvent* moveEvent) override;

		void dropEvent(QDropEvent* dropEvent) override;

		AEResult DropAsset(QObject* object, ShaderType shaderType);

		AEResult SetGameAssetFromID(ShaderType shaderType, uint64_t gameAssetID);

		AEResult SetShaderGameAssetToCurrent(ShaderAsset* shaderAsset);

		void RefreshShaderNames();

		uint64_t SelectShaderAsset();

	private slots:

		void on_m_MaterialsTree_itemChanged(QTreeWidgetItem* item, int column);

		void on_m_MaterialsTree_itemSelectionChanged();

		void on_m_AddMaterialButton_clicked();

		void on_m_RemoveMaterialButton_clicked();

		void on_m_VSAddAssetButton_clicked();

		void on_m_PSAddAssetButton_clicked();

		void on_m_GSAddAssetButton_clicked();

		void on_m_CSAddAssetButton_clicked();

		void on_m_DSAddAssetButton_clicked();

		void on_m_HSAddAssetButton_clicked();

		void on_m_VSClearAssetButton_clicked();

		void on_m_PSClearAssetButton_clicked();

		void on_m_GSClearAssetButton_clicked();

		void on_m_CSClearAssetButton_clicked();

		void on_m_HSClearAssetButton_clicked();

		void on_m_DSClearAssetButton_clicked();

		void on_m_VSPropsButton_clicked();

		void on_m_PSPropsButton_clicked();

		void on_m_GSPropsButton_clicked();

		void on_m_CSPropsButton_clicked();

		void on_m_DSPropsButton_clicked();

		void on_m_HSPropsButton_clicked();

	public:
		GameObjectMaterialComponentsWidget(GameObject* gameObject, GameApp* gameApp, QWidget *parent = nullptr);
		~GameObjectMaterialComponentsWidget();
};


#endif