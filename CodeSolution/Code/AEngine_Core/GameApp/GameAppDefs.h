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

/*************************
*   3rd Party Includes   *
**************************/

/***************************
*   Game Engine Includes   *
****************************/
#include "InputDefs.h"
#include "GraphicsDefs.h"

/************
*   Using   *
*************/

/********************
*   Forward Decls   *
*********************/

/**************
*   Defines   *
***************/

#define AE_PROJ_CONFIG_MAIN_NODE            "ConfigProject"
#define AE_PROJ_CONFIG_NODE                 "Config"
#define AE_PROJ_ASSET_MANAGER_NODE          "AssetManager"
#define AE_PROJ_GAME_OBJECT_MANAGER_NODE    "GameObjectManager"
#define AE_PROJ_LOCALIZATION_NODE           "Localization"
#define AE_PROJ_INPUT_NODE                  "Input"
#define AE_PROJ_GRAPHIC_OPTS_NODE           "GraphicOpts"
#define AE_PROJ_CONFIG_NAME_PROP            "name"
#define AE_PROJ_CONFIG_FILE_PROP            "file"

#define AE_ENGINE_CONFIG_MAIN_NODE          "ConfigEngine"
#define AE_ENGINE_LOCALIZATION_NODE         "Localization"
#define AE_ENGINE_GAMEOPTS                  "GameOpts"
#define AE_ENGINE_CONFIG_FILE_PROP          "file"

/****************
*   Constants   *
*****************/

/************
*   Enums   *
*************/

#ifdef AE_EDITOR_MODE

enum class GameEditorPlayState : uint32_t
{
    Playing = 0,
    Paused,
    Stop
};

#endif //AE_EDITOR_MODE

/*************
*   Struct   *
**************/

/// <summary>
/// This structure is use to save the files needed to initialize
/// the Game Engine
/// </summary>
struct GameEngineConfig sealed : public AEObject
{
    /// <summary>
    /// Saves the file path of that will be use to initialize the Game Application Options
    /// </summary>
    std::string m_GameOptsFile = "";

    /// <summary>
    /// Saves the file path of that will be use to initialize the Localization Strings to handle multiple languages
    /// </summary>
    std::string m_LocalizationFile = "";

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
    std::string m_DevCapFile = "";

    /// <summary>
    /// Saves the file path of that will be use to initialize the input devices
    /// </summary>
    std::string m_InputFile = "";

    /// <summary>
    /// Saves the file path of that will be use to initialize the Localization Strings to handle multiple languages
    /// </summary>
    std::string m_LocalizationFile = "";

    /// <summary>
    /// Saves the file path of that will be use to initialize the Game Graphics Option
    /// </summary>
    std::string m_GraphicOptsFile = "";

    /// <summary>
    /// Saves the file path of that will be use to load the Asset Manager
    /// </summary>
    std::string m_AssetManagerFile = "";

    /// <summary>
    /// Saves the file path of that will be use to load the Game Object Manager File
    /// </summary>
    std::string m_GameObjectManagerFile = "";

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
    std::string m_WinCaption = "";
    
    /// <summary>
    /// For Windows OS it stores the HWND Window Class Name.
    /// </summary>
    std::string m_WinClassName = "";
    
    /// <summary>
    /// Lets the Game Application know if it should sample the Frame Rate.
    /// </summary>
    bool m_SampleFPS = false;
    
    /// <summary>    
    /// Lets the Game Application know which at which level it should store logs.
    /// </summary>
    LogLevel m_AELogLvl = LogLevel::None;
    
    /// <summary>
    /// If saving the files to disk, this will store to which file it should be saved.
    /// </summary>
    std::string m_LogFilePath = "";
    
    /// <summary>
    /// Maximum of logs to be kept in memory.
    /// </summary>
    uint32_t m_LogCapacity = AE_LOG_CAPACITY;
    
    /// <summary>
    /// If true. will save logs to disk.
    /// </summary>
    bool m_LogToFile = false;
    
#ifdef AE_EDITOR_MODE

    /// <summary>
    /// If true, a console window will be open with the Game Application .
    /// </summary>
    bool m_ShowConsole = false;

#endif //AE_EDITOR_MODE

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
    std::string m_ProjectName = "";
    
    std::string m_ProjectConfigFile = "";
    std::string m_ProjectLocation = "";
    GameProjectConfig m_GameProjectConfig;
    
    std::string m_EngineConfigFile = "";
    std::string m_EngineLocation = "";
    GameEngineConfig m_GameEngineConfig;

    GameProject();
};

/********************
*   Function Decl   *
*********************/

namespace XEGameAppHelpers
{
    std::string BuildConfigProjectFile(const std::string& projectName);

    std::string BuildAssetFile();

    std::string BuildLocalizationFile();

    std::string BuildGameObjectManagerFile();
}

#endif
