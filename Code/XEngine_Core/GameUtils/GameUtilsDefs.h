/********************************************************
*
* Author: Carlos Chacón N.
*
* Created: 4/22/2012
*
* Desc:
*
*
* Copyright 2012
*
*********************************************************/

#pragma once

#ifndef _GAME_UTILS_DEFS_H
#define _GAME_UTILS_DEFS_H

/**********************
*   System Includes   *
***********************/
#include <string>
#include <stdint.h>
#include <functional>

/*************************
*   3rd Party Includes   *
**************************/

/***************************
*   Game Engine Includes   *
****************************/
#include "Base\XEObject.h"

/********************
*   Forward Decls   *
*********************/

/**************
*   Defines   *
***************/

#define XE_DEFAULT_GAME_COMPONENT_CALL_ORDER	100

/******************
*   Struct Decl   *
*******************/
struct GCBasicProps sealed : public XEObject
{
	uint64_t m_GCID = 0;
	std::wstring m_GCName = L"";
	uint32_t m_GCCallOrder = 0;
	bool m_Drawable = false;
	bool m_Enabled = false;
	bool m_Visibled = false;

	//Constructor
	GCBasicProps();	
	~GCBasicProps();

	//Use for Angel Script
	GCBasicProps& operator=(const GCBasicProps &other);
	static void Constructor(GCBasicProps* self);
	static void Destructor(GCBasicProps* self);
};

/**************
*   Typedef   *
***************/
typedef std::function<void ()> NeedSortChangeCallback;

#endif