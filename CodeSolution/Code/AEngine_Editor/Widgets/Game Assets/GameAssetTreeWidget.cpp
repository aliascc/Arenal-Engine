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
#include "GraphicsDefs.h"
#include "Utils\AEQTDefs.h"
#include "GameContentDefs.h"
#include "Base\BaseFunctions.h"
#include "GameAssetTreeWidget.h"
#include "GameAssets\GameAsset.h"
#include "Engine Viewer\EngineViewer.h"
#include "GameAssets\Assets\MeshAsset.h"
#include "GameAssets\GameAssetManager.h"
#include "GameAssets\Assets\AudioAsset.h"
#include "GameAssets\Assets\ModelAsset.h"
#include "GameAssets\Assets\ShaderAsset.h"
#include "GameAssets\Assets\TextureAsset.h"
#include "GameAssets\Assets\SkeletonAsset.h"
#include "GameAssets\Assets\AnimationAsset.h"
#include "GameAssets\Assets\GameObjectScriptAsset.h"

//Always include last
#include "Memory\MemLeaks.h"

/********************
*   Function Defs   *
*********************/
GameAssetTreeWidget::GameAssetTreeWidget(QWidget* parent)
    : QTreeWidget(parent)
{
}

GameAssetTreeWidget::~GameAssetTreeWidget()
{
}

void GameAssetTreeWidget::InitFields()
{
    ////////////////////////////////////////
    //Game Assets Tree Type
    AEQTUserTemplateData<AEQTObjType>* gameAssetsTreeType = new AEQTUserTemplateData<AEQTObjType>(AEQTObjType::GameAssetsTree);
    this->setUserData(AE_QT_USER_DATA_OBJ_TYPE_SLOT, gameAssetsTreeType);

    ///////////////////////////////////////////
    //Set preferred indentation
    this->setIndentation(AE_QT_TREE_INDENTATION);

    ///////////////////////////////////////////
    //Set Animation
    this->setAnimated(true);

    ///////////////////////////////////////////
    //Set Alternating Row Colors
    this->setAlternatingRowColors(false);

    ///////////////////////////////////////////
    //Set number of columns and their names
    this->setColumnCount(2);

    QAbstractItemModel* itemModel = this->model();

    itemModel->setHeaderData(0, Qt::Orientation::Horizontal, "Property", Qt::ItemDataRole::DisplayRole);
    itemModel->setHeaderData(1, Qt::Orientation::Horizontal, "Value", Qt::ItemDataRole::DisplayRole);

    ///////////////////////////////////////////
    //Finish
    m_IsReady = true;
}

AEResult GameAssetTreeWidget::RefreshGameAssetsTree()
{
    if(!m_IsReady)
    {
        return AEResult::NotReady;
    }

    AEAssert(m_EngineViewer != nullptr);
    if (m_EngineViewer == nullptr)
    {
        AETODO("Add new error GameAppNull");
        return AEResult::NullObj;
    }

    this->clear();

    ////////////////////////////////////////////
    //Get Scope Lock and lock Game App Loop
    GameAppScopedLock appLock = m_EngineViewer->GetGameAppScopedLock();
    appLock.StartLock();

    const GameAssetMap& gameAssetMap = m_EngineViewer->GetGameAssetManager()->GetGameAssetMap();

    for(auto gameAssetIt : gameAssetMap)
    {
        AETODO("check return");
        if (!gameAssetIt.second->IsBuiltInAsset())
        {
            CreateGameAssetBranch(gameAssetIt.second);
        }
    }

    return AEResult::Ok;
}

AEResult GameAssetTreeWidget::CreateGameAssetBranch(GameAsset* gameAsset)
{
    ////////////////////////////////////////////
    //No need to lock this function as it is internal
    //and calling function should lock it
    ////////////////////////////////////////////

    if(!m_IsReady)
    {
        return AEResult::NotReady;
    }

    AEAssert(m_EngineViewer != nullptr);
    if (m_EngineViewer == nullptr)
    {
        AETODO("Add new error GameAppNull");
        return AEResult::NullObj;
    }

    AEAssert(gameAsset != nullptr);
    if(gameAsset == nullptr)
    {
        return AEResult::NullParameter;
    }

    ////////////////////////////////////////
    //Check if it is a child
    //  When adding parent, child will be added
    if(gameAsset->GetParentAssetID() != 0)
    {
        return AEResult::Ok;
    }

    ////////////////////////////////////////
    //Call appropriate method to add asset branch
    switch (gameAsset->GetGameContentType())
    {
        case GameContentType::Model:
            return CreateModelGameAssetBranch(reinterpret_cast<ModelAsset*>(gameAsset));

        case GameContentType::Shader:
            return CreateShaderGameAssetBranch(reinterpret_cast<ShaderAsset*>(gameAsset));

        case GameContentType::Texture:
            return CreateTextureGameAssetBranch(reinterpret_cast<TextureAsset*>(gameAsset));

        case GameContentType::GameObjectScript:
            return CreateGameObjectScriptGameAssetBranch(reinterpret_cast<GameObjectScriptAsset*>(gameAsset));

        case GameContentType::Audio:
            return CreateAudioAssetGameAssetBranch(reinterpret_cast<AudioAsset*>(gameAsset));

        default:
            AEAssert(false);
            return AEResult::InvalidObjType;
    }
}

