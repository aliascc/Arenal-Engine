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

#pragma once

#ifndef _IMPORTER_FBX_H
#define _IMPORTER_FBX_H

/**********************
*   System Includes   *
***********************/
#include <map>

/*************************
*   3rd Party Includes   *
**************************/
#include "fbxsdk.h"
#include "glm\glm.hpp"

/***************************
*   Game Engine Includes   *
****************************/
#include "Base\Base.h"
#include "ImportersDefs.h"
#include "Base\AEObject.h"
#include "Math\AEMathDefs.h"
#include "Vertex\VertexDefs.h"
#include "Content\ContentDefs.h"

/********************
*   Forward Decls   *
*********************/
class Bone;
struct TreeBone;
struct ModelContent;
struct AnimationContent;

/*****************
*   Class Decl   *
******************/

/// <summary>
/// Imports an FBX file to memory
/// </summary>
class ImporterFBX sealed : public AEObject
{

    private:

        /************************
        *   Private Variables   *
        *************************/
#pragma region Private Variables

        /// <summary>
        /// FBX SDK Manager
        /// </summary>
        FbxManager* m_FBXSdkManager = nullptr;

        /// <summary>
        /// FBX Scene 
        /// </summary>
        FbxScene* m_Scene = nullptr;

        bool m_Clockwise = false;

        bool m_ImportTangentBinormal = false;

        bool m_BurnTransformation = false;

        bool m_GenerateTangenteBinormal = false;

        bool m_HasSecondUV = false;

        bool m_HasUV = false;

        bool m_HasNormal = false;

        bool m_HasColor = false;

        bool m_HasAnimation = false;

        uint32_t m_FramesPerSecond = 0;

        float m_SecondsInFrame = 0.0f;

        int32_t m_StartFrame = 0;

        int32_t m_EndFrame = 0;

        ModelContent* m_Model = nullptr;

        std::map<int32_t, uint64_t> m_MatIdxToMatIDMap;

        std::map<uint64_t, MaterialHolder> m_MaterialHolderMap;

        std::map<uint64_t, Bone*> m_BoneMap;

#pragma endregion

        /**********************
        *   Private Methods   *
        ***********************/
#pragma region Private Methods

        inline void GetVec3FromFBXVec4(const FbxVector4& fbxVec4, glm::vec3& vec)
        {
            vec.x = (float)fbxVec4[0];
            vec.y = (float)fbxVec4[2];
            vec.z = (float)fbxVec4[1];
        }

        inline void GetVec4FromFBXVec4(const FbxVector4& fbxVec4, glm::vec4& vec)
        {
            vec.x = (float)fbxVec4[0];
            vec.y = (float)fbxVec4[2];
            vec.z = (float)fbxVec4[1];
            vec.w = (float)fbxVec4[3];
        }

        /// <summary>
        /// Initializes the FBX SDK Objects
        /// </summary>
        /// <returns>Return AEResult::Ok if Initialize succeeded</returns>
        AEResult InitializeFBXSDK();

        /// <summary>
        /// Loads the File FBX Scene to Memory
        /// </summary>
        /// <returns>Returns AEResult::Ok if Scene was loaded successfully<seealso cref="AEResult"/></returns>
        AEResult LoadScene(const std::wstring& filename);

        AEResult BuildSkeletonHierarchy();

        AEResult BuildSkeletonHierarchy(FbxNode* node, const glm::mat4& parentTransform = AEMathHelpers::Mat4Identity, TreeBone* childBone = nullptr);

        AEResult ExtractContent();

        AEResult ExtractContent(FbxNode* node, const glm::mat4& parentTransform = AEMathHelpers::Mat4Identity);

        AEResult ExtractMesh(FbxNode* node, MeshHolder& meshHolder);

        AEResult GetNodeLocalTransform(FbxNode* node, glm::mat4& localTransform);

        RotationOrder GetRotationOrderFromFBX(FbxEuler::EOrder fbxRotationOrder);

        AEResult GetMeshVertices(FbxMesh* fbxMesh, MeshHolder& meshHolder);

        AEResult GetPolygonUV(FbxMesh* fbxMesh, int32_t controlPointIndex, int32_t polygonIndex, int32_t positionInPolygon, glm::vec2& uv, glm::vec2& uv2);

        AEResult GetPolygonNormal(FbxMesh* fbxMesh, int32_t controlPointIndex, int32_t vertexId, glm::vec3& normal);

        AEResult GetPolygonTangent(FbxMesh* fbxMesh, int32_t controlPointIndex, int32_t vertexId, glm::vec4& tangent);

        AEResult GetPolygonBinormal(FbxMesh* fbxMesh, int32_t controlPointIndex, int32_t vertexId, glm::vec4& binormal);

        AEResult GetMeshMaterialMapping(FbxMesh* fbxMesh, MeshHolder& meshHolder);

        AEResult GetMaterialProperties(FbxGeometry* fbxGeometry);

        AEResult GetMaterialTextures(FbxGeometry* fbxGeometry);

        AEResult GetMeshBlendIndicesWeight(FbxGeometry* fbxGeometry, MeshHolder& meshHolder);

        AEResult CreateMeshParts(MeshHolder& meshHolder);

        /// <summary>
        /// Returns Vertex Type of Current Mesh
        /// </summary>
        /// <returns>Returns Vertex Type<seealso cref="VertexType"/></returns>
        VertexType GetVertexType();

        AEResult ExtractAnimation();

        AEResult ExtractAnimation(FbxAnimStack* animStack, FbxNode* node);

        AEResult ExtractAnimation(AnimationContent& animClip, FbxAnimLayer* animLayer, FbxNode* node);

        AEResult ExtractAnimationChannels(AnimationContent& animClip, FbxNode* node, FbxAnimLayer* animLayer);

        AEResult ExtractAnimationCurve(AnimationContent& animClip, FbxAnimCurve* animCurve[], int32_t boneIndex, const glm::quat& preRotQuad, RotationOrder rotationOrder);

        void CleanUp();

#pragma endregion

    public:

        /***************************************
        *   Constructor & Destructor Methods   *
        ****************************************/
#pragma region Constructor & Destructor Methods

        /// <summary>
        /// ImporterFBX Constructor
        /// </summary>
        ImporterFBX();

        /// <summary>
        /// Default ImporterFBX Destructor
        /// </summary>
        virtual ~ImporterFBX();

        /******************
        *   Get Methods   *
        *******************/
#pragma region Get Methods

#pragma endregion

#pragma endregion

        /************************
        *   Framework Methods   *
        *************************/
#pragma region Framework Methods

        /// <param name="clockwise">How to import a Model for cull mode</param>
        AEResult ImportFBXFile(const std::wstring& filename, ModelContent** model, bool burnTransformation = true, bool importTangentBinormal = true, bool clockwise = true);

#pragma endregion

};

#endif
