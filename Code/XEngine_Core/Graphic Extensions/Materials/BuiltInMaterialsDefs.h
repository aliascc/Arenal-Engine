/********************************************************
*
* Author: Carlos Chacón N.
*
* Created: 2/1/2013
*
* Desc:
*
*
* Copyright 2012
*
*********************************************************/
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
#include "Base\XEObject.h"

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
namespace XEBuiltInMaterialsHelpers
{
	/// <summary>
	/// Helper Function that Creates a Constant Buffer as based in the shader CB: '_XE_CB_World_View_Proj'
	/// </summary>
	/// <remarks>
	/// Constant Buffer will be like this one:
	///
	///		cbuffer _XE_CB_World_View_Proj : register(b0)
	///		{
	///			matrix _XE_World		: packoffset(c0);
	///			matrix _XE_View			: packoffset(c4);
	///			matrix _XE_Projection	: packoffset(c8);
	///		};
	///
	/// </reamrks>
	/// <param name="graphicDevice">Graphic Device constant buffer will be associated with</param>
	/// <param name="cb">If successful new constant buffer will be assigned to this parameter</param>
	/// <returns>XEResult::OK if successful</returns>
	extern XEResult BuildCBWVP(GraphicDevice* graphicDevice, ConstantBuffer** cb);

	/// <summary>
	/// Helper Function that Creates a Constant Buffer as based in the shader CB: '_XE_CB_View_Proj'
	/// </summary>
	/// <remarks>
	/// Constant Buffer will be like this one:
	///
	///		cbuffer _XE_CB_View_Proj : register(b0)
	///		{
	///			matrix _XE_View			: packoffset(c0);
	///			matrix _XE_Projection	: packoffset(c4);
	///		};
	///
	/// </reamrks>
	/// <param name="graphicDevice">Graphic Device constant buffer will be associated with</param>
	/// <param name="cb">If successful new constant buffer will be assigned to this parameter</param>
	/// <returns>XEResult::OK if successful</returns>
	extern XEResult BuildCBVP(GraphicDevice* graphicDevice, ConstantBuffer** cb);

	/// <summary>
	/// Helper Function that Creates a Constant Buffer as based in the shader CB: '_XE_CB_Bones'
	/// </summary>
	/// <remarks>
	/// Constant Buffer will be like this one:
	///
	///		cbuffer _XE_CB_Bones : register(b1)
	///		{
	///			matrix _XE_BoneTransforms[MAX_BONES] : packoffset(c0);
	///		};
	///
	/// </reamrks>
	/// <param name="graphicDevice">Graphic Device constant buffer will be associated with</param>
	/// <param name="cb">If successful new constant buffer will be assigned to this parameter</param>
	/// <returns>XEResult::OK if successful</returns>
	extern XEResult BuildCBBones(GraphicDevice* graphicDevice, ConstantBuffer** cb);

	/// <summary>
	/// Helper Function that Creates a Constant Buffer as based in the shader CB: '_XE_CB_Color'
	/// </summary>
	/// <remarks>
	/// Constant Buffer will be like this one:
	///
	///		cbuffer _XE_CB_Color : register(b0)
	///		{
	///			float4 u_Color : packoffset(c0);
	///		};
	///
	/// </reamrks>
	/// <param name="graphicDevice">Graphic Device constant buffer will be associated with</param>
	/// <param name="cb">If successful new constant buffer will be assigned to this parameter</param>
	/// <returns>XEResult::OK if successful</returns>
	extern XEResult BuildCBColor(GraphicDevice* graphicDevice, ConstantBuffer** cb);

	/// <summary>
	/// Helper Function that Creates a Constant Buffer as based in the shader CB: '_XE_CB_HalfPixel'
	/// </summary>
	/// <remarks>
	/// Constant Buffer will be like this one:
	///
	///		cbuffer _XE_CB_HalfPixel : register(b0)
	///		{
	///			float2 _XE_HalfPixel	: packoffset(c0);
	///		};
	///
	/// </reamrks>
	/// <param name="graphicDevice">Graphic Device constant buffer will be associated with</param>
	/// <param name="cb">If successful new constant buffer will be assigned to this parameter</param>
	/// <returns>XEResult::OK if successful</returns>
	extern XEResult BuildCBHalfPixel(GraphicDevice* graphicDevice, ConstantBuffer** cb);

	/// <summary>
	/// Helper Function that Creates a Constant Buffer as based in the shader CB: '_XE_CB_FPR_LightCulling'
	/// </summary>
	/// <remarks>
	/// Constant Buffer will be like this one:
	///
	///		cbuffer _XE_CB_FPR_LightCulling : register(b0)
	///		{
	///			matrix	_XE_View				: packoffset(c0);
	///			matrix	_XE_InvProjection		: packoffset(c4);
	///		
	///			uint	_XE_WindowHeight		: packoffset(c8.x);
	///			uint	_XE_WindowWidth			: packoffset(c8.y);
	///			uint	_XE_NumLights			: packoffset(c8.z);
	///		};
	///
	/// </reamrks>
	/// <param name="graphicDevice">Graphic Device constant buffer will be associated with</param>
	/// <param name="cb">If successful new constant buffer will be assigned to this parameter</param>
	/// <returns>XEResult::OK if successful</returns>
	extern XEResult BuildCBFPRLightCulling(GraphicDevice* graphicDevice, ConstantBuffer** cb);

	/// <summary>
	/// Helper Function that Creates a Constant Buffer as based in the shader CB: '_XE_CB_FPR_Lights'
	/// </summary>
	/// <remarks>
	/// Constant Buffer will be like this one:
	///
	///		cbuffer _XE_CB_FPR_Lights : register(b0)
	///		{
	///			float3	_XE_CameraPos			: packoffset(c0);
	///			uint	_XE_WindowWidth			: packoffset(c0.w);
	///		};
	///
	/// </reamrks>
	/// <param name="graphicDevice">Graphic Device constant buffer will be associated with</param>
	/// <param name="cb">If successful new constant buffer will be assigned to this parameter</param>
	/// <returns>XEResult::OK if successful</returns>
	extern XEResult BuildCBFPRLights(GraphicDevice* graphicDevice, ConstantBuffer** cb);

	/// <summary>
	/// Helper Function that Creates a Structured Buffer as based in the shader CB: '_XE_LightBuffer'
	/// </summary>
	/// <remarks>
	/// Structured Buffer will be like this one:
	///
	///		StructuredBuffer<Light> _XE_LightBuffer : register(t0);
	///
	/// </reamrks>
	/// <param name="graphicDevice">Graphic Device constant buffer will be associated with</param>
	/// <param name="sb">If successful new structured buffer will be assigned to this parameter</param>
	/// <returns>XEResult::OK if successful</returns>
	extern XEResult BuildBufferLightBuffer(GraphicDevice* graphicDevice, StructuredBuffer** sb);

	/// <summary>
	/// Helper Function that Creates a Simple Buffer as based in the shader CB: '_XE_PerTileLightIndexBuffer'
	/// </summary>
	/// <remarks>
	/// Simple Buffer will be like this one:
	///
	///		RWBuffer<uint> _XE_PerTileLightIndexBuffer : register(u0);
	///
	/// </reamrks>
	/// <param name="graphicDevice">Graphic Device constant buffer will be associated with</param>
	/// <param name="sb">If successful new Simple buffer will be assigned to this parameter</param>
	/// <returns>XEResult::OK if successful</returns>
	extern XEResult BuildBufferPerTileLightIndexBuffer(GraphicDevice* graphicDevice, SimpleBuffer** sb);
};

#endif