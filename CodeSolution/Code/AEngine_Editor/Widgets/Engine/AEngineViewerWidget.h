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

#ifndef _AENGINE_VIEWER_WIDGET_H
#define _AENGINE_VIEWER_WIDGET_H

/**********************
*   System Includes   *
***********************/
#include <thread>
#include <stdint.h>

/*************************
*   3rd Party Includes   *
**************************/
#include "QtWidgets\qwidget.h"

/***************************
*   Game Engine Includes   *
****************************/
#include "Base\Base.h"
#include "GameContentDefs.h"
#include "GameObject\GameObjectDefs.h"

/********************
*   Forward Decls   *
*********************/
class EngineViewer;

/*****************
*   Class Decl   *
******************/
class AEngineViewerWidget sealed : public QWidget
{
	Q_OBJECT

	private:
		std::thread m_EngineViewerThread;

		EngineViewer* m_EngineViewer = nullptr;

		bool m_ViewerHasFocus = false;

		bool m_ThreadRunning = false;

		bool m_IsReady = false;

		void RunEngineViewer();

		void paintEvent(QPaintEvent * event) override;

		bool nativeEvent(const QByteArray & eventType, void * message, long * result) override;

	public:
		AEngineViewerWidget(QWidget *parent = nullptr);
		~AEngineViewerWidget();

		inline EngineViewer* GetEngineViewer() const
		{
			return m_EngineViewer;
		}

		inline bool IsReady() const
		{
			return m_IsReady;
		}

		void LinkEditorToEngine();

		AEResult StartEngineViewerThread(const std::wstring& configProjFile);

		void StopEngineInstanceAndDestroy();

		AEResult AddRawGameAsset(const std::wstring& fileName, GameContentSubtype gameContentSubtype);

		AEResult ImportRawGameAsset(uint64_t id);

		AEResult ReloadRawGameAssets();

		AEResult CreateEmptyGameObject();

		AEResult AddGameObjectComponent(uint64_t gameObjectID, GameObjectComponentType componentType, GameObjectComponentTypeOption componentTypeOption);

		QPaintEngine* paintEngine() const override;
};


#endif