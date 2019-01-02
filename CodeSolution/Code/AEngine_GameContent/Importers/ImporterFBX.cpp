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
#include "precomp_gamecontent.h"

/**********************
*   System Includes   *
***********************/

/*************************
*   3rd Party Includes   *
**************************/
#include "fastformat\fastformat.hpp"
#include "fastformat\sinks\ostream.hpp"
#include "glm\gtc\matrix_transform.hpp"
#include "fastformat\shims\conversion\filter_type\bool.hpp"
#include "fastformat\shims\conversion\filter_type\reals.hpp"

/***************************
*   Game Engine Includes   *
****************************/
#include "ImporterFBX.h"
#include "Content\MeshContent.h"
#include "Models\Skinning\Bone.h"
#include "Content\ModelContent.h"
#include "Content\MeshPartContent.h"
#include "Content\SkeletonContent.h"
#include "Content\AnimationContent.h"
#include "Vertex\Types\VertexTypes.h"

//Always include last
#include "Memory\MemLeaks.h"

/********************
*   Function Defs   *
*********************/
ImporterFBX::ImporterFBX()
{
}

ImporterFBX::~ImporterFBX()
{
    CleanUp();
}

void ImporterFBX::CleanUp()
{
    if (m_FBXSdkManager != nullptr)
    {
        m_FBXSdkManager->Destroy();
        m_FBXSdkManager = nullptr;
        m_Scene = nullptr;
    }

    DeleteMem(m_Model);

    //Clear Structures
    m_BoneMap.clear();
    m_MatIdxToMatIDMap.clear();
    m_MaterialHolderMap.clear();

    //Reset Variables
    m_FramesPerSecond = 0;
    m_SecondsInFrame = 0.0f;
    m_StartFrame = 0;
    m_EndFrame = 0;
    m_Clockwise = false;
    m_ImportTangentBinormal = false;
    m_BurnTransformation = false;
    m_GenerateTangenteBinormal = false;
    m_HasSecondUV = false;
    m_HasUV = false;
    m_HasNormal = false;
    m_HasColor = false;
    m_HasAnimation = false;
}

AEResult ImporterFBX::ImportFBXFile(const std::string& filename, ModelContent** model, bool burnTransformation, bool importTangentBinormal, bool clockwise)
{
    //////////////////////////////////////////////
    //Clean up old variables
    CleanUp();

    m_ImportTangentBinormal = importTangentBinormal;
    m_BurnTransformation = burnTransformation;
    m_Clockwise = clockwise;

    //////////////////////////////////////////////
    //Init FBX SDK
    if (InitializeFBXSDK() != AEResult::Ok)
    {
        AETODO("Add FBX error");
        //AEResult::FBXSDKInitFailed
        return AEResult::Fail;
    }

    //////////////////////////////////////////////
    //Load Scene
    if (LoadScene(filename) != AEResult::Ok)
    {
        AETODO("Add FBX error");
        //AEResult::FBXLoadSceneFailed
        return AEResult::Fail;
    }

    //////////////////////////////////////////////
    //Create Model
    m_Model = new ModelContent();

    //////////////////////////////////////////////
    //Set Filename
    m_Model->m_Name = AE_Base::GetFilenameOnly(filename);

    //////////////////////////////////////////////
    //Build Skeleton
    if (BuildSkeletonHierarchy() != AEResult::Ok)
    {
        CleanUp();

        return AEResult::LoadFileFailed;
    }

    //////////////////////////////////////////////
    //Extract Content
    if (ExtractContent() != AEResult::Ok)
    {
        CleanUp();

        return AEResult::LoadFileFailed;
    }

    //////////////////////////////////////////////
    //Extract Animation
    if (m_HasAnimation)
    {
        if (ExtractAnimation() != AEResult::Ok)
        {
            CleanUp();

            return AEResult::LoadFileFailed;
        }
    }

    *model = m_Model;
    m_Model = nullptr;

    return AEResult::Ok;
}

AEResult ImporterFBX::InitializeFBXSDK()
{
    //////////////////////////////////////////////
    //The first thing to do is to create the FBX SDK manager which is the 
    //object allocator for almost all the classes in the SDK.
    m_FBXSdkManager = FbxManager::Create();
    if (m_FBXSdkManager == nullptr)
    {
        return AEResult::Fail;
    }

    //////////////////////////////////////////////
    //Create an IOSettings object
    FbxIOSettings* ios = FbxIOSettings::Create(m_FBXSdkManager, IOSROOT);
    m_FBXSdkManager->SetIOSettings(ios);

    //////////////////////////////////////////////
    //Load plugins from the executable directory
    FbxString lPath = FbxGetApplicationDirectory();
    m_FBXSdkManager->LoadPluginsDirectory(lPath.Buffer());

    //////////////////////////////////////////////
    //Create the entity that will hold the scene.
    m_Scene = FbxScene::Create(m_FBXSdkManager, "");
    if (m_Scene == nullptr)
    {
        return AEResult::Fail;
    }

    return AEResult::Ok;
}

AEResult ImporterFBX::LoadScene(const std::string& filename)
{
    /////////////////////////////////////////////
    //Variables
    int32_t fileMajor = -1;
    int32_t fileMinor = -1;
    int32_t fileRevision = -1;

    int32_t sdkMajor = -1;
    int32_t sdkMinor = -1;
    int32_t sdkRevision = -1;

    bool status = false;

    /////////////////////////////////////////////
    //Get the file version number generate by the FBX SDK.
    FbxManager::GetFileFormatVersion(sdkMajor, sdkMinor, sdkRevision);

    /////////////////////////////////////////////
    //Create an importer.
    FbxImporter* importer = FbxImporter::Create(m_FBXSdkManager, "");

    /////////////////////////////////////////////
    // Initialize the importer by providing a filename.
    std::string sFilename = AE_Base::WideStr2String(filename);
    status = importer->Initialize(sFilename.c_str(), -1, m_FBXSdkManager->GetIOSettings());
    importer->GetFileVersion(fileMajor, fileMinor, fileRevision);

    if (!status)
    {
        if (importer->GetStatus() == FbxStatus::eInvalidFileVersion)
        {
            AETODO("FBX_FILE_VERSION_NOT_SUPPORTED_ERR_MSG check if it is there");
            std::string msgerr = fmt::format(AELOCMAN.GetLiteral("FBX_FILE_VERSION_NOT_SUPPORTED_ERR_MSG"), __FUNCTION__, sdkMajor, sdkMinor, sdkRevision, filename, fileMajor, fileMinor, fileRevision);
            AELOGGER.AddNewLog(LogLevel::Error, msgerr);
        }

        importer->Destroy();
        return AEResult::Fail;
    }

    if (importer->IsFBX())
    {
        /////////////////////////////////////////////
        // Set the import states. By default, the import states are always set to 
        // true. The code below shows how to change these states
        (*(m_FBXSdkManager->GetIOSettings())).SetBoolProp(IMP_FBX_MATERIAL, true);
        (*(m_FBXSdkManager->GetIOSettings())).SetBoolProp(IMP_FBX_TEXTURE, true);
        (*(m_FBXSdkManager->GetIOSettings())).SetBoolProp(IMP_FBX_LINK, true);
        (*(m_FBXSdkManager->GetIOSettings())).SetBoolProp(IMP_FBX_SHAPE, true);
        (*(m_FBXSdkManager->GetIOSettings())).SetBoolProp(IMP_FBX_GOBO, true);
        (*(m_FBXSdkManager->GetIOSettings())).SetBoolProp(IMP_FBX_ANIMATION, true);
        (*(m_FBXSdkManager->GetIOSettings())).SetBoolProp(IMP_FBX_GLOBAL_SETTINGS, true);
    }

    /////////////////////////////////////////////
    // Import the scene.
    status = importer->Import(m_Scene);
    if (status == false && importer->GetStatus() == FbxStatus::ePasswordError)
    {
        AETODO("FBX_SCENE_PASSWORD_PROTECTED_ERR_MSG check if it is there");
        std::string msgerr = fmt::format(AELOCMAN.GetLiteral("FBX_SCENE_PASSWORD_PROTECTED_ERR_MSG"), __FUNCTION__, filename);
        AELOGGER.AddNewLog(LogLevel::Error, msgerr);
    }

    /////////////////////////////////////////////
    // Destroy the importer.
    importer->Destroy();
    if (!status)
    {
        return AEResult::Fail;
    }

    /////////////////////////////////////////////
    //Check Coordinate System

    //AEngine Axis System 
    //FbxAxisSystem xeAxisSystem(FbxAxisSystem::EUpVector::eYAxis, FbxAxisSystem::EFrontVector::eParityEven, FbxAxisSystem::ECoordSystem::eRightHanded);

    //Converts Scene to AEngine Axis
    //xeAxisSystem.ConvertScene(m_Scene);

    return AEResult::Ok;
}

