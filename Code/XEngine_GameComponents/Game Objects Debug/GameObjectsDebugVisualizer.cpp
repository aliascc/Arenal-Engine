/********************************************************
*
* Author: Carlos Chacón N.
*
* Created: 2/6/2014
*
* Desc:
*
*
* Copyright 2013
*
*********************************************************/


/**********************
*   System Includes   *
***********************/
#include <string>

/*************************
*   3rd Party Includes   *
**************************/

/***************************
*   Game Engine Includes   *
****************************/
#include "Time\XETime.h"
#include "Lights\Light.h"
#include "Utils\Frustum.h"
#include "Camera\Camera.h"
#include "GameApp\GameApp.h"
#include "Lights\SpotLight.h"
#include "Lights\SpotLight.h"
#include "Lights\OmniLight.h"
#include "Shapes\LightShape.h"
#include "Textures\Texture2D.h"
#include "Shapes\QuadShape3D.h"
#include "Lights\LightManager.h"
#include "Shapes\FrustumShape.h"
#include "Camera\CameraUpdater.h"
#include "Camera\CameraManager.h"
#include "GameObject\GameObject.h"
#include "Shapes\OmniLightShape.h"
#include "Shapes\SpotLightShape.h"
#include "Lights\DirectionalLight.h"
#include "GameObjectsDebugVisualizer.h"
#include "Shapes\DirectionalLightShape.h"
#include "GameObject\GameObjectManager.h"
#include "Shaders\Buffers\ConstantBuffer.h"
#include "GameObject\Components\LightGOC.h"
#include "GameObject\Components\CameraGOC.h"
#include "Shaders\Bindings\ShaderTextureBinding.h"
#include "Graphic Extensions\Materials\BuiltInMaterialsDefs.h"
#include "Graphic Extensions\Materials\Basic\BasicLineMaterial.h"
#include "Graphic Extensions\Materials\Basic\BasicColorMaterial.h"
#include "Graphic Extensions\Materials\Basic\DiffuseTextureBasicMaterial.h"
#include "Graphic Extensions\Shader Extensions\Properties\ShaderProperties.h"

//Always include last
#include "Memory\MemLeaks.h"

/********************
*   Function Defs   *
*********************/
XETODO("Check Order of GC Call");
XETODO("Set is ready variable");
GameObjectsDebugVisualizer::GameObjectsDebugVisualizer(GameApp* gameApp, const std::wstring& gameComponentName, const std::wstring& cameraServiceName, uint32_t callOrder)
	: DrawableGameComponent(gameApp, gameComponentName, callOrder)
{
	XETODO("Check var and set variable if it is ready");
	m_CameraUpdater = m_GameApp->GetGameService<CameraUpdater>(cameraServiceName);
	XEAssert(m_CameraUpdater != nullptr);
}

GameObjectsDebugVisualizer::~GameObjectsDebugVisualizer()
{
	DeleteMem(m_DiffuseTextureBasicMaterial);
	DeleteMem(m_BasicLineMaterial);
	DeleteMem(m_BasicColorMaterial);

	DeleteMem(m_OmniLightShape);
	DeleteMem(m_SpotLightShape);
	DeleteMem(m_DirectionalLightShape);
	DeleteMem(m_QuadShape3D);

	XERelease(m_OmniLightIconTex);
	XERelease(m_DirLightIconTex);
	XERelease(m_SpotLightIconTex);
	XERelease(m_CameraIconTex);

	DeleteMem(m_FrustumShape);
}

