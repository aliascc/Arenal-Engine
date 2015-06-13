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
			AE_DEFREND_DFCOMBINE,
			AE_DEFREND_DFSKY_TECHNIQUE
		};

	private:
		//Variables
		EffectParameter*	m_HalfPixel;
		EffectParameter*	m_DepthTexture;
		EffectParameter*	m_LightTexture;
		EffectParameter*	m_DiffuseTexture;

		//Virtual Framework Methods
		AEResult			GetParameters			();

	public:
		//Constructor Destructor.
		DeferredRenderingMaterial();
		virtual ~DeferredRenderingMaterial();

		//Framewrok Methods
		AEResult			ActivateTechnique		(DeferredRenderingTechnique technique);
		AEResult			SetHalfPixel			(XMFLOAT2& halfpixel);
		AEResult			SetDepthTexture			(RenderTarget* depthTexture);
		AEResult			SetLightTexture			(RenderTarget* lightTexture);
		AEResult			SetDiffuseTexture		(RenderTarget* diffuseTexture);
};

#endif

