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

		AEResult CreateRawGameAssetBranch(RawGameAsset* rawAsset);

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

		AEResult RefreshRawGameAssetsTree();

	signals:
		void NewDropRawAssetsFiles(QStringList fileList);

		void EditCodeFile(QString filepath);

		void RawGameAssetReload(uint64_t rawGAID);

};

#endif