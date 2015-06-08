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
#include "cppformat\format.h"

/***************************
*   Game Engine Includes   *
****************************/
#include "GameObject.h"
#include "AudioManager.h"
#include "PhysicsActor.h"
#include "Lights\Light.h"
#include "GraphicDevice.h"
#include "Logger\Logger.h"
#include "PhysicCollider.h"
#include "XML\XEXMLWriter.h"
#include "XML\XEXMLParser.h"
#include "Lights\SpotLight.h"
#include "Lights\OmniLight.h"
#include "GameObjectManager.h"
#include "PhysicColliderBox.h"
#include "Lights\LightManager.h"
#include "PhysicColliderSphere.h"
#include "GameAssets\GameAsset.h"
#include "Camera\CameraManager.h"
#include "Lights\DirectionalLight.h"
#include "GameAssets\Assets\MeshAsset.h"
#include "GameAssets\\GameAssetManager.h"
#include "GameObject\GameObjectManager.h"
#include "GameAssets\Assets\ShaderAsset.h"
#include "GameObject\Components\MeshGOC.h"
#include "AngelScript\AngelScriptManager.h"
#include "GameObject\Components\LightGOC.h"
#include "Shaders\Buffers\ConstantBuffer.h"
#include "GameObject\Components\CameraGOC.h"
#include "GameObject\Components\PhysicsGOC.h"
#include "Localization\LocalizationManager.h"
#include "Localization\LocalizationManagerDefs.h"
#include "GameObject\Components\AudioSourceGOC.h"
#include "Shaders\Bindings\ShaderTextureBinding.h"
#include "GameObject\Components\MeshMaterialGOC.h"
#include "GameObject\Components\MeshAnimationGOC.h"
#include "GameObject\Components\AudioListenerGOC.h"
#include "GameObject\Scripts\GameObjectScriptDefs.h"
#include "GameObject\Components\GameObjectScriptGOC.h"
#include "Graphic Extensions\Shader Extensions\Properties\ShaderProperties.h"


//Always include last
#include "Memory\MemLeaks.h"

/********************
*   Function Defs   *
*********************/
XETODO("Check for Mutex");
GameObjectManager::GameObjectManager(GraphicDevice* graphicDevice, GameAssetManager* gameAssetManager, GameObjectScriptManager* gameObjectScriptManager, AngelScriptManager* angelScriptManager, LightManager* lightManager, CameraManager* cameraManager, AudioManager* audioManager, PhysicsManager* physicsManager)
	: m_GraphicDevice(graphicDevice)
	, m_GameAssetManager(gameAssetManager)
	, m_GameObjectScriptManager(gameObjectScriptManager)
	, m_AngelScriptManager(angelScriptManager)
	, m_LightManager(lightManager)
	, m_CameraManager(cameraManager)
	, m_AudioManager(audioManager)
	, m_PhysicsManager(physicsManager)
{
	XEAssert(m_GraphicDevice != nullptr);
	XEAssert(m_GameAssetManager != nullptr);
	XEAssert(m_GameObjectScriptManager != nullptr);
	XEAssert(m_AngelScriptManager != nullptr);
	XEAssert(m_LightManager != nullptr);
	XEAssert(m_CameraManager != nullptr);
	XEAssert(m_AudioManager != nullptr);
	XEAssert(m_PhysicsManager != nullptr);
}

GameObjectManager::~GameObjectManager()
{
	CleanUp();
}

void GameObjectManager::CleanUp()
{
	for(auto gameObjectIt : m_GameObjectMap)
	{
		DeleteMem(gameObjectIt.second);
	}

	m_GameObjectMap.clear();
}

void GameObjectManager::GameObjectMoveToChild(uint64_t id)
{
	if(!GameObjectExistsTopLevel(id))
	{
		XEAssert(false);
		XETODO("add log");
		return;
	}

	//////////////////////////////////////
	//Remove from Map to become a Child
	m_GameObjectMap.erase(id);

	XETODO("add debug log here");
}

bool GameObjectManager::GameObjectExistsTopLevel(uint64_t goID) const
{
	return (m_GameObjectMap.find(goID) != m_GameObjectMap.end());
}

GameObject* GameObjectManager::SearchGameObjectChilds(uint64_t goID, GameObject* gameObject) const
{
	XEAssert(gameObject != nullptr);
	if(gameObject == nullptr)
	{
		return nullptr;
	}

	if(gameObject->GetUniqueID() == goID)
	{
		return gameObject;
	}

	for(auto goIt : *gameObject)
	{
		GameObject* found = SearchGameObjectChilds(goID, goIt.second);

		if(found != nullptr)
		{
			return found;
		}
	}

	return nullptr;
}

GameObject* GameObjectManager::GetGameObject(uint64_t goID) const
{
	if(GameObjectExistsTopLevel(goID))
	{
		auto foundIt = m_GameObjectMap.find(goID);
		return foundIt->second;
	}

	for(auto goIt : m_GameObjectMap)
	{
		GameObject* found = SearchGameObjectChilds(goID, goIt.second);

		if(found != nullptr)
		{
			return found;
		}
	}

	return nullptr;
}

XEResult GameObjectManager::AddGameObject(GameObject* gameObject)
{
	XEAssert(gameObject != nullptr);

	if(gameObject == nullptr)
	{
		return XEResult::NullParameter;
	}

	if(GameObjectExistsTopLevel(gameObject->GetUniqueID()))
	{
		return XEResult::ObjExists;
	}

	if(gameObject->m_ManagerID != 0 && gameObject->GetManagerID() != this->GetUniqueID())
	{
		XETODO("better return and error message");
		return XEResult::Fail;
	}

	if (gameObject->m_ManagerID == 0)
	{
		gameObject->m_GameObjectMoveToChildCallback = std::bind(&GameObjectManager::GameObjectMoveToChild, this, std::placeholders::_1);

		gameObject->m_ManagerID = this->GetUniqueID();
	}

	if(gameObject->IsChild())
	{
		gameObject->GetParentGameObject()->RemoveChild(gameObject->GetUniqueID());
	}

	m_GameObjectMap[gameObject->GetUniqueID()] = gameObject;

	return XEResult::Ok;
}

XEResult GameObjectManager::SetSelectedGameObject(GameObject* gameObject)
{
	if (gameObject->m_ManagerID != 0 && gameObject->GetManagerID() != this->GetUniqueID())
	{
		XETODO("better return and error message");
		return XEResult::Fail;
	}

	m_SelectedGameObject = gameObject;

	return XEResult::Ok;
}

XEResult GameObjectManager::SaveToXML(const std::wstring& file) const
{
	if (file.empty())
	{
		return XEResult::EmptyFilename;
	}

	XEXMLWriter xmlWriter;
	XEResult ret = XEResult::Ok;

	ret = xmlWriter.CreateXML(file, true);
	if (ret != XEResult::Ok)
	{
		XETODO("Better return code");
		return XEResult::Fail;
	}

	ret = xmlWriter.StartNode(XE_GAME_OBJ_MANAGER_NODE_NAME);
	if (ret != XEResult::Ok)
	{
		XETODO("Better return code");
		return XEResult::Fail;
	}

	for (auto gameObjectPair : m_GameObjectMap)
	{
		ret = SaveToXMLGameObject(xmlWriter, gameObjectPair.second);
		if (ret != XEResult::Ok)
		{
			XETODO("Better return code");
			return XEResult::Fail;
		}
	}

	ret = xmlWriter.EndNode();
	if (ret != XEResult::Ok)
	{
		XETODO("Better return code");
		return XEResult::Fail;
	}

	ret = xmlWriter.FinalizeXML();
	if (ret != XEResult::Ok)
	{
		XETODO("Better return code");
		return XEResult::Fail;
	}

	return XEResult::Ok;
}

XEResult GameObjectManager::SaveToXMLGameObject(XEXMLWriter& xmlWriter, GameObject* gameObject) const
{
	if (gameObject == nullptr)
	{
		return XEResult::NullObj;
	}

	XEResult ret = XEResult::Ok;

	////////////////////////////
	//Game Object Node
	ret = xmlWriter.StartNode(XE_GAME_OBJ_NODE_NAME);
	if (ret != XEResult::Ok)
	{
		XETODO("Better return code");
		return XEResult::Fail;
	}

	////////////////////////////
	//Set Game Object Props
	xmlWriter.WriteString(XE_GAME_OBJ_NAME_PROP, gameObject->GetName());
	xmlWriter.WriteVect3f(XE_GAME_OBJ_POSITION_PROP, gameObject->GetPosition());
	xmlWriter.WriteVect3f(XE_GAME_OBJ_ROTATION_PROP, gameObject->GetRotation());
	xmlWriter.WriteVect3f(XE_GAME_OBJ_SCALE_PROP, gameObject->GetScale());

	////////////////////////////
	//Write Game Object Components
	ret = SaveToXMLGameObjectComponents(xmlWriter, gameObject);
	if (ret != XEResult::Ok)
	{
		XETODO("Better return code");
		return XEResult::Fail;
	}

	////////////////////////////
	//Write Game Object Childs
	ret = xmlWriter.StartNode(XE_GAME_OBJ_CHILDS_NODE_NAME);
	if (ret != XEResult::Ok)
	{
		XETODO("Better return code");
		return XEResult::Fail;
	}

	for (auto childGameObjectPair : *gameObject)
	{
		ret = SaveToXMLGameObject(xmlWriter, childGameObjectPair.second);
		if (ret != XEResult::Ok)
		{
			XETODO("Better return code");
			return XEResult::Fail;
		}
	}

	////////////////////////////
	//End of XE_GAME_OBJ_CHILDS_NODE_NAME
	ret = xmlWriter.EndNode();
	if (ret != XEResult::Ok)
	{
		XETODO("Better return code");
		return XEResult::Fail;
	}

	////////////////////////////
	//End of XE_GAME_OBJ_NODE_NAME
	ret = xmlWriter.EndNode();
	if (ret != XEResult::Ok)
	{
		XETODO("Better return code");
		return XEResult::Fail;
	}

	////////////////////////////
	//Finalize
	return XEResult::Ok;
}

XEResult GameObjectManager::SaveToXMLGameObjectComponents(XEXMLWriter& xmlWriter, GameObject* gameObject) const
{
	if (gameObject == nullptr)
	{
		return XEResult::NullParameter;
	}

	XEResult ret = XEResult::Ok;

	////////////////////////////
	//Write Game Object Components
	ret = xmlWriter.StartNode(XE_GAME_OBJ_COMPONENTS_NODE_NAME);
	if (ret != XEResult::Ok)
	{
		XETODO("Better return code");
		return XEResult::Fail;
	}

	ret = SaveToXMLMeshComponent(xmlWriter, gameObject);
	if (ret != XEResult::Ok)
	{
		XETODO("Better return code");
		return XEResult::Fail;
	}

	ret = SaveToXMLMeshMaterialsComponent(xmlWriter, gameObject);
	if (ret != XEResult::Ok)
	{
		XETODO("Better return code");
		return XEResult::Fail;
	}

	ret = SaveToXMLGameObjectScriptsComponent(xmlWriter, gameObject);
	if (ret != XEResult::Ok)
	{
		XETODO("Better return code");
		return XEResult::Fail;
	}

	ret = SaveToXMLLightComponent(xmlWriter, gameObject);
	if (ret != XEResult::Ok)
	{
		XETODO("Better return code");
		return XEResult::Fail;
	}

	ret = SaveToXMLMeshAnimationComponent(xmlWriter, gameObject);
	if (ret != XEResult::Ok)
	{
		XETODO("Better return code");
		return XEResult::Fail;
	}

	ret = SaveToXMLCameraComponent(xmlWriter, gameObject);
	if (ret != XEResult::Ok)
	{
		XETODO("Better return code");
		return XEResult::Fail;
	}

	ret = SaveToXMLAudioListenerComponent(xmlWriter, gameObject);
	if (ret != XEResult::Ok)
	{
		XETODO("Better return code");
		return XEResult::Fail;
	}

	ret = SaveToXMLAudioSourceComponent(xmlWriter, gameObject);
	if (ret != XEResult::Ok)
	{
		XETODO("Better return code");
		return XEResult::Fail;
	}

	ret = SaveToXMLPhysicsComponent(xmlWriter, gameObject);
	if (ret != XEResult::Ok)
	{
		XETODO("Better return code");
		return XEResult::Fail;
	}

	////////////////////////////
	//End of XE_GAME_OBJ_COMPONENTS_NODE_NAME
	ret = xmlWriter.EndNode();
	if (ret != XEResult::Ok)
	{
		XETODO("Better return code");
		return XEResult::Fail;
	}

	////////////////////////////
	//Finalize
	return XEResult::Ok;
}

