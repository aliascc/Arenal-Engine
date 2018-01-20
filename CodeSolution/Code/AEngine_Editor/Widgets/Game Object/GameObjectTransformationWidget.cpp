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
#include "Utils\AEQTDefs.h"
#include "GameObject\GameObject.h"
#include "GameObjectTransformationWidget.h"

//Always include last
#include "Memory\MemLeaks.h"

/********************
*   Function Defs   *
*********************/
GameObjectTransformationWidget::GameObjectTransformationWidget(GameObject* gameObject, QWidget *parent)
    : QWidget(parent)
    , m_GameObject(gameObject)
{
    m_GameObjectTransformationWidgetQtUI.setupUi(this);

    AEAssert(gameObject != nullptr);

    InitFields();
}

GameObjectTransformationWidget::~GameObjectTransformationWidget()
{
    m_GameObject->RemoveObject3DChangeEventCallback(m_UniqueCallbackID);
}

void GameObjectTransformationWidget::InitFields()
{
    AEAssert(m_GameObject != nullptr);
    if(m_GameObject == nullptr)
    {
        return;
    }

    ////////////////////////////////////////
    // Set Position
    const glm::vec3& pos = m_GameObject->GetPosition();
    m_GameObjectTransformationWidgetQtUI.m_PosX->setValue(static_cast<double>(pos.x));
    m_GameObjectTransformationWidgetQtUI.m_PosY->setValue(static_cast<double>(pos.y));
    m_GameObjectTransformationWidgetQtUI.m_PosZ->setValue(static_cast<double>(pos.z));

    ////////////////////////////////////////
    // Set Rotation
    const glm::vec3& rot = m_GameObject->GetRotation();
    m_GameObjectTransformationWidgetQtUI.m_RotX->setValue(static_cast<double>(rot.x));
    m_GameObjectTransformationWidgetQtUI.m_RotY->setValue(static_cast<double>(rot.y));
    m_GameObjectTransformationWidgetQtUI.m_RotZ->setValue(static_cast<double>(rot.z));

    ////////////////////////////////////////
    // Set Scale
    const glm::vec3& scl = m_GameObject->GetScale();
    m_GameObjectTransformationWidgetQtUI.m_SclX->setValue(static_cast<double>(scl.x));
    m_GameObjectTransformationWidgetQtUI.m_SclY->setValue(static_cast<double>(scl.y));
    m_GameObjectTransformationWidgetQtUI.m_SclZ->setValue(static_cast<double>(scl.z));


    ////////////////////////////////////////
    // Set Callback for GameObject Change
    m_UniqueCallbackID = AE_Base::GetNextUniqueID();
    Object3DChangedEvent changeEvent = std::bind(&GameObjectTransformationWidget::GameObject3DChanged, this, std::placeholders::_1, std::placeholders::_2);

    AEResult ret = m_GameObject->AddObject3DChangeEventCallback(m_UniqueCallbackID, changeEvent);
    if (ret != AEResult::Ok)
    {
        AEAssert(false);
        AETODO("Log critical error");
    }

    connect(this, SIGNAL(GameObject3DChangedSignal(Object3DChangeEventType)), this, SLOT(GameObject3DChangedSlot(Object3DChangeEventType)));
}

void GameObjectTransformationWidget::GameObject3DChanged(Object3DChangeEventType changeType, Object3D* object3D)
{
    emit (GameObject3DChangedSignal(changeType));
}

