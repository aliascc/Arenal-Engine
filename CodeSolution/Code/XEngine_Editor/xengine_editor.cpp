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
#include "qfile.h"
#include "qfiledialog.h"
#include "qmessagebox.h"

/***************************
*   Game Engine Includes   *
****************************/
#include "PhysicsManager.h"
#include "xengine_editor.h"
#include "Utils\XEQTDefs.h"
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
#include "Widgets\Engine\XEngineViewerWidget.h"
#include "Widgets\Project\ProjectSelectionWidget.h"
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
XEngine_Editor::XEngine_Editor(QWidget *parent)
	: QMainWindow(parent)
{
	////////////////////////////////////////
	//Initialize UI
	m_EditorUI.setupUi(this);

	////////////////////////////////////////
	//Initialize Objects
	Initialize();
}

XEngine_Editor::~XEngine_Editor()
{
	DeleteMem(m_CodeEditorMainWindow);
	DeleteMem(m_ProjectSelectionWidget);
}

void XEngine_Editor::Initialize()
{
	////////////////////////////////////////
	//Register Meta Types
	qRegisterMetaType<Object3DChangeEventType>("Object3DChangeEventType");

	////////////////////////////////////////
	//Initialize Engine Widget
	m_XEngineViewerWidget = new XEngineViewerWidget(this);

	////////////////////////////////////////
	//Remove Central Widget
	QWidget* central = this->centralWidget();
	central->setParent(nullptr);
	DeleteMem(central);

	////////////////////////////////////////
	//Set Engine Widget as Central
	this->setCentralWidget(m_XEngineViewerWidget);

	////////////////////////////////////////
	//Load Default Layout
	std::wstring layoutFile = XE_ENGINE_BIN_TO_DATA_PATH_PREFIX;
	layoutFile += XE_PROJ_DEFAULT_LAYOUT_FILE_LOC;;
	LoadLayout(QString::fromStdWString(layoutFile));

	////////////////////////////////////////
	//Create Code Editor Window
	m_CodeEditorMainWindow = new CodeEditorMainWindow();

	////////////////////////////////////////
	//Link Engine to Widget
	m_XEngineViewerWidget->LinkEditorToEngine();

	////////////////////////////////////////
	//Get Game Engine Viewer
	EngineViewer* engineViewer = m_XEngineViewerWidget->GetEngineViewer();

	////////////////////////////////////////
	//Raw Game Assets Tree Init
	m_EditorUI.m_RawGameAssetsTree->SetEngineViewer(engineViewer);

	m_EditorUI.m_RawGameAssetsTree->InitFields();

	connect(m_EditorUI.m_RawGameAssetsTree, SIGNAL(NewDropRawAssetsFiles(QStringList)), this, SLOT(NewDropRawAssetsFilesEvent(QStringList)));
	connect(m_EditorUI.m_RawGameAssetsTree, SIGNAL(EditCodeFile(QString)), this, SLOT(EditCodeFileEvent(QString)));
	connect(m_EditorUI.m_RawGameAssetsTree, SIGNAL(RawGameAssetReload(uint64_t)), this, SLOT(RawGameAssetReloadEvent(uint64_t)));

	////////////////////////////////////////
	//Game Assets Tree Init
	m_EditorUI.m_GameAssetsTree->SetEngineViewer(engineViewer);

	m_EditorUI.m_GameAssetsTree->InitFields();

	////////////////////////////////////////
	//Game Objects Tree Init
	m_EditorUI.m_GameObjectsTree->SetEngineViewer(engineViewer);

	m_EditorUI.m_GameObjectsTree->InitFields();

	connect(m_EditorUI.m_GameObjectsTree, SIGNAL(GameObjectChanged(GameObject*)), this, SLOT(GameObjectChangedEvent(GameObject*)));

	////////////////////////////////////////
	//Game Objects Props Tree Init
	m_EditorUI.m_GameObjectPropsTree->SetEngineViewer(engineViewer);

	m_EditorUI.m_GameObjectPropsTree->InitFields();

	connect(m_EditorUI.m_GameObjectPropsTree, SIGNAL(GameObjectChangedName(uint64_t)), this, SLOT(GameObjectChangedNameEvent(uint64_t)));

	////////////////////////////////////////
	//Maximize WIndow
	this->showMaximized();

	////////////////////////////////////////
	//Show Project Selection Widget
	m_ProjectSelectionWidget = new ProjectSelectionWidget();
}

