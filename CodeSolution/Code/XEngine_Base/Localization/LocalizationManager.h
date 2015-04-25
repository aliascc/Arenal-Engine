/********************************************************
*
* Author: Carlos Chacón N.
*
* Copyright 2012-2015
*
*********************************************************/

#pragma once

#ifndef _LOCALIZATION_MANAGER_H
#define _LOCALIZATION_MANAGER_H

/**********************
*   System Includes   *
***********************/
#include <map>
#include <mutex>
#include <string>
#include <stdint.h>

/*************************
*   3rd Party Includes   *
**************************/

/***************************
*   Game Engine Includes   *
****************************/
#include "Base\Base.h"
#include "Utils\Singleton.h"
#include "LocalizationManagerDefs.h"

/********************
*   Forward Decls   *
*********************/

/*****************
*   Class Decl   *
******************/

/// <summary>
/// Manages the available messages in different languages
/// </summary>
class LocalizationManager sealed : public Singleton<LocalizationManager>
{
	friend class Singleton<LocalizationManager>;

	private:

		/************************
		*   Private Variables   *
		*************************/
#pragma region Private Variables

		/// <summary>
		/// Contains the the Languages and the messages
		/// </summary>
		LanguageMap m_LanguagesMap;

		/// <summary>
		/// Contains all of the messages in the current specified language
		/// </summary>
		LiteralMap m_CurrentLiteralMap;

		/// <summary>
		/// Default Language
		/// </summary>
		std::wstring m_DefaultLanguage = L"";

		/// <summary>
		/// File name where the Languages and their messages are
		/// </summary>
		std::wstring m_Filename = L"";

		/// <summary>
		/// Defines if Localization Manager is ready to run
		/// </summary>
		bool m_IsReady = false;

		/// <summary>
		/// Variable to let logger know it has been called by Localization Manager.
		/// Set this variable to true when calling the Logger, this will avoid 
		/// Logger calling back Localization Manager and causing a deadlock
		/// </summary>
		bool m_CallByLocManager = false;

		/// <summary>
		/// Mutex for Localization Manager to be thread safe
		/// </summary>
		std::mutex m_LocalizationMutex;

		/// <summary>
		/// Project Directory
		/// </summary>
		std::wstring m_ProjectDirectory = L"";

#pragma endregion

		/**********************
		*   Private Methods   *
		***********************/
#pragma region Private Variables
		
		/// <summary>
		/// Loads and specific Language from a file, its literals and messages
		/// </summary>
		/// <param name="name">Name of the Language</param>
		/// <returns>Returns XEResult::OK if Languages was loaded successfully, if not an appropriate error code</returns>
		XEResult LoadLanguageLiterals(const std::wstring& name, const std::wstring& file);
		
		/// <summary>
		/// Clears all information from all of the Languages
		/// </summary>
		void ClearAllMaps();

		/// <summary>
		/// Reloads all of the information for Localization Manager, does not lock the thread
		/// </summary>
		/// <returns>Returns XEResult::OK if Languages was loaded successfully, if not an appropriate error code</returns>
		XEResult ReloadWithoutLock();

		/// <summary>
		/// Sets the current default, does not lock the thread
		/// </summary>
		/// <param name='languageName'>Language to set</param>
		/// <returns>Returns XEResult::OK if Languages was set successfully, if not an appropriate error code</returns>
		XEResult SetDefaultLanguageWithoutLock(const std::wstring& languageName);

#pragma endregion

	public:

		/***************************************
		*   Constructor & Destructor Methods   *
		****************************************/
#pragma region Constructor & Destructor Methods

		/// <summary>
		/// LocalizationManager Constructor
		/// </summary>
		LocalizationManager();

		/// <summary>
		/// Default LocalizationManager Destructor
		/// </summary>
		virtual ~LocalizationManager();

#pragma endregion
		
		/*******************
		 *   Get Methods   *
		 *******************/
#pragma region Get Methods

		/// <summary>
		/// Gets the if Logger has been called by this Manager
		/// </summary>
		/// <returns>True if Logger is been called by this manager</returns>
		inline bool GetCallByLocManager() const
		{
			return m_CallByLocManager;
		}

		/// <summary>
		/// Gets the Current Default Language
		/// </summary>
		/// <returns>Returns XEResult::OK if Languages was loaded successfully, if not an appropriate error code</returns>
		const std::wstring& GetDefaultLanguage() const;

#pragma endregion

		/*************************
		 *   Framework Methods   *
		 *************************/
#pragma region Framework Methods

		/// <summary>
		/// Initializes the Localization Manager
		/// </summary>
		/// <param name='file'>File from where to load the Localization Information</param>
		/// <returns>Returns XEResult::OK if Languages was loaded successfully, if not an appropriate error code</returns>
		XEResult Initialize(const std::wstring& file, const std::wstring& projectDir);

		/// <summary>
		/// Reloads all of the information for Localization Manager
		/// </summary>
		/// <returns>Returns XEResult::OK if Languages was loaded successfully, if not an appropriate error code</returns>
		XEResult Reload();

		/// <summary>
		/// Gets a Literal
		/// </summary>
		/// <param name='literalName'>Name of the literal to get</param>
		/// <returns>Returns Literal if found, if not, just returns XE_LOC_LANG_DEFAULT_LITERAL</returns>
		const std::wstring& GetLiteral(const std::wstring& literalName);

		/// <summary>
		/// Sets the current default
		/// </summary>
		/// <param name='languageName'>Language to set</param>
		/// <returns>Returns XEResult::OK if Languages was set successfully, if not an appropriate error code</returns>
		XEResult SetDefaultLanguage(const std::wstring& languageName);

#pragma endregion
};

#endif