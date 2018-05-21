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

#ifndef _GAME_CONTENT_DEFS_H
#define _GAME_CONTENT_DEFS_H

/**********************
*   System Includes   *
***********************/

/*************************
*   3rd Party Includes   *
**************************/

/***************************
*   Game Engine Includes   *
****************************/
#include "Shaders\ShaderDefs.h"

/************
*   Using   *
*************/

/**************
*   Defines   *
***************/
#define AE_SHADER_SYS_INC_PATH_LEN          wcslen(AE_SHADER_SYS_INC_PATH)

#define AE_CT_AE3D_FILE_HEADER              0xABB1 //Abby
#define AE_CT_AE3D_FILE_FOOTER              0x1BBA
#define AE_CT_AE3D_FILE_VERSION_MAYOR       0
#define AE_CT_AE3D_FILE_VERSION_MINOR       0
#define AE_CT_AE3D_FILE_VERSION_REVISON     1

#define AE_CT_AE3DM_FILE_HEADER             0x7E44 //Jennifer
#define AE_CT_AE3DM_FILE_FOOTER             0x44E7
#define AE_CT_AE3DM_FILE_VERSION_MAYOR      0
#define AE_CT_AE3DM_FILE_VERSION_MINOR      0
#define AE_CT_AE3DM_FILE_VERSION_REVISON    1

#define AE_CT_AE3DS_FILE_HEADER             0xB560 //Bruno
#define AE_CT_AE3DS_FILE_FOOTER             0x065B
#define AE_CT_AE3DS_FILE_VERSION_MAYOR      0
#define AE_CT_AE3DS_FILE_VERSION_MINOR      0
#define AE_CT_AE3DS_FILE_VERSION_REVISON    1

#define AE_CT_AE3DA_FILE_HEADER             0x9191 //Pipo
#define AE_CT_AE3DA_FILE_FOOTER             0x1919
#define AE_CT_AE3DA_FILE_VERSION_MAYOR      0
#define AE_CT_AE3DA_FILE_VERSION_MINOR      0
#define AE_CT_AE3DA_FILE_VERSION_REVISON    1

#define AE_CT_AEHFX_FILE_HEADER             0xA441 //Annie
#define AE_CT_AEHFX_FILE_FOOTER             0x144A
#define AE_CT_AEHFX_FILE_VERSION_MAYOR      0
#define AE_CT_AEHFX_FILE_VERSION_MINOR      2
#define AE_CT_AEHFX_FILE_VERSION_REVISON    0

#define AE_CT_UNKNOWNL_EXT                  "unkwn"
#define AE_CT_MODEL_EXT                     "ae3d"
#define AE_CT_MESH_EXT                      "ae3dm"
#define AE_CT_ANIMATION_EXT                 "ae3da"
#define AE_CT_SKELETON_EXT                  "ae3ds"
#define AE_CT_TEX2D_EXT                     "aet2"
#define AE_CT_TEXCUBE_EXT                   "aetc"
#define AE_CT_SHADER_HLSL_VS_EXT            "aehvs"
#define AE_CT_SHADER_HLSL_PS_EXT            "aehps"
#define AE_CT_SHADER_HLSL_GS_EXT            "aehgs"
#define AE_CT_SHADER_HLSL_DS_EXT            "aehds"
#define AE_CT_SHADER_HLSL_HS_EXT            "aehhs"
#define AE_CT_SHADER_HLSL_CS_EXT            "aehcs"

#define AE_CT_GAMEOBJECT_SCRIPT_EXT         "aegos"

#define AE_CT_MODEL_COLLADA_EXT             "dae"
#define AE_CT_MODEL_FBX_EXT                 "fbx"

#define AE_CT_SHADER_HLSL_1_EXT             "fx"
#define AE_CT_SHADER_HLSL_2_EXT             "hlsl"

#define AE_CT_TEXTURE_DDS_EXT               "dds"

#define AE_CT_AUDIO_1_EXT                   "wav"
#define AE_CT_AUDIO_2_EXT                   "ogg"

/****************
*   Constants   *
*****************/

/************
*   Enums   *
*************/
enum class GameContextFileExt : uint32_t
{
    Unknown = 0,
    GameObjectScript,
    ModelCollada,
    ModelFBX,
    ShaderFX,
    ShaderHLSL,
    TextureDDS,
    AudioWav,
    AudioOgg
};

enum class GameContentType : uint32_t
{
    Unknown = 0,
    Model,
    Mesh,
    Animation,
    Skeleton,
    Texture,
    Shader,
    GameObjectScript,
    Audio
};

enum class GameContentSubtype : uint32_t
{
    Unknown = 0,
    None,
    AnimationClip,
    VertexShaderHLSL,
    PixelShaderHLSL,
    GeometryShaderHLSL,
    DomainShaderHLSL,
    HullShaderHLSL,
    ComputeShaderHLSL,
    Texture2D,
    TextureCube
};

/********************
*   Forward Decls   *
*********************/

/***************
*   Typedefs   *
****************/

/***********************
*   Global Functions   *
************************/
namespace AEGameContentHelpers
{
    extern std::string GameContentTypeToString(GameContentType gameContentType);

    extern std::string GameContentSubtypeToString(GameContentSubtype gameContentSubtype);

    extern AEResult FileGameContentType(const std::string& file, GameContentType& contentType, GameContextFileExt& fileExt);

    extern AEResult FileShaderyType(const std::string& file, ShaderType& shaderType);

    extern void WriteFileHeader(std::ofstream& fileStream, uint16_t fileHeader, uint32_t mayorVersion, uint32_t minorVersion, uint32_t revisionVersion);

    extern void WriteFileFooter(std::ofstream& fileStream, uint16_t fileFooter);

    extern bool ReadFileHeaderAndVerify(std::ifstream& fileStream, uint16_t fileHeader, uint32_t mayorVersion, uint32_t minorVersion, uint32_t revisionVersion);

    extern bool ReadFileFooterAndVerify(std::ifstream& fileStream, uint16_t fileFooter);

    extern void WriteString(std::ofstream& fileStream, const std::string& str);

    extern std::string ReadString(std::ifstream& fileStream);

}

#endif
