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
#include "PhysicsActor.h"
#include "Utils\AEQTDefs.h"
#include "GameContentDefs.h"
#include "PhysicColliderBox.h"
#include "Base\BaseFunctions.h"
#include "PhysicColliderSphere.h"
#include "GameObject\GameObject.h"
#include "GameObjectPropsTreeWidget.h"
#include "Engine Viewer\EngineViewer.h"
#include "GameObject\Components\PhysicsGOC.h"
#include "Widgets\Game Object\GameObjectMeshComponentWidget.h"
#include "Widgets\Game Object\GameObjectTransformationWidget.h"
#include "Widgets\Game Object\GameObjectLightComponentWidget.h"
#include "Widgets\Game Object\GameObjectCameraComponentWidget.h"
#include "Widgets\Game Object\GameObjectBaseInformationWidget.h"
#include "Widgets\Game Object\GameObjectScriptComponentWidget.h"
#include "Widgets\Game Object\GameObjectMaterialComponentsWidget.h"
#include "Widgets\Game Object\GameObjectRigidBodyComponentWidget.h"
#include "Widgets\Game Object\GameObjectAudioSourceComponentWidget.h"
#include "Widgets\Game Object\GameObjectBoxColliderComponentWidget.h"
#include "Widgets\Game Object\GameObjectAudioListenerComponentWidget.h"
#include "Widgets\Game Object\GameObjectMeshAnimationComponentWidget.h"
#include "Widgets\Game Object\GameObjectSphereColliderComponentWidget.h"

//Always include last
#include "Memory\MemLeaks.h"

/********************
*   Function Defs   *
*********************/
AETODO("Finish adding mutex for gameapp");
GameObjectPropsTreeWidget::GameObjectPropsTreeWidget(QWidget* parent)
	: QTreeWidget(parent)
{
}

GameObjectPropsTreeWidget::~GameObjectPropsTreeWidget()
{
}

void GameObjectPropsTreeWidget::InitFields()
{
	////////////////////////////////////////
	//Game Objects Tree Type
	AEQTUserTemplateData<AEQTObjType>* GameObjectsTreeType = new AEQTUserTemplateData<AEQTObjType>(AEQTObjType::GameObjectsPropsTree);
	this->setUserData(AE_QT_USER_DATA_OBJ_TYPE_SLOT, GameObjectsTreeType);

	///////////////////////////////////////////
	//Set preferred indentation
	this->setIndentation(AE_QT_TREE_GAME_OBJECT_PROPS_INDENTATION);

	///////////////////////////////////////////
	//Set Animation
	this->setAnimated(false);

	///////////////////////////////////////////
	//Set Alternating Row Colors
	this->setAlternatingRowColors(false);

	///////////////////////////////////////////
	//Finish
	m_IsReady = true;
}

AEResult GameObjectPropsTreeWidget::DisplayGameObjectInfo(GameObject* gameObject)
{
	if(!m_IsReady)
	{
		return AEResult::NotReady;
	}

	AEAssert(gameObject != nullptr);
	if(gameObject == nullptr)
	{
		return AEResult::NullParameter;
	}

	m_CurrentGameObject = gameObject;

	return ReloadGameObjectInfo();
}

