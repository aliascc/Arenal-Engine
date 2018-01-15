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

#ifndef _GLOBAL_CONFIG_H
#define _GLOBAL_CONFIG_H

/**********************
*   System Includes   *
***********************/
#include <vector>
#include <string>
#include <stdint.h>

/*************************
*   3rd Party Includes   *
**************************/

/***************************
*   Game Engine Includes   *
****************************/
#include "Base\Base.h"

/************
*   Using   *
*************/

/********************
*   Forward Decls   *
*********************/

/***********************
*   Global Functions   *
************************/
namespace AE_GlobalConfig
{
	extern AEResult GetRecentProjects(std::vector<std::wstring>& recentProjects);

	extern AEResult GetRecentProjectDirectory(std::wstring& recentProjectDir);

	extern AEResult AddRecentProject(const std::wstring& recentProject);
	
	extern AEResult SetRecentProjectDirectory(const std::wstring& recentDir);
};

#endif