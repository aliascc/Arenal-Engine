/********************************************************
*
* Author: Carlos Chacón N.
*
* Copyright 2012-2015
*
*********************************************************/

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
#define XE_ENGINE_BIN_TO_DATA_PATH_PREFIX		L"../"

/// <summary>
/// Jump from Engine Binary Directory to Data
/// </summary>
#define XE_ENGINE_BIN_TO_DATA_PATH_ADD			L"/" XE_ENGINE_BIN_TO_DATA_PATH_PREFIX

/// <summary>
/// Default Configuration file path for the Engine
/// </summary>
#define XE_CONFIG_ENGINE_DEFAULT_FILE_PATH		XE_ENGINE_BIN_TO_DATA_PATH_PREFIX L"ConfigEngine.xml"

#define XE_PROJ_DATA_DIR						L"Data"

#define XE_PROJ_ASSETS_DIR						L"Assets"

#define XE_PROJ_RAW_FILES_DIR					L"Raw Files"

#define XE_PROJ_EFFECTS_DIR						L"Effects"

#define XE_PROJ_INPUT_DIR						L"Input"

#define XE_PROJ_GLOBAL_DIR						L"Global"

#define XE_PROJ_LOCALIZATION_DIR				L"Localization"

#define XE_PROJ_GAME_OBJECTS_DIR				L"Game Objects"

#define XE_PROJ_ASSET_MANAGER_DIR				L"Asset Manager"

#define XE_PROJ_LOG_DIR							L"Logs"

#define XE_PROJ_EDITOR_UI_LAYOUTS_DIR			L"Editor UI Layouts"

#define XE_PROJ_ASSET_MANAGER_FILE				L"AssetManager.xml"

#define XE_PROJ_CONFIG_ENGINE_FILE				L"ConfigEngine.xml"

#define XE_PROJ_CONFIG_PROJ_FILE				L"ConfigProject.xml"

#define XE_PROJ_LOCALIZATION_ENGINE_FILE		L"LocalizationEngine.xml"

#define XE_PROJ_LOCALIZATION_PROJ_FILE			L"LocalizationProject.xml"

#define XE_PROJ_GRAPHIC_OPTS_FILE				L"GraphicOpts.xml"

#define XE_PROJ_IPUT_FILE						L"Input.xml"

#define XE_PROJ_DEVCAPS_FILE					L"DevCaps.xml"

#define XE_PROJ_GAME_OBJECT_MANAGER_FILE		L"GameObjectManager.xml"

#define XE_PROJ_LOG_FILE						L"Logs.txt"

#define XE_PROJ_DEFAULT_LAYOUT_FILE				L"default.qtl"

#define XE_PROJ_INPUT_DIR_LOC					(XE_PROJ_DATA_DIR L"/" XE_PROJ_INPUT_DIR L"/")

#define XE_PROJ_ASSETS_DIR_LOC					(XE_PROJ_DATA_DIR L"/" XE_PROJ_ASSETS_DIR L"/")

#define XE_PROJ_RAW_FILES_DIR_LOC				(XE_PROJ_DATA_DIR L"/" XE_PROJ_RAW_FILES_DIR L"/")

#define XE_PROJ_EFFECTS_DIR_LOC					(XE_PROJ_DATA_DIR L"/" XE_PROJ_EFFECTS_DIR L"/")

#define XE_PROJ_GLOBAL_DIR_LOC					(XE_PROJ_DATA_DIR L"/" XE_PROJ_GLOBAL_DIR L"/")

#define XE_PROJ_GAME_OBJECTS_DIR_LOC			(XE_PROJ_DATA_DIR L"/" XE_PROJ_GAME_OBJECTS_DIR L"/")

#define XE_PROJ_ASSET_MANAGER_DIR_LOC			(XE_PROJ_DATA_DIR L"/" XE_PROJ_ASSET_MANAGER_DIR L"/")

#define XE_PROJ_LOCALIZATION_DIR_LOC			(XE_PROJ_DATA_DIR L"/" XE_PROJ_LOCALIZATION_DIR L"/")

#define XE_PROJ_LOG_DIR_LOC						(XE_PROJ_DATA_DIR L"/" XE_PROJ_LOG_DIR L"/")

#define XE_PROJ_ASSET_MANAGER_FILE_LOC			(XE_PROJ_DATA_DIR L"/" XE_PROJ_ASSET_MANAGER_DIR L"/" XE_PROJ_ASSET_MANAGER_FILE)

#define XE_PROJ_LOCALIZATION_PROJ_FILE_LOC		(XE_PROJ_DATA_DIR L"/" XE_PROJ_LOCALIZATION_DIR L"/" XE_PROJ_LOCALIZATION_PROJ_FILE)

#define XE_PROJ_INPUT_FILE_LOC					(XE_PROJ_DATA_DIR L"/" XE_PROJ_LOCALIZATION_DIR L"/" XE_PROJ_IPUT_FILE)

#define XE_PROJ_GRAPHIC_OPTS_FILE_LOC			(XE_PROJ_DATA_DIR L"/" XE_PROJ_LOCALIZATION_DIR L"/" XE_PROJ_GRAPHIC_OPTS_FILE)

#define XE_PROJ_DEVCAPS_FILE_LOC				(XE_PROJ_DATA_DIR L"/" XE_PROJ_LOCALIZATION_DIR L"/" XE_PROJ_DEVCAPS_FILE)

#define XE_PROJ_LOG_FILE_LOC					(XE_PROJ_DATA_DIR L"/" XE_PROJ_LOG_DIR L"/" XE_PROJ_LOG_FILE)

#define XE_PROJ_GAME_OBJECT_MANAGER_FILE_LOC	(XE_PROJ_DATA_DIR L"/" XE_PROJ_GAME_OBJECTS_DIR L"/" XE_PROJ_GAME_OBJECT_MANAGER_FILE)

#define XE_PROJ_EDITOR_UI_LAYOUTS_DIR_LOC		(XE_ENGINE_BIN_TO_DATA_PATH_PREFIX XE_PROJ_DATA_DIR L"/" XE_PROJ_EDITOR_UI_LAYOUTS_DIR L"/")

#define XE_PROJ_DEFAULT_LAYOUT_FILE_LOC			(XE_ENGINE_BIN_TO_DATA_PATH_PREFIX XE_PROJ_DATA_DIR L"/" XE_PROJ_EDITOR_UI_LAYOUTS_DIR L"/" XE_PROJ_DEFAULT_LAYOUT_FILE)

#define XE_SHADER_SYS_INC_PATH					(XE_ENGINE_BIN_TO_DATA_PATH_PREFIX XE_PROJ_DATA_DIR L"/" XE_PROJ_EFFECTS_DIR L"/")

#endif