void GameObjectsDebugVisualizer::Initialize()
{
	XETODO("Change name to define string");
	m_DiffuseTextureBasicMaterial = new DiffuseTextureBasicMaterial(m_GraphicDevice, m_GameResourceManager, XE_GOD_V_DIFFUSE_TEXTURE_BASIC_MAT_NAME);

	m_BasicLineMaterial = new BasicLineMaterial(m_GraphicDevice, m_GameResourceManager, XE_GOD_V_BASIC_LINE_MAT_NAME);

	m_BasicColorMaterial = new BasicColorMaterial(m_GraphicDevice, m_GameResourceManager, XE_GOD_V_BASIC_COLOR_MAT_NAME);

	m_QuadShape3D = new QuadShape3D(m_GraphicDevice);

	m_OmniLightShape = new OmniLightShape(m_GraphicDevice);

	m_SpotLightShape = new SpotLightShape(m_GraphicDevice);

	m_DirectionalLightShape = new DirectionalLightShape(m_GraphicDevice);

	m_LightIconsScale = glm::scale(glm::vec3(m_GameObjectsDebugVisualizerConfig.m_LightIconScale, m_GameObjectsDebugVisualizerConfig.m_LightIconScale, m_GameObjectsDebugVisualizerConfig.m_LightIconScale));

	m_DirLightIconTex = new Texture2D(m_GraphicDevice, XE_GOD_DIRECTIONAL_LIGHT_ICON_TEXTURE_NAME);

	m_OmniLightIconTex = new Texture2D(m_GraphicDevice, XE_GOD_OMNI_LIGHT_ICON_TEXTURE_NAME);

	m_SpotLightIconTex = new Texture2D(m_GraphicDevice, XE_GOD_SPOT_LIGHT_ICON_TEXTURE_NAME);

	m_CameraIconTex = new Texture2D(m_GraphicDevice, XE_GOD_CAMERA_ICON_TEXTURE_NAME);

	m_FrustumShape = new FrustumShape(m_GraphicDevice);

	DrawableGameComponent::Initialize();
}

void GameObjectsDebugVisualizer::LoadContent()
{
	XETODO("Check returns");
	m_DirLightIconTex->CreateFromFile(XE_GOD_DIRECTIONAL_LIGHT_ICON_PATH);
	m_OmniLightIconTex->CreateFromFile(XE_GOD_OMNI_LIGHT_ICON_PATH);
	m_SpotLightIconTex->CreateFromFile(XE_GOD_SPOT_LIGHT_ICON_PATH);
	m_CameraIconTex->CreateFromFile(XE_GOD_CAMERA_ICON_PATH);

	XETODO("Check returns");
	m_DiffuseTextureBasicMaterial->LoadContent();

	XETODO("Check returns");
	m_BasicLineMaterial->LoadContent();

	XETODO("Check returns");
	m_BasicColorMaterial->LoadContent();

	XETODO("Check returns");
	m_QuadShape3D->BuildQuad();

	XETODO("Check returns");
	m_OmniLightShape->BuildLightShape();

	XETODO("Check returns");
	m_SpotLightShape->BuildLightShape();

	XETODO("Check returns");
	m_DirectionalLightShape->BuildLightShape();

	XETODO("Check returns");
	m_FrustumShape->BuildFrustumShape();

	DrawableGameComponent::LoadContent();
}

void GameObjectsDebugVisualizer::Render(const TimerParams& timerParams)
{
	m_GraphicDevice->BeginEvent(L"Game Objects Debug Visualizer");

	XETODO("Check returns");
	if (m_GameObjectsDebugVisualizerConfig.m_GameObjectDebugRenderEnabled)
	{
		SelectedGameObjectDebugRender();
	}

	XETODO("Check returns");
	if (m_GameObjectsDebugVisualizerConfig.m_LightIconDrawEnabled)
	{
		LightIconDraw();
	}

	XETODO("Check returns");
	if (m_GameObjectsDebugVisualizerConfig.m_CameraIconDrawEnabled)
	{
		CameraIconDraw();
	}

	m_GraphicDevice->EndEvent();

	DrawableGameComponent::Render(timerParams);
}

