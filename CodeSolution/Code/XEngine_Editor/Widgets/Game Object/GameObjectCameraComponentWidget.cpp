/********************************************************
*
* Author: Carlos Chacón N.
*
* Created: 9/21/2014
*
* Desc:
*
*
* Copyright 2012-2014
*
*********************************************************/

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
#include "Utils\XEQTDefs.h"
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

	XEAssert(m_GameObject != nullptr);

	XEAssert(m_EngineViewer != nullptr);


	InitFields();
}

GameObjectCameraComponentWidget::~GameObjectCameraComponentWidget()
{
}

CameraGOC* GameObjectCameraComponentWidget::GetCameraGOC()
{
	XEAssert(m_GameObject != nullptr);
	if(m_GameObject == nullptr)
	{
		return nullptr;
	}

	XEAssert(m_GameObject->HasCameraGOC());
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

	XEAssert(cameraGOC != nullptr);
	if (cameraGOC == nullptr)
	{
		return nullptr;
	}

	////////////////////////////////////////
	// Get Light
	Camera* camera = cameraGOC->GetCamera();

	////////////////////////////////////////
	// Verify Light Instance
	XEAssert(camera != nullptr);
	if (camera == nullptr)
	{
		XETODO("Set Log");
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
	XEAssert(m_EngineViewer != nullptr);
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
		XETODO("Add log");

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
		XETODO("Add log");

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
		XETODO("Add log");

		return;
	}

	if (m_EngineViewer->GetCameraManager()->SetDefaultCamera(camera->GetUniqueID()) != XEResult::Ok)
	{
		XETODO("Add log");
	}
	else
	{
		m_GameObjectCameraComponentWidgetQtUI.m_DefaultCamera->setChecked(true);
	}
}

