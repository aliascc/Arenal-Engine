/********************************************************
*
* Author: Carlos Chacón N.
*
* Copyright 2012-2015
*
*********************************************************/

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
#include "Base\UniqueXEObjectNamed.h"

/********************
*   Forward Decls   *
*********************/

/*****************
*   Class Decl   *
******************/
class GameService abstract : public UniqueXEObjectNamed
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