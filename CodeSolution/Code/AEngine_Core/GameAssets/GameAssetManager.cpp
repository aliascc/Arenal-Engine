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
#include "precomp_core.h"

/**********************
*   System Includes   *
***********************/

/*************************
*   3rd Party Includes   *
**************************/

/***************************
*   Game Engine Includes   *
****************************/
#include "AudioDefs.h"
#include "GameAsset.h"
#include "AudioManager.h"
#include "RawGameAsset.h"
#include "GraphicDevice.h"
#include "GameAssetManager.h"
#include "Assets\MeshAsset.h"
#include "Assets\ModelAsset.h"
#include "Assets\AudioAsset.h"
#include "Assets\ShaderAsset.h"
#include "Writers\WriterAE3D.h"
#include "Writers\WriterHLSL.h"
#include "Assets\TextureAsset.h"
#include "Assets\SkeletonAsset.h"
#include "Content\ModelContent.h"
#include "Content\ShaderContent.h"
#include "Assets\AnimationAsset.h"
#include "Importers\ImporterHLSL.h"
#include "Models\Custom\CubeMesh.h"
#include "Importers\ImporterAssimp.h"
#include "Models\Custom\IcoSphereMesh.h"
#include "Assets\GameObjectScriptAsset.h"
#include "Resource\GameResourceManager.h"
#include "AngelScript\AngelScriptManager.h"
#include "Shaders\Custom\DiffuseTexturePS.h"
#include "Shaders\Custom\DiffuseTextureVS.h"

//Always include last
#include "Memory\MemLeaks.h"

/********************
*   Function Defs   *
*********************/

AETODO("Check for Mutex");
AETODO("When importing need to handle change of directory");
AETODO("When importing need to handle change of sub content type");
GameAssetManager::GameAssetManager(GraphicDevice& graphicDevice, GameResourceManager& gameResourceManager, AngelScriptManager& angelScriptManager, AudioManager& audioManager, const std::string& projectDir, const std::string& outputDirAssets)
    : m_GraphicDevice(graphicDevice)
    , m_GameResourceManager(gameResourceManager)
    , m_AngelScriptManager(angelScriptManager)
    , m_AudioManager(audioManager)
    , m_ProjectDirectory(projectDir)
    , m_OutputDirAssets(outputDirAssets)
{
    /////////////////////////////
    //Verify we have not reach the max IDs
    static_assert((uint32_t)GameAssetDefaultIDs::__Max_GameAssetDefaultIDs < AE_GAME_ASSET_UNIQUE_ID_USER_START, "Game Asset Default IDs overpass custom start ID");
}

GameAssetManager::~GameAssetManager()
{
    CleanUp();
}

void GameAssetManager::CleanUp()
{
    for(auto rawGAIT : m_RawGameAssetMap)
    {
        DeleteMem(rawGAIT.second);
    }
    m_RawGameAssetMap.clear();

    ////////////////////////////////////////////////////
    // - Set to null all child assets as Parent will be in charge of deletion
    // - Set null to all callbacks, so we won't have notifications of asset deletion
    for (auto gameAssetPair : m_GameAssetMap)
    {
        GameAsset* gameAsset = gameAssetPair.second;

        gameAsset->m_OnGameAssetDeletionNotifyManagerEvent = nullptr;
        if(gameAsset->GetParentAssetID() != 0)
        {
            m_GameAssetMap[gameAssetPair.first] = nullptr;
        }
    }

    ////////////////////////////////////////////////////
    // - Delete remaining Assets
    for(auto gaPair : m_GameAssetMap)
    {
        DeleteMem(gaPair.second);
    }
    m_GameAssetMap.clear();
}

void GameAssetManager::GameAssetDeleted(uint64_t assetID)
{
    auto gameAssetIT = m_GameAssetMap.find(assetID);

    if(gameAssetIT == m_GameAssetMap.end())
    {
        return;
    }

    m_GameAssetMap.erase(gameAssetIT);
}

uint64_t GameAssetManager::GetNextUniqueAssetID()
{
    std::lock_guard<std::mutex> lock(m_UniqueIDGenerateMutex);

    return m_UniqueAssetIDGen++;
}

RawGameAsset* GameAssetManager::GetRawGameAsset(uint64_t id)
{
    if(!m_IsReady)
    {
        return nullptr;
    }

    std::lock_guard<std::mutex> lock(m_GameAssetManagerMutex);
    
    if(!RawGameAssetExists(id))
    {
        return nullptr;
    }

    return m_RawGameAssetMap[id];
}

RawGameAsset* GameAssetManager::GetRawGameAssetByGameAssetID(uint64_t gameAssetID)
{
    if (!m_IsReady)
    {
        return nullptr;
    }

    //No need for lock as this is an internal method only

    for (auto rawAssetPair : m_RawGameAssetMap)
    {
        if (rawAssetPair.second->GetUniqueAssociatedAssetID() == gameAssetID)
        {
            return rawAssetPair.second;
        }
    }

    return nullptr;
}

GameAsset* GameAssetManager::GetGameAsset(uint64_t id)
{
    if(!m_IsReady)
    {
        return nullptr;
    }

    std::lock_guard<std::mutex> lock(m_GameAssetManagerMutex);
    
    if(!GameAssetExists(id))
    {
        return nullptr;
    }

    return m_GameAssetMap[id];
}

AEResult GameAssetManager::Initialize()
{
    if(m_IsReady)
    {
        return AEResult::Ok;
    }

    std::lock_guard<std::mutex> lock(m_GameAssetManagerMutex);

    AETODO("Check return");
    ImportBuiltInMeshes();

    AETODO("Check return");
    ImportBuiltInShader();

    AETODO("Read file to load assets and unique id gen sequence");

    m_IsReady = true;

    return AEResult::Ok;
}

