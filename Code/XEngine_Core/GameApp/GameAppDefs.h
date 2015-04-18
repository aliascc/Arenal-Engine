/** \file 
 * Contains definitions for use of the Game Application
 */

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

#ifndef _GAME_APP_DEFS_H
#define _GAME_APP_DEFS_H

/**********************
*   System Includes   *
***********************/
#include <mutex>
#include <thread>
#include <string>

/*************************
*   3rd Party Includes   *
**************************/
#include "glm\glm.hpp"

/***************************
*   Game Engine Includes   *
****************************/
#include "InputDefs.h"
#include "GraphicsDefs.h"
#include "Base\XEObject.h"
#include "Math\XEMathDefs.h"
#include "Logger\LoggerDefs.h"

/************
*   Using   *
*************/

/********************
*   Forward Decls   *
*********************/

/**************
*   Defines   *
***************/

#define XE_PROJ_DATA_DIR				L"Data"

#define XE_PROJ_ASSETS_DIR				L"Assets"

#define XE_PROJ_INPUT_DIR				L"Input"

#define XE_PROJ_GLOBAL_DIR				L"Global"

#define XE_PROJ_GAME_OBJECTS_DIR		L"Game Objects"

#define XE_PROJ_ASSET_MANAGER_DIR		L"Asset Manager"

#define XE_PROJ_ASSET_MANAGER_FILE		L"AssetManager.xml"

#define XE_PROJ_CONFIG_ENGINE_FILE		L"ConfigEngine.xml"

#define XE_PROJ_CONFIG_PROJ_FILE		L"ConfigProject.xml"

#define XE_PROJ_INPUT_DIR_LOC			(XE_PROJ_DATA_DIR L"/" XE_PROJ_INPUT_DIR)

#define XE_PROJ_ASSETS_DIR_LOC			(XE_PROJ_DATA_DIR L"/" XE_PROJ_ASSETS_DIR)

#define XE_PROJ_GLOBAL_DIR_LOC			(XE_PROJ_DATA_DIR L"/" XE_PROJ_GLOBAL_DIR)

#define XE_PROJ_GAME_OBJECTS_DIR_LOC	(XE_PROJ_DATA_DIR L"/" XE_PROJ_GAME_OBJECTS_DIR)

#define XE_PROJ_ASSET_MANAGER_DIR_LOC	(XE_PROJ_DATA_DIR L"/" XE_PROJ_ASSET_MANAGER_DIR)

#define XE_PROJ_ASSET_MANAGER_FILE_LOC	(XE_PROJ_DATA_DIR L"/" XE_PROJ_ASSET_MANAGER_DIR L"/" XE_PROJ_ASSET_MANAGER_FILE)

/****************
*   Constants   *
*****************/

/************
*   Enums   *
*************/

enum class GameEditorPlayState : uint32_t
{
	Playing = 0,
	Paused,
	Stop
};

enum class GameAppRunOpt : uint32_t
{
	GameMode = 0,
	EditorMode
};

/*************
*   Struct   *
**************/

/// <summary>
/// This structure is use to lock the Game App Loop with the Mutex from Game App
/// </summary>
struct GameAppScopedLock sealed : public XEObject
{
	private:
		/// <summary>
		/// Mutex Pointer for Game App Mutex
		/// </summary>
		std::mutex* m_GameAppMutexPtr = nullptr;

		/// <summary>
		/// Marks if mutex was locked
		/// </summary>
		bool m_IsLock = false;

	public:
		/// <summary>
		/// GameAppScopedLock Constructor
		/// </summary>
		/// <param name="gameAppMutexPtr">Pointer to Game App Mutex</param>
		GameAppScopedLock(std::mutex* gameAppMutexPtr);
		~GameAppScopedLock();

		/// <summary>
		/// Locks the mutex for the duration of this object's life
		/// or until EndLock is called
		/// </summary>
		void StartLock();

		/// <summary>
		/// Unlocks the mutex
		/// </summary>
		void EndLock();
};