XEResult GameObjectsDebugVisualizer::LightIconDraw()
{
	XETODO("Check returns");

	///////////////////////////////////////////////////
	//Get Light Manager
	LightManager* lightManager = m_GameApp->GetLightManager();

	if (lightManager->GetNumberOfLights() == 0)
	{
		//No need for work as no lights have been created
		return XEResult::Ok;
	}

	///////////////////////////////////////////////////
	//Get Current Main Camera
	Camera* currentCamera = m_CameraUpdater->GetMainCamera();
	if (currentCamera == nullptr)
	{
		return XEResult::CameraNull;
	}

	glm::mat4 worldMatrix = XEMathHelpers::Mat4Identity;
	glm::mat4 rotationMatrix = XEMathHelpers::GetBillboardRotationMatrixFromCameraView(currentCamera->GetViewMatrix());

	///////////////////////////////////////////////////
	//Set VS CB Properties
	ShaderProperties* vsProps = m_DiffuseTextureBasicMaterial->GetVSProps();

	ConstantBuffer* cb = vsProps->GetConstantBuffer(XE_CB_WORLD_VIEW_PROJ_NAME);

	cb->SetValueT<glm::mat4>(XE_CB_VIEW_VAR_NAME, currentCamera->GetViewMatrix());
	cb->SetValueT<glm::mat4>(XE_CB_PROJECTION_VAR_NAME, currentCamera->GetProjectionMatrix());

	///////////////////////////////////////////////////
	//Set Get PS Props to set texture later on
	ShaderProperties* psProps = m_DiffuseTextureBasicMaterial->GetPSProps();

	///////////////////////////////////////////////////
	//Apply Material Properties
	m_DiffuseTextureBasicMaterial->Apply();

	///////////////////////////////////////////////////
	//Set Blend States to Additive
	m_GraphicDevice->SetBlendState(GraphicBlendStates::m_AdditiveState);

	for (auto lightIt : *lightManager)
	{
		Light* light = lightIt.second;

		if (!light->IsEnabled())
		{
			continue;
		}

		Texture2D* texLightIcon = nullptr;

		switch (light->GetLightType())
		{
			case LightType::Spot:
				texLightIcon = m_SpotLightIconTex;
				break;

			case LightType::Omni:
				texLightIcon = m_OmniLightIconTex;
				break;

			case LightType::Directional:
				texLightIcon = m_DirLightIconTex;
				break;

			default:
				XEAssert(false);
				return XEResult::InvalidObjType;
		}

		///////////////////////////////////////////////////
		//Update VS CB with World Transform of Each Light
		worldMatrix = glm::translate(light->GetPosition()) * rotationMatrix * m_LightIconsScale;
		cb->SetValueT<glm::mat4>(XE_CB_WORLD_VAR_NAME, worldMatrix);
		cb->Apply();

		///////////////////////////////////////////////////
		//Update PS Texture with Texture for each light
		psProps->SetTexture(XE_TX_DIFFUSE_TEXTURE_NAME, texLightIcon);
		psProps->ApplyTextureBindings();

		///////////////////////////////////////////////////
		//Draw Quad
		m_QuadShape3D->DrawQuad();
	}

	///////////////////////////////////////////////////
	//Set Blend States to Default
	m_GraphicDevice->SetBlendState(nullptr);

	///////////////////////////////////////////////////
	//unapply  Material Properties
	m_DiffuseTextureBasicMaterial->UnApply();

	///////////////////////////////////////////////////
	//Finish
	return XEResult::Ok;
}

