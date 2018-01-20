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
#include "qmessagebox.h"

/***************************
*   Game Engine Includes   *
****************************/
#include "Utils\AEQTDefs.h"
#include "GameApp\GameApp.h"
#include "Base\BaseFunctions.h"
#include "GameAssets\GameAsset.h"
#include "GameObject\GameObject.h"
#include "Models\Animation\Animation.h"
#include "GameAssets\GameAssetManager.h"
#include "GameAssets\Assets\SkeletonAsset.h"
#include "GameAssets\Assets\AnimationAsset.h"
#include "Models\Animation\AnimationPlayer.h"
#include "GameObject\Components\MeshAnimationGOC.h"
#include "GameObjectMeshAnimationComponentWidget.h"
#include "Dialogs\Game Objects\GameObjectSelectAssetDialog.h"

//Always include last
#include "Memory\MemLeaks.h"

/********************
*   Function Defs   *
*********************/
GameObjectMeshAnimationComponentWidget::GameObjectMeshAnimationComponentWidget(GameObject* gameObject, GameApp* gameApp, QWidget *parent)
    : QWidget(parent)
    , m_GameObject(gameObject)
    , m_GameApp(gameApp)
{
    m_GameObjectMeshComponentWidgetQtUI.setupUi(this);

    AEAssert(m_GameApp != nullptr);
    AEAssert(m_GameObject != nullptr);

    if (m_GameApp != nullptr && m_GameObject != nullptr)
    {
        m_HasInitObjs = true;
    }

    InitFields();
}

GameObjectMeshAnimationComponentWidget::~GameObjectMeshAnimationComponentWidget()
{
}

void GameObjectMeshAnimationComponentWidget::InitFields()
{
    if (!m_HasInitObjs)
    {
        return;
    }

    AEAssert(m_GameObject->HasMeshAnimationGOC());
    if (!m_GameObject->HasMeshAnimationGOC())
    {
        return;
    }

    ////////////////////////////////////////////////////////////
    //Get Mesh Animation GOC
    MeshAnimationGOC* meshAnimGOC = m_GameObject->GetMeshAnimationGOC();

    ////////////////////////////////////////////////////////////
    //Set Skeleton Name
    m_GameObjectMeshComponentWidgetQtUI.m_SkeletonAssetTextBox->setText(QString::fromStdWString(meshAnimGOC->GetSkeletonName()));

    /////////////////////////////////////////////////////////////
    //Fill Animation Data
    RefreshAnimationAssetTree();

    /////////////////////////////////////////////////////////////
    //Set Play Animation Options
    m_GameObjectMeshComponentWidgetQtUI.m_BlendTimeSB->setValue(meshAnimGOC->GetBlendTime());
    m_GameObjectMeshComponentWidgetQtUI.m_BlendAnimCB->setChecked(meshAnimGOC->GetBlendAnimation());
    m_GameObjectMeshComponentWidgetQtUI.m_LoopAnim->setChecked(meshAnimGOC->GetOnLoop());

    m_IsReady = true;
}

AEResult GameObjectMeshAnimationComponentWidget::DropAsset(QObject* object)
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

    return AddGameAssetFromID(gameAssetID);
}

AEResult GameObjectMeshAnimationComponentWidget::AddGameAssetFromID(uint64_t gameAssetID)
{
    if (!m_IsReady)
    {
        return AEResult::NotReady;
    }

    GameAsset* gameAsset = m_GameApp->GetGameAssetManager()->GetGameAsset(gameAssetID);

    if (gameAsset == nullptr)
    {
        return AEResult::NotFound;
    }

    switch (gameAsset->GetGameContentType())
    {
        case GameContentType::Animation:
            return AddAnimationAsset(dynamic_cast<AnimationAsset*>(gameAsset));

        case GameContentType::Skeleton:
            return AddSkeletonAsset(dynamic_cast<SkeletonAsset*>(gameAsset));

        default:
            return AEResult::InvalidObjType;
    }

    return AEResult::Ok;
}

AEResult GameObjectMeshAnimationComponentWidget::AddAnimationAsset(AnimationAsset* animAsset)
{
    ////////////////////////////////////////////////////////////
    //Pre-Checks
    if (!m_IsReady)
    {
        return AEResult::NotReady;
    }

    AEAssert(animAsset != nullptr);
    if (animAsset == nullptr)
    {
        return AEResult::NullParameter;
    }

    ////////////////////////////////////////////////////////////
    //Get Mesh Animation GOC
    MeshAnimationGOC* meshAnimGOC = m_GameObject->GetMeshAnimationGOC();

    ////////////////////////////////////////////////////////////
    //Set Animation Asset
    AEResult ret = meshAnimGOC->AddAnimationAsset(animAsset);
    if (ret != AEResult::Ok)
    {
        return ret;
    }

    ////////////////////////////////////////////////////////////
    //Set Refresh Animation Asset Tree
    RefreshAnimationAssetTree();

    return AEResult::Ok;
}

