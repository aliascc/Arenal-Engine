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

/*************************
*   3rd Party Includes   *
**************************/

/***************************
*   Game Engine Includes   *
****************************/
#include "MeshAsset.h"
#include "ModelAsset.h"
#include "GraphicDevice.h"
#include "SkeletonAsset.h"
#include "AnimationAsset.h"
#include "Base\BaseFunctions.h"
#include "Resource\GameResourceManager.h"

//Always include last
#include "Memory\MemLeaks.h"

/********************
*   Function Defs   *
*********************/
AETODO("Check if this class needs a mutex");
ModelAsset::ModelAsset(const std::wstring& filePath, GameResourceManager* gameResourceManager, GraphicDevice* graphicDevice)
    : GameAsset(GameContentType::Model, filePath, gameResourceManager)
    , m_GraphicDevice(graphicDevice)
{
    AEAssert(m_GraphicDevice != nullptr);
}

ModelAsset::~ModelAsset()
{
    CleanUp();
}

void ModelAsset::CleanUp()
{
    for (auto gameAssetLoadedIt : m_MeshAssetMap)
    {
        GameAssetLoadStatus<MeshAsset>& gameAssetLoaded = gameAssetLoadedIt.second;

        DeleteMem(gameAssetLoaded.m_Asset);
    }
    m_MeshAssetMap.clear();

    for (auto gameAssetLoadedIt : m_AnimationAssetMap)
    {
        GameAssetLoadStatus<AnimationAsset>& gameAssetLoaded = gameAssetLoadedIt.second;

        DeleteMem(gameAssetLoaded.m_Asset);
    }
    m_AnimationAssetMap.clear();

    DeleteMem(m_SkeletonAsset);
}

AEResult ModelAsset::LoadAssetResource()
{
    AEAssert(m_GameResourceManager != nullptr);
    if(m_GameResourceManager == nullptr)
    {
        return AEResult::GameResourceManagerNull;
    }

    AEAssert(!m_FilePath.empty());
    if(m_FilePath.empty())
    {
        return AEResult::EmptyFilename;
    }

    AEResult ret = AEResult::Ok;
    
    ////////////////////////////////////////
    //Load Model
    ret = LoadFile();

    if(ret != AEResult::Ok)
    {
        return ret;
    }

    ////////////////////////////////////////
    //Load Meshes
    for (auto gameAssetLoadedIt : m_MeshAssetMap)
    {
        GameAssetLoadStatus<MeshAsset>& gameAssetLoaded = gameAssetLoadedIt.second;

        ret = gameAssetLoaded.m_Asset->LoadAsset();
        if (ret != AEResult::Ok)
        {
            return ret;
        }
    }

    ////////////////////////////////////////
    //Load Skeleton
    if(m_SkeletonAsset != nullptr)
    {
        m_SkeletonAsset->LoadAsset();
        
        if(ret != AEResult::Ok)
        {
            return ret;
        }
    }

    ////////////////////////////////////////
    //Load Animations
    for (auto gameAssetLoadedIt : m_AnimationAssetMap)
    {
        GameAssetLoadStatus<AnimationAsset>& gameAssetLoaded = gameAssetLoadedIt.second;

        ret = gameAssetLoaded.m_Asset->LoadAsset();
        if (ret != AEResult::Ok)
        {
            return ret;
        }
    }

    ////////////////////////////////////////
    //Finish
    return AEResult::Ok;
}

