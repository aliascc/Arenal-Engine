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