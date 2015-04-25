/********************************************************
*
* Author: Carlos Chacón N.
*
* Copyright 2012-2015
*
*********************************************************/

#pragma once

#ifndef _IMPORTER_ASSIMP_H
#define _IMPORTER_ASSIMP_H

/**********************
*   System Includes   *
***********************/
#include <map>
#include <string>

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
#include "Base\Base.h"
#include "Base\XEObject.h"
#include "Math\XEMathDefs.h"
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
class ImporterAssimp sealed : public XEObject
{
	private:
		
		/*****************************
		*      Private Variables     *
		******************************/
#pragma region Private Variables

		ModelContent* m_Model = nullptr;

		std::map<std::wstring, MeshHolder*> m_MeshHolderMap;

		std::map<uint32_t, MeshHolder*> m_MeshPartIDToMeshHolderMap;

		std::map<std::wstring, TransformsHolder> m_TransformsMap;

		std::map<std::wstring, Bone*> m_BoneMap;

		std::vector<AnimationHolder> m_AnimationHolderVec;

		std::map<uint32_t, MaterialHolder> m_MaterialHolderMap;

#pragma endregion

		XEResult CreateBoudingShapes();

		XEResult ProcessSceneAnimations(const aiScene* scene, const std::vector<std::wstring>& animationNewNames);

		XEResult ProcessScene(const aiScene* scene, const std::vector<std::wstring>& animationNewNames);

		XEResult ProcessMeshes(const aiScene* scene);

		XEResult BuildSkeleton(const aiScene* scene);

		XEResult BuildSkeleton(const aiNode* node, TreeBone* childBone = nullptr);

		XEResult CorrectBoneIndices();

		XEResult ProcessSceneGraph(const aiScene* scene);

		XEResult ProcessNode(const aiNode* node, const glm::mat4& parentTransform = XEMathHelpers::Mat4Identity);

		XEResult ProcessAnimations(const aiScene* scene, const std::vector<std::wstring>& animationNewNames);
		
		XEResult GetAnimationKeyFrames(const aiScene* scene);
		
		XEResult BuildAnimations(const std::vector<std::wstring>& animationNewNames);

		XEResult ProcessMaterials(const aiScene* scene);

		XEResult BuildMaterials();

		std::wstring GetTexturePath(aiMaterial* assimpMat, uint32_t textureID, aiTextureType textType);

		XEResult ProcessEmbeddedTextures(const aiScene* scene);

		XEResult ConstructModel();

		void CleanUpMemory();

		void ReplaceAutoGenAssimpName(std::wstring& name, const std::wstring& newName, const std::wstring& typeName, int32_t idx, bool overrideNotAuto = false);

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

		XEResult ImportModel(const std::wstring& filePath, ModelContent** loadedModel, const std::wstring& texturePath = L".\\", bool calcTangentsBinormals = false, bool clockwise = true, bool lefthanded = false, const std::vector<std::wstring>& animationNewNames = std::vector<std::wstring>(0));

		XEResult ImportAnimations(const std::wstring& filePath, ModelContent** loadedAnimations, bool lefthanded = false, const std::vector<std::wstring>& animationNewNames = std::vector<std::wstring>(0));

};

#endif