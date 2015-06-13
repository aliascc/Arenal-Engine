/********************************************************
*
* Author: Carlos Chacón N.
*
* Copyright 2012-2015
*
*********************************************************/

/**********************
*   System Includes   *
***********************/

/*************************
*   3rd Party Includes   *
**************************/

/***************************
*   Game Engine Includes   *
****************************/
#include "Base\BaseFunctions.h"
#include "GameObjectShaderPropsDialog.h"
#include "GameAssets\GameAssetManager.h"
#include "Graphic Extensions\Shader Extensions\Properties\ShaderProperties.h"

//Always include last
#include "Memory\MemLeaks.h"

/********************
*   Function Defs   *
*********************/
GameObjectShaderPropsDialog::GameObjectShaderPropsDialog(const std::wstring& materialName, const std::wstring& shaderName, GameAssetManager* gameAssetManager, ShaderProperties* shaderProperties, QWidget *parent)
	: QDialog(parent)
	, m_IsReady(false)
{
	m_GameObjectShaderPropsDialogQtUI.setupUi(this);

	InitFields(materialName, shaderName, gameAssetManager, shaderProperties);
}

GameObjectShaderPropsDialog::~GameObjectShaderPropsDialog()
{
}

void GameObjectShaderPropsDialog::InitFields(const std::wstring& materialName, const std::wstring& shaderName, GameAssetManager* gameAssetManager, ShaderProperties* shaderProperties)
{
	///////////////////////////////////////
	//Check that Shader Properties and Game Asset Manager is not null
	AEAssert(shaderProperties != nullptr);
	AEAssert(gameAssetManager != nullptr);
	if(shaderProperties == nullptr || gameAssetManager == nullptr)
	{
		return;
	}

	///////////////////////////////////////
	//Set name for Material and Shader
	QString qMatName = QString::fromStdWString(materialName);
	m_GameObjectShaderPropsDialogQtUI.m_MaterialNameTextBox->setText(qMatName);

	QString qShaderName = QString::fromStdWString(shaderName);
	m_GameObjectShaderPropsDialogQtUI.m_ShaderNameTextBox->setText(qShaderName);

	///////////////////////////////////////
	//Initialize Shader Variables and Display them
	AETODO("check return");
	m_GameObjectShaderPropsDialogQtUI.m_ShaderPropsTree->InitShaderPropsTree(gameAssetManager, shaderProperties);

	///////////////////////////////////////
	//Finish
	m_IsReady = true;
}