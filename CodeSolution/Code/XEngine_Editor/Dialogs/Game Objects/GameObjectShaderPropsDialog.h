
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

#ifndef _GAME_OBJECT_SHADER_PROPS_DIALOG_H
#define _GAME_OBJECT_SHADER_PROPS_DIALOG_H

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
#include "ui_GameObjectShaderPropsDialogQt.h"

/********************
*   Forward Decls   *
*********************/
class GameAssetManager;
class ShaderProperties;

/*****************
*   Class Decl   *
******************/
class GameObjectShaderPropsDialog sealed : public QDialog
{
	Q_OBJECT

	private:
		Ui::GameObjectShaderPropsDialogQt m_GameObjectShaderPropsDialogQtUI;

		bool m_IsReady;

		void InitFields(const std::wstring& materialName, const std::wstring& shaderName, GameAssetManager* gameAssetManager, ShaderProperties* shaderProperties);

	public:
		GameObjectShaderPropsDialog(const std::wstring& materialName, const std::wstring& shaderName, GameAssetManager* gameAssetManager, ShaderProperties* shaderProperties, QWidget *parent = 0);
		virtual ~GameObjectShaderPropsDialog();
};

#endif