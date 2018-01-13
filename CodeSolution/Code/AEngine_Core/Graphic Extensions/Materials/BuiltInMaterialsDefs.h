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

#ifndef _BUILT_IN_MATERIALS_DEFS_H
#define _BUILT_IN_MATERIALS_DEFS_H

/**********************
*   System Includes   *
***********************/

/*************************
*   3rd Party Includes   *
**************************/

/***************************
*   Game Engine Includes   *
****************************/
#include "Base\Base.h"
#include "Base\AEObject.h"

/********************
*   Forward Decls   *
*********************/
class SimpleBuffer;
class GraphicDevice;
class ConstantBuffer;
class StructuredBuffer;

/************
*   Using   *
*************/

/**************
*   Defines   *
***************/

/****************
*   Constants   *
*****************/

/************
*   Enums   *
*************/

/***************
*   Typedefs   *
****************/

/***********************
*   Global Functions   *
************************/
namespace AEBuiltInMaterialsHelpers
{
	/// <summary>
	/// Helper Function that Creates a Constant Buffer as based in the shader CB: '_AE_CB_World_View_Proj'
	/// </summary>
	/// <remarks>
	/// Constant Buffer will be like this one:
	///
	///		cbuffer _AE_CB_World_View_Proj : register(b0)
	///		{
	///			matrix _AE_World		: packoffset(c0);
	///			matrix _AE_View			: packoffset(c4);
	///			matrix _AE_PROJection	: packoffset(c8);
	///		};
	///
	/// </reamrks>
	/// <param name="graphicDevice">Graphic Device constant buffer will be associated with</param>
	/// <param name="cb">If successful new constant buffer will be assigned to this parameter</param>
	/// <returns>AEResult::OK if successful</returns>
	extern AEResult BuildCBWVP(GraphicDevice* graphicDevice, ConstantBuffer** cb);

	/// <summary>
	/// Helper Function that Creates a Constant Buffer as based in the shader CB: '_AE_CB_View_Proj'
	/// </summary>
	/// <remarks>
	/// Constant Buffer will be like this one:
	///
	///		cbuffer _AE_CB_View_Proj : register(b0)
	///		{
	///			matrix _AE_View			: packoffset(c0);
	///			matrix _AE_PROJection	: packoffset(c4);
	///		};
	///
	/// </reamrks>
	/// <param name="graphicDevice">Graphic Device constant buffer will be associated with</param>
	/// <param name="cb">If successful new constant buffer will be assigned to this parameter</param>
	/// <returns>AEResult::OK if successful</returns>
	extern AEResult BuildCBVP(GraphicDevice* graphicDevice, ConstantBuffer** cb);

	/// <summary>
	/// Helper Function that Creates a Constant Buffer as based in the shader CB: '_AE_CB_Bones'
	/// </summary>
	/// <remarks>
	/// Constant Buffer will be like this one:
	///
	///		cbuffer _AE_CB_Bones : register(b1)
	///		{
	///			matrix _AE_BoneTransforms[MAX_BONES] : packoffset(c0);
	///		};
	///
	/// </reamrks>
	/// <param name="graphicDevice">Graphic Device constant buffer will be associated with</param>
	/// <param name="cb">If successful new constant buffer will be assigned to this parameter</param>
	/// <returns>AEResult::OK if successful</returns>
	extern AEResult BuildCBBones(GraphicDevice* graphicDevice, ConstantBuffer** cb);

	/// <summary>
	/// Helper Function that Creates a Constant Buffer as based in the shader CB: '_AE_CB_Color'
	/// </summary>
	/// <remarks>
	/// Constant Buffer will be like this one:
	///
	///		cbuffer _AE_CB_Color : register(b0)
	///		{
	///			float4 u_Color : packoffset(c0);
	///		};
	///
	/// </reamrks>
	/// <param name="graphicDevice">Graphic Device constant buffer will be associated with</param>
	/// <param name="cb">If successful new constant buffer will be assigned to this parameter</param>
	/// <returns>AEResult::OK if successful</returns>
	extern AEResult BuildCBColor(GraphicDevice* graphicDevice, ConstantBuffer** cb);

