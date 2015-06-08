/********************************************************
*
* Author: Carlos Chacón N.
*
* Copyright 2012-2015
*
*********************************************************/

#pragma once

#ifndef _GAME_OBJECT_MANAGER_H
#define _GAME_OBJECT_MANAGER_H

/**********************
*   System Includes   *
***********************/
#include <map>
#include <string>
#include <stdint.h>

/*************************
*   3rd Party Includes   *
**************************/

/***************************
*   Game Engine Includes   *
****************************/
#include "Base\Base.h"
#include "Shaders\ShaderDefs.h"
#include "Base\UniqueXEObject.h"

/********************
*   Forward Decls   *
*********************/
class GameObject;
class XEXMLParser;
class XEXMLWriter;
class AudioManager;
class LightManager;
class CameraManager;
class PhysicsManager;
class ConstantBuffer;
class MeshMaterialGOC;
class GameAssetManager;
class ShaderProperties;
class GameObjectManager;
class AngelScriptManager;
struct ShaderCustomVariable;
class GameObjectScriptManager;
struct GameObjectScriptProperties;

/***************
*   Typedefs   *
****************/
typedef std::map<uint64_t, GameObject*> GameObjectMap;

typedef GameObjectMap::iterator GameObjectMapIt;

typedef GameObjectMap::const_iterator GameObjectMapItConst;

/*****************
*   Class Decl   *
******************/

class GameObjectManager sealed : public UniqueXEObject
{
	private:

		/************************
		*   Private Variables   *
		*************************/
#pragma region Private Variables

		/// <summary>
		/// Specifies the current Game Object that is selected
		/// </summary>
		/// <remarks>
		/// Use by other objects to print information about it.
		/// Normally use by the editor
		/// </remarks>
		GameObject* m_SelectedGameObject = nullptr;

		GameAssetManager* m_GameAssetManager = nullptr;

		GameObjectScriptManager* m_GameObjectScriptManager = nullptr;

		AngelScriptManager* m_AngelScriptManager = nullptr;

		LightManager* m_LightManager = nullptr;

		CameraManager* m_CameraManager = nullptr;

		GraphicDevice* m_GraphicDevice = nullptr;

		AudioManager* m_AudioManager = nullptr;

		PhysicsManager* m_PhysicsManager = nullptr;

		GameObjectMap m_GameObjectMap;

#pragma endregion

		/**********************
		*   Private Methods   *
		***********************/
#pragma region Private Methods

		void GameObjectMoveToChild(uint64_t id);

		GameObject* SearchGameObjectChilds(uint64_t goID, GameObject* gameObject) const;

		void CleanUp();

		XEResult SaveToXMLGameObject(XEXMLWriter& xmlWriter, GameObject* gameObject) const;

		XEResult SaveToXMLGameObjectComponents(XEXMLWriter& xmlWriter, GameObject* gameObject) const;

		XEResult SaveToXMLMeshComponent(XEXMLWriter& xmlWriter, GameObject* gameObject) const;

		XEResult SaveToXMLMeshMaterialsComponent(XEXMLWriter& xmlWriter, GameObject* gameObject) const;

		XEResult SaveToXMLShader(XEXMLWriter& xmlWriter, ShaderType shaderType, uint64_t assetID, ShaderProperties* properties) const;

		XEResult SaveToXMLShaderProperties(XEXMLWriter& xmlWriter, ShaderProperties* properties) const;

		XEResult SaveToXMLCBShaderVariable(XEXMLWriter& xmlWriter, const ShaderCustomVariable* scv, ConstantBuffer* cb) const;

		XEResult SaveToXMLCBScalar(XEXMLWriter& xmlWriter, ConstantBuffer* cb, const std::wstring& varName, ShaderVariableType varType) const;

		XEResult SaveToXMLCBVector(XEXMLWriter& xmlWriter, ConstantBuffer* cb, const std::wstring& varName, ShaderVariableType varType, uint32_t columns) const;

		XEResult SaveToXMLCBMatrix(XEXMLWriter& xmlWriter, ConstantBuffer* cb, const std::wstring& varName, ShaderVariableType varType, uint32_t columns, uint32_t rows) const;

		XEResult SaveToXMLGameObjectScriptsComponent(XEXMLWriter& xmlWriter, GameObject* gameObject) const;

		XEResult SaveToXMLGameObjectScriptsProperties(XEXMLWriter& xmlWriter, const GameObjectScriptProperties* properties) const;

		XEResult SaveToXMLLightComponent(XEXMLWriter& xmlWriter, GameObject* gameObject) const;

		XEResult SaveToXMLMeshAnimationComponent(XEXMLWriter& xmlWriter, GameObject* gameObject) const;

		XEResult SaveToXMLCameraComponent(XEXMLWriter& xmlWriter, GameObject* gameObject) const;

		XEResult SaveToXMLAudioListenerComponent(XEXMLWriter& xmlWriter, GameObject* gameObject) const;

