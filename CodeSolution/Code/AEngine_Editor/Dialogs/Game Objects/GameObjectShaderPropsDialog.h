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