AEResult GameAssetManager::ImportBuiltInMeshes()
{
    AEResult ret = AEResult::Ok;

    ////////////////////////////////////////
    //Import Cube Mesh

    if (m_GameAssetBuiltIns.m_CubeMesh == nullptr)
    {
        m_GameAssetBuiltIns.m_CubeMesh = new CubeMesh(m_GraphicDevice, AE_CUBE_MESH_RESOURCE_NAME);

        ret = m_GameResourceManager.ManageGameResource(m_GameAssetBuiltIns.m_CubeMesh, AE_CUBE_MESH_FAKE_FILE_PATH);
        if (ret != AEResult::Ok)
        {
            AETODO("Add log");
            AERelease(m_GameAssetBuiltIns.m_CubeMesh);

            return AEResult::ResourceManagedFailed;
        }
    }

    if (m_GameAssetMap.find((uint64_t)GameAssetDefaultIDs::CubeMesh) == m_GameAssetMap.end())
    {
        MeshAsset* meshAsset = new MeshAsset(AE_CUBE_MESH_FAKE_FILE_PATH, m_GameResourceManager, m_GraphicDevice);

        meshAsset->SetUniqueAssetID((uint64_t)GameAssetDefaultIDs::CubeMesh);
        meshAsset->m_OnGameAssetDeletionNotifyManagerEvent = std::bind(&GameAssetManager::GameAssetDeleted, this, std::placeholders::_1);

        m_GameAssetMap[(uint64_t)GameAssetDefaultIDs::CubeMesh] = meshAsset;

        meshAsset->SetName(AE_CUBE_MESH_NAME);
        meshAsset->SetIsBuiltInAsset(true);

        AETODO("Check return");
        meshAsset->LoadAsset();
    }

    ////////////////////////////////////////
    //Import Sphere Mesh

    if (m_GameAssetBuiltIns.m_IcoSphereMesh == nullptr)
    {
        m_GameAssetBuiltIns.m_IcoSphereMesh = new IcoSphereMesh(m_GraphicDevice, AE_ICO_SPHERE_MESH_RESOURCE_NAME);

        ret = m_GameResourceManager.ManageGameResource(m_GameAssetBuiltIns.m_IcoSphereMesh, AE_ICO_SPHERE_MESH_FAKE_FILE_PATH);
        if (ret != AEResult::Ok)
        {
            AETODO("Add log");
            AERelease(m_GameAssetBuiltIns.m_IcoSphereMesh);

            return AEResult::ResourceManagedFailed;
        }
    }

    if (m_GameAssetMap.find((uint64_t)GameAssetDefaultIDs::SphereMesh) == m_GameAssetMap.end())
    {
        MeshAsset* meshAsset = new MeshAsset(AE_ICO_SPHERE_MESH_FAKE_FILE_PATH, m_GameResourceManager, m_GraphicDevice);

        meshAsset->SetUniqueAssetID((uint64_t)GameAssetDefaultIDs::SphereMesh);
        meshAsset->m_OnGameAssetDeletionNotifyManagerEvent = std::bind(&GameAssetManager::GameAssetDeleted, this, std::placeholders::_1);

        m_GameAssetMap[(uint64_t)GameAssetDefaultIDs::SphereMesh] = meshAsset;

        meshAsset->SetName(AE_ICO_SPHERE_MESH_NAME);
        meshAsset->SetIsBuiltInAsset(true);

        AETODO("Check return");
        meshAsset->LoadAsset();
    }

    ////////////////////////////////////////
    //Finish
    return AEResult::Ok;
}

AEResult GameAssetManager::ImportBuiltInShader()
{
    if (ImportBuiltInDiffuseTextureShader() != AEResult::Ok)
    {
        return AEResult::Fail;
    }

    return AEResult::Ok;
}

AEResult GameAssetManager::ImportBuiltInDiffuseTextureShader()
{
    AEResult ret = AEResult::Ok;

    ///////////////////////////
    //Import Diffuse Texture VS

    if (m_GameAssetBuiltIns.m_DiffuseTextureVS == nullptr)
    {
        m_GameAssetBuiltIns.m_DiffuseTextureVS = new DiffuseTextureVS(m_GraphicDevice, AE_DIFFUSE_TEXTURE_VS_NAME);

        ret = m_GameResourceManager.ManageGameResource(m_GameAssetBuiltIns.m_DiffuseTextureVS, AE_DIFFUSE_TEXTURE_VS_FAKE_FILE_PATH);
        if (ret != AEResult::Ok)
        {
            AETODO("Add log");
            AERelease(m_GameAssetBuiltIns.m_DiffuseTextureVS);

            return AEResult::ResourceManagedFailed;
        }
    }

    if (m_GameAssetMap.find((uint64_t)GameAssetDefaultIDs::DiffuseTextureVS) == m_GameAssetMap.end())
    {
        ShaderAsset* shaderAsset = new ShaderAsset(AE_DIFFUSE_TEXTURE_VS_FAKE_FILE_PATH, m_GameResourceManager, ShaderType::VertexShader, m_GraphicDevice);

        shaderAsset->SetUniqueAssetID((uint64_t)GameAssetDefaultIDs::DiffuseTextureVS);
        shaderAsset->m_OnGameAssetDeletionNotifyManagerEvent = std::bind(&GameAssetManager::GameAssetDeleted, this, std::placeholders::_1);

        m_GameAssetMap[(uint64_t)GameAssetDefaultIDs::DiffuseTextureVS] = shaderAsset;

        shaderAsset->SetName(AE_DIFFUSE_TEXTURE_VS_NAME);
        shaderAsset->SetIsBuiltInAsset(true);

        AETODO("Check return");
        shaderAsset->LoadAsset();
    }

    ///////////////////////////
    //Import Diffuse Texture PS

    if (m_GameAssetBuiltIns.m_DiffuseTexturePS == nullptr)
    {
        m_GameAssetBuiltIns.m_DiffuseTexturePS = new DiffuseTexturePS(m_GraphicDevice, AE_DIFFUSE_TEXTURE_PS_NAME);

        ret = m_GameResourceManager.ManageGameResource(m_GameAssetBuiltIns.m_DiffuseTexturePS, AE_DIFFUSE_TEXTURE_PS_FAKE_FILE_PATH);
        if (ret != AEResult::Ok)
        {
            AETODO("Add log");
            AERelease(m_GameAssetBuiltIns.m_DiffuseTexturePS);

            return AEResult::ResourceManagedFailed;
        }
    }

    if (m_GameAssetMap.find((uint64_t)GameAssetDefaultIDs::DiffuseTexturePS) == m_GameAssetMap.end())
    {
        ShaderAsset* shaderAsset = new ShaderAsset(AE_DIFFUSE_TEXTURE_PS_FAKE_FILE_PATH, m_GameResourceManager, ShaderType::PixelShader, m_GraphicDevice);

        shaderAsset->SetUniqueAssetID((uint64_t)GameAssetDefaultIDs::DiffuseTexturePS);
        shaderAsset->m_OnGameAssetDeletionNotifyManagerEvent = std::bind(&GameAssetManager::GameAssetDeleted, this, std::placeholders::_1);

        m_GameAssetMap[(uint64_t)GameAssetDefaultIDs::DiffuseTexturePS] = shaderAsset;

        shaderAsset->SetName(AE_DIFFUSE_TEXTURE_PS_NAME);
        shaderAsset->SetIsBuiltInAsset(true);

        AETODO("Check return");
        shaderAsset->LoadAsset();
    }

    ///////////////////////////
    //Finish
    return AEResult::Ok;
}

AEResult GameAssetManager::CreateNewRawGameAsset(const std::string& filePath, GameContentSubtype contentSubtype, uint64_t* rawAssetID)
{
    if(!m_IsReady)
    {
        return AEResult::NotReady;
    }

    AEAssert(!filePath.empty());
    if(filePath.empty())
    {
        return AEResult::EmptyFilename;
    }

    if(!boost::filesystem::exists(filePath))
    {
        return AEResult::FileNotPresent;
    }

    std::lock_guard<std::mutex> lock(m_GameAssetManagerMutex);

    AETODO("Copy raw file to raw directory if it is not present currently there");
    std::string rawAssetFilepath = AE_Base::GetRelativePath(filePath);
    RawGameAsset* rawGA = new RawGameAsset(rawAssetFilepath, m_ProjectDirectory, AE_Base::GetFilenameOnly(filePath));

    rawGA->SetContentSubtype(contentSubtype);

    AEResult ret = rawGA->ParseRawGameAssetFile();
    if(ret != AEResult::Ok)
    {
        DeleteMem(rawGA);

        return ret;
    }

    m_RawGameAssetMap[rawGA->GetUniqueID()] = rawGA;

    if(rawAssetID != nullptr)
    {
        *rawAssetID = rawGA->GetUniqueID();
    }

    return AEResult::Ok;
}

