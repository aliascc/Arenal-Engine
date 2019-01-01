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

#ifndef _GAME_CORE_ADD_ON_AS_H
#define _GAME_CORE_ADD_ON_AS_H

/**********************
*   System Includes   *
***********************/

/*************************
*   3rd Party Includes   *
**************************/

/***************************
*   Game Engine Includes   *
****************************/
#include "AngelScript\Add-ons\AngelScriptAddOn.h"

/************
*   Using   *
*************/

/********************
*   Forward Decls   *
*********************/
class GameObjectScriptManager;
class GameComponentCollection;

/*****************
*   Class Decl   *
******************/

class GameCoreAddOnAS sealed : public AngelScriptAddOn
{
    private:

        /************************
        *   Private Variables   *
        *************************/
#pragma region Private Variables

        /// <summary>
        /// Game Component Collection Object for current Game App
        /// </summary>
        GameComponentCollection* m_GameComponentCollection = nullptr;

        /// <summary>
        /// Game Object Script Manager Object for current Game App
        /// </summary>
        GameObjectScriptManager* m_GameObjectScriptManager = nullptr;

#pragma endregion

        /**********************
        *   Private Methods   *
        ***********************/
#pragma region Private Methods

        /// <summary>
        /// Registers Objects with Angel Script Manager
        /// </summary>
        /// <param name="engine">AS Engine to register with.</param>
        /// <returns>Return Ok if registration was successfully</returns>
        AEResult RegisterObjects(asIScriptEngine* engine);

        /// <summary>
        /// Registers GC Basic Props Object with Angel Script Manager
        /// </summary>
        /// <param name="engine">AS Engine to register with.</param>
        /// <returns>Return Ok if registration was successfully</returns>
        AEResult RegisterGCBasicPropsObject(asIScriptEngine* engine);

        /// <summary>
        /// Registers Game Object Collection Object with Angel Script Manager
        /// </summary>
        /// <param name="engine">AS Engine to register with.</param>
        /// <returns>Return Ok if registration was successfully</returns>
        AEResult RegisterGameComponentCollectionObject(asIScriptEngine* engine);

        /// <summary>
        /// Registers Game Object Collection Object Pointer with Angel Script Manager
        /// </summary>
        /// <param name="engine">AS Engine to register with.</param>
        /// <returns>Return Ok if registration was successfully</returns>
        AEResult RegisterGameComponentCollectionObjectPointer(asIScriptEngine* engine);

        /// <summary>
        /// Registers Game Object Script Interface with Angel Script Manager
        /// </summary>
        /// <param name="engine">AS Engine to register with.</param>
        /// <returns>Return Ok if registration was successfully</returns>
        AEResult RegisterGameObjectScriptInterface(asIScriptEngine* engine);

        /// <summary>
        /// Registers Game Object Script Manager Object with Angel Script Manager
        /// </summary>
        /// <param name="engine">AS Engine to register with.</param>
        /// <returns>Return Ok if registration was successfully</returns>
        AEResult RegisterGameObjectScriptManagerObject(asIScriptEngine* engine);

        /// <summary>
        /// Registers Game Object Script Manager Object with Angel Script Manager
        /// </summary>
        /// <param name="engine">AS Engine to register with.</param>
        /// <returns>Return Ok if registration was successfully</returns>
        AEResult RegisterGameObjectScriptManagerObjectPointer(asIScriptEngine* engine);

        /// <summary>
        /// Registers Game Object Object with Angel Script Manager
        /// </summary>
        /// <param name="engine">AS Engine to register with.</param>
        /// <returns>Return Ok if registration was successfully</returns>
        AEResult RegisterGameObjectObject(asIScriptEngine* engine);

        /// <summary>
        /// Registers Audio Source Game Object Component Object with Angel Script Manager
        /// </summary>
        /// <param name="engine">AS Engine to register with.</param>
        /// <returns>Return Ok if registration was successfully</returns>
        AEResult RegisterAudioSourceGOCObject(asIScriptEngine* engine);

#pragma endregion

    public:

        /***************************************
        *   Constructor & Destructor Methods   *
        ****************************************/
#pragma region Constructor & Destructor Methods

        /// <summary>
        /// GameCoreAddOnAS Constructor
        /// </summary>
        /// <param name="gameComponentCollection">Game Component Collection Object for current Game App.</param>
        /// <param name="gameObjectScriptManager">Game Object Script Manager Object for current Game App.</param>
        GameCoreAddOnAS(GameComponentCollection* gameComponentCollection, GameObjectScriptManager* gameObjectScriptManager);

        /// <summary>
        /// Default GameCoreAddOnAS Destructor
        /// </summary>
        virtual ~GameCoreAddOnAS();

#pragma endregion

        /******************
        *   Get Methods   *
        *******************/
#pragma region Get Methods

#pragma endregion

        /************************
        *   Framework Methods   *
        *************************/
#pragma region Framework Methods

        /// <summary>
        /// Registers the Add-on with the AS Engine.
        /// </summary>
        /// <param name="engine">AS Engine to register with.</param>
        AEResult Register(asIScriptEngine* engine) override;

#pragma endregion

};

#endif
