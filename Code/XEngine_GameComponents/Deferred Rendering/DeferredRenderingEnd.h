/********************************************************
*
* Author: Carlos Chacón N.
*
* Created: 4/30/2012
*
* Desc:
*
*
* Copyright 2012
*
*********************************************************/

#pragma once

#ifndef _DEFERRED_RENDERING_END_H
#define _DEFERRED_RENDERING_END_H

/**********************
*   System Includes   *
***********************/
#include <string>

/***************************
*   Game Engine Includes   *
****************************/
#include "GameUtils\DrawableGameComponent.h"
#include "Base\Base.h"
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
class DeferredRendering;
class DeferredRenderingMaterial;

/*****************
*   Class Decl   *
******************/
class DeferredRenderingEnd : public DrawableGameComponent
{
	protected:
		
		/****************************
		 *   Protected Variables    *
		 ****************************/
#pragma region Protected Variables
	
		///<summary>
		///Deferred Rendering Name
		///</summary>
		const std::string m_DeferredServiceName;
	
		///<summary>
		///Service Name of Deferred Rendering End
		///</summary>
		const std::string m_ServiceName;
	
		///<summary>
		///Deferred Rendering Service
		///</summary>
		DeferredRendering* m_DeferredRendering;
	
		///<summary>
		///Deferred Rendering Light Render Target
		///</summary>
		RenderTarget* m_LightRT;
	
		///<summary>
		///Deferred Rendering Final Render Target
		///</summary>
		RenderTarget* m_FinalRT;
	
		///<summary>
		///Deferred Rendering End Settings
		///</summary>
		DeferredRenderingEndSettings m_Settings;
	
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
		///<returns>XEResult::Ok if Initialize Constant Effect Params succeeded</returns>
		XEResult InitEffectParams();

		///<summary>
		///Sets per Frame Effect Params
		///</summary>
		///<returns>XEResult::Ok if setting Effect Params succeeded</returns>
		XEResult SetFrameEffectParams();
		
		///<summary>
		///Draw Sky Cover on Deferred RT
		///</summary>
		///<returns>XEResult::Ok if Drawing of the Lights succeeded</returns>
		XEResult DrawSkyCover();
		
		///<summary>
		///Draw Lights on Deferred RT
		///</summary>
		///<returns>XEResult::Ok if Drawing of the Lights succeeded</returns>
		XEResult DrawLights();
		
		///<summary>
		///Combines the Light RT and Diffuse RT to a Final Target
		///</summary>
		///<returns>XEResult::Ok if Drawing of the Lights succeeded</returns>
		XEResult DrawCombine();

#pragma endregion
		
	public:
		//Constructor Destructor.
		DeferredRenderingEnd(GameApp* gameApp, const DeferredRenderingEndSettings& settings, uint32_t callOrder = 50, const std::string& serviceName = "DeferredRenderingEnd", const std::string& deferredServiceName = "DeferredRendering", const std::string& gameComponentName = "Deferred Rendering End");
		virtual ~DeferredRenderingEnd();
		
		//Gets
		RenderTarget* GetFinalRT() const 
		{ 
			return m_FinalRT;
		}

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
