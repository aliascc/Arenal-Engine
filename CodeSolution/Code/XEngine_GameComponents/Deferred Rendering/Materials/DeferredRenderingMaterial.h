/********************************************************
*
* Author: Carlos Chacón N.
*
* Copyright 2012-2015
*
*********************************************************/

#pragma once

#ifndef _DEFERRED_RENDERING_MATERIAL_H
#define _DEFERRED_RENDERING_MATERIAL_H

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
class EffectParameter;
class RenderTarget;

/*****************
*   Class Decl   *
******************/
class DeferredRenderingMaterial sealed : public IMaterial
{
	public:
		enum DeferredRenderingTechnique
		{
			XE_DEFREND_DFCOMBINE,
			XE_DEFREND_DFSKY_TECHNIQUE
		};

	private:
		//Variables
		EffectParameter*	m_HalfPixel;
		EffectParameter*	m_DepthTexture;
		EffectParameter*	m_LightTexture;
		EffectParameter*	m_DiffuseTexture;

		//Virtual Framework Methods
		XEResult			GetParameters			();

	public:
		//Constructor Destructor.
		DeferredRenderingMaterial();
		virtual ~DeferredRenderingMaterial();

		//Framewrok Methods
		XEResult			ActivateTechnique		(DeferredRenderingTechnique technique);
		XEResult			SetHalfPixel			(XMFLOAT2& halfpixel);
		XEResult			SetDepthTexture			(RenderTarget* depthTexture);
		XEResult			SetLightTexture			(RenderTarget* lightTexture);
		XEResult			SetDiffuseTexture		(RenderTarget* diffuseTexture);
};

#endif