XEResult GameObjectManager::SaveToXMLMeshComponent(XEXMLWriter& xmlWriter, GameObject* gameObject) const
{
	if (gameObject == nullptr)
	{
		return XEResult::NullParameter;
	}

	if (!gameObject->HasMeshGOC())
	{
		return XEResult::Ok;
	}

	XEResult ret = XEResult::Ok;

	////////////////////////////
	//Write Game Object Components
	ret = xmlWriter.StartNode(XE_GAME_OBJ_GOC_MESH_NODE_NAME);
	if (ret != XEResult::Ok)
	{
		XETODO("Better return code");
		return XEResult::Fail;
	}

	MeshGOC* goc = gameObject->GetMeshGOC();

	////////////////////////////
	//Set Components Props
	xmlWriter.WriteUInt64(XE_GAME_OBJ_COMPONENT_ASSETID_PROP, goc->GetMeshAssetID());

	////////////////////////////
	//End of XE_GAME_OBJ_GOC_MESH_NODE_NAME
	ret = xmlWriter.EndNode();
	if (ret != XEResult::Ok)
	{
		XETODO("Better return code");
		return XEResult::Fail;
	}

	return XEResult::Ok;
}

XEResult GameObjectManager::SaveToXMLMeshMaterialsComponent(XEXMLWriter& xmlWriter, GameObject* gameObject) const
{
	if (gameObject == nullptr)
	{
		return XEResult::NullParameter;
	}

	if (!gameObject->HasMaterialGOCs())
	{
		return XEResult::Ok;
	}

	XEResult ret = XEResult::Ok;

	////////////////////////////
	//Write Game Object Components
	ret = xmlWriter.StartNode(XE_GAME_OBJ_GOC_MATS_NODE_NAME);
	if (ret != XEResult::Ok)
	{
		XETODO("Better return code");
		return XEResult::Fail;
	}

	const MeshMaterialsGOCList& materialList = gameObject->GetMeshMaterialsGOCList();
	for (auto material : materialList)
	{
		xmlWriter.WriteString(XE_GAME_OBJ_GOC_MAT_NAME_PROP, material->GetName());

		////////////////////////////
		//Material Component
		ret = xmlWriter.StartNode(XE_GAME_OBJ_GOC_MAT_NODE_NAME);
		if (ret != XEResult::Ok)
		{
			XETODO("Better return code");
			return XEResult::Fail;
		}

		if (material->GetVertexShaderResource() != nullptr)
		{
			ret = SaveToXMLShader(xmlWriter, ShaderType::VertexShader, material->GetVertexShaderAssetID(), material->GetVertexShaderProperties());
			if (ret != XEResult::Ok)
			{
				XETODO("Better return code");
				return XEResult::Fail;
			}
		}

		if (material->GetPixelShaderResource() != nullptr)
		{
			ret = SaveToXMLShader(xmlWriter, ShaderType::PixelShader, material->GetPixelShaderAssetID(), material->GetPixelShaderProperties());
			if (ret != XEResult::Ok)
			{
				XETODO("Better return code");
				return XEResult::Fail;
			}
		}

		if (material->GetGeometryShaderResource() != nullptr)
		{
			ret = SaveToXMLShader(xmlWriter, ShaderType::GeometryShader, material->GetGeometryShaderAssetID(), material->GetGeometryShaderProperties());
			if (ret != XEResult::Ok)
			{
				XETODO("Better return code");
				return XEResult::Fail;
			}
		}

		if (material->GetComputeShaderResource() != nullptr)
		{
			ret = SaveToXMLShader(xmlWriter, ShaderType::ComputeShader, material->GetComputeShaderAssetID(), material->GetComputeShaderProperties());
			if (ret != XEResult::Ok)
			{
				XETODO("Better return code");
				return XEResult::Fail;
			}
		}

		if (material->GetHullShaderResource() != nullptr)
		{
			ret = SaveToXMLShader(xmlWriter, ShaderType::HullShader, material->GetHullShaderAssetID(), material->GetHullShaderProperties());
			if (ret != XEResult::Ok)
			{
				XETODO("Better return code");
				return XEResult::Fail;
			}
		}

		if (material->GetDomainShaderResource() != nullptr)
		{
			ret = SaveToXMLShader(xmlWriter, ShaderType::DomainShader, material->GetDomainShaderAssetID(), material->GetDomainShaderProperties());
			if (ret != XEResult::Ok)
			{
				XETODO("Better return code");
				return XEResult::Fail;
			}
		}

		////////////////////////////
		//End of XE_GAME_OBJ_GOC_MAT_NODE_NAME
		ret = xmlWriter.EndNode();
		if (ret != XEResult::Ok)
		{
			XETODO("Better return code");
			return XEResult::Fail;
		}
	}

	////////////////////////////
	//End of XE_GAME_OBJ_GOC_MAT_NODE_NAME
	ret = xmlWriter.EndNode();
	if (ret != XEResult::Ok)
	{
		XETODO("Better return code");
		return XEResult::Fail;
	}

	return XEResult::Ok;
}

XEResult GameObjectManager::SaveToXMLShader(XEXMLWriter& xmlWriter, ShaderType shaderType, uint64_t assetID, ShaderProperties* properties) const
{
	if (properties == nullptr)
	{
		return XEResult::NullParameter;
	}

	XEResult ret = XEResult::Ok;

	ret = xmlWriter.StartNode(XE_GAME_OBJ_GOC_MAT_SHADER_NODE_NAME);
	if (ret != XEResult::Ok)
	{
		XETODO("Better return code");
		return XEResult::Fail;
	}

	xmlWriter.WriteUInt64(XE_GAME_OBJ_COMPONENT_ASSETID_PROP, assetID);
	xmlWriter.WriteUInt(XE_GAME_OBJ_GOC_MAT_SHADER_TYPE_PROP, (uint32_t)shaderType);

	ret = SaveToXMLShaderProperties(xmlWriter, properties);
	if (ret != XEResult::Ok)
	{
		XETODO("Better return code");
		return XEResult::Fail;
	}

	////////////////////////////
	//End of XE_GAME_OBJ_GOC_MAT_SHADER_NODE_NAME
	ret = xmlWriter.EndNode();
	if (ret != XEResult::Ok)
	{
		XETODO("Better return code");
		return XEResult::Fail;
	}

	return XEResult::Ok;
}

XEResult GameObjectManager::SaveToXMLShaderProperties(XEXMLWriter& xmlWriter, ShaderProperties* properties) const
{
	if (properties == nullptr)
	{
		return XEResult::NullParameter;
	}

	XEResult ret = XEResult::Ok;

	ret = xmlWriter.StartNode(XE_GAME_OBJ_GOC_MAT_SHADER_PROPS_NODE_NAME);
	if (ret != XEResult::Ok)
	{
		XETODO("Better return code");
		return XEResult::Fail;
	}

	uint32_t cbSize = properties->GetNumberOfConstantBuffers();
	for (uint32_t i = 0; i < cbSize; i++)
	{
		ConstantBuffer* cb = properties->GetConstantBuffer(i);

		///////////////////////////////////////////
		//Only Display if CB contains User Variables
		if (cb->ContainsUserVariables())
		{
			ret = xmlWriter.StartNode(XE_GAME_OBJ_GOC_MAT_SHADER_CB_NODE_NAME);
			if (ret != XEResult::Ok)
			{
				XETODO("Better return code");
				return XEResult::Fail;
			}

			xmlWriter.WriteString(XE_GAME_OBJ_GOC_MAT_SHADER_CB_NAME_PROP, cb->GetName());

			///////////////////////////////////////////
			//Insert CB Variables
			uint32_t numVars = cb->GetNumberOfVariables();
			for (uint32_t j = 0; j < numVars; j++)
			{
				const ShaderCustomVariable* scv = cb->GetVariable(j);

				if (!scv->m_UserVariable)
				{
					continue;
				}

				ret = xmlWriter.StartNode(XE_GAME_OBJ_GOC_MAT_SHADER_VAR_NODE_NAME);
				if (ret != XEResult::Ok)
				{
					XETODO("Better return code");
					return XEResult::Fail;
				}

				SaveToXMLCBShaderVariable(xmlWriter, scv, cb);

				////////////////////////////
				//End of XE_GAME_OBJ_GOC_MAT_SHADER_VAR_NODE_NAME
				ret = xmlWriter.EndNode();
				if (ret != XEResult::Ok)
				{
					XETODO("Better return code");
					return XEResult::Fail;
				}
			}

			////////////////////////////
			//End of XE_GAME_OBJ_GOC_MAT_SHADER_CB_NODE_NAME
			ret = xmlWriter.EndNode();
			if (ret != XEResult::Ok)
			{
				XETODO("Better return code");
				return XEResult::Fail;
			}
		}
	}

	uint32_t textureBindingsSize = properties->GetNumberOfShaderTextureBindings();
	for (uint32_t i = 0; i < textureBindingsSize; i++)
	{
		const ShaderTextureBinding* textureBinding = properties->GetShaderTextureBinding(i);

		if (!textureBinding->HasTexture())
		{
			continue;
		}

		ret = xmlWriter.StartNode(XE_GAME_OBJ_GOC_MAT_TEXTURE_NODE_NAME);
		if (ret != XEResult::Ok)
		{
			XETODO("Better return code");
			return XEResult::Fail;
		}

		xmlWriter.WriteString(XE_GAME_OBJ_GOC_MAT_TEXTURE_NAME_PROP, textureBinding->GetName());
		xmlWriter.WriteUInt64(XE_GAME_OBJ_COMPONENT_ASSETID_PROP, properties->GetTextureAssetID(textureBinding->GetName()));

		////////////////////////////
		//End of XE_GAME_OBJ_GOC_MAT_TEXTURE_NODE_NAME
		ret = xmlWriter.EndNode();
		if (ret != XEResult::Ok)
		{
			XETODO("Better return code");
			return XEResult::Fail;
		}
	}

	////////////////////////////
	//End of XE_GAME_OBJ_GOC_MAT_SHADER_PROPS_NODE_NAME
	ret = xmlWriter.EndNode();
	if (ret != XEResult::Ok)
	{
		XETODO("Better return code");
		return XEResult::Fail;
	}

	return XEResult::Ok;
}

XEResult GameObjectManager::SaveToXMLCBShaderVariable(XEXMLWriter& xmlWriter, const ShaderCustomVariable* scv, ConstantBuffer* cb) const
{
	if (cb == nullptr || scv == nullptr)
	{
		return XEResult::NullParameter;
	}

	XEResult ret = XEResult::Ok;

	xmlWriter.WriteString(XE_GAME_OBJ_GOC_MAT_SHADER_VAR_NAME_PROP, scv->m_Name);
	xmlWriter.WriteUInt(XE_GAME_OBJ_GOC_MAT_SHADER_VAR_CLASS_PROP, (uint32_t)scv->m_ShaderVariableClass);
	xmlWriter.WriteUInt(XE_GAME_OBJ_GOC_MAT_SHADER_VAR_TYPE_PROP, (uint32_t)scv->m_ShaderVariableType);
	xmlWriter.WriteUInt(XE_GAME_OBJ_GOC_MAT_SHADER_COLUMNS_PROP, scv->m_Columns);
	xmlWriter.WriteUInt(XE_GAME_OBJ_GOC_MAT_SHADER_ROWS_PROP, scv->m_Rows);

	switch (scv->m_ShaderVariableClass)
	{
		case ShaderVariableClass::Scalar:
			ret = SaveToXMLCBScalar(xmlWriter, cb, scv->m_Name, scv->m_ShaderVariableType);
			break;

		case ShaderVariableClass::Vector:
			ret = SaveToXMLCBVector(xmlWriter, cb, scv->m_Name, scv->m_ShaderVariableType, scv->m_Columns);
			break;

		case ShaderVariableClass::Matrix:
			ret = SaveToXMLCBMatrix(xmlWriter, cb, scv->m_Name, scv->m_ShaderVariableType, scv->m_Columns, scv->m_Rows);
			break;

		default:
			break;
	}

	if (ret != XEResult::Ok)
	{
		XETODO("Better return code");
		return XEResult::Fail;
	}

	return XEResult::Ok;
}

