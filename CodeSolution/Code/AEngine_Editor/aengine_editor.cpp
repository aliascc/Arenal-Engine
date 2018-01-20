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
#include "qfile.h"
#include "qfiledialog.h"
#include "qmessagebox.h"

/***************************
*   Game Engine Includes   *
****************************/
#include "PhysicsManager.h"
#include "AEngine_editor.h"
#include "Utils\AEQTDefs.h"
#include "Base\BaseFunctions.h"
#include "GameAssets\GameAsset.h"
#include "GameObject\GameObject.h"
#include "Utils\GraphicUtilsDefs.h"
#include "GameAssets\RawGameAsset.h"
#include "Engine Viewer\EngineViewer.h"
#include "GameAssets\GameAssetManager.h"
#include "GameAssets\Assets\MeshAsset.h"
#include "GameAssets\GameAssetManager.h"
#include "GameObject\GameObjectManager.h"
#include "GameObject\GameObjectManager.h"
#include "GameAssets\Assets\ModelAsset.h"
#include "GameAssets\Assets\ShaderAsset.h"
#include "GameAssets\Assets\TextureAsset.h"
#include "GameAssets\Assets\SkeletonAsset.h"
#include "GameAssets\Assets\AnimationAsset.h"
#include "Dialogs\Project\NewProjectDialog.h"
#include "Dialogs\Project\LoadProjectDialog.h"
#include "Widgets\Engine\AEngineViewerWidget.h"
#include "Dialogs\Project\ProjectSelectionDialog.h"
#include "Widgets\Code Editor\CodeEditorMainWindow.h"
#include "Dialogs\Raw Game Assets\NewRawGameAssetDialog.h"
#include "Widgets\Game Object\GameObjectMeshComponentWidget.h"
#include "Dialogs\Game Objects\GameObjectAddComponentDialog.h"
#include "Widgets\Game Object\GameObjectTransformationWidget.h"
#include "Widgets\Game Object\GameObjectBaseInformationWidget.h"
#include "Widgets\Game Object\GameObjectMaterialComponentsWidget.h"

//Always include last
#include "Memory\MemLeaks.h"

/********************
*   Function Defs   *
*********************/
AEngine_Editor::AEngine_Editor(QWidget *parent)
    : QMainWindow(parent)
{
    ////////////////////////////////////////
    //Initialize UI
    m_EditorUI.setupUi(this);

    ////////////////////////////////////////
    //Initialize Objects
    Initialize();
}

AEngine_Editor::~AEngine_Editor()
{
    DeleteMem(m_CodeEditorMainWindow);
}

