/********************************************************
*
* Author: Carlos Chacón N.
*
* Copyright 2012-2015
*
*********************************************************/

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
#include "Base\XEObject.h"

/********************
*   Forward Decls   *
*********************/
class Logger;
class GameService;
class LocalizationManager;

/*****************
*   Class Decl   *
******************/
class GameServiceCollection : public XEObject
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

		XEResult Add(const std::wstring& name, GameService* gameService);

		XEResult Remove(const std::wstring& name);

		inline GameService* operator[] (const std::wstring& name)
		{
			return GetGameService(name);
		}

#pragma endregion

};

#endif