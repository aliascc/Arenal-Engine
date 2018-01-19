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

#ifndef _GAME_RESOURCE_DEFS_H
#define _GAME_RESOURCE_DEFS_H

/**********************
*   System Includes   *
***********************/
#include <stdint.h>
#include <functional>

/***************************
*   Game Engine Includes   *
****************************/

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
/// Classifies Game Resources into different types
/// </summary>
enum class GameResourceType : uint32_t
{
    Mesh = 0,
    GameObjectScript,
    Texture2D,
    TextureCube,
    Skeleton,
    Animation,
    VertexShader,
    PixelShader,
    ComputeShader,
    DomainShader,
    HullShader,
    GeometryShader,
    RenderTarget,
    DepthStencil,
    Audio,
    Custom,
    Unknown
};

/*************
*   Struct   *
**************/

/**************
*   Typedef   *
***************/
typedef std::function<void (uint64_t, bool)> GameResourceReleaseCallback;
typedef std::function<void (uint64_t, const std::wstring&, const std::wstring&)> GameResourceChangeNameCallback;
typedef std::function<void (uint64_t, const std::wstring&, const std::wstring&)> GameResourceChangeFileNameCallback;

#endif