XEResult GameObjectManager::SaveToXMLCBScalar(XEXMLWriter& xmlWriter, ConstantBuffer* cb, const std::wstring& varName, ShaderVariableType varType) const
{
	if (cb == nullptr)
	{
		return XEResult::NullParameter;
	}

	switch (varType)
	{
		case ShaderVariableType::Boolean:
			{
				bool varData = false;
				cb->GetValueT<bool>(varName, varData);

				xmlWriter.WriteBool(XE_GAME_OBJ_GOC_MAT_SHADER_VAR_VALUE_PROP, varData);
			}
			break;

		case ShaderVariableType::Int:
			{
				int32_t varData = 0;
				cb->GetValueT<int32_t>(varName, varData);

				xmlWriter.WriteInt(XE_GAME_OBJ_GOC_MAT_SHADER_VAR_VALUE_PROP, varData);
			}
			break;

		case ShaderVariableType::UInt:
			{
				uint32_t varData = 0;
				cb->GetValueT<uint32_t>(varName, varData);

				xmlWriter.WriteUInt(XE_GAME_OBJ_GOC_MAT_SHADER_VAR_VALUE_PROP, varData);
			}
			break;

		case ShaderVariableType::UInt8:
			{
				uint8_t varData = 0;
				cb->GetValueT<uint8_t>(varName, varData);

				xmlWriter.WriteUInt8(XE_GAME_OBJ_GOC_MAT_SHADER_VAR_VALUE_PROP, varData);
			}
			break;

		case ShaderVariableType::Float:
			{
				float varData = 0;
				cb->GetValueT<float>(varName, varData);

				xmlWriter.WriteFloat(XE_GAME_OBJ_GOC_MAT_SHADER_VAR_VALUE_PROP, varData);
			}
			break;

		case ShaderVariableType::Double:
			{
				double varData = 0;
				cb->GetValueT<double>(varName, varData);

				xmlWriter.WriteDouble(XE_GAME_OBJ_GOC_MAT_SHADER_VAR_VALUE_PROP, varData);
			}
			break;

		default:
			break;
	}

	return XEResult::Ok;
}

XEResult GameObjectManager::SaveToXMLCBVector(XEXMLWriter& xmlWriter, ConstantBuffer* cb, const std::wstring& varName, ShaderVariableType varType, uint32_t columns) const
{
	if (cb == nullptr)
	{
		return XEResult::NullParameter;
	}

	switch (varType)
	{
		case ShaderVariableType::Boolean:
			{
				switch (columns)
				{
					case 2:
						{
							glm::bvec2 varData = XEMathHelpers::Vec2bZero;
							cb->GetValueT<glm::bvec2>(varName, varData);

							xmlWriter.WriteVect2b(XE_GAME_OBJ_GOC_MAT_SHADER_VAR_VALUE_PROP, varData);
						}
						break;

					case 3:
						{
							glm::bvec3 varData = XEMathHelpers::Vec3bZero;
							cb->GetValueT<glm::bvec3>(varName, varData);

							xmlWriter.WriteVect3b(XE_GAME_OBJ_GOC_MAT_SHADER_VAR_VALUE_PROP, varData);
						}
						break;

					case 4:
						{
							glm::bvec4 varData = XEMathHelpers::Vec4bZero;
							cb->GetValueT<glm::bvec4>(varName, varData);

							xmlWriter.WriteVect4b(XE_GAME_OBJ_GOC_MAT_SHADER_VAR_VALUE_PROP, varData);
						}
						break;

					default:
						break;
				}
			}
			break;

		case ShaderVariableType::Int:
		{
			switch (columns)
			{
				case 2:
					{
						glm::ivec2 varData = XEMathHelpers::Vec2iZero;
						cb->GetValueT<glm::ivec2>(varName, varData);

						xmlWriter.WriteVect2i(XE_GAME_OBJ_GOC_MAT_SHADER_VAR_VALUE_PROP, varData);
					}
					break;

				case 3:
					{
						glm::ivec3 varData = XEMathHelpers::Vec3iZero;
						cb->GetValueT<glm::ivec3>(varName, varData);

						xmlWriter.WriteVect3i(XE_GAME_OBJ_GOC_MAT_SHADER_VAR_VALUE_PROP, varData);
					}
					break;

				case 4:
					{
						glm::ivec4 varData = XEMathHelpers::Vec4iZero;
						cb->GetValueT<glm::ivec4>(varName, varData);

						xmlWriter.WriteVect4i(XE_GAME_OBJ_GOC_MAT_SHADER_VAR_VALUE_PROP, varData);
					}
					break;

				default:
					break;
			}
		}
		break;

		case ShaderVariableType::Float:
		{
			switch (columns)
			{
				case 2:
					{
						glm::vec2 varData = XEMathHelpers::Vec2fZero;
						cb->GetValueT<glm::vec2>(varName, varData);

						xmlWriter.WriteVect2f(XE_GAME_OBJ_GOC_MAT_SHADER_VAR_VALUE_PROP, varData);
					}
					break;

				case 3:
					{
						glm::vec3 varData = XEMathHelpers::Vec3fZero;
						cb->GetValueT<glm::vec3>(varName, varData);

						xmlWriter.WriteVect3f(XE_GAME_OBJ_GOC_MAT_SHADER_VAR_VALUE_PROP, varData);
					}
					break;

				case 4:
				{
						glm::vec4 varData = XEMathHelpers::Vec4fZero;
						cb->GetValueT<glm::vec4>(varName, varData);

						xmlWriter.WriteVect4f(XE_GAME_OBJ_GOC_MAT_SHADER_VAR_VALUE_PROP, varData);
					}
					break;

				default:
					break;
			}
		}
		break;

		case ShaderVariableType::Double:
		{
			switch (columns)
			{
				case 2:
					{
						glm::dvec2 varData = XEMathHelpers::Vec2dZero;
						cb->GetValueT<glm::dvec2>(varName, varData);

						xmlWriter.WriteVect2d(XE_GAME_OBJ_GOC_MAT_SHADER_VAR_VALUE_PROP, varData);
					}
					break;

				case 3:
					{
						glm::dvec3 varData = XEMathHelpers::Vec3dZero;
						cb->GetValueT<glm::dvec3>(varName, varData);

						xmlWriter.WriteVect3d(XE_GAME_OBJ_GOC_MAT_SHADER_VAR_VALUE_PROP, varData);
					}
					break;

				case 4:
					{
						glm::dvec4 varData = XEMathHelpers::Vec4dZero;
						cb->GetValueT<glm::dvec4>(varName, varData);

						xmlWriter.WriteVect4d(XE_GAME_OBJ_GOC_MAT_SHADER_VAR_VALUE_PROP, varData);
					}
					break;

				default:
					break;
			}
		}
		break;

		default:
			break;
	}

	return XEResult::Ok;
}

XEResult GameObjectManager::SaveToXMLCBMatrix(XEXMLWriter& xmlWriter, ConstantBuffer* cb, const std::wstring& varName, ShaderVariableType varType, uint32_t columns, uint32_t rows) const
{
	if (cb == nullptr)
	{
		return XEResult::NullParameter;
	}

	if (varType != ShaderVariableType::Float)
	{
		return XEResult::Ok;
	}

	if (columns == 2 && rows == 2)
	{
		glm::mat2 varData = XEMathHelpers::Mat2Identity;
		cb->GetValueT<glm::mat2>(varName, varData);

		xmlWriter.WriteMat2f(XE_GAME_OBJ_GOC_MAT_SHADER_VAR_VALUE_PROP, varData);
	}
	else if (columns == 3 && rows == 3)
	{
		glm::mat3 varData = XEMathHelpers::Mat3Identity;
		cb->GetValueT<glm::mat3>(varName, varData);

		xmlWriter.WriteMat3f(XE_GAME_OBJ_GOC_MAT_SHADER_VAR_VALUE_PROP, varData);
	}
	else if (columns == 4 && rows == 4)
	{
		glm::mat4 varData = XEMathHelpers::Mat4Identity;
		cb->GetValueT<glm::mat4>(varName, varData);

		xmlWriter.WriteMat4f(XE_GAME_OBJ_GOC_MAT_SHADER_VAR_VALUE_PROP, varData);
	}

	return XEResult::Ok;
}

XEResult GameObjectManager::SaveToXMLGameObjectScriptsComponent(XEXMLWriter& xmlWriter, GameObject* gameObject) const
{
	if (gameObject == nullptr)
	{
		return XEResult::NullParameter;
	}

	if (!gameObject->HasGameObjectScriptGOCs())
	{
		return XEResult::Ok;
	}

	XEResult ret = XEResult::Ok;

	////////////////////////////
	//Write Game Object Components
	ret = xmlWriter.StartNode(XE_GAME_OBJ_GOC_SCRIPT_NODE_NAME);
	if (ret != XEResult::Ok)
	{
		XETODO("Better return code");
		return XEResult::Fail;
	}

	const GameObjectScriptGOCList& gosList = gameObject->GetGameObjectScriptGOCList();
	for (auto gosComponent : gosList)
	{
		xmlWriter.WriteString(XE_GAME_OBJ_GOC_SCRIPT_NAME_PROP, gosComponent->GetName());
		xmlWriter.WriteUInt64(XE_GAME_OBJ_COMPONENT_ASSETID_PROP, gosComponent->GetGameObjectScriptAssetID());
		xmlWriter.WriteBool(XE_GAME_OBJ_GOC_SCRIPT_HAS_INSTANCE_PROP, gosComponent->HasScriptInstance());

		if (gosComponent->HasScriptInstance())
		{
			xmlWriter.WriteString(XE_GAME_OBJ_GOC_SCRIPT_INSTANCE_NAME_PROP, gosComponent->GetGameObjectScriptInstanceName());

			ret = SaveToXMLGameObjectScriptsProperties(xmlWriter, gosComponent->GetGameObjectScriptProperties());
			if (ret != XEResult::Ok)
			{
				XETODO("Better return code");
				return XEResult::Fail;
			}
		}
	}

	////////////////////////////
	//End of XE_GAME_OBJ_GOC_SCRIPT_NODE_NAME
	ret = xmlWriter.EndNode();
	if (ret != XEResult::Ok)
	{
		XETODO("Better return code");
		return XEResult::Fail;
	}

	return XEResult::Ok;
}