AEResult ImporterFBX::BuildSkeletonHierarchy()
{
    FbxNode* node = m_Scene->GetRootNode();
    if (node != nullptr)
    {
        int32_t size = node->GetChildCount();
        for (int32_t i = 0; i < size; ++i)
        {
            if (BuildSkeletonHierarchy(node->GetChild(i)) != AEResult::Ok)
            {
                return AEResult::Fail;
            }
        }
    }

    //When all the Scene has been search we Flatten the Skeleton from a Tree Structure to a Vertex Structure
    SkeletonContent* skeleton = m_Model->m_SkeletonContent;

    skeleton->FlattenSkeleton();
    if (skeleton->GetFlattenSkeletonSize() > 0)
    {
        m_HasAnimation = true;
    }

    return AEResult::Ok;
}

AEResult ImporterFBX::BuildSkeletonHierarchy(FbxNode* node, const glm::mat4& parentTransform, TreeBone* childBone)
{
    AEAssert(node != nullptr);
    if (node == nullptr)
    {
        return AEResult::NullParameter;
    }

    FbxNodeAttribute::EType attributeType = FbxNodeAttribute::eUnknown;
    if (node->GetNodeAttribute() == nullptr)
    {
        return AEResult::Ok;
    }

    //Get Local and World Transformations
    glm::mat4 localTransform = AEMathHelpers::Mat4Identity;
    GetNodeLocalTransform(node, localTransform);

    glm::mat4 worldTransform = localTransform * parentTransform;

    attributeType = (node->GetNodeAttribute()->GetAttributeType());
    if (attributeType == FbxNodeAttribute::eSkeleton)
    {
        SkeletonContent* skeletonContent = m_Model->m_SkeletonContent;
        Bone* currentBone = nullptr;

        if (childBone == nullptr)
        {
            childBone = new TreeBone();
            currentBone = new Bone();
            childBone->m_Bone = currentBone;

            skeletonContent->m_DummyRootBone->m_ChildBones.push_back(childBone);
        }
        else
        {
            TreeBone* newChild = new TreeBone();
            currentBone = new Bone();
            newChild->m_Bone = currentBone;

            childBone->m_ChildBones.push_back(newChild);

            childBone = newChild;
        }

        //////////////////////////////////
        //Set Data to bone
        currentBone->SetName(AE_Base::String2WideStr(node->GetName()));
        currentBone->SetLocalMatrix(localTransform);
        currentBone->SetWorldMatrix(worldTransform);

        FbxSkeleton* skeletonFBX = (FbxSkeleton*)node->GetNodeAttribute();
        m_BoneMap[skeletonFBX->GetUniqueID()] = currentBone;
    }

    int32_t childSize = node->GetChildCount();
    for (int32_t i = 0; i < childSize; ++i)
    {
        if (BuildSkeletonHierarchy(node->GetChild(i), worldTransform, childBone) != AEResult::Ok)
        {
            return AEResult::Fail;
        }
    }

    return AEResult::Ok;
}

AEResult ImporterFBX::ExtractContent()
{
    FbxNode* node = m_Scene->GetRootNode();

    if (node)
    {
        int32_t size = node->GetChildCount();
        for (int32_t i = 0; i < size; ++i)
        {
            if (ExtractContent(node->GetChild(i)) != AEResult::Ok)
            {
                return AEResult::Fail;
            }
        }
    }

    return AEResult::Ok;
}

AEResult ImporterFBX::ExtractContent(FbxNode* node, const glm::mat4& parentTransform)
{
    FbxNodeAttribute::EType attributeType = FbxNodeAttribute::eUnknown;
    if (node->GetNodeAttribute() == nullptr)
    {
        return AEResult::Ok;
    }

    ///////////////////////////////////////
    //Get Local & World Transforms
    glm::mat4 localTransform = AEMathHelpers::Mat4Identity;
    GetNodeLocalTransform(node, localTransform);

    glm::mat4 worldTransform = localTransform * parentTransform;

    ///////////////////////////////////////

    attributeType = (node->GetNodeAttribute()->GetAttributeType());

    switch (attributeType)
    {
        case FbxNodeAttribute::eMesh:
        {
            MeshHolder meshHolder;
            meshHolder.m_WorldTransform = worldTransform;

            AEResult ret = ExtractMesh(node, meshHolder);

            if (ret != AEResult::Ok)
            {
                AETODO("Add FBX_MESH_LOAD_FAIL_ERR_MSG");
                std::string msgerr = fmt::format(AELOCMAN.GetLiteral("FBX_MESH_LOAD_FAIL_ERR_MSG"), __FUNCTION__, static_cast<int32_t>(ret));
                AELOGGER.AddNewLog(LogLevel::Error, msgerr);

                return AEResult::Fail;
            }
            else
            {
                MeshContent* meshContent = new MeshContent();
                meshContent->m_Name = meshHolder.m_Name;
                
                for (auto meshPartHolder : meshHolder.m_MeshPartHolderVec)
                {
                    MeshPartContent* meshPart = nullptr;

                    if (AEImporterHelpers::ConvertToMeshContent(meshPartHolder, &meshPart) != AEResult::Ok)
                    {
                        DeleteMem(meshContent);

                        return AEResult::Fail;
                    }

                    meshContent->m_MeshParts.push_back(meshPart);
                }

                m_Model->m_Meshes[meshContent->m_Name] = meshContent;
            }
        }
        break;

        case FbxNodeAttribute::eSkeleton:    //Skeleton is Handle elsewhere
        case FbxNodeAttribute::eNull:        //Can Be a Group, do notthing
            break;

        default:
        {
            AETODO("Add FBX_NODE_ATTRIBUTE_NOT_SUPPORTED_ERR_MSG");
            std::string msgerr = fmt::format(AELOCMAN.GetLiteral("FBX_NODE_ATTRIBUTE_NOT_SUPPORTED_ERR_MSG"), __FUNCTION__, static_cast<uint32_t>(attributeType));
            AELOGGER.AddNewLog(LogLevel::Warning, msgerr);
        }
        break;
    }

    int32_t childSize = node->GetChildCount();
    if (childSize != 0)
    {
        for (int32_t i = 0; i < childSize; ++i)
        {
            if (ExtractContent(node->GetChild(i), worldTransform) != AEResult::Ok)
            {
                return AEResult::Fail;
            }
        }
    }

    return AEResult::Ok;
}

