/********************************************************
*
* Author: Carlos Chacón N.
*
* Created: 4/25/2012
*
* Desc:
*
*
* Copyright 2012
*
*********************************************************/
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
#include "Base\XEObject.h"
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
struct DeferredRenderingEndSettings sealed : public XEObject
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
	DeferredRenderingEndSettings(const std::string& name = "Default", GraphicsPixelFormat lightPixFmt = XE_PX_FMT_A8R8G8B8, bool useDefaultRT = true, GraphicsPixelFormat finalPixFmt = XE_PX_FMT_A8R8G8B8);
};

struct DeferredRenderingSettings sealed : public XEObject
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
	DeferredRenderingSettings(const std::string& name = "Default", GraphicsPixelFormat diffusePixFmt = XE_PX_FMT_A8R8G8B8, GraphicsPixelFormat normalPixFmt = XE_PX_FMT_A8R8G8B8, GraphicsPixelFormat depthPixFmt = XE_PX_FMT_R32F);
};

/********************************
*		Type Defs				*
*********************************/
													 		
#endif