AEResult GameObjectMeshAnimationComponentWidget::RemoveAnimationAsset(uint64_t assetID)
{
    ////////////////////////////////////////////////////////////
    //Pre-Checks
    if (!m_IsReady)
    {
        return AEResult::NotReady;
    }

    ////////////////////////////////
    //Get Animation Asset
    GameAsset* gameAsset = m_GameApp->GetGameAssetManager()->GetGameAsset(assetID);
    if (gameAsset == nullptr)
    {
        return AEResult::NotFound;
    }

    ////////////////////////////////////////////////////////////
    //Get Mesh Animation GOC
    MeshAnimationGOC* meshAnimGOC = m_GameObject->GetMeshAnimationGOC();

    AEResult ret = meshAnimGOC->RemoveAnimationAsset(gameAsset->GetUniqueAssetID());
    if (ret != AEResult::Ok)
    {
        AETODO("Add log and window message");
        return AEResult::Fail;
    }

    ////////////////////////////////
    //Repopulate tree with new data
    RefreshAnimationAssetTree();

    return AEResult::Ok;
}

void GameObjectMeshAnimationComponentWidget::on_m_AddAnimationlButton_clicked()
{
    ////////////////////////////////////////////////////////////
    //Pre-Checks
    if (!m_IsReady)
    {
        return;
    }

    ////////////////////////////////////////////////////////////
    //Set Select Asset Dialog
    GameObjectSelectAssetDialog selectAssetDialog(m_GameApp->GetGameAssetManager(), GameContentType::Animation, this);

    int result = selectAssetDialog.exec();

    if (result != QDialog::Accepted)
    {
        return;
    }

    ////////////////////////////////////////////////////////////
    //Get Selected Asset ID
    uint64_t gameAssetID = selectAssetDialog.GetGameAssetIDSelected();

    ////////////////////////////////////////////////////////////
    //Add Asset
    AddGameAssetFromID(gameAssetID);
}

void GameObjectMeshAnimationComponentWidget::on_m_RemoveAnimationButton_clicked()
{
    if (!m_IsReady)
    {
        return;
    }

    QList<QTreeWidgetItem*> selectedItems = m_GameObjectMeshComponentWidgetQtUI.m_AnimationsTree->selectedItems();

    if (selectedItems.count() == 0)
    {
        return;
    }

    ////////////////////////////////
    //Ask if User is sure to delete item
    AETODO("Add Localization string");
    QMessageBox::StandardButton result = QMessageBox::information(this, "Remove Animation", "Are you sure you want to remove this Animation?", QMessageBox::Yes, QMessageBox::No);

    if (result != QMessageBox::Yes)
    {
        return;
    }

    ////////////////////////////////
    //Get Animation Asset ID
    QTreeWidgetItem* item = selectedItems[0];
    uint64_t meshAnimID = item->data(0, AE_QT_ITEM_DATA_ROLE_UNIQUE_ID).toULongLong();

    ////////////////////////////////
    //Remove Animation Asset
    RemoveAnimationAsset(meshAnimID);
}

void GameObjectMeshAnimationComponentWidget::RefreshAnimationAssetTree()
{
    ////////////////////////////////
    //Pre-checks
    if (!m_HasInitObjs)
    {
        return;
    }

    ////////////////////////////////
    //Clear List
    m_GameObjectMeshComponentWidgetQtUI.m_AnimationsTree->clear();

    ////////////////////////////////////////////////////////////
    //Get Mesh Animation GOC
    MeshAnimationGOC* meshAnimGOC = m_GameObject->GetMeshAnimationGOC();
    
    ////////////////////////////////
    //Get Animation Asset Map
    const AnimationAssetPairMap& animAssetMap = meshAnimGOC->GetAnimationAssetMap();

    ////////////////////////////////
    //Add Animations to Tree
    for (auto animAssetPairIt : animAssetMap)
    {
        ////////////////////////////////
        //Create Branch and Add Animation
        QTreeWidgetItem* animationBranch = new QTreeWidgetItem();
        QString name = QString::fromStdWString(animAssetPairIt.second.m_GameAsset->GetName());
        animationBranch->setData(0, Qt::ItemDataRole::DisplayRole, name);
        animationBranch->setData(0, AE_QT_ITEM_DATA_ROLE_UNIQUE_ID, animAssetPairIt.second.m_GameAsset->GetUniqueAssetID());
        animationBranch->setFlags(animationBranch->flags() | Qt::ItemFlag::ItemIsEditable);

        ////////////////////////////////
        //Add to Tree
        m_GameObjectMeshComponentWidgetQtUI.m_AnimationsTree->addTopLevelItem(animationBranch);
    }
}

