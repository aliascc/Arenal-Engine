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
#include "Base\AEObject.h"

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
#define ELEMENT_SIZE_SIMPLE_BUFFER                          4

#define AE_VARIANCE_SHADOW_MAT_NAME                         L"AE Variance Shadow Material"
#define AE_VARIANCE_SHADOW_MAT_VS_NAME                      L"_AE_VarianceShadowMaterialVS"
#define AE_VARIANCE_SHADOW_MAT_PS_NAME                      L"_AE_VarianceShadowMaterialPS"
#define AE_VARIANCE_SKINNING_SHADOW_MAT_NAME                L"AE Variance Skinning Shadow Material"
#define AE_VARIANCE_SKINNING_SHADOW_MAT_VS_NAME             L"_AE_VarianceSkinningShadowMaterialVS"

#define AE_BASIC_COLOR_MAT_NAME                             L"AE Basic Color Material"
#define AE_BASIC_COLOR_MAT_VS_NAME                          L"_AE_BasicColorMaterialVS"
#define AE_BASIC_COLOR_MAT_PS_NAME                          L"_AE_BasicColorMaterialPS"

#define AE_BASIC_LINE_MAT_NAME                              L"AE Basic Line Material"
#define AE_BASIC_LINE_MAT_VS_NAME                           L"_AE_BasicLineMaterialVS"
#define AE_BASIC_LINE_MAT_PS_NAME                           L"_AE_BasicLineMaterialPS"

#define AE_QUAD_MAT_VS_NAME                                 L"_AE_QuadMaterialVS"

#define AE_QUAD_COLOR_MAT_NAME                              L"AE Quad Color Material"
#define AE_QUAD_COLOR_MAT_PS_NAME                           L"_AE_QuadColorMaterialPS"

#define AE_QUAD_TEXTURE_MAT_NAME                            L"AE Quad Texture Material"
#define AE_QUAD_TEXTURE_MAT_PS_NAME                         L"_AE_QuadTextureMaterialPS"

#define AE_DIFFUSE_TEXTURE_BASIC_MAT_NAME                   L"AE Diffuse Texture Basic Material"
#define AE_DIFFUSE_TEXTURE_BASIC_MAT_VS_NAME                L"_AE_DiffuseTextureBasicMaterialVS"
#define AE_DIFFUSE_TEXTURE_BASIC_MAT_PS_NAME                L"_AE_DiffuseTextureBasicMaterialPS"

#define AE_DIFFUSE_TEXTURE_MAT_NAME                         L"AE Diffuse Texture Material"
#define AE_DIFFUSE_TEXTURE_MAT_VS_NAME                      L"_AE_DiffuseTextureMaterialVS"
#define AE_DIFFUSE_TEXTURE_MAT_PS_NAME                      L"_AE_DiffuseTextureMaterialPS"

#define AE_SKINNING_MAT_NAME                                L"AE Skinning Material"
#define AE_SKINNING_MAT_VS_NAME                             L"_AE_SkinningMaterialVS"
#define AE_SKINNING_MAT_PS_NAME                             L"_AE_SkinningMaterialPS"

#define AE_FORWARD_PLUS_Z_PRE_PASS_MAT_NAME                 L"AE Forward Plus Z Pre Pass"
#define AE_FORWARD_PLUS_Z_PRE_PASS_MAT_VS_NAME              L"_AE_ForwardPlusZPrePassVS"
#define AE_FORWARD_PLUS_Z_PRE_PASS_SKINNING_MAT_VS_NAME     L"_AE_ForwardPlusZPrePassSkinningVS"

#define AE_FORWARD_PLUS_LIGHT_CULLING_MAT_NAME              L"AE Forward Plus Light Culling Material"
#define AE_FORWARD_PLUS_LIGHT_CULLING_MAT_CS_NAME           L"_AE_ForwardPlusLightCullingCS"

#define AE_CB_VIEW_PROJ_NAME                                L"_AE_CB_View_Proj"
#define AE_CB_WORLD_VIEW_PROJ_NAME                          L"_AE_CB_World_View_Proj"
#define AE_CB_BONES_NAME                                    L"_AE_CB_Bones"
#define AE_CB_COLOR_NAME                                    L"_AE_CB_Color"
#define AE_CB_HALF_PIXEL_NAME                               L"_AE_CB_HalfPixel"
#define AE_CB_FPR_LIGHT_CULLING_NAME                        L"_AE_CB_FPR_LightCulling"
#define AE_CB_FPR_LIGHTS_NAME                               L"_AE_CB_FPR_Lights"

