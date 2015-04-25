/********************************************************
*
* Author: Carlos Chacón N.
*
* Copyright 2012-2015
*
*********************************************************/

/**********************
*   System Includes   *
***********************/

/*************************
*   3rd Party Includes   *
**************************/

/***************************
*   Game Engine Includes   *
****************************/
#include "IcoSphereMesh.h"
#include "GraphicDevice.h"
#include "GameContentDefs.h"
#include "Models\MeshPart.h"
#include "Base\BaseFunctions.h"
#include "Vertex\IndexBuffer.h"
#include "Vertex\VertexBuffer.h"
#include "Content\ContentDefs.h"

//Always include last
#include "Memory\MemLeaks.h"

/********************
*   Function Defs   *
*********************/
IcoSphereMesh::IcoSphereMesh(GraphicDevice* graphicDevice, const std::wstring& resourceName)
	: Mesh(graphicDevice, resourceName)
{
}

IcoSphereMesh::~IcoSphereMesh()
{
}

XEResult IcoSphereMesh::Load()
{
	XEAssert(m_GraphicDevice != nullptr);
	if(m_GraphicDevice == nullptr)
	{
		return XEResult::GraphicDeviceNull;
	}

	std::lock_guard<std::mutex> lock(m_GameResourceMutex);

	if (m_IsLoaded)
	{
		return XEResult::Ok;
	}

	/////////////////////////////////////////////
	//Clean Up memory before loading File
	CleanUp();

	m_BoundingBox		= BoundingBox();
	m_BoundingSphere	= BoundingSphere();

	/////////////////////////////////////////////
	//Variables

	//Get Base Size
	float t = (1.0f + sqrt(5.0f)) / 2.0f;

	VertexPositionNormalTexture vtx1;
	VertexPositionNormalTexture vtx2;
	VertexPositionNormalTexture vtx3;
	
	memset(&vtx1, 0, sizeof(VertexPositionNormalTexture));
	memset(&vtx2, 0, sizeof(VertexPositionNormalTexture));
	memset(&vtx3, 0, sizeof(VertexPositionNormalTexture));

	//////////////////////////
	//5 faces around point 0
	//////////////////////////

	//Face 1
	vtx1.m_Position	= glm::vec3(-1.0f,	t,	0.0f);
	vtx1.m_TexCoord	= glm::vec2(0.0f,	0.0f);

	vtx3.m_Position = glm::vec3(-t, 0.0f, 1.0f);
	vtx3.m_TexCoord = glm::vec2(1.0f, 0.0f);

	vtx2.m_Position = glm::vec3(0.0f, 1.0f, t);
	vtx2.m_TexCoord = glm::vec2(0.0f, 1.0f);

	AddFace(vtx1, vtx2, vtx3);

	//Face 2
	vtx1.m_Position = glm::vec3(-1.0f, t, 0.0f);
	vtx1.m_TexCoord = glm::vec2(0.0f, 0.0f);

	vtx3.m_Position = glm::vec3(0.0f, 1.0f, t);
	vtx3.m_TexCoord = glm::vec2(1.0f, 0.0f);

	vtx2.m_Position = glm::vec3(1.0f, t, 0.0f);
	vtx2.m_TexCoord = glm::vec2(0.0f, 1.0f);

	AddFace(vtx1, vtx2, vtx3);

	//Face 3
	vtx1.m_Position = glm::vec3(-1.0f, t, 0.0f);
	vtx1.m_TexCoord = glm::vec2(0.0f, 0.0f);

	vtx3.m_Position = glm::vec3(1.0f, t, 0.0f);
	vtx3.m_TexCoord = glm::vec2(1.0f, 0.0f);

	vtx2.m_Position = glm::vec3(0.0f, 1.0f, -t);
	vtx2.m_TexCoord = glm::vec2(0.0f, 1.0f);

	AddFace(vtx1, vtx2, vtx3);

	//Face 4
	vtx1.m_Position = glm::vec3(-1.0f, t, 0.0f);
	vtx1.m_TexCoord = glm::vec2(0.0f, 0.0f);

	vtx3.m_Position = glm::vec3(0.0f, 1.0f, -t);
	vtx3.m_TexCoord = glm::vec2(1.0f, 0.0f);

	vtx2.m_Position = glm::vec3(-t, 0, -1);
	vtx2.m_TexCoord = glm::vec2(0.0f, 1.0f);

	AddFace(vtx1, vtx2, vtx3);

	//Face 5
	vtx1.m_Position = glm::vec3(-1.0f, t, 0.0f);
	vtx1.m_TexCoord = glm::vec2(0.0f, 0.0f);

	vtx3.m_Position = glm::vec3(-t, 0.0f, -1.0f);
	vtx3.m_TexCoord = glm::vec2(1.0f, 0.0f);

	vtx2.m_Position = glm::vec3(-t, 0.0f, 1.0f);
	vtx2.m_TexCoord = glm::vec2(0.0f, 1.0f);

	AddFace(vtx1, vtx2, vtx3);

	//////////////////////////
	//5 adjacent faces 
	//////////////////////////

	//Face 1
	vtx1.m_Position = glm::vec3(1.0f, t, 0.0f);
	vtx1.m_TexCoord = glm::vec2(0.0f, 0.0f);

	vtx3.m_Position = glm::vec3(0.0f, 1.0f, t);
	vtx3.m_TexCoord = glm::vec2(1.0f, 0.0f);

	vtx2.m_Position = glm::vec3(t, 0.0f, 1.0f);
	vtx2.m_TexCoord = glm::vec2(0.0f, 1.0f);

	AddFace(vtx1, vtx2, vtx3);

	//Face 2
	vtx1.m_Position = glm::vec3(0.0f, 1.0f, t);
	vtx1.m_TexCoord = glm::vec2(0.0f, 0.0f);

	vtx3.m_Position = glm::vec3(-t, 0.0f, 1.0f);
	vtx3.m_TexCoord = glm::vec2(1.0f, 0.0f);

	vtx2.m_Position = glm::vec3(0.0f, -1.0f, t);
	vtx2.m_TexCoord = glm::vec2(0.0f, 1.0f);

	AddFace(vtx1, vtx2, vtx3);

	//Face 3
	vtx1.m_Position = glm::vec3(-t, 0.0f, 1.0f);
	vtx1.m_TexCoord = glm::vec2(0.0f, 0.0f);

	vtx3.m_Position = glm::vec3(-t, 0.0f, -1.0f);
	vtx3.m_TexCoord = glm::vec2(1.0f, 0.0f);

	vtx2.m_Position = glm::vec3(-1.0f, -t, 0.0f);
	vtx2.m_TexCoord = glm::vec2(0.0f, 1.0f);

	AddFace(vtx1, vtx2, vtx3);

	//Face 4
	vtx1.m_Position = glm::vec3(-t, 0.0f, -1.0f);
	vtx1.m_TexCoord = glm::vec2(0.0f, 0.0f);

	vtx3.m_Position = glm::vec3(0.0f, 1.0f, -t);
	vtx3.m_TexCoord = glm::vec2(1.0f, 0.0f);

	vtx2.m_Position = glm::vec3(0.0f, -1.0f, -t);
	vtx2.m_TexCoord = glm::vec2(0.0f, 1.0f);

	AddFace(vtx1, vtx2, vtx3);

	//Face 5
	vtx1.m_Position = glm::vec3(0.0f, 1.0f, -t);
	vtx1.m_TexCoord = glm::vec2(0.0f, 0.0f);

	vtx3.m_Position = glm::vec3(1.0f, t, 0.0f);
	vtx3.m_TexCoord = glm::vec2(1.0f, 0.0f);

	vtx2.m_Position = glm::vec3(t, 0.0f, -1.0f);
	vtx2.m_TexCoord = glm::vec2(0.0f, 1.0f);

	AddFace(vtx1, vtx2, vtx3);

	//////////////////////////
	//5 faces around point 3
	//////////////////////////

	//Face 1
	vtx1.m_Position = glm::vec3(1.0f, -t, 0.0f);
	vtx1.m_TexCoord = glm::vec2(0.0f, 0.0f);

	vtx3.m_Position = glm::vec3(t, 0.0f, 1.0f);
	vtx3.m_TexCoord = glm::vec2(1.0f, 0.0f);

	vtx2.m_Position = glm::vec3(0.0f, -1.0f, t);
	vtx2.m_TexCoord = glm::vec2(0.0f, 1.0f);

	AddFace(vtx1, vtx2, vtx3);

	//Face 2
	vtx1.m_Position = glm::vec3(1.0f, -t, 0.0f);
	vtx1.m_TexCoord = glm::vec2(0.0f, 0.0f);

	vtx3.m_Position = glm::vec3(0.0f, -1.0f, t);
	vtx3.m_TexCoord = glm::vec2(1.0f, 0.0f);

	vtx2.m_Position = glm::vec3(-1.0f, -t, 0.0f);
	vtx2.m_TexCoord = glm::vec2(0.0f, 1.0f);

	AddFace(vtx1, vtx2, vtx3);

	//Face 3
	vtx1.m_Position = glm::vec3(1.0f, -t, 0.0f);
	vtx1.m_TexCoord = glm::vec2(0.0f, 0.0f);

	vtx3.m_Position = glm::vec3(-1.0f, -t, 0.0f);
	vtx3.m_TexCoord = glm::vec2(1.0f, 0.0f);

	vtx2.m_Position = glm::vec3(0.0f, -1.0f, -t);
	vtx2.m_TexCoord = glm::vec2(0.0f, 1.0f);

	AddFace(vtx1, vtx2, vtx3);

	//Face 4
	vtx1.m_Position = glm::vec3(1.0f, -t, 0.0f);
	vtx1.m_TexCoord = glm::vec2(0.0f, 0.0f);

	vtx3.m_Position = glm::vec3(0.0f, -1.0f, -t);
	vtx3.m_TexCoord = glm::vec2(1.0f, 0.0f);

	vtx2.m_Position = glm::vec3(t, 0.0f, -1.0f);
	vtx2.m_TexCoord = glm::vec2(0.0f, 1.0f);

	AddFace(vtx1, vtx2, vtx3);

	//Face 5
	vtx1.m_Position = glm::vec3(1.0f, -t, 0.0f);
	vtx1.m_TexCoord = glm::vec2(0.0f, 0.0f);

	vtx3.m_Position = glm::vec3(t, 0.0f, -1.0f);
	vtx3.m_TexCoord = glm::vec2(1.0f, 0.0f);

	vtx2.m_Position = glm::vec3(t, 0.0f, 1.0f);
	vtx2.m_TexCoord = glm::vec2(0.0f, 1.0f);

	AddFace(vtx1, vtx2, vtx3);

	//////////////////////////
	//5 adjacent faces 
	//////////////////////////

	//Face 1
	vtx1.m_Position = glm::vec3(0.0f, -1.0f, t);
	vtx1.m_TexCoord = glm::vec2(0.0f, 0.0f);

	vtx3.m_Position = glm::vec3(t, 0.0f, 1.0f);
	vtx3.m_TexCoord = glm::vec2(1.0f, 0.0f);

	vtx2.m_Position = glm::vec3(0.0f, 1.0f, t);
	vtx2.m_TexCoord = glm::vec2(0.0f, 1.0f);

	AddFace(vtx1, vtx2, vtx3);

	//Face 2
	vtx1.m_Position = glm::vec3(-1.0f, -t, 0.0f);
	vtx1.m_TexCoord = glm::vec2(0.0f, 0.0f);

	vtx3.m_Position = glm::vec3(0.0f, -1.0f, t);
	vtx3.m_TexCoord = glm::vec2(1.0f, 0.0f);

	vtx2.m_Position = glm::vec3(-t, 0.0f, 1.0f);
	vtx2.m_TexCoord = glm::vec2(0.0f, 1.0f);

	AddFace(vtx1, vtx2, vtx3);

	//Face 3
	vtx1.m_Position = glm::vec3(0.0f, -1.0f, -t);
	vtx1.m_TexCoord = glm::vec2(0.0f, 0.0f);

	vtx3.m_Position = glm::vec3(-1.0f, -t, 0.0f);
	vtx3.m_TexCoord = glm::vec2(1.0f, 0.0f);

	vtx2.m_Position = glm::vec3(-t, 0.0f, -1.0f);
	vtx2.m_TexCoord = glm::vec2(0.0f, 1.0f);

	AddFace(vtx1, vtx2, vtx3);

	//Face 4
	vtx1.m_Position = glm::vec3(t, 0.0f, -1.0f);
	vtx1.m_TexCoord = glm::vec2(0.0f, 0.0f);

	vtx3.m_Position = glm::vec3(0.0f, -1.0f, -t);
	vtx3.m_TexCoord = glm::vec2(1.0f, 0.0f);

	vtx2.m_Position = glm::vec3(0.0f, 1.0f, -t);
	vtx2.m_TexCoord = glm::vec2(0.0f, 1.0f);

	AddFace(vtx1, vtx2, vtx3);

	//Face 5
	vtx1.m_Position = glm::vec3(t, 0.0f, 1.0f);
	vtx1.m_TexCoord = glm::vec2(0.0f, 0.0f);

	vtx2.m_Position = glm::vec3(1.0f, t, 0.0f);
	vtx2.m_TexCoord = glm::vec2(0.0f, 1.0f);

	vtx3.m_Position = glm::vec3(t, 0.0f, -1.0f);
	vtx3.m_TexCoord = glm::vec2(1.0f, 0.0f);

	AddFace(vtx1, vtx2, vtx3);

	///////////////////////////////////////////////////////////

	for (uint32_t i = 0; i < m_RecursionLevels; ++i)
	{
		std::vector<uint16_t> idxBuf2;

		uint32_t oldIndexBufSize = (uint32_t)m_IdxBuff.size();

		for (uint32_t j = 0; j < oldIndexBufSize; j += 3)
		{
			//Replace triangle by 4 triangles
			uint16_t a = GetMiddlePoint(m_IdxBuff[j], m_IdxBuff[j + 1]);
			uint16_t b = GetMiddlePoint(m_IdxBuff[j + 1], m_IdxBuff[j + 2]);
			uint16_t c = GetMiddlePoint(m_IdxBuff[j + 2], m_IdxBuff[j]);

			//New Face 1
			idxBuf2.push_back(m_IdxBuff[j]);
			idxBuf2.push_back(a);
			idxBuf2.push_back(c);

			//New Face 2
			idxBuf2.push_back(m_IdxBuff[j + 1]);
			idxBuf2.push_back(b);
			idxBuf2.push_back(a);

			//New Face 3
			idxBuf2.push_back(m_IdxBuff[j + 2]);
			idxBuf2.push_back(c);
			idxBuf2.push_back(b);

			//New Face 4
			idxBuf2.push_back(a);
			idxBuf2.push_back(b);
			idxBuf2.push_back(c);
		}

		m_IdxBuff = idxBuf2;
	}

	/////////////////////////////////////////////
	//Create Vertex Buffer and Index Buffer Objects

	//Vertex Buffer
	VertexBuffer<VertexPositionNormalTexture>* vertexBuffer = new VertexBuffer<VertexPositionNormalTexture>(m_GraphicDevice);

	XETODO("Check return");
	vertexBuffer->CopyVertexBuffer(m_VtxBuff);
	vertexBuffer->BuildVertexBuffer();

	//Index Buffer
	IndexBuffer* indexBuffer = new IndexBuffer(m_GraphicDevice);

	XETODO("Check return");
	indexBuffer->CopyToIndexBuffer(m_IdxBuff);
	indexBuffer->BuildIndexBuffer();

	/////////////////////////////////////////////
	//Add variables to Mesh Part
	MeshPart* meshPart = new MeshPart();

	meshPart->SetIndexBuffer(indexBuffer);
	meshPart->SetVertexBuffer(vertexBuffer);

	m_MeshPartVector.push_back(meshPart);

	/////////////////////////////////////////////
	//Set name
	SetNameWithoutLock(XE_ICO_SPHERE_MESH_NAME);

	/////////////////////////////////////////////
	//Finish
	m_IsLoaded = true;

	return XEResult::Ok;
}

