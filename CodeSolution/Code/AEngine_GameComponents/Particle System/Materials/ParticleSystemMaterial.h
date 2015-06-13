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
		AEResult				GetParameters				();

	public:
		//Constructor Destructor.
		ParticleSystemMaterial();
		virtual ~ParticleSystemMaterial();

		//Framewrok Methods
		AEResult				SetView						(const XMFLOAT4X4& view);
		AEResult				SetProjection				(const XMFLOAT4X4& projection);
		AEResult				SetViewportScale			(const XMFLOAT2& viewportScale);
		AEResult				SetCurrentTime				(float currentTime);
		AEResult				SetDuration					(float duration);
		AEResult				SetDurationRandomness		(float durationRandomness);
		AEResult				SetGravity					(const XMFLOAT3& gravity);
		AEResult				SetEndVelocity				(float endVelocity);
		AEResult				SetMinColor					(const XMCOLOR& color);
		AEResult				SetMaxColor					(const XMCOLOR& color);
		AEResult				SetRotateSpeed				(const XMFLOAT2& rotateSpeed);
		AEResult				SetStartSize				(const XMFLOAT2& startSize);
		AEResult				SetEndSize					(const XMFLOAT2& endSize);
		AEResult				SetParticleTexture			(const Texture2D* texture);

};

#endif

