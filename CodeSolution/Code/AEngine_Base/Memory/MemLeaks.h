/*
* Copyright (c) 2018 <Carlos Chac�n>
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

#ifndef _MEMLEAKS_H
#define _MEMLEAKS_H

#if defined( _DEBUG )

/**********************
*   System Includes   *
***********************/
#include <map>
#include <list>
#include <string>

#define _CRTDBG_MAP_ALLOC
#include <cstdlib>
#include <crtdbg.h>

#define DBG_NEW new (_NORMAL_BLOCK, __FILE__ , __LINE__)
#define new DBG_NEW

/********************
*   Function Defs   *
*********************/
namespace  MemLeaks
{
    extern bool MemoryBegin();
    extern bool MemoryEnd();
}

#endif

#endif