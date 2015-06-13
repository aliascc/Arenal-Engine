/********************************************************
*
* Author: Carlos Chacón N.
*
* Copyright 2012-2015
*
*********************************************************/

#pragma once

#ifndef _SCRIPT_GOC_H
#define _SCRIPT_GOC_H

/**********************
*   System Includes   *
***********************/
#include <functional>

/*************************
*   3rd Party Includes   *
**************************/

/***************************
*   Game Engine Includes   *
****************************/
#include "Base\Base.h"
#include "angelscript.h"
#include "Time\AETimeDefs.h"
#include "GameObject\GameObjectComponent.h"

/********************
*   Forward Decls   *
*********************/
class GameAsset;
class GameObject;
class GameObjectScript;
class AngelScriptManager;
class GameObjectScriptAsset;
class GameObjectScriptManager;
struct GameObjectScriptInstance;
struct GameObjectScriptFunctions;
struct GameObjectScriptProperties;

/*****************
*   Class Decl   *
******************/
class GameObjectScriptGOC sealed : public GameObjectComponent
{
	private:

		/************************
		*   Private Variables   *
		*************************/
#pragma region Private Variables

		AngelScriptManager* m_AngelScriptManager = nullptr;

		GameObjectScriptManager* m_GameObjectScriptManager = nullptr;

		GameObjectScriptInstance* m_GameObjectScriptInstance = nullptr;

		GameObjectScriptProperties* m_GameObjectScriptProperties = nullptr;

		GameObjectScriptProperties* m_TempGameObjectScriptProperties = nullptr;

		asIScriptContext* m_ASContext = nullptr;

		std::wstring m_Name = L"";

		std::wstring m_TempInstanceName = L"";

		/// <summary>
		/// Game Object Pair with Game Object Script that this Game Object Component uses
		/// </summary>
		GameObjectAssetPair<GameObjectScript> m_GameObjectScript;

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

		void CleanUp();

		void GameObjectScriptAssetDeletion(GameAsset* asset);

		void GameObjectScriptAssetReload(GameAsset* asset);

		void GameObjectScriptAssetPreReload(GameAsset* asset);

		AEResult ClearGameObjectScriptAsset(bool informGameAsset = true);

#pragma endregion

	public:

		/***************************************
		*   Constructor & Destructor Methods   *
		****************************************/
#pragma region Constructor & Destructor Methods

		/// <summary>
		/// GameObjectScriptGOC Constructor
		/// </summary>
		/// <param name="gameObject">Game Object that this Component is attached too</param>
		/// <param name="name">Name of the Script GOC.</param>
		/// <param name="angelScriptManager">Angel Script Manager to associate the Scripts with.</param>
		/// <param name="gameObjectScriptManager">Game Object Script Manager to associate the scripts with.</param>
		GameObjectScriptGOC(GameObject* gameObject, const std::wstring& name, AngelScriptManager* angelScriptManager, GameObjectScriptManager* gameObjectScriptManager);

		/// <summary>
		/// Default GameObjectScriptGOC Destructor
		/// </summary>
		virtual ~GameObjectScriptGOC();

#pragma endregion

		/******************
		*   Get Methods   *
		*******************/
#pragma region Get Methods

		inline const std::wstring& GetName() const
		{
			return m_Name;
		}

		std::wstring GetGameObjectScriptName() const;

		std::wstring GetGameObjectScriptInstanceName() const;

		inline bool HasScriptInstance() const
		{
			return (m_GameObjectScriptInstance != nullptr);
		}

		inline GameObjectScriptProperties* GetGameObjectScriptProperties()
		{
			return m_GameObjectScriptProperties;
		}

		inline uint64_t GetGameObjectScriptAssetID() const
		{
			return m_GameObjectScript.m_AssetID;
		}

#pragma endregion

		/******************
		*   Set Methods   *
		*******************/
#pragma region Set Methods

		inline void SetName(const std::wstring& name)
		{
			m_Name = name;
		}

		AEResult SetGameObjectScriptAsset(GameObjectScriptAsset* asset);

#pragma endregion

		/************************
		*   Framework Methods   *
		*************************/
#pragma region Framework Methods

		AEResult CreateGameObjectScriptInstance(const std::wstring& instanceName);

		AEResult RemoveGameObjectScriptInstance();

		AEResult RemoveGameObjectScriptAsset();

		AEResult ExecuteScriptInitialize();

		AEResult ExecuteScriptConstantUpdate(const TimerParams& timerParams);

		AEResult ExecuteScriptUpdate(const TimerParams& timerParams);

		AEResult ExecuteScriptPostUpdate(const TimerParams& timerParams);

#pragma endregion

};

#endif