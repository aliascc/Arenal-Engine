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
#include "LightGOC.h"
#include "Lights\Light.h"
#include "Lights\SpotLight.h"
#include "Lights\OmniLight.h"
#include "Base\BaseFunctions.h"
#include "Lights\LightManager.h"
#include "Lights\DirectionalLight.h"

//Always include last
#include "Memory\MemLeaks.h"

/********************
*   Function Defs   *
*********************/
XETODO("Add mutex");
LightGOC::LightGOC(GameObject* gameObject, LightManager* lightManager)
	: GameObjectComponent(gameObject, GameObjectComponentType::Light)
	, m_LightManager(lightManager)
{
	XEAssert(lightManager != nullptr);

	ChangeLightType(LightType::Spot);
}

LightGOC::~LightGOC()
{
	if(m_LightManager != nullptr && m_Light != nullptr)
	{
		XETODO("Check return");
		m_LightManager->RemoveLight(m_Light);
	}

	DeleteMem(m_Light);
}

XEResult LightGOC::ChangeLightType(LightType lightType)
{
	XEAssert(m_LightManager != nullptr);
	if(m_LightManager == nullptr)
	{
		return XEResult::LightManagerNull;
	}

	Light* newLight = nullptr;

	switch (lightType)
	{
		case LightType::Spot:
			newLight = new SpotLight();
			break;

		case LightType::Omni:
			newLight = new OmniLight();
			break;

		case LightType::Directional:
			newLight = new DirectionalLight();
			break;

		case LightType::Invalid:
			return XEResult::InvalidObjType;

		default:
			XEAssert(false);
			return XEResult::InvalidObjType;
	}

	if(m_Light != nullptr)
	{
		*newLight = *m_Light;

		XETODO("Check return");
		m_LightManager->RemoveLight(m_Light);

		DeleteMem(m_Light);
	}

	m_Light = newLight;

	XEResult ret = m_LightManager->AddLight(m_Light);

	if(ret != XEResult::Ok)
	{
		DeleteMem(m_Light);

		return ret;
	}

	return XEResult::Ok;
}