AEResult GameAssetTreeWidget::CreateModelGameAssetBranch(ModelAsset* gameAsset)
{
    ////////////////////////////////////////////
    //No need to lock this function as it is internal
    //and calling function should lock it
    ////////////////////////////////////////////

    if(!m_IsReady)
    {
        return AEResult::NotReady;
    }

    AEAssert(m_EngineViewer != nullptr);
    if (m_EngineViewer == nullptr)
    {
        AETODO("Add new error GameAppNull");
        return AEResult::NullObj;
    }

    AEAssert(gameAsset != nullptr);
    if(gameAsset == nullptr)
    {
        return AEResult::NullParameter;
    }

    ////////////////////////////////////////
    //Create Branch
    QTreeWidgetItem* branch = new QTreeWidgetItem();
    branch->setData(0, Qt::ItemDataRole::DisplayRole, QString::fromStdWString(gameAsset->GetName()));
    branch->setData(0, AE_QT_ITEM_DATA_ROLE_GAME_ASSET_UNIQUE_ID, QVariant(gameAsset->GetUniqueAssetID()));

    ////////////////////////////////////////
    //Add to Main tree
    this->addTopLevelItem(branch);

    ////////////////////////////////////////
    //Create Childs

    ////////////////////////////////////////
    //Game Content Type:
    QTreeWidgetItem* gameContentTypeChild = new QTreeWidgetItem();
    gameContentTypeChild->setData(0, Qt::ItemDataRole::DisplayRole, "GCT");
    gameContentTypeChild->setData(1, Qt::ItemDataRole::DisplayRole, "Model");
    gameContentTypeChild->setToolTip(0, "Game Content Type");

    ////////////////////////////////////////
    //Add Child to Model Branch
    branch->addChild(gameContentTypeChild);

    ////////////////////////////////////////
    //Meshes:
    const MeshAssetMap& meshAssetMap = gameAsset->GetMeshAssetMap();
    uint32_t meshCount = (uint32_t)meshAssetMap.size();

    QTreeWidgetItem* meshesChildBranch = new QTreeWidgetItem();
    meshesChildBranch->setData(0, Qt::ItemDataRole::DisplayRole, "Meshes");
    meshesChildBranch->setData(1, Qt::ItemDataRole::DisplayRole, meshCount);
    meshesChildBranch->setToolTip(1, "Number of Meshes");

    ////////////////////////////////////////
    //Add Child to Model Branch
    branch->addChild(meshesChildBranch);

    ////////////////////////////////////////
    //Add Meshes
    for (auto gaIt : meshAssetMap)
    {
        MeshAsset* meshAsset = gaIt.second.m_Asset;

        ////////////////////////////////////////
        //Mesh Childs
        QTreeWidgetItem* meshBranch = new QTreeWidgetItem();
        meshBranch->setFirstColumnSpanned(true);
        QString name = QString::fromStdWString(meshAsset->GetName());
        meshBranch->setData(0, Qt::ItemDataRole::DisplayRole, name);
        meshBranch->setData(0, AE_QT_ITEM_DATA_ROLE_GAME_ASSET_UNIQUE_ID, QVariant(meshAsset->GetUniqueAssetID()));

        ////////////////////////////////////////
        //Add Child to Meshes List Branch
        meshesChildBranch->addChild(meshBranch);

        ////////////////////////////////////////
        //Mesh Game Content Type:
        QTreeWidgetItem* meshContentTypeChild = new QTreeWidgetItem();
        meshContentTypeChild->setData(0, Qt::ItemDataRole::DisplayRole, "GCT");
        meshContentTypeChild->setData(1, Qt::ItemDataRole::DisplayRole, "Mesh");
        meshContentTypeChild->setToolTip(0, "Game Content Type");

        ////////////////////////////////////////
        //Add Child to Mesh Branch
        meshBranch->addChild(meshContentTypeChild);
    }

    ////////////////////////////////////////
    //Animations:
    const AnimationAssetMap& animationAssetMap = gameAsset->GetAnimationAssetMap();
    uint32_t animCount = (uint32_t)animationAssetMap.size();

    QTreeWidgetItem* animationsChildBranch = new QTreeWidgetItem();
    animationsChildBranch->setData(0, Qt::ItemDataRole::DisplayRole, "Animations");
    animationsChildBranch->setData(1, Qt::ItemDataRole::DisplayRole, animCount);
    animationsChildBranch->setToolTip(1, "Number of Animations");

    ////////////////////////////////////////
    //Add Child to Model Branch
    branch->addChild(animationsChildBranch);

    ////////////////////////////////////////
    //Add Animations
    for (auto gaIt : animationAssetMap)
    {
        AnimationAsset* animAsset = gaIt.second.m_Asset;

        ////////////////////////////////////////
        //Animation Childs
        QTreeWidgetItem* animBranch = new QTreeWidgetItem();
        animBranch->setFirstColumnSpanned(true);
        animBranch->setData(0, Qt::ItemDataRole::DisplayRole, QString::fromStdWString(animAsset->GetName()));
        animBranch->setData(0, AE_QT_ITEM_DATA_ROLE_GAME_ASSET_UNIQUE_ID, QVariant(animAsset->GetUniqueAssetID()));

        //Add Child to Animations List Branch
        animationsChildBranch->addChild(animBranch);
        
        //Animation Game Content Type:
        QTreeWidgetItem* animContentTypeChild = new QTreeWidgetItem();
        animContentTypeChild->setData(0, Qt::ItemDataRole::DisplayRole, "GCT");
        animContentTypeChild->setData(1, Qt::ItemDataRole::DisplayRole, "Animation");
        animContentTypeChild->setToolTip(0, "Game Content Type");

        //Add Child to Animation Branch
        animBranch->addChild(animContentTypeChild);
    }

    ////////////////////////////////////////
    //Skeleton:
    SkeletonAsset* skeletonAsset = gameAsset->GetSkeletonAsset();

    if(skeletonAsset != nullptr)
    {
        QTreeWidgetItem* skeletonChildBranch = new QTreeWidgetItem();
        skeletonChildBranch->setData(0, Qt::ItemDataRole::DisplayRole, "Skeleton");
        skeletonChildBranch->setToolTip(1, "Skeleton of the Mesh");

        ////////////////////////////////////////
        //Add Child to Model Branch
        branch->addChild(skeletonChildBranch);

        ////////////////////////////////////////
        //Skeleton Child
        QTreeWidgetItem* skeletonBranch = new QTreeWidgetItem();
        skeletonBranch->setFirstColumnSpanned(true);
        skeletonBranch->setData(0, Qt::ItemDataRole::DisplayRole, QString::fromStdWString(skeletonAsset->GetName()));
        skeletonBranch->setData(0, AE_QT_ITEM_DATA_ROLE_GAME_ASSET_UNIQUE_ID, QVariant(skeletonAsset->GetUniqueAssetID()));

        ////////////////////////////////////////
        //Add Child to Skeleton List Branch
        skeletonChildBranch->addChild(skeletonBranch);

        ////////////////////////////////////////
        //Skeleton Game Content Type:
        QTreeWidgetItem* skeletonContentTypeChild = new QTreeWidgetItem();
        skeletonContentTypeChild->setData(0, Qt::ItemDataRole::DisplayRole, "GCT");
        skeletonContentTypeChild->setData(1, Qt::ItemDataRole::DisplayRole, "Skeleton");
        skeletonContentTypeChild->setToolTip(0, "Game Content Type");

        ////////////////////////////////////////
        //Add Child to Animation Branch
        skeletonBranch->addChild(skeletonContentTypeChild);
    }

    ////////////////////////////////////////
    //Finish
    return AEResult::Ok;
}

