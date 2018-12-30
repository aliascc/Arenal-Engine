/*
* Copyright (c) 2018 <Carlos Chacón>
* All rights reserved
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
* http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*/

#pragma once

#ifndef _GAME_OBJECT_MANAGER_H
#define _GAME_OBJECT_MANAGER_H

/**********************
*   System Includes   *
***********************/

/*************************
*   3rd Party Includes   *
**************************/

/***************************
*   Game Engine Includes   *
****************************/
#include "Shaders\ShaderDefs.h"

/********************
*   Forward Decls   *
*********************/
class GameObject;
class AEXMLParser;
class AEXMLWriter;
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

class GameObjectManager sealed : public AEObject
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

        GameAssetManager& m_GameAssetManager;

        GameObjectScriptManager& m_GameObjectScriptManager;

        AngelScriptManager& m_AngelScriptManager;

        LightManager& m_LightManager;

        CameraManager& m_CameraManager;

        GraphicDevice& m_GraphicDevice;

        AudioManager& m_AudioManager;

        PhysicsManager& m_PhysicsManager;

        GameObjectMap m_GameObjectMap;

#pragma endregion

        /**********************
        *   Private Methods   *
        ***********************/
#pragma region Private Methods

        void GameObjectMoveToChild(uint64_t id);

        GameObject* SearchGameObjectChilds(uint64_t goID, GameObject* gameObject) const;

        void CleanUp();

        AEResult SaveToXMLGameObject(AEXMLWriter& xmlWriter, GameObject& gameObject) const;

        AEResult SaveToXMLGameObjectComponents(AEXMLWriter& xmlWriter, GameObject& gameObject) const;

        AEResult SaveToXMLMeshComponent(AEXMLWriter& xmlWriter, GameObject& gameObject) const;

        AEResult SaveToXMLMeshMaterialsComponent(AEXMLWriter& xmlWriter, GameObject& gameObject) const;

        AEResult SaveToXMLShader(AEXMLWriter& xmlWriter, ShaderType shaderType, uint64_t assetID, ShaderProperties* properties) const;

        AEResult SaveToXMLShaderProperties(AEXMLWriter& xmlWriter, ShaderProperties* properties) const;

        AEResult SaveToXMLCBShaderVariable(AEXMLWriter& xmlWriter, const ShaderCustomVariable* scv, ConstantBuffer* cb) const;

        AEResult SaveToXMLCBScalar(AEXMLWriter& xmlWriter, ConstantBuffer* cb, const std::string& varName, ShaderVariableType varType) const;

        AEResult SaveToXMLCBVector(AEXMLWriter& xmlWriter, ConstantBuffer* cb, const std::string& varName, ShaderVariableType varType, uint32_t columns) const;

        AEResult SaveToXMLCBMatrix(AEXMLWriter& xmlWriter, ConstantBuffer* cb, const std::string& varName, ShaderVariableType varType, uint32_t columns, uint32_t rows) const;

        AEResult SaveToXMLGameObjectScriptsComponent(AEXMLWriter& xmlWriter, GameObject& gameObject) const;

        AEResult SaveToXMLGameObjectScriptsProperties(AEXMLWriter& xmlWriter, const GameObjectScriptProperties* properties) const;

        AEResult SaveToXMLLightComponent(AEXMLWriter& xmlWriter, GameObject& gameObject) const;

        AEResult SaveToXMLMeshAnimationComponent(AEXMLWriter& xmlWriter, GameObject& gameObject) const;

        AEResult SaveToXMLCameraComponent(AEXMLWriter& xmlWriter, GameObject& gameObject) const;

        AEResult SaveToXMLAudioListenerComponent(AEXMLWriter& xmlWriter, GameObject& gameObject) const;

        AEResult SaveToXMLAudioSourceComponent(AEXMLWriter& xmlWriter, GameObject& gameObject) const;

        AEResult SaveToXMLPhysicsComponent(AEXMLWriter& xmlWriter, GameObject& gameObject) const;

        AEResult LoadXMLGameObject(AEXMLParser& xmlParser, GameObject* parent);

        AEResult LoadXMLGameObjectComponents(AEXMLParser& xmlParser, GameObject& gameObject);

        AEResult LoadXMLMeshComponent(AEXMLParser& xmlParser, GameObject& gameObject);

        AEResult LoadXMLMeshMaterialsComponent(AEXMLParser& xmlParser, GameObject& gameObject);

        AEResult LoadXMLMeshMaterialComponent(AEXMLParser& xmlParser, GameObject& gameObject);

        AEResult LoadXMLShader(AEXMLParser& xmlParser, MeshMaterialGOC* material);

        AEResult LoadXMLShaderProperties(AEXMLParser& xmlParser, ShaderProperties* properties);

        AEResult LoadXMLCBShaderVariable(AEXMLParser& xmlParser, ConstantBuffer* cb);

        AEResult LoadXMLCBScalar(AEXMLParser& xmlParser, ConstantBuffer* cb, const std::string& varName, ShaderVariableType varType);

        AEResult LoadXMLCBVector(AEXMLParser& xmlParser, ConstantBuffer* cb, const std::string& varName, ShaderVariableType varType, uint32_t columns);

        AEResult LoadXMLCBMatrix(AEXMLParser& xmlParser, ConstantBuffer* cb, const std::string& varName, ShaderVariableType varType, uint32_t columns, uint32_t rows);

        AEResult LoadXMLGameObjectScriptsComponents(AEXMLParser& xmlParser, GameObject& gameObject);

        AEResult LoadXMLGameObjectScriptsComponent(AEXMLParser& xmlParser, GameObject& gameObject);

        AEResult LoadXMLGameObjectScriptsProperties(AEXMLParser& xmlParser, GameObjectScriptProperties* properties);

        AEResult LoadXMLLightComponent(AEXMLParser& xmlParser, GameObject& gameObject);

        AEResult LoadXMLMeshAnimationComponent(AEXMLParser& xmlParser, GameObject& gameObject);

        AEResult LoadXMLCameraComponent(AEXMLParser& xmlParser, GameObject& gameObject);

        AEResult LoadXMLAudioListenerComponent(AEXMLParser& xmlParser, GameObject& gameObject);

        AEResult LoadXMLAudioSourceComponent(AEXMLParser& xmlParser, GameObject& gameObject);

        AEResult LoadXMLPhysicsComponent(AEXMLParser& xmlParser, GameObject& gameObject);

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
        GameObjectManager(GraphicDevice& graphicDevice, GameAssetManager& gameAssetManager, GameObjectScriptManager& gameObjectScriptManager, AngelScriptManager& angelScriptManager, LightManager& lightManager, CameraManager& cameraManager, AudioManager& audioManager, PhysicsManager& physicsManager);

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

        AEResult SetSelectedGameObject(GameObject* gameObject);

#pragma endregion

        /************************
        *   Framework Methods   *
        *************************/
#pragma region Framework Methods

        AEResult LoadGameObjectManagerFile(const std::string& file);

        AEResult SaveToXML(const std::string& file) const;

        bool GameObjectExistsTopLevel(uint64_t goID) const;

        AEResult AddGameObject(GameObject* gameObject);

        GameObjectMapIt begin();

        GameObjectMapIt end();

        GameObjectMapItConst begin() const;

        GameObjectMapItConst end() const;

        GameObjectMapItConst cbegin() const;

        GameObjectMapItConst cend() const;

#pragma endregion

};

#endif