XEResult GameObjectManager::SaveToXMLGameObjectScriptsProperties(XEXMLWriter& xmlWriter, const GameObjectScriptProperties* properties) const
{
	if (properties == nullptr)
	{
		return XEResult::NullParameter;
	}

	XEResult ret = XEResult::Ok;

	for (auto propertyIt : properties->m_PropertiesMap)
	{
		GameObjectScriptProperty* gosProp = propertyIt.second;

		////////////////////////////
		//Write Game Object Components
		ret = xmlWriter.StartNode(XE_GAME_OBJ_GOC_SCRIPT_PROPERTY_NODE_NAME);
		if (ret != XEResult::Ok)
		{
			XETODO("Better return code");
			return XEResult::Fail;
		}

		xmlWriter.WriteString(XE_GAME_OBJ_GOC_SCRIPT_PROPERTY_NAME_PROP, gosProp->m_PropertyName);
		xmlWriter.WriteUInt(XE_GAME_OBJ_GOC_SCRIPT_PROPERTY_TYPE_PROP, (uint32_t)gosProp->m_PropertyType);

		switch (gosProp->m_PropertyType)
		{
			case asTYPEID_BOOL:
				{
					bool value = false;
					memcpy(&value, gosProp->m_PropertyAddress, sizeof(bool));

					xmlWriter.WriteBool(XE_GAME_OBJ_GOC_SCRIPT_PROPERTY_VALUE_PROP, value);
				}
				break;

			case asTYPEID_INT8:
				{
					int8_t value = 0;
					memcpy(&value, gosProp->m_PropertyAddress, sizeof(int8_t));

					xmlWriter.WriteInt8(XE_GAME_OBJ_GOC_SCRIPT_PROPERTY_VALUE_PROP, value);
				}
				break;

			case asTYPEID_INT16:
				{
					int16_t value = 0;
					memcpy(&value, gosProp->m_PropertyAddress, sizeof(int16_t));

					xmlWriter.WriteInt16(XE_GAME_OBJ_GOC_SCRIPT_PROPERTY_VALUE_PROP, value);
				}
				break;

			case asTYPEID_INT32:
				{
					int32_t value = 0;
					memcpy(&value, gosProp->m_PropertyAddress, sizeof(int32_t));

					xmlWriter.WriteInt(XE_GAME_OBJ_GOC_SCRIPT_PROPERTY_VALUE_PROP, value);
				}
				break;

			case asTYPEID_INT64:
				{
					int64_t value = 0;
					memcpy(&value, gosProp->m_PropertyAddress, sizeof(int64_t));

					xmlWriter.WriteInt64(XE_GAME_OBJ_GOC_SCRIPT_PROPERTY_VALUE_PROP, value);
				}
				break;

			case asTYPEID_UINT8:
				{
					uint8_t value = 0;
					memcpy(&value, gosProp->m_PropertyAddress, sizeof(uint8_t));

					xmlWriter.WriteUInt8(XE_GAME_OBJ_GOC_SCRIPT_PROPERTY_VALUE_PROP, value);
				}
				break;

			case asTYPEID_UINT16:
				{
					uint16_t value = 0;
					memcpy(&value, gosProp->m_PropertyAddress, sizeof(uint16_t));

					xmlWriter.WriteUInt16(XE_GAME_OBJ_GOC_SCRIPT_PROPERTY_VALUE_PROP, value);
				}
				break;

			case asTYPEID_UINT32:
				{
					uint32_t value = 0;
					memcpy(&value, gosProp->m_PropertyAddress, sizeof(uint32_t));

					xmlWriter.WriteUInt(XE_GAME_OBJ_GOC_SCRIPT_PROPERTY_VALUE_PROP, value);
				}
				break;

			case asTYPEID_UINT64:
				{
					uint64_t value = 0;
					memcpy(&value, gosProp->m_PropertyAddress, sizeof(uint64_t));

					xmlWriter.WriteUInt64(XE_GAME_OBJ_GOC_SCRIPT_PROPERTY_VALUE_PROP, value);
				}
				break;

			case asTYPEID_FLOAT:
				{
					float value = 0.0f;
					memcpy(&value, gosProp->m_PropertyAddress, sizeof(float));

					xmlWriter.WriteFloat(XE_GAME_OBJ_GOC_SCRIPT_PROPERTY_VALUE_PROP, value);
				}
				break;

			case asTYPEID_DOUBLE:
				{
					double value = 0.0;
					memcpy(&value, gosProp->m_PropertyAddress, sizeof(double));

					xmlWriter.WriteDouble(XE_GAME_OBJ_GOC_SCRIPT_PROPERTY_VALUE_PROP, value);
				}
				break;

			default:
				break;
		}

		////////////////////////////
		//End of XE_GAME_OBJ_GOC_SCRIPT_PROPERTY_NODE_NAME
		ret = xmlWriter.EndNode();
		if (ret != XEResult::Ok)
		{
			XETODO("Better return code");
			return XEResult::Fail;
		}
	}

	return XEResult::Ok;
}

XEResult GameObjectManager::SaveToXMLLightComponent(XEXMLWriter& xmlWriter, GameObject* gameObject) const
{
	if (gameObject == nullptr)
	{
		return XEResult::NullParameter;
	}

	if (!gameObject->HasLightGOC())
	{
		return XEResult::Ok;
	}

	XEResult ret = XEResult::Ok;

	////////////////////////////
	//Write Game Object Components
	ret = xmlWriter.StartNode(XE_GAME_OBJ_GOC_LIGHT_NODE_NAME);
	if (ret != XEResult::Ok)
	{
		XETODO("Better return code");
		return XEResult::Fail;
	}

	LightGOC* lightGOC = gameObject->GetLightGOC();

	Light* light = lightGOC->GetLight();

	xmlWriter.WriteBool(XE_GAME_OBJ_GOC_LIGHT_FRUSTUM_CASCADE_PROP, lightGOC->IsDrawFrustumCascadesEnabled());
	xmlWriter.WriteBool(XE_GAME_OBJ_GOC_LIGHT_DEBUG_DRAW_PROP, lightGOC->IsDrawDebugEnabled());
	xmlWriter.WriteBool(XE_GAME_OBJ_GOC_LIGHT_DRAW_FRUSTUM_SIMPLE_PROP, lightGOC->IsDrawFrustumSimple());
	xmlWriter.WriteBool(XE_GAME_OBJ_GOC_LIGHT_DRAW_FRUSTUM_ENABLE_PROP, lightGOC->IsDrawFrustumEnabled());

	xmlWriter.WriteUInt(XE_GAME_OBJ_GOC_LIGHT_LIGHT_TYPE_PROP, (uint32_t)light->GetLightType());
	xmlWriter.WriteBool(XE_GAME_OBJ_GOC_LIGHT_ENABLED_PROP, light->IsEnabled());
	xmlWriter.WriteBool(XE_GAME_OBJ_GOC_LIGHT_SHADOW_ENABLED_PROP, light->IsShadowEnabled());
	xmlWriter.WriteFloat(XE_GAME_OBJ_GOC_LIGHT_NEAR_ATTENUATION_PROP, light->GetNearAttenuation());
	xmlWriter.WriteFloat(XE_GAME_OBJ_GOC_LIGHT_FAR_ATTENUATION_PROP, light->GetFarAttenuation());
	xmlWriter.WriteVect4f(XE_GAME_OBJ_GOC_LIGHT_COLOR_PROP, light->GetColor().ToVector4f());

	switch (light->GetLightType())
	{
		case LightType::Spot:
		{
			SpotLight* spotLight = reinterpret_cast<SpotLight*>(light);

			xmlWriter.WriteFloat(XE_GAME_OBJ_GOC_LIGHT_FALL_OFF_ANGLE_PROP, spotLight->GetFallOffAngle());
			xmlWriter.WriteFloat(XE_GAME_OBJ_GOC_LIGHT_ANGLE_PROP, spotLight->GetAngle());
		}
		break;

		case LightType::Directional:
			break;

		case LightType::Omni:
			break;

		default:
			return XEResult::InvalidObjType;
	}

	////////////////////////////
	//End of XE_GAME_OBJ_GOC_LIGHT_NODE_NAME
	ret = xmlWriter.EndNode();
	if (ret != XEResult::Ok)
	{
		XETODO("Better return code");
		return XEResult::Fail;
	}

	return XEResult::Ok;
}

XEResult GameObjectManager::SaveToXMLMeshAnimationComponent(XEXMLWriter& xmlWriter, GameObject* gameObject) const
{
	if (gameObject == nullptr)
	{
		return XEResult::NullParameter;
	}

	if (!gameObject->HasMeshAnimationGOC())
	{
		return XEResult::Ok;
	}

	XEResult ret = XEResult::Ok;

	////////////////////////////
	//Write Game Object Components
	ret = xmlWriter.StartNode(XE_GAME_OBJ_GOC_ANIM_NODE_NAME);
	if (ret != XEResult::Ok)
	{
		XETODO("Better return code");
		return XEResult::Fail;
	}

	MeshAnimationGOC* animGOC = gameObject->GetMeshAnimationGOC();

	xmlWriter.WriteBool(XE_GAME_OBJ_GOC_ANIM_BLEND_ANIM_PROP, animGOC->GetBlendAnimation());
	xmlWriter.WriteFloat(XE_GAME_OBJ_GOC_ANIM_BLEND_TIME_PROP, animGOC->GetBlendTime());
	xmlWriter.WriteBool(XE_GAME_OBJ_GOC_ANIM_ON_LOOP_ANIM_PROP, animGOC->GetOnLoop());
	xmlWriter.WriteUInt64(XE_GAME_OBJ_GOC_ANIM_SKELETON_ASSET_ID_PROP, animGOC->GetSkeletonAssetID());

	const AnimationAssetPairMap& animAssetMap = animGOC->GetAnimationAssetMap();
	for (auto animAssetPair : animAssetMap)
	{
		ret = xmlWriter.StartNode(XE_GAME_OBJ_GOC_ANIM_ASSET_NODE_NAME);
		if (ret != XEResult::Ok)
		{
			XETODO("Better return code");
			return XEResult::Fail;
		}

		xmlWriter.WriteUInt64(XE_GAME_OBJ_COMPONENT_ASSETID_PROP, animAssetPair.second.m_AssetID);

		////////////////////////////
		//End of XE_GAME_OBJ_GOC_ANIM_ASSET_NODE_NAME
		ret = xmlWriter.EndNode();
		if (ret != XEResult::Ok)
		{
			XETODO("Better return code");
			return XEResult::Fail;
		}
	}

	////////////////////////////
	//End of XE_GAME_OBJ_GOC_MESH_ANIMATION_NODE_NAME
	ret = xmlWriter.EndNode();
	if (ret != XEResult::Ok)
	{
		XETODO("Better return code");
		return XEResult::Fail;
	}

	return XEResult::Ok;
}

XEResult GameObjectManager::SaveToXMLCameraComponent(XEXMLWriter& xmlWriter, GameObject* gameObject) const
{
	if (gameObject == nullptr)
	{
		return XEResult::NullParameter;
	}

	if (!gameObject->HasCameraGOC())
	{
		return XEResult::Ok;
	}

	XEResult ret = XEResult::Ok;

	////////////////////////////
	//Write Game Object Components
	ret = xmlWriter.StartNode(XE_GAME_OBJ_GOC_CAM_NODE_NAME);
	if (ret != XEResult::Ok)
	{
		XETODO("Better return code");
		return XEResult::Fail;
	}

	CameraGOC* cameraGOC = gameObject->GetCameraGOC();

	xmlWriter.WriteBool(XE_GAME_OBJ_GOC_CAM_DEBUG_DRAW_ENABLED_PROP, cameraGOC->IsDebugDrawEnabled());
	xmlWriter.WriteBool(XE_GAME_OBJ_GOC_CAM_IS_DEFAULT_CAM_PROP, cameraGOC->IsDefaultCamera());
	xmlWriter.WriteBool(XE_GAME_OBJ_GOC_CAM_DRAW_FRUSTUM_ENABLED_PROP, cameraGOC->IsDrawFrustumEnabled());
	xmlWriter.WriteBool(XE_GAME_OBJ_GOC_CAM_DRAW_FRUSTUM_SIMPLE_PROP, cameraGOC->IsDrawFrustumSimple());

	////////////////////////////
	//End of XE_GAME_OBJ_GOC_CAMERA_NODE_NAME
	ret = xmlWriter.EndNode();
	if (ret != XEResult::Ok)
	{
		XETODO("Better return code");
		return XEResult::Fail;
	}

	return XEResult::Ok;
}

XEResult GameObjectManager::SaveToXMLAudioListenerComponent(XEXMLWriter& xmlWriter, GameObject* gameObject) const
{
	if (gameObject == nullptr)
	{
		return XEResult::NullParameter;
	}

	if (!gameObject->HasAudioListenerGOC())
	{
		return XEResult::Ok;
	}

	XEResult ret = XEResult::Ok;

	////////////////////////////
	//Write Game Object Components
	ret = xmlWriter.StartNode(XE_GAME_OBJ_GOC_AUDIO_LISTENER_NODE_NAME);
	if (ret != XEResult::Ok)
	{
		XETODO("Better return code");
		return XEResult::Fail;
	}

	////////////////////////////
	//End of XE_GAME_OBJ_GOC_AUDIO_LISTENER_NODE_NAME
	ret = xmlWriter.EndNode();
	if (ret != XEResult::Ok)
	{
		XETODO("Better return code");
		return XEResult::Fail;
	}

	return XEResult::Ok;
}