void AEngine_Editor::Initialize()
{
    ////////////////////////////////////////
    //Register Meta Types
    qRegisterMetaType<Object3DChangeEventType>("Object3DChangeEventType");

    ////////////////////////////////////////
    //Initialize Engine Widget
    m_AEngineViewerWidget = new AEngineViewerWidget(this);

    ////////////////////////////////////////
    //Remove Central Widget
    QWidget* central = this->centralWidget();
    central->setParent(nullptr);
    DeleteMem(central);

    ////////////////////////////////////////
    //Set Engine Widget as Central
    this->setCentralWidget(m_AEngineViewerWidget);

    ////////////////////////////////////////
    //Load Default Layout
    std::wstring layoutFile = AE_PROJ_DEFAULT_LAYOUT_FILE_LOC;;
    LoadLayout(QString::fromStdWString(layoutFile));

    ////////////////////////////////////////
    //Create Code Editor Window
    m_CodeEditorMainWindow = new CodeEditorMainWindow();

    ////////////////////////////////////////
    //Link Engine to Widget
    m_AEngineViewerWidget->LinkEditorToEngine();

    ////////////////////////////////////////
    //Raw Game Assets Tree Init
    m_EditorUI.m_RawGameAssetsTree->InitFields();

    connect(m_EditorUI.m_RawGameAssetsTree, SIGNAL(NewDropRawAssetsFiles(QStringList)), this, SLOT(NewDropRawAssetsFilesEvent(QStringList)));
    connect(m_EditorUI.m_RawGameAssetsTree, SIGNAL(EditCodeFile(QString)), this, SLOT(EditCodeFileEvent(QString)));
    connect(m_EditorUI.m_RawGameAssetsTree, SIGNAL(RawGameAssetReload(uint64_t)), this, SLOT(RawGameAssetReloadEvent(uint64_t)));

    ////////////////////////////////////////
    //Game Assets Tree Init
    m_EditorUI.m_GameAssetsTree->InitFields();

    ////////////////////////////////////////
    //Game Objects Tree Init
    m_EditorUI.m_GameObjectsTree->InitFields();

    connect(m_EditorUI.m_GameObjectsTree, SIGNAL(GameObjectChanged(GameObject*)), this, SLOT(GameObjectChangedEvent(GameObject*)));

    ////////////////////////////////////////
    //Game Objects Props Tree Init
    m_EditorUI.m_GameObjectPropsTree->InitFields();

    connect(m_EditorUI.m_GameObjectPropsTree, SIGNAL(GameObjectChangedName(uint64_t)), this, SLOT(GameObjectChangedNameEvent(uint64_t)));

    ////////////////////////////////////////
    //Maximize WIndow
    this->showMaximized();

    ////////////////////////////////////////
    //Show Project Selection Widget
    EngineViewer* engineViewer = m_AEngineViewerWidget->GetEngineViewer();

    ProjectSelectionDialog projSelection(engineViewer);

    int result = projSelection.exec();
    if (result != QDialog::Accepted)
    {
        return;
    }

    //const std::wstring& configFile = newProject.GetConfigFile();

    //AETODO("Check return");
    //SetNewEngineInstance(configFile);

    m_IsInitialized = true;
}

void AEngine_Editor::SetNewEngineInstance(const std::wstring& configProjFile)
{
    AETODO("Check return");
    m_AEngineViewerWidget->StopEngineInstanceAndDestroy();

    ////////////////////////////////////////
    //Link Engine to Widget
    m_AEngineViewerWidget->LinkEditorToEngine();

    SetEngineInstanceToAssets();

    AETODO("Check return");
    m_AEngineViewerWidget->StartEngineViewerThread(configProjFile);
}

void AEngine_Editor::SetEngineInstanceToAssets()
{
    ////////////////////////////////////////
    //Get Game Engine Viewer
    EngineViewer* engineViewer = m_AEngineViewerWidget->GetEngineViewer();

    AEAssert(engineViewer != nullptr);
    if (engineViewer == nullptr)
    {
        return;
    }

    ////////////////////////////////////////
    //Raw Game Assets Tree Init
    m_EditorUI.m_RawGameAssetsTree->SetEngineViewer(engineViewer);

    m_EditorUI.m_RawGameAssetsTree->clear();

    ////////////////////////////////////////
    //Game Assets Tree Init
    m_EditorUI.m_GameAssetsTree->SetEngineViewer(engineViewer);

    m_EditorUI.m_GameAssetsTree->clear();

    ////////////////////////////////////////
    //Game Objects Tree Init
    m_EditorUI.m_GameObjectsTree->SetEngineViewer(engineViewer);

    m_EditorUI.m_GameObjectsTree->clear();

    ////////////////////////////////////////
    //Game Objects Props Tree Init
    m_EditorUI.m_GameObjectPropsTree->SetEngineViewer(engineViewer);

    m_EditorUI.m_GameObjectPropsTree->clear();
}

void AEngine_Editor::RefreshAllViews()
{
    AETODO("Check returns");

    m_EditorUI.m_GameAssetsTree->RefreshGameAssetsTree();

    m_EditorUI.m_RawGameAssetsTree->RefreshRawGameAssetsTree();

    m_EditorUI.m_GameObjectsTree->RefreshGameObjectsTree();
}

void AEngine_Editor::closeEvent(QCloseEvent* cEvent)
{
    if(m_CodeEditorMainWindow != nullptr)
    {
        m_CodeEditorMainWindow->close();
    }

    SaveLayout(QString::fromStdWString(AE_PROJ_DEFAULT_LAYOUT_FILE_LOC));

    QMainWindow::closeEvent(cEvent);
}

