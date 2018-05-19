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

#ifndef _LOCALIZATION_MANAGER_DEFS_H
#define _LOCALIZATION_MANAGER_DEFS_H

/**********************
*   System Includes   *
***********************/

/***************************
*   Game Engine Includes   *
****************************/
#include "Base\Named.h"

/********************
*   Forward Decls   *
*********************/
class LocalizationManager;

/**************
*   Defines   *
***************/

/// <summary>
/// Define for Localization Manager Singleton Instance Get Call 
/// </summary>
#define AELOCMAN                        LocalizationManager::GetInstance()

/// <summary>
/// XML Localization Node Name
/// </summary>
#define AE_LOC_LOCALIZATION_NODE_NAME   "Localization"

/// <summary>
/// XML Default Language Node Name
/// </summary>
#define AE_LOC_DEFAULT_LANG_NODE_NAME   "Default"

/// <summary>
/// XML Language Property Name
/// </summary>
#define AE_LOC_LANGUAGE_PROP_NAME       "lang"

/// <summary>
/// XML Language Property Node Name
/// </summary>
#define AE_LOC_LANGUAGE_NODE_NAME       "Language"

/// <summary>
/// XML Language Name Property Name
/// </summary>
#define AE_LOC_LANG_NAME_PROP_NAME      "name"

/// <summary>
/// XML File Property Name
/// </summary>
#define AE_LOC_FILE_PROP_NAME           "file"

/// <summary>
/// XML Literal Node Name
/// </summary>
#define AE_LOC_LITERAL_NODE_NAME        "Literal"

/// <summary>
/// XML Literal Property Name
/// </summary>
#define AE_LOC_LITERAL_NAME_PROP_NAME   "name"

/// <summary>
/// XML Literal Message Property Name
/// </summary>
#define AE_LOC_LITERAL_MSG_PROP_NAME    "msg"

/****************
*   Constants   *
*****************/
extern const std::string AE_LOC_LANG_DEFAULT_LITERAL;
extern const std::string AE_LOC_LANG_NO_LANGUAGE;

/************
*   Enums   *
*************/

/*************
*   Struct   *
**************/
struct LocalizationLiteral sealed : public Named
{
    std::string m_Msg = "";

    //Constructors
    LocalizationLiteral();
    LocalizationLiteral(const std::string& name, const std::string& message);
};

/***************
*   Typedefs   *
****************/
typedef std::unordered_map<std::string, LocalizationLiteral>   LiteralMap;
typedef std::unordered_map<std::string, LiteralMap>            LanguageMap;

#endif