AEResult GameAssetTreeWidget::CreateShaderGameAssetBranch(ShaderAsset* gameAsset)
{
    ////////////////////////////////////////////
    //No need to lock this function as it is internal
    //and calling function should lock it
    ////////////////////////////////////////////

    if(!m_IsReady)
    {
        return AEResult::NotReady;
    }

    AEAssert(m_EngineViewer != nullptr);
    if (m_EngineViewer == nullptr)
    {
        AETODO("Add new error GameAppNull");
        return AEResult::NullObj;
    }

    AEAssert(gameAsset != nullptr);
    if(gameAsset == nullptr)
    {
        return AEResult::NullParameter;
    }
    
    ////////////////////////////////////////
    //Create Branch
    QTreeWidgetItem* branch = new QTreeWidgetItem();
    branch->setData(0, Qt::ItemDataRole::DisplayRole, QString::fromStdWString(gameAsset->GetName()));
    branch->setData(0, AE_QT_ITEM_DATA_ROLE_GAME_ASSET_UNIQUE_ID, QVariant(gameAsset->GetUniqueAssetID()));

    ////////////////////////////////////////
    //Add to tree
    this->addTopLevelItem(branch);
    
    ////////////////////////////////////////
    //Game Content Type
    QTreeWidgetItem* gameContentTypeChild = new QTreeWidgetItem();
    gameContentTypeChild->setData(0, Qt::ItemDataRole::DisplayRole, "GCT");
    gameContentTypeChild->setData(1, Qt::ItemDataRole::DisplayRole, "Shader");
    gameContentTypeChild->setToolTip(0, "Game Content Type");

    ////////////////////////////////////////
    //Add Child to Model Branch
    branch->addChild(gameContentTypeChild);
    
    ////////////////////////////////////////
    //Shader Type
    QTreeWidgetItem* shaderTypeChild = new QTreeWidgetItem();
    shaderTypeChild->setData(0, Qt::ItemDataRole::DisplayRole, "Shader Type");
    QString shaderTypeString = QString::fromStdWString(AEGraphicHelpers::GetShaderTypeString(gameAsset->GetShaderType()));
    shaderTypeChild->setData(1, Qt::ItemDataRole::DisplayRole, shaderTypeString);
    shaderTypeChild->setToolTip(0, "Shader Type");

    ////////////////////////////////////////
    //Add Child to Model Branch
    branch->addChild(shaderTypeChild);

    ////////////////////////////////////////
    //Finish
    return AEResult::Ok;
}

