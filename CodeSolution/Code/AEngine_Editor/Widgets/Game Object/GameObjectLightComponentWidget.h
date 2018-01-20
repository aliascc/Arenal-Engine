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

#ifndef _GAME_OBJECT_LIGHT_COMPONENT_WIDGET_H
#define _GAME_OBJECT_LIGHT_COMPONENT_WIDGET_H

/**********************
*   System Includes   *
***********************/
#include <stdint.h>

/*************************
*   3rd Party Includes   *
**************************/
#include "QtWidgets\qwidget.h"

/***************************
*   Game Engine Includes   *
****************************/
#include "Lights\LightDefs.h"
#include "ui_GameObjectLightComponentWidgetQt.h"

/********************
*   Forward Decls   *
*********************/
class Light;
class LightGOC;
class GameObject;
class EngineViewer;

/*****************
*   Class Decl   *
******************/
class GameObjectLightComponentWidget sealed : public QWidget
{
    Q_OBJECT

    private:

        Ui::GameObjectLightComponentWidgetQt m_GameObjectLightComponentWidgetQtUI;

        EngineViewer* m_EngineViewer = nullptr;

        GameObject* m_GameObject = nullptr;

        bool m_IsReady = false;

        void InitFields();

        LightGOC* GetLightGOC();

        Light* GetLight();

        void SetColorToColorWidget(const QColor& qColor);

        void SetLightTypeComboBoxIndex(LightType lightType);

    private slots:

        void on_m_IntensitySB_valueChanged(double newValue);

        void on_m_NearAttSB_editingFinished();

        void on_m_FarAttSB_editingFinished();

        void on_m_AngleSB_editingFinished();

        void on_m_FalloffAngleSB_editingFinished();

        void on_m_ColorChangerButton_clicked();

        void on_m_Enabled_stateChanged(int newState);

        void on_m_ShadowEnabled_stateChanged(int newState);

        void on_m_DrawFrustum_stateChanged(int newState);

        void on_m_DrawCascadeFrustums_stateChanged(int newState);

        void on_m_DrawSimpleFrustum_stateChanged(int newState);

        void on_m_DrawDebug_stateChanged(int newState);

        void on_m_LightTypeComboBox_currentIndexChanged(int newIndex);

    public:
        GameObjectLightComponentWidget(GameObject* gameObject, EngineViewer* engineViewer, QWidget *parent = nullptr);
        ~GameObjectLightComponentWidget();
};


#endif
