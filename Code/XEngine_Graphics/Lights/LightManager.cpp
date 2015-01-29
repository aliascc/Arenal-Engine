/********************************************************
*
* Author: Carlos Chacón N.
*
* Created: 3/22/2012
*
* Desc:
*
*
* Copyright 2012
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
#include "Light.h"
#include "SpotLight.h"
#include "LightManager.h"
#include "GraphicDevice.h"
#include "Camera\Camera.h"
#include "DirectionalLight.h"
#include "Textures\RenderTarget.h"
#include "Shaders\Variables\Texture2DArray.h"

//Always include last
#include "Memory\MemLeaks.h"

/********************
*   Function Defs   *
*********************/
XETODO("May need thread lock");
LightManager::LightManager(GraphicDevice* graphicDevice)
	: m_GraphicDevice(graphicDevice)
{
	XEAssert(m_GraphicDevice != nullptr);

	memset(m_LightFXVec, 0, sizeof(LightFX) * XE_MAX_LIGHTS);

	memset(m_SpotLightShadowInfoFX, 0, sizeof(SpotLightShadowInfoFX)* XE_MAX_LIGHTS);

	memset(m_DirLightShadowInfoFX, 0, sizeof(LightCascadeInfoFX)* XE_MAX_LIGHTS);

	m_SpotLightShadowTextureArray = new Texture2DArray(m_GraphicDevice, XE_TXA_SPOT_LIGHT_SHADOW_TEXTURE_ARRAY_NAME, XE_BI_SHADOW_SPOT_LIGHT_TEXT_ARRAY_BUFFER_BIND_IDX);

	m_DirLightShadowTextureArray = new Texture2DArray(m_GraphicDevice, XE_TXA_DIR_LIGHT_SHADOW_TEXTURE_ARRAY_NAME, XE_BI_SHADOW_DIR_LIGHT_TEXT_ARRAY_BUFFER_BIND_IDX);
}

LightManager::~LightManager()
{
	DeleteMem(m_SpotLightShadowTextureArray);
	DeleteMem(m_DirLightShadowTextureArray);
}

uint32_t LightManager::GetNumberOfLights() const
{
	return static_cast<uint32_t>(m_LightsMap.size());
}

void LightManager::ClearFXVectorChangeFlag()
{
	m_FXVectorChanged = false;
}

void LightManager::ClearFXSpotShadowVectorChangedFlag()
{
	m_FXSpotShadowVectorChanged = false;
}

void LightManager::ClearFXDirShadowVectorChangedFlag()
{
	m_FXDirShadowVectorChanged = false;
}

bool LightManager::LightExists(uint64_t lightID)
{
	return (m_LightsMap.find(lightID) != m_LightsMap.end());
}

XEResult LightManager::AddLight(Light* light)
{
	XEAssert(light != nullptr);
	if(light == nullptr)
	{
		return XEResult::NullParameter;
	}

	if(LightExists(light->GetUniqueID()))
	{
		return XEResult::ObjExists;
	}

XETODO("Add log about full lights");
	if(m_LightsMap.size() >= XE_MAX_LIGHTS)
	{
		return XEResult::FullContainer;
	}

	m_LightsMap[light->GetUniqueID()] = light;
	m_LightsVector.push_back(light);

	m_MapChanged = true;

	return XEResult::Ok;
}

XEResult LightManager::RemoveLight(uint64_t lightID)
{
	if(!LightExists(lightID))
	{
		return XEResult::NotFound;
	}

	m_LightsMap.erase(lightID);

	auto vecIt = m_LightsVector.begin();
	auto endVecIt = m_LightsVector.end();
	for (; vecIt != endVecIt; vecIt++)
	{
		if ((*vecIt)->GetUniqueID() == lightID)
		{
			m_LightsVector.erase(vecIt);
			break;
		}
	}

	m_MapChanged = true;

	return XEResult::Ok;
}

XEResult LightManager::RemoveLight(Light* light)
{
	XEAssert(light != nullptr);
	if(light == nullptr)
	{
		return XEResult::NullParameter;
	}

	return RemoveLight(light->GetUniqueID());
}

XEResult LightManager::UpdateSpotLightShadowInformation(SpotLight* light, uint32_t& shadowTextIdx)
{
	XEAssert(light != nullptr);
	if (light == nullptr)
	{
		return XEResult::NullParameter;
	}

	if (!light->m_ShadowEnabled)
	{
		if (light->m_ShadowTextureIndex != XE_LIGHT_SHADOW_DISABLE_INDEX)
		{
			light->m_ShadowTextureIndex = XE_LIGHT_SHADOW_DISABLE_INDEX;
		}

		return XEResult::Ok;
	}

	if (light->m_ShadowTextureIndex != shadowTextIdx)
	{
		light->m_ShadowTextureIndex = shadowTextIdx;
	}

	light->UpdateLightMatrices();

	glm::mat4 lightVP = light->GetProjectionMatrix() * light->GetViewMatrix();

	if (lightVP != m_SpotLightShadowInfoFX[shadowTextIdx].m_ViewProjectionMatrix)
	{
		m_SpotLightShadowInfoFX[shadowTextIdx].m_ViewProjectionMatrix = lightVP;
		m_FXSpotShadowVectorChanged = true;
	}

	shadowTextIdx++;

	return XEResult::Ok;
}

