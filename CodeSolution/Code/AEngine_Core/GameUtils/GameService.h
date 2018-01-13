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

#ifndef _GAME_SERVICE_H
#define _GAME_SERVICE_H

/**********************
*   System Includes   *
***********************/
#include <string>
#include <stdint.h>

/*************************
*   3rd Party Includes   *
**************************/

/***************************
*   Game Engine Includes   *
****************************/
#include "GameUtilsDefs.h"
#include "Base\UniqueAEObjectNamed.h"

/********************
*   Forward Decls   *
*********************/

/*****************
*   Class Decl   *
******************/
class GameService abstract : public UniqueAEObjectNamed
{
	private:

		/************************
		 *   Private Variables   *
		 *************************/
#pragma region Private Variables

#pragma endregion

	public:

		/****************************************
		 *   Constructor & Destructor Methods   *
		 ****************************************/
#pragma region Constructor & Destructor Methods

		///<summary>
		///GameService Constructor
		///</summary>
		GameService(const std::wstring& name = L"");

		///<summary>
		///Default GameService Destructor
		///</summary>
		virtual ~GameService();

#pragma endregion

		/*******************
		 *   Get Methods   *
		 *******************/
#pragma region Get Methods

#pragma endregion

		/*******************
		 *   Set Methods   *
		 *******************/
#pragma region Set Methods

#pragma endregion

		/*************************
		 *   Framework Methods   *
		 *************************/
#pragma region Framework Methods

#pragma endregion

};

#endif