/** \file 
 * Contains definitions for use of the Game Application
 */

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
#include "Base\AEObject.h"
#include "Math\AEMathDefs.h"
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

#define AE_PROJ_CONFIG_MAIN_NODE            L"ConfigProject"
#define AE_PROJ_CONFIG_NODE                 L"Config"
#define AE_PROJ_ASSET_MANAGER_NODE          L"AssetManager"
#define AE_PROJ_GAME_OBJECT_MANAGER_NODE    L"GameObjectManager"
#define AE_PROJ_LOCALIZATION_NODE           L"Localization"
#define AE_PROJ_INPUT_NODE                  L"Input"
#define AE_PROJ_DEV_CAPS_NODE               L"DevCaps"
#define AE_PROJ_GRAPHIC_OPTS_NODE           L"GraphicOpts"
#define AE_PROJ_CONFIG_NAME_PROP            L"name"
#define AE_PROJ_CONFIG_FILE_PROP            L"file"

#define AE_ENGINE_CONFIG_MAIN_NODE          L"ConfigEngine"
#define AE_ENGINE_LOCALIZATION_NODE         L"Localization"
#define AE_ENGINE_GAMEOPTS                  L"GameOpts"
#define AE_ENGINE_CONFIG_FILE_PROP          L"file"

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
struct GameAppScopedLock sealed : public AEObject
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
struct GameEngineConfig sealed : public AEObject
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
struct GameProjectConfig sealed : public AEObject
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
    /// Saves the file path of that will be use to load the Game Object Manager File
    /// </summary>
    std::wstring m_GameObjectManagerFile = L"";

    /// <summary>
    /// Default GameProjectConfig Constructor
    /// </summary>
    GameProjectConfig();
};

/// <summary>    
/// This structure is use to save the variables needed to initialize
/// the Game Application.
/// </summary>
struct GameAppOpts sealed : public AEObject
{
    /// <summary>
    /// Stores the initial position on the Desktop for the Game Window.
    /// </summary>
    glm::ivec2 m_InitialWinPos = AEMathHelpers::Vec2iZero;
    
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
    LogLevel m_AELogLvl = LogLevel::None;
    
    /// <summary>
    /// If saving the files to disk, this will store to which file it should be saved.
    /// </summary>
    std::wstring m_LogFilePath = L"";
    
    /// <summary>
    /// Maximum of logs to be kept in memory.
    /// </summary>
    uint32_t m_LogCapacity = AE_LOG_CAPACITY;
    
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
struct GameProject sealed : public AEObject
{
    std::wstring m_ProjectName = L"";
    
    std::wstring m_ProjectConfigFile = L"";
    std::wstring m_ProjectLocation = L"";
    GameProjectConfig m_GameProjectConfig;
    
    std::wstring m_EngineConfigFile = L"";
    std::wstring m_EngineLocation = L"";
    GameEngineConfig m_GameEngineConfig;

    GameProject();
};

/********************
*   Function Decl   *
*********************/

namespace XEGameAppHelpers
{
    std::wstring BuildConfigProjectFile(const std::wstring& projectName);

    std::wstring BuildAssetFile();

    std::wstring BuildLocalizationFile();

    std::wstring BuildGameObjectManagerFile();
}

#endif