AEResult GameAssetManager::CheckForLatestRawGameAssets()
{
    if(!m_IsReady)
    {
        return AEResult::NotReady;
    }

    for(auto rawGAIT : m_RawGameAssetMap)
    {
        RawGameAsset* rawGA = rawGAIT.second;

        if(rawGA->CheckIfLatest() != AEResult::Ok)
        {
            AETODO("Add log here");
        }
    }

    return AEResult::Ok;
}

AEResult GameAssetManager::CheckForLatestRawGameAssetsAndImport()
{
    if(!m_IsReady)
    {
        return AEResult::NotReady;
    }
    
    std::lock_guard<std::mutex> lock(m_GameAssetManagerMutex);

    AEResult ret = CheckForLatestRawGameAssets();

    if(ret != AEResult::Ok)
    {
        return ret;
    }

    for(auto rawGAIT : m_RawGameAssetMap)
    {
        RawGameAsset* rawGA = rawGAIT.second;

        if(rawGA->IsReloadNeeded())
        {
            if(ImportRawGameAssetWithoutLock(rawGA->GetUniqueID()) != AEResult::Ok)
            {
                AETODO("add warning");
            }
        }
    }

    return AEResult::Ok;
}

AEResult GameAssetManager::ImportRawGameAsset(uint64_t rawAssetID)
{
    if(!m_IsReady)
    {
        return AEResult::NotReady;
    }

    std::lock_guard<std::mutex> lock(m_GameAssetManagerMutex);

    return ImportRawGameAssetWithoutLock(rawAssetID);
}

AEResult GameAssetManager::ImportRawGameAssetWithoutLock(uint64_t rawAssetID)
{
    if(!m_IsReady)
    {
        return AEResult::NotReady;
    }

    if (!RawGameAssetExists(rawAssetID))
    {
        return AEResult::NotFound;
    }

    AEResult ret = AEResult::Ok;

    AETODO("Lock game loop when loading raw asset");
    RawGameAsset* rawGA = m_RawGameAssetMap[rawAssetID];

    ret = rawGA->CheckIfLatest();
    if(ret != AEResult::Ok)
    {
        AETODO("set better return code");
        return AEResult::Fail;
    }

    if(!rawGA->IsReloadNeeded())
    {
        return AEResult::Ok;
    }

    switch (rawGA->GetContentType())
    {
        case GameContentType::Model:
            ret = ImportModel(rawGA);
            break;

        case GameContentType::Shader:
            ret = ImportShader(rawGA);
            break;

        case GameContentType::Texture:
            ret = ImportTexture(rawGA);
            break;

        case GameContentType::GameObjectScript:
            ret = ImportGameObjectScript(rawGA);
            break;

        case GameContentType::Audio:
            ret = ImportAudio(rawGA);
            break;

        default:
            AETODO("Add Log");
            ret = AEResult::InvalidObjType;
            break;
    }

    if(ret == AEResult::Ok)
    {
        rawGA->ImportDone();
    }

    return ret;
}

AEResult GameAssetManager::ImportModel(RawGameAsset* rawGA)
{
    if(!m_IsReady)
    {
        return AEResult::NotReady;
    }

    AEAssert(rawGA != nullptr);
    if(rawGA == nullptr)
    {
        return AEResult::NullParameter;
    }

    /////////////////////////////////////////////////////////////
    //Import Model from Raw File
    AEResult ret = AEResult::Ok;
    ModelContent* modelContent = nullptr;

    ImporterAssimp importAssimp;

    ret = importAssimp.ImportModel(rawGA->GetFilePath(), &modelContent);
    if(ret != AEResult::Ok)
    {
        AETODO("Add Log");

        return AEResult::ImportFail;
    }
    
    /////////////////////////////////////////////////////////////
    //Write Imported Model to a File the assets can read
    WriterAE3D writerAE3D;
    ret = writerAE3D.WriteToFile(modelContent, rawGA->GetOutputFileName(), m_OutputDirAssets);

    DeleteMem(modelContent);

    if(ret != AEResult::Ok)
    {
        AETODO("Add Log");

        return AEResult::WriteToFileFailed;
    }
    
    /////////////////////////////////////////////////////////////
    //Create Asset if needed and load new file
    bool isNew = false;
    ModelAsset* modelAsset = nullptr;
    
    if(GameAssetExists(rawGA->GetUniqueAssociatedAssetID()))
    {
        modelAsset = reinterpret_cast<ModelAsset*>(m_GameAssetMap[rawGA->GetUniqueAssociatedAssetID()]);
    }
    else
    {
        isNew = true;

        modelAsset = new ModelAsset(writerAE3D.GetOutputFilePath(), m_GameResourceManager, m_GraphicDevice);

        modelAsset->SetCustomName(modelAsset->GetCustomName());
        modelAsset->SetUniqueAssetID(this->GetNextUniqueAssetID());
    }

    ret = modelAsset->LoadAsset();

    if(ret != AEResult::Ok)
    {
        AETODO("add log");

        if(isNew)
        {
            DeleteMem(modelAsset);
        }

        return AEResult::LoadFileFailed;
    }

    if(isNew)
    {
        modelAsset->m_OnGameAssetDeletionNotifyManagerEvent = std::bind(&GameAssetManager::GameAssetDeleted, this, std::placeholders::_1);

        m_GameAssetMap[modelAsset->GetUniqueAssetID()] = modelAsset;

        rawGA->SetUniqueAssociatedAssetID(modelAsset->GetUniqueAssetID());
    }

    /////////////////////////////////////////////////////////////
    //Set IDs to Assets that have been recently imported
    //

    MeshAssetMap& meshAssetMap = modelAsset->GetMeshAssetMap();
    for (auto gaIt : meshAssetMap)
    {
        MeshAsset* meshAsset = gaIt.second.m_Asset;
        if (meshAsset->GetUniqueAssetID() == 0)
        {
            meshAsset->SetUniqueAssetID(this->GetNextUniqueAssetID());

            meshAsset->m_OnGameAssetDeletionNotifyManagerEvent = std::bind(&GameAssetManager::GameAssetDeleted, this, std::placeholders::_1);

            m_GameAssetMap[meshAsset->GetUniqueAssetID()] = meshAsset;
        }
    }

    AnimationAssetMap& animAssetMap = modelAsset->GetAnimationAssetMap();
    for (auto gaIt : animAssetMap)
    {
        AnimationAsset* animAsset = gaIt.second.m_Asset;
        if (animAsset->GetUniqueAssetID() == 0)
        {
            animAsset->SetUniqueAssetID(this->GetNextUniqueAssetID());

            animAsset->m_OnGameAssetDeletionNotifyManagerEvent = std::bind(&GameAssetManager::GameAssetDeleted, this, std::placeholders::_1);

            m_GameAssetMap[animAsset->GetUniqueAssetID()] = animAsset;
        }
    }

    SkeletonAsset* skeletonAsset = modelAsset->GetSkeletonAsset();
    if(skeletonAsset != nullptr && skeletonAsset->GetUniqueAssetID() == 0)
    {
        skeletonAsset->SetUniqueAssetID(this->GetNextUniqueAssetID());

        skeletonAsset->m_OnGameAssetDeletionNotifyManagerEvent = std::bind(&GameAssetManager::GameAssetDeleted, this, std::placeholders::_1);

        m_GameAssetMap[skeletonAsset->GetUniqueAssetID()] = skeletonAsset;
    }

    return AEResult::Ok;
}

