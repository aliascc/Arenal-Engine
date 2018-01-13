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