#define AE_CB_WORLD_VAR_NAME                                L"_AE_World"
#define AE_CB_VIEW_VAR_NAME                                 L"_AE_View"
#define AE_CB_PROJECTION_VAR_NAME                           L"_AE_Projection"
#define AE_CB_INV_PROJECTION_VAR_NAME                       L"_AE_InvProjection"
#define AE_CB_BONE_TRANSFORMS_VAR_NAME                      L"_AE_BoneTransforms"
#define AE_CB_COLOR_VAR_NAME                                L"u_Color"
#define AE_CB_HALF_PIXEL_VAR_NAME                           L"_AE_HalfPixel"
#define AE_CB_WINDOW_HEIGHT_VAR_NAME                        L"_AE_WindowHeight"
#define AE_CB_WINDOW_WIDTH_VAR_NAME                         L"_AE_WindowWidth"
#define AE_CB_NUM_LIGHTS_VAR_NAME                           L"_AE_NumLights"
#define AE_CB_CAMERA_POS_VAR_NAME                           L"_AE_CameraPos"
#define AE_CB_BONE_TRANSFORMS_VAR_NAME                      L"_AE_BoneTransforms"

#define AE_TX_DEPTH_TEXTURE_CULL_NAME                       L"_AE_DepthTexture"
#define AE_TX_DIFFUSE_TEXTURE_NAME                          L"DiffuseTexture"

#define AE_SAM_DIFFUSE_TEXTURE_SAMPLER_NAME                 L"_AE_DiffuseTextureSampler"
#define AE_SAM_SHADOW_TEXTURE_SAMPLER_NAME                  L"_AE_ShadowTextureSampler"

#define AE_TXA_SPOT_LIGHT_SHADOW_TEXTURE_ARRAY_NAME         L"_AE_SpotLightShadowTextureArray"
#define AE_TXA_DIR_LIGHT_SHADOW_TEXTURE_ARRAY_NAME          L"_AE_DirLightShadowTextureArray"

#define AE_BF_LIGHT_BUFFER_NAME                             L"_AE_LightBuffer"
#define AE_BF_SHADOW_SPOT_LIGHT_INFO_BUFFER_NAME            L"_AE_ShadowSpotLightInfoBuffer"
#define AE_BF_PER_TILE_LIGHT_INDEX_BUFFER_NAME              L"_AE_PerTileLightIndexBuffer"
#define AE_BF_SHADOW_DIR_LIGHT_INFO_BUFFER_NAME             L"_AE_ShadowDirLightInfoBuffer"

#define AE_BI_FPR_LIGHTS_CB_BIND_IDX                        0
#define AE_BI_SHADOW_TEXTURE_SAMPLER_BIND_IDX               0
#define AE_BI_DEPTH_TEXTURE_CULL_BIND_IDX                   0
#define AE_BI_LIGHT_BUFFER_CULL_BIND_IDX                    0
#define AE_BI_PER_TILE_LIGHT_INDEX_BUFFER_CULL_BIND_IDX     0
#define AE_BI_LIGHT_BUFFER_DRAW_BIND_IDX                    0
#define AE_BI_PER_TILE_LIGHT_INDEX_BUFFER_DRAW_BIND_IDX     1
#define AE_BI_SHADOW_SPOT_LIGHT_INFO_BUFFER_BIND_IDX        2
#define AE_BI_SHADOW_SPOT_LIGHT_TEXT_ARRAY_BUFFER_BIND_IDX  3
#define AE_BI_SHADOW_DIR_LIGHT_TEXT_ARRAY_BUFFER_BIND_IDX   4
#define AE_BI_SHADOW_DIR_LIGHT_INFO_BUFFER_BIND_IDX         5

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

namespace AEShaderHelper
{
    extern D3D11_FILTER GetDXFilter(ShaderFilter shaderFilter);

    extern D3D11_TEXTURE_ADDRESS_MODE GetDXTextureAddressMode(ShaderTextureAddressMode textureAddressMode);

    extern D3D11_COMPARISON_FUNC GetDXCompFunct(ShaderCompFunct compFunct);

    extern uint32_t GetShaderVariableTypeSize(ShaderVariableType svType);

    extern bool IsShaderVariableTypeScalar(ShaderVariableType svType);
};

#endif
