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

#ifndef _GAME_OBJECT_SCRIPT_H
#define _GAME_OBJECT_SCRIPT_H

/**********************
*   System Includes   *
***********************/

/*************************
*   3rd Party Includes   *
**************************/

/***************************
*   Game Engine Includes   *
****************************/
#include "Resource\GameResource.h"
#include "GameObject\Scripts\GameObjectScriptDefs.h"

/********************
*   Forward Decls   *
*********************/
class AngelScriptManager;
struct GameObjectScriptFunctions;

/*****************
*   Class Decl   *
******************/
class GameObjectScript sealed : public GameResource
{
    private:

        /************************
        *   Private Variables   *
        *************************/
#pragma region Private Variables

        AngelScriptManager* m_AngelScriptManager = nullptr;

        GameObjectScriptFunctions* m_GameObjectScriptFunctions = nullptr;

        void CleanUp();

#pragma endregion

        /**********************
        *   Private Methods   *
        ***********************/
#pragma region Private Methods;

#pragma endregion

    public:

        /***************************************
        *   Constructor & Destructor Methods   *
        ****************************************/
#pragma region Constructor & Destructor Methods

        /// <summary>
        /// GameObjectScript Constructor
        /// </summary>
        /// <param name="resourceName">Resource Name</param>
        /// <param name="angelScriptManager">Angel Script Manager</param>
        GameObjectScript(const std::string& resourceName, AngelScriptManager* angelScriptManager);

        /// <summary>
        /// Default GameObjectScript Destructor
        /// </summary>
        virtual ~GameObjectScript();

#pragma endregion

        /******************
        *   Get Methods   *
        *******************/
#pragma region Get Methods

        const GameObjectScriptFunctions* GetGameObjectScriptFunctions() const
        {
            return m_GameObjectScriptFunctions;
        }

#pragma endregion

        /******************
        *   Set Methods   *
        *******************/
#pragma region Set Methods

#pragma endregion

        /************************
        *   Framework Methods   *
        *************************/
#pragma region Framework Methods

        AEResult Load() override;

#pragma endregion

        // Prevent copying.
        GameObjectScript(GameObjectScript const&) = delete;
        GameObjectScript& operator= (GameObjectScript const&) = delete;

};

#endif
