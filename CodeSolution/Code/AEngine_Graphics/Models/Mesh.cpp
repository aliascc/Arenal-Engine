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
    AEAssert(m_GraphicDevice != nullptr);
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

AEResult Mesh::Load()
{
    AEAssert(m_GraphicDevice != nullptr);

    if(m_GraphicDevice == nullptr)
    {
        return AEResult::GraphicDeviceNull;
    }

    std::lock_guard<std::mutex> lock(m_GameResourceMutex);

    AEAssert(!m_FileName.empty());

    if(m_FileName.empty())
    {
        return AEResult::EmptyFilename;
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
        AETODO("add log");

        return AEResult::OpenFileFail;
    }

    char* tempPtr = nullptr;
    uint32_t tempUInt32 = 0;
    int32_t tempInt32 = 0;
    uint32_t sizeToRead = 0;
    
    /////////////////////////////////////////////
    //Verify Header
    bool verifyHeader = AEGameContentHelpers::ReadFileHeaderAndVerify(meshFile, AE_CT_AE3DM_FILE_HEADER, AE_CT_AE3DM_FILE_VERSION_MAYOR, AE_CT_AE3DM_FILE_VERSION_MINOR, AE_CT_AE3DM_FILE_VERSION_REVISON);

    if(!verifyHeader)
    {
        AETODO("Add log");

        return AEResult::InvalidFileHeader;
    }
    
    /////////////////////////////////////////////
    //Get Name of Mesh
    this->SetFileNameWithoutLock(AEGameContentHelpers::ReadString(meshFile));
    
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
        AETODO("To change to 16 or 32");
        IndexBufferType idxType = IndexBufferType::Index16;
        tempPtr = reinterpret_cast<char*>(&idxType);
        sizeToRead = sizeof(uint32_t);
        meshFile.read(tempPtr, sizeToRead);
        
        /////////////////////////////////////////////
        //Read size of Index Buffer
        AETODO("add error when vtx or idx size is 0");
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

        if(!AEImporterHelpers::IsVertexTypeValid(vtxType))
        {
            AETODO("add log");

            CleanUp();

            DeleteMemArr(idxBuf);

            return AEResult::Fail;
        }

        /////////////////////////////////////////////
        //Read size of Vertex Buffer
        uint32_t vtxBufSize = 0;
        tempPtr = reinterpret_cast<char*>(&vtxBufSize);
        sizeToRead = sizeof(uint32_t);
        meshFile.read(tempPtr, sizeToRead);

        /////////////////////////////////////////////
        //Read Vertex Buffer
        uint32_t sizeOfVertex = AEVertexHelper::GetVertexSizeByType(vtxType);
        uint8_t* vtxBuffer = new uint8_t[sizeOfVertex * vtxBufSize];
        ZeroMemory(vtxBuffer, sizeOfVertex * vtxBufSize);
        tempPtr = reinterpret_cast<char*>(vtxBuffer);
        sizeToRead = sizeOfVertex * vtxBufSize;
        meshFile.read(tempPtr, sizeToRead);

        /////////////////////////////////////////////
        //Create Vertex Buffer and Index Buffer Objects

        //Vertex Buffer
        IVertexBuffer* vertexBuffer = nullptr;
        AEResult ret = AEImporterHelpers::BuildVertexBuffer(m_GraphicDevice, &vertexBuffer, vtxType, vtxBuffer, vtxBufSize);

        if(ret != AEResult::Ok)
        {
            AETODO("add log");

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
    bool verifyFooter = AEGameContentHelpers::ReadFileFooterAndVerify(meshFile, AE_CT_AE3DM_FILE_FOOTER);
    if(!verifyFooter)
    {
        AETODO("Add Warning log");
    }

    /////////////////////////////////////////////
    //Finish
    meshFile.close();

    return AEResult::Ok;
}

AEResult Mesh::AddMeshPart(MeshPart* meshPart)
{        
    AEAssert(meshPart != nullptr);

    if(meshPart == nullptr)
    {
        return AEResult::NullParameter;
    }

    std::lock_guard<std::mutex> lock(m_GameResourceMutex);

    m_MeshPartVector.push_back(meshPart);

    return AEResult::Ok;
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
