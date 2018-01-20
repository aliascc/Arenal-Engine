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
#include "XML\AEXMLDefs.h"
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
    AEAssert(m_GameAppMutexPtr != nullptr);
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

        xmlFile << AE_XML_HEADER;
        xmlFile << L"<"        << AE_PROJ_CONFIG_MAIN_NODE          << L">";

        xmlFile << L"<"        << AE_PROJ_CONFIG_NODE               << L" " << AE_PROJ_CONFIG_NAME_PROP << L"=\""       << projectName                              << L"\" />";
        xmlFile << L"<"        << AE_PROJ_DEV_CAPS_NODE             << L" " << AE_PROJ_CONFIG_FILE_PROP << L" = \""     << AE_PROJ_DEVCAPS_FILE_LOC                 << L"\" />";
        xmlFile << L"<"        << AE_PROJ_GRAPHIC_OPTS_NODE         << L" " << AE_PROJ_CONFIG_FILE_PROP << L" = \""     << AE_PROJ_GRAPHIC_OPTS_FILE_LOC            << L"\" />";
        xmlFile << L"<"        << AE_PROJ_INPUT_NODE                << L" " << AE_PROJ_CONFIG_FILE_PROP << L" = \""     << AE_PROJ_INPUT_FILE_LOC                   << L"\" />";
        xmlFile << L"<"        << AE_PROJ_LOCALIZATION_NODE         << L" " << AE_PROJ_CONFIG_FILE_PROP << L" = \""     << AE_PROJ_LOCALIZATION_PROJ_FILE_LOC       << L"\" />";
        xmlFile << L"<"        << AE_PROJ_ASSET_MANAGER_NODE        << L" " << AE_PROJ_CONFIG_FILE_PROP << L" = \""     << AE_PROJ_ASSET_MANAGER_FILE_LOC           << L"\" />";
        xmlFile << L"<"        << AE_PROJ_GAME_OBJECT_MANAGER_NODE  << L" " << AE_PROJ_CONFIG_FILE_PROP << L" = \""     << AE_PROJ_GAME_OBJECT_MANAGER_FILE_LOC     << L"\" />";

        xmlFile << L"</"       << AE_PROJ_CONFIG_MAIN_NODE          << L">";

        return xmlFile.str();
    }

    std::wstring BuildAssetFile()
    {
        std::wstringstream xmlFile;

        xmlFile << AE_XML_HEADER;
        xmlFile << L"<"     << AE_ASSET_MANAGER_NODE_NAME   << L">";

        xmlFile << L"<"     << AE_ASSET_CONFIG_NODE_NAME    << L" "     << AE_ASSET_PERSISTANTUNIQUEID_PROP_NAME    << L"=\""   << AE_GAME_ASSET_UNIQUE_ID_USER_START   << L"\"/>";
        xmlFile << L"<"     << AE_ASSETS_NODE_NAME          << L">";
        xmlFile << L"</"    << AE_ASSETS_NODE_NAME          << L">";
        xmlFile << L"<"     << AE_RAW_FILES_NODE_NAME       << L">";
        xmlFile << L"</"    << AE_RAW_FILES_NODE_NAME       << L">";

        xmlFile << L"</"    << AE_ASSET_MANAGER_NODE_NAME   << L">";

        return xmlFile.str();
    }

    std::wstring BuildLocalizationFile()
    {
        std::wstringstream xmlFile;

        xmlFile << AE_XML_HEADER;
        xmlFile << L"<"             << AE_LOC_LOCALIZATION_NODE_NAME    << L">";
        xmlFile << L"</"            << AE_LOC_LOCALIZATION_NODE_NAME    << L">";

        return xmlFile.str();
    }

    std::wstring BuildGameObjectManagerFile()
    {
        std::wstringstream xmlFile;

        xmlFile << AE_XML_HEADER;
        xmlFile << L"<"             << AE_GAME_OBJ_MANAGER_NODE_NAME << L">";
        xmlFile << L"</"            << AE_GAME_OBJ_MANAGER_NODE_NAME << L">";

        return xmlFile.str();
    }
}
