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
#include "Base\BaseFunctions.h"
#include "GameAssets\GameAssetDefs.h"

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

		xmlFile << L"<?xml version = \"1.0\" encoding = \"iso-8859-1\"?>\n";
		xmlFile << L"<ConfigProject>";
		xmlFile << L"<Config name=\"" << projectName << "\" />";
		xmlFile << L"<DevCaps file = \"Data/Global/DevCaps.xml\" />";
		xmlFile << L"<GraphicOpts file = \"Data/Graphics/GraphicOpts.xml\" />";
		xmlFile << L"<Input file = \"Data/Input/Input.xml\" />";
		xmlFile << L"<Localization file = \"Data/Localization/Localization.xml\" />";
		xmlFile << L"<AssetManager file = \"Data/Asset Manager/AssetManager.xml\" />";
		xmlFile << L"</ConfigProject>";

		return xmlFile.str();
	}

	std::wstring BuildAssetFile()
	{
		std::wstringstream xmlFile;

		xmlFile << L"<?xml version = \"1.0\" encoding = \"iso-8859-1\"?>\n";
		xmlFile << L"<AssetManager>";
		xmlFile << L"<Config PersistantUniqueID=\"" << XE_GAME_ASSET_UNIQUE_ID_USER_START << "\"/>";
		xmlFile << L"<Assets>";
		xmlFile << L"</Assets>";
		xmlFile << L"<RawFiles>";
		xmlFile << L"</RawFiles>";
		xmlFile << L"</AssetManager>";

		return xmlFile.str();
	}
}
