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
#include "PhysicColliderBox.h"
#include "Base\BaseFunctions.h"
#include "GameObject\GameObject.h"
#include "Engine Viewer\EngineViewer.h"
#include "GameObject\Components\PhysicsGOC.h"
#include "GameObjectBoxColliderComponentWidget.h"

//Always include last
#include "Memory\MemLeaks.h"

/********************
*   Function Defs   *
*********************/
GameObjectBoxColliderComponentWidget::GameObjectBoxColliderComponentWidget(GameObject* gameObject, uint64_t physicColliderID, QWidget *parent)
    : QWidget(parent)
    , m_GameObject(gameObject)
    , m_PhysicColliderID(physicColliderID)
{
    m_GameObjectBoxColliderComponentWidgetQtUI.setupUi(this);

    AEAssert(m_GameObject != nullptr);

    InitFields();
}

GameObjectBoxColliderComponentWidget::~GameObjectBoxColliderComponentWidget()
{
}

void GameObjectBoxColliderComponentWidget::InitFields()
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

    if (physicCollider->GetCollisionShape() != CollisionShape::Box)
    {
        return;
    }

    PhysicColliderBox* physicColliderBox = dynamic_cast<PhysicColliderBox*>(physicCollider);

    /////////////////////////////
    //Set variable types
    const glm::vec3& size = physicColliderBox->GetSize();

    m_GameObjectBoxColliderComponentWidgetQtUI.m_SizeXSB->setValue(size.x);
    m_GameObjectBoxColliderComponentWidgetQtUI.m_SizeYSB->setValue(size.y);
    m_GameObjectBoxColliderComponentWidgetQtUI.m_SizeZSB->setValue(size.z);

    /////////////////////////////
    //Finish
    m_IsReady = true;
}

void GameObjectBoxColliderComponentWidget::on_m_SizeXSB_valueChanged(double newValue)
{
    if (!m_IsReady)
    {
        return;
    }

    PhysicsActor* physicsActor = m_GameObject->GetPhysicsGOC()->GetPhysicsActor();
    PhysicCollider* physicCollider = physicsActor->GetPhysicCollider(m_PhysicColliderID);
    PhysicColliderBox* physicColliderBox = dynamic_cast<PhysicColliderBox*>(physicCollider);

    glm::vec3 size = physicColliderBox->GetSize();
    size.x = m_GameObjectBoxColliderComponentWidgetQtUI.m_SizeXSB->value();

    physicColliderBox->SetSize(size);
}

void GameObjectBoxColliderComponentWidget::on_m_SizeYSB_valueChanged(double newValue)
{
    if (!m_IsReady)
    {
        return;
    }

    PhysicsActor* physicsActor = m_GameObject->GetPhysicsGOC()->GetPhysicsActor();
    PhysicCollider* physicCollider = physicsActor->GetPhysicCollider(m_PhysicColliderID);
    PhysicColliderBox* physicColliderBox = dynamic_cast<PhysicColliderBox*>(physicCollider);

    glm::vec3 size = physicColliderBox->GetSize();
    size.y = m_GameObjectBoxColliderComponentWidgetQtUI.m_SizeYSB->value();

    physicColliderBox->SetSize(size);
}

void GameObjectBoxColliderComponentWidget::on_m_SizeZSB_valueChanged(double newValue)
{
    if (!m_IsReady)
    {
        return;
    }

    PhysicsActor* physicsActor = m_GameObject->GetPhysicsGOC()->GetPhysicsActor();
    PhysicCollider* physicCollider = physicsActor->GetPhysicCollider(m_PhysicColliderID);
    PhysicColliderBox* physicColliderBox = dynamic_cast<PhysicColliderBox*>(physicCollider);

    glm::vec3 size = physicColliderBox->GetSize();
    size.z = m_GameObjectBoxColliderComponentWidgetQtUI.m_SizeZSB->value();

    physicColliderBox->SetSize(size);
}
