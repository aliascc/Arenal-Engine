/********************************************************
*
* Author: Carlos Chac�n N.
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
		/// <returns>Returns XEResult::OK if Languages was loaded successfully, if not an appropriate error code</returns>
		XEResult LoadLanguageLiterals(const std::wstring& name, const std::wstring& file);

		/// <summary>
		/// Loads and specific Extended Languages from a file, its literals and messages
		/// </summary>
		/// <param name="name">Name of the Language</param>
		/// <param name="file">File from where to load</param>
		/// <returns>Returns XEResult::OK if Languages was loaded successfully, if not an appropriate error code</returns>
		XEResult LoadExtendedLanguageLiterals(const std::wstring& name, const std::wstring& file);

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
		/// <returns>Returns XEResult::OK if Languages was loaded successfully, if not an appropriate error code</returns>
		XEResult ReloadAllWithoutLock();

		/// <summary>
		/// Reloads Engine information for Localization Manager, does not lock the thread
		/// </summary>
		/// <returns>Returns XEResult::OK if Languages was loaded successfully, if not an appropriate error code</returns>
		XEResult ReloadEngineWithoutLock();

		/// <summary>
		/// Reloads Project information for Localization Manager, does not lock the thread
		/// </summary>
		/// <returns>Returns XEResult::OK if Languages was loaded successfully, if not an appropriate error code</returns>
		XEResult ReloadProjectWithoutLock();

		/// <summary>
		/// Sets the current default, does not lock the thread
		/// </summary>
		/// <param name='languageName'>Language to set</param>
		/// <returns>Returns XEResult::OK if Languages was set successfully, if not an appropriate error code</returns>
		XEResult SetDefaultLanguageWithoutLock(const std::wstring& languageName);

		/// <summary>
		/// Save Extended Literals from a language to an XML
		/// </summary>
		/// <param name='filename'>Filename to extend</param>
		/// <param name='literalSet'>Extended Literal Set</param>
		/// <param name='languageName'>Language of Literals</param>
		/// <returns>Returns XEResult::OK if it was successful</returns>
		XEResult SaveToXMLExtentedLiteral(const std::wstring& filename, const LiteralsSet& literalSet, const std::wstring& languageName);

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
		/// <returns>Returns XEResult::OK if Languages was loaded successfully, if not an appropriate error code</returns>
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
		/// <returns>Returns XEResult::OK if Languages was loaded successfully, if not an appropriate error code</returns>
		XEResult Initialize(const std::wstring& file);

		/// <summary>
		/// Reloads all of the information for Localization Manager
		/// </summary>
		/// <returns>Returns XEResult::OK if Languages was loaded successfully, if not an appropriate error code</returns>
		XEResult ReloadAll();

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

		/// <summary>
		/// Load Project Localization
		/// </summary>
		/// <param name='filename'>Localization Project Filename</param>
		/// <returns>Returns XEResult::OK if it was successful</returns>
		XEResult LoadProjectFile(const std::wstring& filename);

		/// <summary>
		/// Add a Language to Extend
		/// </summary>
		/// <param name='languageName'>Language to Extend</param>
		/// <returns>Returns XEResult::OK if it was successful</returns>
		XEResult AddExtendedLanguage(const std::wstring& languageName);

		/// <summary>
		/// Add Extended Literal
		/// </summary>
		/// <param name='languageName'>Language of the Literal</param>
		/// <param name='literalName'>Literal Name</param>
		/// <param name='literalMessage'>Literal Message</param>
		/// <returns>Returns XEResult::OK if it was successful</returns>
		XEResult AddExtendedLiteral(const std::wstring& languageName, const std::wstring& literalName, const std::wstring& literalMessage);

		/// <summary>
		/// Remove Extended Language
		/// </summary>
		/// <param name='languageName'>Extended Language</param>
		/// <returns>Returns XEResult::OK if it was successful</returns>
		XEResult RemoveExtendedLanguage(const std::wstring& languageName);

		/// <summary>
		/// Remove Extended Literal
		/// </summary>
		/// <param name='languageName'>Extended Language</param>
		/// <param name='literalName'>Literal Name</param>
		/// <returns>Returns XEResult::OK if it was successful</returns>
		XEResult RemoveExtendedLiteral(const std::wstring& languageName, const std::wstring& literalName);

		/// <summary>
		/// Save All Extended Literals to XML
		/// </summary>
		/// <param name='filename'>Filename to extend</param>
		/// <param name='projectDir'>Project Directory</param>
		/// <returns>Returns XEResult::OK if it was successful</returns>
		XEResult SaveToXML(const std::wstring& filename, const std::wstring& projectDir);

#pragma endregion

};

#endif