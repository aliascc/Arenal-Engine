/********************************************************
*
* Author: Carlos Chacón N.
*
* Created: 1/30/2013
*
* Desc:
*
*
* Copyright 2012
*
*********************************************************/
#pragma once

#ifndef _CONTENT_DEFS_H
#define _CONTENT_DEFS_H

/**********************
*   System Includes   *
***********************/
#include <map>
#include <vector>
#include <string>
#include <stdint.h>

/*************************
*   3rd Party Includes   *
**************************/
#include "glm\glm.hpp"
#include "assimp\scene.h"

/***************************
*   Game Engine Includes   *
****************************/
#include "Base\Base.h"
#include "Color\Color.h"
#include "Base\XEObject.h"
#include "Math\XEMathDefs.h"
#include "Color\XEColorDefs.h"
#include "Vertex\VertexDefs.h"
#include "Shaders\ShaderDefs.h"
#include "Textures\TextureDefs.h"
#include "Vertex\Types\VertexTypes.h"
#include "Models\Skinning\SkinningDefs.h"
#include "Shaders\Variables\ShaderCustomVariable.h"

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

/********************
*   Forward Decls   *
*********************/
class Bone;
class GraphicDevice;
struct VertexHolder;
class IVertexBuffer;
struct PolygonHolder;
struct MeshPartContent;
struct IVertexBufferContent;

/***************
*   Typedefs   *
****************/
typedef std::vector<VertexHolder> VertexHolderVector;
typedef std::vector<PolygonHolder> PolygonHolderVector;
typedef std::map<int32_t, std::vector<PolygonHolder>> PolygonHolderVecMap;

/*************
*   Struct   *
**************/

struct SamplerShaderHolder sealed : public XEObject
{
	std::wstring m_Name = L"";

	uint32_t m_BindIndex = 0;

	SamplerShaderHolder()
	{
	}
};

struct TextureArrayShaderVariableHolder sealed : public XEObject
{
	std::wstring m_Name = L"";

	uint32_t m_BindIndex = 0;

	TextureType m_TextureType = TextureType::Texture2D;

	TextureArrayShaderVariableHolder()
	{
	}
};

struct SimpleBufferShaderHolder sealed : public XEObject
{
	std::wstring m_Name = L"";

	uint32_t m_BindIndex = 0;

	bool m_IsRW = false;

	ShaderVariableClass m_VariableClass = ShaderVariableClass::Scalar;

	ShaderVariableType m_VariableType = ShaderVariableType::Int;

	uint32_t m_ElementCount = 0;

	SimpleBufferShaderHolder()
	{
	}
};

struct StructuredBufferShaderHolder sealed : public XEObject
{
	std::wstring m_Name = L"";
	uint32_t m_BindIndex = 0;
	bool m_IsRW = false;

	StructuredBufferShaderHolder()
	{
	}
};

struct ConstantBufferShaderHolder sealed : public XEObject
{
	std::wstring m_Name = L"";

	uint32_t m_BindIndex = 0;

	std::list<ShaderCustomVariable> m_ShaderCustomVariableList;

	ConstantBufferShaderHolder()
	{
	}
};

struct TextureShaderVariableHolder sealed : public XEObject
{
	std::wstring m_Name = L"";

	uint32_t m_BindIndex = 0;

	TextureType m_TextureType = TextureType::Texture2D;

	TextureShaderVariableHolder()
	{
	}
};

/// <summary>
/// Struct to temporally hold Vertex information 
/// </summary>
struct VertexHolder sealed : public XEObject
{
	glm::vec3 m_Position = XEMathHelpers::Vec3fZero;

	glm::vec2 m_UV = XEMathHelpers::Vec2fZero;

	glm::vec2 m_UV2 = XEMathHelpers::Vec2fZero;

	glm::vec3 m_Normal = XEMathHelpers::Vec3fZero;

	glm::vec4 m_Tangent = XEMathHelpers::Vec4fZero;

	glm::vec4 m_Binormal = XEMathHelpers::Vec4fZero;

	Color m_Color = XEColors::White;

	glm::ivec4 m_BlendIndices = XEMathHelpers::Vec4iZero;

	glm::vec4 m_BlendWeights = XEMathHelpers::Vec4fZero;;

	std::map<std::wstring, uint32_t> m_BoneToIndex;

	std::map<uint32_t, std::wstring> m_IndexToBone;