XEResult GameObjectManager::SaveToXMLAudioSourceComponent(XEXMLWriter& xmlWriter, GameObject* gameObject) const
{
	if (gameObject == nullptr)
	{
		return XEResult::NullParameter;
	}

	if (!gameObject->HasAudioSourceGOCs())
	{
		return XEResult::Ok;
	}

	XEResult ret = XEResult::Ok;

	const AudioSourceGOCList& audioSourceList = gameObject->GetAudioSourceGOCList();
	for (auto audioSource : audioSourceList)
	{
		////////////////////////////
		//Write Game Object Components
		ret = xmlWriter.StartNode(XE_GAME_OBJ_GOC_AUDIO_SOURCE_NODE_NAME);
		if (ret != XEResult::Ok)
		{
			XETODO("Better return code");
			return XEResult::Fail;
		}

		xmlWriter.WriteString(XE_GAME_OBJ_GOC_AUDIO_SOURCE_NAME_PROP, audioSource->GetName());
		xmlWriter.WriteUInt64(XE_GAME_OBJ_COMPONENT_ASSETID_PROP, audioSource->GetAudioAssetID());

		////////////////////////////
		//End of XE_GAME_OBJ_GOC_AUDIO_SOURCE_NODE_NAME
		ret = xmlWriter.EndNode();
		if (ret != XEResult::Ok)
		{
			XETODO("Better return code");
			return XEResult::Fail;
		}
	}

	return XEResult::Ok;
}

XEResult GameObjectManager::SaveToXMLPhysicsComponent(XEXMLWriter& xmlWriter, GameObject* gameObject) const
{
	if (gameObject == nullptr)
	{
		return XEResult::NullParameter;
	}

	if (!gameObject->HasPhysicsGOC())
	{
		return XEResult::Ok;
	}

	XEResult ret = XEResult::Ok;

	////////////////////////////
	//Write Game Object Components
	ret = xmlWriter.StartNode(XE_GAME_OBJ_GOC_PHYSICS_NODE_NAME);
	if (ret != XEResult::Ok)
	{
		XETODO("Better return code");
		return XEResult::Fail;
	}

	PhysicsGOC* physicsGOC = gameObject->GetPhysicsGOC();

	xmlWriter.WriteBool(XE_GAME_OBJ_GOC_PHYSICS_IS_RIGID_BODY_PROP, physicsGOC->IsRigidBody());

	PhysicsActor* actor = physicsGOC->GetPhysicsActor();
	const PhysicColliderMap& colliderMap = actor->GetPhysicColliderMap();
	for (auto colliderPair : colliderMap)
	{
		////////////////////////////
		//Write Game Object Components
		ret = xmlWriter.StartNode(XE_GAME_OBJ_GOC_PHYSICS_COLLIDER_NODE_NAME);
		if (ret != XEResult::Ok)
		{
			XETODO("Better return code");
			return XEResult::Fail;
		}

		PhysicCollider* collider = colliderPair.second;

		CollisionShape shape = collider->GetCollisionShape();

		xmlWriter.WriteUInt8(XE_GAME_OBJ_GOC_PHYSICS_COLLIDER_TYPE_PROP, (uint8_t)shape);
		xmlWriter.WriteVect3f(XE_GAME_OBJ_GOC_PHYSICS_COLLIDER_SCALE_PROP, collider->GetScale());

		switch (shape)
		{
			case CollisionShape::Box:
				{
					PhysicColliderBox* box = (PhysicColliderBox*)collider;

					xmlWriter.WriteVect3f(XE_GAME_OBJ_GOC_PHYSICS_COLLIDER_SIZE_PROP, box->GetSize());
				}
				break;

			case CollisionShape::Sphere:
				{
					PhysicColliderSphere* sphere = (PhysicColliderSphere*)collider;

					xmlWriter.WriteFloat(XE_GAME_OBJ_GOC_PHYSICS_COLLIDER_RADIUS_PROP, sphere->GetRadius());
				}
				break;

			default:
				break;
		}

		////////////////////////////
		//End of XE_GAME_OBJ_GOC_PHYSICS_COLLIDER_NODE_NAME
		ret = xmlWriter.EndNode();
		if (ret != XEResult::Ok)
		{
			XETODO("Better return code");
			return XEResult::Fail;
		}
	}

	////////////////////////////
	//End of XE_GAME_OBJ_GOC_PHYSICS_NODE_NAME
	ret = xmlWriter.EndNode();
	if (ret != XEResult::Ok)
	{
		XETODO("Better return code");
		return XEResult::Fail;
	}

	return XEResult::Ok;
}

XEResult GameObjectManager::LoadGameObjectManagerFile(const std::wstring& file)
{
	if (m_GameAssetManager == nullptr)
	{
		return XEResult::GameAssetManagerNull;
	}

	if (m_AngelScriptManager == nullptr)
	{
		XETODO("Better return type");
		return XEResult::NullObj;
	}

	if (m_GameObjectScriptManager == nullptr)
	{
		XETODO("Better return type");
		return XEResult::NullObj;
	}

	if (m_LightManager == nullptr)
	{
		return XEResult::LightManagerNull;
	}

	if (m_CameraManager == nullptr)
	{
		XETODO("Better return type");
		return XEResult::NullObj;
	}

	if (m_GraphicDevice == nullptr)
	{
		return XEResult::GraphicDeviceNull;
	}

	if (m_AudioManager == nullptr)
	{
		XETODO("Better return type");
		return XEResult::NullObj;
	}

	if (m_PhysicsManager == nullptr)
	{
		XETODO("Better return type");
		return XEResult::NullObj;
	}

	if (file.empty())
	{
		return XEResult::EmptyFilename;
	}

	XEXMLParser newFile;
	if (newFile.LoadFile(file) != XEResult::Ok)
	{
		std::wstring msg_error = fmt::format(XELOCMAN->GetLiteral(L"INIT_COULDNT_READ_FILE_MSG"), __FUNCTIONW__, file);

		XELOGGER->AddNewLog(LogLevel::Error, msg_error);
		return XEResult::OpenFileFail;
	}

	XEXMLParser configXML = newFile[XE_GAME_OBJ_MANAGER_NODE_NAME];
	if (!configXML.IsReady())
	{
		return XEResult::XMLReadError;
	}

	uint32_t l_Count = configXML.GetNumChildren();
	for (uint32_t i = 0; i < l_Count; ++i)
	{
		XEXMLParser child = configXML(i);

		std::wstring l_Type = child.GetName();

		if (l_Type.compare(XE_GAME_OBJ_NODE_NAME) == 0)
		{
			if (LoadXMLGameObject(child, nullptr) != XEResult::Ok)
			{
				return XEResult::Fail;
			}
		}
	}

	return XEResult::Ok;
}

XEResult GameObjectManager::LoadXMLGameObject(XEXMLParser& xmlParser, GameObject* parent)
{
	XEResult ret = XEResult::Ok;

	////////////////////////////
	//Game Object
	GameObject* gameObject = new GameObject();

	if (parent != nullptr)
	{
		parent->AddChild(gameObject);
	}
	else
	{
		this->AddGameObject(gameObject);
	}

	////////////////////////////
	//Get Game Object Props
	std::wstring name	= xmlParser.GetString(XE_GAME_OBJ_NAME_PROP);
	glm::vec3 pos		= xmlParser.GetVect3f(XE_GAME_OBJ_POSITION_PROP);
	glm::vec3 rot		= xmlParser.GetVect3f(XE_GAME_OBJ_ROTATION_PROP);
	glm::vec3 scale		= xmlParser.GetVect3f(XE_GAME_OBJ_SCALE_PROP);

	////////////////////////////
	//Set Props
	gameObject->SetName(name);
	gameObject->SetPosition(pos);
	gameObject->SetRotation(rot);
	gameObject->SetScale(scale);

	////////////////////////////
	//Get Game Object Components
	uint32_t l_Count = xmlParser.GetNumChildren();
	for (uint32_t i = 0; i < l_Count; ++i)
	{
		XEXMLParser child = xmlParser(i);

		std::wstring l_Type = child.GetName();

		if (l_Type.compare(XE_GAME_OBJ_COMPONENTS_NODE_NAME) == 0)
		{
			if (LoadXMLGameObjectComponents(child, gameObject) != XEResult::Ok)
			{
				return XEResult::Fail;
			}
		}
		else if (l_Type.compare(XE_GAME_OBJ_CHILDS_NODE_NAME) == 0)
		{
			uint32_t childrenCount = child.GetNumChildren();
			for (uint32_t i = 0; i < childrenCount; ++i)
			{
				XEXMLParser childObject = child(i);

				std::wstring childType = childObject.GetName();

				if (l_Type.compare(XE_GAME_OBJ_NODE_NAME) == 0)
				{
					if (LoadXMLGameObject(childObject, gameObject) != XEResult::Ok)
					{
						return XEResult::Fail;
					}
				}
			}
		}
	}

	////////////////////////////
	//Finalize
	return XEResult::Ok;
}

XEResult GameObjectManager::LoadXMLGameObjectComponents(XEXMLParser& xmlParser, GameObject* gameObject)
{
	if (gameObject == nullptr)
	{
		return XEResult::NullParameter;
	}

	XEResult ret = XEResult::Ok;

	////////////////////////////
	//Load Game Object Components

	uint32_t componentCount = xmlParser.GetNumChildren();
	for (uint32_t i = 0; i < componentCount; ++i)
	{
		XEXMLParser child = xmlParser(i);

		std::wstring childType = child.GetName();

		if (childType.compare(XE_GAME_OBJ_GOC_MESH_NODE_NAME) == 0)
		{
			if (LoadXMLMeshComponent(child, gameObject) != XEResult::Ok)
			{
				return XEResult::Fail;
			}
		}
		else if (childType.compare(XE_GAME_OBJ_GOC_MATS_NODE_NAME) == 0)
		{
			if (LoadXMLMeshMaterialsComponent(child, gameObject) != XEResult::Ok)
			{
				return XEResult::Fail;
			}
		}
		else if (childType.compare(XE_GAME_OBJ_GOC_SCRIPTS_NODE_NAME) == 0)
		{
			if (LoadXMLGameObjectScriptsComponent(child, gameObject) != XEResult::Ok)
			{
				return XEResult::Fail;
			}
		}
		else if (childType.compare(XE_GAME_OBJ_GOC_LIGHT_NODE_NAME) == 0)
		{
			if (LoadXMLLightComponent(child, gameObject) != XEResult::Ok)
			{
				return XEResult::Fail;
			}
		}
		else if (childType.compare(XE_GAME_OBJ_GOC_ANIM_NODE_NAME) == 0)
		{
			if (LoadXMLMeshAnimationComponent(child, gameObject) != XEResult::Ok)
			{
				return XEResult::Fail;
			}
		}
		else if (childType.compare(XE_GAME_OBJ_GOC_CAM_NODE_NAME) == 0)
		{
			if (LoadXMLCameraComponent(child, gameObject) != XEResult::Ok)
			{
				return XEResult::Fail;
			}
		}
		else if (childType.compare(XE_GAME_OBJ_GOC_AUDIO_LISTENER_NODE_NAME) == 0)
		{
			if (LoadXMLAudioListenerComponent(child, gameObject) != XEResult::Ok)
			{
				return XEResult::Fail;
			}
		}
		else if (childType.compare(XE_GAME_OBJ_GOC_AUDIO_SOURCE_NODE_NAME) == 0)
		{
			if (LoadXMLAudioSourceComponent(child, gameObject) != XEResult::Ok)
			{
				return XEResult::Fail;
			}
		}
		else if (childType.compare(XE_GAME_OBJ_GOC_PHYSICS_NODE_NAME) == 0)
		{
			if (LoadXMLPhysicsComponent(child, gameObject) != XEResult::Ok)
			{
				return XEResult::Fail;
			}
		}
	}

	////////////////////////////
	//Finalize
	return XEResult::Ok;
}