void XEngine_Editor::closeEvent(QCloseEvent* cEvent)
{
	if(m_CodeEditorMainWindow != nullptr)
	{
		m_CodeEditorMainWindow->close();
	}

	SaveLayout(QString::fromStdWString(XE_PROJ_DEFAULT_LAYOUT_FILE_LOC));

	QMainWindow::closeEvent(cEvent);
}

void XEngine_Editor::on_m_SaveLayoutAction_triggered()
{
	QString extSelected;
	QString fileName = QFileDialog::getSaveFileName(this, tr("Save layout"), QString::fromStdWString(XE_PROJ_EDITOR_UI_LAYOUTS_DIR_LOC), QString::fromStdWString(XE_QT_UI_LAYOUT_EXT), &extSelected);
	
	if(!extSelected.isEmpty())
	{
		fileName = fileName + tr(".") + extSelected;
	}

	SaveLayout(fileName);
}

void XEngine_Editor::SaveLayout(const QString& fileName)
{
	if(fileName.isEmpty())
	{
		return;
	}

	QFile file(fileName);

	if(!file.open(QFile::WriteOnly))
	{
		XETODO("Use literal");
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
		XETODO("Use literal");
		QString msg = tr("Error writing to %1\n%2")
			.arg(fileName)
			.arg(file.errorString());

		QMessageBox::warning(this, tr("Error"), msg);
	}
}

void XEngine_Editor::on_m_LoadLayoutAction_triggered()
{
	QString fileName = QFileDialog::getOpenFileName(this, tr("Load layout"), QString::fromStdWString(XE_PROJ_EDITOR_UI_LAYOUTS_DIR_LOC), QString::fromStdWString(XE_QT_UI_LAYOUT_EXT));

	LoadLayout(fileName);
}

void XEngine_Editor::LoadLayout(const QString& fileName)
{
	if(fileName.isEmpty())
	{
		return;
	}

	QFile file(fileName);
	
	if(!file.open(QFile::ReadOnly)) 
	{
		XETODO("Use literal");
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
		XETODO("Use literal");
		QString msg = tr("Error reading %1")
			.arg(fileName);
		QMessageBox::warning(this, tr("Error"), msg);
	}
}

void XEngine_Editor::on_m_NewRawGameAssetAction_triggered()
{
	NewRawGameAssetDialog rawAssetDiag(this);

	int result = rawAssetDiag.exec();
	
	if(result != QDialog::Accepted)
	{
		return;
	}

	XEResult ret = m_XEngineViewerWidget->AddRawGameAsset(rawAssetDiag.GetFilename(), rawAssetDiag.GetGameContentSubtype());

	if(ret != XEResult::Ok)
	{
		return;
	}

	XETODO("Check return");
	m_EditorUI.m_RawGameAssetsTree->RefreshRawGameAssetsTree();
}

void XEngine_Editor::on_m_ReloadOutdatedRawGameAssetsAction_triggered()
{
	m_XEngineViewerWidget->ReloadRawGameAssets();

	XETODO("Check return");
	m_EditorUI.m_GameAssetsTree->RefreshGameAssetsTree();

	XETODO("Check return");
	m_EditorUI.m_GameObjectPropsTree->ReloadGameObjectInfo();
}

void XEngine_Editor::on_m_CreateEmptyGameObjectAction_triggered()
{
	XEResult ret = m_XEngineViewerWidget->CreateEmptyGameObject();

	if(ret != XEResult::Ok)
	{
		return;
	}

	XETODO("check return");
	m_EditorUI.m_GameObjectsTree->RefreshGameObjectsTree();
}

void XEngine_Editor::on_m_AddGOComponentButton_clicked()
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
			XEAssert(false);
			break;
	}

	XEResult ret = m_XEngineViewerWidget->AddGameObjectComponent(gameObjectID, componentType, componentTypeOption);
	if(ret != XEResult::Ok)
	{
		return;
	}

	GameObjectManager* gameObjectManager = m_XEngineViewerWidget->GetEngineViewer()->GetGameObjectManager();

	GameObject* gameObject = gameObjectManager->GetGameObject(gameObjectID);

	m_EditorUI.m_GameObjectPropsTree->DisplayGameObjectInfo(gameObject);
}

/***********************
*   Game Object Tree   *
************************/
void XEngine_Editor::GameObjectChangedEvent(GameObject* gameObject)
{
	XETODO("Check return");
	m_EditorUI.m_GameObjectPropsTree->DisplayGameObjectInfo(gameObject);
}