AEResult GameAssetManager::ImportShader(RawGameAsset* rawGA)
{
    if(!m_IsReady)
    {
        return AEResult::NotReady;
    }

    AEAssert(rawGA != nullptr);
    if(rawGA == nullptr)
    {
        return AEResult::NullParameter;
    }

    /////////////////////////////////////////////////////////////
    //Import Shader from Raw File
    ImporterHLSL importerHLSL;

    ShaderType shaderType = ShaderType::VertexShader;

    switch (rawGA->GetContentSubtype())
    {
        case GameContentSubtype::VertexShaderHLSL:
            shaderType = ShaderType::VertexShader;
            break;

        case GameContentSubtype::PixelShaderHLSL:
            shaderType = ShaderType::PixelShader;
            break;

        case GameContentSubtype::GeometryShaderHLSL:
            shaderType = ShaderType::GeometryShader;
            break;

        case GameContentSubtype::ComputeShaderHLSL:
            shaderType = ShaderType::ComputeShader;
            break;

        case GameContentSubtype::HullShaderHLSL:
            shaderType = ShaderType::HullShader;
            break;

        case GameContentSubtype::DomainShaderHLSL:
            shaderType = ShaderType::DomainShader;
            break;

        default:
            return AEResult::InvalidShaderType;
            break;
    }

    ShaderContent* shaderContent = nullptr;

    AETODO("make this an option");
    ShaderModel shaderModel = ShaderModel::SM_5_0;

    AEResult ret = importerHLSL.ImportShader(rawGA->GetFilePath(), shaderType, shaderModel, &shaderContent);
    if(ret != AEResult::Ok)
    {
        AETODO("Add Log");

        return AEResult::ImportFail;
    }
    
    /////////////////////////////////////////////////////////////
    //Write Imported Shader to a File the assets can read
    WriterHLSL writerHLSL;
    ret = writerHLSL.WriteToFile(shaderContent, rawGA->GetOutputFileName(), m_OutputDirAssets);

    DeleteMem(shaderContent);

    if(ret != AEResult::Ok)
    {
        AETODO("Add Log");

        return AEResult::WriteToFileFailed;
    }
    
    /////////////////////////////////////////////////////////////
    //Create Asset if needed and load new file
    bool isNew = false;
    ShaderAsset* shaderAsset = nullptr;
    
    if(GameAssetExists(rawGA->GetUniqueAssociatedAssetID()))
    {
        shaderAsset = reinterpret_cast<ShaderAsset*>(m_GameAssetMap[rawGA->GetUniqueAssociatedAssetID()]);
    }
    else
    {
        isNew = true;

        shaderAsset = new ShaderAsset(writerHLSL.GetOutputFilePath(), m_GameResourceManager, shaderType, m_GraphicDevice);

        shaderAsset->SetCustomName(shaderAsset->GetCustomName());
        shaderAsset->SetUniqueAssetID(this->GetNextUniqueAssetID());
    }

    ret = shaderAsset->LoadAsset();

    if(ret != AEResult::Ok)
    {
        AETODO("add log");

        if(isNew)
        {
            DeleteMem(shaderAsset);
        }

        return AEResult::LoadFileFailed;
    }

    if(isNew)
    {
        shaderAsset->m_OnGameAssetDeletionNotifyManagerEvent = std::bind(&GameAssetManager::GameAssetDeleted, this, std::placeholders::_1);

        m_GameAssetMap[shaderAsset->GetUniqueAssetID()] = shaderAsset;

        rawGA->SetUniqueAssociatedAssetID(shaderAsset->GetUniqueAssetID());
    }

    return AEResult::Ok;
}

AEResult GameAssetManager::ImportTexture(RawGameAsset* rawGA)
{
    if(!m_IsReady)
    {
        return AEResult::NotReady;
    }

    AEAssert(rawGA != nullptr);
    if(rawGA == nullptr)
    {
        return AEResult::NullParameter;
    }

    AEResult ret = AEResult::Ok;

    /////////////////////////////////////////////////////////////
    //Import Texture from Raw File (Import is done at runtime)
    TextureType textureType = TextureType::Texture2D;

    switch (rawGA->GetContentSubtype())
    {
        case GameContentSubtype::Texture2D:
            textureType = TextureType::Texture2D;
            break;

        case GameContentSubtype::TextureCube:
            textureType = TextureType::TextureCube;
            break;

        default:
            return AEResult::InvalidTextureType;
            break;
    }

    /////////////////////////////////////////////////////////////
    //Create Asset if needed and load new file
    bool isNew = false;
    TextureAsset* textureAsset = nullptr;

    if(GameAssetExists(rawGA->GetUniqueAssociatedAssetID()))
    {
        textureAsset = reinterpret_cast<TextureAsset*>(m_GameAssetMap[rawGA->GetUniqueAssociatedAssetID()]);
    }
    else
    {
        isNew = true;

        AETODO("When creating own texture reader make sure to put it in the asset paths the output");
        std::string textureName = AE_Base::GetFilenameOnly(rawGA->GetFilePath());
        textureAsset = new TextureAsset(rawGA->GetFilePath(), m_GameResourceManager, textureType, m_GraphicDevice);
        textureAsset->SetName(textureName);
        textureAsset->SetCustomName(textureName);

        textureAsset->SetUniqueAssetID(this->GetNextUniqueAssetID());
    }

    ret = textureAsset->LoadAsset();
    if(ret != AEResult::Ok)
    {
        AETODO("add log");

        if(isNew)
        {
            DeleteMem(textureAsset);
        }

        return AEResult::LoadFileFailed;
    }

    if(isNew)
    {
        textureAsset->m_OnGameAssetDeletionNotifyManagerEvent = std::bind(&GameAssetManager::GameAssetDeleted, this, std::placeholders::_1);

        m_GameAssetMap[textureAsset->GetUniqueAssetID()] = textureAsset;

        rawGA->SetUniqueAssociatedAssetID(textureAsset->GetUniqueAssetID());
    }

    return AEResult::Ok;
}

