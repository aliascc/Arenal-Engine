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

#ifndef _AE_QT_DEFS_H
#define _AE_QT_DEFS_H

/**********************
*   System Includes   *
***********************/
#include <stdint.h>

/*************************
*   3rd Party Includes   *
**************************/
#include "qevent.h"
#include "qwidget.h"
#include "qvariant.h"
#include "qcombobox.h"
#include "qmimedata.h"

/***************************
*   Game Engine Includes   *
****************************/
#include "Color\Color.h"
#include "Base\AEObject.h"
#include "Base\BaseLocations.h"

/**************
*   Defines   *
***************/
#define AE_QT_URL_FILE_HEADER                       "file:///"

#define AE_QT_MIME_FILE_TYPE_1                      "application/x-qt-windows-mime;value=\"FileName\""
#define AE_QT_MIME_FILE_TYPE_2                      "application/x-qt-windows-mime;value=\"FileNameW\""

#define AE_QT_UI_LAYOUT_EXT                         L"qtl"

#define AE_QT_USER_DATA_SLOT_START                  0
#define AE_QT_USER_DATA_RAW_ASSET_ID_SLOT           AE_QT_USER_DATA_SLOT_START + 0
#define AE_QT_USER_DATA_OBJ_TYPE_SLOT               AE_QT_USER_DATA_SLOT_START + 1
#define AE_QT_USER_DATA_SHADER_VAR_INFO             AE_QT_USER_DATA_SLOT_START + 2
#define AE_QT_USER_DATA_SHADER_TEX_BINDING_NAME     AE_QT_USER_DATA_SLOT_START + 3

#define AE_QT_ITEM_DATA_ROLE_UNIQUE_ID              Qt::ItemDataRole::UserRole + 1
#define AE_QT_ITEM_DATA_ROLE_GAME_ASSET_UNIQUE_ID   Qt::ItemDataRole::UserRole + 2
#define AE_QT_ITEM_DATA_ROLE_PROJECT_DIR_LOCATION   Qt::ItemDataRole::UserRole + 3

#define AE_QT_TREE_INDENTATION                      10
#define AE_QT_TREE_GAME_OBJECT_PROPS_INDENTATION    0

#define AE_QT_NUMBER_FORMAT                         'G'
#define AE_QT_PRECISION                             6

/****************
*   Constants   *
*****************/
extern const QString AE_QT_Pos_Vec_Names[4];

/************
*   Enums   *
*************/
enum class DragDropType : uint32_t
{
    File = 0,
    GameAsset,
    GameObject,
    RawGameAsset,
    Unknown
};

enum class AEQTObjType : uint32_t
{
    GameAssetsTree = 0,
    RawGameAssetsTree,
    GameObjectsTree,
    GameObjectsPropsTree
};

/*****************
*   Struct Def   *
******************/

template <class T>
struct AEQTUserTemplateData : public QObjectUserData
{
    T m_Data;

    AEQTUserTemplateData()
    {
        memset(&m_Data, 0, sizeof(T));
    }

    AEQTUserTemplateData(const T data)
        : m_Data(data)
    {
    }
};


/***********************
*   Global Functions   *
************************/

namespace AEQTHelpers
{
    extern void SetComboBoxGameContentType(QComboBox* comboBox);

    extern void SetComboBoxGameContentSubType(QComboBox* comboBox);

    extern void SetComboBoxGameObjectComponentTypeOption(QComboBox* comboBox);

    extern void SetComboBoxLightType(QComboBox* comboBox);

    extern DragDropType GetDragDropType(QDropEvent* dropEvent);

    extern Color GetColorFromQColor(const QColor& qColor);

    extern QColor GetQColorFromColor(const Color& color);
}


/***********************
*   Template Classes   *
************************/

template<class T>
class WigetSignalBlocker
{
    private:
        T* m_Blocked = nullptr;
        bool m_PreviousState = false;

    public:
        WigetSignalBlocker(T* blockedWidget)
            : m_Blocked(blockedWidget)
            , m_PreviousState(blockedWidget->blockSignals(true))
        {
        }

        ~WigetSignalBlocker()
        {
            m_Blocked->blockSignals(m_PreviousState);
        }

        T* operator->()
        {
            return m_Blocked;
        }
};

#endif