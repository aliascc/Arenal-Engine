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

#ifndef _ANGEL_SCRIPT_DEFS_H
#define _ANGEL_SCRIPT_DEFS_H

/**********************
*   System Includes   *
***********************/

/*************************
*   3rd Party Includes   *
**************************/

/***************************
*   Game Engine Includes   *
****************************/

/************
*   Using   *
*************/

/********************
*   Forward Decls   *
*********************/
struct AELog;
struct TimeStamp;
struct TimerParams;

/*************
*   Define   *
**************/

/// <summary>
/// Defines the Base Add on ID for Users Add-on
/// </summary>
#define AE_AS_ADD_ON_ID_USER_START        10000

/***********************
*   Global Functions   *
************************/

namespace AEAngelScriptHelpers
{
    extern uint32_t GetSizeOfType(asETypeIdFlags typeID);

    extern bool TypeIsBasic(asETypeIdFlags typeID);
}

#endif