AEResult GameAssetManager::ImportGameObjectScript(RawGameAsset* rawGA)
{
    if (!m_IsReady)
    {
        return AEResult::NotReady;
    }

    AEAssert(rawGA != nullptr);
    if (rawGA == nullptr)
    {
        return AEResult::NullParameter;
    }

    AEResult ret = AEResult::Ok;

    /////////////////////////////////////////////////////////////
    //Create Asset if needed and load new file
    bool isNew = false;
    GameObjectScriptAsset* gosAsset = nullptr;

    if (GameAssetExists(rawGA->GetUniqueAssociatedAssetID()))
    {
        gosAsset = reinterpret_cast<GameObjectScriptAsset*>(m_GameAssetMap[rawGA->GetUniqueAssociatedAssetID()]);
    }
    else
    {
        isNew = true;

        AETODO("When creating own texture reader make sure to put it in the asset paths the output");
        std::string gameScriptObjectName = AE_Base::GetFilenameOnly(rawGA->GetFilePath());
        gosAsset = new GameObjectScriptAsset(rawGA->GetFilePath(), m_GameResourceManager, m_AngelScriptManager);
        gosAsset->SetName(gameScriptObjectName);
        gosAsset->SetCustomName(gameScriptObjectName);

        gosAsset->SetUniqueAssetID(this->GetNextUniqueAssetID());
    }

    ret = gosAsset->LoadAsset();
    if (ret != AEResult::Ok)
    {
        AETODO("add log");

        if (isNew)
        {
            DeleteMem(gosAsset);
        }

        return AEResult::LoadFileFailed;
    }

    if (isNew)
    {
        gosAsset->m_OnGameAssetDeletionNotifyManagerEvent = std::bind(&GameAssetManager::GameAssetDeleted, this, std::placeholders::_1);

        m_GameAssetMap[gosAsset->GetUniqueAssetID()] = gosAsset;

        rawGA->SetUniqueAssociatedAssetID(gosAsset->GetUniqueAssetID());
    }

    return AEResult::Ok;
}

AEResult GameAssetManager::ImportAudio(RawGameAsset* rawGA)
{
    if (!m_IsReady)
    {
        return AEResult::NotReady;
    }

    AEAssert(rawGA != nullptr);
    if (rawGA == nullptr)
    {
        return AEResult::NullParameter;
    }

    AEResult ret = AEResult::Ok;

    /////////////////////////////////////////////////////////////
    //Import from Raw File (Import is done at runtime)

    /////////////////////////////////////////////////////////////
    //Create Asset if needed and load new file
    bool isNew = false;
    AudioAsset* audioAsset = nullptr;

    if (GameAssetExists(rawGA->GetUniqueAssociatedAssetID()))
    {
        audioAsset = reinterpret_cast<AudioAsset*>(m_GameAssetMap[rawGA->GetUniqueAssociatedAssetID()]);
    }
    else
    {
        isNew = true;

        AETODO("When creating own audio reader make sure to put it in the asset paths the output");
        std::string audioName = AE_Base::GetFilenameOnly(rawGA->GetFilePath());
        audioAsset = new AudioAsset(rawGA->GetFilePath(), m_GameResourceManager, m_AudioManager);
        audioAsset->SetName(audioName);
        audioAsset->SetCustomName(audioName);

        audioAsset->SetUniqueAssetID(this->GetNextUniqueAssetID());
    }

    ret = audioAsset->LoadAsset();
    if (ret != AEResult::Ok)
    {
        AETODO("add log");

        if (isNew)
        {
            DeleteMem(audioAsset);
        }

        return AEResult::LoadFileFailed;
    }

    if (isNew)
    {
        audioAsset->m_OnGameAssetDeletionNotifyManagerEvent = std::bind(&GameAssetManager::GameAssetDeleted, this, std::placeholders::_1);

        m_GameAssetMap[audioAsset->GetUniqueAssetID()] = audioAsset;

        rawGA->SetUniqueAssociatedAssetID(audioAsset->GetUniqueAssetID());
    }

    return AEResult::Ok;
}

void GameAssetManager::SetAssetDetails(const GameAssetLoadingDetails& details, GameAsset* asset)
{
    if (asset == nullptr)
    {
        return;
    }

    asset->SetName(details.m_Name);
    asset->SetCustomName(details.m_CustomName);
    asset->SetUniqueAssetID(details.m_AssetID);
    asset->SetParentAssetID(details.m_ParentAssetID);
    asset->m_IsLoaded = details.m_IsLoaded;
}

AEResult GameAssetManager::LoadModelAsset(const GameAssetLoadingDetails& details)
{
    if (!m_IsReady)
    {
        return AEResult::NotReady;
    }

    if (GameAssetExists(details.m_AssetID))
    {
        return AEResult::GameAssetIDInUse;
    }

    AEResult ret = AEResult::Ok;

    ModelAsset* asset = new ModelAsset(details.m_Filepath, m_GameResourceManager, m_GraphicDevice);

    SetAssetDetails(details, asset);

    asset->m_OnGameAssetDeletionNotifyManagerEvent = std::bind(&GameAssetManager::GameAssetDeleted, this, std::placeholders::_1);

    m_GameAssetMap[asset->GetUniqueAssetID()] = asset;

    return AEResult::Ok;
}

AEResult GameAssetManager::LoadMeshAsset(const GameAssetLoadingDetails& details)
{
    if (!m_IsReady)
    {
        return AEResult::NotReady;
    }

    if (GameAssetExists(details.m_AssetID))
    {
        return AEResult::GameAssetIDInUse;
    }

    GameAsset* gameAsset = GetGameAsset(details.m_ParentAssetID);
    if (gameAsset == nullptr || gameAsset->GetGameContentType() != GameContentType::Model)
    {
        return AEResult::GameAssetInvalidID;
    }
    ModelAsset* modelAsset = reinterpret_cast<ModelAsset*>(gameAsset);

    AEResult ret = AEResult::Ok;

    MeshAsset* asset = new MeshAsset(details.m_Filepath, m_GameResourceManager, m_GraphicDevice);

    SetAssetDetails(details, asset);

    if (asset->IsLoaded())
    {
        ret = asset->LoadAsset();
        if (ret != AEResult::Ok)
        {
            AETODO("add log");

            DeleteMem(asset);

            return AEResult::LoadFileFailed;
        }
    }

    MeshAssetMap& meshMap = modelAsset->GetMeshAssetMap();
    meshMap[asset->GetName()] = GameAssetLoadStatus<MeshAsset>(asset, true);

    asset->m_OnGameAssetDeletionNotifyManagerEvent = std::bind(&GameAssetManager::GameAssetDeleted, this, std::placeholders::_1);

    m_GameAssetMap[asset->GetUniqueAssetID()] = asset;

    return AEResult::Ok;
}