XEResult GameObjectsDebugVisualizer::CameraIconDraw()
{
	XETODO("Check returns");

	///////////////////////////////////////////////////
	//Get Camera Manager
	CameraManager* cameraManager = m_GameApp->GetCameraManager();

	if (cameraManager->GetSize() == 0)
	{
		//No need for work as no camera have been created
		return XEResult::Ok;
	}

	///////////////////////////////////////////////////
	//Get Current Main Camera
	Camera* currentCamera = m_CameraUpdater->GetMainCamera();
	if (currentCamera == nullptr)
	{
		return XEResult::CameraNull;
	}

	glm::mat4 worldMatrix = XEMathHelpers::Mat4Identity;
	glm::mat4 rotationMatrix = XEMathHelpers::GetBillboardRotationMatrixFromCameraView(currentCamera->GetViewMatrix());

	///////////////////////////////////////////////////
	//Set VS CB Properties
	ShaderProperties* vsProps = m_DiffuseTextureBasicMaterial->GetVSProps();

	ConstantBuffer* cb = vsProps->GetConstantBuffer(XE_CB_WORLD_VIEW_PROJ_NAME);

	cb->SetValueT<glm::mat4>(XE_CB_VIEW_VAR_NAME, currentCamera->GetViewMatrix());
	cb->SetValueT<glm::mat4>(XE_CB_PROJECTION_VAR_NAME, currentCamera->GetProjectionMatrix());

	///////////////////////////////////////////////////
	//Set Get PS Props to set texture later on
	ShaderProperties* psProps = m_DiffuseTextureBasicMaterial->GetPSProps();

	///////////////////////////////////////////////////
	//Apply Material Properties
	m_DiffuseTextureBasicMaterial->Apply();

	///////////////////////////////////////////////////
	//Set Blend States to Additive
	m_GraphicDevice->SetBlendState(GraphicBlendStates::m_AdditiveState);

	for (auto cameraIt : *cameraManager)
	{
		Camera* goCamera = cameraIt.second;

		//If displaying camera is the same as the one we are going to draw the icon as
		//skip it.
		if (currentCamera->GetUniqueID() == goCamera->GetUniqueID())
		{
			continue;
		}

		///////////////////////////////////////////////////
		//Update VS CB with World Transform of Each Light
		worldMatrix = glm::translate(goCamera->GetPosition()) * rotationMatrix * m_LightIconsScale;
		cb->SetValueT<glm::mat4>(XE_CB_WORLD_VAR_NAME, worldMatrix);
		cb->Apply();

		///////////////////////////////////////////////////
		//Update PS Texture with Texture for each light
		psProps->SetTexture(XE_TX_DIFFUSE_TEXTURE_NAME, m_CameraIconTex);
		psProps->ApplyTextureBindings();

		///////////////////////////////////////////////////
		//Draw Quad
		m_QuadShape3D->DrawQuad();
	}

	///////////////////////////////////////////////////
	//Set Blend States to Default
	m_GraphicDevice->SetBlendState(nullptr);

	///////////////////////////////////////////////////
	//unapply  Material Properties
	m_DiffuseTextureBasicMaterial->UnApply();

	///////////////////////////////////////////////////
	//Finish
	return XEResult::Ok;
}

XEResult GameObjectsDebugVisualizer::SelectedGameObjectDebugRender()
{
	///////////////////////////////////////////////////
	//Get Current selected Game Object
	GameObject* gameObject = m_GameApp->GetGameObjectManager()->GetSelectedGameObject();
	if (gameObject == nullptr)
	{
		return XEResult::Ok;
	}

	///////////////////////////////////////////////////
	//Get Current Main Camera
	Camera* currentCamera = m_CameraUpdater->GetMainCamera();
	if (currentCamera == nullptr)
	{
		return XEResult::CameraNull;
	}

	///////////////////////////////////////////////////
	//Draw Light Shape if it has a Light
	if (gameObject->HasLightGOC())
	{
		XETODO("Check return");
		GameObjectLightRender(gameObject, currentCamera);
	}

	///////////////////////////////////////////////////
	//Draw Camera Frustum if it has a Camera
	if (gameObject->HasCameraGOC())
	{
		XETODO("Check return");
		GameObjectCameraRender(gameObject, currentCamera);
	}

	return XEResult::Ok;
}

XEResult GameObjectsDebugVisualizer::GameObjectCameraRender(const GameObject* gameObject, const Camera* camera)
{
	///////////////////////////////////////////////////
	//Pre-checks on variables
	XEAssert(gameObject != nullptr);
	XEAssert(camera != nullptr);
	if (gameObject == nullptr || camera == nullptr)
	{
		return XEResult::NullParameter;
	}

	XEAssert(gameObject->HasCameraGOC());
	if (!gameObject->HasCameraGOC())
	{
		return XEResult::NullObj;
	}

	///////////////////////////////////////////////////
	//Get Camera Info
	CameraGOC* cameraGOC = gameObject->GetCameraGOC();
	Camera* goCamera = cameraGOC->GetCamera();

	///////////////////////////////////////////////////
	//Select Light Shape by Type
	if (cameraGOC->IsDrawFrustumEnabled())
	{
		const Frustum* frustum = &goCamera->GetFrustum();

		DrawFrustum(frustum, cameraGOC->IsDrawFrustumSimple(), camera);
	}

	return XEResult::Ok;
}