AEResult ImporterFBX::ExtractMesh(FbxNode* node, MeshHolder& meshHolder)
{
    AEAssert(node != nullptr);
    if (node == nullptr)
    {
        return AEResult::NullParameter;
    }

    //Set true to has UV Coords, so it only applies per Mesh
    m_HasUV = true;

    //Set true to has Normals, so it only applies per Mesh
    m_HasNormal = true;

    //Set False to has Second UV, so it only applies per Mesh
    m_HasSecondUV = false;

    //Set False to Generate Tangent and Binormal, if we are importing
    //T & B and there are non we need to generate it, do this per Mesh basis
    m_GenerateTangenteBinormal = false;

    ////////////////////////////////////////////
    //Set Name
    std::string wMeshName = AE_Base::String2WideStr(node->GetName());
    meshHolder.m_Name = wMeshName;

    ////////////////////////////////////////////
    //Get FBX Mesh
    FbxMesh* meshFBX = (FbxMesh*)node->GetNodeAttribute();

    if (GetMeshVertices(meshFBX, meshHolder) != AEResult::Ok)
    {
        AETODO("Add FBXReadMeshVerticesFailed");
        return AEResult::Fail;
    }

    if (GetMeshMaterialMapping(meshFBX, meshHolder) != AEResult::Ok)
    {
        AETODO("Add FBXMapMaterialsFailed");
        return AEResult::Fail;
    }

    if (GetMaterialProperties(meshFBX) != AEResult::Ok)
    {
        AETODO("Add FBXGetMaterialsPropFailed");
        return AEResult::Fail;
    }

    if (GetMaterialTextures(meshFBX) != AEResult::Ok)
    {
        AETODO("Add FBXGetMaterialsTexturesFailed");
        return AEResult::Fail;
    }

    if (m_HasAnimation)
    {
        if (GetMeshBlendIndicesWeight(meshFBX, meshHolder) != AEResult::Ok)
        {
            AETODO("Add special return");
            return AEResult::Fail;
        }
    }

    if (CreateMeshParts(meshHolder) != AEResult::Ok)
    {
        AETODO("Add FBXCreateMeshPartsFailed");
        return AEResult::Fail;
    }

    return AEResult::Ok;
}

AEResult ImporterFBX::GetNodeLocalTransform(FbxNode* node, glm::mat4& localTransform)
{
    /////////////////////////////////////
    //Pre-checks
    AEAssert(node != nullptr);
    if (node == nullptr)
    {
        return AEResult::NullParameter;
    }

    /////////////////////////////////////
    //Get Rotation Mul Order
    EFbxRotationOrder lRotationOrder;
    node->GetRotationOrder(FbxNode::eSourcePivot, lRotationOrder);
    RotationOrder ro = GetRotationOrderFromFBX(lRotationOrder);

    /////////////////////////////////////
    //Get FBX Local Transformation Info
    FbxVector4 fbxTranslation = node->LclTranslation.Get();
    FbxVector4 fbxRotation = node->LclRotation.Get();
    FbxVector4 fbxScaling = node->LclScaling.Get();

    /////////////////////////////////////
    //Convert to System Variables
    glm::vec3 translation = AEMathHelpers::Vec3fZero;
    glm::vec3 rotation = AEMathHelpers::Vec3fZero;
    glm::vec3 scale = AEMathHelpers::Vec3fZero;

    GetVec3FromFBXVec4(fbxTranslation, translation);
    GetVec3FromFBXVec4(fbxRotation, rotation);
    GetVec3FromFBXVec4(fbxScaling, scale);

    /////////////////////////////////////
    //Calculate Transform Matrix
    localTransform = AEMathHelpers::CreateTransform(translation, rotation, scale, ro);

    /////////////////////////////////////
    //Finish
    return AEResult::Ok;
}

RotationOrder ImporterFBX::GetRotationOrderFromFBX(FbxEuler::EOrder fbxRotationOrder)
{
    switch (fbxRotationOrder)
    {
        case FbxEuler::eOrderXYZ:
            return RotationOrder::EulerXYZ;

        case FbxEuler::eOrderXZY:
            return RotationOrder::EulerXZY;

        case FbxEuler::eOrderYZX:
            return RotationOrder::EulerYZX;

        case FbxEuler::eOrderYXZ:
            return RotationOrder::EulerYXZ;

        case FbxEuler::eOrderZXY:
            return RotationOrder::EulerZXY;

        case FbxEuler::eOrderZYX:
            return RotationOrder::EulerZYX;

        case FbxEuler::eOrderSphericXYZ:
        default:
            AETODO("Log error");
            AEAssert(false);

            return RotationOrder::EulerXYZ;
    }
}

AEResult ImporterFBX::GetMeshVertices(FbxMesh* fbxMesh, MeshHolder& meshHolder)
{
    AEAssert(fbxMesh != nullptr);
    if (fbxMesh == nullptr)
    {
        return AEResult::NullParameter;
    }

    int32_t polygonCount = fbxMesh->GetPolygonCount();
    FbxVector4* controlPoints = fbxMesh->GetControlPoints();
    int vertexId = 0;
    for (int32_t i = 0; i < polygonCount; ++i)
    {
        PolygonHolder polygonHolder;
        polygonHolder.m_PolygonIndex = i;

        int32_t polygonSize = fbxMesh->GetPolygonSize(i);

        AEAssert(polygonSize == 3);
        if (polygonSize != 3)
        {
            return AEResult::Fail;
        }

        int32_t j = 0;
        int32_t jSum = 1;

        for (int32_t x = 0; x < 3; ++x, j += jSum)
        {
            VertexHolder vertexHolder;

            ////////////////////////////////////////////
            //Set Vertex ID
            vertexHolder.m_VertexID = vertexId;

            ////////////////////////////////////////////
            //Set Control Point
            int32_t controlPointIndex = fbxMesh->GetPolygonVertex(i, j);
            vertexHolder.m_ControlPointIndex = controlPointIndex;

            ////////////////////////////////////////////
            //Set Position
            FbxVector4 vertice = controlPoints[controlPointIndex];
            GetVec3FromFBXVec4(vertice, vertexHolder.m_Position);

            ////////////////////////////////////////////
            //Get UVs
            GetPolygonUV(fbxMesh, controlPointIndex, i, j, vertexHolder.m_UV, vertexHolder.m_UV2);

            ////////////////////////////////////////////
            //Get Normal
            GetPolygonNormal(fbxMesh, controlPointIndex, vertexId, vertexHolder.m_Normal);

            if (m_ImportTangentBinormal && !m_GenerateTangenteBinormal)
            {
                ////////////////////////////////////////////
                //Get Tangent
                GetPolygonTangent(fbxMesh, controlPointIndex, vertexId, vertexHolder.m_Tangent);

                ////////////////////////////////////////////
                //Get Binormal
                GetPolygonBinormal(fbxMesh, controlPointIndex, vertexId, vertexHolder.m_Binormal);
            }

            if (m_BurnTransformation)
            {
                //Transform Position
                vertexHolder.m_Position = (glm::vec3)(meshHolder.m_WorldTransform * glm::vec4(vertexHolder.m_Position, 1));

                //Transform Normal
                vertexHolder.m_Normal = (glm::vec3)(meshHolder.m_WorldTransform * glm::vec4(vertexHolder.m_Normal, 0));

                //Transform Tangent
                vertexHolder.m_Tangent = (meshHolder.m_WorldTransform * vertexHolder.m_Tangent);

                //Transform Binormal
                vertexHolder.m_Binormal = (meshHolder.m_WorldTransform * vertexHolder.m_Binormal);
            }

            polygonHolder.m_Vertexs.push_back(vertexHolder);

            vertexId++;
        }

        meshHolder.m_PolygonHolderContainer.Add(polygonHolder);
    }

    return AEResult::Ok;
}

