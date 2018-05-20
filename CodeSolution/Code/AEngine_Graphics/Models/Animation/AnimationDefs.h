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

#ifndef _ANIMATION_DEFS_H
#define _ANIMATION_DEFS_H

/**********************
*   System Includes   *
***********************/

/***************************
*   Game Engine Includes   *
****************************/
#include "Base\AEObject.h"

/************
*   Using   *
*************/

/********************
*   Forward Decls   *
*********************/

/**************
*   Defines   *
***************/

/****************
*   Constants   *
*****************/

/************
*   Enums   *
*************/
enum class AnimationBlendType : uint32_t
{
    Lerp = 0
};

/**************
*   Typedef   *
***************/
typedef std::function<void ()> AnimationCallBackFunc; 

/*************
*   Struct   *
**************/
struct AnimationCallBack sealed : public AEObject
{
    float m_Time = 0.0f;
    AnimationCallBackFunc m_AnimFuncPtr = nullptr;

    AnimationCallBack();
};

#endif
