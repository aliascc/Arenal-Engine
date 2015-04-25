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

XETODO("May need thread lock");
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

XEResult CameraManager::AddCamera(Camera* camera)
{
	XEAssert(camera != nullptr);
	if (camera == nullptr)
	{
		return XEResult::NullParameter;
	}

	XEAssert(!camera->GetName().empty());
	if (camera->GetName().empty())
	{
		return XEResult::EmptyName;
	}

	if (CameraExists(camera->GetUniqueID()))
	{
		return XEResult::ObjExists;
	}

	m_CameraMap[camera->GetUniqueID()] = camera;

	return XEResult::Ok;
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

XEResult CameraManager::RemoveCamera(uint64_t cameraID)
{
	if (!CameraExists(cameraID))
	{
		return XEResult::NotFound;
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

	return XEResult::Ok;
}

XEResult CameraManager::SetMainCamera(uint64_t cameraID)
{
	if (!CameraExists(cameraID))
	{
		return XEResult::NotFound;
	}

	m_TempMainCamera = m_CameraMap[cameraID];

	return XEResult::Ok;
}

XEResult CameraManager::SetDefaultCamera(uint64_t cameraID)
{
	if (!CameraExists(cameraID))
	{
		return XEResult::NotFound;
	}

	m_DefaultCamera = m_CameraMap[cameraID];

	return XEResult::Ok;
}

XEResult CameraManager::SetEditorCamera(uint64_t cameraID)
{
	if (!CameraExists(cameraID))
	{
		return XEResult::NotFound;
	}

	m_EditorCamera = m_CameraMap[cameraID];

	return XEResult::Ok;
}

uint64_t CameraManager::GetDefaultCameraID()
{
	if (m_DefaultCamera == nullptr)
	{
		return 0;
	}

	return m_DefaultCamera->GetUniqueID();
}

XEResult CameraManager::SetDefaultCameraAsMain()
{
	if (m_DefaultCamera == nullptr)
	{
		return XEResult::NullObj;
	}

	m_TempMainCamera = m_DefaultCamera;

	return XEResult::Ok;
}

XEResult CameraManager::SetEditorCameraAsMain()
{
	if (m_EditorCamera == nullptr)
	{
		return XEResult::NullObj;
	}

	m_TempMainCamera = m_EditorCamera;

	return XEResult::Ok;
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
