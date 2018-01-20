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
#include "Camera.h"
#include "CameraManager.h"
#include "Base\BaseFunctions.h"

//Always include last
#include "Memory\MemLeaks.h"

/********************
*   Function Defs   *
*********************/

AETODO("May need thread lock");
CameraManager::CameraManager()
{
}

CameraManager::~CameraManager()
{
    //////////////////////////////////////////////////
    //Delete all cameras
    for (auto cameraIt : m_CameraMap)
    {
        DeleteMem(cameraIt.second);
    }

    m_CameraMap.clear();
}

uint32_t CameraManager::GetNumberOfCamerass() const
{
    return static_cast<uint32_t>(m_CameraMap.size());
}

bool CameraManager::CameraExists(uint64_t cameraID)
{
    return (m_CameraMap.find(cameraID) != m_CameraMap.end());
}

AEResult CameraManager::AddCamera(Camera* camera)
{
    AEAssert(camera != nullptr);
    if (camera == nullptr)
    {
        return AEResult::NullParameter;
    }

    AEAssert(!camera->GetName().empty());
    if (camera->GetName().empty())
    {
        return AEResult::EmptyName;
    }

    if (CameraExists(camera->GetUniqueID()))
    {
        return AEResult::ObjExists;
    }

    m_CameraMap[camera->GetUniqueID()] = camera;

    return AEResult::Ok;
}

const Camera* CameraManager::GetCamera(uint64_t cameraID)
{
    if (!CameraExists(cameraID))
    {
        return nullptr;
    }

    auto cameraIt = m_CameraMap.find(cameraID);

    return cameraIt->second;
}

AEResult CameraManager::RemoveCamera(uint64_t cameraID)
{
    if (!CameraExists(cameraID))
    {
        return AEResult::NotFound;
    }

    auto cameraIt = m_CameraMap.find(cameraID);

    Camera* camera = cameraIt->second;

    m_CameraMap.erase(cameraIt);

    if (camera->GetUniqueID() == m_MainCamera->GetUniqueID())
    {
        m_RemoveMainCamera = true;
    }
    else
    {
        DeleteMem(camera);
    }

    return AEResult::Ok;
}

AEResult CameraManager::SetMainCamera(uint64_t cameraID)
{
    if (!CameraExists(cameraID))
    {
        return AEResult::NotFound;
    }

    m_TempMainCamera = m_CameraMap[cameraID];

    return AEResult::Ok;
}

AEResult CameraManager::SetDefaultCamera(uint64_t cameraID)
{
    if (!CameraExists(cameraID))
    {
        return AEResult::NotFound;
    }

    m_DefaultCamera = m_CameraMap[cameraID];

    return AEResult::Ok;
}

AEResult CameraManager::SetEditorCamera(uint64_t cameraID)
{
    if (!CameraExists(cameraID))
    {
        return AEResult::NotFound;
    }

    m_EditorCamera = m_CameraMap[cameraID];

    return AEResult::Ok;
}

uint64_t CameraManager::GetDefaultCameraID()
{
    if (m_DefaultCamera == nullptr)
    {
        return 0;
    }

    return m_DefaultCamera->GetUniqueID();
}

AEResult CameraManager::SetDefaultCameraAsMain()
{
    if (m_DefaultCamera == nullptr)
    {
        return AEResult::NullObj;
    }

    m_TempMainCamera = m_DefaultCamera;

    return AEResult::Ok;
}

AEResult CameraManager::SetEditorCameraAsMain()
{
    if (m_EditorCamera == nullptr)
    {
        return AEResult::NullObj;
    }

    m_TempMainCamera = m_EditorCamera;

    return AEResult::Ok;
}

void CameraManager::Update(const TimerParams& timerParams)
{
    //////////////////////////////////////////////////
    //Check if removal of Main camera is needed
    if (m_RemoveMainCamera)
    {
        m_RemoveMainCamera = false;

        DeleteMem(m_MainCamera);
    }

    //////////////////////////////////////////////////
    //Check if Change of Main camera is needed
    if (m_TempMainCamera != nullptr)
    {
        m_MainCamera = m_TempMainCamera;

        m_TempMainCamera = nullptr;
    }

    //////////////////////////////////////////////////
    //Update all cameras
    for (auto cameraIt : m_CameraMap)
    {
        cameraIt.second->Update(timerParams);
    }
}

CameraMapIt CameraManager::begin()
{
    return m_CameraMap.begin();
}

CameraMapIt CameraManager::end()
{
    return m_CameraMap.end();
}

CameraMapItConst CameraManager::begin() const
{
    return m_CameraMap.begin();
}

CameraMapItConst CameraManager::end() const
{
    return m_CameraMap.end();
}

CameraMapItConst CameraManager::cbegin() const
{
    return m_CameraMap.cbegin();
}

CameraMapItConst CameraManager::cend() const
{
    return m_CameraMap.cend();
}
