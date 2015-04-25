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
#include <fstream>

/*************************
*   3rd Party Includes   *
**************************/

/***************************
*   Game Engine Includes   *
****************************/
#include "Mesh.h"
#include "MeshPart.h"
#include "GraphicDevice.h"
#include "GameContentDefs.h"
#include "Base\BaseFunctions.h"
#include "Vertex\IndexBuffer.h"
#include "Vertex\VertexBuffer.h"
#include "Content\ContentDefs.h"

//Always include last
#include "Memory\MemLeaks.h"

/********************
*   Function Defs   *
*********************/
Mesh::Mesh(GraphicDevice* graphicDevice, const std::wstring& resourceName)
	: GameResource(resourceName, GameResourceType::Mesh)
	, m_GraphicDevice(graphicDevice)
{
	XEAssert(m_GraphicDevice != nullptr);
}

Mesh::~Mesh()
{
	CleanUp();
}

void Mesh::CleanUp()
{
	uint32_t size = (uint32_t)m_MeshPartVector.size();
	for (uint32_t i = 0; i < size; ++i)
	{
		DeleteMem(m_MeshPartVector[i]);
	}
	m_MeshPartVector.clear();
}

XEResult Mesh::Load()
{
	XEAssert(m_GraphicDevice != nullptr);

	if(m_GraphicDevice == nullptr)
	{
		return XEResult::GraphicDeviceNull;
	}

	std::lock_guard<std::mutex> lock(m_GameResourceMutex);

	XEAssert(!m_FileName.empty());

	if(m_FileName.empty())
	{
		return XEResult::EmptyFilename;
	}
	
	/////////////////////////////////////////////
	//Clean Up memory before loading File
	CleanUp();
		
	/////////////////////////////////////////////
	//Start reading file
	std::ifstream meshFile;

	meshFile.open(m_FileName, std::ios::binary | std::ios::in);

	if(!meshFile.is_open())
	{
		XETODO("add log");

		return XEResult::OpenFileFail;
	}

	char* tempPtr = nullptr;
	uint32_t tempUInt32 = 0;
	int32_t tempInt32 = 0;
	uint32_t sizeToRead = 0;
	
	/////////////////////////////////////////////
	//Verify Header
	bool verifyHeader = XEGameContentHelpers::ReadFileHeaderAndVerify(meshFile, XE_CT_XE3DM_FILE_HEADER, XE_CT_XE3DM_FILE_VERSION_MAYOR, XE_CT_XE3DM_FILE_VERSION_MINOR, XE_CT_XE3DM_FILE_VERSION_REVISON);

	if(!verifyHeader)
	{
		XETODO("Add log");

		return XEResult::InvalidFileHeader;
	}
	
	/////////////////////////////////////////////
	//Get Name of Mesh
	this->SetFileNameWithoutLock(XEGameContentHelpers::ReadString(meshFile));
	
	/////////////////////////////////////////////
	//Read Number of Mesh Parts
	uint32_t numMeshesParts = 0;
	tempPtr = reinterpret_cast<char*>(&numMeshesParts);
	sizeToRead = sizeof(uint32_t);
	meshFile.read(tempPtr, sizeToRead);

	/////////////////////////////////////////////
	//Load Mesh Parts
	for (uint32_t i = 0; i < numMeshesParts; ++i)
	{		
		/////////////////////////////////////////////
		//Read Index Type
		XETODO("To change to 16 or 32");
		IndexBufferType idxType = IndexBufferType::Index16;
		tempPtr = reinterpret_cast<char*>(&idxType);
		sizeToRead = sizeof(uint32_t);
		meshFile.read(tempPtr, sizeToRead);
		
		/////////////////////////////////////////////
		//Read size of Index Buffer
		XETODO("add error when vtx or idx size is 0");
		uint32_t idxBufSize = 0;
		tempPtr = reinterpret_cast<char*>(&idxBufSize);
		sizeToRead = sizeof(uint32_t);
		meshFile.read(tempPtr, sizeToRead);

		/////////////////////////////////////////////
		//Read Index Buffer
		uint16_t* idxBuf = new uint16_t[idxBufSize];
		ZeroMemory(idxBuf, sizeof(uint16_t) * idxBufSize);
		tempPtr = reinterpret_cast<char*>(idxBuf);
		sizeToRead = sizeof(uint16_t) * idxBufSize;
		meshFile.read(tempPtr, sizeToRead);

		/////////////////////////////////////////////
		//Read Vertex Buffer Type
		VertexType vtxType = VertexType::VtxNone;
		tempPtr = reinterpret_cast<char*>(&vtxType);
		sizeToRead = sizeof(uint32_t);
		meshFile.read(tempPtr, sizeToRead);

		if(!XEImporterHelpers::IsVertexTypeValid(vtxType))
		{
			XETODO("add log");

			CleanUp();

			DeleteMemArr(idxBuf);

			return XEResult::Fail;
		}

		/////////////////////////////////////////////
		//Read size of Vertex Buffer
		uint32_t vtxBufSize = 0;
		tempPtr = reinterpret_cast<char*>(&vtxBufSize);
		sizeToRead = sizeof(uint32_t);
		meshFile.read(tempPtr, sizeToRead);

		/////////////////////////////////////////////
		//Read Vertex Buffer
		uint32_t sizeOfVertex = XEVertexHelper::GetVertexSizeByType(vtxType);
		uint8_t* vtxBuffer = new uint8_t[sizeOfVertex * vtxBufSize];
		ZeroMemory(vtxBuffer, sizeOfVertex * vtxBufSize);
		tempPtr = reinterpret_cast<char*>(vtxBuffer);
		sizeToRead = sizeOfVertex * vtxBufSize;
		meshFile.read(tempPtr, sizeToRead);

		/////////////////////////////////////////////
		//Create Vertex Buffer and Index Buffer Objects

		//Vertex Buffer
		IVertexBuffer* vertexBuffer = nullptr;
		XEResult ret = XEImporterHelpers::BuildVertexBuffer(m_GraphicDevice, &vertexBuffer, vtxType, vtxBuffer, vtxBufSize);

		if(ret != XEResult::Ok)
		{
			XETODO("add log");

			CleanUp();

			DeleteMemArr(idxBuf);
			
			DeleteMem(vtxBuffer);

			return ret;
		}

		//Index Buffer
		IndexBuffer* indexBuffer = new IndexBuffer(m_GraphicDevice);
		indexBuffer->CopyToIndexBuffer(idxBuf, idxBufSize);
		indexBuffer->BuildIndexBuffer();

		/////////////////////////////////////////////
		//Add variables to Mesh Part
		MeshPart* meshPart = new MeshPart();

		meshPart->SetIndexBuffer(indexBuffer);
		meshPart->SetVertexBuffer(vertexBuffer);

		m_MeshPartVector.push_back(meshPart);

		/////////////////////////////////////////////
		//Erase temp Buffer
		DeleteMemArr(idxBuf);

		DeleteMem(vtxBuffer);
	}

	/////////////////////////////////////////////
	//Read Bounding Shapes
	tempPtr = reinterpret_cast<char*>(&m_BoundingBox.m_MinPos);
	sizeToRead = sizeof(glm::vec3);
	meshFile.read(tempPtr, sizeToRead);

	tempPtr = reinterpret_cast<char*>(&m_BoundingBox.m_MaxPos);
	sizeToRead = sizeof(glm::vec3);
	meshFile.read(tempPtr, sizeToRead);

	tempPtr = reinterpret_cast<char*>(&m_BoundingSphere.m_Radius);
	sizeToRead = sizeof(float);
	meshFile.read(tempPtr, sizeToRead);

	/////////////////////////////////////////////
	//Read Footer 
	bool verifyFooter = XEGameContentHelpers::ReadFileFooterAndVerify(meshFile, XE_CT_XE3DM_FILE_FOOTER);
	if(!verifyFooter)
	{
		XETODO("Add Warning log");
	}

	/////////////////////////////////////////////
	//Finish
	meshFile.close();

	return XEResult::Ok;
}

