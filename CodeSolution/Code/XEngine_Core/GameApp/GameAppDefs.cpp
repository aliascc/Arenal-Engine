/********************************************************
*
* Author: Carlos Chacón N.
*
* Copyright 2012-2015
*
*********************************************************/

/**********************
*   System Includes   *
***********************/
#include <sstream>

/*************************
*   3rd Party Includes   *
**************************/

/***************************
*   Game Engine Includes   *
****************************/
#include "GameAppDefs.h"
#include "XML\XEXMLDefs.h"
#include "Base\BaseFunctions.h"
#include "Base\BaseLocations.h"
#include "GameAssets\GameAssetDefs.h"
#include "GameObject\GameObjectDefs.h"
#include "Localization\LocalizationManagerDefs.h"

//Always include last
#include "Memory\MemLeaks.h"

/*********************
*   Functions Defs   *
**********************/

/************************
*   GameAppScopedLock   *
*************************/
GameAppScopedLock::GameAppScopedLock(std::mutex* gameAppMutexPtr)
	: m_GameAppMutexPtr(gameAppMutexPtr)
{
	XEAssert(m_GameAppMutexPtr != nullptr);
}

GameAppScopedLock::~GameAppScopedLock()
{
	EndLock();
}

void GameAppScopedLock::StartLock()
{
	if (m_GameAppMutexPtr != nullptr)
	{
		m_GameAppMutexPtr->lock();
		m_IsLock = true;
	}
}

void GameAppScopedLock::EndLock()
{
	if (m_GameAppMutexPtr != nullptr && m_IsLock)
	{
		m_GameAppMutexPtr->unlock();
		m_IsLock = false;
	}
}

/***********************
*   GameEngineConfig   *
************************/
GameEngineConfig::GameEngineConfig()
{
}

/***********************
*   GameProjectConfig   *
************************/
GameProjectConfig::GameProjectConfig()
{
}

/******************
*   GameAppOpts   *
*******************/
GameAppOpts::GameAppOpts()
{
}

/******************
*   GameProject   *
*******************/
GameProject::GameProject()
{
}

/********************
*   Function Defs   *
*********************/

namespace XEGameAppHelpers
{
	std::wstring BuildConfigProjectFile(const std::wstring& projectName)
	{
		std::wstringstream xmlFile;

		xmlFile << XE_XML_HEADER;
		xmlFile << L"<"		<< XE_PROJ_CONFIG_MAIN_NODE		<< L">";

		xmlFile << L"<"		<< XE_PROJ_CONFIG_NODE			<< L" " << XE_PROJ_CONFIG_NAME_PROP << L"=\""	<< projectName							<< L"\" />";
		xmlFile << L"<"		<< XE_PROJ_DEV_CAPS_NODE		<< L" " << XE_PROJ_CONFIG_FILE_PROP << L" = \""	<< XE_PROJ_DEVCAPS_FILE_LOC				<< L"\" />";
		xmlFile << L"<"		<< XE_PROJ_GRAPHIC_OPTS_NODE	<< L" " << XE_PROJ_CONFIG_FILE_PROP << L" = \""	<< XE_PROJ_GRAPHIC_OPTS_FILE_LOC		<< L"\" />";
		xmlFile << L"<"		<< XE_PROJ_INPUT_NODE			<< L" " << XE_PROJ_CONFIG_FILE_PROP << L" = \""	<< XE_PROJ_INPUT_FILE_LOC				<< L"\" />";
		xmlFile << L"<"		<< XE_PROJ_LOCALIZATION_NODE	<< L" " << XE_PROJ_CONFIG_FILE_PROP << L" = \""	<< XE_PROJ_LOCALIZATION_PROJ_FILE_LOC	<< L"\" />";
		xmlFile << L"<"		<< XE_PROJ_ASSET_MANAGER_NODE	<< L" " << XE_PROJ_CONFIG_FILE_PROP << L" = \""	<< XE_PROJ_ASSET_MANAGER_FILE_LOC		<< L"\" />";

		xmlFile << L"</"	<< XE_PROJ_CONFIG_MAIN_NODE		<< L">";

		return xmlFile.str();
	}

	std::wstring BuildAssetFile()
	{
		std::wstringstream xmlFile;

		xmlFile << XE_XML_HEADER;
		xmlFile << L"<"		<< XE_ASSET_MANAGER_NODE_NAME	<< L">";

		xmlFile << L"<"		<< XE_ASSET_CONFIG_NODE_NAME	<< L" "	<< XE_ASSET_PERSISTANTUNIQUEID_PROP_NAME	<< L"=\""	<< XE_GAME_ASSET_UNIQUE_ID_USER_START	<< L"\"/>";
		xmlFile << L"<"		<< XE_ASSETS_NODE_NAME			<< L">";
		xmlFile << L"</"	<< XE_ASSETS_NODE_NAME			<< L">";
		xmlFile << L"<"		<< XE_RAW_FILES_NODE_NAME		<< L">";
		xmlFile << L"</"	<< XE_RAW_FILES_NODE_NAME		<< L">";

		xmlFile << L"</"	<< XE_ASSET_MANAGER_NODE_NAME << L">";

		return xmlFile.str();
	}

	std::wstring BuildLocalizationFile()
	{
		std::wstringstream xmlFile;

		xmlFile << XE_XML_HEADER;
		xmlFile << L"<"		<< XE_LOC_LOCALIZATION_NODE_NAME	<< L">";
		xmlFile << L"</"	<< XE_LOC_LOCALIZATION_NODE_NAME	<< L">";

		return xmlFile.str();
	}

	std::wstring BuildGameObjectManagerFile()
	{
		std::wstringstream xmlFile;

		xmlFile << XE_XML_HEADER;
		xmlFile << L"<" << XE_GAME_OBJ_MANAGER_NODE_NAME << L">";
		xmlFile << L"</" << XE_GAME_OBJ_MANAGER_NODE_NAME << L">";

		return xmlFile.str();
	}
}
