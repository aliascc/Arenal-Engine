/********************************************************
*
* Author: Carlos Chacón N.
*
* Copyright 2012-2015
*
*********************************************************/

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