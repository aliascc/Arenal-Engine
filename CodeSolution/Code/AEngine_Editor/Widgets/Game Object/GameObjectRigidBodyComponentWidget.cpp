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
#include "Base\BaseFunctions.h"
#include "GameObject\GameObject.h"
#include "Engine Viewer\EngineViewer.h"
#include "GameObject\Components\PhysicsGOC.h"
#include "GameObjectRigidBodyComponentWidget.h"

//Always include last
#include "Memory\MemLeaks.h"

/********************
*   Function Defs   *
*********************/
GameObjectRigidBodyComponentWidget::GameObjectRigidBodyComponentWidget(GameObject* gameObject, QWidget *parent)
    : QWidget(parent)
    , m_GameObject(gameObject)
{
    m_GameObjectRigidBodyComponentWidgetQtUI.setupUi(this);

    AEAssert(m_GameObject != nullptr);

    InitFields();
}

GameObjectRigidBodyComponentWidget::~GameObjectRigidBodyComponentWidget()
{
}

void GameObjectRigidBodyComponentWidget::InitFields()
{
    /////////////////////////////
    //Pre-check
    AEAssert(m_GameObject != nullptr);
    if (m_GameObject == nullptr)
    {
        return;
    }

    /////////////////////////////
    //Check that we have a Physics Actor and that is a dynamic body
    //and has been initialized
    AEAssert(m_GameObject->HasPhysicsGOC());
    if (!m_GameObject->HasPhysicsGOC())
    {
        return;
    }

    if (!m_GameObject->GetPhysicsGOC()->IsRigidBody())
    {
        return;
    }

    PhysicsActor* physicsActor = m_GameObject->GetPhysicsGOC()->GetPhysicsActor();
    if (physicsActor == nullptr)
    {
        return;
    }

    /////////////////////////////
    //Set variable types
    m_GameObjectRigidBodyComponentWidgetQtUI.m_MassSB->setValue(physicsActor->GetMass());
    m_GameObjectRigidBodyComponentWidgetQtUI.m_AngularDampingSB->setValue(physicsActor->GetAngularDamping());
    m_GameObjectRigidBodyComponentWidgetQtUI.m_LinearDampingSB->setValue(physicsActor->GetLinearDamping());
    m_GameObjectRigidBodyComponentWidgetQtUI.m_GravityEnabledCB->setChecked(physicsActor->GetGravityEnable());
    m_GameObjectRigidBodyComponentWidgetQtUI.m_IsKinematicCB->setChecked(physicsActor->GetKinematic());

    /////////////////////////////
    //Finish
    m_IsReady = true;
}

void GameObjectRigidBodyComponentWidget::on_m_MassSB_valueChanged(double newValue)
{
    if (!m_IsReady)
    {
        return;
    }

    PhysicsActor* physicsActor = m_GameObject->GetPhysicsGOC()->GetPhysicsActor();

    float value = m_GameObjectRigidBodyComponentWidgetQtUI.m_MassSB->value();

    physicsActor->SetMass(value);
}

void GameObjectRigidBodyComponentWidget::on_m_LinearDampingSB_valueChanged(double newValue)
{
    if (!m_IsReady)
    {
        return;
    }

    PhysicsActor* physicsActor = m_GameObject->GetPhysicsGOC()->GetPhysicsActor();

    float value = m_GameObjectRigidBodyComponentWidgetQtUI.m_LinearDampingSB->value();

    physicsActor->SetLinearDamping(value);
}

void GameObjectRigidBodyComponentWidget::on_m_AngularDampingSB_valueChanged(double newValue)
{
    if (!m_IsReady)
    {
        return;
    }

    PhysicsActor* physicsActor = m_GameObject->GetPhysicsGOC()->GetPhysicsActor();

    float value = m_GameObjectRigidBodyComponentWidgetQtUI.m_AngularDampingSB->value();

    physicsActor->SetAngularDamping(value);
}

void GameObjectRigidBodyComponentWidget::on_m_GravityEnabledCB_stateChanged(int newState)
{
    if (!m_IsReady)
    {
        return;
    }

    PhysicsActor* physicsActor = m_GameObject->GetPhysicsGOC()->GetPhysicsActor();

    bool value = m_GameObjectRigidBodyComponentWidgetQtUI.m_GravityEnabledCB->isChecked();

    physicsActor->SetGravityEnable(value);
}

void GameObjectRigidBodyComponentWidget::on_m_IsKinematicCB_stateChanged(int newState)
{
    if (!m_IsReady)
    {
        return;
    }

    PhysicsActor* physicsActor = m_GameObject->GetPhysicsGOC()->GetPhysicsActor();

    bool value = m_GameObjectRigidBodyComponentWidgetQtUI.m_IsKinematicCB->isChecked();

    physicsActor->SetKinematic(value);
}