		XEResult SaveToXMLAudioSourceComponent(XEXMLWriter& xmlWriter, GameObject* gameObject) const;

		XEResult SaveToXMLPhysicsComponent(XEXMLWriter& xmlWriter, GameObject* gameObject) const;

		XEResult LoadXMLGameObject(XEXMLParser& xmlParser, GameObject* parent);

		XEResult LoadXMLGameObjectComponents(XEXMLParser& xmlParser, GameObject* gameObject);

		XEResult LoadXMLMeshComponent(XEXMLParser& xmlParser, GameObject* gameObject);

		XEResult LoadXMLMeshMaterialsComponent(XEXMLParser& xmlParser, GameObject* gameObject);

		XEResult LoadXMLMeshMaterialComponent(XEXMLParser& xmlParser, GameObject* gameObject);

		XEResult LoadXMLShader(XEXMLParser& xmlParser, MeshMaterialGOC* material);

		XEResult LoadXMLShaderProperties(XEXMLParser& xmlParser, ShaderProperties* properties);

		XEResult LoadXMLCBShaderVariable(XEXMLParser& xmlParser, ConstantBuffer* cb);

		XEResult LoadXMLCBScalar(XEXMLParser& xmlParser, ConstantBuffer* cb, const std::wstring& varName, ShaderVariableType varType);

		XEResult LoadXMLCBVector(XEXMLParser& xmlParser, ConstantBuffer* cb, const std::wstring& varName, ShaderVariableType varType, uint32_t columns);

		XEResult LoadXMLCBMatrix(XEXMLParser& xmlParser, ConstantBuffer* cb, const std::wstring& varName, ShaderVariableType varType, uint32_t columns, uint32_t rows);

		XEResult LoadXMLGameObjectScriptsComponents(XEXMLParser& xmlParser, GameObject* gameObject);

		XEResult LoadXMLGameObjectScriptsComponent(XEXMLParser& xmlParser, GameObject* gameObject);

		XEResult LoadXMLGameObjectScriptsProperties(XEXMLParser& xmlParser, GameObjectScriptProperties* properties);

		XEResult LoadXMLLightComponent(XEXMLParser& xmlParser, GameObject* gameObject);

		XEResult LoadXMLMeshAnimationComponent(XEXMLParser& xmlParser, GameObject* gameObject);

		XEResult LoadXMLCameraComponent(XEXMLParser& xmlParser, GameObject* gameObject);

		XEResult LoadXMLAudioListenerComponent(XEXMLParser& xmlParser, GameObject* gameObject);

		XEResult LoadXMLAudioSourceComponent(XEXMLParser& xmlParser, GameObject* gameObject);

		XEResult LoadXMLPhysicsComponent(XEXMLParser& xmlParser, GameObject* gameObject);

#pragma endregion

	public:

		/***************************************
		*   Constructor & Destructor Methods   *
		****************************************/
#pragma region Constructor & Destructor Methods

		/// <summary>
		/// Default GameObjectManager Constructor
		/// </summary>
		/// <param name="graphicDevice">Graphic Device</param>
		/// <param name="gameAssetManager">Game Asset Manager</param>
		/// <param name="gameObjectScriptManager">Game Object Script Manager</param>
		/// <param name="angelScriptManager">Angel Script Manager</param>
		/// <param name="lightManager">Light Manager</param>
		/// <param name="cameraManager">Camera Manager</param>
		/// <param name="audioManager">Audio Manager</param>
		/// <param name="physicsManager">Physics Manager</param>
		GameObjectManager(GraphicDevice* graphicDevice, GameAssetManager* gameAssetManager, GameObjectScriptManager* gameObjectScriptManager, AngelScriptManager* angelScriptManager, LightManager* lightManager, CameraManager* cameraManager, AudioManager* audioManager, PhysicsManager* physicsManager);

		/// <summary>
		/// Default GameObjectManager Destructor
		/// </summary>
		virtual ~GameObjectManager();

#pragma endregion

		/******************
		*   Get Methods   *
		*******************/
#pragma region Get Methods

		inline GameObject* GetSelectedGameObject() const
		{
			return m_SelectedGameObject;
		}

		GameObject* GetGameObject(uint64_t goID) const;

#pragma endregion

		/******************
		*   Set Methods   *
		*******************/
#pragma region Set Methods

		XEResult SetSelectedGameObject(GameObject* gameObject);

#pragma endregion

		/************************
		*   Framework Methods   *
		*************************/
#pragma region Framework Methods

		XEResult LoadGameObjectManagerFile(const std::wstring& file);

		XEResult SaveToXML(const std::wstring& file) const;

		bool GameObjectExistsTopLevel(uint64_t goID) const;

		XEResult AddGameObject(GameObject* gameObject);

		GameObjectMapIt begin();

		GameObjectMapIt end();

		GameObjectMapItConst begin() const;

		GameObjectMapItConst end() const;

		GameObjectMapItConst cbegin() const;

		GameObjectMapItConst cend() const;

#pragma endregion

};

#endif