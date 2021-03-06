/*
* Copyright (c) 2018 <Carlos Chac�n>
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

#ifndef _IMPORTER_ASSIMP_H
#define _IMPORTER_ASSIMP_H

/**********************
*   System Includes   *
***********************/

/*************************
*   3rd Party Includes   *
**************************/
#include "assimp\scene.h"
#include "assimp\anim.h"
#include "assimp\mesh.h"
#include "assimp\Importer.hpp"
#include "assimp\postprocess.h"

/***************************
*   Game Engine Includes   *
****************************/
#include "Vertex\VertexDefs.h"
#include "Content\ContentDefs.h"

/************
*   Using   *
*************/

/********************
*   Forward Decls   *
*********************/
struct ModelContent;

/*****************
*   Class Decl   *
******************/
class ImporterAssimp sealed : public AEObject
{
    private:
        
        /*****************************
        *      Private Variables     *
        ******************************/
#pragma region Private Variables

        ModelContent* m_Model = nullptr;

        std::map<std::string, MeshHolder*> m_MeshHolderMap;

        std::map<uint32_t, MeshHolder*> m_MeshPartIDToMeshHolderMap;

        std::map<std::string, TransformsHolder> m_TransformsMap;

        std::map<std::string, Bone*> m_BoneMap;

        std::vector<AnimationHolder> m_AnimationHolderVec;

        std::map<uint32_t, MaterialHolder> m_MaterialHolderMap;

#pragma endregion

        AEResult CreateBoudingShapes();

        AEResult ProcessSceneAnimations(const aiScene* scene, const std::vector<std::string>& animationNewNames);

        AEResult ProcessScene(const aiScene* scene, const std::vector<std::string>& animationNewNames);

        AEResult ProcessMeshes(const aiScene* scene);

        AEResult BuildSkeleton(const aiScene* scene);

        AEResult BuildSkeleton(const aiNode* node, TreeBone* childBone = nullptr);

        AEResult CorrectBoneIndices();

        AEResult ProcessSceneGraph(const aiScene* scene);

        AEResult ProcessNode(const aiNode* node, const glm::mat4& parentTransform = AEMathHelpers::Mat4Identity);

        AEResult ProcessAnimations(const aiScene* scene, const std::vector<std::string>& animationNewNames);
        
        AEResult GetAnimationKeyFrames(const aiScene* scene);
        
        AEResult BuildAnimations(const std::vector<std::string>& animationNewNames);

        AEResult ProcessMaterials(const aiScene* scene);

        AEResult BuildMaterials();

        std::string GetTexturePath(aiMaterial* assimpMat, uint32_t textureID, aiTextureType textType);

        AEResult ProcessEmbeddedTextures(const aiScene* scene);

        AEResult ConstructModel();

        void CleanUpMemory();

        void ReplaceAutoGenAssimpName(std::string& name, const std::string& newName, const std::string& typeName, int32_t idx, bool overrideNotAuto = false);

    public:

        /***************************************
        *   Constructor & Destructor Methods   *
        ****************************************/
#pragma region Constructor & Destructor Methods

        /// <summary>
        /// ImporterAssimp Constructor
        /// </summary>
        ImporterAssimp();

        /// <summary>
        /// Default ImporterAssimp Destructor
        /// </summary>
        virtual ~ImporterAssimp();

#pragma endregion

        AEResult ImportModel(const std::string& filePath, ModelContent** loadedModel, const std::string& texturePath = ".\\", bool calcTangentsBinormals = false, bool clockwise = true, bool lefthanded = false, const std::vector<std::string>& animationNewNames = std::vector<std::string>(0));

        AEResult ImportAnimations(const std::string& filePath, ModelContent** loadedAnimations, bool lefthanded = false, const std::vector<std::string>& animationNewNames = std::vector<std::string>(0));

};

#endif
