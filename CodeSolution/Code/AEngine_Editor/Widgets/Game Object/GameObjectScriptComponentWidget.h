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

#ifndef _GAME_OBJECT_SCRIPT_COMPONENT_WIDGET_H
#define _GAME_OBJECT_SCRIPT_COMPONENT_WIDGET_H

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
#include "ui_GameObjectScriptComponentWidgetQt.h"

/********************
*   Forward Decls   *
*********************/
class GameObject;
class EngineViewer;
class GameAssetManager;
class AngelScriptManager;
class GameObjectScriptGOC;
class GameObjectScriptAsset;

/*****************
*   Class Decl   *
******************/
class GameObjectScriptComponentWidget sealed : public QWidget
{
    Q_OBJECT

    private:

        Ui::GameObjectScriptComponentWidgetQt m_GameObjectScriptComponentWidgetQtUI;

        GameObject* m_GameObject = nullptr;

        EngineViewer* m_EngineViewer = nullptr;

        bool m_IsReady = false;

        void InitFields();

        void PopulateGOSTree(uint32_t selectedIndex = 0);

        void dragEnterEvent(QDragEnterEvent* enterEvent) override;

        void dragLeaveEvent(QDragLeaveEvent* leaveEvent) override;

        void dragMoveEvent(QDragMoveEvent* moveEvent) override;

        void dropEvent(QDropEvent* dropEvent) override;

        AEResult DropAsset(QObject* object);

        AEResult SetGameAssetFromID(uint64_t gameAssetID);

        AEResult RemoveGameAsset();

        GameObjectScriptGOC* GetGameObjectScriptGOC(QTreeWidgetItem* item = nullptr);

    private slots:

        void on_m_AddGOScript_clicked();

        void on_m_RemoveGOScript_clicked();

        void on_m_SearchScriptAsset_clicked();

        void on_m_CreateRemoveInstance_clicked();

        void on_m_GameObjectScriptTree_itemChanged(QTreeWidgetItem* item, int column);

        void on_m_GameObjectScriptTree_itemSelectionChanged();

    public:
        GameObjectScriptComponentWidget(GameObject* gameObject, EngineViewer* engineViewer, QWidget *parent = nullptr);
        ~GameObjectScriptComponentWidget();
};


#endif