AEResult GameObjectMeshAnimationComponentWidget::AddSkeletonAsset(SkeletonAsset* skeletonAsset)
{
    ////////////////////////////////////////////////////////////
    //Pre-Checks
    if (!m_IsReady)
    {
        return AEResult::NotReady;
    }

    AEAssert(skeletonAsset != nullptr);
    if (skeletonAsset == nullptr)
    {
        return AEResult::NullParameter;
    }

    ////////////////////////////////////////////////////////////
    //Get Mesh Animation GOC
    MeshAnimationGOC* meshAnimGOC = m_GameObject->GetMeshAnimationGOC();

    ////////////////////////////////////////////////////////////
    //Set Skeleton Asset;
    AEResult ret = meshAnimGOC->SetSkeletonAsset(skeletonAsset);
    if (ret != AEResult::Ok)
    {
        return ret;
    }

    ////////////////////////////////////////////////////////////
    //Set Skeleton Name
    m_GameObjectMeshComponentWidgetQtUI.m_SkeletonAssetTextBox->setText(QString::fromStdWString(skeletonAsset->GetName()));

    return AEResult::Ok;
}

AEResult GameObjectMeshAnimationComponentWidget::RemoveSkeletonAsset()
{
    ////////////////////////////////////////////////////////////
    //Pre-Checks
    if (!m_IsReady)
    {
        return AEResult::NotReady;
    }

    ////////////////////////////////////////////////////////////
    //Get Mesh Animation GOC
    MeshAnimationGOC* meshAnimGOC = m_GameObject->GetMeshAnimationGOC();

    ////////////////////////////////////////////////////////////
    //Remove Skeleton
    AEResult ret = meshAnimGOC->RemoveSkeletonAsset();
    if (ret != AEResult::Ok)
    {
        return ret;
    }

    ////////////////////////////////////////////////////////////
    //Clear name
    m_GameObjectMeshComponentWidgetQtUI.m_SkeletonAssetTextBox->setText("");

    ////////////////////////////////////////////////////////////
    //Finish
    return AEResult::Ok;
}

void GameObjectMeshAnimationComponentWidget::on_m_AddSkeletonAssetButton_clicked()
{
    ////////////////////////////////////////////////////////////
    //Pre-Checks
    if(!m_IsReady)
    {
        return;
    }

    ////////////////////////////////////////////////////////////
    //Set Select Asset Dialog
    GameObjectSelectAssetDialog selectAssetDialog(m_GameApp->GetGameAssetManager(), GameContentType::Skeleton, this);

    int result = selectAssetDialog.exec();

    if(result != QDialog::Accepted)
    {
        return;
    }

    ////////////////////////////////////////////////////////////
    //Get Selected Asset ID
    uint64_t gameAssetID = selectAssetDialog.GetGameAssetIDSelected();

    ////////////////////////////////////////////////////////////
    //Add Asset
    AddGameAssetFromID(gameAssetID);
}

void GameObjectMeshAnimationComponentWidget::on_m_ClearSkeletonAssetButton_clicked()
{
    ////////////////////////////////////////////////////////////
    //Pre-Checks
    if(!m_IsReady)
    {
        return;
    }

    ////////////////////////////////////////////////////////////
    //Remove Asset
    RemoveSkeletonAsset();
}

void GameObjectMeshAnimationComponentWidget::on_m_PlayTestAnimationButton_clicked()
{
    ////////////////////////////////
    //Pre-checks
    if (!m_IsReady)
    {
        return;
    }

    QList<QTreeWidgetItem*> selectedItems = m_GameObjectMeshComponentWidgetQtUI.m_AnimationsTree->selectedItems();

    if (selectedItems.count() == 0)
    {
        return;
    }

    ////////////////////////////////
    //Get Animation Asset ID
    QTreeWidgetItem* item = selectedItems[0];
    uint64_t meshAnimID = item->data(0, AE_QT_ITEM_DATA_ROLE_UNIQUE_ID).toULongLong();

    ////////////////////////////////
    //Get Animation Asset
    GameAsset* gameAsset = m_GameApp->GetGameAssetManager()->GetGameAsset(meshAnimID);
    if (gameAsset == nullptr)
    {
        return;
    }

    ////////////////////////////////////////////////////////////
    //Get Mesh Animation GOC
    MeshAnimationGOC* meshAnimGOC = m_GameObject->GetMeshAnimationGOC();

    ////////////////////////////////////////////////////////////
    //Get Mesh Animation GOC
    meshAnimGOC->PlayTestAnimation(gameAsset->GetUniqueAssetID());
}

