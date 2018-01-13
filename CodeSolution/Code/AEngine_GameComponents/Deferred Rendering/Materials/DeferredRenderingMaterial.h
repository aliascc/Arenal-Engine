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

