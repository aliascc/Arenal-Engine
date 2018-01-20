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

#ifndef _GAME_OBJECT_MESH_ANIMATION_COMPONENT_WIDGET_H
#define _GAME_OBJECT_MESH_ANIMATION_COMPONENT_WIDGET_H

/**********************
*   System Includes   *
***********************/
#include <stdint.h>

/*************************
*   3rd Party Includes   *
**************************/
#include "qevent.h"
#include "QtWidgets\qwidget.h"

/***************************
*   Game Engine Includes   *
****************************/
#include "Base\Base.h"
#include "ui_GameObjectMeshAnimationComponentWidgetQt.h"

/********************
*   Forward Decls   *
*********************/
class GameApp;
class GameObject;
class SkeletonAsset;
class AnimationAsset;
class GameAssetManager;

/*****************
*   Class Decl   *
******************/
class GameObjectMeshAnimationComponentWidget sealed : public QWidget
{
    Q_OBJECT

    private:

        Ui::GameObjectMeshAnimationComponentWidgetQt m_GameObjectMeshComponentWidgetQtUI;

        GameObject* m_GameObject = nullptr;

        GameApp* m_GameApp = nullptr;

        bool m_IsReady = false;

        bool m_HasInitObjs = false;

        void dragEnterEvent(QDragEnterEvent* enterEvent) override;

        void dragLeaveEvent(QDragLeaveEvent* leaveEvent) override;

        void dragMoveEvent(QDragMoveEvent* moveEvent) override;

        void dropEvent(QDropEvent* dropEvent) override;

        void InitFields();

        AEResult DropAsset(QObject* object);

        AEResult AddGameAssetFromID(uint64_t gameAssetID);

        AEResult AddSkeletonAsset(SkeletonAsset* skeletonAsset);

        AEResult RemoveSkeletonAsset();

        AEResult AddAnimationAsset(AnimationAsset* animAsset);

        AEResult RemoveAnimationAsset(uint64_t assetID);

        void RefreshAnimationAssetTree();

    private slots:

        void on_m_AddAnimationlButton_clicked();

        void on_m_RemoveAnimationButton_clicked();

        void on_m_AddSkeletonAssetButton_clicked();

        void on_m_ClearSkeletonAssetButton_clicked();

        void on_m_PlayTestAnimationButton_clicked();

        void on_m_LoopAnim_stateChanged(int newState);

        void on_m_BlendAnimCB_stateChanged(int newState);

        void on_m_BlendTimeSB_editingFinished();

    public:
        GameObjectMeshAnimationComponentWidget(GameObject* gameObject, GameApp* gameApp, QWidget *parent = nullptr);
        ~GameObjectMeshAnimationComponentWidget();
};


#endif