/// <summary>
/// This structure is use to save the files needed to initialize
/// the Game Engine
/// </summary>
struct GameEngineConfig sealed : public XEObject
{
	/// <summary>
	/// Saves the file path of that will be use to initialize the Game Application Options
	/// </summary>
	std::wstring m_GameOptsFile = L"";

	/// <summary>
	/// Saves the file path of that will be use to initialize the Localization Strings to handle multiple languages
	/// </summary>
	std::wstring m_LocalizationFile = L"";

	/// <summary>
	/// Default GameEngineConfig Constructor
	/// </summary>
	GameEngineConfig();
};

/// <summary>
/// This structure is use to save the files needed to initialize
/// the Game Project
/// </summary>
struct GameProjectConfig sealed : public XEObject
{
	/// <summary>
	/// Saves the file path of that will be use to test certain the device capabilities
	/// </summary>
	std::wstring m_DevCapFile = L"";

	/// <summary>
	/// Saves the file path of that will be use to initialize the input devices
	/// </summary>
	std::wstring m_InputFile = L"";

	/// <summary>
	/// Saves the file path of that will be use to initialize the Localization Strings to handle multiple languages
	/// </summary>
	std::wstring m_LocalizationFile = L"";

	/// <summary>
	/// Saves the file path of that will be use to initialize the Game Graphics Option
	/// </summary>
	std::wstring m_GraphicOptsFile = L"";

	/// <summary>
	/// Saves the file path of that will be use to load the Asset Manager
	/// </summary>
	std::wstring m_AssetManagerFile = L"";

	/// <summary>
	/// Default GameProjectConfig Constructor
	/// </summary>
	GameProjectConfig();
};

/// <summary>	
/// This structure is use to save the variables needed to initialize
/// the Game Application.
/// </summary>
struct GameAppOpts sealed : public XEObject
{
	/// <summary>
	/// Stores the initial position on the Desktop for the Game Window.
	/// </summary>
	glm::ivec2 m_InitialWinPos = XEMathHelpers::Vec2iZero;
	
	/// <summary>
	/// Stores the Window Title.
	/// </summary>
	std::wstring m_WinCaption = L"";
	
	/// <summary>
	/// For Windows OS it stores the HWND Window Class Name.
	/// </summary>
	std::wstring m_WinClassName = L"";
	
	/// <summary>
	/// Lets the Game Application know if it should sample the Frame Rate.
	/// </summary>
	bool m_SampleFPS = false;
	
	/// <summary>	
	/// If true, a console window will be open with the Game Application .
	/// </summary>
	/// <remarks>
	/// This should only be use for debugging the Game.
	/// </remarks>
	bool m_ShowConsole = false;
	
	/// <summary>	
	/// Lets the Game Application know which at which level it should store logs.
	/// </summary>
	LogLevel m_XELogLvl = LogLevel::None;
	
	/// <summary>
	/// If saving the files to disk, this will store to which file it should be saved.
	/// </summary>
	std::wstring m_LogFilePath = XE_LOG_DEFAULT_PATH;
	
	/// <summary>
	/// Maximum of logs to be kept in memory.
	/// </summary>
	uint32_t m_LogCapacity = XE_LOG_CAPACITY;
	
	/// <summary>
	/// If true. will save logs to disk.
	/// </summary>
	bool m_LogToFile = false;
	
	/// <summary>
	/// Default GameConfigInput Constructor
	/// </summary>
	GameAppOpts();
};

/// <summary>	
/// This structure is use to save the variables needed to for the Game Project
/// </summary>
struct GameProject sealed : public XEObject
{
	std::wstring m_ProjectName = L"";
	std::wstring m_ProjectConfigFile = L"";
	std::wstring m_ProjectLocation = L"";
	GameProjectConfig m_GameProjectConfig;

	GameProject();
};

/********************
*   Function Decl   *
*********************/

namespace XEGameAppHelpers
{
	std::wstring BuildConfigProjectFile(const std::wstring& projectName);

	std::wstring BuildAssetFile();
}

#endif