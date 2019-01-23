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

/*************************
*   Precompiled Header   *
**************************/
#include "precomp_core.h"

/**********************
*   System Includes   *
***********************/

/*************************
*   3rd Party Includes   *
**************************/

/***************************
*   Game Engine Includes   *
****************************/
#include "GameAppDefs.h"
#include "GameAssets\GameAssetDefs.h"
#include "GameObject\GameObjectDefs.h"

//Always include last
#include "Memory\MemLeaks.h"

/*********************
*   Functions Defs   *
**********************/

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
    std::string BuildConfigProjectFile(const std::string& projectName)
    {
        std::stringstream xmlFile;

        xmlFile << AE_XML_HEADER;
        xmlFile << "<"        << AE_PROJ_CONFIG_MAIN_NODE          << ">";

        xmlFile << "<"        << AE_PROJ_CONFIG_NODE               << " " << AE_PROJ_CONFIG_NAME_PROP << "=\""       << projectName                              << "\" />";
        xmlFile << "<"        << AE_PROJ_GRAPHIC_OPTS_NODE         << " " << AE_PROJ_CONFIG_FILE_PROP << " = \""     << AE_PROJ_GRAPHIC_OPTS_FILE_LOC            << "\" />";
        xmlFile << "<"        << AE_PROJ_INPUT_NODE                << " " << AE_PROJ_CONFIG_FILE_PROP << " = \""     << AE_PROJ_INPUT_FILE_LOC                   << "\" />";
        xmlFile << "<"        << AE_PROJ_LOCALIZATION_NODE         << " " << AE_PROJ_CONFIG_FILE_PROP << " = \""     << AE_PROJ_LOCALIZATION_PROJ_FILE_LOC       << "\" />";
        xmlFile << "<"        << AE_PROJ_ASSET_MANAGER_NODE        << " " << AE_PROJ_CONFIG_FILE_PROP << " = \""     << AE_PROJ_ASSET_MANAGER_FILE_LOC           << "\" />";
        xmlFile << "<"        << AE_PROJ_GAME_OBJECT_MANAGER_NODE  << " " << AE_PROJ_CONFIG_FILE_PROP << " = \""     << AE_PROJ_GAME_OBJECT_MANAGER_FILE_LOC     << "\" />";

        xmlFile << "</"       << AE_PROJ_CONFIG_MAIN_NODE          << ">";

        return xmlFile.str();
    }

    std::string BuildAssetFile()
    {
        std::stringstream xmlFile;

        xmlFile << AE_XML_HEADER;
        xmlFile << "<"     << AE_ASSET_MANAGER_NODE_NAME   << ">";

        xmlFile << "<"     << AE_ASSET_CONFIG_NODE_NAME    << " "     << AE_ASSET_PERSISTANTUNIQUEID_PROP_NAME    << "=\""   << AE_GAME_ASSET_UNIQUE_ID_USER_START   << "\"/>";
        xmlFile << "<"     << AE_ASSETS_NODE_NAME          << ">";
        xmlFile << "</"    << AE_ASSETS_NODE_NAME          << ">";
        xmlFile << "<"     << AE_RAW_FILES_NODE_NAME       << ">";
        xmlFile << "</"    << AE_RAW_FILES_NODE_NAME       << ">";

        xmlFile << "</"    << AE_ASSET_MANAGER_NODE_NAME   << ">";

        return xmlFile.str();
    }

    std::string BuildLocalizationFile()
    {
        std::stringstream xmlFile;

        xmlFile << AE_XML_HEADER;
        xmlFile << "<"             << AE_LOC_LOCALIZATION_NODE_NAME    << ">";
        xmlFile << "</"            << AE_LOC_LOCALIZATION_NODE_NAME    << ">";

        return xmlFile.str();
    }

    std::string BuildGameObjectManagerFile()
    {
        std::stringstream xmlFile;

        xmlFile << AE_XML_HEADER;
        xmlFile << "<"             << AE_GAME_OBJ_MANAGER_NODE_NAME << ">";
        xmlFile << "</"            << AE_GAME_OBJ_MANAGER_NODE_NAME << ">";

        return xmlFile.str();
    }
}