XEResult GameObjectManager::LoadXMLMeshComponent(XEXMLParser& xmlParser, GameObject* gameObject)
{
	if (gameObject == nullptr)
	{
		return XEResult::NullParameter;
	}

	XEResult ret = XEResult::Ok;

	////////////////////////////
	//Get Mesh Asset
	uint64_t assetID = xmlParser.GetUInt64(XE_GAME_OBJ_COMPONENT_ASSETID_PROP);

	GameAsset* gameAsset = m_GameAssetManager->GetGameAsset(assetID);
	if (gameAsset != nullptr)
	{
		return XEResult::NullObj;
	}

	if (gameAsset->GetGameContentType() != GameContentType::Mesh)
	{
		return XEResult::InvalidObjType;
	}

	////////////////////////////
	//Create Game Object Components
	MeshGOC* meshGOC = new MeshGOC(gameObject);

	////////////////////////////
	//Set Components Props
	if (meshGOC->SetMeshAsset(reinterpret_cast<MeshAsset*>(gameAsset)) != XEResult::Ok)
	{
		DeleteMem(meshGOC);

		return XEResult::Fail;
	}

	if (gameObject->SetMeshGOC(meshGOC) != XEResult::Ok)
	{
		DeleteMem(meshGOC);

		return XEResult::Fail;
	}

	return XEResult::Ok;
}

XEResult GameObjectManager::LoadXMLMeshMaterialsComponent(XEXMLParser& xmlParser, GameObject* gameObject)
{
	if (gameObject == nullptr)
	{
		return XEResult::NullParameter;
	}

	XEResult ret = XEResult::Ok;

	MeshMaterialGOC* material = new MeshMaterialGOC(gameObject);

	uint32_t childCount = xmlParser.GetNumChildren();
	for (uint32_t i = 0; i < childCount; ++i)
	{
		XEXMLParser childObject = xmlParser(i);

		std::wstring childType = childObject.GetName();

		if (childType.compare(XE_GAME_OBJ_GOC_MATS_NODE_NAME) == 0)
		{
			if (LoadXMLMeshMaterialComponent(childObject, gameObject) != XEResult::Ok)
			{
				return XEResult::Fail;
			}
		}
	}

	return XEResult::Ok;
}

XEResult GameObjectManager::LoadXMLMeshMaterialComponent(XEXMLParser& xmlParser, GameObject* gameObject)
{
	if (gameObject == nullptr)
	{
		return XEResult::NullParameter;
	}

	XEResult ret = XEResult::Ok;

	MeshMaterialGOC* material = new MeshMaterialGOC(gameObject);

	std::wstring name = xmlParser.GetString(XE_GAME_OBJ_GOC_MAT_NAME_PROP);

	material->SetName(name);

	uint32_t childCount = xmlParser.GetNumChildren();
	for (uint32_t i = 0; i < childCount; ++i)
	{
		XEXMLParser childObject = xmlParser(i);

		std::wstring childType = childObject.GetName();

		if (childType.compare(XE_GAME_OBJ_GOC_MAT_SHADER_NODE_NAME) == 0)
		{
			if (LoadXMLShader(childObject, material) != XEResult::Ok)
			{
				return XEResult::Fail;
			}
		}
	}

	return XEResult::Ok;
}

XEResult GameObjectManager::LoadXMLShader(XEXMLParser& xmlParser, MeshMaterialGOC* material)
{
	if (material == nullptr)
	{
		return XEResult::NullParameter;
	}

	XEResult ret = XEResult::Ok;

	uint64_t assetID = xmlParser.GetUInt64(XE_GAME_OBJ_COMPONENT_ASSETID_PROP);
	ShaderType type = (ShaderType)xmlParser.GetUInt(XE_GAME_OBJ_GOC_MAT_SHADER_TYPE_PROP);

	GameAsset* gameAsset = m_GameAssetManager->GetGameAsset(assetID);
	if (gameAsset == nullptr)
	{
		return XEResult::Fail;
	}

	if (gameAsset->GetGameContentType() != GameContentType::Shader)
	{
		return XEResult::InvalidObjType;
	}

	ShaderAsset* shaderAsset = reinterpret_cast<ShaderAsset*>(gameAsset);
	ShaderProperties* properties = nullptr;

	switch (type)
	{
		case ShaderType::VertexShader:
			{
				if (material->SetVertexShaderAsset(shaderAsset) != XEResult::Ok)
				{
					return XEResult::Fail;
				}

				properties = material->GetVertexShaderProperties();
			}
			break;

		case ShaderType::PixelShader:
			{
				if (material->SetPixelShaderAsset(shaderAsset) != XEResult::Ok)
				{
					return XEResult::Fail;
				}

				properties = material->GetPixelShaderProperties();
			}
			break;

		case ShaderType::GeometryShader:
			{
				if (material->SetGeometryShaderAsset(shaderAsset) != XEResult::Ok)
				{
					return XEResult::Fail;
				}

				properties = material->GetGeometryShaderProperties();
			}
			break;

		case ShaderType::HullShader:
			{
				if (material->SetHullShaderAsset(shaderAsset) != XEResult::Ok)
				{
					return XEResult::Fail;
				}

				properties = material->GetHullShaderProperties();
			}
			break;

		case ShaderType::ComputeShader:
			{
				if (material->SetComputeShaderAsset(shaderAsset) != XEResult::Ok)
				{
					return XEResult::Fail;
				}

				properties = material->GetComputeShaderProperties();
			}
			break;

		case ShaderType::DomainShader:
			{
				if (material->SetDomainShaderAsset(shaderAsset) != XEResult::Ok)
				{
					return XEResult::Fail;
				}

				properties = material->GetDomainShaderProperties();
			}
			break;

		default:
			break;
	}

	uint32_t childCount = xmlParser.GetNumChildren();
	for (uint32_t i = 0; i < childCount; ++i)
	{
		XEXMLParser childObject = xmlParser(i);

		std::wstring childType = childObject.GetName();

		if (childType.compare(XE_GAME_OBJ_GOC_MAT_SHADER_PROPS_NODE_NAME) == 0)
		{
			if (LoadXMLShaderProperties(childObject, properties) != XEResult::Ok)
			{
				return XEResult::Fail;
			}
		}
	}

	return XEResult::Ok;
}

XEResult GameObjectManager::LoadXMLShaderProperties(XEXMLParser& xmlParser, ShaderProperties* properties)
{
	if (properties == nullptr)
	{
		return XEResult::NullParameter;
	}

	uint32_t childCount = xmlParser.GetNumChildren();
	for (uint32_t i = 0; i < childCount; ++i)
	{
		XEXMLParser childObject = xmlParser(i);

		std::wstring childType = childObject.GetName();

		if (childType.compare(XE_GAME_OBJ_GOC_MAT_SHADER_CB_NODE_NAME) == 0)
		{
			std::wstring cbName = childObject.GetString(XE_GAME_OBJ_GOC_MAT_SHADER_CB_NAME_PROP);

			ConstantBuffer* cb = properties->GetConstantBuffer(cbName);
			if (cb == nullptr)
			{
				return XEResult::NullObj;
			}

			if (LoadXMLCBShaderVariable(childObject, cb) != XEResult::Ok)
			{
				return XEResult::Fail;
			}
		}

		if (childType.compare(XE_GAME_OBJ_GOC_MAT_TEXTURE_NODE_NAME) == 0)
		{
			std::wstring name = xmlParser.GetString(XE_GAME_OBJ_GOC_MAT_TEXTURE_NAME_PROP);
			uint64_t assetID = xmlParser.GetUInt64(XE_GAME_OBJ_COMPONENT_ASSETID_PROP);

			GameAsset* gameAsset = m_GameAssetManager->GetGameAsset(assetID);
			if (gameAsset == nullptr || gameAsset->GetGameContentType() != GameContentType::Texture)
			{
				return XEResult::InvalidObjType;
			}

			TextureAsset* textureAsset = reinterpret_cast<TextureAsset*>(gameAsset);
			if (properties->SetTexture(name, textureAsset) != XEResult::Ok)
			{
				return XEResult::Fail;
			}
		}
	}

	return XEResult::Ok;
}

XEResult GameObjectManager::LoadXMLCBShaderVariable(XEXMLParser& xmlParser, ConstantBuffer* cb)
{
	if (cb == nullptr)
	{
		return XEResult::NullParameter;
	}

	uint32_t varCount = xmlParser.GetNumChildren();
	for (uint32_t i = 0; i < varCount; ++i)
	{
		XEXMLParser varObject = xmlParser(i);

		std::wstring varType = varObject.GetName();

		if (varType.compare(XE_GAME_OBJ_GOC_MAT_SHADER_VAR_NODE_NAME) == 0)
		{
			XEResult ret = XEResult::Ok;

			std::wstring varName		= xmlParser.GetString(XE_GAME_OBJ_GOC_MAT_SHADER_VAR_NAME_PROP);
			ShaderVariableClass svc		= (ShaderVariableClass)xmlParser.GetUInt(XE_GAME_OBJ_GOC_MAT_SHADER_VAR_CLASS_PROP);
			ShaderVariableType svt		= (ShaderVariableType)xmlParser.GetUInt(XE_GAME_OBJ_GOC_MAT_SHADER_VAR_TYPE_PROP);
			uint32_t columns			= xmlParser.GetUInt(XE_GAME_OBJ_GOC_MAT_SHADER_COLUMNS_PROP);
			uint32_t rows				= xmlParser.GetUInt(XE_GAME_OBJ_GOC_MAT_SHADER_ROWS_PROP);

			switch (svc)
			{
				case ShaderVariableClass::Scalar:
					ret = LoadXMLCBScalar(xmlParser, cb, varName, svt);
					break;

				case ShaderVariableClass::Vector:
					ret = LoadXMLCBVector(xmlParser, cb, varName, svt, columns);
					break;

				case ShaderVariableClass::Matrix:
					ret = LoadXMLCBMatrix(xmlParser, cb, varName, svt, columns, rows);
					break;

				default:
					break;
			}

			if (ret != XEResult::Ok)
			{
				return XEResult::Fail;
			}
		}
	}

	return XEResult::Ok;
}

XEResult GameObjectManager::LoadXMLCBScalar(XEXMLParser& xmlParser, ConstantBuffer* cb, const std::wstring& varName, ShaderVariableType varType)
{
	if (cb == nullptr)
	{
		return XEResult::NullParameter;
	}

	switch (varType)
	{
		case ShaderVariableType::Boolean:
		{
			bool varData = xmlParser.GetBool(XE_GAME_OBJ_GOC_MAT_SHADER_VAR_VALUE_PROP);;

			cb->SetValueT<bool>(varName, varData);
		}
		break;

		case ShaderVariableType::Int:
		{
			int32_t varData = xmlParser.GetInt(XE_GAME_OBJ_GOC_MAT_SHADER_VAR_VALUE_PROP);

			cb->SetValueT<int32_t>(varName, varData);
		}
		break;

		case ShaderVariableType::UInt:
		{
			uint32_t varData = xmlParser.GetUInt(XE_GAME_OBJ_GOC_MAT_SHADER_VAR_VALUE_PROP);

			cb->SetValueT<uint32_t>(varName, varData);
		}
		break;

		case ShaderVariableType::UInt8:
		{
			uint8_t varData = xmlParser.GetUInt8(XE_GAME_OBJ_GOC_MAT_SHADER_VAR_VALUE_PROP);

			cb->SetValueT<uint8_t>(varName, varData);
		}
		break;

		case ShaderVariableType::Float:
		{
			float varData = xmlParser.GetFloat(XE_GAME_OBJ_GOC_MAT_SHADER_VAR_VALUE_PROP);

			cb->SetValueT<float>(varName, varData);
		}
		break;

		case ShaderVariableType::Double:
		{
			double varData = xmlParser.GetDouble(XE_GAME_OBJ_GOC_MAT_SHADER_VAR_VALUE_PROP);

			cb->SetValueT<double>(varName, varData);
		}
		break;

		default:
			break;
	}

	return XEResult::Ok;
}

