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
#include "PhysicCollider.h"
#include "Base\BaseFunctions.h"
#include "PhysicColliderSphere.h"
#include "GameObject\GameObject.h"
#include "Engine Viewer\EngineViewer.h"
#include "GameObject\Components\PhysicsGOC.h"
#include "GameObjectSphereColliderComponentWidget.h"

//Always include last
#include "Memory\MemLeaks.h"

/********************
*   Function Defs   *
*********************/
GameObjectSphereColliderComponentWidget::GameObjectSphereColliderComponentWidget(GameObject* gameObject, uint64_t physicColliderID, QWidget *parent)
	: QWidget(parent)
	, m_GameObject(gameObject)
	, m_PhysicColliderID(physicColliderID)
{
	m_GameObjectSphereColliderComponentWidgetUI.setupUi(this);

	AEAssert(m_GameObject != nullptr);

	InitFields();
}

GameObjectSphereColliderComponentWidget::~GameObjectSphereColliderComponentWidget()
{
}

void GameObjectSphereColliderComponentWidget::InitFields()
{
	/////////////////////////////
	//Pre-check
	AEAssert(m_GameObject != nullptr);
	if (m_GameObject == nullptr)
	{
		return;
	}

	/////////////////////////////
	//Check that we have a Physics Actor and that is a collider
	//and has been initialized
	AEAssert(m_GameObject->HasPhysicsGOC());
	if (!m_GameObject->HasPhysicsGOC())
	{
		return;
	}

	PhysicsActor* physicsActor = m_GameObject->GetPhysicsGOC()->GetPhysicsActor();
	if (physicsActor == nullptr)
	{
		return;
	}

	if (!physicsActor->IsReady())
	{
		return;
	}

	/////////////////////////////
	//Get Physic Collider
	PhysicCollider* physicCollider = physicsActor->GetPhysicCollider(m_PhysicColliderID);
	if (physicCollider == nullptr)
	{
		return;
	}

	if (physicCollider->GetCollisionShape() != CollisionShape::Sphere)
	{
		return;
	}

	PhysicColliderSphere* physicColliderSphere = dynamic_cast<PhysicColliderSphere*>(physicCollider);

	/////////////////////////////
	//Set variable types
	float radius = physicColliderSphere->GetRadius();

	m_GameObjectSphereColliderComponentWidgetUI.m_RadiusSB->setValue(radius);

	/////////////////////////////
	//Finish
	m_IsReady = true;
}

void GameObjectSphereColliderComponentWidget::on_m_RadiusSB_valueChanged(double newValue)
{
	if (!m_IsReady)
	{
		return;
	}

	PhysicsActor* physicsActor = m_GameObject->GetPhysicsGOC()->GetPhysicsActor();
	PhysicCollider* physicCollider = physicsActor->GetPhysicCollider(m_PhysicColliderID);
	PhysicColliderSphere* physicColliderSphere = dynamic_cast<PhysicColliderSphere*>(physicCollider);

	float radius = m_GameObjectSphereColliderComponentWidgetUI.m_RadiusSB->value();

	physicColliderSphere->SetRadius(radius);
}
