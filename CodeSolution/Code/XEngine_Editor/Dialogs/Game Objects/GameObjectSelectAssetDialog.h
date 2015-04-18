
/********************************************************
*
* Author: Carlos Chacón N.
*
* Created: 7/26/2013
*
* Desc:
*
*
* Copyright 2013
*
*********************************************************/

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