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
#define XELOCMAN	LocalizationManager::GetInstance()

#define XE_LOC_LOCALIZATION_NODE_NAME		L"Localization"

#define XE_LOC_DEFAULT_LANG_NODE_NAME		L"Default"

#define XE_LOC_LANGUAGE_PROP_NAME			L"lang"

#define XE_LOC_LANGUAGE_NODE_NAME			L"Language"

#define XE_LOC_LANG_NAME_PROP_NAME			L"name"

#define XE_LOC_FILE_PROP_NAME				L"file"

#define XE_LOC_LITERAL_NODE_NAME			L"Literal"

#define XE_LOC_LITERAL_NAME_PROP_NAME		L"name"

#define XE_LOC_LITERAL_MSG_PROP_NAME		L"msg"

/****************
*   Constants   *
*****************/
extern const std::wstring XE_LOC_LANG_DEFAULT_LITERAL;
extern const std::wstring XE_LOC_LANG_NO_LANGUAGE;

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