AEResult ImporterFBX::GetPolygonUV(FbxMesh* fbxMesh, int32_t controlPointIndex, int32_t polygonIndex, int32_t positionInPolygon, glm::vec2& uv, glm::vec2& uv2)
{
    AEAssert(fbxMesh != nullptr);
    if (fbxMesh == nullptr)
    {
        return AEResult::NullParameter;
    }

    int32_t uvCount = fbxMesh->GetElementUVCount();

    glm::vec2 uvsArr[2] = { AEMathHelpers::Vec2fZero, AEMathHelpers::Vec2fZero };

    uv = AEMathHelpers::Vec2fZero;
    uv2 = AEMathHelpers::Vec2fZero;

    if (uvCount == 0)
    {
        return AEResult::Ok;
    }

    if (uvCount > 2)
    {
        AETODO("Add FBX_UV_COUNT_ERR_MSG");
        std::string msgerr = fmt::format(AELOCMAN.GetLiteral("FBX_UV_COUNT_ERR_MSG"), __FUNCTION__, uvCount, 2);
        AELOGGER.AddNewLog(LogLevel::Warning, msgerr);
    }

    if (uvCount >= 2)
    {
        m_HasSecondUV = true;
    }

    for (int32_t i = 0; i < uvCount; ++i)
    {
        FbxGeometryElementUV* uvGE = fbxMesh->GetElementUV(i);
        FbxVector2 fbxUV(0, 0);

        switch (uvGE->GetMappingMode())
        {
            case FbxGeometryElement::eByControlPoint:
                switch (uvGE->GetReferenceMode())
                {
                    case FbxGeometryElement::eDirect:
                        fbxUV = uvGE->GetDirectArray().GetAt(controlPointIndex);
                        break;

                    case FbxGeometryElement::eIndexToDirect:
                        {
                            int id = uvGE->GetIndexArray().GetAt(controlPointIndex);
                            fbxUV = uvGE->GetDirectArray().GetAt(id);
                        }
                        break;

                    default:
                        {
                            AETODO("Add FBX_INVALID_UV_REF_MODE_ERR_MSG");
                            std::string msgerr = fmt::format(AELOCMAN.GetLiteral("FBX_INVALID_UV_REF_MODE_ERR_MSG"), __FUNCTION__);
                            AELOGGER.AddNewLog(LogLevel::Error, msgerr);
                        }
                        break; // other reference modes not shown here!
                }
                break;

            case FbxGeometryElement::eByPolygonVertex:
                {
                int textureUVIndex = fbxMesh->GetTextureUVIndex(polygonIndex, positionInPolygon);

                    switch (uvGE->GetReferenceMode())
                    {
                        case FbxGeometryElement::eDirect:
                        case FbxGeometryElement::eIndexToDirect:
                            fbxUV = uvGE->GetDirectArray().GetAt(textureUVIndex);
                            break;

                        default:
                        {
                            AETODO("Add FBX_INVALID_UV_REF_MODE_ERR_MSG");
                            std::string msgerr = fmt::format(AELOCMAN.GetLiteral("FBX_INVALID_UV_REF_MODE_ERR_MSG"), __FUNCTION__);
                            AELOGGER.AddNewLog(LogLevel::Error, msgerr);
                        }
                        break; // other reference modes not shown here!
                    }
                }
                break;

            case FbxGeometryElement::eByPolygon:    // doesn't make much sense for UVs
            case FbxGeometryElement::eAllSame:        // doesn't make much sense for UVs
            case FbxGeometryElement::eNone:            // doesn't make much sense for UVs
                {
                    AETODO("Add FBX_INVALID_UV_MAP_MODE_ERR_MSG");
                    std::string msgerr = fmt::format(AELOCMAN.GetLiteral("FBX_INVALID_UV_MAP_MODE_ERR_MSG"), __FUNCTION__);
                    AELOGGER.AddNewLog(LogLevel::Error, msgerr);
                }
                break;
        }

        //Set UV
        uvsArr[i].x = (float)fbxUV[0];
        uvsArr[i].y = 1 - (float)fbxUV[1];
    }

    uv = uvsArr[0];
    uv2 = uvsArr[1];

    return AEResult::Ok;
}

AEResult ImporterFBX::GetPolygonNormal(FbxMesh* fbxMesh, int32_t controlPointIndex, int32_t vertexId, glm::vec3& normal)
{
    AEAssert(fbxMesh != nullptr);
    if (fbxMesh == nullptr)
    {
        return AEResult::NullParameter;
    }

    int32_t normalCount = fbxMesh->GetElementNormalCount();

    normal = AEMathHelpers::Vec3fZero;
    if (normalCount == 0)
    {
        m_HasNormal = false;

        return AEResult::Ok;
    }

    if (normalCount > 1)
    {
        AETODO("Add FBX_NORMAL_COUNT_ERR_MSG");
        std::string msgerr = fmt::format(AELOCMAN.GetLiteral("FBX_NORMAL_COUNT_ERR_MSG"), __FUNCTION__, normalCount, 1);
        AELOGGER.AddNewLog(LogLevel::Warning, msgerr);
    }

    FbxGeometryElementNormal* normalElement = fbxMesh->GetElementNormal(0);
    FbxVector4 fbxNormal(0, 0, 0, 0);

    if (normalElement->GetMappingMode() == FbxGeometryElement::eByPolygonVertex)
    {
        switch (normalElement->GetReferenceMode())
        {
            case FbxGeometryElement::eDirect:
                fbxNormal = normalElement->GetDirectArray().GetAt(vertexId);
                break;

            case FbxGeometryElement::eIndexToDirect:
                {
                    int id = normalElement->GetIndexArray().GetAt(vertexId);
                    fbxNormal = normalElement->GetDirectArray().GetAt(id);
                }
                break;

            default:
            {
                AETODO("Add FBX_INVALID_NORMAL_REF_MODE_ERR_MSG");
                std::string msgerr = fmt::format(AELOCMAN.GetLiteral("FBX_INVALID_NORMAL_REF_MODE_ERR_MSG"), __FUNCTION__);
                AELOGGER.AddNewLog(LogLevel::Error, msgerr);
            }
            break; // other reference modes not shown here!
        }
    }
    else
    {
        AETODO("Add FBX_INVALID_NORMAL_MAP_MODE_ERR_MSG");
        std::string msgerr = fmt::format(AELOCMAN.GetLiteral("FBX_INVALID_NORMAL_MAP_MODE_ERR_MSG"), __FUNCTION__);
        AELOGGER.AddNewLog(LogLevel::Error, msgerr);
    }

    ///////////////////////////////////
    //Set Normal
    GetVec3FromFBXVec4(fbxNormal, normal);

    return AEResult::Ok;
}