XEResult LightManager::UpdateDirLightShadowInformation(DirectionalLight* light, uint32_t& shadowTextIdx, Camera* camera)
{
	XEAssert(light != nullptr);
	if (light == nullptr)
	{
		return XEResult::NullParameter;
	}

	if (!light->m_ShadowEnabled)
	{
		if (light->m_ShadowTextureIndex != XE_LIGHT_SHADOW_DISABLE_INDEX)
		{
			light->m_ShadowTextureIndex = XE_LIGHT_SHADOW_DISABLE_INDEX;
		}

		return XEResult::Ok;
	}

	if (light->m_ShadowTextureIndex != shadowTextIdx)
	{
		light->m_ShadowTextureIndex = shadowTextIdx;
	}

	XETODO("Check return");
	light->UpdateLightMatrices();

	XETODO("Check return");
	light->UpdateCasacadeMatrices(camera);

	const LightCascadeInfo& lightCascadeInfo = light->GetLightCascadeInfo();

	for (size_t j = 0; j < (XE_LIGHT_NUM_CASCADE_MAPS + 1); j++)
	{
		if (m_DirLightShadowInfoFX[shadowTextIdx].m_CascadeDepths[j] != lightCascadeInfo.m_CascadeDepths[j])
		{
			m_DirLightShadowInfoFX[shadowTextIdx].m_CascadeDepths[j] = lightCascadeInfo.m_CascadeDepths[j];
			m_FXDirShadowVectorChanged = true;
		}
	}

	for (size_t j = 0; j < XE_LIGHT_NUM_CASCADE_MAPS; j++)
	{
		glm::mat4 lightVP = lightCascadeInfo.m_CascadeProjectionMatrix[j] * lightCascadeInfo.m_CascadeViewMatrix[j];
		if (m_DirLightShadowInfoFX[shadowTextIdx].m_CascadeViewProjectionMatrix[j] != lightVP)
		{
			m_DirLightShadowInfoFX[shadowTextIdx].m_CascadeViewProjectionMatrix[j] = lightVP;
			m_FXDirShadowVectorChanged = true;
		}
	}

	shadowTextIdx++;

	return XEResult::Ok;
}

XEResult LightManager::Update(Camera* camera)
{
	XEAssert(m_GraphicDevice != nullptr);
	if (m_GraphicDevice == nullptr)
	{
		return XEResult::GraphicDeviceNull;
	}

	XEAssert(camera != nullptr);
	if (camera == nullptr)
	{
		return XEResult::NullParameter;
	}

	uint32_t shadowSpotIdx = 0;
	uint32_t shadowDirIdx = 0;
	for (size_t i = 0; i < m_LightsVector.size(); i++)
	{
		Light* light = m_LightsVector[i];

		switch (light->m_LightType)
		{
			case LightType::Spot:
				UpdateSpotLightShadowInformation(reinterpret_cast<SpotLight*>(light), shadowSpotIdx);
				break;

			case LightType::Directional:
				UpdateDirLightShadowInformation(reinterpret_cast<DirectionalLight*>(light), shadowDirIdx, camera);
				break;

			default:
				break;
		}

		if (m_MapChanged || *light != m_LightFXVec[i])
		{
			light->UpdateLighFX(m_LightFXVec[i]);
			m_FXVectorChanged = true;
		}
	}

	if (m_NumDirLightsWithShadows != shadowDirIdx)
	{
		XETODO("check return");
		UpdateDirLightShadowTextureArray(shadowDirIdx);

		m_NumDirLightsWithShadows = shadowDirIdx;
	}

	if (m_NumSpotLightsWithShadows != shadowSpotIdx)
	{
		XETODO("check return");
		UpdateSpotLightShadowTextureArray(shadowSpotIdx);

		m_NumSpotLightsWithShadows = shadowSpotIdx;
	}

	m_MapChanged = false;

	return XEResult::Ok;
}

XEResult LightManager::UpdateSpotLightShadowTextureArray(uint32_t size)
{
	/////////////////////////////////////////////////////
	//Pre-checks
	XEAssert(m_GraphicDevice != nullptr);
	if (m_GraphicDevice == nullptr)
	{
		return XEResult::GraphicDeviceNull;
	}

	if (size == 0)
	{
		return m_SpotLightShadowTextureArray->Deinitialize();
	}
	else
	{
		return m_SpotLightShadowTextureArray->Initialize(size, XE_LIGHT_SPOT_SHADOW_TEXTURE_WIDTH, XE_LIGHT_SPOT_SHADOW_TEXTURE_HEIGHT, 1, XE_LIGHT_SHADOW_TEXTURE_FORMAT, TextureBindOption::RenderTarget);
	}

	return XEResult::Ok;
}

XEResult LightManager::UpdateDirLightShadowTextureArray(uint32_t size)
{
	/////////////////////////////////////////////////////
	//Pre-checks
	XEAssert(m_GraphicDevice != nullptr);
	if (m_GraphicDevice == nullptr)
	{
		return XEResult::GraphicDeviceNull;
	}

	if (size == 0)
	{
		return m_DirLightShadowTextureArray->Deinitialize();
	}
	else
	{
		return m_DirLightShadowTextureArray->Initialize(size, XE_LIGHT_DIR_SHADOW_TEXTURE_WIDTH * XE_LIGHT_NUM_CASCADE_MAPS, XE_LIGHT_DIR_SHADOW_TEXTURE_HEIGHT, 1, XE_LIGHT_SHADOW_TEXTURE_FORMAT, TextureBindOption::RenderTarget);
	}

	return XEResult::Ok;
}

LightsMapIt LightManager::begin()
{
	return m_LightsMap.begin();
}

LightsMapIt LightManager::end()
{
	return m_LightsMap.end();
}

LightsMapItConst LightManager::begin() const
{
	return m_LightsMap.begin();
}

LightsMapItConst LightManager::end() const
{
	return m_LightsMap.end();
}

LightsMapItConst LightManager::cbegin() const
{
	return m_LightsMap.cbegin();
}

LightsMapItConst LightManager::cend() const
{
	return m_LightsMap.cend();
}