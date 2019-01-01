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

#ifndef _DEFERRED_RENDERING_H
#define _DEFERRED_RENDERING_H

/**********************
*   System Includes   *
***********************/
#include <string>

/***************************
*   Game Engine Includes   *
****************************/
#include "DeferredRenderingDefs.h"

/********************************
*		Using					*
*********************************/

/********************
*   Forward Decls   *
*********************/
struct TimerParams;
class GameApp;
class RenderTarget;
class DeferredRenderingMaterial;

/*****************
*   Class Decl   *
******************/
class DeferredRendering : public DrawableGameComponent
{
	protected:
		

		/****************************
		 *   Protected Variables    *
		 ****************************/
#pragma region Protected Variables
	
		///<summary>
		///Deferred Rendering Name
		///</summary>
		const std::string m_ServiceName;
	
		///<summary>
		///Deferred Rendering Diffuse Render Target
		///</summary>
		RenderTarget* m_DiffuseRT;
	
		///<summary>
		///Deferred Rendering Normal Render Target
		///</summary>
		RenderTarget* m_NormalRT;
	
		///<summary>
		///Deferred Rendering Depth Render Target
		///</summary>
		RenderTarget* m_DepthRT;
	
		///<summary>
		///Deferred Rendering Settings
		///</summary>
		DeferredRenderingSettings m_Settings;
	
		///<summary>
		///Deferred Rendering Material
		///</summary>
		DeferredRenderingMaterial* m_DREffect;

#pragma endregion
		
		/************************************
		 *   Protected Framework Methods    *
		 ************************************/
#pragma region Protected Framework Methods

		///<summary>
		///Initialize Constant Effect Params
		///</summary>
		///<returns>AEResult::Ok if Initialize Constant Effect Params succeeded</returns>
		AEResult InitEffectParams();

#pragma endregion

	public:
		//Constructor Destructor.
		DeferredRendering(GameApp* gameApp, const DeferredRenderingSettings& settings, uint32_t callOrder = 0, const std::string serviceName = "DeferredRendering", const std::string& gameComponentName = "Deferred Rendering");
		virtual ~DeferredRendering();

		//Gets
		RenderTarget*			GetDiffuseRT				()						{ return m_DiffuseRT; }
		RenderTarget*			GetNormalRT					()						{ return m_NormalRT; }
		RenderTarget*			GetDepthRT					()						{ return m_DepthRT; }
		
		//Framework Methods

		//Drawable Game Component Override methods
		void					Initialize					();
		void					LoadContent					();
		void					Update						(const TimerParams& timerParams);
		void					Render						(const TimerParams& timerParams);
		void					OnLostDevice				();
		void					OnResetDevice				();
};

#endif
