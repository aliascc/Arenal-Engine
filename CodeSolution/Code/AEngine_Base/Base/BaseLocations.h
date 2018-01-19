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
#define AE_ENGINE_BIN_TO_DATA_PATH_PREFIX       L"../"

/// <summary>
/// Jump from Engine Binary Directory to Data
/// </summary>
#define AE_ENGINE_BIN_TO_DATA_PATH_ADD          L"/" AE_ENGINE_BIN_TO_DATA_PATH_PREFIX

/// <summary>
/// Default Configuration file path for the Engine
/// </summary>
#define AE_CONFIG_ENGINE_DEFAULT_FILE_PATH      AE_ENGINE_BIN_TO_DATA_PATH_PREFIX L"ConfigEngine.xml"

/// <summary>
/// Core Dump File Name
/// </summary>
#define AE_CONFIG_ENGINE_CORE_DUMP              AE_ENGINE_BIN_TO_DATA_PATH_PREFIX L"coredump-{0}.dmp"

#define AE_PROJ_DATA_DIR                        L"Data"

#define AE_PROJ_ASSETS_DIR                      L"Assets"

#define AE_PROJ_RAW_FILES_DIR                   L"Raw Files"

#define AE_PROJ_EFFECTS_DIR                     L"Effects"

#define AE_PROJ_INPUT_DIR                       L"Input"

#define AE_PROJ_GLOBAL_DIR                      L"Global"

#define AE_PROJ_LOCALIZATION_DIR                L"Localization"

#define AE_PROJ_GAME_OBJECTS_DIR                L"Game Objects"

#define AE_PROJ_ASSET_MANAGER_DIR               L"Asset Manager"

#define AE_PROJ_LOG_DIR                         L"Logs"

#define AE_PROJ_EDITOR_UI_LAYOUTS_DIR           L"Editor UI Layouts"

#define AE_PROJ_ASSET_MANAGER_FILE              L"AssetManager.xml"

#define AE_PROJ_CONFIG_ENGINE_FILE              L"ConfigEngine.xml"

#define AE_PROJ_CONFIG_PROJ_FILE                L"ConfigProject.xml"

#define AE_PROJ_LOCALIZATION_ENGINE_FILE        L"LocalizationEngine.xml"

#define AE_PROJ_LOCALIZATION_PROJ_FILE          L"LocalizationProject.xml"

#define AE_PROJ_GRAPHIC_OPTS_FILE               L"GraphicOpts.xml"

#define AE_PROJ_IPUT_FILE                       L"Input.xml"

#define AE_PROJ_DEVCAPS_FILE                    L"DevCaps.xml"

#define AE_PROJ_GAME_OBJECT_MANAGER_FILE        L"GameObjectManager.xml"

#define AE_PROJ_LOG_FILE                        L"Logs.txt"

#define AE_PROJ_DEFAULT_LAYOUT_FILE             L"default.qtl"

#define AE_PROJ_INPUT_DIR_LOC                   (AE_PROJ_DATA_DIR L"/" AE_PROJ_INPUT_DIR L"/")

#define AE_PROJ_ASSETS_DIR_LOC                  (AE_PROJ_DATA_DIR L"/" AE_PROJ_ASSETS_DIR L"/")

#define AE_PROJ_RAW_FILES_DIR_LOC               (AE_PROJ_DATA_DIR L"/" AE_PROJ_RAW_FILES_DIR L"/")

#define AE_PROJ_EFFECTS_DIR_LOC                 (AE_PROJ_DATA_DIR L"/" AE_PROJ_EFFECTS_DIR L"/")

#define AE_PROJ_GLOBAL_DIR_LOC                  (AE_PROJ_DATA_DIR L"/" AE_PROJ_GLOBAL_DIR L"/")

#define AE_PROJ_GAME_OBJECTS_DIR_LOC            (AE_PROJ_DATA_DIR L"/" AE_PROJ_GAME_OBJECTS_DIR L"/")

#define AE_PROJ_ASSET_MANAGER_DIR_LOC           (AE_PROJ_DATA_DIR L"/" AE_PROJ_ASSET_MANAGER_DIR L"/")

#define AE_PROJ_LOCALIZATION_DIR_LOC            (AE_PROJ_DATA_DIR L"/" AE_PROJ_LOCALIZATION_DIR L"/")

#define AE_PROJ_LOG_DIR_LOC                     (AE_PROJ_DATA_DIR L"/" AE_PROJ_LOG_DIR L"/")

#define AE_PROJ_ASSET_MANAGER_FILE_LOC          (AE_PROJ_DATA_DIR L"/" AE_PROJ_ASSET_MANAGER_DIR L"/" AE_PROJ_ASSET_MANAGER_FILE)

#define AE_PROJ_LOCALIZATION_PROJ_FILE_LOC      (AE_PROJ_DATA_DIR L"/" AE_PROJ_LOCALIZATION_DIR L"/" AE_PROJ_LOCALIZATION_PROJ_FILE)

#define AE_PROJ_INPUT_FILE_LOC                  (AE_PROJ_DATA_DIR L"/" AE_PROJ_LOCALIZATION_DIR L"/" AE_PROJ_IPUT_FILE)

#define AE_PROJ_GRAPHIC_OPTS_FILE_LOC           (AE_PROJ_DATA_DIR L"/" AE_PROJ_LOCALIZATION_DIR L"/" AE_PROJ_GRAPHIC_OPTS_FILE)

#define AE_PROJ_DEVCAPS_FILE_LOC                (AE_PROJ_DATA_DIR L"/" AE_PROJ_LOCALIZATION_DIR L"/" AE_PROJ_DEVCAPS_FILE)

#define AE_PROJ_LOG_FILE_LOC                    (AE_PROJ_DATA_DIR L"/" AE_PROJ_LOG_DIR L"/" AE_PROJ_LOG_FILE)

#define AE_PROJ_GAME_OBJECT_MANAGER_FILE_LOC    (AE_PROJ_DATA_DIR L"/" AE_PROJ_GAME_OBJECTS_DIR L"/" AE_PROJ_GAME_OBJECT_MANAGER_FILE)

#define AE_PROJ_EDITOR_UI_LAYOUTS_DIR_LOC       (AE_ENGINE_BIN_TO_DATA_PATH_PREFIX AE_PROJ_DATA_DIR L"/" AE_PROJ_EDITOR_UI_LAYOUTS_DIR L"/")

#define AE_PROJ_DEFAULT_LAYOUT_FILE_LOC         (AE_ENGINE_BIN_TO_DATA_PATH_PREFIX AE_PROJ_DATA_DIR L"/" AE_PROJ_EDITOR_UI_LAYOUTS_DIR L"/" AE_PROJ_DEFAULT_LAYOUT_FILE)

#define AE_SHADER_SYS_INC_PATH                  (AE_ENGINE_BIN_TO_DATA_PATH_PREFIX AE_PROJ_DATA_DIR L"/" AE_PROJ_EFFECTS_DIR L"/")

#endif