void IcoSphereMesh::AddFace(const VertexPositionNormalTexture& vtx1, const VertexPositionNormalTexture& vtx2, const VertexPositionNormalTexture& vtx3)
{
	uint16_t idx = 0;

	//V1
	idx = AddVtx(vtx1);
	AddIdx(idx);

	//V2
	idx = AddVtx(vtx2);
	AddIdx(idx);

	//V3
	idx = AddVtx(vtx3);
	AddIdx(idx);
}

uint16_t IcoSphereMesh::AddVtx(const VertexPositionNormalTexture& vtx)
{
	VertexPositionNormalTexture tmpVtx = vtx;

	glm::vec3 vect(tmpVtx.m_Position.x, tmpVtx.m_Position.y, tmpVtx.m_Position.z);
	vect = glm::normalize(vect);

	tmpVtx.m_Position.x = vect.x;
	tmpVtx.m_Position.y = vect.y;
	tmpVtx.m_Position.z = vect.z;

	std::wstring vtxStr = XEVertexHelper::GetVertexString(tmpVtx);
	uint16_t idx = 0;

	if (m_VtxMap.find(vtxStr) != m_VtxMap.end())
	{
		idx = m_VtxMap[vtxStr];
	}
	else
	{
		m_BoundingBox.PushPositionPoint(tmpVtx.m_Position);
		m_BoundingSphere.PushPositionPoint(tmpVtx.m_Position);

		idx = (uint16_t)m_VtxBuff.size();
		m_VtxMap[vtxStr] = (uint16_t)m_VtxBuff.size();
		m_VtxBuff.push_back(tmpVtx);
	}

	return idx;
}

void IcoSphereMesh::AddIdx(uint16_t idx)
{
	m_IdxBuff.push_back(idx);
}

uint16_t IcoSphereMesh::GetMiddlePoint(uint32_t vtxIndex1, uint32_t vtxIndex2)
{
	glm::vec3 middlePos = ((m_VtxBuff[vtxIndex1].m_Position + m_VtxBuff[vtxIndex2].m_Position) / 2.0f);

	glm::vec2 middleTex = ((m_VtxBuff[vtxIndex1].m_TexCoord + m_VtxBuff[vtxIndex2].m_TexCoord) / 2.0f);

	VertexPositionNormalTexture newVtx;
	memset(&newVtx, 0, sizeof(VertexPositionNormalTexture));

	newVtx.m_Position = middlePos;
	newVtx.m_TexCoord = middleTex;

	//Add vertex makes sure point is on unit sphere
	uint16_t idx = AddVtx(newVtx);

	return idx;
}