AEResult ModelAsset::LoadFile()
{
    AEAssert(m_GraphicDevice != nullptr);
    if (m_GraphicDevice == nullptr)
    {
        return AEResult::GraphicDeviceNull;
    }

    AEAssert(m_GameResourceManager != nullptr);
    if (m_GameResourceManager == nullptr)
    {
        return AEResult::GameResourceManagerNull;
    }

    AEAssert(!m_FilePath.empty());
    if(m_FilePath.empty())
    {
        return AEResult::EmptyFilename;
    }

    AEResult ret = AEResult::Ok;
    
    std::ifstream modelFile;

    modelFile.open(m_FilePath, std::ios::binary | std::ios::in);

    if(!modelFile.is_open())
    {
        AETODO("add log");
        return AEResult::OpenFileFail;
    }
    
    /////////////////////////////////////////////
    //Verify Header
    bool verifyHeader = AEGameContentHelpers::ReadFileHeaderAndVerify(modelFile, AE_CT_AE3D_FILE_HEADER, AE_CT_AE3D_FILE_VERSION_MAYOR, AE_CT_AE3D_FILE_VERSION_MINOR, AE_CT_AE3D_FILE_VERSION_REVISON);

    if(!verifyHeader)
    {
        AETODO("Add log");
        return AEResult::InvalidFileHeader;
    }

    /////////////////////////////////////////////
    //Initial check pass, Read Information for
    //Model Asset

    /////////////////////////////////////////////
    //Read Name of Model
    this->SetName(AEGameContentHelpers::ReadString(modelFile));

    /////////////////////////////////////////////
    //Read Meshes Filenames
    ret = ReadModelMeshFiles(modelFile);
    if(ret != AEResult::Ok)
    {
        AETODO("Add log");

        CleanUp();

        return ret;
    }
    
    /////////////////////////////////////////////
    //Read Skeleton Filename
    ret = ReadModelSkeletonFile(modelFile);
    if(ret != AEResult::Ok)
    {
        AETODO("Add log");

        CleanUp();

        return ret;
    }
    
    /////////////////////////////////////////////
    //Read Animations Filenames 
    ret = ReadModelAnimationFiles(modelFile);
    if(ret != AEResult::Ok)
    {
        AETODO("Add log");
        
        CleanUp();

        return ret;
    }
    
    /////////////////////////////////////////////
    //Read Footer 
    bool verifyFooter = AEGameContentHelpers::ReadFileFooterAndVerify(modelFile, AE_CT_AE3D_FILE_FOOTER);
    if(!verifyFooter)
    {
        AETODO("Add Warning log");
    }

    ////////////////////////////////////////
    //Finish
    modelFile.close();

    return AEResult::Ok;
}

AEResult ModelAsset::ReadModelMeshFiles(std::ifstream& fileStream)
{
    char* tempPtr = nullptr;
    uint32_t sizeToRead = 0;

    /////////////////////////////////////////////
    //Read Number of Meshes
    uint32_t numMeshes = 0;
    tempPtr = reinterpret_cast<char*>(&numMeshes);
    sizeToRead = sizeof(uint32_t);
    fileStream.read(tempPtr, sizeToRead);
    
    /////////////////////////////////////////////
    //Set all Meshes to unloaded
    for (auto gameAssetLoadedIt : m_MeshAssetMap)
    {
        gameAssetLoadedIt.second.m_IsLoaded = false;
    }

    /////////////////////////////////////////////
    //Get Mesh File Names
    for(uint32_t i = 0; i < numMeshes; ++i)
    {
        /////////////////////////////////////////////
        //Read Mesh Name and File Name
        std::wstring meshName = AEGameContentHelpers::ReadString(fileStream);
        std::wstring meshPath = AEGameContentHelpers::ReadString(fileStream);

        auto mapIt = m_MeshAssetMap.find(meshName);
        if (mapIt != m_MeshAssetMap.end())
        {
            GameAssetLoadStatus<MeshAsset>& gameAssetLoaded = mapIt->second;
            MeshAsset* meshAsset = gameAssetLoaded.m_Asset;

            AEAssert(meshAsset != nullptr);
            if (meshAsset == nullptr)
            {
                return AEResult::NullObj;
            }

            meshAsset->SetFilePath(meshPath);

            gameAssetLoaded.m_IsLoaded = true;
        }
        else
        {
            MeshAsset* meshAsset = new MeshAsset(meshPath, m_GameResourceManager, m_GraphicDevice);

            meshAsset->SetFilePath(meshPath);
            meshAsset->SetName(meshName);
            meshAsset->SetCustomName(meshName);
            meshAsset->SetParentAssetID(this->GetUniqueAssetID());

            m_MeshAssetMap[meshName] = GameAssetLoadStatus<MeshAsset>(meshAsset, true);
        }
    }

    /////////////////////////////////////////////
    //Remove any unused Asset
    for (auto gameAssetLoadedIt : m_MeshAssetMap)
    {
        GameAssetLoadStatus<MeshAsset>& gameAssetLoaded = gameAssetLoadedIt.second;

        if (!gameAssetLoaded.m_IsLoaded)
        {
            DeleteMem(gameAssetLoaded.m_Asset);

            m_MeshAssetMap.erase(gameAssetLoadedIt.first);
        }
    }

    return AEResult::Ok;
}

