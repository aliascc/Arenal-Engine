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

#ifndef _GRAPHIC_UTILS_DEFS_H
#define _GRAPHIC_UTILS_DEFS_H

/**********************
*   System Includes   *
***********************/

/***************************
*   Game Engine Includes   *
****************************/

/********************
*   Forward Decls   *
*********************/
class Object3D;

/************
*   Using   *
*************/

/**************
*   Defines   *
***************/

/****************
*   Constants   *
*****************/

/************
*   Enums   *
*************/

/// <summary>
/// Type of Object 3D Change Event
/// </summary>
enum class Object3DChangeEventType : uint32_t
{
    PositionChange = 0,
    RotationChange,
    ScaleChange,
    ParentChange
};

/*************
*   Struct   *
**************/

/**************
*   Typedef   *
***************/
typedef std::function<void(Object3DChangeEventType, Object3D&)> Object3DChangedEvent;


#endif
