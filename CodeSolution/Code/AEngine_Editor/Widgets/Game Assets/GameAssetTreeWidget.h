/********************************************************
*
* Author: Carlos Chacón N.
*
* Copyright 2012-2015
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

		AEResult CreateGameAssetBranch(GameAsset* gameAsset);

		AEResult CreateModelGameAssetBranch(ModelAsset* gameAsset);

		AEResult CreateShaderGameAssetBranch(ShaderAsset* gameAsset);

		AEResult CreateTextureGameAssetBranch(TextureAsset* gameAsset);

		AEResult CreateGameObjectScriptGameAssetBranch(GameObjectScriptAsset* gameAsset);

		AEResult CreateAudioAssetGameAssetBranch(AudioAsset* gameAsset);

	public:
		GameAssetTreeWidget(QWidget* parent = nullptr);
		~GameAssetTreeWidget();

		inline void SetEngineViewer(EngineViewer* engineViewer)
		{
			m_EngineViewer = engineViewer;
		}

		void InitFields();

		AEResult RefreshGameAssetsTree();
};

#endif