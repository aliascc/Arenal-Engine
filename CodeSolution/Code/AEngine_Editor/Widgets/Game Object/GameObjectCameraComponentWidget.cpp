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
#include "qcolordialog.h"

/***************************
*   Game Engine Includes   *
****************************/
#include "Camera\Camera.h"
#include "Utils\AEQTDefs.h"
#include "Camera\CameraManager.h"
#include "GameObject\GameObject.h"
#include "Engine Viewer\EngineViewer.h"
#include "GameObjectCameraComponentWidget.h"
#include "GameObject\Components\CameraGOC.h"

//Always include last
#include "Memory\MemLeaks.h"

/********************
*   Function Defs   *
*********************/

GameObjectCameraComponentWidget::GameObjectCameraComponentWidget(GameObject* gameObject, EngineViewer* engineViewer, QWidget *parent)
    : QWidget(parent)
    , m_GameObject(gameObject)
    , m_EngineViewer(engineViewer)
{
    m_GameObjectCameraComponentWidgetQtUI.setupUi(this);

    AEAssert(m_GameObject != nullptr);

    AEAssert(m_EngineViewer != nullptr);

    InitFields();
}

GameObjectCameraComponentWidget::~GameObjectCameraComponentWidget()
{
}

CameraGOC* GameObjectCameraComponentWidget::GetCameraGOC()
{
    AEAssert(m_GameObject != nullptr);
    if(m_GameObject == nullptr)
    {
        return nullptr;
    }

    AEAssert(m_GameObject->HasCameraGOC());
    if (!m_GameObject->HasCameraGOC())
    {
        return nullptr;
    }

    ////////////////////////////////////////
    // Get Camera GOC
    CameraGOC* cameraGOC = m_GameObject->GetCameraGOC();

    return cameraGOC;
}

Camera* GameObjectCameraComponentWidget::GetCamera()
{
    ////////////////////////////////////////
    // Get Camera GOC
    CameraGOC* cameraGOC = GetCameraGOC();

    AEAssert(cameraGOC != nullptr);
    if (cameraGOC == nullptr)
    {
        return nullptr;
    }

    ////////////////////////////////////////
    // Get Light
    Camera* camera = cameraGOC->GetCamera();

    ////////////////////////////////////////
    // Verify Light Instance
    AEAssert(camera != nullptr);
    if (camera == nullptr)
    {
        AETODO("Set Log");
        return nullptr;
    }

    return camera;
}

void GameObjectCameraComponentWidget::InitFields()
{
    Camera* camera = nullptr;
    CameraGOC* cameraGOC = nullptr;

    ////////////////////////////////////////
    // Set Ready to false, so Light cannot change
    // properties while been initialized
    m_IsReady = false;

    ////////////////////////////////////////
    // Check Engine Viewer
    AEAssert(m_EngineViewer != nullptr);
    if (m_EngineViewer == nullptr)
    {
        return;
    }

    ////////////////////////////////////////
    // Verify LOC and Get Camera Object
    cameraGOC = GetCameraGOC();
    camera = GetCamera();

    if (camera == nullptr || cameraGOC == nullptr)
    {
        AETODO("Add log");

        return;
    }

    ////////////////////////////////////////
    // Set Debug Draw
    m_GameObjectCameraComponentWidgetQtUI.m_DrawFrustumEnabled->setChecked(cameraGOC->IsDrawFrustumEnabled());

    ////////////////////////////////////////
    // Set Default camera
    bool isDefault = (m_EngineViewer->GetCameraManager()->GetDefaultCameraID() == camera->GetUniqueID());
    m_GameObjectCameraComponentWidgetQtUI.m_DefaultCamera->setChecked(isDefault);

    ////////////////////////////////////////
    // Camera is Ready to change properties
    m_IsReady = true;
}

void GameObjectCameraComponentWidget::on_m_DrawFrustumEnabled_stateChanged(int newState)
{
    if (!m_IsReady)
    {
        return;
    }

    ////////////////////////////////////////
    // Verify LOC and Get Camera Object
    CameraGOC* cameraGOC = GetCameraGOC();
    if (cameraGOC == nullptr)
    {
        AETODO("Add log");

        return;
    }

    cameraGOC->SetDrawFrustumEnabled(m_GameObjectCameraComponentWidgetQtUI.m_DrawFrustumEnabled->isChecked());
}

void GameObjectCameraComponentWidget::on_m_MakeDefaultButton_clicked()
{
    if (!m_IsReady)
    {
        return;
    }

    ////////////////////////////////////////
    // Verify LOC and Get Camera Object
    Camera* camera = GetCamera();
    if (camera == nullptr)
    {
        AETODO("Add log");

        return;
    }

    if (m_EngineViewer->GetCameraManager()->SetDefaultCamera(camera->GetUniqueID()) != AEResult::Ok)
    {
        AETODO("Add log");
    }
    else
    {
        m_GameObjectCameraComponentWidgetQtUI.m_DefaultCamera->setChecked(true);
    }
}

