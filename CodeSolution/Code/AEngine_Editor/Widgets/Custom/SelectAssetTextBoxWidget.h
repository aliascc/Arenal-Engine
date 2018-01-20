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

#ifndef _SELECT_ASSET_TEXT_BOX_WIDGET_H
#define _SELECT_ASSET_TEXT_BOX_WIDGET_H

/**********************
*   System Includes   *
***********************/
#include <stdint.h>

/*************************
*   3rd Party Includes   *
**************************/
#include "QtWidgets\qwidget.h"

/***************************
*   Game Engine Includes   *
****************************/
#include "GameContentDefs.h"
#include "ui_SelectAssetTextBoxWidgetQt.h"

/********************
*   Forward Decls   *
*********************/
class GameAssetManager;

/*****************
*   Class Decl   *
******************/
class SelectAssetTextBoxWidget sealed : public QWidget
{
    Q_OBJECT

    private:

        Ui::SelectAssetTextBoxWidgetQt m_SelectAssetTextBoxWidgetUI;

        GameAssetManager* m_GameAssetManager;

        GameContentType m_GameContentType;

        uint64_t m_SelectedID;

        AEResult UpdateTextBox();

    private slots:

        void on_m_AddAsset_clicked();

        void on_m_RemoveAsset_clicked();

    public:
        SelectAssetTextBoxWidget(GameAssetManager* gameAssetManager, GameContentType gameContentType, QWidget *parent = nullptr);
        ~SelectAssetTextBoxWidget();

        inline uint64_t GetSelectedID() const
        {
            return m_SelectedID;
        }

        AETODO("need to remove this and find a better way");
        void SetTextureName(const QString& textureName);

        void ClearSelectedID();

    signals:

        void SelectedIDChanged(uint64_t gameAssetID);
};


#endif