AEResult GameAssetManager::LoadAnimationAsset(const GameAssetLoadingDetails& details)
{
    if (!m_IsReady)
    {
        return AEResult::NotReady;
    }

    if (GameAssetExists(details.m_AssetID))
    {
        return AEResult::GameAssetIDInUse;
    }

    GameAsset* gameAsset = GetGameAsset(details.m_ParentAssetID);
    if (gameAsset == nullptr || gameAsset->GetGameContentType() != GameContentType::Model)
    {
        return AEResult::GameAssetInvalidID;
    }
    ModelAsset* modelAsset = reinterpret_cast<ModelAsset*>(gameAsset);

    AEResult ret = AEResult::Ok;

    AnimationAsset* asset = new AnimationAsset(details.m_Filepath, m_GameResourceManager);

    SetAssetDetails(details, asset);

    if (asset->IsLoaded())
    {
        ret = asset->LoadAsset();
        if (ret != AEResult::Ok)
        {
            AETODO("add log");

            DeleteMem(asset);

            return AEResult::LoadFileFailed;
        }
    }

    AnimationAssetMap& animMap = modelAsset->GetAnimationAssetMap();
    animMap[asset->GetName()] = GameAssetLoadStatus<AnimationAsset>(asset, true);

    asset->m_OnGameAssetDeletionNotifyManagerEvent = std::bind(&GameAssetManager::GameAssetDeleted, this, std::placeholders::_1);

    m_GameAssetMap[asset->GetUniqueAssetID()] = asset;

    return AEResult::Ok;
}

AEResult GameAssetManager::LoadSkeletonAsset(const GameAssetLoadingDetails& details)
{
    if (!m_IsReady)
    {
        return AEResult::NotReady;
    }

    if (GameAssetExists(details.m_AssetID))
    {
        return AEResult::GameAssetIDInUse;
    }

    GameAsset* gameAsset = GetGameAsset(details.m_ParentAssetID);
    if (gameAsset == nullptr || gameAsset->GetGameContentType() != GameContentType::Model)
    {
        return AEResult::GameAssetInvalidID;
    }
    ModelAsset* modelAsset = reinterpret_cast<ModelAsset*>(gameAsset);

    AEResult ret = AEResult::Ok;

    SkeletonAsset* asset = new SkeletonAsset(details.m_Filepath, m_GameResourceManager);

    SetAssetDetails(details, asset);

    if (asset->IsLoaded())
    {
        ret = asset->LoadAsset();
        if (ret != AEResult::Ok)
        {
            AETODO("add log");

            DeleteMem(asset);

            return AEResult::LoadFileFailed;
        }
    }

    modelAsset->SetSkeletonAsset(asset);

    asset->m_OnGameAssetDeletionNotifyManagerEvent = std::bind(&GameAssetManager::GameAssetDeleted, this, std::placeholders::_1);

    m_GameAssetMap[asset->GetUniqueAssetID()] = asset;

    return AEResult::Ok;
}

AEResult GameAssetManager::LoadShaderAsset(const GameAssetLoadingDetails& details)
{
    if (!m_IsReady)
    {
        return AEResult::NotReady;
    }

    if (GameAssetExists(details.m_AssetID))
    {
        return AEResult::GameAssetIDInUse;
    }

    AEResult ret = AEResult::Ok;

    RawGameAsset* rawGameAsset = GetRawGameAssetByGameAssetID(details.m_AssetID);
    if (rawGameAsset == nullptr)
    {
        return AEResult::RawGameAssetNotFound;
    }

    ShaderType shaderType = ShaderType::VertexShader;
    switch (rawGameAsset->GetContentSubtype())
    {
        case GameContentSubtype::VertexShaderHLSL:
            shaderType = ShaderType::VertexShader;
            break;

        case GameContentSubtype::PixelShaderHLSL:
            shaderType = ShaderType::PixelShader;
            break;

        case GameContentSubtype::GeometryShaderHLSL:
            shaderType = ShaderType::GeometryShader;
            break;

        case GameContentSubtype::ComputeShaderHLSL:
            shaderType = ShaderType::ComputeShader;
            break;

        case GameContentSubtype::HullShaderHLSL:
            shaderType = ShaderType::HullShader;
            break;

        case GameContentSubtype::DomainShaderHLSL:
            shaderType = ShaderType::DomainShader;
            break;

        default:
            return AEResult::InvalidShaderType;
            break;
    }

    ShaderAsset* asset = new ShaderAsset(details.m_Filepath, m_GameResourceManager, shaderType, m_GraphicDevice);

    SetAssetDetails(details, asset);

    if (asset->IsLoaded())
    {
        ret = asset->LoadAsset();
        if (ret != AEResult::Ok)
        {
            AETODO("add log");

            DeleteMem(asset);

            return AEResult::LoadFileFailed;
        }
    }

    asset->m_OnGameAssetDeletionNotifyManagerEvent = std::bind(&GameAssetManager::GameAssetDeleted, this, std::placeholders::_1);

    m_GameAssetMap[asset->GetUniqueAssetID()] = asset;

    return AEResult::Ok;
}

AEResult GameAssetManager::LoadTextureAsset(const GameAssetLoadingDetails& details)
{
    if (!m_IsReady)
    {
        return AEResult::NotReady;
    }

    if (GameAssetExists(details.m_AssetID))
    {
        return AEResult::GameAssetIDInUse;
    }

    AEResult ret = AEResult::Ok;

    RawGameAsset* rawGameAsset = GetRawGameAssetByGameAssetID(details.m_AssetID);
    if (rawGameAsset == nullptr)
    {
        return AEResult::RawGameAssetNotFound;
    }

    TextureType textureType = TextureType::Texture2D;
    switch (rawGameAsset->GetContentSubtype())
    {
        case GameContentSubtype::Texture2D:
            textureType = TextureType::Texture2D;
            break;

        case GameContentSubtype::TextureCube:
            textureType = TextureType::TextureCube;
            break;

        default:
            return AEResult::InvalidTextureType;
            break;
    }

    TextureAsset* asset = new TextureAsset(details.m_Filepath, m_GameResourceManager, textureType, m_GraphicDevice);

    SetAssetDetails(details, asset);

    if (asset->IsLoaded())
    {
        ret = asset->LoadAsset();
        if (ret != AEResult::Ok)
        {
            AETODO("add log");

            DeleteMem(asset);

            return AEResult::LoadFileFailed;
        }
    }

    asset->m_OnGameAssetDeletionNotifyManagerEvent = std::bind(&GameAssetManager::GameAssetDeleted, this, std::placeholders::_1);

    m_GameAssetMap[asset->GetUniqueAssetID()] = asset;

    return AEResult::Ok;
}

AEResult GameAssetManager::LoadGameObjectScriptAsset(const GameAssetLoadingDetails& details)
{
    if (!m_IsReady)
    {
        return AEResult::NotReady;
    }

    if (GameAssetExists(details.m_AssetID))
    {
        return AEResult::GameAssetIDInUse;
    }

    AEResult ret = AEResult::Ok;

    GameObjectScriptAsset* asset = new GameObjectScriptAsset(details.m_Filepath, m_GameResourceManager, m_AngelScriptManager);

    SetAssetDetails(details, asset);

    if (asset->IsLoaded())
    {
        ret = asset->LoadAsset();
        if (ret != AEResult::Ok)
        {
            AETODO("add log");

            DeleteMem(asset);

            return AEResult::LoadFileFailed;
        }
    }

    asset->m_OnGameAssetDeletionNotifyManagerEvent = std::bind(&GameAssetManager::GameAssetDeleted, this, std::placeholders::_1);

    m_GameAssetMap[asset->GetUniqueAssetID()] = asset;

    return AEResult::Ok;
}

