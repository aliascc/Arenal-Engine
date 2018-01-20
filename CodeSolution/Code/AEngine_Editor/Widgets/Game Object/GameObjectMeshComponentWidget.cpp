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
#include "qtreewidget.h"

/***************************
*   Game Engine Includes   *
****************************/
#include "Utils\AEQTDefs.h"
#include "Base\BaseFunctions.h"
#include "GameAssets\GameAsset.h"
#include "GameObject\GameObject.h"
#include "GameAssets\Assets\MeshAsset.h"
#include "GameAssets\GameAssetManager.h"
#include "GameObjectMeshComponentWidget.h"
#include "GameObject\Components\MeshGOC.h"
#include "Dialogs\Game Objects\GameObjectSelectAssetDialog.h"

//Always include last
#include "Memory\MemLeaks.h"

/********************
*   Function Defs   *
*********************/
GameObjectMeshComponentWidget::GameObjectMeshComponentWidget(GameObject* gameObject, GameAssetManager* gameAssetManager, QWidget *parent)
    : QWidget(parent)
    , m_GameObject(gameObject)
    , m_GameAssetManager(gameAssetManager)
{
    m_GameObjectMeshComponentWidgetQtUI.setupUi(this);

    AEAssert(m_GameObject != nullptr);

    AEAssert(m_GameAssetManager != nullptr);

    InitFields();
}

GameObjectMeshComponentWidget::~GameObjectMeshComponentWidget()
{
}

void GameObjectMeshComponentWidget::InitFields()
{
    AEAssert(m_GameAssetManager != nullptr);
    if(m_GameAssetManager == nullptr)
    {
        return;
    }

    AEAssert(m_GameObject != nullptr);
    if(m_GameObject == nullptr)
    {
        return;
    }

    AEAssert(m_GameObject->HasMeshGOC());
    if(!m_GameObject->HasMeshGOC())
    {
        return;
    }

    MeshGOC* meshGOC = m_GameObject->GetMeshGOC();
    
    m_GameObjectMeshComponentWidgetQtUI.m_MeshAssetTextBox->setText(QString::fromStdWString(meshGOC->GetMeshName()));

    m_IsReady = true;
}

AEResult GameObjectMeshComponentWidget::DropAsset(QObject* object)
{
    if(!m_IsReady)
    {
        return AEResult::NotReady;
    }

    AEAssert(object != nullptr);
    if(object == nullptr)
    {
        return AEResult::NullParameter;
    }

    QTreeWidget* gameAssetTree = reinterpret_cast<QTreeWidget*>(object);

    QList<QTreeWidgetItem*> selectedItems = gameAssetTree->selectedItems();

    if(selectedItems.count() == 0)
    {
        return AEResult::Ok;
    }

    QTreeWidgetItem* item = selectedItems[0];
    
    uint64_t gameAssetID = item->data(0, AE_QT_ITEM_DATA_ROLE_GAME_ASSET_UNIQUE_ID).toULongLong();

    return SetGameAssetFromID(gameAssetID);
}

void GameObjectMeshComponentWidget::on_m_AddMeshAssetButton_clicked()
{
    if(!m_IsReady)
    {
        return;
    }

    GameObjectSelectAssetDialog selectAssetDialog(m_GameAssetManager, GameContentType::Mesh, this);

    int result = selectAssetDialog.exec();

    if(result != QDialog::Accepted)
    {
        return;
    }

    uint64_t gameAssetID = selectAssetDialog.GetGameAssetIDSelected();

    SetGameAssetFromID(gameAssetID);
}

AEResult GameObjectMeshComponentWidget::SetGameAssetFromID(uint64_t gameAssetID)
{
    if(!m_IsReady)
    {
        return AEResult::NotReady;
    }

    GameAsset* gameAsset = m_GameAssetManager->GetGameAsset(gameAssetID);

    if(gameAsset == nullptr)
    {
        return AEResult::NotFound;
    }

    if(gameAsset->GetGameContentType() != GameContentType::Mesh)
    {
        return AEResult::InvalidObjType;
    }

    MeshGOC* meshGOC = m_GameObject->GetMeshGOC();

    AEResult ret = meshGOC->SetMeshAsset(reinterpret_cast<MeshAsset*>(gameAsset));

    if(ret != AEResult::Ok)
    {
        return ret;
    }

    m_GameObjectMeshComponentWidgetQtUI.m_MeshAssetTextBox->setText(QString::fromStdWString(meshGOC->GetMeshName()));

    return AEResult::Ok;
}

void GameObjectMeshComponentWidget::on_m_ClearMeshAssetButton_clicked()
{
    if(!m_IsReady)
    {
        return;
    }

    RemoveGameAsset();
}

AEResult GameObjectMeshComponentWidget::RemoveGameAsset()
{
    if(!m_IsReady)
    {
        return AEResult::NotReady;
    }

    MeshGOC* meshGOC = m_GameObject->GetMeshGOC();

    AEResult ret = meshGOC->RemoveMeshAsset();

    if(ret != AEResult::Ok)
    {
        return ret;
    }

    m_GameObjectMeshComponentWidgetQtUI.m_MeshAssetTextBox->setText("");

    return AEResult::Ok;
}

void GameObjectMeshComponentWidget::dragEnterEvent(QDragEnterEvent* enterEvent)
{
    DragDropType type = AEQTHelpers::GetDragDropType(enterEvent);

    if(type == DragDropType::GameAsset)
    {
        enterEvent->accept();
        return;
    }

    enterEvent->ignore();
}

void GameObjectMeshComponentWidget::dragLeaveEvent(QDragLeaveEvent* leaveEvent)
{
    leaveEvent->accept();
}

void GameObjectMeshComponentWidget::dragMoveEvent(QDragMoveEvent* moveEvent)
{
    DragDropType type = AEQTHelpers::GetDragDropType(moveEvent);

    if(m_GameObjectMeshComponentWidgetQtUI.m_MeshAssetTextBox->rect().contains(m_GameObjectMeshComponentWidgetQtUI.m_MeshAssetTextBox->mapFromParent(moveEvent->pos()))) 
    {
        if(type == DragDropType::GameAsset)
        {
            moveEvent->accept();
            return;
        }
    }

    moveEvent->ignore();
}

void GameObjectMeshComponentWidget::dropEvent(QDropEvent* dropEvent)
{
    DragDropType type = AEQTHelpers::GetDragDropType(dropEvent);
    
    if(m_GameObjectMeshComponentWidgetQtUI.m_MeshAssetTextBox->rect().contains(m_GameObjectMeshComponentWidgetQtUI.m_MeshAssetTextBox->mapFromParent(dropEvent->pos()))) 
    {
        if(type == DragDropType::GameAsset)
        {
            if(DropAsset(dropEvent->source()) == AEResult::Ok)
            {
                dropEvent->accept();
                return;
            }
        }
    }

    dropEvent->ignore();
}
