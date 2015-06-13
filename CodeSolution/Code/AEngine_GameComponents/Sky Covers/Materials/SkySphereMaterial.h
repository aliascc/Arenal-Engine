/********************************************************
*
* Author: Carlos Chacón N.
*
* Copyright 2012-2015
*
*********************************************************/

#pragma once

#ifndef _SKY_SPHERE_MATERIAL_H
#define _SKY_SPHERE_MATERIAL_H

/**********************
*   System Includes   *
***********************/
#include <DirectXMath.h>

/***************************
*   Game Engine Includes   *
****************************/
#include "Effects\Materials\IMaterial.h"

/********************************
*		Using					*
*********************************/
using namespace DirectX;

/********************
*   Forward Decls   *
*********************/
class TextureCube;
class EffectParameter;

/*****************
*   Class Decl   *
******************/
class SkySphereMaterial sealed : public IMaterial
{
	public:
		enum SkySphereTechnique
		{
			AE_SKY_SPHERE_EFFECT = 0,
			AE_SKY_SPHERE_DEFERRED_EFFECT
		};

	private:
		//Variables
		EffectParameter*		m_WorldViewProjection;
		EffectParameter*		m_SkyTexture;

		//Virtual Framework Methods
		AEResult				GetParameters				();

	public:
		//Constructor Destructor.
		SkySphereMaterial();
		virtual ~SkySphereMaterial();

		//Framewrok Methods
		AEResult				ActivateTechnique			(SkySphereTechnique technique);
		AEResult				SetWorldViewProjection		(const XMFLOAT4X4& worldViewProjection);
		AEResult				SetSkyTexture				(const TextureCube* skyTexture);

};

#endif