XEResult Mesh::AddMeshPart(MeshPart* meshPart)
{		
	XEAssert(meshPart != nullptr);

	if(meshPart == nullptr)
	{
		return XEResult::NullParameter;
	}

	std::lock_guard<std::mutex> lock(m_GameResourceMutex);

	m_MeshPartVector.push_back(meshPart);

	return XEResult::Ok;
}

MeshPart* Mesh::GetMeshPart(uint32_t index)
{
	std::lock_guard<std::mutex> lock(m_GameResourceMutex);

	if(index >= (uint32_t)m_MeshPartVector.size())
	{
		return nullptr;
	}

	return m_MeshPartVector[index];
}

MeshPart* Mesh::operator[](uint32_t index)
{
	return GetMeshPart(index);
}

MeshPartVectorIt Mesh::begin()
{
	return m_MeshPartVector.begin();
}

MeshPartVectorIt Mesh::end()
{
	return m_MeshPartVector.end();
}

MeshPartVectorItConst Mesh::begin() const
{
	return m_MeshPartVector.begin();
}

MeshPartVectorItConst Mesh::end() const
{
	return m_MeshPartVector.end();
}

MeshPartVectorItConst Mesh::cbegin() const
{
	return m_MeshPartVector.cbegin();
}

MeshPartVectorItConst Mesh::cend() const
{
	return m_MeshPartVector.cend();
}
