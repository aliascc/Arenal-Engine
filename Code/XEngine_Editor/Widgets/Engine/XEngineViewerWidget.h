
/********************************************************
*
* Author: Carlos Chacón N.
*
* Desc:
*
*
* Copyright 2012-2015
*
*********************************************************/

#pragma once

#ifndef _XENGINE_VIEWER_WIDGET_H
#define _XENGINE_VIEWER_WIDGET_H

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
class XEngineViewerWidget sealed : public QWidget
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
		XEngineViewerWidget(QWidget *parent = nullptr);
		~XEngineViewerWidget();

		inline EngineViewer* GetEngineViewer() const
		{
			return m_EngineViewer;
		}

		inline bool IsReady() const
		{
			return m_IsReady;
		}

		void LinkEditorToEngine();

		XEResult StartEngineViewerThread(const std::wstring& configProjFile);

		XEResult AddRawGameAsset(const std::wstring& fileName, GameContentSubtype gameContentSubtype);

		XEResult ImportRawGameAsset(uint64_t id);

		XEResult ReloadRawGameAssets();

		XEResult CreateEmptyGameObject();

		XEResult AddGameObjectComponent(uint64_t gameObjectID, GameObjectComponentType componentType, GameObjectComponentTypeOption componentTypeOption);

		QPaintEngine* paintEngine() const override;
};


#endif