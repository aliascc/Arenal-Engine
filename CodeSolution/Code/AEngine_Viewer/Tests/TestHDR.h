/********************************************************
*
* Author: Carlos Chacón N.
*
* Copyright 2012-2015
*
*********************************************************/

#pragma once

#ifndef TEST_HDR_H
#define TEST_HDR_H

/**********************
*   System Includes   *
***********************/
#include <string>
#include <DirectXMath.h>
#include <DirectXPackedVector.h>

/***************************
*   Game Engine Includes   *
****************************/
#include "GameUtils\DrawableGameComponent.h"
#include "Base\Base.h"
#include "Shapes\SphereShape.h"
#include "Effects\Materials\BasicColorMaterial.h"
#include "Effects\Materials\BasicDeferredRenderingDraw.h"
#include "Effects\Materials\BasicTextureMaterial.h"
#include "Camera\Camera.h"

/********************************
*		Using					*
*********************************/
using namespace DirectX;
using namespace PackedVector;

/********************
*   Forward Decls   *
*********************/
struct TimerParams;
class GameApp;
class Model;

/*****************
*   Class Decl   *
******************/
class TestHDR : public DrawableGameComponent
{
	protected:
		SphereShape*		m_SphereShape;
		BasicColorMaterial*	m_BasicColorMaterial;
		BasicDeferredRenderingDraw* m_BasicDeferredRenderingDraw;
		BasicTextureMaterial* m_BasicTextureMaterial;
		Camera*				m_Camera;
		XMFLOAT4X4			m_World;
		XMFLOAT4X4			m_WVP;
		Model*				m_Room;
		Model*				m_Tea;

	public:
		//Constructor Destructor.
		TestHDR(GameApp* gameApp);
		virtual ~TestHDR();

		//Framework Methods
		void DrawModel(Model* model);

		//Drawable Game Component Override methods
		void					Initialize					();
		void					LoadContent					();
		void					Update						(const TimerParams& timerParams);
		void					Render						(const TimerParams& timerParams);
		void					OnLostDevice				();
		void					OnResetDevice				();
};

#endif
