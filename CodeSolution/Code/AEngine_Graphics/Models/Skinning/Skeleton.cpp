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

/*************************
*   Precompiled Header   *
**************************/
#include "precomp_graphics.h"

/**********************
*   System Includes   *
***********************/

/*************************
*   3rd Party Includes   *
**************************/

/***************************
*   Game Engine Includes   *
****************************/
#include "Bone.h"
#include "Skeleton.h"
#include "GameContentDefs.h"
#include "Math\AEMathDefs.h"
#include "Base\BaseFunctions.h"

//Always include last
#include "Memory\MemLeaks.h"

/********************
*   Function Defs   *
*********************/
Skeleton::Skeleton(const std::string& resourceName)
    : GameResource(resourceName, GameResourceType::Skeleton)
{
}

Skeleton::~Skeleton()
{
    CleanUp();
}

void Skeleton::CleanUp()
{
    uint32_t size = (uint32_t)m_BoneHierarchy.size();
    for(uint32_t i = 0; i < size; ++i)
    {
        DeleteMem(m_BoneHierarchy[i]);
    }

    m_BoneHierarchy.clear();
}

AEResult Skeleton::Load()
{
    std::lock_guard<std::mutex> lock(m_GameResourceMutex);

    AEAssert(!m_FileName.empty());
    if(m_FileName.empty())
    {
        return AEResult::EmptyFilename;
    }
    
    /////////////////////////////////////////////
    //Clean Up memory before loading File
    CleanUp();

    AEResult ret = AEResult::Ok;
    
    /////////////////////////////////////////////
    //Start reading file
    std::ifstream skeletonFile;

    skeletonFile.open(m_FileName, std::ios::binary | std::ios::in);

    if(!skeletonFile.is_open())
    {
        AETODO("add log");

        return AEResult::OpenFileFail;
    }

    char* tempPtr = nullptr;
    uint32_t sizeToRead = 0;
    
    /////////////////////////////////////////////
    //Verify Header
    bool verifyHeader = AEGameContentHelpers::ReadFileHeaderAndVerify(skeletonFile, AE_CT_AE3DS_FILE_HEADER, AE_CT_AE3DS_FILE_VERSION_MAYOR, AE_CT_AE3DS_FILE_VERSION_MINOR, AE_CT_AE3DS_FILE_VERSION_REVISON);

    if(!verifyHeader)
    {
        AETODO("Add log");

        return AEResult::InvalidFileHeader;
    }

    /////////////////////////////////////////////
    //Read Number of Bones
    uint32_t numBones = 0;
    tempPtr = reinterpret_cast<char*>(&numBones);
    sizeToRead = sizeof(uint32_t);
    skeletonFile.read(tempPtr, sizeToRead);
    
    /////////////////////////////////////////////
    //Read Bones
    for(uint32_t i = 0; i < numBones; ++i)
    {
        Bone* bone = new Bone();
        glm::mat4 tempMat = AEMathHelpers::Mat4Identity;
        int32_t tempInt32 = 0;

        /////////////////////////////////////////////
        //Write name of Bone and size of Name
        bone->SetName(AEGameContentHelpers::ReadString(skeletonFile));

        /////////////////////////////////////////////
        //Read Bone Matrices
        sizeToRead = sizeof(glm::mat4);
        tempPtr = reinterpret_cast<char*>(&tempMat);

        //Local Matrix
        skeletonFile.read(tempPtr, sizeToRead);
        bone->SetLocalMatrix(tempMat);

        //World Matrix
        skeletonFile.read(tempPtr, sizeToRead);
        bone->SetWorldMatrix(tempMat);
         
        //Off Set Matrix
        skeletonFile.read(tempPtr, sizeToRead);
        bone->SetOffSetMatrix(tempMat);
        
        /////////////////////////////////////////////
        //Read Bone Indices
        tempPtr = reinterpret_cast<char*>(&tempInt32);
        sizeToRead = sizeof(int32_t);

        //Parent Index
        skeletonFile.read(tempPtr, sizeToRead);
        bone->SetParentIndex(tempInt32);

        //Bone Index
        skeletonFile.read(tempPtr, sizeToRead);
        bone->SetIndex(tempInt32);
        
        /////////////////////////////////////////////
        //Add Bone to Skeleton
        m_BoneHierarchy.push_back(bone);
    }
    
    /////////////////////////////////////////////
    //Read Footer 
    bool verifyFooter = AEGameContentHelpers::ReadFileFooterAndVerify(skeletonFile, AE_CT_AE3DS_FILE_FOOTER);
    if(!verifyFooter)
    {
        AETODO("Add Warning log");
    }
    
    /////////////////////////////////////////////
    //Finish
    skeletonFile.close();

    return AEResult::Ok;
}

Bone* Skeleton::GetBone(uint32_t index) const
{    
    if(index >= (uint32_t)m_BoneHierarchy.size())
    {
        return nullptr;
    }

    return m_BoneHierarchy[index];
}

Bone* Skeleton::operator[](uint32_t index) const
{
    return GetBone(index);
}

AEResult Skeleton::AddBone(Bone* bone)
{
    AEAssert(bone != nullptr);

    if(bone == nullptr)
    {
        return AEResult::NullParameter;
    }
    
    std::lock_guard<std::mutex> lock(m_GameResourceMutex);

    AETODO("add warning");
    //bone->GetIndex() != m_BoneHierarchy.size(); //should be the same

    m_BoneHierarchy.push_back(bone);

    return AEResult::Ok;
}
