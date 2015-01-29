/********************************************************
*
* Author: Carlos Chacón N.
*
* Created: 6/28/2014
*
* Desc:
*
*
* Copyright 2014
*
*********************************************************/

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
#include "Base\Base.h"
#include "Base\XEObject.h"
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
		GameObjectScript(const std::wstring& resourceName, AngelScriptManager* angelScriptManager);

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

		XEResult Load() override;

#pragma endregion

		// Prevent copying.
		GameObjectScript(GameObjectScript const&) = delete;
		GameObjectScript& operator= (GameObjectScript const&) = delete;

};

#endif