/********************************************************
*
* Author: Carlos Chacón N.
*
* Copyright 2012-2015
*
*********************************************************/

#pragma once

#ifndef _SHADER_DEFS_H
#define _SHADER_DEFS_H

/**********************
*   System Includes   *
***********************/
#include <stdint.h>
#include <d3d11_1.h>

/*************************
*   3rd Party Includes   *
**************************/

/***************************
*   Game Engine Includes   *
****************************/
#include "Base\Base.h"
#include "Base\XEObject.h"

/************
*   Using   *
*************/

/********************
*   Forward Decls   *
*********************/
class Logger;
class LocalizationManager;

/**************
*   Defines   *
***************/
#define ELEMENT_SIZE_SIMPLE_BUFFER							4

#define XE_VARIANCE_SHADOW_MAT_NAME							L"XE Variance Shadow Material"
#define XE_VARIANCE_SHADOW_MAT_VS_NAME						L"_XE_VarianceShadowMaterialVS"
#define XE_VARIANCE_SHADOW_MAT_PS_NAME						L"_XE_VarianceShadowMaterialPS"
#define XE_VARIANCE_SKINNING_SHADOW_MAT_NAME				L"XE Variance Skinning Shadow Material"
#define XE_VARIANCE_SKINNING_SHADOW_MAT_VS_NAME				L"_XE_VarianceSkinningShadowMaterialVS"

#define XE_BASIC_COLOR_MAT_NAME								L"XE Basic Color Material"
#define XE_BASIC_COLOR_MAT_VS_NAME							L"_XE_BasicColorMaterialVS"
#define XE_BASIC_COLOR_MAT_PS_NAME							L"_XE_BasicColorMaterialPS"

#define XE_BASIC_LINE_MAT_NAME								L"XE Basic Line Material"
#define XE_BASIC_LINE_MAT_VS_NAME							L"_XE_BasicLineMaterialVS"
#define XE_BASIC_LINE_MAT_PS_NAME							L"_XE_BasicLineMaterialPS"

#define XE_QUAD_MAT_VS_NAME									L"_XE_QuadMaterialVS"

#define XE_QUAD_COLOR_MAT_NAME								L"XE Quad Color Material"
#define XE_QUAD_COLOR_MAT_PS_NAME							L"_XE_QuadColorMaterialPS"

#define XE_QUAD_TEXTURE_MAT_NAME							L"XE Quad Texture Material"
#define XE_QUAD_TEXTURE_MAT_PS_NAME							L"_XE_QuadTextureMaterialPS"

#define XE_DIFFUSE_TEXTURE_BASIC_MAT_NAME					L"XE Diffuse Texture Basic Material"
#define XE_DIFFUSE_TEXTURE_BASIC_MAT_VS_NAME				L"_XE_DiffuseTextureBasicMaterialVS"
#define XE_DIFFUSE_TEXTURE_BASIC_MAT_PS_NAME				L"_XE_DiffuseTextureBasicMaterialPS"

#define XE_DIFFUSE_TEXTURE_MAT_NAME							L"XE Diffuse Texture Material"
#define XE_DIFFUSE_TEXTURE_MAT_VS_NAME						L"_XE_DiffuseTextureMaterialVS"
#define XE_DIFFUSE_TEXTURE_MAT_PS_NAME						L"_XE_DiffuseTextureMaterialPS"

#define XE_SKINNING_MAT_NAME								L"XE Skinning Material"
#define XE_SKINNING_MAT_VS_NAME								L"_XE_SkinningMaterialVS"
#define XE_SKINNING_MAT_PS_NAME								L"_XE_SkinningMaterialPS"

#define XE_FORWARD_PLUS_Z_PRE_PASS_MAT_NAME					L"XE Forward Plus Z Pre Pass"
#define XE_FORWARD_PLUS_Z_PRE_PASS_MAT_VS_NAME				L"_XE_ForwardPlusZPrePassVS"
#define XE_FORWARD_PLUS_Z_PRE_PASS_SKINNING_MAT_VS_NAME		L"_XE_ForwardPlusZPrePassSkinningVS"

