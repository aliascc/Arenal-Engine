/********************************************************
*
* Author: Carlos Chacón N.
*
* Copyright 2012-2015
*
*********************************************************/

#pragma once

#ifndef _PARTICLE_SYSTEM_MATERIAL_H
#define _PARTICLE_SYSTEM_MATERIAL_H

/**********************
*   System Includes   *
***********************/
#include <DirectXMath.h>
#include <DirectXPackedVector.h>

/***************************
*   Game Engine Includes   *
****************************/
#include "Effects\Materials\IMaterial.h"

/********************************
*		Using					*
*********************************/
using namespace DirectX;
using namespace PackedVector;

/********************
*   Forward Decls   *
*********************/
class Texture2D;
class EffectParameter;

/*****************
*   Class Decl   *
******************/
class ParticleSystemMaterial sealed : public IMaterial
{
	private:
		//Variables
		EffectParameter*		m_ViewParam;
		EffectParameter*		m_ProjectionParam;
		EffectParameter*		m_ViewportScaleParam;
		EffectParameter*		m_CurrentTimeParam;
		EffectParameter*		m_DurationParam;
		EffectParameter*		m_DurationRandomnessParam;
		EffectParameter*		m_GravityParam;
		EffectParameter*		m_EndVelocityParam;
		EffectParameter*		m_MinColorParam;
		EffectParameter*		m_MaxColorParam;
		EffectParameter*		m_RotateSpeedParam;
		EffectParameter*		m_StartSizeParam;
		EffectParameter*		m_EndSizeParam;
		EffectParameter*		m_ParticleTextureParam;

		//Virtual Framework Methods
		XEResult				GetParameters				();

	public:
		//Constructor Destructor.
		ParticleSystemMaterial();
		virtual ~ParticleSystemMaterial();

		//Framewrok Methods
		XEResult				SetView						(const XMFLOAT4X4& view);
		XEResult				SetProjection				(const XMFLOAT4X4& projection);
		XEResult				SetViewportScale			(const XMFLOAT2& viewportScale);
		XEResult				SetCurrentTime				(float currentTime);
		XEResult				SetDuration					(float duration);
		XEResult				SetDurationRandomness		(float durationRandomness);
		XEResult				SetGravity					(const XMFLOAT3& gravity);
		XEResult				SetEndVelocity				(float endVelocity);
		XEResult				SetMinColor					(const XMCOLOR& color);
		XEResult				SetMaxColor					(const XMCOLOR& color);
		XEResult				SetRotateSpeed				(const XMFLOAT2& rotateSpeed);
		XEResult				SetStartSize				(const XMFLOAT2& startSize);
		XEResult				SetEndSize					(const XMFLOAT2& endSize);
		XEResult				SetParticleTexture			(const Texture2D* texture);

};

#endif

