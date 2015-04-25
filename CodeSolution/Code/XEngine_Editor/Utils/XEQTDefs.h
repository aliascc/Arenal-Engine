/********************************************************
*
* Author: Carlos Chacón N.
*
* Copyright 2012-2015
*
*********************************************************/

#pragma once

#ifndef _XE_QT_DEFS_H
#define _XE_QT_DEFS_H

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
#include "Base\XEObject.h"

/**************
*   Defines   *
***************/
#define XE_QT_URL_FILE_HEADER						"file:///"

#define XE_QT_MIME_FILE_TYPE_1						"application/x-qt-windows-mime;value=\"FileName\""
#define XE_QT_MIME_FILE_TYPE_2						"application/x-qt-windows-mime;value=\"FileNameW\""

#define XE_QT_UI_LAYOUT_EXT							"qtl"
#define XE_QT_UI_LAYOUT_DIR							XE_PREFIX_ENGINE_FILE_PATH_ASCII "Data\\Editor UI Layouts\\"
#define XE_QT_UI_DEFAULT_LAYOUT_PATH				XE_QT_UI_LAYOUT_DIR "default.qtl"

#define XE_QT_USER_DATA_SLOT_START					0
#define XE_QT_USER_DATA_RAW_ASSET_ID_SLOT			XE_QT_USER_DATA_SLOT_START + 0
#define XE_QT_USER_DATA_OBJ_TYPE_SLOT				XE_QT_USER_DATA_SLOT_START + 1
#define XE_QT_USER_DATA_SHADER_VAR_INFO				XE_QT_USER_DATA_SLOT_START + 2
#define XE_QT_USER_DATA_SHADER_TEX_BINDING_NAME		XE_QT_USER_DATA_SLOT_START + 3

#define XE_QT_ITEM_DATA_ROLE_UNIQUE_ID				Qt::ItemDataRole::UserRole + 1
#define XE_QT_ITEM_DATA_ROLE_GAME_ASSET_UNIQUE_ID	Qt::ItemDataRole::UserRole + 2

#define XE_QT_TREE_INDENTATION						10
#define XE_QT_TREE_GAME_OBJECT_PROPS_INDENTATION	0

#define XE_QT_NUMBER_FORMAT					'G'
#define XE_QT_PRECISION						6

/****************
*   Constants   *
*****************/
extern const QString XE_QT_Pos_Vec_Names[4];

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

enum class XEQTObjType : uint32_t
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
struct XEQTUserTemplateData : public QObjectUserData
{
	T m_Data;

	XEQTUserTemplateData()
	{
		memset(&m_Data, 0, sizeof(T));
	}

	XEQTUserTemplateData(const T data)
		: m_Data(data)
	{
	}
};


/***********************
*   Global Functions   *
************************/

namespace XEQTHelpers
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