#define XE_FORWARD_PLUS_LIGHT_CULLING_MAT_NAME				L"XE Forward Plus Light Culling Material"
#define XE_FORWARD_PLUS_LIGHT_CULLING_MAT_CS_NAME			L"_XE_ForwardPlusLightCullingCS"

#define XE_CB_VIEW_PROJ_NAME								L"_XE_CB_View_Proj"
#define XE_CB_WORLD_VIEW_PROJ_NAME							L"_XE_CB_World_View_Proj"
#define XE_CB_BONES_NAME									L"_XE_CB_Bones"
#define XE_CB_COLOR_NAME									L"_XE_CB_Color"
#define XE_CB_HALF_PIXEL_NAME								L"_XE_CB_HalfPixel"
#define XE_CB_FPR_LIGHT_CULLING_NAME						L"_XE_CB_FPR_LightCulling"
#define XE_CB_FPR_LIGHTS_NAME								L"_XE_CB_FPR_Lights"

#define XE_CB_WORLD_VAR_NAME								L"_XE_World"
#define XE_CB_VIEW_VAR_NAME									L"_XE_View"
#define XE_CB_PROJECTION_VAR_NAME							L"_XE_Projection"
#define XE_CB_INV_PROJECTION_VAR_NAME						L"_XE_InvProjection"
#define XE_CB_BONE_TRANSFORMS_VAR_NAME						L"_XE_BoneTransforms"
#define XE_CB_COLOR_VAR_NAME								L"u_Color"
#define XE_CB_HALF_PIXEL_VAR_NAME							L"_XE_HalfPixel"
#define XE_CB_WINDOW_HEIGHT_VAR_NAME						L"_XE_WindowHeight"
#define XE_CB_WINDOW_WIDTH_VAR_NAME							L"_XE_WindowWidth"
#define XE_CB_NUM_LIGHTS_VAR_NAME							L"_XE_NumLights"
#define XE_CB_CAMERA_POS_VAR_NAME							L"_XE_CameraPos"
#define XE_CB_BONE_TRANSFORMS_VAR_NAME						L"_XE_BoneTransforms"

#define XE_TX_DEPTH_TEXTURE_CULL_NAME						L"_XE_DepthTexture"
#define XE_TX_DIFFUSE_TEXTURE_NAME							L"DiffuseTexture"

#define XE_SAM_DIFFUSE_TEXTURE_SAMPLER_NAME					L"_XE_DiffuseTextureSampler"
#define XE_SAM_SHADOW_TEXTURE_SAMPLER_NAME					L"_XE_ShadowTextureSampler"

#define XE_TXA_SPOT_LIGHT_SHADOW_TEXTURE_ARRAY_NAME			L"_XE_SpotLightShadowTextureArray"
#define XE_TXA_DIR_LIGHT_SHADOW_TEXTURE_ARRAY_NAME			L"_XE_DirLightShadowTextureArray"

#define XE_BF_LIGHT_BUFFER_NAME								L"_XE_LightBuffer"
#define XE_BF_SHADOW_SPOT_LIGHT_INFO_BUFFER_NAME			L"_XE_ShadowSpotLightInfoBuffer"
#define XE_BF_PER_TILE_LIGHT_INDEX_BUFFER_NAME				L"_XE_PerTileLightIndexBuffer"
#define XE_BF_SHADOW_DIR_LIGHT_INFO_BUFFER_NAME				L"_XE_ShadowDirLightInfoBuffer"

#define XE_BI_FPR_LIGHTS_CB_BIND_IDX						0
#define XE_BI_SHADOW_TEXTURE_SAMPLER_BIND_IDX				0
#define XE_BI_DEPTH_TEXTURE_CULL_BIND_IDX					0
#define XE_BI_LIGHT_BUFFER_CULL_BIND_IDX					0
#define XE_BI_PER_TILE_LIGHT_INDEX_BUFFER_CULL_BIND_IDX		0
#define XE_BI_LIGHT_BUFFER_DRAW_BIND_IDX					0
#define XE_BI_PER_TILE_LIGHT_INDEX_BUFFER_DRAW_BIND_IDX		1
#define XE_BI_SHADOW_SPOT_LIGHT_INFO_BUFFER_BIND_IDX		2
#define XE_BI_SHADOW_SPOT_LIGHT_TEXT_ARRAY_BUFFER_BIND_IDX	3
#define XE_BI_SHADOW_DIR_LIGHT_TEXT_ARRAY_BUFFER_BIND_IDX	4
#define XE_BI_SHADOW_DIR_LIGHT_INFO_BUFFER_BIND_IDX			5