AEResult GameAssetTreeWidget::CreateTextureGameAssetBranch(TextureAsset* gameAsset)
{
    ////////////////////////////////////////////
    //No need to lock this function as it is internal
    //and calling function should lock it
    ////////////////////////////////////////////

    if(!m_IsReady)
    {
        return AEResult::NotReady;
    }

    AEAssert(m_EngineViewer != nullptr);
    if (m_EngineViewer == nullptr)
    {
        AETODO("Add new error GameAppNull");
        return AEResult::NullObj;
    }

    AEAssert(gameAsset != nullptr);
    if(gameAsset == nullptr)
    {
        return AEResult::NullParameter;
    }
    
    ////////////////////////////////////////
    //Create Branch
    QTreeWidgetItem* branch = new QTreeWidgetItem();
    branch->setData(0, Qt::ItemDataRole::DisplayRole, QString::fromStdWString(gameAsset->GetName()));
    branch->setData(0, AE_QT_ITEM_DATA_ROLE_GAME_ASSET_UNIQUE_ID, QVariant(gameAsset->GetUniqueAssetID()));

    ////////////////////////////////////////
    //Add to Main Tree
    this->addTopLevelItem(branch);
    
    ////////////////////////////////////////
    //Game Content Type
    QTreeWidgetItem* gameContentTypeChild = new QTreeWidgetItem();
    gameContentTypeChild->setData(0, Qt::ItemDataRole::DisplayRole, "GCT");
    gameContentTypeChild->setData(1, Qt::ItemDataRole::DisplayRole, "Texture");
    gameContentTypeChild->setToolTip(0, "Game Content Type");

    ////////////////////////////////////////
    //Add Child to Model Branch
    branch->addChild(gameContentTypeChild);
    
    ////////////////////////////////////////
    //Texture Type
    QTreeWidgetItem* textureTypeChild = new QTreeWidgetItem();
    textureTypeChild->setData(0, Qt::ItemDataRole::DisplayRole, "Texture Type");
    QString shaderTypeString = QString::fromStdWString(AEGraphicHelpers::GetTextureTypeString(gameAsset->GetTextureType()));
    textureTypeChild->setData(1, Qt::ItemDataRole::DisplayRole, shaderTypeString);
    textureTypeChild->setToolTip(0, "Texture Type");

    ////////////////////////////////////////
    //Add Child to Model Branch
    branch->addChild(textureTypeChild);

    ////////////////////////////////////////
    //Finish
    return AEResult::Ok;
}