	uint32_t m_BlendIndexCount = 0;

	int32_t m_VertexID = -1;

	int32_t m_ControlPointIndex = -1;

	VertexHolder()
	{
	}

	std::wstring ToString() const override;
};

struct PolygonHolder final : public XEObject
{
	int32_t m_PolygonIndex = -1;
	int32_t m_MaterialIndex = -1;
	uint64_t m_MaterialID = 0;

	VertexHolderVector m_Vertexs;

	PolygonHolder()
	{
	}
};

struct PolygonHolderContainer final : public XEObject
{
	PolygonHolderVector m_Polygons;

	PolygonHolderContainer();

	PolygonHolderVecMap GetPolygonsSeparate();

	XEResult SetMaterialIndexToPolygons(int32_t polyIdx, int32_t matIdx);

	void SetMaterialIndexToAllPolygons(int32_t matIdx);

	void SetMaterialIDToPolygons(uint64_t matID, int32_t matIdx);

	void SetBlendIndicesWeightToVertex(int32_t controlPointIndex, int32_t blendIndex, float blendWeight);

	inline void Clear()
	{
		m_Polygons.clear();
	}

	inline void Add(const PolygonHolder& poly)
	{
		m_Polygons.push_back(poly);
	}
};

struct MaterialHolder sealed : public XEObject
{
	int32_t m_MaterialIndex = -1;

	uint64_t m_MaterialID = 0;

	std::wstring m_Name = L"";

	std::wstring m_DiffuseTextureString = L"";

	std::wstring m_NormalTextureString = L"";

	std::wstring m_SpecularTextureString = L"";

	std::wstring m_LightMapTextureString = L"";

	std::wstring m_DisplacementTextureString = L"";

	Color m_DiffuseColor = XEColors::White;

	MaterialHolder()
	{
	}
};

struct MeshPartHolder sealed : public XEObject
{
	VertexType m_VertexType = VertexType::VtxNone;

	uint32_t m_MaterialID = 0;

	VertexHolderVector m_VtxHolderVec;

	std::vector<uint16_t> m_IndexHolder;

	MeshPartHolder()
	{
	}
};

struct MeshHolder sealed : public XEObject
{
	std::wstring m_Name = L"";

	glm::mat4 m_WorldTransform = XEMathHelpers::Mat4Identity;

	std::vector<MeshPartHolder> m_MeshPartHolderVec;

	PolygonHolderContainer m_PolygonHolderContainer;

	MeshHolder()
	{
	}
};

struct TransformsHolder sealed : public XEObject
{
	glm::mat4 m_LocalTransform = XEMathHelpers::Mat4Identity;

	glm::mat4 m_WorldTransform = XEMathHelpers::Mat4Identity;

	TransformsHolder()
	{
	}
};

struct KeyFrameHolder sealed : public XEObject
{
	std::map<float, BonePose> m_BonePoseMap;

	std::wstring m_BoneName = L"";

	KeyFrameHolder()
	{
	}
};

struct AnimationHolder sealed : public XEObject
{
	std::vector<KeyFrameHolder> m_KeyFramesVec;

	float m_Duration = 0.0f;

	std::wstring m_Name = L"";

	AnimationHolder()
	{
	}
};

struct TreeBone sealed : public XEObject
{
	Bone* m_Bone = nullptr;

	std::list<TreeBone*> m_ChildBones;

	TreeBone();
	virtual ~TreeBone();
};

XETODO("Should rename of move out of here");

namespace XEImporterHelpers
{
	extern glm::mat4 ConvertAssimpMatrix(const aiMatrix4x4& assimpMatrix);

	extern XEResult BuildVertexBuffer(GraphicDevice* graphicDevice, IVertexBuffer** vertexBuffer, VertexType vtxType, void* buffer, uint32_t size);

	extern XEResult ConvertToMeshContent(const MeshPartHolder& meshPartHolder, MeshPartContent** meshPartContent, bool generateTangentBinormal = false);

	extern XEResult BuildVertexBufferContent(VertexType vtxType, const VertexHolderVector& tempVtxBuffer, IVertexBufferContent** vtxBufffer, bool generateTangentBinormal = false, const std::vector<uint16_t>& idxsData = std::vector<uint16_t>(0));

	extern bool IsVertexTypeValid(VertexType vtxType);

