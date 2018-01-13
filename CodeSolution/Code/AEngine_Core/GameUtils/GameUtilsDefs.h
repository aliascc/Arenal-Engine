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
#include "Base\AEObject.h"

/********************
*   Forward Decls   *
*********************/

/**************
*   Defines   *
***************/

#define AE_DEFAULT_GAME_COMPONENT_CALL_ORDER	100

/******************
*   Struct Decl   *
*******************/
struct GCBasicProps sealed : public AEObject
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