AEResult GameObjectPropsTreeWidget::ReloadGameObjectInfo()
{
	if (!m_IsReady)
	{
		return AEResult::NotReady;
	}

	if (m_CurrentGameObject == nullptr)
	{
		return AEResult::Ok;
	}

	AEAssert(m_EngineViewer != nullptr);
	if (m_EngineViewer == nullptr)
	{
		AETODO("Add new error GameAppNull");
		return AEResult::NullObj;
	}

	int minSectionSize = 0;

	////////////////////////////////////////
	//Clear Game Object Properties
	this->clear();

	////////////////////////////////////////
	//Create Base Information Branch and Child
	QTreeWidgetItem* baseInfoBranch = new QTreeWidgetItem();
	baseInfoBranch->setData(0, Qt::ItemDataRole::DisplayRole, "Base Information");
	baseInfoBranch->setToolTip(0, "Basic Properties (Double Click to expand)");
	this->addTopLevelItem(baseInfoBranch);

	QTreeWidgetItem* baseInfoBranchChild = new QTreeWidgetItem();
	baseInfoBranch->addChild(baseInfoBranchChild);

	////////////////////////////////////////
	//Add Base Information Widget
	GameObjectBaseInformationWidget* baseInfoWidget = new GameObjectBaseInformationWidget(m_CurrentGameObject, m_EngineViewer);
	connect(baseInfoWidget, SIGNAL(NameChanged(uint64_t)), this, SLOT(GameObjectChangedNameEvent(uint64_t)));
	this->setItemWidget(baseInfoBranchChild, 0, baseInfoWidget);
	minSectionSize = (minSectionSize < baseInfoWidget->size().width()) ? baseInfoWidget->size().width() : minSectionSize;

	////////////////////////////////////////
	//Create Transformation Branch and Child
	QTreeWidgetItem* transBranch = new QTreeWidgetItem();
	transBranch->setData(0, Qt::ItemDataRole::DisplayRole, "Transformation");
	transBranch->setToolTip(0, "Transformation Properties (Double Click to expand)");
	this->addTopLevelItem(transBranch);

	QTreeWidgetItem* transBranchChild = new QTreeWidgetItem();
	transBranch->addChild(transBranchChild);

	////////////////////////////////////////
	//Add Transformation Widget
	GameObjectTransformationWidget* transWidget = new GameObjectTransformationWidget(m_CurrentGameObject);
	this->setItemWidget(transBranchChild, 0, transWidget);
	minSectionSize = (minSectionSize < transWidget->size().width()) ? transWidget->size().width() : minSectionSize;

	////////////////////////////////////////
	//Check if Game Object has Camera GOC
	if (m_CurrentGameObject->HasCameraGOC())
	{
		////////////////////////////////////////
		//Create Camera GOC Branch and Child
		QTreeWidgetItem* cameraGOCBranch = new QTreeWidgetItem();
		cameraGOCBranch->setData(0, Qt::ItemDataRole::DisplayRole, "Camera Game Object Component");
		cameraGOCBranch->setToolTip(0, "Camera Game Object Component (Double Click to expand)");
		this->addTopLevelItem(cameraGOCBranch);

		QTreeWidgetItem* cameraGOCBranchChild = new QTreeWidgetItem();
		cameraGOCBranch->addChild(cameraGOCBranchChild);

		////////////////////////////////////////
		//Add Camera GOC Widget
		GameObjectCameraComponentWidget* cameraGOCWidget = new GameObjectCameraComponentWidget(m_CurrentGameObject, m_EngineViewer);
		this->setItemWidget(cameraGOCBranchChild, 0, cameraGOCWidget);
		minSectionSize = (minSectionSize < cameraGOCWidget->size().width()) ? cameraGOCWidget->size().width() : minSectionSize;
	}

	////////////////////////////////////////
	//Check if Game Object has Mesh GOC
	if (m_CurrentGameObject->HasMeshGOC())
	{
		////////////////////////////////////////
		//Create Mesh GOC Branch and Child
		QTreeWidgetItem* meshGOCBranch = new QTreeWidgetItem();
		meshGOCBranch->setData(0, Qt::ItemDataRole::DisplayRole, "Mesh Game Object Component");
		meshGOCBranch->setToolTip(0, "Mesh Game Object Component (Double Click to expand)");
		this->addTopLevelItem(meshGOCBranch);

		QTreeWidgetItem* meshGOCBranchChild = new QTreeWidgetItem();
		meshGOCBranch->addChild(meshGOCBranchChild);

		////////////////////////////////////////
		//Add Mesh GOC Widget
		AETODO("Change Asset manager for engine viewer");
		GameObjectMeshComponentWidget* meshGOCWidget = new GameObjectMeshComponentWidget(m_CurrentGameObject, m_EngineViewer->GetGameAssetManager());
		this->setItemWidget(meshGOCBranchChild, 0, meshGOCWidget);
		minSectionSize = (minSectionSize < meshGOCWidget->size().width()) ? meshGOCWidget->size().width() : minSectionSize;
	}

	////////////////////////////////////////
	//Check if Game Object has any Material GOC
	if (m_CurrentGameObject->HasMaterialGOCs())
	{
		////////////////////////////////////////
		//Create Material GOC Branch and Child
		QTreeWidgetItem* materialGOCBranch = new QTreeWidgetItem();
		materialGOCBranch->setData(0, Qt::ItemDataRole::DisplayRole, "Material Game Object Components");
		materialGOCBranch->setToolTip(0, "Material Game Object Components (Double Click to expand)");
		this->addTopLevelItem(materialGOCBranch);

		QTreeWidgetItem* materialGOCBranchChild = new QTreeWidgetItem();
		materialGOCBranch->addChild(materialGOCBranchChild);

		////////////////////////////////////////
		//Add Material GOC Widget
		AETODO("Change Asset manager for engine viewer");
		GameObjectMaterialComponentsWidget* materialGOCWidget = new GameObjectMaterialComponentsWidget(m_CurrentGameObject, m_EngineViewer);
		this->setItemWidget(materialGOCBranchChild, 0, materialGOCWidget);
		minSectionSize = (minSectionSize < materialGOCWidget->size().width()) ? materialGOCWidget->size().width() : minSectionSize;
	}

	////////////////////////////////////////
	//Check if Game Object has Mesh Anim GOC
	if (m_CurrentGameObject->HasMeshAnimationGOC())
	{
		////////////////////////////////////////
		//Create Mesh Anim GOC Branch and Child
		QTreeWidgetItem* meshAnimGOCBranch = new QTreeWidgetItem();
		meshAnimGOCBranch->setData(0, Qt::ItemDataRole::DisplayRole, "Mesh Animation Game Object Component");
		meshAnimGOCBranch->setToolTip(0, "Mesh Animation Game Object Component (Double Click to expand)");
		this->addTopLevelItem(meshAnimGOCBranch);

		QTreeWidgetItem* meshAnimGOCBranchChild = new QTreeWidgetItem();
		meshAnimGOCBranch->addChild(meshAnimGOCBranchChild);

		////////////////////////////////////////
		//Add Mesh Animation GOC Widget
		GameObjectMeshAnimationComponentWidget* meshAnimGOCWidget = new GameObjectMeshAnimationComponentWidget(m_CurrentGameObject, m_EngineViewer);
		this->setItemWidget(meshAnimGOCBranchChild, 0, meshAnimGOCWidget);
		minSectionSize = (minSectionSize < meshAnimGOCWidget->size().width()) ? meshAnimGOCWidget->size().width() : minSectionSize;
	}

	////////////////////////////////////////
	//Check if Game Object has Light GOC
	if (m_CurrentGameObject->HasLightGOC())
	{
		////////////////////////////////////////
		//Create Light GOC Branch and Child
		QTreeWidgetItem* lightGOCBranch = new QTreeWidgetItem();
		lightGOCBranch->setData(0, Qt::ItemDataRole::DisplayRole, "Light Game Object Component");
		lightGOCBranch->setToolTip(0, "Light Game Object Component (Double Click to expand)");
		this->addTopLevelItem(lightGOCBranch);

		QTreeWidgetItem* lightGOCBranchChild = new QTreeWidgetItem();
		lightGOCBranch->addChild(lightGOCBranchChild);

		////////////////////////////////////////
		//Add Light GOC Widget
		GameObjectLightComponentWidget* lightGOCWidget = new GameObjectLightComponentWidget(m_CurrentGameObject, m_EngineViewer);
		this->setItemWidget(lightGOCBranchChild, 0, lightGOCWidget);
		minSectionSize = (minSectionSize < lightGOCWidget->size().width()) ? lightGOCWidget->size().width() : minSectionSize;
	}

	////////////////////////////////////////
	//Check if Game Object has Script GOCs
	if (m_CurrentGameObject->HasGameObjectScriptGOCs())
	{
		////////////////////////////////////////
		//Create Game Object GOC Branch and Child
		QTreeWidgetItem* gameObjectScriptGOCBranch = new QTreeWidgetItem();
		gameObjectScriptGOCBranch->setData(0, Qt::ItemDataRole::DisplayRole, "Game Object Script Game Object Components");
		gameObjectScriptGOCBranch->setToolTip(0, "Game Object Script Game Object Components(Double Click to expand)");
		this->addTopLevelItem(gameObjectScriptGOCBranch);

		QTreeWidgetItem* gameObjectScriptGOCBranchChild = new QTreeWidgetItem();
		gameObjectScriptGOCBranch->addChild(gameObjectScriptGOCBranchChild);

		////////////////////////////////////////
		//Add Game Object GOC Widget
		AETODO("Change Asset manager for engine viewer");
		GameObjectScriptComponentWidget* gameObjectScriptGOCWidget = new GameObjectScriptComponentWidget(m_CurrentGameObject, m_EngineViewer);
		this->setItemWidget(gameObjectScriptGOCBranchChild, 0, gameObjectScriptGOCWidget);
		minSectionSize = (minSectionSize < gameObjectScriptGOCWidget->size().width()) ? gameObjectScriptGOCWidget->size().width() : minSectionSize;
	}

	////////////////////////////////////////
	//Check if Game Object has Audio Listener GOC
	if (m_CurrentGameObject->HasAudioListenerGOC())
	{
		////////////////////////////////////////
		//Create Game Object GOC Branch and Child
		QTreeWidgetItem* audioListenerGOCBranch = new QTreeWidgetItem();
		audioListenerGOCBranch->setData(0, Qt::ItemDataRole::DisplayRole, "Audio Listener Game Object Components");
		audioListenerGOCBranch->setToolTip(0, "Audio Listener Game Object Components (Double Click to expand)");
		this->addTopLevelItem(audioListenerGOCBranch);

		QTreeWidgetItem* audioListenerGOCBranchChild = new QTreeWidgetItem();
		audioListenerGOCBranch->addChild(audioListenerGOCBranchChild);

		////////////////////////////////////////
		//Add Game Object GOC Widget
		AETODO("Change Asset manager for engine viewer");
		GameObjectAudioListenerComponentWidget* audioListenerGOCWidget = new GameObjectAudioListenerComponentWidget(m_CurrentGameObject);
		this->setItemWidget(audioListenerGOCBranchChild, 0, audioListenerGOCWidget);
		minSectionSize = (minSectionSize < audioListenerGOCWidget->size().width()) ? audioListenerGOCWidget->size().width() : minSectionSize;
	}

	////////////////////////////////////////
	//Check if Game Object has Audio Source GOC
	if (m_CurrentGameObject->HasAudioSourceGOCs())
	{
		////////////////////////////////////////
		//Create Game Object GOC Branch and Child
		QTreeWidgetItem* audioSourceGOCBranch = new QTreeWidgetItem();
		audioSourceGOCBranch->setData(0, Qt::ItemDataRole::DisplayRole, "Audio Source Game Object Components");
		audioSourceGOCBranch->setToolTip(0, "Audio Source Game Object Components (Double Click to expand)");
		this->addTopLevelItem(audioSourceGOCBranch);

		QTreeWidgetItem* audioSourceGOCBranchChild = new QTreeWidgetItem();
		audioSourceGOCBranch->addChild(audioSourceGOCBranchChild);

		////////////////////////////////////////
		//Add Game Object GOC Widget
		GameObjectAudioSourceComponentWidget* audioSourceGOCWidget = new GameObjectAudioSourceComponentWidget(m_CurrentGameObject, m_EngineViewer);
		this->setItemWidget(audioSourceGOCBranchChild, 0, audioSourceGOCWidget);
		minSectionSize = (minSectionSize < audioSourceGOCWidget->size().width()) ? audioSourceGOCWidget->size().width() : minSectionSize;
	}

	////////////////////////////////////////
	//Check if Game Object has Physics GOC
	if (m_CurrentGameObject->HasPhysicsGOC())
	{
		PhysicsGOC* physicsGOC = m_CurrentGameObject->GetPhysicsGOC();
		if (physicsGOC->IsRigidBody())
		{
			////////////////////////////////////////
			//Create Game Object GOC Branch and Child
			QTreeWidgetItem* rigidBodyBranch = new QTreeWidgetItem();
			rigidBodyBranch->setData(0, Qt::ItemDataRole::DisplayRole, "Rigid Body Game Object Components");
			rigidBodyBranch->setToolTip(0, "Rigid Body Game Object Components (Double Click to expand)");
			this->addTopLevelItem(rigidBodyBranch);

			QTreeWidgetItem* rigidBodyBranchChild = new QTreeWidgetItem();
			rigidBodyBranch->addChild(rigidBodyBranchChild);

			////////////////////////////////////////
			//Add Game Object GOC Widget
			GameObjectRigidBodyComponentWidget* rigidBodyWidget = new GameObjectRigidBodyComponentWidget(m_CurrentGameObject);
			this->setItemWidget(rigidBodyBranchChild, 0, rigidBodyWidget);
			minSectionSize = (minSectionSize < rigidBodyWidget->size().width()) ? rigidBodyWidget->size().width() : minSectionSize;
		}

		PhysicsActor* physicsActor = m_CurrentGameObject->GetPhysicsGOC()->GetPhysicsActor();
		if (physicsActor != nullptr)
		{
			const PhysicColliderMap& physicColliderMap = physicsActor->GetPhysicColliderMap();
			QWidget* colliderWidget = nullptr;

			for (auto physicColliderPair : physicColliderMap)
			{
				PhysicCollider* collider = physicColliderPair.second;
				uint64_t id = collider->GetUniqueID();

				std::string title		= " Game Object Components";
				std::string titleTip	= " Game Object Components (Double Click to expand)";

				switch (collider->GetCollisionShape())
				{
					case CollisionShape::Box:
						title			= "Box Collider" + title;
						titleTip		= "Box Collider" + titleTip;
						colliderWidget	= new GameObjectBoxColliderComponentWidget(m_CurrentGameObject, id);
						break;

					case CollisionShape::Sphere:
						title			= "Sphere Collider" + title;
						titleTip		= "Sphere Collider" + titleTip;
						colliderWidget	= new GameObjectSphereColliderComponentWidget(m_CurrentGameObject, id);
						break;

					default:
						AEAssert(false);
						continue;
						break;
				}

				////////////////////////////////////////
				//Create Game Object GOC Branch and Child
				QTreeWidgetItem* colliderBranch = new QTreeWidgetItem();
				colliderBranch->setData(0, Qt::ItemDataRole::DisplayRole, title.c_str());
				colliderBranch->setToolTip(0, titleTip.c_str());
				this->addTopLevelItem(colliderBranch);

				QTreeWidgetItem* colliderBranchChild = new QTreeWidgetItem();
				colliderBranch->addChild(colliderBranchChild);

				////////////////////////////////////////
				//Add Game Object GOC Widget
				this->setItemWidget(colliderBranchChild, 0, colliderWidget);
				minSectionSize = (minSectionSize < colliderWidget->size().width()) ? colliderWidget->size().width() : minSectionSize;
			}
		}
	}

	////////////////////////////////////////
	//Expand all items
	this->expandAll();

	////////////////////////////////////////
	//Set Minimum Section Size as Max Size of Largest Widget
	this->header()->setMinimumSectionSize(minSectionSize);

	////////////////////////////////////////
	//Finish
	return AEResult::Ok;
}

void GameObjectPropsTreeWidget::GameObjectChangedNameEvent(uint64_t gameObjectID)
{
	emit GameObjectChangedName(gameObjectID);
}
