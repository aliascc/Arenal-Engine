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

/***************************
*   Game Engine Includes   *
****************************/
#include "Utils\Frustum.h"
#include "Camera\Camera.h"
#include "DirectionalLight.h"

//Always include last
#include "Memory\MemLeaks.h"

#undef min

/********************
*   Function Defs   *
*********************/
DirectionalLight::DirectionalLight(const std::wstring& name, const glm::vec3& position, const glm::vec3& direction, const Color& color, float nearAtteniation, float farAttenuation, float intensity, bool enabled)
	: Light(LightType::Directional, name, position, direction, color, farAttenuation, nearAtteniation, intensity, enabled)
{
}

DirectionalLight::~DirectionalLight()
{
}

void DirectionalLight::UpdateLightMatrices()
{
	/////////////////////////////////////////////////
	//Update View Matrix
	m_ViewMatrix = glm::lookAt(m_Position, m_Position + m_Direction, AEMathHelpers::Vec3fUp);

	/////////////////////////////////////////////////
	//Update Projection Matrix
	float halfWidth = (float)AE_LIGHT_DIR_SHADOW_TEXTURE_WIDTH / 2.0f;
	float halfHeight = (float)AE_LIGHT_DIR_SHADOW_TEXTURE_HEIGHT / 2.0f;
	m_ProjectionMatrix = glm::ortho(-halfWidth, halfWidth, halfHeight, -halfHeight, 1.0f, m_FarAttenuation);

	/////////////////////////////////////////////////
	//Update Light Main Frustum
	m_Frustum->UpdateFrustum(m_ViewMatrix, m_ProjectionMatrix);
}

AEResult DirectionalLight::UpdateCasacadeMatrices(const Camera* camera)
{
	/////////////////////////////////////////////////////
	//Pre-checks
	AEAssert(camera != nullptr);
	if (camera == nullptr)
	{
		return AEResult::NullParameter;
	}

	/////////////////////////////////////////////////////
	//Step 2: Get Near Clip & Minimum Far Clip
	float nearClip = camera->GetZNear();
	float farClip = glm::min(camera->GetZFar(), m_FarAttenuation);

	/////////////////////////////////////////////////////
	//Step 3: Compute Far Distance for each Shadow Frustum
	m_LightCascadeInfo.m_CascadeDepths[0] = nearClip;
	m_LightCascadeInfo.m_CascadeDepths[AE_LIGHT_NUM_CASCADE_MAPS] = farClip;

	//for (uint32_t i = 1; i < AE_LIGHT_NUM_CASCADE_MAPS; i++)
	//{
	//	float temp = ((float)i / (float)AE_LIGHT_NUM_CASCADE_MAPS);

	//	m_LightCascadeInfo.m_CascadeDepths[i] = nearClip + ((farClip - nearClip) * glm::pow<float>(temp, 2));
	//}

	AETODO("Find a better way to calculate shadow cascade depths");
	m_LightCascadeInfo.m_CascadeDepths[1] = nearClip + (farClip - nearClip) * 0.10f;
	m_LightCascadeInfo.m_CascadeDepths[2] = nearClip + (farClip - nearClip) * 0.40f;

	//m_CascadeDistances.x = m_SplitDepthsTmp[0];
	//m_CascadeDistances.y = m_SplitDepthsTmp[1] * 0.5f;
	//m_CascadeDistances.z = m_SplitDepthsTmp[2] * 0.5f;

	for (uint32_t i = 0; i < AE_LIGHT_NUM_CASCADE_MAPS; i++)
	{
		CalculateCascadeViewProjection(camera, farClip, m_LightCascadeInfo.m_CascadeDepths[i], m_LightCascadeInfo.m_CascadeDepths[i + 1], m_LightCascadeInfo.m_CascadeViewMatrix[i], m_LightCascadeInfo.m_CascadeProjectionMatrix[i], m_LightCascadeInfo.m_CascadeShadowPosition[i]);

		m_LightCascadeInfo.m_CascadeFrustums[i].UpdateFrustum(m_LightCascadeInfo.m_CascadeViewMatrix[i], m_LightCascadeInfo.m_CascadeProjectionMatrix[i]);
	}

	AETODO("Find a better way to calculate shadow cascade depths");
	m_LightCascadeInfo.m_CascadeDepths[1] = m_LightCascadeInfo.m_CascadeDepths[1] * 0.75f;
	m_LightCascadeInfo.m_CascadeDepths[2] = m_LightCascadeInfo.m_CascadeDepths[2] * 0.75f;

	return AEResult::Ok;
}

