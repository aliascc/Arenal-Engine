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

#ifndef _GAME_OBJECT_BASE_INFORMATION_WIDGET_H
#define _GAME_OBJECT_BASE_INFORMATION_WIDGET_H

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
#include "ui_GameObjectBaseInformationWidgetQt.h"

/********************
*   Forward Decls   *
*********************/
class GameObject;
class EngineViewer;

/*****************
*   Class Decl   *
******************/
class GameObjectBaseInformationWidget sealed : public QWidget
{
    Q_OBJECT

    private:

        Ui::GameObjectBaseInformationWidgetQt m_GameObjectBaseInformationWidgetQtUI;

        EngineViewer* m_EngineViewer = nullptr;

        GameObject* m_GameObject;

        void InitFields();

    private slots:

        void on_m_GameObjectNameTextBox_editingFinished();

    public:
        GameObjectBaseInformationWidget(GameObject* gameObject, EngineViewer* engineViewer, QWidget *parent = nullptr);
        ~GameObjectBaseInformationWidget();

    signals:

        void NameChanged(uint64_t gameObjectID);
};


#endif
