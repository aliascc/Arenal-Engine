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
#include "GameObject.h"
#include "XML\XEXMLWriter.h"
#include "GameObjectManager.h"
#include "GameObject\Components\MeshGOC.h"
#include "Shaders\Buffers\ConstantBuffer.h"
#include "GameObject\Components\MeshMaterialGOC.h"
#include "Shaders\Bindings\ShaderTextureBinding.h"
#include "GameObject\Scripts\GameObjectScriptDefs.h"
#include "GameObject\Components\GameObjectScriptGOC.h"
#include "Graphic Extensions\Shader Extensions\Properties\ShaderProperties.h"

//Always include last
#include "Memory\MemLeaks.h"

/********************
*   Function Defs   *
*********************/
XETODO("Check for Mutex");
GameObjectManager::GameObjectManager()
{
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

XEResult GameObjectManager::SaveToXML(const std::wstring& file)
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

XEResult GameObjectManager::SaveToXMLGameObject(XEXMLWriter& xmlWriter, GameObject* gameObject)
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

XEResult GameObjectManager::SaveToXMLGameObjectComponents(XEXMLWriter& xmlWriter, GameObject* gameObject)
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

	ret = SaveToXMLGameObject(xmlWriter, gameObject);
	if (ret != XEResult::Ok)
	{
		XETODO("Better return code");
		return XEResult::Fail;
	}

	ret = SaveToXMLGameObjectComponents(xmlWriter, gameObject);
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

XEResult GameObjectManager::SaveToXMLMeshComponent(XEXMLWriter& xmlWriter, GameObject* gameObject)
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

XEResult GameObjectManager::SaveToXMLMeshMaterialsComponent(XEXMLWriter& xmlWriter, GameObject* gameObject)
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
	ret = xmlWriter.StartNode(XE_GAME_OBJ_GOC_MAT_NODE_NAME);
	if (ret != XEResult::Ok)
	{
		XETODO("Better return code");
		return XEResult::Fail;
	}

	const MeshMaterialsGOCList& materialList = gameObject->GetMeshMaterialsGOCList();
	for (auto material : materialList)
	{
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

XEResult GameObjectManager::SaveToXMLShader(XEXMLWriter& xmlWriter, ShaderType shaderType, uint64_t assetID, ShaderProperties* properties)
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

XEResult GameObjectManager::SaveToXMLShaderProperties(XEXMLWriter& xmlWriter, ShaderProperties* properties)
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

XEResult GameObjectManager::SaveToXMLCBShaderVariable(XEXMLWriter& xmlWriter, const ShaderCustomVariable* scv, ConstantBuffer* cb)
{
	if (cb == nullptr || scv == nullptr)
	{
		return XEResult::NullParameter;
	}

	XEResult ret = XEResult::Ok;

	xmlWriter.WriteString(XE_GAME_OBJ_GOC_MAT_SHADER_VAR_NAME_PROP, scv->m_Name);
	xmlWriter.WriteUInt(XE_GAME_OBJ_GOC_MAT_SHADER_VAR_CLASS_PROP, (uint32_t)scv->m_ShaderVariableClass);
	xmlWriter.WriteUInt(XE_GAME_OBJ_GOC_MAT_SHADER_VAR_TYPE_PROP, (uint32_t)scv->m_ShaderVariableType);

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

XEResult GameObjectManager::SaveToXMLCBScalar(XEXMLWriter& xmlWriter, ConstantBuffer* cb, const std::wstring& varName, ShaderVariableType varType)
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

XEResult GameObjectManager::SaveToXMLCBVector(XEXMLWriter& xmlWriter, ConstantBuffer* cb, const std::wstring& varName, ShaderVariableType varType, uint32_t columns)
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

XEResult GameObjectManager::SaveToXMLCBMatrix(XEXMLWriter& xmlWriter, ConstantBuffer* cb, const std::wstring& varName, ShaderVariableType varType, uint32_t columns, uint32_t rows)
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

XEResult GameObjectManager::SaveToXMLGameObjectScriptsComponent(XEXMLWriter& xmlWriter, GameObject* gameObject)
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
		xmlWriter.WriteUInt64(XE_GAME_OBJ_COMPONENT_ASSETID_PROP, gosComponent->GetGameObjectScriptAssetID());
		xmlWriter.WriteBool(XE_GAME_OBJ_GOC_SCRIPT_HAS_INSTANCE_PROP, gosComponent->HasScriptInstance());

		if (gosComponent->HasScriptInstance())
		{
			xmlWriter.WriteString(XE_GAME_OBJ_GOC_SCRIPT_INSTANCE_NAME_PROP, gosComponent->GetGameObjectScriptInstanceName());

			GameObjectScriptProperties* properties = gosComponent->GetGameObjectScriptProperties();
			
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

XEResult GameObjectManager::SaveToXMLLightComponent(XEXMLWriter& xmlWriter, GameObject* gameObject)
{
	if (gameObject == nullptr)
	{
		return XEResult::NullParameter;
	}

	XEResult ret = XEResult::Ok;

	//////////////////////////////
	////Write Game Object Components
	//ret = xmlWriter.StartNode(XE_GAME_OBJ_COMPONENT_NODE_NAME);
	//if (ret != XEResult::Ok)
	//{
	//	XETODO("Better return code");
	//	return XEResult::Fail;
	//}



	//////////////////////////////
	////End of XE_GAME_OBJ_COMPONENT_NODE_NAME
	//ret = xmlWriter.EndNode();
	//if (ret != XEResult::Ok)
	//{
	//	XETODO("Better return code");
	//	return XEResult::Fail;
	//}

	return XEResult::Ok;
}

XEResult GameObjectManager::SaveToXMLMeshAnimationComponent(XEXMLWriter& xmlWriter, GameObject* gameObject)
{
	if (gameObject == nullptr)
	{
		return XEResult::NullParameter;
	}

	XEResult ret = XEResult::Ok;

	//////////////////////////////
	////Write Game Object Components
	//ret = xmlWriter.StartNode(XE_GAME_OBJ_COMPONENT_NODE_NAME);
	//if (ret != XEResult::Ok)
	//{
	//	XETODO("Better return code");
	//	return XEResult::Fail;
	//}



	//////////////////////////////
	////End of XE_GAME_OBJ_COMPONENT_NODE_NAME
	//ret = xmlWriter.EndNode();
	//if (ret != XEResult::Ok)
	//{
	//	XETODO("Better return code");
	//	return XEResult::Fail;
	//}

	return XEResult::Ok;
}

XEResult GameObjectManager::SaveToXMLCameraComponent(XEXMLWriter& xmlWriter, GameObject* gameObject)
{
	if (gameObject == nullptr)
	{
		return XEResult::NullParameter;
	}

	XEResult ret = XEResult::Ok;

	//////////////////////////////
	////Write Game Object Components
	//ret = xmlWriter.StartNode(XE_GAME_OBJ_COMPONENT_NODE_NAME);
	//if (ret != XEResult::Ok)
	//{
	//	XETODO("Better return code");
	//	return XEResult::Fail;
	//}



	//////////////////////////////
	////End of XE_GAME_OBJ_COMPONENT_NODE_NAME
	//ret = xmlWriter.EndNode();
	//if (ret != XEResult::Ok)
	//{
	//	XETODO("Better return code");
	//	return XEResult::Fail;
	//}

	return XEResult::Ok;
}

XEResult GameObjectManager::SaveToXMLAudioListenerComponent(XEXMLWriter& xmlWriter, GameObject* gameObject)
{
	if (gameObject == nullptr)
	{
		return XEResult::NullParameter;
	}

	XEResult ret = XEResult::Ok;

	//////////////////////////////
	////Write Game Object Components
	//ret = xmlWriter.StartNode(XE_GAME_OBJ_COMPONENT_NODE_NAME);
	//if (ret != XEResult::Ok)
	//{
	//	XETODO("Better return code");
	//	return XEResult::Fail;
	//}



	//////////////////////////////
	////End of XE_GAME_OBJ_COMPONENT_NODE_NAME
	//ret = xmlWriter.EndNode();
	//if (ret != XEResult::Ok)
	//{
	//	XETODO("Better return code");
	//	return XEResult::Fail;
	//}

	return XEResult::Ok;
}

XEResult GameObjectManager::SaveToXMLAudioSourceComponent(XEXMLWriter& xmlWriter, GameObject* gameObject)
{
	if (gameObject == nullptr)
	{
		return XEResult::NullParameter;
	}

	XEResult ret = XEResult::Ok;

	//////////////////////////////
	////Write Game Object Components
	//ret = xmlWriter.StartNode(XE_GAME_OBJ_COMPONENT_NODE_NAME);
	//if (ret != XEResult::Ok)
	//{
	//	XETODO("Better return code");
	//	return XEResult::Fail;
	//}



	//////////////////////////////
	////End of XE_GAME_OBJ_COMPONENT_NODE_NAME
	//ret = xmlWriter.EndNode();
	//if (ret != XEResult::Ok)
	//{
	//	XETODO("Better return code");
	//	return XEResult::Fail;
	//}

	return XEResult::Ok;
}

XEResult GameObjectManager::SaveToXMLPhysicsComponent(XEXMLWriter& xmlWriter, GameObject* gameObject)
{
	if (gameObject == nullptr)
	{
		return XEResult::NullParameter;
	}

	XEResult ret = XEResult::Ok;

	//////////////////////////////
	////Write Game Object Components
	//ret = xmlWriter.StartNode(XE_GAME_OBJ_COMPONENT_NODE_NAME);
	//if (ret != XEResult::Ok)
	//{
	//	XETODO("Better return code");
	//	return XEResult::Fail;
	//}



	//////////////////////////////
	////End of XE_GAME_OBJ_COMPONENT_NODE_NAME
	//ret = xmlWriter.EndNode();
	//if (ret != XEResult::Ok)
	//{
	//	XETODO("Better return code");
	//	return XEResult::Fail;
	//}

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