void AEngine_Editor::on_m_SaveLayoutAction_triggered()
{
    QString extSelected;
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save layout"), QString::fromStdWString(AE_PROJ_EDITOR_UI_LAYOUTS_DIR_LOC), QString::fromStdWString(AE_QT_UI_LAYOUT_EXT), &extSelected);
    
    if(!extSelected.isEmpty())
    {
        fileName = fileName + tr(".") + extSelected;
    }

    SaveLayout(fileName);
}

void AEngine_Editor::SaveLayout(const QString& fileName)
{
    if(fileName.isEmpty())
    {
        return;
    }

    QFile file(fileName);

    if(!file.open(QFile::WriteOnly))
    {
        AETODO("Use literal");
        QString msg = tr("Failed to open %1\n%2")
            .arg(fileName)
            .arg(file.errorString());

        QMessageBox::warning(this, tr("Error"), msg);

        return;
    }

    QByteArray geo_data = saveGeometry();
    QByteArray layout_data = saveState();

    bool ok = file.putChar((BYTE)geo_data.size());

    if(ok)
    {
        ok = file.write(geo_data) == geo_data.size();
    }

    if(ok)
    {
        ok = file.write(layout_data) == layout_data.size();
    }

    if(!ok)
    {
        AETODO("Use literal");
        QString msg = tr("Error writing to %1\n%2")
            .arg(fileName)
            .arg(file.errorString());

        QMessageBox::warning(this, tr("Error"), msg);
    }
}

void AEngine_Editor::on_m_LoadLayoutAction_triggered()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Load layout"), QString::fromStdWString(AE_PROJ_EDITOR_UI_LAYOUTS_DIR_LOC), QString::fromStdWString(AE_QT_UI_LAYOUT_EXT));

    LoadLayout(fileName);
}

void AEngine_Editor::LoadLayout(const QString& fileName)
{
    if(fileName.isEmpty())
    {
        return;
    }

    QFile file(fileName);
    
    if(!file.open(QFile::ReadOnly)) 
    {
        AETODO("Use literal");
        QString msg = tr("Failed to open %1\n%2")
            .arg(fileName)
            .arg(file.errorString());
        
        QMessageBox::warning(this, tr("Error"), msg);

        return;
    }

    BYTE geo_size = 0;
    QByteArray geo_data;
    QByteArray layout_data;

    bool ok = file.getChar((char*)&geo_size);
    
    if(ok)
    {
        geo_data = file.read(geo_size);
        ok = geo_data.size() == geo_size;
    }
    
    if(ok)
    {
        layout_data = file.readAll();
        ok = layout_data.size() > 0;
    }

    if (ok)
    {
        ok = restoreGeometry(geo_data);
    }
    
    if(ok)
    {
        ok = restoreState(layout_data);
    }

    if(!ok)
    {
        AETODO("Use literal");
        QString msg = tr("Error reading %1")
            .arg(fileName);
        QMessageBox::warning(this, tr("Error"), msg);
    }
}

void AEngine_Editor::on_m_NewRawGameAssetAction_triggered()
{
    NewRawGameAssetDialog rawAssetDiag(this);

    int result = rawAssetDiag.exec();
    
    if(result != QDialog::Accepted)
    {
        return;
    }

    AEResult ret = m_AEngineViewerWidget->AddRawGameAsset(rawAssetDiag.GetFilename(), rawAssetDiag.GetGameContentSubtype());

    if(ret != AEResult::Ok)
    {
        return;
    }

    AETODO("Check return");
    m_EditorUI.m_RawGameAssetsTree->RefreshRawGameAssetsTree();
}

void AEngine_Editor::on_m_ReloadOutdatedRawGameAssetsAction_triggered()
{
    m_AEngineViewerWidget->ReloadRawGameAssets();

    AETODO("Check return");
    m_EditorUI.m_GameAssetsTree->RefreshGameAssetsTree();

    AETODO("Check return");
    m_EditorUI.m_GameObjectPropsTree->ReloadGameObjectInfo();
}

