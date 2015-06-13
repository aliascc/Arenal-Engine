/********************************************************
*
* Author: Carlos Chacón N.
*
* Copyright 2012-2015
*
*********************************************************/

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