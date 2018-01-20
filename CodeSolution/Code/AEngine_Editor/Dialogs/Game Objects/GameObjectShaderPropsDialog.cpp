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
