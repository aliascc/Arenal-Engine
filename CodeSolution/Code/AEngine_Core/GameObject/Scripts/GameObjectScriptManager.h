/*
* Copyright (c) 2018 <Carlos Chac�n>
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

#ifndef _GAME_OBJECT_SCRIPT_MANAGER_H
#define _GAME_OBJECT_SCRIPT_MANAGER_H

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
#include "GameObjectScriptDefs.h"

/********************
*   Forward Decls   *
*********************/

/***************
*   Typedefs   *
****************/
typedef std::unordered_map<std::string, GameObjectScriptInstance*> GameObjectScriptMap;
typedef GameObjectScriptMap::iterator GameObjectScriptMapIt;
typedef GameObjectScriptMap::const_iterator GameObjectScriptMapItConst;

/*****************
*   Class Decl   *
******************/

class GameObjectScriptManager sealed : public AEObject
{
    private:

        /************************
        *   Private Variables   *
        *************************/
#pragma region Private Variables

        /// <summary>
        /// Game Object Script Map.
        /// </summary>
        GameObjectScriptMap m_GameObjectScriptMap;

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

        /// <summary>
        /// Clean up memory use by the class.
        /// </summary>
        void CleanUp();

#pragma endregion

    public:

        /****************************************
         *   Constructor & Destructor Methods   *
         ****************************************/
#pragma region Constructor & Destructor Methods

        /// <summary>
        /// GameObjectScriptManager Constructor
        /// </summary>
        GameObjectScriptManager();

        /// <summary>
        /// Default GameObjectScriptManager Destructor
        /// </summary>
        virtual ~GameObjectScriptManager();

        //Delete copy constructor/operator
        GameObjectScriptManager(const GameObjectScriptManager&) = delete;
        GameObjectScriptManager& operator=(const GameObjectScriptManager&) = delete;

#pragma endregion

        /************************
        *   Framework Methods   *
        *************************/
#pragma region Framework Methods

        bool Exists(const std::string& name);

        AEResult Add(const std::string& name, GameObjectScriptInstance* instance);

        AEResult Remove(const std::string& name);

        GameObjectScriptInstance* Get(const std::string& name);

        asIScriptObject* GetScript(const std::string& name);

        GameObjectScriptMapIt begin();

        GameObjectScriptMapIt end();

        GameObjectScriptMapItConst begin() const;

        GameObjectScriptMapItConst end() const;

        GameObjectScriptMapItConst cbegin() const;

        GameObjectScriptMapItConst cend() const;

        GameObjectScriptInstance* operator[](const std::string& name);

#pragma endregion

};

#endif
