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

/**********************
*   System Includes   *
***********************/

/*************************
*   3rd Party Includes   *
**************************/
#include "fastformat\fastformat.hpp"
#include "fastformat\sinks\ostream.hpp"
#include "fastformat\shims\conversion\filter_type\reals.hpp"

/***************************
*   Game Engine Includes   *
****************************/
#include "VertexDefs.h"
#include "Types\VertexTypes.h"

//Always include last
#include "Memory\MemLeaks.h"

/********************
*   Function Defs   *
*********************/

/*********************
*   XEVertexHelper   *
**********************/
namespace XEVertexHelper
{
	uint32_t GetVertexSizeByType(VertexType vtxType)
	{
		switch (vtxType)
		{
			case VertexType::VtxPos:
				return VertexPosition::VertexSize();

			case VertexType::VtxPosCol:
				return VertexPositionColor::VertexSize();

			case VertexType::VtxPosNor:
				return VertexPositionNormal::VertexSize();

			case VertexType::VtxPosTex:
				return VertexPositionTexture::VertexSize();

			case VertexType::VtxPosNorCol:
				return VertexPositionNormalColor::VertexSize();

			case VertexType::VtxPosNorTex:
				return VertexPositionNormalTexture::VertexSize();

			case VertexType::VtxPosNorTexTex2:
				return VertexPositionNormalTextureTexture2::VertexSize();

			case VertexType::VtxPosNorTexIdxWght:
				return VertexPositionNormalTextureIndicesWeight::VertexSize();

			case VertexType::VtxPosNorTanBinTex:
				return VertexPositionNormalTangentBinormalTexture::VertexSize();

			case VertexType::VtxPosNorTanBinTexTex2:
				return VertexPositionNormalTangentBinormalTextureTexture2::VertexSize();

			case VertexType::VtxPosNorTanBinTexIdxWght:
				return VertexPositionNormalTangentBinormalTextureIndicesWeight::VertexSize();

			case VertexType::VtxCustom:
			case VertexType::VtxNone:
			default:
				return 0;
		}
	}

	std::wstring GetVertexString(const VertexPositionColor& vtx)
	{
		std::wstring vtxString = L"";

		fastformat::fmt(vtxString, L"{0}{1}{2}{3}",
				vtx.m_Position.x,			//0
				vtx.m_Position.y,			//1
				vtx.m_Position.z,			//2
				(uint32_t)vtx.m_Color		//3
			);

		return vtxString;
	}

	std::wstring GetVertexString(const VertexPositionNormalTexture& vtx)
	{
		std::wstring vtxString = L"";

		fastformat::fmt(vtxString, L"{0}{1}{2}{3}{4}{5}{6}{7}",
				vtx.m_Position.x,			//0
				vtx.m_Position.y,			//1
				vtx.m_Position.z,			//2
				vtx.m_Normal.x,				//3
				vtx.m_Normal.y,				//4
				vtx.m_Normal.z,				//5
				vtx.m_TexCoord.x,			//6
				vtx.m_TexCoord.y			//7
			);

		return vtxString;
	}
}
