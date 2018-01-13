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
#include <string>
#include <fstream>
#include <stdint.h>

/*************************
*   3rd Party Includes   *
**************************/

/***************************
*   Game Engine Includes   *
****************************/
#include "Base\Base.h"
#include "Base\AEObject.h"
#include "Base\BaseLocations.h"
#include "Shaders\ShaderDefs.h"

/************
*   Using   *
*************/

/**************
*   Defines   *
***************/
#define AE_SHADER_SYS_INC_PATH_LEN			wcslen(AE_SHADER_SYS_INC_PATH)

#define AE_CT_AE3D_FILE_HEADER				0xABB1 //Abby
#define AE_CT_AE3D_FILE_FOOTER				0x1BBA
#define AE_CT_AE3D_FILE_VERSION_MAYOR		0
#define AE_CT_AE3D_FILE_VERSION_MINOR		0
#define AE_CT_AE3D_FILE_VERSION_REVISON		1

#define AE_CT_AE3DM_FILE_HEADER				0x7E44 //Jennifer
#define AE_CT_AE3DM_FILE_FOOTER				0x44E7
#define AE_CT_AE3DM_FILE_VERSION_MAYOR		0
#define AE_CT_AE3DM_FILE_VERSION_MINOR		0
#define AE_CT_AE3DM_FILE_VERSION_REVISON	1

#define AE_CT_AE3DS_FILE_HEADER				0xB560 //Bruno
#define AE_CT_AE3DS_FILE_FOOTER				0x065B
#define AE_CT_AE3DS_FILE_VERSION_MAYOR		0
#define AE_CT_AE3DS_FILE_VERSION_MINOR		0
#define AE_CT_AE3DS_FILE_VERSION_REVISON	1

#define AE_CT_AE3DA_FILE_HEADER				0x9191 //Pipo
#define AE_CT_AE3DA_FILE_FOOTER				0x1919
#define AE_CT_AE3DA_FILE_VERSION_MAYOR		0
#define AE_CT_AE3DA_FILE_VERSION_MINOR		0
#define AE_CT_AE3DA_FILE_VERSION_REVISON	1

#define AE_CT_AEHFX_FILE_HEADER				0xA441 //Annie
#define AE_CT_AEHFX_FILE_FOOTER				0x144A
#define AE_CT_AEHFX_FILE_VERSION_MAYOR		0
#define AE_CT_AEHFX_FILE_VERSION_MINOR		2
#define AE_CT_AEHFX_FILE_VERSION_REVISON	0

#define AE_CT_UNKNOWNL_EXT					L"unkwn"
#define AE_CT_MODEL_EXT						L"ae3d"
#define AE_CT_MESH_EXT						L"ae3dm"
#define AE_CT_ANIMATION_EXT					L"ae3da"
#define AE_CT_SKELETON_EXT					L"ae3ds"
#define AE_CT_TEX2D_EXT						L"aet2"
#define AE_CT_TEXCUBE_EXT					L"aetc"
#define AE_CT_SHADER_HLSL_VS_EXT			L"aehvs"
#define AE_CT_SHADER_HLSL_PS_EXT			L"aehps"
#define AE_CT_SHADER_HLSL_GS_EXT			L"aehgs"
#define AE_CT_SHADER_HLSL_DS_EXT			L"aehds"
#define AE_CT_SHADER_HLSL_HS_EXT			L"aehhs"
#define AE_CT_SHADER_HLSL_CS_EXT			L"aehcs"

#define AE_CT_GAMEOBJECT_SCRIPT_EXT			L"aegos"

#define AE_CT_MODEL_COLLADA_EXT				L"dae"
#define AE_CT_MODEL_FBX_EXT					L"fbx"

#define AE_CT_SHADER_HLSL_1_EXT				L"fx"
#define AE_CT_SHADER_HLSL_2_EXT				L"hlsl"

#define AE_CT_TEXTURE_DDS_EXT				L"dds"

#define AE_CT_AUDIO_1_EXT					L"wav"
#define AE_CT_AUDIO_2_EXT					L"ogg"

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
	extern std::wstring GameContentTypeToString(GameContentType gameContentType);

	extern std::wstring GameContentSubtypeToString(GameContentSubtype gameContentSubtype);

	extern AEResult FileGameContentType(const std::wstring& file, GameContentType& contentType, GameContextFileExt& fileExt);

	extern AEResult FileShaderyType(const std::wstring& file, ShaderType& shaderType);

	extern void WriteFileHeader(std::ofstream& fileStream, uint16_t fileHeader, uint32_t mayorVersion, uint32_t minorVersion, uint32_t revisionVersion);

	extern void WriteFileFooter(std::ofstream& fileStream, uint16_t fileFooter);

	extern bool ReadFileHeaderAndVerify(std::ifstream& fileStream, uint16_t fileHeader, uint32_t mayorVersion, uint32_t minorVersion, uint32_t revisionVersion);

	extern bool ReadFileFooterAndVerify(std::ifstream& fileStream, uint16_t fileFooter);

	extern void WriteString(std::ofstream& fileStream, const std::wstring& str);

	extern std::wstring ReadString(std::ifstream& fileStream);

}

#endif