AEResult ImporterFBX::GetPolygonTangent(FbxMesh* fbxMesh, int32_t controlPointIndex, int32_t vertexId, glm::vec4& tangent)
{
    AEAssert(fbxMesh != nullptr);
    if (fbxMesh == nullptr)
    {
        return AEResult::NullParameter;
    }

    int32_t tangentCount = fbxMesh->GetElementTangentCount();

    tangent = AEMathHelpers::Vec4fZero;

    if (tangentCount == 0)
    {
        m_GenerateTangenteBinormal = true;

        return AEResult::Ok;
    }

    if (tangentCount > 1)
    {
        AETODO("Add FBX_TANGENT_COUNT_ERR_MSG");
        std::string msgerr = fmt::format(AELOCMAN.GetLiteral("FBX_TANGENT_COUNT_ERR_MSG"), __FUNCTION__, tangentCount, 1);
        AELOGGER.AddNewLog(LogLevel::Warning, msgerr);
    }

    FbxGeometryElementTangent* tangentElement = fbxMesh->GetElementTangent(0);
    FbxVector4 fbxTangent(0, 0, 0, 0);

    if (tangentElement->GetMappingMode() == FbxGeometryElement::eByPolygonVertex)
    {
        switch (tangentElement->GetReferenceMode())
        {
            case FbxGeometryElement::eDirect:
                fbxTangent = tangentElement->GetDirectArray().GetAt(vertexId);
                break;

            case FbxGeometryElement::eIndexToDirect:
                {
                    int id = tangentElement->GetIndexArray().GetAt(vertexId);
                    fbxTangent = tangentElement->GetDirectArray().GetAt(id);
                }
                break;

            default:
                {
                    AETODO("Add FBX_INVALID_TANGENT_REF_MODE_ERR_MSG");
                    std::string msgerr = fmt::format(AELOCMAN.GetLiteral("FBX_INVALID_TANGENT_REF_MODE_ERR_MSG"), __FUNCTION__);
                    AELOGGER.AddNewLog(LogLevel::Error, msgerr);
                }
                break; // other reference modes not shown here!
        }
    }
    else
    {
        AETODO("Add FBX_INVALID_TANGENT_MAP_MODE_ERR_MSG");
        std::string msgerr = fmt::format(AELOCMAN.GetLiteral("FBX_INVALID_TANGENT_MAP_MODE_ERR_MSG"), __FUNCTION__);
        AELOGGER.AddNewLog(LogLevel::Error, msgerr);
    }

    /////////////////////////////////////////
    //Set Tangent
    GetVec4FromFBXVec4(fbxTangent, tangent);

    return AEResult::Ok;
}

AEResult ImporterFBX::GetPolygonBinormal(FbxMesh* fbxMesh, int32_t controlPointIndex, int32_t vertexId, glm::vec4& binormal)
{
    AEAssert(fbxMesh != nullptr);
    if (fbxMesh == nullptr)
    {
        return AEResult::NullParameter;
    }

    int32_t binormalCount = fbxMesh->GetElementBinormalCount();

    binormal = AEMathHelpers::Vec4fZero;

    if (binormalCount == 0)
    {
        m_GenerateTangenteBinormal = true;

        return AEResult::Ok;
    }

    if (binormalCount > 1)
    {
        AETODO("Add FBX_BINORMAL_COUNT_ERR_MSG");
        std::string msgerr = fmt::format(AELOCMAN.GetLiteral("FBX_BINORMAL_COUNT_ERR_MSG"), __FUNCTION__, binormalCount, 1);
        AELOGGER.AddNewLog(LogLevel::Warning, msgerr);
    }

    FbxGeometryElementBinormal* binormalElement = fbxMesh->GetElementBinormal(0);
    FbxVector4 fbxBinormal(0, 0, 0, 0);

    if (binormalElement->GetMappingMode() == FbxGeometryElement::eByPolygonVertex)
    {
        switch (binormalElement->GetReferenceMode())
        {
            case FbxGeometryElement::eDirect:
                fbxBinormal = binormalElement->GetDirectArray().GetAt(vertexId);
                break;

            case FbxGeometryElement::eIndexToDirect:
                {
                    int id = binormalElement->GetIndexArray().GetAt(vertexId);
                    fbxBinormal = binormalElement->GetDirectArray().GetAt(id);
                }
                break;

            default:
            {
                AETODO("Add FBX_INVALID_BINORMAL_REF_MODE_ERR_MSG");
                std::string msgerr = fmt::format(AELOCMAN.GetLiteral("FBX_INVALID_BINORMAL_REF_MODE_ERR_MSG"), __FUNCTION__);
                AELOGGER.AddNewLog(LogLevel::Error, msgerr);
            }
            break; // other reference modes not shown here!
        }
    }
    else
    {
        AETODO("Add FBX_INVALID_BINORMAL_MAP_MODE_ERR_MSG");
        std::string msgerr = fmt::format(AELOCMAN.GetLiteral("FBX_INVALID_BINORMAL_MAP_MODE_ERR_MSG"), __FUNCTION__);
        AELOGGER.AddNewLog(LogLevel::Error, msgerr);
    }

    //////////////////////////////////
    //Set Binormal
    GetVec4FromFBXVec4(fbxBinormal, binormal);

    return AEResult::Ok;
}

AEResult ImporterFBX::GetMeshMaterialMapping(FbxMesh* fbxMesh, MeshHolder& meshHolder)
{
    AEAssert(fbxMesh != nullptr);
    if (fbxMesh == nullptr)
    {
        return AEResult::NullParameter;
    }

    int32_t size = fbxMesh->GetElementMaterialCount();
    for (int32_t l = 0; l < size; ++l)
    {
        FbxGeometryElementMaterial* matElement = fbxMesh->GetElementMaterial(l);

        if (matElement != nullptr)
        {
            if (matElement->GetReferenceMode() == FbxGeometryElement::eIndex ||
                matElement->GetReferenceMode() == FbxGeometryElement::eIndexToDirect)
            {
                if (matElement->GetMappingMode() == FbxGeometryElement::eAllSame)
                {
                    int32_t matIdx = matElement->GetIndexArray().GetAt(0);
                    meshHolder.m_PolygonHolderContainer.SetMaterialIndexToAllPolygons(matIdx);
                }
                else
                {
                    int32_t lIndexArrayCount = matElement->GetIndexArray().GetCount();
                    for (int32_t i = 0; i < lIndexArrayCount; ++i)
                    {
                        int32_t matIdx = matElement->GetIndexArray().GetAt(i);
                        meshHolder.m_PolygonHolderContainer.SetMaterialIndexToPolygons(i, matIdx);
                    }
                }
            }
            else
            {
                AETODO("Add FBX_UNSUPPORTED_MATERIAL_REF_MODE_ERR_MSG");
                std::string msgerr = fmt::format(AELOCMAN.GetLiteral("FBX_UNSUPPORTED_MATERIAL_REF_MODE_ERR_MSG"), __FUNCTION__, (int32_t)matElement->GetReferenceMode());
                AELOGGER.AddNewLog(LogLevel::Error, msgerr);
            }
        }
    }

    return AEResult::Ok;
}