/****************
*   Constants   *
*****************/

/************
*   Enums   *
*************/
enum class ShaderType : uint32_t
{
	VertexShader = 0,
	PixelShader,
	GeometryShader,
	HullShader,
	ComputeShader,
	DomainShader
};

enum class ShaderModel : uint32_t
{
	SM_3_0 = 0,
	SM_4_0,
	SM_4_1,
	SM_5_0,
};

enum class ShaderVariableClass : uint32_t
{
	Scalar = 0,
	Vector,
	Matrix,
	Structure
};

enum class ShaderVariableType : uint32_t
{
	Boolean = 0,
	Int,
	UInt,
	UInt8,
	Float,
	Double,
	Void,
	Texture2D,
	Texture3D,
	TextureCube,
	Sampler
};

enum class ShaderFilter : uint32_t
{
	MinMagMipPoint = 0,
	MinMagPoint_MipLinear,
	MinPoint_MagLinear_MipPoint,
	MinPoint_MagMipLinear,
	MinLinear_MagMipPoint,
	MinLinear_MagPoint_MipLinear,
	MinMagLinear_MipPoint,
	MinMagMipLinear,
	Anisotropic,

	Comp_MinMagMipPoint,
	Comp_MinMagPoint_MipLinear,
	Comp_MinPoint_MagLinear_MipPoint,
	Comp_MinPoint_MagMipLinear,
	Comp_MinLinear_MagMipPoint,
	Comp_MinLinear_MagPoint_MipLinear,
	Comp_MinMagLinear_MipPoint,
	Comp_MinMagMipLinear,
	Comp_Anisotropic,

	Minimum_MinMagMipPoint,
	Minimum_MinMagPoint_MipLinear,
	Minimum_MinPoint_MagLinear_MipPoint,
	Minimum_MinPoint_MagMipLinear,
	Minimum_MinLinear_MagMipPoint,
	Minimum_MinLinear_MagPoint_MipLinear,
	Minimum_MinMagLinear_MipPoint,
	Minimum_MinMagMipLinear,
	Minimum_Anisotropic,

	Maximum_MinMagMipPoint,
	Maximum_MinMagPoint_MipLinear,
	Maximum_MinPoint_MagLinear_MipPoint,
	Maximum_MinPoint_MagMipLinear,
	Maximum_MinLinear_MagMipPoint,
	Maximum_MinLinear_MagPoint_MipLinear,
	Maximum_MinMagLinear_MipPoint,
	Maximum_MinMagMipLinear,
	Maximum_Anisotropic
};

enum class ShaderTextureAddressMode : uint32_t
{
	Wrap = 0,
	Mirror,
	Clamp,
	Border,
	MirrorOnce
};

enum class ShaderCompFunct : uint32_t
{
	Never = 0,
	Less,
	Equal,
	LessEqual,
	Greater,
	NotEqual,
	GreaterEqual,
	Always
};

/***********************
*   Helper Functions   *
************************/

namespace XEShaderHelper
{
	extern D3D11_FILTER GetDXFilter(ShaderFilter shaderFilter);

	extern D3D11_TEXTURE_ADDRESS_MODE GetDXTextureAddressMode(ShaderTextureAddressMode textureAddressMode);

	extern D3D11_COMPARISON_FUNC GetDXCompFunct(ShaderCompFunct compFunct);

	extern uint32_t GetShaderVariableTypeSize(ShaderVariableType svType);

	extern bool IsShaderVariableTypeScalar(ShaderVariableType svType);
};

#endif