/********************************************************
*
* Author: Carlos Chacón N.
*
* Created: 3/22/2012
*
* Desc:
*
*
* Copyright 2012
*
*********************************************************/

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