AEResult GameAssetManager::LoadAudioAsset(const GameAssetLoadingDetails& details)
{
    if (!m_IsReady)
    {
        return AEResult::NotReady;
    }

    if (GameAssetExists(details.m_AssetID))
    {
        return AEResult::GameAssetIDInUse;
    }

    AEResult ret = AEResult::Ok;

    AudioAsset* asset = new AudioAsset(details.m_Filepath, m_GameResourceManager, m_AudioManager);

    SetAssetDetails(details, asset);

    if (asset->IsLoaded())
    {
        ret = asset->LoadAsset();
        if (ret != AEResult::Ok)
        {
            AETODO("add log");

            DeleteMem(asset);

            return AEResult::LoadFileFailed;
        }
    }

    asset->m_OnGameAssetDeletionNotifyManagerEvent = std::bind(&GameAssetManager::GameAssetDeleted, this, std::placeholders::_1);

    m_GameAssetMap[asset->GetUniqueAssetID()] = asset;

    return AEResult::Ok;
}

AEResult GameAssetManager::SaveToXML(const std::string& gameAssetsFilename) const
{
    if (!m_IsReady)
    {
        return AEResult::NotReady;
    }

    if (gameAssetsFilename.empty())
    {
        return AEResult::EmptyFilename;
    }

    AEXMLWriter xmlWriter;
    AEResult ret = AEResult::Ok;

    ret = xmlWriter.CreateXML(gameAssetsFilename, true);
    if (ret != AEResult::Ok)
    {
        AETODO("Better return code");
        return AEResult::Fail;
    }

    ret = xmlWriter.StartNode(AE_ASSET_MANAGER_NODE_NAME);
    if (ret != AEResult::Ok)
    {
        AETODO("Better return code");
        return AEResult::Fail;
    }

    ret = xmlWriter.StartNode(AE_ASSET_CONFIG_NODE_NAME);
    if (ret != AEResult::Ok)
    {
        AETODO("Better return code");
        return AEResult::Fail;
    }

    xmlWriter.WriteUInt64(AE_ASSET_PERSISTANTUNIQUEID_PROP_NAME, this->m_UniqueAssetIDGen);

    ret = xmlWriter.EndNode();
    if (ret != AEResult::Ok)
    {
        AETODO("Better return code");
        return AEResult::Fail;
    }

    ret = SaveToXMLRawGameAssets(xmlWriter);
    if (ret != AEResult::Ok)
    {
        AETODO("Better return code");
        return AEResult::Fail;
    }

    ret = SaveToXMLGameAssets(xmlWriter);
    if (ret != AEResult::Ok)
    {
        AETODO("Better return code");
        return AEResult::Fail;
    }

    ret = xmlWriter.EndNode();
    if (ret != AEResult::Ok)
    {
        AETODO("Better return code");
        return AEResult::Fail;
    }

    ret = xmlWriter.FinalizeXML();
    if (ret != AEResult::Ok)
    {
        AETODO("Better return code");
        return AEResult::Fail;
    }

    return AEResult::Ok;
}

AEResult GameAssetManager::SaveToXMLGameAssets(AEXMLWriter& xmlWriter) const
{
    AEResult ret = AEResult::Ok;

    ret = xmlWriter.StartNode(AE_ASSETS_NODE_NAME);
    if (ret != AEResult::Ok)
    {
        AETODO("Better return code");
        return AEResult::Fail;
    }

    for (auto gameAssetPair : m_GameAssetMap)
    {
        GameAsset* gameAsset = gameAssetPair.second;

        if (gameAsset->IsBuiltInAsset())
        {
            continue;
        }

        ret = xmlWriter.StartNode(AE_ASSET_NODE_NAME);
        if (ret != AEResult::Ok)
        {
            AETODO("Better return code");
            return AEResult::Fail;
        }

        xmlWriter.WriteString(AE_ASSET_NAME_PROP, gameAsset->GetName());
        xmlWriter.WriteString(AE_ASSET_CUSTOM_NAME_PROP, gameAsset->GetCustomName());
        xmlWriter.WriteString(AE_ASSET_FILEPATH_PROP, gameAsset->GetFilePath());
        xmlWriter.WriteUInt64(AE_ASSET_UNIQUEASSETID_PROP, gameAsset->GetUniqueAssetID());
        xmlWriter.WriteUInt64(AE_ASSET_PARENTASSETID_PROP, gameAsset->GetParentAssetID());
        xmlWriter.WriteUInt(AE_ASSET_GAMECONTENTTYPE_PROP, (uint32_t)gameAsset->GetGameContentType());
        xmlWriter.WriteBool(AE_ASSET_ISLOADED_PROP, gameAsset->IsLoaded());

        ret = xmlWriter.EndNode();
        if (ret != AEResult::Ok)
        {
            AETODO("Better return code");
            return AEResult::Fail;
        }
    }

    ret = xmlWriter.EndNode();
    if (ret != AEResult::Ok)
    {
        AETODO("Better return code");
        return AEResult::Fail;
    }

    return AEResult::Ok;
}

AEResult GameAssetManager::SaveToXMLRawGameAssets(AEXMLWriter& xmlWriter) const
{
    AEResult ret = AEResult::Ok;

    ret = xmlWriter.StartNode(AE_RAW_FILES_NODE_NAME);
    if (ret != AEResult::Ok)
    {
        AETODO("Better return code");
        return AEResult::Fail;
    }

    for (auto rawAssetPair : m_RawGameAssetMap)
    {
        RawGameAsset* rawGameAsset = rawAssetPair.second;

        ret = xmlWriter.StartNode(AE_RAW_FILE_NODE_NAME);
        if (ret != AEResult::Ok)
        {
            AETODO("Better return code");
            return AEResult::Fail;
        }

        xmlWriter.WriteString(AE_RAW_FILE_NAME_PROP, rawGameAsset->GetName());
        xmlWriter.WriteString(AE_RAW_FILE_CUSTOM_NAME_PROP, rawGameAsset->GetCustomName());
        xmlWriter.WriteUInt(AE_RAW_FILE_CONTENTTYPE_PROP, (uint32_t)rawGameAsset->GetContentType());
        xmlWriter.WriteUInt(AE_RAW_FILE_CONTENTSUBTYPE_PROP, (uint32_t)rawGameAsset->GetContentSubtype());
        xmlWriter.WriteUInt(AE_RAW_FILE_GAMECONTEXTFILEEXT_PROP, (uint32_t)rawGameAsset->GetGameContextFileExt());
        xmlWriter.WriteString(AE_RAW_FILE_LASTMODIFIEDTIMESTAMP_PROP, rawGameAsset->GetLastModifiedTimeStamp().ToString());
        xmlWriter.WriteString(AE_RAW_FILE_FILEPATH_PROP, rawGameAsset->GetFilePath());
        xmlWriter.WriteString(AE_RAW_FILE_OUTPUTFILENAME_PROP, rawGameAsset->GetOutputFileName());
        xmlWriter.WriteBool(AE_RAW_FILE_RELOADNEEDED_PROP, rawGameAsset->IsReloadNeeded());
        xmlWriter.WriteBool(AE_RAW_FILE_CONTENTSUBTYPECHANGED_PROP, rawGameAsset->GetContentSubtypeChanged());
        xmlWriter.WriteUInt64(AE_RAW_FILE_ASSOCIATED_ASSET_ID_PROP, rawGameAsset->GetUniqueAssociatedAssetID());

        ret = xmlWriter.EndNode();
        if (ret != AEResult::Ok)
        {
            AETODO("Better return code");
            return AEResult::Fail;
        }
    }

    ret = xmlWriter.EndNode();
    if (ret != AEResult::Ok)
    {
        AETODO("Better return code");
        return AEResult::Fail;
    }

    return AEResult::Ok;
}

