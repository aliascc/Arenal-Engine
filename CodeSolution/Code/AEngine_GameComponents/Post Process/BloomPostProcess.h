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