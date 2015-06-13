/********************************************************
*
* Author: Carlos Chacón N.
*
* Copyright 2012-2015
*
*********************************************************/

#pragma once

#ifndef _LOCALIZATION_MANAGER_DEFS_H
#define _LOCALIZATION_MANAGER_DEFS_H

/**********************
*   System Includes   *
***********************/
#include <unordered_map>
#include <string>

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
#define AELOCMAN	LocalizationManager::GetInstance()

/// <summary>
/// XML Localization Node Name
/// </summary>
#define AE_LOC_LOCALIZATION_NODE_NAME		L"Localization"

/// <summary>
/// XML Default Language Node Name
/// </summary>
#define AE_LOC_DEFAULT_LANG_NODE_NAME		L"Default"

/// <summary>
/// XML Language Property Name
/// </summary>
#define AE_LOC_LANGUAGE_PROP_NAME			L"lang"

/// <summary>
/// XML Language Property Node Name
/// </summary>
#define AE_LOC_LANGUAGE_NODE_NAME			L"Language"

/// <summary>
/// XML Language Name Property Name
/// </summary>
#define AE_LOC_LANG_NAME_PROP_NAME			L"name"

/// <summary>
/// XML File Property Name
/// </summary>
#define AE_LOC_FILE_PROP_NAME				L"file"

/// <summary>
/// XML Literal Node Name
/// </summary>
#define AE_LOC_LITERAL_NODE_NAME			L"Literal"

/// <summary>
/// XML Literal Property Name
/// </summary>
#define AE_LOC_LITERAL_NAME_PROP_NAME		L"name"

/// <summary>
/// XML Literal Message Property Name
/// </summary>
#define AE_LOC_LITERAL_MSG_PROP_NAME		L"msg"

/****************
*   Constants   *
*****************/
extern const std::wstring AE_LOC_LANG_DEFAULT_LITERAL;
extern const std::wstring AE_LOC_LANG_NO_LANGUAGE;

/************
*   Enums   *
*************/

/*************
*   Struct   *
**************/
struct LocalizationLiteral sealed : public Named
{
	std::wstring m_Msg = L"";

	//Constructors
	LocalizationLiteral();
	LocalizationLiteral(const std::wstring& name, const std::wstring& message);
};

/***************
*   Typedefs   *
****************/
typedef std::unordered_map<std::wstring, LocalizationLiteral>	LiteralMap;
typedef std::unordered_map<std::wstring, LiteralMap>			LanguageMap;

#endif