AEResult ModelAsset::ReadModelSkeletonFile(std::ifstream& fileStream)
{    
    uint32_t tempUInt32 = 0;
    char* tempPtr = nullptr;
    uint32_t sizeToRead = 0;
    
    /////////////////////////////////////////////
    //Write if Skeleton is present
    bool skeletonExists = false;
    tempPtr = reinterpret_cast<char*>(&skeletonExists);
    sizeToRead = sizeof(bool);
    fileStream.read(tempPtr, sizeToRead);
    
    if(!skeletonExists)
    {
        return AEResult::Ok;
    }

    /////////////////////////////////////////////
    //Read Skeleton Name and File Name
    std::wstring skeletonName = AEGameContentHelpers::ReadString(fileStream);
    std::wstring skeletonFile = AEGameContentHelpers::ReadString(fileStream);

    if(m_SkeletonAsset == nullptr || (m_SkeletonAsset->GetName().compare(skeletonName) != 0))
    {
        DeleteMem(m_SkeletonAsset);

        m_SkeletonAsset = new SkeletonAsset(skeletonFile, m_GameResourceManager);
        m_SkeletonAsset->SetName(skeletonName);
        m_SkeletonAsset->SetCustomName(skeletonName);
        m_SkeletonAsset->SetParentAssetID(this->GetUniqueAssetID());
    }
    else
    {
        m_SkeletonAsset->SetFilePath(skeletonFile);
    }

    return AEResult::Ok;
}

AEResult ModelAsset::ReadModelAnimationFiles(std::ifstream& fileStream)
{
    char* tempPtr = nullptr;
    uint32_t sizeToRead = 0;
    
    /////////////////////////////////////////////
    //Read Number of Animations
    uint32_t numAnim = 0;
    tempPtr = reinterpret_cast<char*>(&numAnim);
    sizeToRead = sizeof(uint32_t);
    fileStream.read(tempPtr, sizeToRead);
    
    /////////////////////////////////////////////
    //Set Vector Size and initialize unassigned to null
    for (auto gameAssetLoadedIt : m_AnimationAssetMap)
    {
        gameAssetLoadedIt.second.m_IsLoaded = false;
    }

    /////////////////////////////////////////////
    //Get Animations File Names
    for(uint32_t i = 0; i < numAnim; ++i)
    {
        /////////////////////////////////////////////
        //Read Animation Name and File Name
        std::wstring animName = AEGameContentHelpers::ReadString(fileStream);
        std::wstring animFilePath = AEGameContentHelpers::ReadString(fileStream);

        auto mapIt = m_AnimationAssetMap.find(animName);
        if (mapIt != m_AnimationAssetMap.end())
        {
            GameAssetLoadStatus<AnimationAsset>& gameAssetLoaded = mapIt->second;
            AnimationAsset* animAsset = gameAssetLoaded.m_Asset;

            AEAssert(animAsset != nullptr);
            if (animAsset == nullptr)
            {
                return AEResult::NullObj;
            }

            animAsset->SetFilePath(animFilePath);

            gameAssetLoaded.m_IsLoaded = true;
        }
        else
        {
            AnimationAsset* animAsset = new AnimationAsset(animFilePath, m_GameResourceManager);

            animAsset->SetFilePath(animFilePath);
            animAsset->SetName(animName);
            animAsset->SetCustomName(animName);
            animAsset->SetParentAssetID(this->GetUniqueAssetID());

            m_AnimationAssetMap[animName] = GameAssetLoadStatus<AnimationAsset>(animAsset, true);
        }
    }

    /////////////////////////////////////////////
    //Remove any unused Asset
    for (auto gameAssetLoadedIt : m_AnimationAssetMap)
    {
        GameAssetLoadStatus<AnimationAsset>& gameAssetLoaded = gameAssetLoadedIt.second;

        if (!gameAssetLoaded.m_IsLoaded)
        {
            DeleteMem(gameAssetLoaded.m_Asset);

            m_MeshAssetMap.erase(gameAssetLoadedIt.first);
        }
    }

    return AEResult::Ok;
}