AEResult ImporterFBX::GetMaterialProperties(FbxGeometry* fbxGeometry)
{
    AEAssert(fbxGeometry != nullptr);
    if (fbxGeometry == nullptr)
    {
        return AEResult::NullParameter;
    }

    int32_t materialCount = 0;
    FbxNode* node = nullptr;

    node = fbxGeometry->GetNode();
    if (node != nullptr)
    {
        materialCount = node->GetMaterialCount();
    }

    if (materialCount <= 0)
    {
        return AEResult::Ok;
    }

    for (int32_t count = 0; count < materialCount; ++count)
    {
        FbxSurfaceMaterial* material = node->GetMaterial(count);

        uint64_t matID = material->GetUniqueID();

        m_MatIdxToMatIDMap[count] = matID;

        //Material not in Map and we need to get its properties
        if (m_MaterialHolderMap.find(matID) == m_MaterialHolderMap.end())
        {
            MaterialHolder materialHolder;
            materialHolder.m_MaterialID = matID;
            materialHolder.m_Name = AE_Base::String2WideStr(material->GetName());

            if (material->GetClassId().Is(FbxSurfacePhong::ClassId))
            {
                Color color;

                //Get the Diffuse Color
                FbxDouble3 doubleFbx3 = ((FbxSurfacePhong *)material)->Diffuse.Get();
                color = Color(    (float)doubleFbx3[0],
                                (float)doubleFbx3[1],
                                (float)doubleFbx3[2],
                                1.0f);

                materialHolder.m_DiffuseColor = color;
            }
            else if (material->GetClassId().Is(FbxSurfaceLambert::ClassId))
            {
                Color color;

                //Get the Diffuse Color
                FbxDouble3 doubleFbx3 = ((FbxSurfaceLambert *)material)->Diffuse.Get();
                color = Color(    (float)doubleFbx3[0],
                                (float)doubleFbx3[1],
                                (float)doubleFbx3[2],
                                1.0f);

                materialHolder.m_DiffuseColor = color;
            }
            else
            {
                AEAssert(false);

                AETODO("Add FBX_MATERIAL_NOT_SUPPORTED_ERR_MSG");
                std::string msgerr = fmt::format(AELOCMAN.GetLiteral("FBX_MATERIAL_NOT_SUPPORTED_ERR_MSG"), __FUNCTION__, materialHolder.m_Name);
                AELOGGER.AddNewLog(LogLevel::Error, msgerr);
            }

            m_MaterialHolderMap[matID] = materialHolder;
        }
    }

    return AEResult::Ok;
}

