
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
			XE_SKY_SPHERE_EFFECT = 0,
			XE_SKY_SPHERE_DEFERRED_EFFECT
		};

	private:
		//Variables
		EffectParameter*		m_WorldViewProjection;
		EffectParameter*		m_SkyTexture;

		//Virtual Framework Methods
		XEResult				GetParameters				();

	public:
		//Constructor Destructor.
		SkySphereMaterial();
		virtual ~SkySphereMaterial();

		//Framewrok Methods
		XEResult				ActivateTechnique			(SkySphereTechnique technique);
		XEResult				SetWorldViewProjection		(const XMFLOAT4X4& worldViewProjection);
		XEResult				SetSkyTexture				(const TextureCube* skyTexture);

};

#endif

