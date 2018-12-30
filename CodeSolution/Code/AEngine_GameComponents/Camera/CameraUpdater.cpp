/*
* Copyright (c) 2018 <Carlos Chac�n>
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
#include "precomp_gamecomponents.h"

/**********************
*   System Includes   *
***********************/

/*************************
*   3rd Party Includes   *
**************************/

/***************************
*   Game Engine Includes   *
****************************/
#include "CameraUpdater.h"
#include "Camera\Camera.h"
#include "Camera\CameraManager.h"

//Always include last
#include "Memory\MemLeaks.h"

/********************
*   Function Defs   *
*********************/

CameraUpdater::CameraUpdater(GameApp& gameApp, GameResourceManager& gameResourceManager, const std::string& name, const std::string& serviceName, uint32_t callOrder)
    : GameComponent(gameApp, gameResourceManager, name, callOrder)
    , m_ServiceName(serviceName)
{
    AEAssert(!m_ServiceName.empty());
    if (!m_ServiceName.empty())
    {
        if (m_GameApp.RegisterGameService(m_ServiceName, this) == AEResult::Ok)
        {
            m_IsReady = true;
        }
        else
        {
            AETODO("Log Error");
        }
    }
}

CameraUpdater::~CameraUpdater()
{
    if (m_IsReady)
    {
        m_GameApp.UnRegisterGameService(m_ServiceName);
    }
}

uint32_t CameraUpdater::GetNumberOfCameras() const
{
    return static_cast<uint32_t>(m_GameApp.GetCameraManager()->GetSize());
}

Camera* CameraUpdater::GetMainCamera() const
{
    return m_GameApp.GetCameraManager()->GetMainCamera();
}

bool CameraUpdater::CameraExists(uint64_t cameraID)
{
    return m_GameApp.GetCameraManager()->CameraExists(cameraID);
}

AEResult CameraUpdater::SetMainCamera(uint64_t cameraID)
{
    return m_GameApp.GetCameraManager()->SetMainCamera(cameraID);
}

void CameraUpdater::Update(const TimerParams& timerParams)
{
    m_GameApp.GetCameraManager()->Update(timerParams);
}

void CameraUpdater::OnResetDevice()
{
    //////////////////////////////////////////////////
    //Update all cameras
    CameraManager* cameraManager = m_GameApp.GetCameraManager();
    GraphicDevice* graphicDevice = m_GameApp.GetGraphicsDevice();

    for (auto cameraIt : *cameraManager)
    {
        cameraIt.second->ScreenDimensionsChanged(graphicDevice->GetGraphicPP().m_BackBufferWidth, graphicDevice->GetGraphicPP().m_BackBufferHeight);
    }
}
