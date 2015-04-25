/********************************************************
*
* Author: Carlos Chacón N.
*
* Copyright 2012-2015
*
*********************************************************/

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
		///<returns>XEResult::Ok if Initialize Constant Effect Params succeeded</returns>
		XEResult InitEffectParams();

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
