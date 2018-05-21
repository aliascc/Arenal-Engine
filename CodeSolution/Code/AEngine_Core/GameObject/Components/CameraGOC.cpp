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

/*************************
*   Precompiled Header   *
**************************/
#include "precomp_core.h"

/**********************
*   System Includes   *
***********************/

/*************************
*   3rd Party Includes   *
**************************/

/***************************
*   Game Engine Includes   *
****************************/
#include "CameraGOC.h"
#include "Camera\Camera.h"
#include "GraphicDevice.h"
#include "Camera\CameraManager.h"
#include "GameObject\GameObject.h"
#include "Camera\GameObjectCamera.h"

//Always include last
#include "Memory\MemLeaks.h"

/********************
*   Function Defs   *
*********************/

AETODO("Add mutex");
CameraGOC::CameraGOC(GameObject* gameObject, CameraManager* cameraManager, GraphicDevice* graphicDevice)
    : GameObjectComponent(gameObject, GameObjectComponentType::Camera)
    , m_CameraManager(cameraManager)
    , m_GraphicDevice(graphicDevice)
{
    AEAssert(m_CameraManager != nullptr);
    AEAssert(m_GraphicDevice != nullptr);

    if (m_GraphicDevice != nullptr)
    {
        AETODO("See how to add better name");
        AETODO("Far and Near Att");

        glm::ivec2 dimension(m_GraphicDevice->GetGraphicPP().m_BackBufferWidth, m_GraphicDevice->GetGraphicPP().m_BackBufferHeight);
        m_Camera = new GameObjectCamera("Game Object Camera", AEMathHelpers::Vec3fZero, AEMathHelpers::Vec3fFwrZP, AEMathHelpers::Vec3fUp, dimension, 45.0f, 1.0f, 1000.0f);
    }

    if (m_CameraManager != nullptr && m_Camera != nullptr)
    {
        m_CameraManager->AddCamera(m_Camera);

        m_IsReady = true;
    }
}

CameraGOC::~CameraGOC()
{
    if (m_IsReady)
    {
        m_CameraManager->RemoveCamera(m_Camera->GetUniqueID());
    }
}

bool CameraGOC::IsDefaultCamera()
{
    if (!m_IsReady)
    {
        return false;
    }

    return (m_CameraManager->GetDefaultCameraID() == m_Camera->GetUniqueID());
}

AEResult CameraGOC::SetAsDefaultCamera()
{
    if (!m_IsReady)
    {
        return AEResult::NotReady;
    }

    return m_CameraManager->SetDefaultCamera(m_Camera->GetUniqueID());
}
