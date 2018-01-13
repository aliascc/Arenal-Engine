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

#ifndef _LOCALIZATION_MANAGER_H
#define _LOCALIZATION_MANAGER_H

/**********************
*   System Includes   *
***********************/
#include <map>
#include <set>
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

		typedef std::set<std::wstring> LiteralsSet;

		typedef std::map<std::wstring, LiteralsSet> ExtendedLiteralsMap;

		/************************
		*   Private Variables   *
		*************************/
#pragma region Private Variables

		/// <summary>
		/// Extended Languages Literals
		/// </summary>
		ExtendedLiteralsMap m_ExtendedLiteralsMap;

		/// <summary>
		/// Contains the Languages and the messages
		/// </summary>
		LanguageMap m_LanguagesMap;

		/// <summary>
		/// Contains all of the messages in the current specified language
		/// </summary>
		LiteralMap* m_CurrentLiteralMap = nullptr;

		/// <summary>
		/// Default Language
		/// </summary>
		std::wstring m_DefaultLanguage = L"";

		/// <summary>
		/// Engine File name where the Languages and their messages are
		/// </summary>
		std::wstring m_FilenameEngine = L"";

		/// <summary>
		/// Project File name where the Languages and their messages are
		/// </summary>
		std::wstring m_FilenameProject = L"";

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

#pragma endregion

		/**********************
		*   Private Methods   *
		***********************/
#pragma region Private Variables

		/// <summary>
		/// Loads and specific Language from a file, its literals and messages
		/// </summary>
		/// <param name="name">Name of the Language</param>
		/// <param name="file">File from where to load</param>
		/// <returns>Returns AEResult::OK if Languages was loaded successfully, if not an appropriate error code</returns>
		AEResult LoadLanguageLiterals(const std::wstring& name, const std::wstring& file);

		/// <summary>
		/// Loads and specific Extended Languages from a file, its literals and messages
		/// </summary>
		/// <param name="name">Name of the Language</param>
		/// <param name="file">File from where to load</param>
		/// <returns>Returns AEResult::OK if Languages was loaded successfully, if not an appropriate error code</returns>
		AEResult LoadExtendedLanguageLiterals(const std::wstring& name, const std::wstring& file);

		/// <summary>
		/// Clears all information from all of the Languages
		/// </summary>
		void ClearAllMaps();

		/// <summary>
		/// Clears Project information
		/// </summary>
		void CleanProjectInfo();

		/// <summary>
		/// Reloads all of the information for Localization Manager, does not lock the thread
		/// </summary>
		/// <param name='enginePath'>Path to look for the language files for the engine localization file</param> 
		/// <param name='projectPath'>Path to look for the language files for the project localization file</param> 
		/// <returns>Returns AEResult::OK if Languages was loaded successfully, if not an appropriate error code</returns>
		AEResult ReloadAllWithoutLock(const std::wstring& enginePath, const std::wstring& projectPath);

		/// <summary>
		/// Reloads Engine information for Localization Manager, does not lock the thread
		/// </summary>
		/// <param name='enginePath'>Path to look for the language files</param> 
		/// <returns>Returns AEResult::OK if Languages was loaded successfully, if not an appropriate error code</returns>
		AEResult ReloadEngineWithoutLock(const std::wstring& enginePath);

		/// <summary>
		/// Reloads Project information for Localization Manager, does not lock the thread
		/// </summary>
		/// <param name='projectPath'>Path to look for the language files</param> 
		/// <returns>Returns AEResult::OK if Languages was loaded successfully, if not an appropriate error code</returns>
		AEResult ReloadProjectWithoutLock(const std::wstring& projectPath);

		/// <summary>
		/// Sets the current default, does not lock the thread
		/// </summary>
		/// <param name='languageName'>Language to set</param>
		/// <returns>Returns AEResult::OK if Languages was set successfully, if not an appropriate error code</returns>
		AEResult SetDefaultLanguageWithoutLock(const std::wstring& languageName);

		/// <summary>
		/// Save Extended Literals from a language to an XML
		/// </summary>
		/// <param name='filename'>Filename to extend</param>
		/// <param name='literalSet'>Extended Literal Set</param>
		/// <param name='languageName'>Language of Literals</param>
		/// <returns>Returns AEResult::OK if it was successful</returns>
		AEResult SaveToXMLExtentedLiteral(const std::wstring& filename, const LiteralsSet& literalSet, const std::wstring& languageName) const;

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
		
		/******************
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
		/// <returns>Returns AEResult::OK if Languages was loaded successfully, if not an appropriate error code</returns>
		const std::wstring& GetDefaultLanguage() const;

#pragma endregion

		/************************
		*   Framework Methods   *
		*************************/
