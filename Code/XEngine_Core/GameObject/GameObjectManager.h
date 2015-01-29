/********************************************************
*
* Author: Carlos Chacón N.
*
* Desc:
*
*
* Copyright 2012-2014
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
#include "Base\UniqueXEObject.h"

/********************
*   Forward Decls   *
*********************/
class GameObject;

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
		
		GameObjectMap m_GameObjectMap;

#pragma endregion

		/**********************
		*   Private Methods   *
		***********************/
#pragma region Private Methods

		void GameObjectMoveToChild(uint64_t id);

		GameObject* SearchGameObjectChilds(uint64_t goID, GameObject* gameObject) const;

		void CleanUp();

#pragma endregion

	public:

		/***************************************
		*   Constructor & Destructor Methods   *
		****************************************/
#pragma region Constructor & Destructor Methods

		/// <summary>
		/// Default GameObjectManager Constructor
		/// </summary>
		GameObjectManager();

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