XEResult GameObjectsDebugVisualizer::GameObjectLightRender(const GameObject* gameObject, const Camera* camera)
{
	///////////////////////////////////////////////////
	//Pre-checks on variables
	XEAssert(gameObject != nullptr);
	XEAssert(camera != nullptr);
	if (gameObject == nullptr || camera == nullptr)
	{
		return XEResult::NullParameter;
	}

	XEAssert(gameObject->HasLightGOC());
	if (!gameObject->HasLightGOC())
	{
		return XEResult::NullObj;
	}

	///////////////////////////////////////////////////
	//Get Light Info
	LightGOC* lightGOC = gameObject->GetLightGOC();
	Light* light = lightGOC->GetLight();

	///////////////////////////////////////////////////
	//Select Light Shape by Type
	switch (light->GetLightType())
	{
		case LightType::Spot:
			return GameObjectSpotLightRender(gameObject, reinterpret_cast<SpotLight*>(light), camera, lightGOC->IsDrawDebugEnabled(), lightGOC->IsDrawFrustumEnabled(), lightGOC->IsDrawFrustumSimple());

		case LightType::Omni:
			return GameObjectOmniLightRender(gameObject, reinterpret_cast<OmniLight*>(light), camera, lightGOC->IsDrawDebugEnabled(), lightGOC->IsDrawFrustumEnabled(), lightGOC->IsDrawFrustumSimple());

		case LightType::Directional:
			return GameObjectDirectionalLightRender(gameObject, reinterpret_cast<DirectionalLight*>(light), camera, lightGOC->IsDrawDebugEnabled(), lightGOC->IsDrawFrustumEnabled(), lightGOC->IsDrawFrustumSimple(), lightGOC->IsDrawFrustumCascadesEnabled());

		default:
			XEAssert(false);
			return XEResult::InvalidObjType;
	}
}

XEResult GameObjectsDebugVisualizer::GameObjectOmniLightRender(const GameObject* gameObject, const OmniLight* omniLight, const Camera* camera, bool drawDebug, bool renderFrustum, bool frustumSimple)
{
	XEAssert(gameObject != nullptr);
	XEAssert(omniLight != nullptr);
	XEAssert(camera != nullptr);
	if (gameObject == nullptr || omniLight == nullptr || camera == nullptr)
	{
		return XEResult::NullParameter;
	}

	XETODO("Check returns");

	if (drawDebug)
	{
		///////////////////////////////////////////////////
		//Calculate World Transform for Near Attenuation
		glm::mat4 translation = glm::translate(gameObject->GetWorldPosition());
		glm::mat4 rotation = glm::mat4_cast(gameObject->GetWorldRotationQuat());

		float scaleByNear = static_cast<float>(omniLight->GetNearAttenuation()) * 2.0f;
		glm::mat4 scaleNear = glm::scale(glm::vec3(scaleByNear, scaleByNear, scaleByNear));

		float scaleByFar = static_cast<float>(omniLight->GetFarAttenuation()) * 2.0f;
		glm::mat4 scaleFar = glm::scale(glm::vec3(scaleByFar, scaleByFar, scaleByFar));

		glm::mat4 worldTransformNear = translation * rotation * scaleNear;
		glm::mat4 worldTransformFar = translation * rotation * scaleFar;

		///////////////////////////////////////////////////
		//Set PS CB Properties
		ShaderProperties* psProps = m_BasicLineMaterial->GetPSProps();
		ConstantBuffer* psCB = psProps->GetConstantBuffer(XE_CB_COLOR_NAME);

		psCB->SetValueT<glm::vec4>(XE_CB_COLOR_VAR_NAME, XE_LIGHT_SHAPE_DEBUG_DEFAULT_NEAR_COLOR);

		///////////////////////////////////////////////////
		//Apply Material Properties
		m_BasicLineMaterial->Apply();

		///////////////////////////////////////////////////
		//Set VS CB Properties (After Material Apply so we only apply CB when we change it)
		ShaderProperties* vsProps = m_BasicLineMaterial->GetVSProps();
		ConstantBuffer* vsCB = vsProps->GetConstantBuffer(XE_CB_WORLD_VIEW_PROJ_NAME);

		vsCB->SetValueT<glm::mat4>(XE_CB_VIEW_VAR_NAME, camera->GetViewMatrix());
		vsCB->SetValueT<glm::mat4>(XE_CB_PROJECTION_VAR_NAME, camera->GetProjectionMatrix());

		///////////////////////////////////////////////////
		//Draw Light Near Shape 
		vsCB->SetValueT<glm::mat4>(XE_CB_WORLD_VAR_NAME, worldTransformNear);
		vsCB->Apply();

		m_OmniLightShape->DrawLightShape();

		///////////////////////////////////////////////////
		//Draw Light Far Shape 
		vsCB->SetValueT<glm::mat4>(XE_CB_WORLD_VAR_NAME, worldTransformFar);
		vsCB->Apply();

		psCB->SetValueT<glm::vec4>(XE_CB_COLOR_VAR_NAME, XE_LIGHT_SHAPE_DEBUG_DEFAULT_FAR_COLOR);
		psCB->Apply();

		m_OmniLightShape->DrawLightShape();

		///////////////////////////////////////////////////
		//Unapply Material Properties
		m_BasicLineMaterial->UnApply();
	}

	return XEResult::Ok;
}