/*****************************
*   Game Object Props Tree   *
******************************/
void XEngine_Editor::GameObjectChangedNameEvent(uint64_t gameObjectID)
{
	XETODO("Check return");
	m_EditorUI.m_GameObjectsTree->GameObjectNameChanged(gameObjectID);
}

/**********************************
*   Raw Game Assets Tree Widget   *
***********************************/
void XEngine_Editor::NewDropRawAssetsFilesEvent(QStringList fileList)
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

		XEResult ret = m_XEngineViewerWidget->AddRawGameAsset(rawAssetDiag.GetFilename(), rawAssetDiag.GetGameContentSubtype());

		if(ret != XEResult::Ok)
		{
			continue;
		}
	}

	XETODO("Check return");
	m_EditorUI.m_RawGameAssetsTree->RefreshRawGameAssetsTree();
}

void XEngine_Editor::EditCodeFileEvent(QString filepath)
{
	XETODO("check return");
	XEResult ret = m_CodeEditorMainWindow->AddCodeTab(filepath);

	m_CodeEditorMainWindow->show();

	m_CodeEditorMainWindow->activateWindow(); // for Windows

	//m_CodeEditorMainWindow->raise();  // for MacOS
}

void XEngine_Editor::RawGameAssetReloadEvent(uint64_t rawGAID)
{
	XETODO("Check return");
	m_EditorUI.m_GameAssetsTree->RefreshGameAssetsTree();

	XETODO("Check return");
	m_EditorUI.m_GameObjectPropsTree->ReloadGameObjectInfo();
}

void XEngine_Editor::on_m_RefreshAllViewsAction_triggered()
{
	XETODO("Check returns");

	m_EditorUI.m_GameAssetsTree->RefreshGameAssetsTree();

	m_EditorUI.m_RawGameAssetsTree->RefreshRawGameAssetsTree();

	m_EditorUI.m_GameObjectsTree->RefreshGameObjectsTree();
}

/***************
*   Tool Bar   *
****************/
void XEngine_Editor::on_m_PlayAction_triggered()
{
	XETODO("Check return and log error");
	m_XEngineViewerWidget->GetEngineViewer()->EditorPlay();
}

void XEngine_Editor::on_m_PauseAction_triggered()
{
	XETODO("Check return and log error");
	m_XEngineViewerWidget->GetEngineViewer()->EditorPause();
}

void XEngine_Editor::on_m_StopAction_triggered()
{
	XETODO("Check return and log error");
	m_XEngineViewerWidget->GetEngineViewer()->EditorStop();
}

void XEngine_Editor::on_m_StartPhysXDebuggerAction_triggered()
{
	PhysicsManager* physicsManager = m_XEngineViewerWidget->GetEngineViewer()->GetPhysicsManager();

	XEResult ret = physicsManager->ConnectToPhysXDebugger();
	if (ret != XEResult::Ok)
	{
		XETODO("Set message box error");
	}
}

void XEngine_Editor::on_m_StopPhysXDebuggerAction_triggered()
{
	PhysicsManager* physicsManager = m_XEngineViewerWidget->GetEngineViewer()->GetPhysicsManager();

	XEResult ret = physicsManager->DisconnectToPhysXDebugger();
	if (ret != XEResult::Ok)
	{
		XETODO("Set message box error");
	}
}

/**************
*   Project   *
***************/

void XEngine_Editor::on_m_MenuFileCreateProjectAction_triggered()
{
	////////////////////////////////////////
	//Get Game Engine Viewer
	EngineViewer* engineViewer = m_XEngineViewerWidget->GetEngineViewer();

	NewProjectDialog newProject(engineViewer);

	int result = newProject.exec();
	if (result != QDialog::Accepted)
	{
		return;
	}

	const std::wstring& configFile = newProject.GetConfigFile();

	XETODO("Check return");
	m_XEngineViewerWidget->StartEngineViewerThread(configFile);
}

void XEngine_Editor::on_m_MenuFileSaveProjectAction_triggered()
{
	XEResult ret = m_XEngineViewerWidget->GetEngineViewer()->SaveGameInfo();

	XETODO("Use literals");
	if (ret != XEResult::Ok)
	{
		XETODO("Use literal");
		QString msg = tr("Unable to save project");

		QMessageBox::warning(this, tr("Error"), msg);
	}
}

void XEngine_Editor::LoadProject(const std::wstring& projectConfig)
{
	////////////////////////////////////////
	//Get Game Engine Viewer
	EngineViewer* engineViewer = m_XEngineViewerWidget->GetEngineViewer();

	engineViewer->ShutDownGameApp();
}