AEResult GameAssetManager::LoadAssetManagerFile(const std::string& gameAssetsFilename)
{
    if (!m_IsReady)
    {
        return AEResult::NotReady;
    }

    if (gameAssetsFilename.empty())
    {
        return AEResult::EmptyFilename;
    }

    AEXMLParser newFile;
    if (newFile.LoadFile(gameAssetsFilename) != AEResult::Ok)
    {
        std::string msg_error = fmt::format(AELOCMAN->GetLiteral("INIT_COULDNT_READ_FILE_MSG"), __FUNCTION__, gameAssetsFilename);

        AELOGGER->AddNewLog(LogLevel::Error, msg_error);
        return AEResult::OpenFileFail;
    }

    AEXMLParser configXML = newFile[AE_ASSET_MANAGER_NODE_NAME];
    if (!configXML.IsReady())
    {
        return AEResult::XMLReadError;
    }

    uint32_t l_Count = configXML.GetNumChildren();
    for (uint32_t i = 0; i < l_Count; ++i)
    {
        AEXMLParser child = configXML(i);

        std::string l_Type = child.GetName();

        if (l_Type.compare(AE_ASSET_CONFIG_NODE_NAME) == 0)
        {
            m_UniqueAssetIDGen = child.GetUInt64(AE_ASSET_PERSISTANTUNIQUEID_PROP_NAME);
        }
        else if (l_Type.compare(AE_RAW_FILES_NODE_NAME) == 0)
        {
            if (LoadRawAssets(child) != AEResult::Ok)
            {
                return AEResult::Fail;
            }
        }
        else if (l_Type.compare(AE_ASSETS_NODE_NAME) == 0)
        {
            if (LoadGameAssets(child) != AEResult::Ok)
            {
                return AEResult::Fail;
            }
        }
    }

    return AEResult::Ok;
}

AEResult GameAssetManager::LoadRawAssets(AEXMLParser& rawAssetXML)
{
    uint32_t l_Count = rawAssetXML.GetNumChildren();
    for (uint32_t i = 0; i < l_Count; ++i)
    {
        AEXMLParser child = rawAssetXML(i);

        std::string l_Type = child.GetName();

        if (l_Type.compare("RawFile") == 0)
        {
            TimeStamp modTimeStamp(child.GetString(AE_RAW_FILE_LASTMODIFIEDTIMESTAMP_PROP));
            std::string name                = child.GetString(AE_RAW_FILE_NAME_PROP);
            std::string customName          = child.GetString(AE_RAW_FILE_CUSTOM_NAME_PROP);
            std::string filepath            = child.GetString(AE_RAW_FILE_FILEPATH_PROP);
            GameContentSubtype subType      = (GameContentSubtype)child.GetUInt(AE_RAW_FILE_CONTENTSUBTYPE_PROP);
            uint64_t uniqueAssociatedID     = child.GetUInt64(AE_RAW_FILE_ASSOCIATED_ASSET_ID_PROP);
            GameContentType contentType     = (GameContentType)child.GetUInt(AE_RAW_FILE_CONTENTTYPE_PROP);
            GameContextFileExt fileExt      = (GameContextFileExt)child.GetUInt(AE_RAW_FILE_GAMECONTEXTFILEEXT_PROP);
            std::string outputFilename      = child.GetString(AE_RAW_FILE_OUTPUTFILENAME_PROP);
            bool reloadNeeded               = child.GetBool(AE_RAW_FILE_RELOADNEEDED_PROP);
            bool contentSubtypeChanged      = child.GetBool(AE_RAW_FILE_CONTENTSUBTYPECHANGED_PROP);

            RawGameAsset* rawGameAsset = new RawGameAsset(filepath, m_ProjectDirectory, name);

            rawGameAsset->SetContentSubtype(subType);
            rawGameAsset->SetUniqueAssociatedAssetID(uniqueAssociatedID);
            rawGameAsset->SetContentType(contentType);
            rawGameAsset->SetGameContextFileExt(fileExt);
            rawGameAsset->SetLastModifiedTimeStamp(modTimeStamp);
            rawGameAsset->SetOutputFileName(outputFilename);
            rawGameAsset->SetReloadNeeded(reloadNeeded);
            rawGameAsset->SetContentSubtypeChanged(contentSubtypeChanged);

            m_RawGameAssetMap[rawGameAsset->GetUniqueID()] = rawGameAsset;
        }
    }

    return AEResult::Ok;
}

AEResult GameAssetManager::LoadGameAssets(AEXMLParser& gameAssetXML)
{
    uint32_t l_Count = gameAssetXML.GetNumChildren();
    for (uint32_t i = 0; i < l_Count; ++i)
    {
        AEXMLParser child = gameAssetXML(i);

        std::string l_Type = child.GetName();

        if (l_Type.compare("Asset") == 0)
        {
            GameContentType type = (GameContentType)child.GetUInt(AE_ASSET_GAMECONTENTTYPE_PROP);

            GameAssetLoadingDetails details;

            details.m_Name              = child.GetString(AE_ASSET_NAME_PROP);
            details.m_CustomName        = child.GetString(AE_ASSET_CUSTOM_NAME_PROP);
            details.m_Filepath          = child.GetString(AE_ASSET_FILEPATH_PROP);
            details.m_AssetID           = child.GetUInt64(AE_ASSET_UNIQUEASSETID_PROP);
            details.m_ParentAssetID     = child.GetUInt64(AE_ASSET_PARENTASSETID_PROP);
            details.m_IsLoaded          = child.GetBool(AE_ASSET_ISLOADED_PROP);

            AEResult ret = AEResult::Ok;
            switch (type)
            {
                case GameContentType::Model:
                    ret = LoadModelAsset(details);
                    break;

                case GameContentType::Mesh:
                    ret = LoadMeshAsset(details);
                    break;

                case GameContentType::Animation:
                    ret = LoadAnimationAsset(details);
                    break;

                case GameContentType::Skeleton:
                    ret = LoadSkeletonAsset(details);
                    break;

                case GameContentType::Texture:
                    ret = LoadTextureAsset(details);
                    break;

                case GameContentType::Shader:
                    ret = LoadShaderAsset(details);
                    break;

                case GameContentType::GameObjectScript:
                    ret = LoadGameObjectScriptAsset(details);
                    break;

                case GameContentType::Audio:
                    ret = LoadAudioAsset(details);
                    break;

                default:
                    return AEResult::InvalidObjType;
            }

            if (ret != AEResult::Ok)
            {
                return ret;
            }
        }
    }

    return AEResult::Ok;
}