void GameObjectTransformationWidget::GameObject3DChangedSlot(Object3DChangeEventType changeType)
{
    switch (changeType)
    {
        ////////////////////////////////////////
        // Set Position
        case Object3DChangeEventType::PositionChange:
            {
                if (m_GameObjectTransformationWidgetQtUI.m_PosX == nullptr ||
                    m_GameObjectTransformationWidgetQtUI.m_PosY == nullptr ||
                    m_GameObjectTransformationWidgetQtUI.m_PosZ == nullptr)
                {
                    break;
                }

                const glm::vec3& pos = m_GameObject->GetPosition();
                WigetSignalBlocker<QDoubleSpinBox>(m_GameObjectTransformationWidgetQtUI.m_PosX)->setValue(static_cast<double>(pos.x));
                WigetSignalBlocker<QDoubleSpinBox>(m_GameObjectTransformationWidgetQtUI.m_PosY)->setValue(static_cast<double>(pos.y));
                WigetSignalBlocker<QDoubleSpinBox>(m_GameObjectTransformationWidgetQtUI.m_PosZ)->setValue(static_cast<double>(pos.z));
            }
            break;

        ////////////////////////////////////////
        // Set Rotation
        case Object3DChangeEventType::RotationChange:
            {
                if (m_GameObjectTransformationWidgetQtUI.m_RotX == nullptr ||
                    m_GameObjectTransformationWidgetQtUI.m_RotY == nullptr ||
                    m_GameObjectTransformationWidgetQtUI.m_RotZ == nullptr)
                {
                    break;
                }

                const glm::vec3& rot = m_GameObject->GetRotation();
                WigetSignalBlocker<QDoubleSpinBox>(m_GameObjectTransformationWidgetQtUI.m_RotX)->setValue(static_cast<double>(rot.x));
                WigetSignalBlocker<QDoubleSpinBox>(m_GameObjectTransformationWidgetQtUI.m_RotY)->setValue(static_cast<double>(rot.y));
                WigetSignalBlocker<QDoubleSpinBox>(m_GameObjectTransformationWidgetQtUI.m_RotZ)->setValue(static_cast<double>(rot.z));
            }
            break;

        ////////////////////////////////////////
        // Set Scale
        case Object3DChangeEventType::ScaleChange:
            {
                if (m_GameObjectTransformationWidgetQtUI.m_SclX == nullptr ||
                    m_GameObjectTransformationWidgetQtUI.m_SclY == nullptr ||
                    m_GameObjectTransformationWidgetQtUI.m_SclZ == nullptr)
                {
                    break;
                }

                const glm::vec3& scl = m_GameObject->GetScale();
                WigetSignalBlocker<QDoubleSpinBox>(m_GameObjectTransformationWidgetQtUI.m_SclX)->setValue(static_cast<double>(scl.x));
                WigetSignalBlocker<QDoubleSpinBox>(m_GameObjectTransformationWidgetQtUI.m_SclY)->setValue(static_cast<double>(scl.y));
                WigetSignalBlocker<QDoubleSpinBox>(m_GameObjectTransformationWidgetQtUI.m_SclZ)->setValue(static_cast<double>(scl.z));
            }
            break;

        default:
            return;
    }
}

void GameObjectTransformationWidget::on_m_PosX_valueChanged(double newValue)
{
    float value = static_cast<float>(m_GameObjectTransformationWidgetQtUI.m_PosX->value());

    m_GameObject->SetPositionX(value, m_UniqueCallbackID);
}

void GameObjectTransformationWidget::on_m_PosY_valueChanged(double newValue)
{
    float value = static_cast<float>(m_GameObjectTransformationWidgetQtUI.m_PosY->value());

    m_GameObject->SetPositionY(value, m_UniqueCallbackID);
}

void GameObjectTransformationWidget::on_m_PosZ_valueChanged(double newValue)
{
    float value = static_cast<float>(m_GameObjectTransformationWidgetQtUI.m_PosZ->value());

    m_GameObject->SetPositionZ(value, m_UniqueCallbackID);
}

void GameObjectTransformationWidget::on_m_RotX_valueChanged(double newValue)
{
    double valueD = m_GameObjectTransformationWidgetQtUI.m_RotX->value();

    if (valueD > 360.0 || valueD < -360.0)
    {
        m_GameObjectTransformationWidgetQtUI.m_RotX->setValue(0.0);

        return;
    }

    float value = static_cast<float>(valueD);

    m_GameObject->SetPitch(value, m_UniqueCallbackID);
}

void GameObjectTransformationWidget::on_m_RotY_valueChanged(double newValue)
{
    double valueD = m_GameObjectTransformationWidgetQtUI.m_RotY->value();

    if (valueD > 360.0 || valueD < -360.0)
    {
        m_GameObjectTransformationWidgetQtUI.m_RotY->setValue(0.0);

        return;
    }

    float value = static_cast<float>(valueD);

    m_GameObject->SetYaw(value, m_UniqueCallbackID);
}

void GameObjectTransformationWidget::on_m_RotZ_valueChanged(double newValue)
{
    double valueD = m_GameObjectTransformationWidgetQtUI.m_RotZ->value();

    if (valueD > 360.0 || valueD < -360.0)
    {
        m_GameObjectTransformationWidgetQtUI.m_RotZ->setValue(0.0);

        return;
    }

    float value = static_cast<float>(valueD);

    m_GameObject->SetRoll(value, m_UniqueCallbackID);
}

void GameObjectTransformationWidget::on_m_SclX_valueChanged(double newValue)
{
    float value = static_cast<float>(m_GameObjectTransformationWidgetQtUI.m_SclX->value());

    m_GameObject->SetScaleX(value, m_UniqueCallbackID);
}

void GameObjectTransformationWidget::on_m_SclY_valueChanged(double newValue)
{
    float value = static_cast<float>(m_GameObjectTransformationWidgetQtUI.m_SclY->value());

    m_GameObject->SetScaleY(value, m_UniqueCallbackID);
}

void GameObjectTransformationWidget::on_m_SclZ_valueChanged(double newValue)
{
    float value = static_cast<float>(m_GameObjectTransformationWidgetQtUI.m_SclZ->value());

    m_GameObject->SetScaleZ(value, m_UniqueCallbackID);
}