void AEngine_Editor::on_m_CreateEmptyGameObjectAction_triggered()
{
    AEResult ret = m_AEngineViewerWidget->CreateEmptyGameObject();

    if(ret != AEResult::Ok)
    {
        return;
    }

    AETODO("check return");
    m_EditorUI.m_GameObjectsTree->RefreshGameObjectsTree();
}

void AEngine_Editor::on_m_AddGOComponentButton_clicked()
{
    uint64_t gameObjectID = m_EditorUI.m_GameObjectsTree->GetCurrentGameObjectID();

    if(gameObjectID == 0)
    {
        return;
    }

    GameObjectAddComponentDialog addComponentDiag(this);

    int result = addComponentDiag.exec();
    if(result != QDialog::Accepted)
    {
        return;
    }

    GameObjectComponentType componentType = GameObjectComponentType::Mesh;
    GameObjectComponentTypeOption componentTypeOption = addComponentDiag.GetGameObjectComponentTypeOption();

    switch (componentTypeOption)
    {
        case GameObjectComponentTypeOption::GameObjectScript:
            componentType = GameObjectComponentType::GameObjectScript;
            break;

        case GameObjectComponentTypeOption::Mesh:
            componentType = GameObjectComponentType::Mesh;
            break;

        case GameObjectComponentTypeOption::MeshMaterial:
            componentType = GameObjectComponentType::MeshMaterial;
            break;

        case GameObjectComponentTypeOption::MeshAnimation:
            componentType = GameObjectComponentType::MeshAnimation;
            break;

        case GameObjectComponentTypeOption::Light:
            componentType = GameObjectComponentType::Light;
            break;

        case GameObjectComponentTypeOption::RigidBody:
            componentType = GameObjectComponentType::Physics;
            break;

        case GameObjectComponentTypeOption::BoxCollider:
            componentType = GameObjectComponentType::Physics;
            break;

        case GameObjectComponentTypeOption::SphereCollider:
            componentType = GameObjectComponentType::Physics;
            break;

        case GameObjectComponentTypeOption::Camera:
            componentType = GameObjectComponentType::Camera;
            break;

        case GameObjectComponentTypeOption::AudioSource:
            componentType = GameObjectComponentType::AudioSource;
            break;

        case GameObjectComponentTypeOption::AudioListener:
            componentType = GameObjectComponentType::AudioListener;
            break;

        default:
            AEAssert(false);
            break;
    }

    AEResult ret = m_AEngineViewerWidget->AddGameObjectComponent(gameObjectID, componentType, componentTypeOption);
    if(ret != AEResult::Ok)
    {
        return;
    }

    GameObjectManager* gameObjectManager = m_AEngineViewerWidget->GetEngineViewer()->GetGameObjectManager();

    GameObject* gameObject = gameObjectManager->GetGameObject(gameObjectID);

    m_EditorUI.m_GameObjectPropsTree->DisplayGameObjectInfo(gameObject);
}

/***********************
*   Game Object Tree   *
************************/
void AEngine_Editor::GameObjectChangedEvent(GameObject* gameObject)
{
    AETODO("Check return");
    m_EditorUI.m_GameObjectPropsTree->DisplayGameObjectInfo(gameObject);
}

/*****************************
*   Game Object Props Tree   *
******************************/
void AEngine_Editor::GameObjectChangedNameEvent(uint64_t gameObjectID)
{
    AETODO("Check return");
    m_EditorUI.m_GameObjectsTree->GameObjectNameChanged(gameObjectID);
}

