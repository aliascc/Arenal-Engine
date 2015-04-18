/********************************************************
*
* Author: Carlos Chacón N.
*
* Created: 1/29/2012
*
* Desc:
*
*
* Copyright 2012
*
*********************************************************/
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