AEResult DirectionalLight::CalculateCascadeViewProjection(const Camera* camera, float farClip, float frustumNearClip, float frustumFarClip, glm::mat4& viewMatrix, glm::mat4& projectionMatrix, glm::vec3& shadowPos)
{
	/////////////////////////////////////////////////////
	//Pre-checks
	AEAssert(camera != nullptr);
	if (camera == nullptr)
	{
		return AEResult::NullParameter;
	}

	/////////////////////////////////////////////////////
	//Step 1: Get Frustum Corners & transform them to Camera View Space
	const FrustumCorners& cameraFrustumCornersWS = camera->GetFrustum().GetCorners();
	const glm::mat4& cameraViewMatrix = camera->GetViewMatrix();
	FrustumCorners cameraFrustumCornersVS;

	for (size_t i = 0; i < cameraFrustumCornersWS.size(); i++)
	{
		glm::vec4 transformVec = cameraViewMatrix * glm::vec4(cameraFrustumCornersWS[i], 1.0f);

		cameraFrustumCornersVS[i] = glm::vec3(transformVec.x, transformVec.y, transformVec.z);
	}

	/////////////////////////////////////////////////////
	//Step 2
	FrustumCorners cameraSplitFrustumCornersVS;

	for (uint32_t i = 0; i < 4; i++)
	{
		cameraSplitFrustumCornersVS[i] = cameraFrustumCornersVS[i + 4] * (frustumNearClip / camera->GetZFar());
	}

	for (uint32_t i = 4; i < 8; i++)
	{
		cameraSplitFrustumCornersVS[i] = cameraFrustumCornersVS[i] * (frustumFarClip / camera->GetZFar());
	}

	/////////////////////////////////////////////////////
	//Step 3
	FrustumCorners cameraFrustumCornersCameraPos;

	for (uint32_t i = 0; i < 8; i++)
	{
		glm::mat4 translationMatrix = glm::translate(camera->GetPosition());
		//camera->GetWorldTransform()
		glm::vec4 transformVec = translationMatrix * glm::vec4(cameraSplitFrustumCornersVS[i], 1.0f);

		cameraFrustumCornersCameraPos[i] = glm::vec3(transformVec.x, transformVec.y, transformVec.z);
	}

	glm::vec3 cameraUpVector = camera->GetUpVector();
	if (glm::abs(glm::dot(cameraUpVector, -m_Direction)) > 0.9f)
	{
		AETODO("Change to forward");
		cameraUpVector = glm::vec3(0.0f, 0.0f, 1.0f);
	}

	glm::mat4 lightRotation = glm::lookAt(AEMathHelpers::Vec3fZero, m_Direction, cameraUpVector);

	for (uint32_t i = 0; i < 8; i++)
	{
		glm::vec4 transformVec = lightRotation * glm::vec4(cameraFrustumCornersCameraPos[i], 1.0f);

		cameraFrustumCornersCameraPos[i] = glm::vec3(transformVec.x, transformVec.y, transformVec.z);
	}

	/////////////////////////////////////////////////////
	//Step 4
	glm::vec3 minPos = AEMathHelpers::Vec3fZero;
	glm::vec3 maxPos = AEMathHelpers::Vec3fZero;
	GetMaxMinPoints(cameraFrustumCornersCameraPos, maxPos, minPos);

	///////////////////////////////////////////////////////
	////Step 5: Fix Shadow Jittering
	float diagonalLength = glm::length(cameraFrustumCornersCameraPos[0] - cameraFrustumCornersCameraPos[6]);

	//Without this, the shadow map isn't big enough in the world.
	diagonalLength += 2;

	float worldsUnitsPerTexel = diagonalLength / (float)AE_LIGHT_DIR_SHADOW_TEXTURE_WIDTH;

	glm::vec3 borderOffset = (glm::vec3(diagonalLength, diagonalLength, diagonalLength) - (maxPos - minPos)) * 0.5f;
	maxPos += borderOffset;
	minPos -= borderOffset;

	minPos /= worldsUnitsPerTexel;
	minPos.x = glm::floor(minPos.x);
	minPos.y = glm::floor(minPos.y);
	minPos.z = glm::floor(minPos.z);
	minPos *= worldsUnitsPerTexel;

	maxPos /= worldsUnitsPerTexel;
	maxPos.x = glm::floor(maxPos.x);
	maxPos.y = glm::floor(maxPos.y);
	maxPos.z = glm::floor(maxPos.z);
	maxPos *= worldsUnitsPerTexel;

	/////////////////////////////////////////////////////
	//Step 6
	glm::vec3 boxSize = maxPos - minPos;

	if (boxSize.x == 0 || boxSize.y == 0 || boxSize.z == 0)
	{
		boxSize = AEMathHelpers::Vec3fOne;
	}

	glm::vec3 halfBoxSize = boxSize * 0.5f;

	glm::vec3 lightPosition = minPos + halfBoxSize;
	lightPosition.z = minPos.z;

	/////////////////////////////////////////////////////
	//Step 7
	lightRotation = glm::inverse(lightRotation);

	glm::vec4 transformLight = lightRotation * glm::vec4(lightPosition, 1.0f);

	//shadowPos = glm::vec3(transformLight.x, transformLight.y - 20, transformLight.z - 25);
	shadowPos = glm::vec3(transformLight.x, transformLight.y, transformLight.z);
	
	// Create the view matrix for the light
	viewMatrix = glm::lookAt(shadowPos, shadowPos + m_Direction, cameraUpVector);

	// Create the projection matrix for the light
	projectionMatrix = glm::ortho(-boxSize.x / 2.0f, boxSize.x / 2.0f, -boxSize.y/2.0f, boxSize.y/2.0f, -boxSize.z, boxSize.z);
	//projectionMatrix = glm::ortho(0.0f, boxSize.x, 0.0f, boxSize.y, 0.0f, boxSize.z);

	return AEResult::Ok;
}

AETODO("Remove for when we have bounding box");
void DirectionalLight::GetMaxMinPoints(const FrustumCorners& corners, glm::vec3& maxPoint, glm::vec3& minPoint)
{
	maxPoint = corners[0];
	minPoint = corners[0];

	for (size_t i = 1; i < corners.size(); i++)
	{
		if (minPoint.x > corners[i].x)
		{
			minPoint.x = corners[i].x;
		}
		if (minPoint.y > corners[i].y)
		{
			minPoint.y = corners[i].y;
		}
		if (minPoint.z > corners[i].z)
		{
			minPoint.z = corners[i].z;
		}

		if (maxPoint.x < corners[i].x)
		{
			maxPoint.x = corners[i].x;
		}
		if (maxPoint.y < corners[i].y)
		{
			maxPoint.y = corners[i].y;
		}
		if (maxPoint.z < corners[i].z)
		{
			maxPoint.z = corners[i].z;
		}
	}
}