void GameObjectMeshAnimationComponentWidget::on_m_LoopAnim_stateChanged(int newState)
{
    ////////////////////////////////
    //Pre-checks
    if (!m_IsReady)
    {
        return;
    }

    ////////////////////////////////////////////////////////////
    //Get Mesh Animation GOC
    MeshAnimationGOC* meshAnimGOC = m_GameObject->GetMeshAnimationGOC();

    ////////////////////////////////////////////////////////////
    //Set value
    meshAnimGOC->SetOnLoop(m_GameObjectMeshComponentWidgetQtUI.m_LoopAnim->isChecked());
}

void GameObjectMeshAnimationComponentWidget::on_m_BlendAnimCB_stateChanged(int newState)
{
    ////////////////////////////////
    //Pre-checks
    if (!m_IsReady)
    {
        return;
    }

    ////////////////////////////////////////////////////////////
    //Get Mesh Animation GOC
    MeshAnimationGOC* meshAnimGOC = m_GameObject->GetMeshAnimationGOC();

    ////////////////////////////////////////////////////////////
    //Set value
    meshAnimGOC->SetBlendAnimation(m_GameObjectMeshComponentWidgetQtUI.m_BlendAnimCB->isChecked());
}

void GameObjectMeshAnimationComponentWidget::on_m_BlendTimeSB_editingFinished()
{
    ////////////////////////////////
    //Pre-checks
    if (!m_IsReady)
    {
        return;
    }

    ////////////////////////////////////////////////////////////
    //Get Blend Time Value
    float blendTime = static_cast<float>(m_GameObjectMeshComponentWidgetQtUI.m_BlendTimeSB->value());

    ////////////////////////////////////////////////////////////
    //Get Mesh Animation GOC
    MeshAnimationGOC* meshAnimGOC = m_GameObject->GetMeshAnimationGOC();

    ////////////////////////////////////////////////////////////
    //Set value
    meshAnimGOC->SetBlendTime(blendTime);
}

///////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////

void GameObjectMeshAnimationComponentWidget::dragEnterEvent(QDragEnterEvent* enterEvent)
{
    DragDropType type = AEQTHelpers::GetDragDropType(enterEvent);

    if(type == DragDropType::GameAsset)
    {
        enterEvent->accept();
        return;
    }

    enterEvent->ignore();
}

void GameObjectMeshAnimationComponentWidget::dragLeaveEvent(QDragLeaveEvent* leaveEvent)
{
    leaveEvent->accept();
}

void GameObjectMeshAnimationComponentWidget::dragMoveEvent(QDragMoveEvent* moveEvent)
{
    DragDropType type = AEQTHelpers::GetDragDropType(moveEvent);

    if (m_GameObjectMeshComponentWidgetQtUI.m_SkeletonAssetTextBox->rect().contains(m_GameObjectMeshComponentWidgetQtUI.m_SkeletonAssetTextBox->mapFromParent(moveEvent->pos())))
    {
        if(type == DragDropType::GameAsset)
        {
            moveEvent->accept();
            return;
        }
    }

    if (m_GameObjectMeshComponentWidgetQtUI.m_AnimationsTree->rect().contains(m_GameObjectMeshComponentWidgetQtUI.m_AnimationsTree->mapFromParent(moveEvent->pos())))
    {
        if (type == DragDropType::GameAsset)
        {
            moveEvent->accept();
            return;
        }
    }

    moveEvent->ignore();
}

void GameObjectMeshAnimationComponentWidget::dropEvent(QDropEvent* dropEvent)
{
    DragDropType type = AEQTHelpers::GetDragDropType(dropEvent);

    if (m_GameObjectMeshComponentWidgetQtUI.m_SkeletonAssetTextBox->rect().contains(m_GameObjectMeshComponentWidgetQtUI.m_SkeletonAssetTextBox->mapFromParent(dropEvent->pos())))
    {
        if(type == DragDropType::GameAsset)
        {
            if (DropAsset(dropEvent->source()) == AEResult::Ok)
            {
                dropEvent->accept();
                return;
            }
        }
    }

    if (m_GameObjectMeshComponentWidgetQtUI.m_AnimationsTree->rect().contains(m_GameObjectMeshComponentWidgetQtUI.m_AnimationsTree->mapFromParent(dropEvent->pos())))
    {
        if (type == DragDropType::GameAsset)
        {
            if (DropAsset(dropEvent->source()) == AEResult::Ok)
            {
                dropEvent->accept();
                return;
            }
        }
    }

    dropEvent->ignore();
}
