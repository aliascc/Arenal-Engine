/********************************************************
*
* Author: Carlos Chacón N.
*
* Desc:
*
*
* Copyright 2012-2014
*
*********************************************************/
#pragma once

#ifndef _VERTEX_DEFS_H
#define _VERTEX_DEFS_H

/**********************
*   System Includes   *
***********************/
#include <string>
#include <stdint.h>

/***************************
*   Game Engine Includes   *
****************************/
#include "Base\XEObject.h"

/********************
*   Forward Decls   *
*********************/
struct VertexPositionColor;
struct VertexPositionNormalTexture;

/**************
*   Defines   *
***************/
#define XE_VERTEX_TYPE_NONE				0x00000000
#define XE_VERTEX_TYPE_POSITION			0x00000001
#define XE_VERTEX_TYPE_NORMAL			0x00000002
#define XE_VERTEX_TYPE_BINORMAL			0x00000004
#define XE_VERTEX_TYPE_TANGENT			0x00000008
#define XE_VERTEX_TYPE_TEXCOORD			0x00000010
#define XE_VERTEX_TYPE_TEXCOORD2		0x00000020
#define XE_VERTEX_TYPE_COLOR			0x00000040
#define XE_VERTEX_TYPE_WIRE_COLOR		0x00000080
#define XE_VERTEX_TYPE_CUSTOM			0x00000100
#define XE_VERTEX_TYPE_PARTICLE			0x00000200
#define XE_VERTEX_TYPE_INDICES			0x00000400
#define XE_VERTEX_TYPE_WEIGHT			0x00000800

/****************
*   Constants   *
*****************/

/************
*   Enums   *
*************/
enum class IndexBufferType : uint32_t
{
	Index16 = 0,
	Index32
};

enum class VertexType : uint32_t
{
	//Vertex None
	VtxNone = XE_VERTEX_TYPE_NONE,

	//Vertex Custom for users
	VtxCustom = XE_VERTEX_TYPE_CUSTOM,

	//Vertex Position
	VtxPos = XE_VERTEX_TYPE_POSITION,

	//Vertex Position Color
	VtxPosCol = XE_VERTEX_TYPE_POSITION | XE_VERTEX_TYPE_COLOR,

	//Vertex Position Normal
	VtxPosNor = XE_VERTEX_TYPE_POSITION | XE_VERTEX_TYPE_NORMAL,

	//Vertex Position Texture
	VtxPosTex = XE_VERTEX_TYPE_POSITION | XE_VERTEX_TYPE_TEXCOORD,

	//Vertex Position Normal COlor
	VtxPosNorCol = XE_VERTEX_TYPE_POSITION | XE_VERTEX_TYPE_NORMAL | XE_VERTEX_TYPE_COLOR,

	//Vertex Position Normal Texture
	VtxPosNorTex = XE_VERTEX_TYPE_POSITION | XE_VERTEX_TYPE_NORMAL | XE_VERTEX_TYPE_TEXCOORD,

	//Vertex Position Normal Texture Texture2
	VtxPosNorTexTex2 = XE_VERTEX_TYPE_POSITION | XE_VERTEX_TYPE_NORMAL | XE_VERTEX_TYPE_TEXCOORD | XE_VERTEX_TYPE_TEXCOORD2,

	//Vertex Position Normal Texture Indices Weight
	VtxPosNorTexIdxWght = XE_VERTEX_TYPE_POSITION | XE_VERTEX_TYPE_NORMAL | XE_VERTEX_TYPE_TEXCOORD | XE_VERTEX_TYPE_INDICES | XE_VERTEX_TYPE_WEIGHT,

	//Vertex Position Normal Tangent Binormal Texture
	VtxPosNorTanBinTex = XE_VERTEX_TYPE_POSITION | XE_VERTEX_TYPE_NORMAL | XE_VERTEX_TYPE_TANGENT | XE_VERTEX_TYPE_BINORMAL | XE_VERTEX_TYPE_TEXCOORD,

	//Vertex Position Normal Tangent Binormal Texture Texture 2
	VtxPosNorTanBinTexTex2 = XE_VERTEX_TYPE_POSITION | XE_VERTEX_TYPE_NORMAL | XE_VERTEX_TYPE_TANGENT | XE_VERTEX_TYPE_BINORMAL | XE_VERTEX_TYPE_TEXCOORD | XE_VERTEX_TYPE_TEXCOORD2,

	//Vertex Position Normal Tangent Binormal Texture Indices Weight
	VtxPosNorTanBinTexIdxWght = XE_VERTEX_TYPE_POSITION | XE_VERTEX_TYPE_NORMAL | XE_VERTEX_TYPE_TANGENT | XE_VERTEX_TYPE_BINORMAL | XE_VERTEX_TYPE_TEXCOORD | XE_VERTEX_TYPE_INDICES | XE_VERTEX_TYPE_WEIGHT
};

/*************
*   Struct   *
**************/
namespace XEVertexHelper
{
	extern uint32_t GetVertexSizeByType(VertexType vtxType);

	extern std::wstring GetVertexString(const VertexPositionColor& vtx);

	extern std::wstring GetVertexString(const VertexPositionNormalTexture& vtx);
}

#endif