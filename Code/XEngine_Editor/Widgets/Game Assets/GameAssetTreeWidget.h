
/********************************************************
*
* Author: Carlos Chacón N.
*
* Created: 8/10/2013
*
* Desc:
*
*
* Copyright 2013
*
*********************************************************/

#pragma once

#ifndef _GAME_ASSET_TREE_WIDGET_H
#define _GAME_ASSET_TREE_WIDGET_H

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
class GameAsset;
class AudioAsset;
class ModelAsset;
class ShaderAsset;
class TextureAsset;
class EngineViewer;
class GameObjectScriptAsset;

/*****************
*   Class Decl   *
******************/
class GameAssetTreeWidget sealed : public QTreeWidget
{
	Q_OBJECT

	private:

		EngineViewer* m_EngineViewer = nullptr;

		bool m_IsReady = false;

		XEResult CreateGameAssetBranch(GameAsset* gameAsset);

		XEResult CreateModelGameAssetBranch(ModelAsset* gameAsset);

		XEResult CreateShaderGameAssetBranch(ShaderAsset* gameAsset);

		XEResult CreateTextureGameAssetBranch(TextureAsset* gameAsset);

		XEResult CreateGameObjectScriptGameAssetBranch(GameObjectScriptAsset* gameAsset);

		XEResult CreateAudioAssetGameAssetBranch(AudioAsset* gameAsset);

	public:
		GameAssetTreeWidget(QWidget* parent = nullptr);
		~GameAssetTreeWidget();

		inline void SetEngineViewer(EngineViewer* engineViewer)
		{
			m_EngineViewer = engineViewer;
		}

		void InitFields();

		XEResult RefreshGameAssetsTree();
};

#endif