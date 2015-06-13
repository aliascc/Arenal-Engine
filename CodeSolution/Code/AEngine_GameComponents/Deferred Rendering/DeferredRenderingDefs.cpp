/********************************************************
*
* Author: Carlos Chacón N.
*
* Copyright 2012-2015
*
*********************************************************/

/**********************
*   System Includes   *
***********************/

/***************************
*   Game Engine Includes   *
****************************/
#include "DeferredRenderingDefs.h"

/********************
*   Function Defs   *
*********************/

/*********************************
*	DeferredRenderingSettings	 *
**********************************/
#pragma region struct DeferredRenderingSettings

DeferredRenderingSettings::DeferredRenderingSettings(const std::string& name, GraphicsPixelFormat diffusePixFmt, GraphicsPixelFormat normalPixFmt, GraphicsPixelFormat depthPixFmt)
	: m_Name(name)
	, m_DiffusePixFmt(diffusePixFmt)
	, m_NormalPixFmt(normalPixFmt)
	, m_DepthPixFmt(depthPixFmt)
{
}

#pragma endregion

/*********************************
*  DeferredRenderingEndSettings  *
**********************************/
#pragma region struct DeferredRenderingEndSettings

DeferredRenderingEndSettings::DeferredRenderingEndSettings(const std::string& name, GraphicsPixelFormat lightPixFmt, bool useDefaultRT, GraphicsPixelFormat finalPixFmt)
	: m_Name(name)
	, m_UseDefaultRT(useDefaultRT)
	, m_FinalPixFmt(finalPixFmt)
	, m_LightPixFmt(lightPixFmt)
{
}

#pragma endregion