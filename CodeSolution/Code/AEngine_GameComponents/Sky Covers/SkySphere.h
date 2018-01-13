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

#ifndef _SKY_SPHERE_H
#define _SKY_SPHERE_H

/**********************
*   System Includes   *
***********************/
#include <string>
#include <DirectXMath.h>

/***************************
*   Game Engine Includes   *
****************************/
#include "GameUtils\DrawableGameComponent.h"

/********************************
*		Using					*
*********************************/
using namespace DirectX;

/********************
*   Forward Decls   *
*********************/
struct TimerParams;
class GameApp;
class SphereShape;
class SkySphereMaterial;
class Camera;
class TextureCube;

/*****************
*   Class Decl   *
******************/
class SkySphere : public DrawableGameComponent
{
	protected:
		//Variables
		SphereShape*			m_SphereShape;
		SkySphereMaterial*		m_SkyEffect;
		Camera*					m_Camera;
		XMFLOAT4X4				m_WVPMatrix;
		TextureCube*			m_SkyTexture;
		std::string				m_SkyTextureFile;
		std::string				m_CameraServiceName;
		bool					m_Deferred;

		const XMFLOAT3			m_Size;


		void InitEffectParams();

	public:
		//Constructor Destructor.
		SkySphere(GameApp* gameApp, const std::string& skyTextureFile, bool deferred = false, uint32_t callOrder = 0, const XMFLOAT3& skySphereSize = XMFLOAT3(500.0f, 450.0f, 500.0f), const std::string& cameraSeviceName = "Camera", const std::string& gameComponentName = "Sky Sphere");
		virtual ~SkySphere();

		//Gets
		
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