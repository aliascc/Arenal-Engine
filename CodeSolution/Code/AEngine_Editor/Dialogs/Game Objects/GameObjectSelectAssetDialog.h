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

#ifndef _GAME_OBJECT_SELECT_ASSET_DIALOG_H
#define _GAME_OBJECT_SELECT_ASSET_DIALOG_H

/**********************
*   System Includes   *
***********************/

/*************************
*   3rd Party Includes   *
**************************/
#include "QtWidgets\qdialog.h"

/***************************
*   Game Engine Includes   *
****************************/
#include "GameContentDefs.h"
#include "ui_GameObjectSelectAssetDialogQt.h"

/********************
*   Forward Decls   *
*********************/
class GameAssetManager;
class StringProxyFilterModel;

/*****************
*   Class Decl   *
******************/
class GameObjectSelectAssetDialog sealed : public QDialog
{
    Q_OBJECT

    private:
        Ui::GameObjectSelectAssetDialogQt m_GameObjectSelectAssetDialogQtUI;

        GameAssetManager* m_GameAssetManager;

        StringProxyFilterModel* m_StringProxyFilterModel;

        bool m_IsReady;

        void InitFields(GameContentType gameContentType);

        void PopulateGameAssetsTree(GameContentType gameContentType);

    private slots:

        void on_m_FilterAssetTextBox_textChanged(QString string);

        void on_m_GameContentTypeComboBox_currentIndexChanged(int index);

    public:
        GameObjectSelectAssetDialog(GameAssetManager* gameAssetManager, GameContentType gameContentType, QWidget *parent = 0);
        virtual ~GameObjectSelectAssetDialog();

        void SetGameContentType(GameContentType gameContentType);

        uint64_t GetGameAssetIDSelected();
};

#endif
