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

#ifndef _DEFERRED_RENDERING_DEFS_H
#define _DEFERRED_RENDERING_DEFS_H

/**********************
*   System Includes   *
***********************/
#include <string>

/***************************
*   Game Engine Includes   *
****************************/
#include "Base\AEObject.h"
#include "GraphicsDefs.h"

/********************************
*		Using					*
*********************************/

/**************
*   Defines   *
***************/

/****************
*   Constants   *
*****************/

/************
*   Enums   *
*************/

/*************
*   Struct   *
**************/
struct DeferredRenderingEndSettings sealed : public AEObject
{
	///<summary>
	///Named of the Deferred Render End Settings
	///</summary>
	std::string m_Name;

	///<summary>
	///Use Default Render Target
	///</summary>
	bool m_UseDefaultRT;

	///<summary>
	///Pixel Format for Final Render Target
	///</summary>
	GraphicsPixelFormat m_FinalPixFmt;

	///<summary>
	///Pixel Format for Light Render Target
	///</summary>
	GraphicsPixelFormat m_LightPixFmt;

	///<summary>
	///Constructor of DeferredRenderingEndSettings
	///</summary>
	///<param name="name">Named of the Deferred Render End Settings</param>
	///<param name="lightPixFmt">Pixel Format for Light Render Target</param>
	///<param name="useDefaultRT">Use Default Render Target</param>
	///<param name="finalPixFmt">Pixel Format for Final Render Target</param>
	DeferredRenderingEndSettings(const std::string& name = "Default", GraphicsPixelFormat lightPixFmt = AE_PX_FMT_A8R8G8B8, bool useDefaultRT = true, GraphicsPixelFormat finalPixFmt = AE_PX_FMT_A8R8G8B8);
};

struct DeferredRenderingSettings sealed : public AEObject
{	
	///<summary>
	///Named of the Deferred Render Settings
	///</summary>
	std::string m_Name;

	///<summary>
	///Pixel Format for Diffuse Render Target
	///</summary>
	GraphicsPixelFormat m_DiffusePixFmt;
	
	///<summary>
	///Pixel Format for Normal Render Target
	///</summary>
	GraphicsPixelFormat m_NormalPixFmt;
	
	///<summary>
	///Pixel Format for Depth Render Target
	///</summary>
	GraphicsPixelFormat m_DepthPixFmt;

	///<summary>
	///Constructor of DeferredRenderingSettings
	///</summary>
	///<param name="name">Named of the Deferred Render Settings</param>
	///<param name="diffusePixFmt">Pixel Format for Diffuse Render Target</param>
	///<param name="normalPixFmt">Pixel Format for Normal Render Target</param>
	///<param name="depthPixFmt">Pixel Format for Depth Render Target</param>
	DeferredRenderingSettings(const std::string& name = "Default", GraphicsPixelFormat diffusePixFmt = AE_PX_FMT_A8R8G8B8, GraphicsPixelFormat normalPixFmt = AE_PX_FMT_A8R8G8B8, GraphicsPixelFormat depthPixFmt = AE_PX_FMT_R32F);
};

/********************************
*		Type Defs				*
*********************************/
													 		
#endif