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

#ifndef _BASE_LOCATIONS_H
#define _BASE_LOCATIONS_H

/**********************
*   System Includes   *
***********************/

/*************************
*   3rd Party Includes   *
**************************/

/***************************
*   Game Engine Includes   *
****************************/

/************
*   Using   *
*************/

/**************
*   Defines   *
***************/

/// <summary>
/// Jump from Engine Binary Directory to Data
/// </summary>
#define AE_ENGINE_BIN_TO_DATA_PATH_PREFIX       "../"

/// <summary>
/// Jump from Engine Binary Directory to Data
/// </summary>
#define AE_ENGINE_BIN_TO_DATA_PATH_ADD          "/" AE_ENGINE_BIN_TO_DATA_PATH_PREFIX

/// <summary>
/// Default Configuration file path for the Engine
/// </summary>
#define AE_CONFIG_ENGINE_DEFAULT_FILE_PATH      AE_ENGINE_BIN_TO_DATA_PATH_PREFIX "ConfigEngine.xml"

/// <summary>
/// Core Dump File Name
/// </summary>
#define AE_CONFIG_ENGINE_CORE_DUMP              AE_ENGINE_BIN_TO_DATA_PATH_PREFIX "coredump-{0}.dmp"

#define AE_PROJ_DATA_DIR                        "Data"

#define AE_PROJ_ASSETS_DIR                      "Assets"

#define AE_PROJ_RAW_FILES_DIR                   "Raw Files"

#define AE_PROJ_EFFECTS_DIR                     "Effects"

#define AE_PROJ_INPUT_DIR                       "Input"

#define AE_PROJ_GLOBAL_DIR                      "Global"

#define AE_PROJ_LOCALIZATION_DIR                "Localization"

#define AE_PROJ_GAME_OBJECTS_DIR                "Game Objects"

#define AE_PROJ_ASSET_MANAGER_DIR               "Asset Manager"

#define AE_PROJ_LOG_DIR                         "Logs"

#define AE_PROJ_EDITOR_UI_LAYOUTS_DIR           "Editor UI Layouts"

#define AE_PROJ_ASSET_MANAGER_FILE              "AssetManager.xml"

#define AE_PROJ_CONFIG_ENGINE_FILE              "ConfigEngine.xml"

#define AE_PROJ_CONFIG_PROJ_FILE                "ConfigProject.xml"

#define AE_PROJ_LOCALIZATION_ENGINE_FILE        "LocalizationEngine.xml"

#define AE_PROJ_LOCALIZATION_PROJ_FILE          "LocalizationProject.xml"

#define AE_PROJ_GRAPHIC_OPTS_FILE               "GraphicOpts.xml"

#define AE_PROJ_IPUT_FILE                       "Input.xml"

#define AE_PROJ_GAME_OBJECT_MANAGER_FILE        "GameObjectManager.xml"

#define AE_PROJ_LOG_FILE                        "Logs.txt"

#define AE_PROJ_INPUT_DIR_LOC                   (AE_PROJ_DATA_DIR "/" AE_PROJ_INPUT_DIR "/")

#define AE_PROJ_ASSETS_DIR_LOC                  (AE_PROJ_DATA_DIR "/" AE_PROJ_ASSETS_DIR "/")

#define AE_PROJ_RAW_FILES_DIR_LOC               (AE_PROJ_DATA_DIR "/" AE_PROJ_RAW_FILES_DIR "/")

#define AE_PROJ_EFFECTS_DIR_LOC                 (AE_PROJ_DATA_DIR "/" AE_PROJ_EFFECTS_DIR "/")

#define AE_PROJ_GLOBAL_DIR_LOC                  (AE_PROJ_DATA_DIR "/" AE_PROJ_GLOBAL_DIR "/")

#define AE_PROJ_GAME_OBJECTS_DIR_LOC            (AE_PROJ_DATA_DIR "/" AE_PROJ_GAME_OBJECTS_DIR "/")

#define AE_PROJ_ASSET_MANAGER_DIR_LOC           (AE_PROJ_DATA_DIR "/" AE_PROJ_ASSET_MANAGER_DIR "/")

#define AE_PROJ_LOCALIZATION_DIR_LOC            (AE_PROJ_DATA_DIR "/" AE_PROJ_LOCALIZATION_DIR "/")

#define AE_PROJ_LOG_DIR_LOC                     (AE_PROJ_DATA_DIR "/" AE_PROJ_LOG_DIR "/")

#define AE_PROJ_ASSET_MANAGER_FILE_LOC          (AE_PROJ_DATA_DIR "/" AE_PROJ_ASSET_MANAGER_DIR "/" AE_PROJ_ASSET_MANAGER_FILE)

#define AE_PROJ_LOCALIZATION_PROJ_FILE_LOC      (AE_PROJ_DATA_DIR "/" AE_PROJ_LOCALIZATION_DIR "/" AE_PROJ_LOCALIZATION_PROJ_FILE)

#define AE_PROJ_INPUT_FILE_LOC                  (AE_PROJ_DATA_DIR "/" AE_PROJ_LOCALIZATION_DIR "/" AE_PROJ_IPUT_FILE)

#define AE_PROJ_GRAPHIC_OPTS_FILE_LOC           (AE_PROJ_DATA_DIR "/" AE_PROJ_LOCALIZATION_DIR "/" AE_PROJ_GRAPHIC_OPTS_FILE)

#define AE_PROJ_LOG_FILE_LOC                    (AE_PROJ_DATA_DIR "/" AE_PROJ_LOG_DIR "/" AE_PROJ_LOG_FILE)

#define AE_PROJ_GAME_OBJECT_MANAGER_FILE_LOC    (AE_PROJ_DATA_DIR "/" AE_PROJ_GAME_OBJECTS_DIR "/" AE_PROJ_GAME_OBJECT_MANAGER_FILE)

#define AE_PROJ_EDITOR_UI_LAYOUTS_DIR_LOC       (AE_ENGINE_BIN_TO_DATA_PATH_PREFIX AE_PROJ_DATA_DIR "/" AE_PROJ_EDITOR_UI_LAYOUTS_DIR "/")

#define AE_PROJ_DEFAULT_LAYOUT_FILE_LOC         (AE_ENGINE_BIN_TO_DATA_PATH_PREFIX AE_PROJ_DATA_DIR "/" AE_PROJ_EDITOR_UI_LAYOUTS_DIR "/" AE_PROJ_DEFAULT_LAYOUT_FILE)

#define AE_SHADER_SYS_INC_PATH                  (AE_ENGINE_BIN_TO_DATA_PATH_PREFIX AE_PROJ_DATA_DIR "/" AE_PROJ_EFFECTS_DIR "/")

#endif