XEResult GameObjectManager::LoadXMLCBVector(XEXMLParser& xmlParser, ConstantBuffer* cb, const std::wstring& varName, ShaderVariableType varType, uint32_t columns)
{
	if (cb == nullptr)
	{
		return XEResult::NullParameter;
	}

	switch (varType)
	{
		case ShaderVariableType::Boolean:
			{
				switch (columns)
				{
					case 2:
						{
							glm::bvec2 varData = xmlParser.GetVect2b(XE_GAME_OBJ_GOC_MAT_SHADER_VAR_VALUE_PROP);;

							cb->SetValueT<glm::bvec2>(varName, varData);
						}
						break;

					case 3:
						{
							glm::bvec3 varData = xmlParser.GetVect3b(XE_GAME_OBJ_GOC_MAT_SHADER_VAR_VALUE_PROP);

							cb->SetValueT<glm::bvec3>(varName, varData);
						}
						break;

					case 4:
						{
							glm::bvec4 varData = xmlParser.GetVect4b(XE_GAME_OBJ_GOC_MAT_SHADER_VAR_VALUE_PROP);

							cb->SetValueT<glm::bvec4>(varName, varData);
						}
						break;

					default:
						break;
				}
			}
			break;

		case ShaderVariableType::Int:
			{
				switch (columns)
				{
					case 2:
						{
							glm::ivec2 varData = xmlParser.GetVect2i(XE_GAME_OBJ_GOC_MAT_SHADER_VAR_VALUE_PROP);

							cb->SetValueT<glm::ivec2>(varName, varData);
						}
						break;

					case 3:
						{
							glm::ivec3 varData = xmlParser.GetVect3i(XE_GAME_OBJ_GOC_MAT_SHADER_VAR_VALUE_PROP);

							cb->SetValueT<glm::ivec3>(varName, varData);
						}
						break;

					case 4:
						{
							glm::ivec4 varData = xmlParser.GetVect4i(XE_GAME_OBJ_GOC_MAT_SHADER_VAR_VALUE_PROP);

							cb->SetValueT<glm::ivec4>(varName, varData);
						}
						break;

					default:
						break;
				}
			}
			break;

		case ShaderVariableType::Float:
			{
				switch (columns)
				{
					case 2:
						{
							glm::vec2 varData = xmlParser.GetVect2f(XE_GAME_OBJ_GOC_MAT_SHADER_VAR_VALUE_PROP);

							cb->SetValueT<glm::vec2>(varName, varData);
						}
						break;

					case 3:
						{
							glm::vec3 varData = xmlParser.GetVect3f(XE_GAME_OBJ_GOC_MAT_SHADER_VAR_VALUE_PROP);

							cb->SetValueT<glm::vec3>(varName, varData);
						}
						break;

					case 4:
						{
							glm::vec4 varData = xmlParser.GetVect4f(XE_GAME_OBJ_GOC_MAT_SHADER_VAR_VALUE_PROP);

							cb->SetValueT<glm::vec4>(varName, varData);
						}
						break;

					default:
						break;
				}
			}
			break;

		case ShaderVariableType::Double:
			{
				switch (columns)
				{
					case 2:
					{
						glm::dvec2 varData = xmlParser.GetVect2d(XE_GAME_OBJ_GOC_MAT_SHADER_VAR_VALUE_PROP);

						cb->SetValueT<glm::dvec2>(varName, varData);
					}
					break;

					case 3:
					{
						glm::dvec3 varData = xmlParser.GetVect3d(XE_GAME_OBJ_GOC_MAT_SHADER_VAR_VALUE_PROP);

						cb->SetValueT<glm::dvec3>(varName, varData);
					}
					break;

					case 4:
					{
						glm::dvec4 varData = xmlParser.GetVect4d(XE_GAME_OBJ_GOC_MAT_SHADER_VAR_VALUE_PROP);

						cb->SetValueT<glm::dvec4>(varName, varData);
					}
					break;

					default:
						break;
				}
			}
			break;

		default:
			break;
	}

	return XEResult::Ok;
}

XEResult GameObjectManager::LoadXMLCBMatrix(XEXMLParser& xmlParser, ConstantBuffer* cb, const std::wstring& varName, ShaderVariableType varType, uint32_t columns, uint32_t rows)
{
	if (cb == nullptr)
	{
		return XEResult::NullParameter;
	}

	if (varType != ShaderVariableType::Float)
	{
		return XEResult::Ok;
	}

	if (columns == 2 && rows == 2)
	{
		glm::mat2 varData = xmlParser.GetMat2f(XE_GAME_OBJ_GOC_MAT_SHADER_VAR_VALUE_PROP);

		cb->SetValueT<glm::mat2>(varName, varData);
	}
	else if (columns == 3 && rows == 3)
	{
		glm::mat3 varData = xmlParser.GetMat3f(XE_GAME_OBJ_GOC_MAT_SHADER_VAR_VALUE_PROP);

		cb->SetValueT<glm::mat3>(varName, varData);
	}
	else if (columns == 4 && rows == 4)
	{
		glm::mat4 varData = xmlParser.GetMat4f(XE_GAME_OBJ_GOC_MAT_SHADER_VAR_VALUE_PROP);

		cb->SetValueT<glm::mat4>(varName, varData);
	}

	return XEResult::Ok;
}

XEResult GameObjectManager::LoadXMLGameObjectScriptsComponents(XEXMLParser& xmlParser, GameObject* gameObject)
{
	if (gameObject == nullptr)
	{
		return XEResult::NullParameter;
	}

	uint32_t childCount = xmlParser.GetNumChildren();
	for (uint32_t i = 0; i < childCount; ++i)
	{
		XEXMLParser childObject = xmlParser(i);

		std::wstring childType = childObject.GetName();

		if (childType.compare(XE_GAME_OBJ_GOC_SCRIPT_NODE_NAME) == 0)
		{
			if (LoadXMLGameObjectScriptsComponent(childObject, gameObject) != XEResult::Ok)
			{
				return XEResult::Fail;
			}
		}
	}

	return XEResult::Ok;
}

XEResult GameObjectManager::LoadXMLGameObjectScriptsComponent(XEXMLParser& xmlParser, GameObject* gameObject)
{
	if (gameObject == nullptr)
	{
		return XEResult::NullParameter;
	}

	std::wstring name	= xmlParser.GetString(XE_GAME_OBJ_GOC_SCRIPT_NAME_PROP);
	uint64_t assetID	= xmlParser.GetUInt64(XE_GAME_OBJ_COMPONENT_ASSETID_PROP);
	bool hasInstance	= xmlParser.GetBool(XE_GAME_OBJ_GOC_SCRIPT_HAS_INSTANCE_PROP);

	GameObjectScriptGOC* gameObjectScriptGOC = new GameObjectScriptGOC(gameObject, name, m_AngelScriptManager, m_GameObjectScriptManager);

	if (hasInstance)
	{
		std::wstring instanceIname = xmlParser.GetString(XE_GAME_OBJ_GOC_SCRIPT_INSTANCE_NAME_PROP);
		if (gameObjectScriptGOC->CreateGameObjectScriptInstance(instanceIname) != XEResult::Ok)
		{
			DeleteMem(gameObjectScriptGOC);

			return XEResult::Fail;
		}

		GameObjectScriptProperties* properties = gameObjectScriptGOC->GetGameObjectScriptProperties();
		if (LoadXMLGameObjectScriptsProperties(xmlParser, properties) != XEResult::Ok)
		{
			DeleteMem(gameObjectScriptGOC);

			return XEResult::Fail;
		}
	}

	gameObject->AddGameObjectScriptGOC(gameObjectScriptGOC);

	return XEResult::Ok;
}

XEResult GameObjectManager::LoadXMLGameObjectScriptsProperties(XEXMLParser& xmlParser, GameObjectScriptProperties* properties)
{
	if (properties == nullptr)
	{
		return XEResult::NullParameter;
	}


	uint32_t childCount = xmlParser.GetNumChildren();
	for (uint32_t i = 0; i < childCount; ++i)
	{
		XEXMLParser childObject = xmlParser(i);

		std::wstring childType = childObject.GetName();

		if (childType.compare(XE_GAME_OBJ_GOC_SCRIPT_PROPERTY_NODE_NAME) == 0)
		{
			std::wstring propName = xmlParser.GetString(XE_GAME_OBJ_GOC_SCRIPT_PROPERTY_NAME_PROP);
			asETypeIdFlags propType = (asETypeIdFlags)xmlParser.GetUInt(XE_GAME_OBJ_GOC_SCRIPT_PROPERTY_TYPE_PROP);

			GameObjectScriptProperty* prop = properties->GetGameObjectScriptProperty(propName);
			if (prop == nullptr || prop->m_PropertyType != propType)
			{
				return XEResult::Fail;
			}

			switch (prop->m_PropertyType)
			{
				case asTYPEID_BOOL:
					{
						bool value = xmlParser.GetBool(XE_GAME_OBJ_GOC_SCRIPT_PROPERTY_VALUE_PROP);

						memcpy(prop->m_PropertyAddress, &value, sizeof(bool));
					}
					break;

				case asTYPEID_INT8:
					{
						int8_t value = xmlParser.GetInt8(XE_GAME_OBJ_GOC_SCRIPT_PROPERTY_VALUE_PROP);

						memcpy(prop->m_PropertyAddress, &value, sizeof(int8_t));
					}
					break;

				case asTYPEID_INT16:
					{
						int16_t value = xmlParser.GetInt16(XE_GAME_OBJ_GOC_SCRIPT_PROPERTY_VALUE_PROP);

						memcpy(prop->m_PropertyAddress, &value, sizeof(int16_t));
					}
					break;

				case asTYPEID_INT32:
					{
						int32_t value = xmlParser.GetInt(XE_GAME_OBJ_GOC_SCRIPT_PROPERTY_VALUE_PROP);

						memcpy(prop->m_PropertyAddress, &value, sizeof(int32_t));
					}
					break;

				case asTYPEID_INT64:
					{
						int64_t value = xmlParser.GetInt64(XE_GAME_OBJ_GOC_SCRIPT_PROPERTY_VALUE_PROP);

						memcpy(prop->m_PropertyAddress, &value, sizeof(int64_t));
					}
					break;

				case asTYPEID_UINT8:
					{
						uint8_t value = xmlParser.GetUInt8(XE_GAME_OBJ_GOC_SCRIPT_PROPERTY_VALUE_PROP);

						memcpy(prop->m_PropertyAddress, &value, sizeof(uint8_t));
					}
					break;

				case asTYPEID_UINT16:
					{
						uint16_t value = xmlParser.GetUInt16(XE_GAME_OBJ_GOC_SCRIPT_PROPERTY_VALUE_PROP);

						memcpy(prop->m_PropertyAddress, &value, sizeof(uint16_t));
					}
					break;

				case asTYPEID_UINT32:
					{
						uint32_t value = xmlParser.GetUInt(XE_GAME_OBJ_GOC_SCRIPT_PROPERTY_VALUE_PROP);

						memcpy(prop->m_PropertyAddress, &value, sizeof(uint32_t));
					}
					break;

				case asTYPEID_UINT64:
					{
						uint64_t value = xmlParser.GetUInt64(XE_GAME_OBJ_GOC_SCRIPT_PROPERTY_VALUE_PROP);

						memcpy(prop->m_PropertyAddress, &value, sizeof(uint64_t));
					}
					break;

				case asTYPEID_FLOAT:
					{
						float value = xmlParser.GetFloat(XE_GAME_OBJ_GOC_SCRIPT_PROPERTY_VALUE_PROP);

						memcpy(prop->m_PropertyAddress, &value, sizeof(float));
					}
					break;

				case asTYPEID_DOUBLE:
					{
						double value = xmlParser.GetDouble(XE_GAME_OBJ_GOC_SCRIPT_PROPERTY_VALUE_PROP);

						memcpy(prop->m_PropertyAddress, &value, sizeof(double));
					}
					break;

				default:
					break;
			}
		}
	}

	return XEResult::Ok;
}