AEResult ImporterFBX::GetMaterialTextures(FbxGeometry* fbxGeometry)
{
    AEAssert(fbxGeometry != nullptr);
    if (fbxGeometry == nullptr)
    {
        return AEResult::NullParameter;
    }

    FbxNode* node = fbxGeometry->GetNode();
    if (node == nullptr)
    {
        return AEResult::NullObj;
    }

    int32_t matSize = node->GetSrcObjectCount<FbxSurfaceMaterial>();
    for (int32_t materialIndex = 0; materialIndex < matSize; ++materialIndex)
    {
        if (m_MatIdxToMatIDMap.find(materialIndex) == m_MatIdxToMatIDMap.end())
        {
            AETODO("Add FBX_MAT_INDEX_NOT_FOUND_ERR_MSG");
            std::string msgerr = fmt::format(AELOCMAN.GetLiteral("FBX_MAT_INDEX_NOT_FOUND_ERR_MSG"), __FUNCTION__, materialIndex);
            AELOGGER.AddNewLog(LogLevel::Warning, msgerr);

            return AEResult::Fail;
        }

        uint64_t matID = m_MatIdxToMatIDMap[materialIndex];

        FbxSurfaceMaterial *material = node->GetSrcObject<FbxSurfaceMaterial>(materialIndex);

        //go through all the possible textures
        if (material != nullptr)
        {
            int textureIndex = 0;
            FBXSDK_FOR_EACH_TEXTURE(textureIndex)
            {
                FbxProperty propertyFbx = material->FindProperty(FbxLayerElement::sTextureChannelNames[textureIndex]);

                if (propertyFbx.IsValid())
                {
                    int32_t textureCount = propertyFbx.GetSrcObjectCount<FbxTexture>();

                    for (int32_t j = 0; j < textureCount; ++j)
                    {
                        //Here we have to check if it's layered textures, or just textures:
                        FbxLayeredTexture* layeredTexture = propertyFbx.GetSrcObject<FbxLayeredTexture>(j);
                        if (layeredTexture)
                        {
                            AETODO("Add FBX_LAYERED_TEXTURE_NOT_SUPPORTED_ERR_MSG");
                            std::string msgerr = fmt::format(AELOCMAN.GetLiteral("FBX_LAYERED_TEXTURE_NOT_SUPPORTED_ERR_MSG"), __FUNCTION__);
                            AELOGGER.AddNewLog(LogLevel::Error, msgerr);
                        }
                        else
                        {
                            //no layered texture simply get on the property
                            FbxTexture* texture = propertyFbx.GetSrcObject<FbxTexture>(j);

                            if (texture != nullptr)
                            {
                                FbxFileTexture *fileTexture = FbxCast<FbxFileTexture>(texture);

                                if (fileTexture != nullptr)
                                {
                                    std::string sTextureType = propertyFbx.GetName();
                                    std::string textureType = AE_Base::String2WideStr(sTextureType);
                                    std::string filename = AE_Base::GetFilename(AE_Base::String2WideStr(fileTexture->GetFileName()));

                                    if (textureType.compare(L"DiffuseColor") == 0)
                                    {
                                        m_MaterialHolderMap[matID].m_DiffuseTextureString = filename;
                                    }
                                    else if (textureType.compare(L"Bump") == 0)
                                    {
                                        m_MaterialHolderMap[matID].m_NormalTextureString = filename;
                                    }
                                    else
                                    {
                                        AETODO("Add FBX_TEXTURE_TYPE_NOT_SUPPORTED_ERR_MSG");
                                        std::string msgerr = fmt::format(AELOCMAN.GetLiteral("FBX_TEXTURE_TYPE_NOT_SUPPORTED_ERR_MSG"), __FUNCTION__, textureType);
                                        AELOGGER.AddNewLog(LogLevel::Warning, msgerr);
                                    }
                                }
                                else
                                {
                                    std::string sTextureType = texture->GetTextureType();
                                    std::string textureType = AE_Base::String2WideStr(sTextureType);

                                    AETODO("Add FBX_TEXTURE_TYPE_NOT_SUPPORTED_ERR_MSG");
                                    std::string msgerr = fmt::format(AELOCMAN.GetLiteral("FBX_TEXTURE_TYPE_NOT_SUPPORTED_ERR_MSG"), __FUNCTION__, textureType);
                                    AELOGGER.AddNewLog(LogLevel::Warning, msgerr);
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    return AEResult::Ok;
}

AEResult ImporterFBX::GetMeshBlendIndicesWeight(FbxGeometry* fbxGeometry, MeshHolder& meshHolder)
{
    AEAssert(fbxGeometry != nullptr);
    if (fbxGeometry == nullptr)
    {
        return AEResult::NullParameter;
    }

    int32_t skinCount = fbxGeometry->GetDeformerCount(FbxDeformer::eSkin);
    for (int32_t i = 0; i < skinCount; ++i)
    {
        int32_t clusterCount = ((FbxSkin *)fbxGeometry->GetDeformer(i, FbxDeformer::eSkin))->GetClusterCount();

        for (int32_t j = 0; j < clusterCount; ++j)
        {
            FbxCluster* clusterFBX = ((FbxSkin *)fbxGeometry->GetDeformer(i, FbxDeformer::eSkin))->GetCluster(j);

            //Get Specific Bone Index to add to Blend Indices and Variables
            FbxSkeleton* skeletonFBX = (FbxSkeleton*)clusterFBX->GetLink()->GetNodeAttribute();
            uint64_t uniqueID = skeletonFBX->GetUniqueID();
            int32_t boneIndex = m_BoneMap[uniqueID]->GetIndex();

            int32_t indexCount = clusterFBX->GetControlPointIndicesCount();
            int* controlIndices = clusterFBX->GetControlPointIndices();
            double* weights = clusterFBX->GetControlPointWeights();

            for (int32_t k = 0; k < indexCount; ++k)
            {
                meshHolder.m_PolygonHolderContainer.SetBlendIndicesWeightToVertex(controlIndices[k], boneIndex, (float)weights[k]);
            }
        }
    }

    return AEResult::Ok;
}

AEResult ImporterFBX::CreateMeshParts(MeshHolder& meshHolder)
{
    PolygonHolderVecMap tempPart = meshHolder.m_PolygonHolderContainer.GetPolygonsSeparate();

    auto it = tempPart.begin();
    auto itEnd = tempPart.end();
    for (; it != itEnd; ++it)
    {
        const PolygonHolderVector& polyVect = it->second;

        MeshPartHolder meshPartHolder;

        std::map<std::string, uint16_t> vxtToIdx;
        uint64_t matID = 0;

        uint32_t polySize = static_cast<uint32_t>(polyVect.size());
        if (polySize != 0)
        {
            matID = m_MatIdxToMatIDMap[polyVect[0].m_MaterialIndex];
        }

        for (uint32_t i = 0; i < polySize; ++i)
        {
            const VertexHolderVector& vertexs = polyVect[i].m_Vertexs;
            uint16_t idx[] = { 0, 0, 0 };

            //Must be 3 Vertex per polygon
            AEAssert(vertexs.size() == 3);
            if (vertexs.size() != 3)
            {
                return AEResult::Fail;
            }

            for (uint32_t j = 0; j < 3; ++j)
            {
                std::string vtxString = vertexs[j].ToString();

                if (vxtToIdx.find(vtxString) != vxtToIdx.end())
                {
                    idx[j] = vxtToIdx[vtxString];
                }
                else
                {
                    idx[j] = static_cast<uint16_t>(meshPartHolder.m_VtxHolderVec.size());
                    vxtToIdx[vtxString] = idx[j];
                    meshPartHolder.m_VtxHolderVec.push_back(vertexs[j]);
                }
            }

            if (m_Clockwise)
            {
                meshPartHolder.m_IndexHolder.push_back(idx[0]);
                meshPartHolder.m_IndexHolder.push_back(idx[1]);
                meshPartHolder.m_IndexHolder.push_back(idx[2]);
            }
            else 
            {
                meshPartHolder.m_IndexHolder.push_back(idx[0]);
                meshPartHolder.m_IndexHolder.push_back(idx[2]);
                meshPartHolder.m_IndexHolder.push_back(idx[1]);
            }
        }

        meshPartHolder.m_VertexType = GetVertexType();

        meshHolder.m_MeshPartHolderVec.push_back(meshPartHolder);
    }

    return AEResult::Ok;
}

VertexType ImporterFBX::GetVertexType()
{
    if (!m_HasUV && !m_HasNormal && !m_HasSecondUV && !m_ImportTangentBinormal && !m_HasColor)
    {
        return VertexType::VtxPos;
    }

    if (!m_HasUV && !m_HasNormal && !m_HasSecondUV && !m_ImportTangentBinormal && m_HasColor)
    {
        return VertexType::VtxPosCol;
    }

    if (!m_HasUV && m_HasNormal && !m_HasSecondUV && !m_ImportTangentBinormal && !m_HasColor)
    {
        return VertexType::VtxPosNor;
    }

    if (!m_HasUV && m_HasNormal && !m_HasSecondUV && !m_ImportTangentBinormal && m_HasColor)
    {
        return VertexType::VtxPosNorCol;
    }

    if (m_HasUV && !m_HasNormal && !m_HasSecondUV && !m_ImportTangentBinormal)
    {
        return VertexType::VtxPosTex;
    }

    if (m_HasUV && m_HasNormal && !m_HasSecondUV && !m_ImportTangentBinormal && !m_HasAnimation)
    {
        return VertexType::VtxPosNorTex;
    }

    if (m_HasUV && m_HasNormal && !m_HasSecondUV && !m_ImportTangentBinormal && m_HasAnimation)
    {
        return VertexType::VtxPosNorTexIdxWght;
    }

    if (m_HasUV && m_HasNormal && m_HasSecondUV && !m_ImportTangentBinormal)
    {
        return VertexType::VtxPosNorTexTex2;
    }

    if (m_HasUV && m_HasNormal && !m_HasSecondUV && m_ImportTangentBinormal && !m_HasAnimation)
    {
        return VertexType::VtxPosNorTanBinTex;
    }

    if (m_HasUV && m_HasNormal && !m_HasSecondUV && m_ImportTangentBinormal && m_HasAnimation)
    {
        return VertexType::VtxPosNorTanBinTexIdxWght;
    }

    if (m_HasUV && m_HasNormal && m_HasSecondUV && m_ImportTangentBinormal)
    {
        return VertexType::VtxPosNorTanBinTexTex2;
    }

    AETODO("Add FBX_INVALID_COMB_VTX_TYPE_ERR_MSG");
    std::string msgerr = fmt::format(AELOCMAN.GetLiteral("FBX_INVALID_COMB_VTX_TYPE_ERR_MSG"), __FUNCTION__, "Position-Normal-TextCoords");
    AELOGGER.AddNewLog(LogLevel::Error, msgerr);

    return VertexType::VtxPosNorTex;
}

AEResult ImporterFBX::ExtractAnimation()
{
    //Get Frames Per Second, Frame Start and Frame End
    FbxTimeSpan timeSpanFBX;
    FbxTime startFrameFBX;
    FbxTime endFrameFBX;
    FbxTime timeFBX;

    m_Scene->GetGlobalSettings().GetTimelineDefaultTimeSpan(timeSpanFBX);
    startFrameFBX = timeSpanFBX.GetStart();
    endFrameFBX = timeSpanFBX.GetStop();

    m_StartFrame = (int32_t)startFrameFBX.GetFrameCount();
    m_EndFrame = (int32_t)endFrameFBX.GetFrameCount();

    m_FramesPerSecond = (uint32_t)timeFBX.GetFrameRate(m_Scene->GetGlobalSettings().GetTimeMode());
    m_SecondsInFrame = (1.0f / (float)m_FramesPerSecond);

    //Start Extracting Animation
    for (int32_t i = 0; i < m_Scene->GetSrcObjectCount<FbxAnimStack>(); ++i)
    {
        FbxAnimStack* animStack = m_Scene->GetSrcObject<FbxAnimStack>(i);

        //Extract Animation Collection
        ExtractAnimation(animStack, m_Scene->GetRootNode());
    }

    return AEResult::Ok;
}

AEResult ImporterFBX::ExtractAnimation(FbxAnimStack* animStack, FbxNode* node)
{
    AEAssert(node != nullptr);
    AEAssert(animStack != nullptr);

    if (node == nullptr || animStack == nullptr)
    {
        return AEResult::NullParameter;
    }

    //Animation Clip
    AnimationContent* animClip = new AnimationContent();

    std::string animClipName = animStack->GetName();
    animClip->m_Name = AE_Base::String2WideStr(animClipName);
    animClip->m_Duration = ((float)(m_EndFrame - m_StartFrame) * m_SecondsInFrame);

    int32_t animLayers = animStack->GetMemberCount<FbxAnimLayer>();

    for (int32_t i = 0; i < animLayers; ++i)
    {
        FbxAnimLayer* animLayer = animStack->GetMember<FbxAnimLayer>(i);

        ExtractAnimation(*animClip, animLayer, node);
    }

    m_Model->m_Animations[animClip->m_Name] = animClip;

    return AEResult::Ok;
}

AEResult ImporterFBX::ExtractAnimation(AnimationContent& animClip, FbxAnimLayer* animLayer, FbxNode* node)
{
    AEAssert(node != nullptr);
    AEAssert(animLayer != nullptr);
    if (node == nullptr || animLayer == nullptr)
    {
        return AEResult::NullParameter;
    }

    FbxNodeAttribute::EType attributeType = FbxNodeAttribute::eUnknown;
    if (node->GetNodeAttribute() != nullptr)
    {
        attributeType = (node->GetNodeAttribute()->GetAttributeType());

        if (attributeType == FbxNodeAttribute::eSkeleton)
        {
            //Extract Animation Sequence by Model (Mesh, bone...)
            ExtractAnimationChannels(animClip, node, animLayer);
        }
        else
        {
            AETODO("Only Extracting Animation from Bones");
        }
    }

    for (int32_t i = 0; i < node->GetChildCount(); ++i)
    {
        ExtractAnimation(animClip, animLayer, node->GetChild(i));
    }

    return AEResult::Ok;
}

AEResult ImporterFBX::ExtractAnimationChannels(AnimationContent& animClip, FbxNode* node, FbxAnimLayer* animLayer)
{
    AEAssert(node != nullptr);
    AEAssert(animLayer != nullptr);
    if (node == nullptr || animLayer == nullptr)
    {
        return AEResult::NullParameter;
    }


    //Get Rotation Mul Order
    EFbxRotationOrder lRotationOrder;
    node->GetRotationOrder(FbxNode::eSourcePivot, lRotationOrder);
    RotationOrder rotationOrder = GetRotationOrderFromFBX(lRotationOrder);

    //Get Pre-Rotations and Post Rotations
    FbxVector4 fbxRotationPre = node->GetPreRotation(FbxNode::eSourcePivot);
    glm::vec3 rotationPre = AEMathHelpers::Vec3fZero;
    GetVec3FromFBXVec4(fbxRotationPre, rotationPre);
    glm::quat preRotQuad = AEMathHelpers::CreateQuatRotation(rotationPre, rotationOrder);

    FbxVector4 fbxRotationPost = node->GetPostRotation(FbxNode::eSourcePivot);
    glm::vec3 rotationPost = AEMathHelpers::Vec3fZero;
    GetVec3FromFBXVec4(fbxRotationPost, rotationPost);
    glm::quat postRotQuad = AEMathHelpers::CreateQuatRotation(rotationPost, rotationOrder);

    /////////////////
    FbxAnimCurve* animCurve[9] = { 0 };

    //Extract Animation Curve for Translation if available
    animCurve[0] = node->LclTranslation.GetCurve(animLayer, FBXSDK_CURVENODE_COMPONENT_X);
    animCurve[1] = node->LclTranslation.GetCurve(animLayer, FBXSDK_CURVENODE_COMPONENT_Y);
    animCurve[2] = node->LclTranslation.GetCurve(animLayer, FBXSDK_CURVENODE_COMPONENT_Z);

    //Extract Animation Curve for Rotation if available
    animCurve[3] = node->LclRotation.GetCurve(animLayer, FBXSDK_CURVENODE_COMPONENT_X);
    animCurve[4] = node->LclRotation.GetCurve(animLayer, FBXSDK_CURVENODE_COMPONENT_Y);
    animCurve[5] = node->LclRotation.GetCurve(animLayer, FBXSDK_CURVENODE_COMPONENT_Z);

    //Extract Animation Curve for Scaling if available
    animCurve[6] = node->LclScaling.GetCurve(animLayer, FBXSDK_CURVENODE_COMPONENT_X);
    animCurve[7] = node->LclScaling.GetCurve(animLayer, FBXSDK_CURVENODE_COMPONENT_Y);
    animCurve[8] = node->LclScaling.GetCurve(animLayer, FBXSDK_CURVENODE_COMPONENT_Z);

    FbxSkeleton* skeletonFBX = (FbxSkeleton*)node->GetNodeAttribute();
    Bone* bone = m_BoneMap[skeletonFBX->GetUniqueID()];
    if (bone == nullptr)
    {
        return AEResult::NullObj;
    }

    return ExtractAnimationCurve(animClip, animCurve, bone->GetIndex(), preRotQuad, rotationOrder);
}

AEResult ImporterFBX::ExtractAnimationCurve(AnimationContent& animClip, FbxAnimCurve* animCurve[], int32_t boneIndex, const glm::quat& preRotQuad, RotationOrder rotationOrder)
{
    AEAssert(animCurve != nullptr);
    if (animCurve == nullptr)
    {
        return AEResult::NullParameter;
    }

    if (boneIndex < 0)
    {
        return AEResult::OutsideRange;
    }

    FbxTime fbxTime;
    for (int32_t i = m_StartFrame, timeMul = 0; i < m_EndFrame; ++i, ++timeMul)
    {
        BonePose bonePose;

        fbxTime.SetFrame(i);

        //Translation
        //bonePose.m_Translation.x = (animCurve[0] != nullptr) ? animCurve[0]->Evaluate(fbxTime) : bonePose.m_Translation.x;
        //bonePose.m_Translation.y = (animCurve[2] != nullptr) ? animCurve[2]->Evaluate(fbxTime) : bonePose.m_Translation.y;
        //bonePose.m_Translation.z = (animCurve[1] != nullptr) ? animCurve[1]->Evaluate(fbxTime) : bonePose.m_Translation.z;
        bonePose.m_Translation = AEMathHelpers::Vec3fZero;

        //Rotation
        glm::vec3 rotation = AEMathHelpers::Vec3fZero;
        //rotation.x = (animCurve[0 + 3] != nullptr) ? animCurve[0 + 3]->Evaluate(fbxTime) : rotation.x;
        //rotation.y = (animCurve[2 + 3] != nullptr) ? animCurve[2 + 3]->Evaluate(fbxTime) : rotation.y;
        //rotation.z = (animCurve[1 + 3] != nullptr) ? animCurve[1 + 3]->Evaluate(fbxTime) : rotation.z;

        bonePose.m_RotationQ = AEMathHelpers::CreateQuatRotation(rotation, rotationOrder);

        //bonePose.m_RotationQ = preRotQuad * bonePose.m_RotationQ;

        //Scaling
        //bonePose.m_Scale.x = (animCurve[0 + 6] != nullptr) ? animCurve[0 + 6]->Evaluate(fbxTime) : bonePose.m_Scale.x;
        //bonePose.m_Scale.y = (animCurve[2 + 6] != nullptr) ? animCurve[2 + 6]->Evaluate(fbxTime) : bonePose.m_Scale.y;
        //bonePose.m_Scale.z = (animCurve[1 + 6] != nullptr) ? animCurve[1 + 6]->Evaluate(fbxTime) : bonePose.m_Scale.z;
        bonePose.m_Scale = AEMathHelpers::Vec3fOne;

        //Once Information is set in Bone Pose, calculate Transformation
        bonePose.CalculateTransformMatrix();

        KeyFrame frame;

        frame.SetBoneIndex(boneIndex);
        frame.SetBonePose(bonePose);
        frame.SetTime(m_SecondsInFrame * timeMul);

        animClip.m_KeyFrames.push_back(frame);
    }

    return AEResult::Ok;
}