	/// <summary>
	/// Helper Function that Creates a Constant Buffer as based in the shader CB: '_AE_CB_HalfPixel'
	/// </summary>
	/// <remarks>
	/// Constant Buffer will be like this one:
	///
	///		cbuffer _AE_CB_HalfPixel : register(b0)
	///		{
	///			float2 _AE_HalfPixel	: packoffset(c0);
	///		};
	///
	/// </reamrks>
	/// <param name="graphicDevice">Graphic Device constant buffer will be associated with</param>
	/// <param name="cb">If successful new constant buffer will be assigned to this parameter</param>
	/// <returns>AEResult::OK if successful</returns>
	extern AEResult BuildCBHalfPixel(GraphicDevice* graphicDevice, ConstantBuffer** cb);

	/// <summary>
	/// Helper Function that Creates a Constant Buffer as based in the shader CB: '_AE_CB_FPR_LightCulling'
	/// </summary>
	/// <remarks>
	/// Constant Buffer will be like this one:
	///
	///		cbuffer _AE_CB_FPR_LightCulling : register(b0)
	///		{
	///			matrix	_AE_View				: packoffset(c0);
	///			matrix	_AE_InvProjection		: packoffset(c4);
	///		
	///			uint	_AE_WindowHeight		: packoffset(c8.x);
	///			uint	_AE_WindowWidth			: packoffset(c8.y);
	///			uint	_AE_NumLights			: packoffset(c8.z);
	///		};
	///
	/// </reamrks>
	/// <param name="graphicDevice">Graphic Device constant buffer will be associated with</param>
	/// <param name="cb">If successful new constant buffer will be assigned to this parameter</param>
	/// <returns>AEResult::OK if successful</returns>
	extern AEResult BuildCBFPRLightCulling(GraphicDevice* graphicDevice, ConstantBuffer** cb);

	/// <summary>
	/// Helper Function that Creates a Constant Buffer as based in the shader CB: '_AE_CB_FPR_Lights'
	/// </summary>
	/// <remarks>
	/// Constant Buffer will be like this one:
	///
	///		cbuffer _AE_CB_FPR_Lights : register(b0)
	///		{
	///			float3	_AE_CameraPos			: packoffset(c0);
	///			uint	_AE_WindowWidth			: packoffset(c0.w);
	///		};
	///
	/// </reamrks>
	/// <param name="graphicDevice">Graphic Device constant buffer will be associated with</param>
	/// <param name="cb">If successful new constant buffer will be assigned to this parameter</param>
	/// <returns>AEResult::OK if successful</returns>
	extern AEResult BuildCBFPRLights(GraphicDevice* graphicDevice, ConstantBuffer** cb);

	/// <summary>
	/// Helper Function that Creates a Structured Buffer as based in the shader CB: '_AE_LightBuffer'
	/// </summary>
	/// <remarks>
	/// Structured Buffer will be like this one:
	///
	///		StructuredBuffer<Light> _AE_LightBuffer : register(t0);
	///
	/// </reamrks>
	/// <param name="graphicDevice">Graphic Device constant buffer will be associated with</param>
	/// <param name="sb">If successful new structured buffer will be assigned to this parameter</param>
	/// <returns>AEResult::OK if successful</returns>
	extern AEResult BuildBufferLightBuffer(GraphicDevice* graphicDevice, StructuredBuffer** sb);

	/// <summary>
	/// Helper Function that Creates a Simple Buffer as based in the shader CB: '_AE_PerTileLightIndexBuffer'
	/// </summary>
	/// <remarks>
	/// Simple Buffer will be like this one:
	///
	///		RWBuffer<uint> _AE_PerTileLightIndexBuffer : register(u0);
	///
	/// </reamrks>
	/// <param name="graphicDevice">Graphic Device constant buffer will be associated with</param>
	/// <param name="sb">If successful new Simple buffer will be assigned to this parameter</param>
	/// <returns>AEResult::OK if successful</returns>
	extern AEResult BuildBufferPerTileLightIndexBuffer(GraphicDevice* graphicDevice, SimpleBuffer** sb);
};

#endif