#pragma region Framework Methods

		/// <summary>
		/// Initializes the Localization Manager
		/// </summary>
		/// <param name='file'>File from where to load the Localization Information</param> 
		/// <param name='enginePath'>Path to look for the language files</param> 
		/// <returns>Returns AEResult::OK if Languages was loaded successfully, if not an appropriate error code</returns>
		AEResult Initialize(const std::wstring& file, const std::wstring& enginePath);

		/// <summary>
		/// Reloads all of the information for Localization Manager
		/// </summary>
		/// <param name='enginePath'>Path to look for the language files for the engine localization file</param> 
		/// <param name='projectPath'>Path to look for the language files for the project localization file</param> 
		/// <returns>Returns AEResult::OK if Languages was loaded successfully, if not an appropriate error code</returns>
		AEResult ReloadAll(const std::wstring& enginePath, const std::wstring& projectPath);

		/// <summary>
		/// Gets a Literal
		/// </summary>
		/// <param name='literalName'>Name of the literal to get</param>
		/// <returns>Returns Literal if found, if not, just returns AE_LOC_LANG_DEFAULT_LITERAL</returns>
		const std::wstring& GetLiteral(const std::wstring& literalName);

		/// <summary>
		/// Sets the current default
		/// </summary>
		/// <param name='languageName'>Language to set</param>
		/// <returns>Returns AEResult::OK if Languages was set successfully, if not an appropriate error code</returns>
		AEResult SetDefaultLanguage(const std::wstring& languageName);

		/// <summary>
		/// Load Project Localization
		/// </summary>
		/// <param name='filename'>Localization Project Filename</param>
		/// <param name='projectPath'>Path to look for the language files for the project localization file</param> 
		/// <returns>Returns AEResult::OK if it was successful</returns>
		AEResult LoadProjectFile(const std::wstring& filename, const std::wstring& projectPath);

		/// <summary>
		/// Add a Language to Extend
		/// </summary>
		/// <param name='languageName'>Language to Extend</param>
		/// <returns>Returns AEResult::OK if it was successful</returns>
		AEResult AddExtendedLanguage(const std::wstring& languageName);

		/// <summary>
		/// Add Extended Literal
		/// </summary>
		/// <param name='languageName'>Language of the Literal</param>
		/// <param name='literalName'>Literal Name</param>
		/// <param name='literalMessage'>Literal Message</param>
		/// <returns>Returns AEResult::OK if it was successful</returns>
		AEResult AddExtendedLiteral(const std::wstring& languageName, const std::wstring& literalName, const std::wstring& literalMessage);

		/// <summary>
		/// Remove Extended Language
		/// </summary>
		/// <param name='languageName'>Extended Language</param>
		/// <returns>Returns AEResult::OK if it was successful</returns>
		AEResult RemoveExtendedLanguage(const std::wstring& languageName);

		/// <summary>
		/// Remove Extended Literal
		/// </summary>
		/// <param name='languageName'>Extended Language</param>
		/// <param name='literalName'>Literal Name</param>
		/// <returns>Returns AEResult::OK if it was successful</returns>
		AEResult RemoveExtendedLiteral(const std::wstring& languageName, const std::wstring& literalName);

		/// <summary>
		/// Save All Extended Literals to XML
		/// </summary>
		/// <param name='filename'>Filename to extend</param>
		/// <param name='projectDir'>Project Directory</param>
		/// <returns>Returns AEResult::OK if it was successful</returns>
		AEResult SaveToXML(const std::wstring& filename, const std::wstring& projectDir) const;

#pragma endregion

};

#endif