XEResult GameObjectsDebugVisualizer::GameObjectSpotLightRender(const GameObject* gameObject, const SpotLight* spotLight, const Camera* camera, bool drawDebug, bool renderFrustum, bool frustumSimple)
{
	///////////////////////////////////////////////////
	//Pre-checks
	XEAssert(gameObject != nullptr);
	XEAssert(camera != nullptr);
	if (gameObject == nullptr || spotLight == nullptr || camera == nullptr)
	{
		return XEResult::NullParameter;
	}

	XETODO("Check returns");

	if (drawDebug)
	{
		///////////////////////////////////////////////////
		//Update Spot Light Info
		XETODO("Check return");
		m_SpotLightShape->UpdateLightInfo(spotLight->GetNearAttenuation(), spotLight->GetFarAttenuation(), spotLight->GetAngle(), spotLight->GetFallOffAngle());

		///////////////////////////////////////////////////
		//Calculate World Transform for Near Attenuation
		glm::mat4 translation = glm::translate(gameObject->GetWorldPosition());
		glm::mat4 rotation = glm::mat4_cast(gameObject->GetWorldRotationQuat());

		glm::mat4 worldTransform = translation * rotation;

		///////////////////////////////////////////////////
		//Set VS CB Properties
		ShaderProperties* vsProps = m_BasicColorMaterial->GetVSProps();
		ConstantBuffer* vsCB = vsProps->GetConstantBuffer(XE_CB_WORLD_VIEW_PROJ_NAME);

		vsCB->SetValueT<glm::mat4>(XE_CB_WORLD_VAR_NAME, worldTransform);
		vsCB->SetValueT<glm::mat4>(XE_CB_VIEW_VAR_NAME, camera->GetViewMatrix());
		vsCB->SetValueT<glm::mat4>(XE_CB_PROJECTION_VAR_NAME, camera->GetProjectionMatrix());

		///////////////////////////////////////////////////
		//Apply Material Properties
		m_BasicColorMaterial->Apply();

		///////////////////////////////////////////////////
		//Draw Light 
		m_SpotLightShape->DrawLightShape();

		///////////////////////////////////////////////////
		//Unapply Material Properties
		m_BasicColorMaterial->UnApply();
	}

	///////////////////////////////////////////////////
	//Draw Light Frustum if Enabled
	if (renderFrustum)
	{
		const Frustum* frustum = spotLight->GetFrustum();

		DrawFrustum(frustum, frustumSimple, camera);
	}

	return XEResult::Ok;
}

