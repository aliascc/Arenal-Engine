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
AETODO("Add mutex");
LightGOC::LightGOC(GameObject* gameObject, LightManager* lightManager)
	: GameObjectComponent(gameObject, GameObjectComponentType::Light)
	, m_LightManager(lightManager)
{
	AEAssert(lightManager != nullptr);

	ChangeLightType(LightType::Spot);
}

LightGOC::~LightGOC()
{
	if(m_LightManager != nullptr && m_Light != nullptr)
	{
		AETODO("Check return");
		m_LightManager->RemoveLight(m_Light);
	}

	DeleteMem(m_Light);
}

AEResult LightGOC::ChangeLightType(LightType lightType)
{
	AEAssert(m_LightManager != nullptr);
	if(m_LightManager == nullptr)
	{
		return AEResult::LightManagerNull;
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
			return AEResult::InvalidObjType;

		default:
			AEAssert(false);
			return AEResult::InvalidObjType;
	}

	if(m_Light != nullptr)
	{
		*newLight = *m_Light;

		AETODO("Check return");
		m_LightManager->RemoveLight(m_Light);

		DeleteMem(m_Light);
	}

	m_Light = newLight;

	AEResult ret = m_LightManager->AddLight(m_Light);
	if(ret != AEResult::Ok)
	{
		DeleteMem(m_Light);

		return ret;
	}

	return AEResult::Ok;
}