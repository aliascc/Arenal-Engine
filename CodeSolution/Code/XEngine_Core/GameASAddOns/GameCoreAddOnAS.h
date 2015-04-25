/********************************************************
*
* Author: Carlos Chacón N.
*
* Copyright 2012-2015
*
*********************************************************/

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
#include "Base\Base.h"
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
		XEResult RegisterObjects(asIScriptEngine* engine);

		/// <summary>
		/// Registers GC Basic Props Object with Angel Script Manager
		/// </summary>
		/// <param name="engine">AS Engine to register with.</param>
		/// <returns>Return Ok if registration was successfully</returns>
		XEResult RegisterGCBasicPropsObject(asIScriptEngine* engine);

		/// <summary>
		/// Registers Game Object Collection Object with Angel Script Manager
		/// </summary>
		/// <param name="engine">AS Engine to register with.</param>
		/// <returns>Return Ok if registration was successfully</returns>
		XEResult RegisterGameComponentCollectionObject(asIScriptEngine* engine);

		/// <summary>
		/// Registers Game Object Collection Object Pointer with Angel Script Manager
		/// </summary>
		/// <param name="engine">AS Engine to register with.</param>
		/// <returns>Return Ok if registration was successfully</returns>
		XEResult RegisterGameComponentCollectionObjectPointer(asIScriptEngine* engine);

		/// <summary>
		/// Registers Game Object Script Interface with Angel Script Manager
		/// </summary>
		/// <param name="engine">AS Engine to register with.</param>
		/// <returns>Return Ok if registration was successfully</returns>
		XEResult RegisterGameObjectScriptInterface(asIScriptEngine* engine);

		/// <summary>
		/// Registers Game Object Script Manager Object with Angel Script Manager
		/// </summary>
		/// <param name="engine">AS Engine to register with.</param>
		/// <returns>Return Ok if registration was successfully</returns>
		XEResult RegisterGameObjectScriptManagerObject(asIScriptEngine* engine);

		/// <summary>
		/// Registers Game Object Script Manager Object with Angel Script Manager
		/// </summary>
		/// <param name="engine">AS Engine to register with.</param>
		/// <returns>Return Ok if registration was successfully</returns>
		XEResult RegisterGameObjectScriptManagerObjectPointer(asIScriptEngine* engine);

		/// <summary>
		/// Registers Game Object Object with Angel Script Manager
		/// </summary>
		/// <param name="engine">AS Engine to register with.</param>
		/// <returns>Return Ok if registration was successfully</returns>
		XEResult RegisterGameObjectObject(asIScriptEngine* engine);

		/// <summary>
		/// Registers Audio Source Game Object Component Object with Angel Script Manager
		/// </summary>
		/// <param name="engine">AS Engine to register with.</param>
		/// <returns>Return Ok if registration was successfully</returns>
		XEResult RegisterAudioSourceGOCObject(asIScriptEngine* engine);

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
		XEResult Register(asIScriptEngine* engine) override;

#pragma endregion

};

#endif