XEResult GameObjectsDebugVisualizer::GameObjectDirectionalLightRender(const GameObject* gameObject, const DirectionalLight* directionalLight, const Camera* camera, bool drawDebug, bool renderFrustum, bool frustumSimple, bool drawFustumCascades)
{
	XEAssert(gameObject != nullptr);
	XEAssert(directionalLight != nullptr);
	XEAssert(camera != nullptr);
	if (gameObject == nullptr || directionalLight == nullptr || camera == nullptr)
	{
		return XEResult::NullParameter;
	}

	XETODO("Check returns");

	if (drawDebug)
	{
		///////////////////////////////////////////////////
		//Calculate World Transform for Near Attenuation
		glm::vec3 lightWorldPos = gameObject->GetWorldPosition();

		glm::mat4 translation = glm::translate(lightWorldPos);
		glm::mat4 rotation = glm::mat4_cast(gameObject->GetWorldRotationQuat());

		glm::mat4 scale = glm::scale(glm::vec3(m_GameObjectsDebugVisualizerConfig.m_DirectionalLightShapeScale, m_GameObjectsDebugVisualizerConfig.m_DirectionalLightShapeScale, m_GameObjectsDebugVisualizerConfig.m_DirectionalLightShapeScale));

		glm::mat4 worldTransform = translation * rotation * scale;

		///////////////////////////////////////////////////
		//Set VS CB Properties (After Material Apply so we only apply CB when we change it)
		ShaderProperties* vsProps = m_BasicLineMaterial->GetVSProps();
		ConstantBuffer* vsCB = vsProps->GetConstantBuffer(XE_CB_WORLD_VIEW_PROJ_NAME);

		vsCB->SetValueT<glm::mat4>(XE_CB_WORLD_VAR_NAME, worldTransform);
		vsCB->SetValueT<glm::mat4>(XE_CB_VIEW_VAR_NAME, camera->GetViewMatrix());
		vsCB->SetValueT<glm::mat4>(XE_CB_PROJECTION_VAR_NAME, camera->GetProjectionMatrix());

		///////////////////////////////////////////////////
		//Set PS CB Properties
		ShaderProperties* psProps = m_BasicLineMaterial->GetPSProps();
		ConstantBuffer* psCB = psProps->GetConstantBuffer(XE_CB_COLOR_NAME);

		psCB->SetValueT<glm::vec4>(XE_CB_COLOR_VAR_NAME, XE_LIGHT_SHAPE_DEBUG_DEFAULT_NEAR_COLOR);

		///////////////////////////////////////////////////
		//Apply Material Properties
		m_BasicLineMaterial->Apply();

		///////////////////////////////////////////////////
		//Draw Light
		m_DirectionalLightShape->DrawLightShape();

		///////////////////////////////////////////////////
		//Unapply Material Properties
		m_BasicLineMaterial->UnApply();
	}

	///////////////////////////////////////////////////
	//Draw Light Frustum if Enabled
	if (renderFrustum)
	{
		if (drawFustumCascades)
		{
			const LightCascadeInfo& lightCascadeInfo = directionalLight->GetLightCascadeInfo();
			for (size_t i = 0; i < lightCascadeInfo.m_CascadeFrustums.size(); i++)
			{
				const Frustum& frustum = lightCascadeInfo.m_CascadeFrustums[i];

				DrawFrustum(&frustum, frustumSimple, camera);
			}
		}
		else
		{
			const Frustum* frustum = directionalLight->GetFrustum();

			DrawFrustum(frustum, frustumSimple, camera);
		}
	}

	return XEResult::Ok;
}

XEResult GameObjectsDebugVisualizer::DrawFrustum(const Frustum* frustum, bool simple, const Camera* camera)
{
	XEAssert(camera != nullptr);
	XEAssert(frustum != nullptr);
	if (frustum == nullptr || camera == nullptr)
	{
		return XEResult::NullParameter;
	}

	///////////////////////////////////////////////////
	//Update Frustum Shape
	XETODO("Check return");
	m_FrustumShape->UpdateFrustumShape(frustum);

	///////////////////////////////////////////////////
	//Set VS CB Properties
	ShaderProperties* vsProps = m_BasicColorMaterial->GetVSProps();
	ConstantBuffer* vsCB = vsProps->GetConstantBuffer(XE_CB_WORLD_VIEW_PROJ_NAME);

	vsCB->SetValueT<glm::mat4>(XE_CB_WORLD_VAR_NAME, XEMathHelpers::Mat4Identity);
	vsCB->SetValueT<glm::mat4>(XE_CB_VIEW_VAR_NAME, camera->GetViewMatrix());
	vsCB->SetValueT<glm::mat4>(XE_CB_PROJECTION_VAR_NAME, camera->GetProjectionMatrix());

	///////////////////////////////////////////////////
	//Apply Material Properties
	m_BasicColorMaterial->Apply();

	///////////////////////////////////////////////////
	//Draw Frustum Shape
	XETODO("check return");
	m_FrustumShape->DrawFrustumShape(simple);

	///////////////////////////////////////////////////
	//Unapply Material Properties
	m_BasicColorMaterial->UnApply();

	///////////////////////////////////////////////////
	//Finish
	return XEResult::Ok;
}