	extern XEResult GetVertexArray(VertexType vtxType, const VertexHolderVector& vxtHolderVector, void* vtxArr, uint32_t size);
	extern XEResult GetVertexArrayP(const VertexHolderVector& vxtHolderVector, VertexPosition vtxArr[], uint32_t size);
	extern XEResult GetVertexArrayPC(const VertexHolderVector& vxtHolderVector, VertexPositionColor vtxArr[], uint32_t size);
	extern XEResult GetVertexArrayPN(const VertexHolderVector& vxtHolderVector, VertexPositionNormal vtxArr[], uint32_t size);
	extern XEResult GetVertexArrayPT(const VertexHolderVector& vxtHolderVector, VertexPositionTexture vtxArr[], uint32_t size);
	extern XEResult GetVertexArrayPNC(const VertexHolderVector& vxtHolderVector, VertexPositionNormalColor vtxArr[], uint32_t size);
	extern XEResult GetVertexArrayPNT(const VertexHolderVector& vxtHolderVector, VertexPositionNormalTexture vtxArr[], uint32_t size);
	extern XEResult GetVertexArrayPNTT2(const VertexHolderVector& vxtHolderVector, VertexPositionNormalTextureTexture2 vtxArr[], uint32_t size);
	extern XEResult GetVertexArrayPNTIW(const VertexHolderVector& vxtHolderVector, VertexPositionNormalTextureIndicesWeight vtxArr[], uint32_t size);
	extern XEResult GetVertexArrayPNTBT(const VertexHolderVector& vxtHolderVector, VertexPositionNormalTangentBinormalTexture vtxArr[], uint32_t size);
	extern XEResult GetVertexArrayPNTBTT2(const VertexHolderVector& vxtHolderVector, VertexPositionNormalTangentBinormalTextureTexture2 vtxArr[], uint32_t size);
	extern XEResult GetVertexArrayPNTBTIW(const VertexHolderVector& vxtHolderVector, VertexPositionNormalTangentBinormalTextureIndicesWeight vtxArr[], uint32_t size);

	template<class T>
	XEResult CreateVertexBuffer(GraphicDevice* graphicDevice, IVertexBuffer** vertexBuffer, void* buffer, uint32_t size)
	{
		XEAssert(graphicDevice != nullptr);
		XEAssert(vertexBuffer != nullptr);
		XEAssert(buffer != nullptr);

		if (graphicDevice == nullptr || vertexBuffer == nullptr || buffer == nullptr)
		{
			return XEResult::NullParameter;
		}

		VertexBuffer<T>* vtxBufferTemplate = new VertexBuffer<T>(graphicDevice);

		T* arrayBuffer = reinterpret_cast<T*>(buffer);

		vtxBufferTemplate->CopyVertexBuffer(arrayBuffer, size);
		vtxBufferTemplate->BuildVertexBuffer();

		(*vertexBuffer) = vtxBufferTemplate;

		return XEResult::Ok;
	}

	template<class T>
	XEResult CreateVertexBufferContent(const VertexHolderVector& vxtHolderVector, IVertexBufferContent** vtxBuff, bool generateTangentBinormal = false, const std::vector<uint16_t>& idxsData = std::vector<uint16_t>(0), uint32_t geometryStride = 0, uint32_t normalStride = 12, uint32_t tangentStride = 24, uint32_t binormalStride = 40, uint32_t textureCoordsStride = 56)
	{
		XEAssert(vtxBuff != nullptr);

		if (vtxBuff == nullptr)
		{
			return XEResult::NullParameter;
		}

		uint32_t size = (uint32_t)vxtHolderVector.size();

		T* tmpBuff = new T[size];
		ZeroMemory(tmpBuff, sizeof(T)* size);

		XEImporterHelpers::GetVertexArray(T::GetVertexType(), vxtHolderVector, tmpBuff, size);

		if (generateTangentBinormal == true)
		{
			XEMathHelpers::CalculateTangentsAndBinormal(tmpBuff, idxsData, size, sizeof(T), geometryStride, normalStride, tangentStride, binormalStride, textureCoordsStride);
		}

		VertexBufferContent<T>* xeBuff = new VertexBufferContent<T>();
		xeBuff->m_Buffer = tmpBuff;
		xeBuff->m_Size = size;

		(*vtxBuff) = xeBuff;

		return XEResult::Ok;
	}
}

#endif