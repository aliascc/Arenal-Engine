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

#ifndef _NEW_RAW_GAME_ASSET_DIALOG_H
#define _NEW_RAW_GAME_ASSET_DIALOG_H

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
#include "ui_NewRawGameAssetDialogQt.h"

/********************
*   Forward Decls   *
*********************/

/*****************
*   Class Decl   *
******************/
class NewRawGameAssetDialog sealed : public QDialog
{
    Q_OBJECT

    private:

        Ui::NewRawGameAssetDialogQt m_NewRawGameAssetDialogQtUI;

    private slots:

        void on_m_OpenFile_clicked();

    public:
        NewRawGameAssetDialog(QWidget *parent = 0);
        virtual ~NewRawGameAssetDialog();

        void SetFilename(const QString& filename);

        std::wstring GetFilename() const;

        GameContentSubtype GetGameContentSubtype() const;
};

#endif
