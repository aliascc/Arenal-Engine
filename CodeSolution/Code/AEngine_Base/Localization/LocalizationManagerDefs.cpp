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

/***************************
*   Game Engine Includes   *
****************************/
#include "LocalizationManagerDefs.h"

//Always include last
#include "Memory\MemLeaks.h"

/****************
*   Constants   *
*****************/
const std::wstring AE_LOC_LANG_DEFAULT_LITERAL	= L"NO LITERAL FOUND!";
const std::wstring AE_LOC_LANG_NO_LANGUAGE		= L"NO LANGUAGE FOUND!";

/********************
*   Function Defs   *
*********************/

/**************************
*   LocalizationLiteral   *
***************************/
#pragma region Struct LocalizationLiteral

LocalizationLiteral::LocalizationLiteral()
	: Named(L"")
{
}

LocalizationLiteral::LocalizationLiteral(const std::wstring& name, const std::wstring& message)
	: Named(name)
	, m_Msg(message)
{
}

#pragma endregion
