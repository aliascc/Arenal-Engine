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
#include "CameraUpdater.h"
#include "Camera\Camera.h"
#include "GraphicDevice.h"
#include "GameApp\GameApp.h"
#include "Base\BaseFunctions.h"
#include "Camera\CameraManager.h"

//Always include last
#include "Memory\MemLeaks.h"

/********************
*   Function Defs   *
*********************/

CameraUpdater::CameraUpdater(GameApp* gameApp, const std::wstring& name, const std::wstring& serviceName, uint32_t callOrder)
	: GameComponent(gameApp, name, callOrder)
	, m_ServiceName(serviceName)
{
	AEAssert(!m_ServiceName.empty());
	if (m_GameApp != nullptr && !m_ServiceName.empty())
	{
		if (m_GameApp->RegisterGameService(m_ServiceName, this) == AEResult::Ok)
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
		m_GameApp->UnRegisterGameService(m_ServiceName);
	}
}

uint32_t CameraUpdater::GetNumberOfCameras() const
{
	return static_cast<uint32_t>(m_GameApp->GetCameraManager()->GetSize());
}

Camera* CameraUpdater::GetMainCamera() const
{
	return m_GameApp->GetCameraManager()->GetMainCamera();
}

bool CameraUpdater::CameraExists(uint64_t cameraID)
{
	return m_GameApp->GetCameraManager()->CameraExists(cameraID);
}

AEResult CameraUpdater::SetMainCamera(uint64_t cameraID)
{
	return m_GameApp->GetCameraManager()->SetMainCamera(cameraID);
}

void CameraUpdater::Update(const TimerParams& timerParams)
{
	m_GameApp->GetCameraManager()->Update(timerParams);

	GameComponent::Update(timerParams);
}

void CameraUpdater::OnResetDevice()
{
	//////////////////////////////////////////////////
	//Update all cameras
	CameraManager* cameraManager = m_GameApp->GetCameraManager();
	GraphicDevice* graphicDevice = m_GameApp->GetGraphicsDevice();

	for (auto cameraIt : *cameraManager)
	{
		cameraIt.second->ScreenDimensionsChanged(graphicDevice->GetGraphicPP().m_BackBufferWidth, graphicDevice->GetGraphicPP().m_BackBufferHeight);
	}

	GameComponent::OnResetDevice();
}