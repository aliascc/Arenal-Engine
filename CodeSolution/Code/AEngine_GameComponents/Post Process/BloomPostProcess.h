/********************************************************
*
* Author: Carlos Chacón N.
*
* Copyright 2012-2015
*
*********************************************************/

#pragma once

#ifndef _BLOOM_POST_PROCESS_H
#define _BLOOM_POST_PROCESS_H

/**********************
*   System Includes   *
***********************/
#include <stdint.h>
#include <DirectXMath.h>

/***************************
*   Game Engine Includes   *
****************************/
#include "GameUtils\DrawableGameComponent.h"
#include "BloomPostProcessDefs.h"
#include "Base\Base.h"

/************
*   Using   *
*************/
using namespace DirectX;

/********************
*   Forward Decls   *
*********************/
struct TimerParams;
class GameApp;
class RenderTarget;
class GraphicDevice;
class BloomPostProcessMaterial;

/*****************
*   Class Decl   *
******************/
class BloomPostProcess : public DrawableGameComponent
{
	protected:
		//Variables
		RenderTarget*				m_SceneRT;
		RenderTarget*				m_RT1;
		RenderTarget*				m_RT2;
		BloomPostProcessMaterial*	m_BloomEffect;
		BloomPostProcessSettings	m_Settings;

		//Methods
		void						SetBlurParameters(float dx, float dy);
		AEResult					RenderBloomExtract();
		AEResult					RenderBloomGaussianBlur();
		AEResult					RenderBloomCombine();

	public:
		//Constructor and Destructor
		BloomPostProcess(GameApp* gameApp, const std::string& gameComponentName = "Bloom Post Process");
		BloomPostProcess(GameApp* gameApp, const BloomPostProcessSettings& settings, const std::string& gameComponentName = "Bloom Post Process");
		virtual ~BloomPostProcess();

		//Get 
		BloomPostProcessSettings	GetSettings					() const										{ return m_Settings; }
		
		//Set 
		void						SetSettings					(const BloomPostProcessSettings& settings)		{ m_Settings = settings; }

		//Drawable Game Component Override methods
		void						Initialize					();
		void						LoadContent					();
		void						Update						(const TimerParams& timerParams);
		void						Render						(const TimerParams& timerParams);
		void						OnLostDevice				();
		void						OnResetDevice				();
};

#endif