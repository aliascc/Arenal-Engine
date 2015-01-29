
/********************************************************
*
* Author: Carlos Chacón N.
*
* Desc:
*
*
* Copyright 2012-2014
*
*********************************************************/

#pragma once

#ifndef _RAW_GAME_ASSET_TREE_WIDGET_H
#define _RAW_GAME_ASSET_TREE_WIDGET_H

/**********************
*   System Includes   *
***********************/
#include <string>
#include <stdint.h>

/*************************
*   3rd Party Includes   *
**************************/
#include "qtreewidget.h"

/***************************
*   Game Engine Includes   *
****************************/
#include "Base\Base.h"

/********************
*   Forward Decls   *
*********************/
class RawGameAsset;
class EngineViewer;

/*****************
*   Class Decl   *
******************/
class RawGameAssetTreeWidget sealed : public QTreeWidget
{
	Q_OBJECT

	private:

		EngineViewer* m_EngineViewer = nullptr;

		bool m_IsReady = false;

		void ParseDropFileList(const QList<QUrl>& urlList);

		XEResult CreateRawGameAssetBranch(RawGameAsset* rawAsset);

		void dragEnterEvent(QDragEnterEvent* enterEvent) override;

		void dragLeaveEvent(QDragLeaveEvent* leaveEvent) override;

		void dragMoveEvent(QDragMoveEvent* moveEvent) override;

		void dropEvent(QDropEvent* dropEvent) override;

	private slots:
		void EditCodePushButtonEvent();

		void ImportSingleRawAssetEvent();

	public:
		RawGameAssetTreeWidget(QWidget* parent = nullptr);
		~RawGameAssetTreeWidget();

		inline void SetEngineViewer(EngineViewer* engineViewer)
		{
			m_EngineViewer = engineViewer;
		}

		void InitFields();

		XEResult RefreshRawGameAssetsTree();

	signals:
		void NewDropRawAssetsFiles(QStringList fileList);

		void EditCodeFile(QString filepath);

		void RawGameAssetReload(uint64_t rawGAID);

};

#endif