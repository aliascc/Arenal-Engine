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

#ifndef _GAME_SERVICE_COLLECTION_H
#define _GAME_SERVICE_COLLECTION_H

/**********************
*   System Includes   *
***********************/
#include <string>
#include <stdint.h>
#include <unordered_map>

/*************************
*   3rd Party Includes   *
**************************/

/***************************
*   Game Engine Includes   *
****************************/
#include "Base\AEObject.h"

/********************
*   Forward Decls   *
*********************/
class Logger;
class GameService;
class LocalizationManager;

/*****************
*   Class Decl   *
******************/
class GameServiceCollection : public AEObject
{
	//Typedefs to handle Map Collection
	typedef std::unordered_map<std::wstring, GameService*> GameServiceMap;

	private:

		/************************
		*   Private Variables   *
		*************************/
#pragma region Private Variables

		/// <summary>
		/// Map of Game Services
		/// </summary>
		GameServiceMap m_GameServiceMap;

#pragma endregion

		/***********************
		 *   Private Methods   *
		 ***********************/
#pragma region Private Methods

#pragma endregion

	public:

		/****************************************
		 *   Constructor & Destructor Methods   *
		 ****************************************/
#pragma region Constructor & Destructor Methods

		/// <summary>
		/// GameServiceCollection Constructor
		/// </summary>
		GameServiceCollection();

		/// <summary>
		/// Default GameServiceCollection Destructor
		/// </summary>
		virtual ~GameServiceCollection();

#pragma endregion

		/*******************
		 *   Get Methods   *
		 *******************/
#pragma region Get Methods

		GameService* GetGameService(const std::wstring& name);

#pragma endregion

		/*************************
		 *   Framework Methods   *
		 *************************/
#pragma region Framework Methods

		bool Exists(const std::wstring& name) const;

		AEResult Add(const std::wstring& name, GameService* gameService);

		AEResult Remove(const std::wstring& name);

		inline GameService* operator[] (const std::wstring& name)
		{
			return GetGameService(name);
		}

#pragma endregion

};

#endif