XEResult GameObjectManager::LoadXMLLightComponent(XEXMLParser& xmlParser, GameObject* gameObject)
{
	if (gameObject == nullptr)
	{
		return XEResult::NullParameter;
	}

	LightGOC* lightGOC = new LightGOC(gameObject, m_LightManager);

	bool isDrawFurstumCascadeEnable		= xmlParser.GetBool(XE_GAME_OBJ_GOC_LIGHT_FRUSTUM_CASCADE_PROP);
	bool isDrawDebugEnable				= xmlParser.GetBool(XE_GAME_OBJ_GOC_LIGHT_DEBUG_DRAW_PROP);
	bool isDrawFrustumSimple			= xmlParser.GetBool(XE_GAME_OBJ_GOC_LIGHT_DRAW_FRUSTUM_SIMPLE_PROP);
	bool isDrawFrustumEnable			= xmlParser.GetBool(XE_GAME_OBJ_GOC_LIGHT_DRAW_FRUSTUM_ENABLE_PROP);

	lightGOC->SetDrawFrustumCascadesEnabled(isDrawFurstumCascadeEnable);
	lightGOC->SetDrawDebugEnabled(isDrawDebugEnable);
	lightGOC->SetDrawFrustumSimple(isDrawFrustumSimple);
	lightGOC->SetDrawFrustumEnabled(isDrawFrustumEnable);

	LightType lightType = (LightType)xmlParser.GetUInt(XE_GAME_OBJ_GOC_LIGHT_LIGHT_TYPE_PROP);

	if (lightGOC->ChangeLightType(lightType) != XEResult::Ok)
	{
		DeleteMem(lightGOC);

		return XEResult::Ok;
	}

	bool isEnabled			= xmlParser.GetBool(XE_GAME_OBJ_GOC_LIGHT_ENABLED_PROP);
	bool isShadowEnabled	=  xmlParser.GetBool(XE_GAME_OBJ_GOC_LIGHT_SHADOW_ENABLED_PROP);
	float nearAttenuation	= xmlParser.GetFloat(XE_GAME_OBJ_GOC_LIGHT_NEAR_ATTENUATION_PROP);
	float farAttenuation	= xmlParser.GetFloat(XE_GAME_OBJ_GOC_LIGHT_FAR_ATTENUATION_PROP);
	Color lightColor		= xmlParser.GetVect4f(XE_GAME_OBJ_GOC_LIGHT_COLOR_PROP);

	Light* light = lightGOC->GetLight();

	light->SetEnabled(isEnabled);
	light->SetShadowEnabled(isShadowEnabled);
	light->SetNearAttenuation(nearAttenuation);
	light->SetFarAttenuation(farAttenuation);
	light->SetColor(lightColor);

	switch (lightType)
	{
		case LightType::Spot:
			{
				SpotLight* spotLight = reinterpret_cast<SpotLight*>(light);

				float fallOffAngle	= xmlParser.GetFloat(XE_GAME_OBJ_GOC_LIGHT_FALL_OFF_ANGLE_PROP);
				float angle			= xmlParser.GetFloat(XE_GAME_OBJ_GOC_LIGHT_ANGLE_PROP);

				spotLight->SetFallOffAngle(fallOffAngle);
				spotLight->SetAngle(angle);
			}
			break;

		case LightType::Directional:
			break;

		case LightType::Omni:
			break;

		default:
			return XEResult::InvalidObjType;
	}

	gameObject->SetLightGOC(lightGOC);

	return XEResult::Ok;
}

XEResult GameObjectManager::LoadXMLMeshAnimationComponent(XEXMLParser& xmlParser, GameObject* gameObject)
{
	if (gameObject == nullptr)
	{
		return XEResult::NullParameter;
	}

	bool blendAnim = xmlParser.GetBool(XE_GAME_OBJ_GOC_ANIM_BLEND_ANIM_PROP);
	float blendTime = xmlParser.GetFloat(XE_GAME_OBJ_GOC_ANIM_BLEND_TIME_PROP);
	bool onLoop = xmlParser.GetBool(XE_GAME_OBJ_GOC_ANIM_ON_LOOP_ANIM_PROP);
	uint64_t skeletonID = xmlParser.GetUInt64(XE_GAME_OBJ_GOC_ANIM_SKELETON_ASSET_ID_PROP);

	MeshAnimationGOC* animGOC = new MeshAnimationGOC(gameObject);

	animGOC->SetBlendAnimation(blendAnim);
	animGOC->SetBlendTime(blendTime);
	animGOC->SetOnLoop(onLoop);

	if (skeletonID != 0)
	{
		GameAsset* gameAsset = m_GameAssetManager->GetGameAsset(skeletonID);
		if (gameAsset == nullptr || gameAsset->GetGameContentType() != GameContentType::Skeleton)
		{
			DeleteMem(animGOC);

			return XEResult::InvalidObjType;
		}

		SkeletonAsset* skeletonAsset = reinterpret_cast<SkeletonAsset*>(gameAsset);
		if (animGOC->SetSkeletonAsset(skeletonAsset) != XEResult::Ok)
		{
			DeleteMem(animGOC);

			return XEResult::Fail;
		}
	}

	uint32_t childCount = xmlParser.GetNumChildren();
	for (uint32_t i = 0; i < childCount; ++i)
	{
		XEXMLParser childObject = xmlParser(i);

		std::wstring childType = childObject.GetName();

		if (childType.compare(XE_GAME_OBJ_GOC_ANIM_ASSET_NODE_NAME) == 0)
		{
			uint64_t animAssetID = xmlParser.GetUInt64(XE_GAME_OBJ_COMPONENT_ASSETID_PROP);

			GameAsset* gameAsset = m_GameAssetManager->GetGameAsset(animAssetID);
			if (gameAsset == nullptr || gameAsset->GetGameContentType() != GameContentType::Animation)
			{
				DeleteMem(animGOC);

				return XEResult::Fail;
			}

			AnimationAsset* animAsset = reinterpret_cast<AnimationAsset*>(gameAsset);
			if (animGOC->AddAnimationAsset(animAsset) != XEResult::Ok)
			{
				DeleteMem(animGOC);

				return XEResult::Fail;
			}
		}
	}

	return XEResult::Ok;
}

XEResult GameObjectManager::LoadXMLCameraComponent(XEXMLParser& xmlParser, GameObject* gameObject)
{
	if (gameObject == nullptr)
	{
		return XEResult::NullParameter;
	}

	bool isDebugDrawEnable		= xmlParser.GetBool(XE_GAME_OBJ_GOC_CAM_DEBUG_DRAW_ENABLED_PROP);
	bool isDefaultCamera		= xmlParser.GetBool(XE_GAME_OBJ_GOC_CAM_IS_DEFAULT_CAM_PROP);
	bool isDrawFrustumEnabled	= xmlParser.GetBool(XE_GAME_OBJ_GOC_CAM_DRAW_FRUSTUM_ENABLED_PROP);
	bool isDrawFrustumSimple	= xmlParser.GetBool(XE_GAME_OBJ_GOC_CAM_DRAW_FRUSTUM_SIMPLE_PROP);

	CameraGOC* cameraGOC = new CameraGOC(gameObject, m_CameraManager, m_GraphicDevice);

	cameraGOC->SetDebugDrawEnabled(isDebugDrawEnable);
	cameraGOC->SetDrawFrustumEnabled(isDrawFrustumEnabled);
	cameraGOC->SetDrawFrustumSimple(isDrawFrustumSimple);

	if (isDefaultCamera)
	{
		if (cameraGOC->SetAsDefaultCamera() != XEResult::Ok)
		{
			DeleteMem(cameraGOC);

			return XEResult::Fail;
		}
	}

	if (gameObject->SetCameraGOC(cameraGOC) != XEResult::Ok)
	{
		DeleteMem(cameraGOC);

		return XEResult::Fail;
	}

	return XEResult::Ok;
}

XEResult GameObjectManager::LoadXMLAudioListenerComponent(XEXMLParser& xmlParser, GameObject* gameObject)
{
	if (gameObject == nullptr)
	{
		return XEResult::NullParameter;
	}

	AudioListenerGOC* audioListener = new AudioListenerGOC(gameObject, m_AudioManager);

	if (gameObject->SetAudioListenerGOC(audioListener) != XEResult::Ok)
	{
		DeleteMem(audioListener);

		return XEResult::Fail;
	}

	return XEResult::Ok;
}

XEResult GameObjectManager::LoadXMLAudioSourceComponent(XEXMLParser& xmlParser, GameObject* gameObject)
{
	if (gameObject == nullptr)
	{
		return XEResult::NullParameter;
	}

	uint32_t childCount = xmlParser.GetNumChildren();
	for (uint32_t i = 0; i < childCount; ++i)
	{
		XEXMLParser childObject = xmlParser(i);

		std::wstring childType = childObject.GetName();

		if (childType.compare(XE_GAME_OBJ_GOC_AUDIO_SOURCE_NODE_NAME) == 0)
		{
			std::wstring name		= xmlParser.GetString(XE_GAME_OBJ_GOC_AUDIO_SOURCE_NAME_PROP);
			uint64_t audioAssetID	= xmlParser.GetUInt64(XE_GAME_OBJ_COMPONENT_ASSETID_PROP);

			AudioSourceGOC* audioSourceGOC = new AudioSourceGOC(gameObject, name);

			GameAsset* gameAsset = m_GameAssetManager->GetGameAsset(audioAssetID);
			if (gameAsset == nullptr || gameAsset->GetGameContentType() != GameContentType::Audio)
			{
				DeleteMem(audioSourceGOC);

				return XEResult::Fail;
			}

			AudioAsset* audioAsset = reinterpret_cast<AudioAsset*>(gameAsset);
			if (audioSourceGOC->SetAudioAsset(audioAsset) != XEResult::Ok)
			{
				DeleteMem(audioSourceGOC);

				return XEResult::Fail;
			}

			if (gameObject->AddAudioSourceGOC(audioSourceGOC) != XEResult::Ok)
			{
				DeleteMem(audioSourceGOC);

				return XEResult::Fail;
			}
		}
	}

	return XEResult::Ok;
}

XEResult GameObjectManager::LoadXMLPhysicsComponent(XEXMLParser& xmlParser, GameObject* gameObject)
{
	if (gameObject == nullptr)
	{
		return XEResult::NullParameter;
	}

	PhysicsGOC* physicsGOC = new PhysicsGOC(gameObject, m_PhysicsManager);

	bool isRigidBody = xmlParser.GetBool(XE_GAME_OBJ_GOC_PHYSICS_IS_RIGID_BODY_PROP);

	if (isRigidBody)
	{
		if (physicsGOC->AddRigidBody() != XEResult::Ok)
		{
			DeleteMem(physicsGOC);

			return XEResult::Fail;
		}
	}

	uint32_t childCount = xmlParser.GetNumChildren();
	for (uint32_t i = 0; i < childCount; ++i)
	{
		XEXMLParser childObject = xmlParser(i);

		std::wstring childType = childObject.GetName();

		if (childType.compare(XE_GAME_OBJ_GOC_PHYSICS_COLLIDER_NODE_NAME) == 0)
		{
			CollisionShape shape	= (CollisionShape)xmlParser.GetUInt8(XE_GAME_OBJ_GOC_PHYSICS_COLLIDER_TYPE_PROP);
			glm::vec3 scale			= xmlParser.GetVect3f(XE_GAME_OBJ_GOC_PHYSICS_COLLIDER_SCALE_PROP);

			uint64_t colliderID = 0;
			if (physicsGOC->AddCollider(shape, colliderID) != XEResult::Ok)
			{
				DeleteMem(physicsGOC);

				return XEResult::Fail;
			}

			PhysicsActor* actor = physicsGOC->GetPhysicsActor();
			PhysicCollider* collider = actor->GetPhysicCollider(colliderID);

			collider->SetScale(scale);

			switch (shape)
			{
				case CollisionShape::Box:
					{
						PhysicColliderBox* box = reinterpret_cast<PhysicColliderBox*>(collider);

						glm::vec3 size = xmlParser.GetVect3f(XE_GAME_OBJ_GOC_PHYSICS_COLLIDER_SIZE_PROP);

						box->SetSize(size);
					}
					break;

				case CollisionShape::Sphere:
					{
						PhysicColliderSphere* sphere = reinterpret_cast<PhysicColliderSphere*>(collider);

						float radius = xmlParser.GetFloat(XE_GAME_OBJ_GOC_PHYSICS_COLLIDER_RADIUS_PROP);

						sphere->SetRadius(radius);
					}
					break;

				default:
					DeleteMem(physicsGOC);
					return XEResult::Fail;
			}
		}
	}

	return XEResult::Ok;
}

GameObjectMapIt GameObjectManager::begin()
{
	return m_GameObjectMap.begin();
}

GameObjectMapIt GameObjectManager::end()
{
	return m_GameObjectMap.end();
}

GameObjectMapItConst GameObjectManager::begin() const
{
	return m_GameObjectMap.begin();
}

GameObjectMapItConst GameObjectManager::end() const
{
	return m_GameObjectMap.end();
}

GameObjectMapItConst GameObjectManager::cbegin() const
{
	return m_GameObjectMap.cbegin();
}

GameObjectMapItConst GameObjectManager::cend() const
{
	return m_GameObjectMap.cend();
}
