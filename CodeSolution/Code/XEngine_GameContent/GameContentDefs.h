/********************************************************
*
* Author: Carlos Chacón N.
*
* Copyright 2012-2015
*
*********************************************************/

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
#include "Base\XEObject.h"
#include "Base\BaseLocations.h"
#include "Shaders\ShaderDefs.h"

/************
*   Using   *
*************/

/**************
*   Defines   *
***************/
#define XE_SHADER_SYS_INC_PATH_LEN			wcslen(XE_SHADER_SYS_INC_PATH)

#define XE_CT_XE3D_FILE_HEADER				0xABB1 //Abby
#define XE_CT_XE3D_FILE_FOOTER				0x1BBA
#define XE_CT_XE3D_FILE_VERSION_MAYOR		0
#define XE_CT_XE3D_FILE_VERSION_MINOR		0
#define XE_CT_XE3D_FILE_VERSION_REVISON		1

#define XE_CT_XE3DM_FILE_HEADER				0x7E44 //Jennifer
#define XE_CT_XE3DM_FILE_FOOTER				0x44E7
#define XE_CT_XE3DM_FILE_VERSION_MAYOR		0
#define XE_CT_XE3DM_FILE_VERSION_MINOR		0
#define XE_CT_XE3DM_FILE_VERSION_REVISON	1

#define XE_CT_XE3DS_FILE_HEADER				0xB560  //Bruno
#define XE_CT_XE3DS_FILE_FOOTER				0x065B
#define XE_CT_XE3DS_FILE_VERSION_MAYOR		0
#define XE_CT_XE3DS_FILE_VERSION_MINOR		0
#define XE_CT_XE3DS_FILE_VERSION_REVISON	1

#define XE_CT_XE3DA_FILE_HEADER				0x9191 //Pipo
#define XE_CT_XE3DA_FILE_FOOTER				0x1919
#define XE_CT_XE3DA_FILE_VERSION_MAYOR		0
#define XE_CT_XE3DA_FILE_VERSION_MINOR		0
#define XE_CT_XE3DA_FILE_VERSION_REVISON	1

#define XE_CT_XEHFX_FILE_HEADER				0xA441 //Annie
#define XE_CT_XEHFX_FILE_FOOTER				0x144A
#define XE_CT_XEHFX_FILE_VERSION_MAYOR		0
#define XE_CT_XEHFX_FILE_VERSION_MINOR		2
#define XE_CT_XEHFX_FILE_VERSION_REVISON	0

#define XE_CT_UNKNOWNL_EXT					L"unkwn"
#define XE_CT_MODEL_EXT						L"xe3d"
#define XE_CT_MESH_EXT						L"xe3dm"
#define XE_CT_ANIMATION_EXT					L"xe3da"
#define XE_CT_SKELETON_EXT					L"xe3ds"
#define XE_CT_TEX2D_EXT						L"xet2"
#define XE_CT_TEXCUBE_EXT					L"xetc"
#define XE_CT_SHADER_HLSL_VS_EXT			L"xehvs"
#define XE_CT_SHADER_HLSL_PS_EXT			L"xehps"
#define XE_CT_SHADER_HLSL_GS_EXT			L"xehgs"
#define XE_CT_SHADER_HLSL_DS_EXT			L"xehds"
#define XE_CT_SHADER_HLSL_HS_EXT			L"xehhs"
#define XE_CT_SHADER_HLSL_CS_EXT			L"xehcs"

#define XE_CT_GAMEOBJECT_SCRIPT_EXT			L"xegos"

#define XE_CT_MODEL_COLLADA_EXT				L"dae"
#define XE_CT_MODEL_FBX_EXT					L"fbx"

#define XE_CT_SHADER_HLSL_1_EXT				L"fx"
#define XE_CT_SHADER_HLSL_2_EXT				L"hlsl"

#define XE_CT_TEXTURE_DDS_EXT				L"dds"

#define XE_CT_AUDIO_1_EXT					L"wav"
#define XE_CT_AUDIO_2_EXT					L"ogg"

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
namespace XEGameContentHelpers
{
	extern std::wstring GameContentTypeToString(GameContentType gameContentType);

	extern std::wstring GameContentSubtypeToString(GameContentSubtype gameContentSubtype);

	extern XEResult FileGameContentType(const std::wstring& file, GameContentType& contentType, GameContextFileExt& fileExt);

	extern XEResult FileShaderyType(const std::wstring& file, ShaderType& shaderType);

	extern void WriteFileHeader(std::ofstream& fileStream, uint16_t fileHeader, uint32_t mayorVersion, uint32_t minorVersion, uint32_t revisionVersion);

	extern void WriteFileFooter(std::ofstream& fileStream, uint16_t fileFooter);

	extern bool ReadFileHeaderAndVerify(std::ifstream& fileStream, uint16_t fileHeader, uint32_t mayorVersion, uint32_t minorVersion, uint32_t revisionVersion);

	extern bool ReadFileFooterAndVerify(std::ifstream& fileStream, uint16_t fileFooter);

	extern void WriteString(std::ofstream& fileStream, const std::wstring& str);

	extern std::wstring ReadString(std::ifstream& fileStream);

}

#endif