AEResult GameAssetTreeWidget::CreateGameObjectScriptGameAssetBranch(GameObjectScriptAsset* gameAsset)
{
    ////////////////////////////////////////////
    //No need to lock this function as it is internal
    //and calling function should lock it
    ////////////////////////////////////////////

    if (!m_IsReady)
    {
        return AEResult::NotReady;
    }

    AEAssert(m_EngineViewer != nullptr);
    if (m_EngineViewer == nullptr)
    {
        AETODO("Add new error GameAppNull");
        return AEResult::NullObj;
    }

    AEAssert(gameAsset != nullptr);
    if (gameAsset == nullptr)
    {
        return AEResult::NullParameter;
    }

    ////////////////////////////////////////
    //Create Branch
    QTreeWidgetItem* branch = new QTreeWidgetItem();
    branch->setData(0, Qt::ItemDataRole::DisplayRole, QString::fromStdWString(gameAsset->GetName()));
    branch->setData(0, AE_QT_ITEM_DATA_ROLE_GAME_ASSET_UNIQUE_ID, QVariant(gameAsset->GetUniqueAssetID()));

    ////////////////////////////////////////
    //Add to Main Tree
    this->addTopLevelItem(branch);

    ////////////////////////////////////////
    //Set background as red if it not loaded
    if (!gameAsset->IsLoaded())
    {
        branch->setBackgroundColor(0, QColor(255, 0, 0));
        branch->setBackgroundColor(1, QColor(255, 0, 0));
    }

    ////////////////////////////////////////
    //Game Content Type
    QTreeWidgetItem* gameContentTypeChild = new QTreeWidgetItem();
    gameContentTypeChild->setData(0, Qt::ItemDataRole::DisplayRole, "GCT");
    gameContentTypeChild->setData(1, Qt::ItemDataRole::DisplayRole, "Game Object Script");
    gameContentTypeChild->setToolTip(0, "Game Content Type");

    ////////////////////////////////////////
    //Add Child to Model Branch
    branch->addChild(gameContentTypeChild);

    ////////////////////////////////////////
    //Finish
    return AEResult::Ok;
}

AEResult GameAssetTreeWidget::CreateAudioAssetGameAssetBranch(AudioAsset* gameAsset)
{
    ////////////////////////////////////////////
    //No need to lock this function as it is internal
    //and calling function should lock it
    ////////////////////////////////////////////

    if (!m_IsReady)
    {
        return AEResult::NotReady;
    }

    AEAssert(m_EngineViewer != nullptr);
    if (m_EngineViewer == nullptr)
    {
        AETODO("Add new error GameAppNull");
        return AEResult::NullObj;
    }

    AEAssert(gameAsset != nullptr);
    if (gameAsset == nullptr)
    {
        return AEResult::NullParameter;
    }

    ////////////////////////////////////////
    //Create Branch
    QTreeWidgetItem* branch = new QTreeWidgetItem();
    branch->setData(0, Qt::ItemDataRole::DisplayRole, QString::fromStdWString(gameAsset->GetName()));
    branch->setData(0, AE_QT_ITEM_DATA_ROLE_GAME_ASSET_UNIQUE_ID, QVariant(gameAsset->GetUniqueAssetID()));

    ////////////////////////////////////////
    //Add to Main Tree
    this->addTopLevelItem(branch);

    ////////////////////////////////////////
    //Set background as red if it not loaded
    if (!gameAsset->IsLoaded())
    {
        branch->setBackgroundColor(0, QColor(255, 0, 0));
        branch->setBackgroundColor(1, QColor(255, 0, 0));
    }

    ////////////////////////////////////////
    //Game Content Type
    QTreeWidgetItem* gameContentTypeChild = new QTreeWidgetItem();
    gameContentTypeChild->setData(0, Qt::ItemDataRole::DisplayRole, "GCT");
    gameContentTypeChild->setData(1, Qt::ItemDataRole::DisplayRole, "Audio");
    gameContentTypeChild->setToolTip(0, "Game Content Type");

    ////////////////////////////////////////
    //Add Child to Model Branch
    branch->addChild(gameContentTypeChild);

    ////////////////////////////////////////
    //Finish
    return AEResult::Ok;
}