/**********************************
*   Raw Game Assets Tree Widget   *
***********************************/
void AEngine_Editor::NewDropRawAssetsFilesEvent(QStringList fileList)
{
    for(auto filename : fileList)
    {
        NewRawGameAssetDialog rawAssetDiag;

        rawAssetDiag.SetFilename(filename);

        int result = rawAssetDiag.exec();

        if(result != QDialog::Accepted)
        {
            continue;
        }

        AEResult ret = m_AEngineViewerWidget->AddRawGameAsset(rawAssetDiag.GetFilename(), rawAssetDiag.GetGameContentSubtype());

        if(ret != AEResult::Ok)
        {
            continue;
        }
    }

    AETODO("Check return");
    m_EditorUI.m_RawGameAssetsTree->RefreshRawGameAssetsTree();
}

void AEngine_Editor::EditCodeFileEvent(QString filepath)
{
    AETODO("check return");
    AEResult ret = m_CodeEditorMainWindow->AddCodeTab(filepath);

    m_CodeEditorMainWindow->show();

    m_CodeEditorMainWindow->activateWindow(); // for Windows

    //m_CodeEditorMainWindow->raise();  // for MacOS
}

void AEngine_Editor::RawGameAssetReloadEvent(uint64_t rawGAID)
{
    AETODO("Check return");
    m_EditorUI.m_GameAssetsTree->RefreshGameAssetsTree();

    AETODO("Check return");
    m_EditorUI.m_GameObjectPropsTree->ReloadGameObjectInfo();
}

void AEngine_Editor::on_m_RefreshAllViewsAction_triggered()
{
    RefreshAllViews();
}

/***************
*   Tool Bar   *
****************/
void AEngine_Editor::on_m_PlayAction_triggered()
{
    AETODO("Check return and log error");
    m_AEngineViewerWidget->GetEngineViewer()->EditorPlay();
}

void AEngine_Editor::on_m_PauseAction_triggered()
{
    AETODO("Check return and log error");
    m_AEngineViewerWidget->GetEngineViewer()->EditorPause();
}

void AEngine_Editor::on_m_StopAction_triggered()
{
    AETODO("Check return and log error");
    m_AEngineViewerWidget->GetEngineViewer()->EditorStop();
}

void AEngine_Editor::on_m_StartPhysXDebuggerAction_triggered()
{
    PhysicsManager* physicsManager = m_AEngineViewerWidget->GetEngineViewer()->GetPhysicsManager();

    AEResult ret = physicsManager->ConnectToPhysXDebugger();
    if (ret != AEResult::Ok)
    {
        AETODO("Set message box error");
    }
}

void AEngine_Editor::on_m_StopPhysXDebuggerAction_triggered()
{
    PhysicsManager* physicsManager = m_AEngineViewerWidget->GetEngineViewer()->GetPhysicsManager();

    AEResult ret = physicsManager->DisconnectToPhysXDebugger();
    if (ret != AEResult::Ok)
    {
        AETODO("Set message box error");
    }
}

/**************
*   Project   *
***************/

void AEngine_Editor::on_m_MenuFileCreateProjectAction_triggered()
{
    ////////////////////////////////////////
    //Get Game Engine Viewer
    EngineViewer* engineViewer = m_AEngineViewerWidget->GetEngineViewer();

    NewProjectDialog newProject(engineViewer);

    int result = newProject.exec();
    if (result != QDialog::Accepted)
    {
        return;
    }

    const std::wstring& configFile = newProject.GetConfigFile();

    AETODO("Check return");
    SetNewEngineInstance(configFile);
}

void AEngine_Editor::on_m_MenuFileSaveProjectAction_triggered()
{
    AEResult ret = m_AEngineViewerWidget->GetEngineViewer()->SaveGameInfo();

    AETODO("Use literals");
    if (ret != AEResult::Ok)
    {
        AETODO("Use literal");
        QString msg = tr("Unable to save project");

        QMessageBox::warning(this, tr("Error"), msg);
    }
}

void AEngine_Editor::on_m_MenuFileLoadProjectAction_triggered()
{
    LoadProjectDialog loadProject(nullptr);

    int result = loadProject.exec();
    if (result != QDialog::Accepted)
    {
        return;
    }

    const std::wstring& configFile = loadProject.GetConfigFile();

    AETODO("Check return");
    SetNewEngineInstance(configFile);

    RefreshAllViews();
}

