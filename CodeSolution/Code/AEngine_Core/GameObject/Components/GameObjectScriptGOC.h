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

#ifndef _SCRIPT_GOC_H
#define _SCRIPT_GOC_H

/**********************
*   System Includes   *
***********************/

/*************************
*   3rd Party Includes   *
**************************/

/***************************
*   Game Engine Includes   *
****************************/
#include "angelscript.h"
#include "GameObject\GameObjectComponent.h"

/********************
*   Forward Decls   *
*********************/
class GameAsset;
class GameObject;
class GameObjectScript;
class AngelScriptManager;
class GameObjectScriptAsset;
class GameObjectScriptManager;
struct GameObjectScriptInstance;
struct GameObjectScriptFunctions;
struct GameObjectScriptProperties;

/*****************
*   Class Decl   *
******************/
class GameObjectScriptGOC sealed : public GameObjectComponent
{
    private:

        /************************
        *   Private Variables   *
        *************************/
#pragma region Private Variables

        AngelScriptManager* m_AngelScriptManager = nullptr;

        GameObjectScriptManager* m_GameObjectScriptManager = nullptr;

        GameObjectScriptInstance* m_GameObjectScriptInstance = nullptr;

        GameObjectScriptProperties* m_GameObjectScriptProperties = nullptr;

        GameObjectScriptProperties* m_TempGameObjectScriptProperties = nullptr;

        asIScriptContext* m_ASContext = nullptr;

        std::string m_Name = "";

        std::string m_TempInstanceName = "";

        /// <summary>
        /// Game Object Pair with Game Object Script that this Game Object Component uses
        /// </summary>
        GameObjectAssetPair<GameObjectScript> m_GameObjectScript;

        /// <summary>
        /// Is Ready flag that determines if the object 
        /// is initialize and ready to run.
        /// </summary>
        bool m_IsReady = false;

#pragma endregion

        /**********************
        *   Private Methods   *
        ***********************/
#pragma region Private Methods

        void CleanUp();

        void GameObjectScriptAssetDeletion(GameAsset* asset);

        void GameObjectScriptAssetReload(GameAsset* asset);

        void GameObjectScriptAssetPreReload(GameAsset* asset);

        AEResult ClearGameObjectScriptAsset(bool informGameAsset = true);

#pragma endregion

    public:

        /***************************************
        *   Constructor & Destructor Methods   *
        ****************************************/
#pragma region Constructor & Destructor Methods

        /// <summary>
        /// GameObjectScriptGOC Constructor
        /// </summary>
        /// <param name="gameObject">Game Object that this Component is attached too</param>
        /// <param name="name">Name of the Script GOC.</param>
        /// <param name="angelScriptManager">Angel Script Manager to associate the Scripts with.</param>
        /// <param name="gameObjectScriptManager">Game Object Script Manager to associate the scripts with.</param>
        GameObjectScriptGOC(GameObject* gameObject, const std::string& name, AngelScriptManager* angelScriptManager, GameObjectScriptManager* gameObjectScriptManager);

        /// <summary>
        /// Default GameObjectScriptGOC Destructor
        /// </summary>
        virtual ~GameObjectScriptGOC();

#pragma endregion

        /******************
        *   Get Methods   *
        *******************/
#pragma region Get Methods

        inline const std::string& GetName() const
        {
            return m_Name;
        }

        std::string GetGameObjectScriptName() const;

        std::string GetGameObjectScriptInstanceName() const;

        inline bool HasScriptInstance() const
        {
            return (m_GameObjectScriptInstance != nullptr);
        }

        inline GameObjectScriptProperties* GetGameObjectScriptProperties()
        {
            return m_GameObjectScriptProperties;
        }

        inline uint64_t GetGameObjectScriptAssetID() const
        {
            return m_GameObjectScript.m_AssetID;
        }

#pragma endregion

        /******************
        *   Set Methods   *
        *******************/
#pragma region Set Methods

        inline void SetName(const std::string& name)
        {
            m_Name = name;
        }

        AEResult SetGameObjectScriptAsset(GameObjectScriptAsset* asset);

#pragma endregion

        /************************
        *   Framework Methods   *
        *************************/
#pragma region Framework Methods

        AEResult CreateGameObjectScriptInstance(const std::string& instanceName);

        AEResult RemoveGameObjectScriptInstance();

        AEResult RemoveGameObjectScriptAsset();

        AEResult ExecuteScriptInitialize();

        AEResult ExecuteScriptConstantUpdate(const TimerParams& timerParams);

        AEResult ExecuteScriptUpdate(const TimerParams& timerParams);

        AEResult ExecuteScriptPostUpdate(const TimerParams& timerParams);

#pragma endregion

};

#endif
