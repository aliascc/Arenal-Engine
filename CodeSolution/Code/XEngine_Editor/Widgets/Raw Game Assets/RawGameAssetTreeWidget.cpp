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
#include "qmimedata.h"
#include "qpushbutton.h"

/***************************
*   Game Engine Includes   *
****************************/
#include "Utils\XEQTDefs.h"
#include "GameContentDefs.h"
#include "Base\BaseFunctions.h"
#include "RawGameAssetTreeWidget.h"
#include "GameAssets\RawGameAsset.h"
#include "Engine Viewer\EngineViewer.h"
#include "GameAssets\GameAssetManager.h"

//Always include last
#include "Memory\MemLeaks.h"

/********************
*   Function Defs   *
*********************/
RawGameAssetTreeWidget::RawGameAssetTreeWidget(QWidget* parent)
	: QTreeWidget(parent)
{
}

RawGameAssetTreeWidget::~RawGameAssetTreeWidget()
{
}

void RawGameAssetTreeWidget::InitFields()
{
	////////////////////////////////////////
	//Raw Game Assets Tree Type
	XEQTUserTemplateData<XEQTObjType>* RawGameAssetsTreeType = new XEQTUserTemplateData<XEQTObjType>(XEQTObjType::RawGameAssetsTree);
	this->setUserData(XE_QT_USER_DATA_OBJ_TYPE_SLOT, RawGameAssetsTreeType);

	///////////////////////////////////////////
	//Set preferred indentation
	this->setIndentation(XE_QT_TREE_INDENTATION);

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

XEResult RawGameAssetTreeWidget::RefreshRawGameAssetsTree()
{
	if(!m_IsReady)
	{
		return XEResult::NotReady;
	}

	XEAssert(m_EngineViewer != nullptr);
	if (m_EngineViewer == nullptr)
	{
		XETODO("Add new error GameAppNull");
		return XEResult::NullObj;
	}

	this->clear();

	////////////////////////////////////////////
	//Get Scope Lock and lock Game App Loop
	GameAppScopedLock appLock = m_EngineViewer->GetGameAppScopedLock();
	appLock.StartLock();

	const RawGameAssetMap& rawAssetMap = m_EngineViewer->GetGameAssetManager()->GetRawGameAssetMap();

	for(auto rawAssetIt : rawAssetMap)
	{
		XETODO("check return");
		CreateRawGameAssetBranch(rawAssetIt.second);
	}

	return XEResult::Ok;
}

XEResult RawGameAssetTreeWidget::CreateRawGameAssetBranch(RawGameAsset* rawAsset)
{
	////////////////////////////////////////////
	//No need to lock this function as it is internal
	//and calling function should lock it
	////////////////////////////////////////////

	if(!m_IsReady)
	{
		return XEResult::NotReady;
	}

	XEAssert(m_EngineViewer != nullptr);
	if (m_EngineViewer == nullptr)
	{
		XETODO("Add new error GameAppNull");
		return XEResult::NullObj;
	}

	XEAssert(rawAsset != nullptr);
	if(rawAsset == nullptr)
	{
		return XEResult::NullParameter;
	}

	////////////////////////////////////////
	//Create Leaf for Raw Asset
	QTreeWidgetItem* branch = new QTreeWidgetItem();
	branch->setFirstColumnSpanned(true);
	branch->setData(0, Qt::ItemDataRole::DisplayRole, QString::fromStdWString(rawAsset->GetName()));

	////////////////////////////////////////
	//Add to Main Tree
	this->addTopLevelItem(branch);

	////////////////////////////////////////
	//Create Childs

	////////////////////////////////////////
	//Game Content Sub Type:
	QTreeWidgetItem* gameContentSubTypeChild = new QTreeWidgetItem();
	gameContentSubTypeChild->setData(0, Qt::ItemDataRole::DisplayRole, "GCS");
	gameContentSubTypeChild->setToolTip(0, "Game Content Subtype");

	QString gameContentSubTypeString = QString::fromStdWString(XEGameContentHelpers::GameContentSubtypeToString(rawAsset->GetContentSubtype()));
	gameContentSubTypeChild->setData(1, Qt::ItemDataRole::DisplayRole, gameContentSubTypeString);

	////////////////////////////////////////
	//Add to Branch
	branch->addChild(gameContentSubTypeChild);

	////////////////////////////////////////
	//If Shader or Script set Button to Edit Code
	if(rawAsset->GetContentType() == GameContentType::Shader || rawAsset->GetContentType() == GameContentType::GameObjectScript)
	{
		////////////////////////////////////////
		//Modify Shader/Script File:
		QTreeWidgetItem* editCodeFileChild = new QTreeWidgetItem();
		editCodeFileChild->setData(0, Qt::ItemDataRole::DisplayRole, "Edit Code");

		////////////////////////////////////////
		//Add to Branch
		branch->addChild(editCodeFileChild);

		////////////////////////////////////////
		//Modify Shader/Script File Push Button
		QPushButton* editCodeFile = new QPushButton("Edit");
		XEQTUserTemplateData<uint64_t>* rawIDUserDataEditCode = new XEQTUserTemplateData<uint64_t>(rawAsset->GetUniqueID());
		editCodeFile->setUserData(XE_QT_USER_DATA_RAW_ASSET_ID_SLOT, rawIDUserDataEditCode);
		connect(editCodeFile, SIGNAL(clicked()), this, SLOT(EditCodePushButtonEvent()));
		this->setItemWidget(editCodeFileChild, 1, editCodeFile);
	}

	////////////////////////////////////////
	//Import Raw Asset Button
	QTreeWidgetItem* rawAssetImport = new QTreeWidgetItem();
	rawAssetImport->setData(0, Qt::ItemDataRole::DisplayRole, "Import");

	////////////////////////////////////////
	//Add to Branch
	branch->addChild(rawAssetImport);

	////////////////////////////////////////
	//Set Import Raw Asset Push Button
	QPushButton* importRawAssetButton = new QPushButton("Import");
	XEQTUserTemplateData<uint64_t>* rawIDUserDataImport = new XEQTUserTemplateData<uint64_t>(rawAsset->GetUniqueID());
	importRawAssetButton->setUserData(XE_QT_USER_DATA_RAW_ASSET_ID_SLOT, rawIDUserDataImport);
	connect(importRawAssetButton, SIGNAL(clicked()), this, SLOT(ImportSingleRawAssetEvent()));
	this->setItemWidget(rawAssetImport, 1, importRawAssetButton);

	////////////////////////////////////////
	//Finish
	return XEResult::Ok;
}

void RawGameAssetTreeWidget::ImportSingleRawAssetEvent()
{
	if(!m_IsReady)
	{
		return;
	}

	XEAssert(m_EngineViewer != nullptr);
	if (m_EngineViewer == nullptr)
	{
		return;
	}

	QObject* senderObject = QObject::sender();

	XEAssert(senderObject != nullptr);
	if(senderObject == nullptr)
	{
		return;
	}

	XEQTUserTemplateData<uint64_t>* userData = reinterpret_cast<XEQTUserTemplateData<uint64_t>*>(senderObject->userData(XE_QT_USER_DATA_RAW_ASSET_ID_SLOT));

	XEAssert(userData != nullptr);
	if(userData == nullptr)
	{
		return;
	}

	////////////////////////////////////////////
	//Get Scope Lock and lock Game App Loop
	GameAppScopedLock appLock = m_EngineViewer->GetGameAppScopedLock();
	appLock.StartLock();

	XEResult ret = m_EngineViewer->GetGameAssetManager()->ImportRawGameAsset(userData->m_Data);

	if(ret != XEResult::Ok)
	{
		XETODO("set message box error");
	}

	////////////////////////////////////////////
	//Get Unlock Game App Loop and let 
	//handlers for event lock if necessary 
	appLock.EndLock();

	emit RawGameAssetReload(userData->m_Data);
}

void RawGameAssetTreeWidget::EditCodePushButtonEvent()
{
	////////////////////////////////////////////
	//No need to lock this function as it is internal
	//and only opens an editor for the code
	////////////////////////////////////////////

	if(!m_IsReady)
	{
		return;
	}

	XEAssert(m_EngineViewer != nullptr);
	if (m_EngineViewer == nullptr)
	{
		return;
	}

	QObject* senderObject = QObject::sender();

	XEAssert(senderObject != nullptr);
	if(senderObject == nullptr)
	{
		return;
	}

	XEQTUserTemplateData<uint64_t>* userData = reinterpret_cast<XEQTUserTemplateData<uint64_t>*>(senderObject->userData(XE_QT_USER_DATA_RAW_ASSET_ID_SLOT));

	XEAssert(userData != nullptr);
	if(userData == nullptr)
	{
		return;
	}

	RawGameAsset* rawGameAsset = m_EngineViewer->GetGameAssetManager()->GetRawGameAsset(userData->m_Data);

	if(rawGameAsset == nullptr)
	{
		return;
	}

	QString qFileName = QString::fromStdWString(rawGameAsset->GetFilePath());

	emit EditCodeFile(qFileName);
}

void RawGameAssetTreeWidget::ParseDropFileList(const QList<QUrl>& urlList)
{
	QList<QString> fileList;

	for(auto file : urlList)
	{
		QString filename = file.toString().remove(XE_QT_URL_FILE_HEADER);

		fileList.append(filename);
	}

	emit NewDropRawAssetsFiles(fileList);
}

void RawGameAssetTreeWidget::dragEnterEvent(QDragEnterEvent* enterEvent)
{
	enterEvent->accept();
}

void RawGameAssetTreeWidget::dragLeaveEvent(QDragLeaveEvent* leaveEvent)
{
	leaveEvent->accept();
}

void RawGameAssetTreeWidget::dragMoveEvent(QDragMoveEvent* moveEvent)
{
	DragDropType type = XEQTHelpers::GetDragDropType(moveEvent);

	if(type == DragDropType::File)
	{
		moveEvent->accept();

		return;
	}

	moveEvent->ignore();
}

void RawGameAssetTreeWidget::dropEvent(QDropEvent* dropEvent)
{
	DragDropType type = XEQTHelpers::GetDragDropType(dropEvent);

	if(type == DragDropType::File)
	{
		ParseDropFileList(dropEvent->mimeData()->urls());

		dropEvent->accept();

		return;
	}

	dropEvent->ignore();
}
