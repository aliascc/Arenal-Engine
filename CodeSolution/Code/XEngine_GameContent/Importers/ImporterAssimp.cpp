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
#include <vector>

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
#include "Models\Mesh.h"
#include "Logger\Logger.h"
#include "ImportersDefs.h"
#include "ImporterAssimp.h"
#include "Utils\BoundingBox.h"
#include "Base\BaseFunctions.h"
#include "Content\MeshContent.h"
#include "Models\Skinning\Bone.h"
#include "Content\ModelContent.h"
#include "Utils\BoundingSphere.h"
#include "Content\MeshPartContent.h"
#include "Content\SkeletonContent.h"
#include "Content\AnimationContent.h"
#include "Models\Skinning\SkinningDefs.h"
#include "Localization\LocalizationManager.h"
#include "Localization\LocalizationManagerDefs.h"

//Always include last
#include "Memory\MemLeaks.h"

/************
*   Using   *
*************/

/********************
*   Function Defs   *
*********************/
XETODO("Need to change materials to create material and associate it with shader, that is when texture is read from the model, we will not associated with it, but create a Material (Shader + Texture Prop) and add it to a material folder");
ImporterAssimp::ImporterAssimp()
{
}

ImporterAssimp::~ImporterAssimp()
{
	CleanUpMemory();
}

void ImporterAssimp::CleanUpMemory()
{
	for(auto itMeshHolder : m_MeshHolderMap)
	{
		DeleteMem(itMeshHolder.second);
	}

	//Clear Mesh Map
	m_MeshHolderMap.clear();

	//Clean Bones
	m_BoneMap.clear();

	//Clean Transform Maps for Nodes
	m_TransformsMap.clear();
	
	//Clear Mesh Part ID to Mesh Holder Map
	m_MeshPartIDToMeshHolderMap.clear();

	//Clear Animation Vector
	m_AnimationHolderVec.clear();

	//Clear Material Map
	m_MaterialHolderMap.clear();

	//Clear Model
	m_Model = nullptr;
}

XEResult ImporterAssimp::ImportAnimations(const std::wstring& filePath, ModelContent** loadedAnimations, bool lefthanded, const std::vector<std::wstring>& animationNewNames)
{
	XEAssert(loadedAnimations != nullptr);
	if(loadedAnimations == nullptr)
	{
		return XEResult::NullParameter;
	}

	XEAssert(!filePath.empty());
	if(filePath.empty())
	{
		return XEResult::EmptyFilename;
	}

	uint32_t assimpImportFlags =	aiProcess_LimitBoneWeights	|		//Limit Bone to 4
									aiProcess_RemoveComponent	;		//Remove unnecessary components
	
	if(lefthanded)
	{
		assimpImportFlags |= aiProcess_MakeLeftHanded; //Make Left Handed
	}

	std::string sFilePath = XE_Base::WideStr2String(filePath);

	Assimp::Importer assimpImporter;

	//Set Remove Component Flags
	assimpImporter.SetPropertyInteger(AI_CONFIG_PP_RVC_FLAGS,	aiComponent_NORMALS					|  
																aiComponent_TANGENTS_AND_BITANGENTS	|
																aiComponent_COLORS					|
																aiComponent_TEXCOORDS				|  
																aiComponent_TEXTURES				|
																aiComponent_LIGHTS					|
																aiComponent_CAMERAS					|
																aiComponent_MESHES					|
																aiComponent_MATERIALS				);

	const aiScene* assimpScene = assimpImporter.ReadFile(sFilePath, assimpImportFlags);

	if(assimpScene == nullptr)
	{
		std::wstring errMsg = L"";
		std::wstring assimpError = XE_Base::String2WideStr(assimpImporter.GetErrorString());
		fastformat::fmt(errMsg, XELOCMAN->GetLiteral(L"ASSIMP_LOAD_SCENE_FAIL_ERR_MSG"), __FUNCTIONW__, assimpError);
		XELOGGER->AddNewLog(LogLevel::Error, errMsg);

		return XEResult::LoadFileFailed;
	}

	//Clean up Memory
	CleanUpMemory();

	//Create Model
	m_Model = new ModelContent();

	//Set Filename
	m_Model->m_FileName = XE_Base::GetFilenameOnly(filePath);

	//Process Scene
	XEResult ret = ProcessSceneAnimations(assimpScene, animationNewNames);
	if(ret != XEResult::Ok)
	{
		DeleteMem(m_Model);

		CleanUpMemory();

		return ret;
	}

	XETODO("Move ASSIMP_MODEL_FAIL_TO_INIT to another place");

	(*loadedAnimations) = m_Model;
	
	CleanUpMemory();

	return XEResult::Ok;
}

XEResult ImporterAssimp::ProcessSceneAnimations(const aiScene* scene, const std::vector<std::wstring>& animationNewNames)
{
	if(scene == nullptr)
	{
		return XEResult::NullParameter;
	}

	//Get Transformations for all nodes and stores them
	if(ProcessSceneGraph(scene) != XEResult::Ok)
	{
		return XEResult::AssimpProcessGraph;
	}

	if(BuildSkeleton(scene) != XEResult::Ok)
	{
		return XEResult::AssimpProcessSkeleton;
	}

	if(ProcessAnimations(scene, animationNewNames) != XEResult::Ok)
	{
		return XEResult::AssimpProcessAnimation;
	}

	return XEResult::Ok;
}

XEResult ImporterAssimp::ImportModel(const std::wstring& filePath, ModelContent** loadedModel, const std::wstring& texturePath, bool calcTangentsBinormals, bool clockwise, bool lefthanded, const std::vector<std::wstring>& animationNewNames)
{
	XEAssert(loadedModel != nullptr);

	if(loadedModel == nullptr)
	{
		return XEResult::NullParameter;
	}

	XEAssert(!filePath.empty());

	if(filePath.empty())
	{
		return XEResult::EmptyFilename;
	}

	uint32_t assimpImportFlags =	aiProcess_Triangulate			|	//Triangulate 
									aiProcess_JoinIdenticalVertices	|	//Join Vertex for Index Buffer
									aiProcess_SortByPType			|
									aiProcess_LimitBoneWeights		|	//Limit Bone to 4
									aiProcess_FlipUVs;					//This step flips all UV coordinates along the y-axis and adjusts material settings and bitangents accordingly

	if(calcTangentsBinormals)
	{
		assimpImportFlags |= aiProcess_CalcTangentSpace;
	}

	if(lefthanded)
	{
		assimpImportFlags |= aiProcess_MakeLeftHanded; //Make Left Handed
	}

	if(clockwise)
	{
		assimpImportFlags |= aiProcess_FlipWindingOrder; //For Clockwise
	}

	std::string sFilePath = XE_Base::WideStr2String(filePath);

	Assimp::Importer assimpImporter;

	const aiScene* assimpScene = assimpImporter.ReadFile(sFilePath, assimpImportFlags);

	if(assimpScene == nullptr)
	{
		std::wstring errMsg = L"";
		std::wstring assimpError = XE_Base::String2WideStr(assimpImporter.GetErrorString());
		fastformat::fmt(errMsg, XELOCMAN->GetLiteral(L"ASSIMP_LOAD_SCENE_FAIL_ERR_MSG"), __FUNCTIONW__, assimpError);
		XELOGGER->AddNewLog(LogLevel::Error, errMsg);

		return XEResult::LoadFileFailed;
	}

	//Clean up Memory
	CleanUpMemory();

	//Create Model
	m_Model = new ModelContent();

	//Set Filename
	m_Model->m_FileName = XE_Base::GetFilenameOnly(filePath);

	//Process Scene
	XEResult ret = ProcessScene(assimpScene, animationNewNames);
	if(ret != XEResult::Ok)
	{
		DeleteMem(m_Model);

		CleanUpMemory();

		return ret;
	}

	XETODO("Move ASSIMP_MODEL_FAIL_TO_INIT to another place");

	(*loadedModel) = m_Model;

	CleanUpMemory();

	return XEResult::Ok;
}

XEResult ImporterAssimp::ProcessScene(const aiScene* scene, const std::vector<std::wstring>& animationNewNames)
{
	if(scene == nullptr)
	{
		return XEResult::NullParameter;
	}

	//Get Transformations for all nodes and stores them
	if(ProcessSceneGraph(scene) != XEResult::Ok)
	{
		return XEResult::AssimpProcessGraph;
	}

	if(ProcessMaterials(scene) != XEResult::Ok)
	{
		return XEResult::AssimpProcessMaterials;
	}

	if(ProcessEmbeddedTextures(scene) != XEResult::Ok)
	{
		return XEResult::AssimpProcessEmbeddedTextures;
	}

	if(ProcessMeshes(scene) != XEResult::Ok)
	{
		return XEResult::AssimpProcessMeshes;
	}

	if(BuildSkeleton(scene) != XEResult::Ok)
	{
		return XEResult::AssimpProcessSkeleton;
	}

	if(ProcessAnimations(scene, animationNewNames) != XEResult::Ok)
	{
		return XEResult::AssimpProcessAnimation;
	}

	if(ConstructModel() != XEResult::Ok)
	{
		return XEResult::AssimpProcessConstructModel;
	}

	if (CreateBoudingShapes() != XEResult::Ok)
	{
		XETODO("Add better return code");
		return XEResult::Fail;
	}

	return XEResult::Ok;
}

XEResult ImporterAssimp::ProcessSceneGraph(const aiScene* scene)
{
	if(scene == nullptr)
	{
		return XEResult::NullParameter;
	}

	aiNode* root = scene->mRootNode;
	
	if(root == nullptr)
	{
		return XEResult::Ok;
	}

	m_Model->m_Name = XE_Base::String2WideStr(scene->mRootNode->mName.C_Str());
	ReplaceAutoGenAssimpName(m_Model->m_Name, m_Model->m_FileName, XE_IMPORTER_TYPE_MODEL_NAME, -1);

	for(uint32_t i = 0; i < root->mNumChildren; ++i)
	{
		if(ProcessNode(root->mChildren[i]) != XEResult::Ok)
		{
			return XEResult::Fail;
		}
	}
	
	return XEResult::Ok;
}

XEResult ImporterAssimp::ProcessNode(const aiNode* node, const glm::mat4& parentTransform)
{
	if(node == nullptr)
	{
		return XEResult::Ok;
	}

	std::wstring nodeName = XE_Base::String2WideStr(node->mName.C_Str());

	if(nodeName.empty())
	{
		std::wstring parentName = L"!NO PARENT!";
	
		if(node->mParent != nullptr)
		{
			parentName = XE_Base::String2WideStr(node->mParent->mName.C_Str());
		}

		std::wstring errMsg = L"";
		fastformat::fmt(errMsg, XELOCMAN->GetLiteral(L"ASSIMP_NODE_NAME_EMPTY_ERR_MSG"), __FUNCTIONW__, parentName);
		XELOGGER->AddNewLog(LogLevel::Warning, errMsg);

		return XEResult::Ok;
	}
	
	glm::mat4 localTransform = XEImporterHelpers::ConvertAssimpMatrix(node->mTransformation);
	glm::mat4 worldTransform = localTransform * parentTransform;

	if(node->mNumMeshes != 0)
	{
		XEAssert(m_MeshHolderMap.find(nodeName) == m_MeshHolderMap.end());

		MeshHolder* meshHolder = new MeshHolder();

		meshHolder->m_Name = nodeName;
		meshHolder->m_WorldTransform = worldTransform;

		for(uint32_t j = 0; j < node->mNumMeshes; ++j)
		{
			uint32_t meshPartID = node->mMeshes[j];

			XEAssert(m_MeshPartIDToMeshHolderMap.find(meshPartID) == m_MeshPartIDToMeshHolderMap.end());

			m_MeshPartIDToMeshHolderMap[meshPartID] = meshHolder;
		}

		m_MeshHolderMap[nodeName] = meshHolder;
	}
	else
	{	
		XEAssert(m_TransformsMap.find(nodeName) == m_TransformsMap.end());

		TransformsHolder transHolder;

		transHolder.m_LocalTransform = localTransform;
		transHolder.m_WorldTransform = worldTransform;

		m_TransformsMap[nodeName] = transHolder;
	}

	for(uint32_t i = 0; i < node->mNumChildren; ++i)
	{
		ProcessNode(node->mChildren[i], worldTransform);
	}

	return XEResult::Ok;
}

XEResult ImporterAssimp::ProcessMeshes(const aiScene* scene)
{
	if(scene == nullptr)
	{
		return XEResult::NullParameter;
	}
	
	for(uint32_t i = 0; i < scene->mNumMeshes; ++i)
	{
		uint32_t j = 0;
		uint32_t x = 0;

		//Get Transform Holder to Translate Vertices to world space
		XEAssert(m_MeshPartIDToMeshHolderMap.find(i) != m_MeshPartIDToMeshHolderMap.end());
		MeshHolder* meshHolder = m_MeshPartIDToMeshHolderMap[i];

		//Replace Autoname
		ReplaceAutoGenAssimpName(meshHolder->m_Name, m_Model->m_FileName, XE_IMPORTER_TYPE_MESH_NAME, i);

		//Get Assimp Mesh
		aiMesh* assimpMesh = scene->mMeshes[i];

		//We only are accepting Triangle type primitives for now
		//We also expect that Polygons do not appear as we have 
		//triangulated the mesh in the importer.
		if(assimpMesh->mPrimitiveTypes != aiPrimitiveType_TRIANGLE)
		{
			std::wstring errMsg = L"";
			fastformat::fmt(errMsg, XELOCMAN->GetLiteral(L"ASSIMP_PRIMTIVE_TYPE_WRG_ERR_MSG"), __FUNCTIONW__, meshHolder->m_Name);
			XELOGGER->AddNewLog(LogLevel::Warning, errMsg);
			
			continue;
		}

		//If mesh does not have Faces that means it is not
		//indexed, we will not accept this
		if(!assimpMesh->HasFaces())
		{
			std::wstring errMsg = L"";
			fastformat::fmt(errMsg, XELOCMAN->GetLiteral(L"ASSIMP_NO_FACES_ERR_MSG"), __FUNCTIONW__, meshHolder->m_Name);
			XELOGGER->AddNewLog(LogLevel::Warning, errMsg);
			
			continue;
		}
		
		//We want to know if the Mesh we are importing can be supported by
		//our Vertex Types, all imported Meshes must have Positions
		uint32_t vtxType = XE_VERTEX_TYPE_POSITION;

		if(assimpMesh->HasPositions() == false)
		{
			std::wstring errMsg = L"";
			fastformat::fmt(errMsg, XELOCMAN->GetLiteral(L"ASSIMP_NO_POSITIONS_ERR_MSG"), __FUNCTIONW__, meshHolder->m_Name);
			XELOGGER->AddNewLog(LogLevel::Warning, errMsg);
			
			continue;
		}

		//We only Support 2 Tex Coords right now
		//Let the user know we will import only 2 and the rest are ignored
		if(assimpMesh->GetNumUVChannels() > XE_IMPORTER_MAX_TEX_COORD_CHANNELS)
		{
			std::wstring errMsg = L"";
			fastformat::fmt(errMsg, XELOCMAN->GetLiteral(L"ASSIMP_TEX_COORD_NUM_CHANNELS_ERR_MSG"), __FUNCTIONW__, meshHolder->m_Name, assimpMesh->GetNumUVChannels(), XE_IMPORTER_MAX_TEX_COORD_CHANNELS);
			XELOGGER->AddNewLog(LogLevel::Warning, errMsg);
		}

		//Check UV Channels Configuration
		if(assimpMesh->HasTextureCoords(0))
		{
			//We currently only support 2D Tex Coords
			//Let the user know that there was an error 
			//and continue with next mesh			
			if(assimpMesh->mNumUVComponents[0] != XE_IMPORTER_MAX_TEX_COORD_COMPONENTS)
			{
				std::wstring errMsg = L"";
				fastformat::fmt(errMsg, XELOCMAN->GetLiteral(L"ASSIMP_TEX_COORD_NUM_COMPONENTS_ERR_MSG"), __FUNCTIONW__, meshHolder->m_Name, assimpMesh->GetNumUVChannels(), XE_IMPORTER_MAX_TEX_COORD_COMPONENTS);
				XELOGGER->AddNewLog(LogLevel::Warning, errMsg);

				continue;
			}

			vtxType |= XE_VERTEX_TYPE_TEXCOORD;

			if(assimpMesh->HasTextureCoords(1))
			{	
				//We currently only support 2D Tex Coords
				//Let the user know that there was an error 
				//and continue with next mesh			
				if(assimpMesh->mNumUVComponents[1] != 2)
				{
					XETODO("Log error");

					continue;
				}

				vtxType |= XE_VERTEX_TYPE_TEXCOORD2;
			}
		}
		else if(assimpMesh->GetNumColorChannels() > 0)
		{
			//We only Support 1 Color Channel right now
			//Let the user know we will import only 1 and the rest are ignored
			if(assimpMesh->GetNumColorChannels() > XE_IMPORTER_MAX_COLOR_CHANNELS)
			{
				std::wstring errMsg = L"";
				fastformat::fmt(errMsg, XELOCMAN->GetLiteral(L"ASSIMP_COLOR_NUM_CHANNELS_ERR_MSG"), __FUNCTIONW__, meshHolder->m_Name, assimpMesh->GetNumUVChannels(), XE_IMPORTER_MAX_COLOR_CHANNELS);
				XELOGGER->AddNewLog(LogLevel::Warning, errMsg);
			}

			vtxType |= XE_VERTEX_TYPE_COLOR;
		}

		//Check if Mesh has normals
		if(assimpMesh->HasNormals())
		{
			vtxType |= XE_VERTEX_TYPE_NORMAL;
		}


		//Check if Mesh has Tangents and Binormals
		if(assimpMesh->HasTangentsAndBitangents())
		{
			vtxType |= XE_VERTEX_TYPE_BINORMAL | XE_VERTEX_TYPE_TANGENT;
		}

		//Check if our mesh has bones
		if(assimpMesh->HasBones())
		{
			vtxType |= XE_VERTEX_TYPE_INDICES | XE_VERTEX_TYPE_WEIGHT;
		}

		//Check if our vertex combinations matches a possible
		//Vertex Type we can configure, if it does not, let the
		//User know
		if(!XEImporterHelpers::IsVertexTypeValid((VertexType)vtxType))
		{
			std::wstring errMsg = L"";
			fastformat::fmt(errMsg, XELOCMAN->GetLiteral(L"ASSIMP_COMB_VERTEX_PROPS_ERR_MSG"), __FUNCTIONW__, meshHolder->m_Name, vtxType);
			XELOGGER->AddNewLog(LogLevel::Warning, errMsg);
			
			continue;
		}

		////////////////////////////////////////////////////////////////////////////////
		//Now that the Pre-check is ready we move on to the fun part, extract
		//the information from ASSIMP
		////////////////////////////////////////////////////////////////////////////////

		//Get Material ID
		uint32_t materialID = assimpMesh->mMaterialIndex;

		//Vector for Vertices Info
		std::vector<VertexHolder> vtxHolderVect;

		//Get Vertices Information
		for(j = 0; j < assimpMesh->mNumVertices; ++j)
		{
			VertexHolder vtxHolder;
			
			//Get Position
			aiVector3D vtx = assimpMesh->mVertices[j];
			vtxHolder.m_Position = glm::vec3(vtx.x, vtx.y, vtx.z);

			//Transform Position
			vtxHolder.m_Position = (glm::vec3)(meshHolder->m_WorldTransform * glm::vec4(vtxHolder.m_Position, 1));

			if(vtxType & XE_VERTEX_TYPE_NORMAL)
			{
				//Get Normals
				aiVector3D nrm = assimpMesh->mNormals[j];
				vtxHolder.m_Normal = glm::vec3(nrm.x, nrm.y, nrm.z);

				//Transform Normals
				vtxHolder.m_Normal = (glm::vec3)(meshHolder->m_WorldTransform * glm::vec4(vtxHolder.m_Normal, 0));
			}

			if(vtxType & XE_VERTEX_TYPE_TEXCOORD)
			{
				//Get TexCoord 1
				aiVector3D uv = assimpMesh->mTextureCoords[0][j];
				vtxHolder.m_UV = glm::vec2(uv.x, uv.y);
			}

			if(vtxType & XE_VERTEX_TYPE_TEXCOORD2)
			{
				//Get TexCoord 2
				aiVector3D uv = assimpMesh->mTextureCoords[1][j];
				vtxHolder.m_UV2 = glm::vec2(uv.x, uv.y);
			}

			if(vtxType & XE_VERTEX_TYPE_COLOR)
			{
				//Get Color
				aiColor4D color = assimpMesh->mColors[0][j];
				vtxHolder.m_Color = Color(color.r, color.g, color.b, color.a);
			}
			
			if(vtxType & (XE_VERTEX_TYPE_TANGENT | XE_VERTEX_TYPE_BINORMAL))
			{
				//Get Tangent and Binormal(also known as bitangent)
				aiVector3D tgt = assimpMesh->mTangents[j];
				aiVector3D binor = assimpMesh->mBitangents[j];
				
				vtxHolder.m_Tangent = glm::vec4(tgt.x, tgt.y, tgt.z, 0);
				vtxHolder.m_Binormal = glm::vec4(binor.x, binor.y, binor.z, 0);

				//Transform Tangent and Binormal
				vtxHolder.m_Tangent = meshHolder->m_WorldTransform * vtxHolder.m_Tangent;
				vtxHolder.m_Binormal = meshHolder->m_WorldTransform * vtxHolder.m_Binormal;
			}

			vtxHolderVect.push_back(vtxHolder);
		}

		//Get Indices
		XETODO("Check to be 16 or 32");
		std::vector<uint16_t> indices;

		//Get Indices
		for(j = 0; j < assimpMesh->mNumFaces; ++j)
		{
			aiFace face = assimpMesh->mFaces[j];

			XEAssert(face.mNumIndices == 3);
			
			for(x = 0; x < face.mNumIndices; ++x)
			{
				indices.push_back((uint16_t)face.mIndices[x]);
			}
		}

		//Get Bones
		for (j = 0; j < assimpMesh->mNumBones; ++j)
		{
			aiBone* assimpBone = assimpMesh->mBones[j];
			Bone* bone = nullptr;

			std::wstring boneName = XE_Base::String2WideStr(assimpBone->mName.C_Str());

			glm::mat4 boneOffset = XEImporterHelpers::ConvertAssimpMatrix(assimpBone->mOffsetMatrix);

			if(m_BoneMap.find(boneName) == m_BoneMap.end())
			{
				//Get Transform Holder to Bones
				XEAssert(m_TransformsMap.find(boneName) != m_TransformsMap.end());
				const TransformsHolder& boneTransHolder = m_TransformsMap[boneName];

				bone = new Bone();

				bone->SetOffSetMatrix(boneOffset);
				bone->SetLocalMatrix(boneTransHolder.m_LocalTransform);
				bone->SetWorldMatrix(boneTransHolder.m_WorldTransform);
				bone->SetName(boneName);

				m_BoneMap[boneName] = bone;
			}
			
			for(x = 0; x < assimpBone->mNumWeights; ++x)
			{
				uint32_t boneIndex = 0;
				aiVertexWeight vtxWeight = assimpBone->mWeights[x];

				uint32_t vtxID = vtxWeight.mVertexId;

				if(vtxHolderVect[vtxID].m_BoneToIndex.find(boneName) != vtxHolderVect[vtxID].m_BoneToIndex.end())
				{
					boneIndex = vtxHolderVect[vtxID].m_BoneToIndex[boneName];
				}
				else
				{
					boneIndex = vtxHolderVect[vtxID].m_BlendIndexCount;

					vtxHolderVect[vtxID].m_BlendIndexCount++;

					XETODO("Set error for more thanm 4 weights");
					XEAssert(vtxHolderVect[vtxID].m_BlendIndexCount <= 4);

					vtxHolderVect[vtxID].m_BoneToIndex[boneName] = boneIndex;
					vtxHolderVect[vtxID].m_IndexToBone[boneIndex] = boneName;
				}

				float* boneWeights = (float*)&vtxHolderVect[vtxID].m_BlendWeights;

				boneWeights[boneIndex] = vtxWeight.mWeight;
			}
		}

		//Now save the Data to a Mesh Holder Struct
		MeshPartHolder meshPart;

		meshPart.m_MaterialID = materialID;
		meshPart.m_VertexType = (VertexType)vtxType;
		meshPart.m_IndexHolder = indices;
		meshPart.m_VtxHolderVec = vtxHolderVect;

		meshHolder->m_MeshPartHolderVec.push_back(meshPart);
	}

	return XEResult::Ok;
}

XEResult ImporterAssimp::BuildSkeleton(const aiScene* scene)
{
	if(scene == nullptr)
	{
		return XEResult::NullParameter;
	}

	//Create and Set new Skeleton for Model
	SkeletonContent* skeleton = m_Model->m_SkeletonContent;
	
	aiNode* root = scene->mRootNode;

	//Go through scene and build Skeleton
	if(root != nullptr)
	{
		for(uint32_t i = 0; i < root->mNumChildren; ++i)
		{
			if(BuildSkeleton(root->mChildren[i]) != XEResult::Ok)
			{
				return XEResult::Fail;
			}
		}
	}

	//Now that we have our Skeleton built
	//Flatten it
	if(skeleton->FlattenSkeleton() != XEResult::Ok)
	{
		return XEResult::Fail;
	}

	if(skeleton->GetFlattenSkeletonSize() != 0)
	{
		//Correct Vertices Bone Indices
		if(CorrectBoneIndices() != XEResult::Ok)
		{
			return XEResult::Fail;
		}
	}

	//We are done building Skeleton
	return XEResult::Ok;
}

XEResult ImporterAssimp::BuildSkeleton(const aiNode* node, TreeBone* childBone)
{
	if(node == nullptr)
	{
		return XEResult::Ok;
	}
	
	std::wstring nodeName = XE_Base::String2WideStr(node->mName.C_Str());

	//This Means there are no child bones
	//No need to explore more...
	if(node->mNodeType != aiNodeType_Bone)
	{
		return XEResult::Ok;
	}

	Bone* currentBone = nullptr;

	if(m_BoneMap.find(nodeName) != m_BoneMap.end())
	{
		currentBone = m_BoneMap[nodeName];
	}
	else 
	{
		//No bone was present with meshes but that does not mean we 
		//can ignore it, we need to add it to our Hierarchy
		currentBone = new Bone();

		//We know we have its transforms
		XEAssert(m_TransformsMap.find(nodeName) != m_TransformsMap.end());
		const TransformsHolder& transHolder = m_TransformsMap[nodeName];

		//Set Data to bone
		currentBone->SetName(nodeName);
		currentBone->SetLocalMatrix(transHolder.m_LocalTransform);
		currentBone->SetWorldMatrix(transHolder.m_WorldTransform);

		//Because bone was not use we do not need to set any Off Set Matrix
		//We Set an Identity Matrix (More just a protocol, to leave the code clear)
		currentBone->SetOffSetMatrix(XEMathHelpers::Mat4Identity);

		//Add to Bone Map
		m_BoneMap[nodeName] = currentBone;
	}

	if(childBone == nullptr)
	{	
		childBone = new TreeBone();		
		childBone->m_Bone = currentBone;

		m_Model->m_SkeletonContent->m_DummyRootBone->m_ChildBones.push_back(childBone);
	}
	else
	{
		TreeBone* newChild = new TreeBone();
		newChild->m_Bone = currentBone;

		childBone->m_ChildBones.push_back(newChild);
		childBone = newChild;
	}
	
	for(uint32_t i = 0; i < node->mNumChildren; ++i)
	{
		BuildSkeleton(node->mChildren[i], childBone);
	}

	return XEResult::Ok;
}

XEResult ImporterAssimp::CorrectBoneIndices()
{
	auto it = m_MeshHolderMap.begin();
	auto itEnd = m_MeshHolderMap.end();

	for (; it != itEnd; ++it)
	{
		MeshHolder* meshHolder = it->second;

		for(uint32_t i = 0; i < (uint32_t)meshHolder->m_MeshPartHolderVec.size(); ++i)
		{
			MeshPartHolder& meshPart = meshHolder->m_MeshPartHolderVec[i];

			for (uint32_t j = 0; j < (uint32_t)meshPart.m_VtxHolderVec.size(); ++j)
			{
				VertexHolder& vtxHolder = meshPart.m_VtxHolderVec[j];

				for(uint32_t x = 0;  x < vtxHolder.m_BlendIndexCount; ++x)
				{
					XEAssert(vtxHolder.m_IndexToBone.find(x) != vtxHolder.m_IndexToBone.end());

					const std::wstring& boneName = vtxHolder.m_IndexToBone[x];

					XEAssert(m_BoneMap.find(boneName) != m_BoneMap.end());

					Bone* bone = m_BoneMap[boneName];

					int32_t boneIndex = bone->GetIndex();
				
					int32_t* boneIndices = (int32_t*)&vtxHolder.m_BlendIndices;

					boneIndices[x] = boneIndex;
				}
			}
		}
	}

	return XEResult::Ok;
}

XEResult ImporterAssimp::ProcessAnimations(const aiScene* scene, const std::vector<std::wstring>& animationNewNames)
{
	if(scene == nullptr)
	{
		return XEResult::NullParameter;
	}

	//Get Animations and their Key Frames
	if(GetAnimationKeyFrames(scene) != XEResult::Ok)
	{
		return XEResult::Fail;
	}	

	//Now Build Animations to Match our Format
	if(BuildAnimations(animationNewNames) != XEResult::Ok)
	{
		return XEResult::Fail;
	}

	return XEResult::Ok;
}

XEResult ImporterAssimp::GetAnimationKeyFrames(const aiScene* scene)
{
	if(scene == nullptr)
	{
		return XEResult::NullParameter;
	}

	for(uint32_t i = 0; i < scene->mNumAnimations; ++i)
	{
		uint32_t j = 0;
		uint32_t x = 0;

		//Get Assimp Animation
		aiAnimation* assimpAnim = scene->mAnimations[i];
		
		//Get Animation Name
		std::wstring animName = XE_Base::String2WideStr(assimpAnim->mName.C_Str());

		//Get Duration of Animation
		float duration = (float)assimpAnim->mDuration;

		//Get Rate of Animation to determine time
		float ticksPerSecond = (float)assimpAnim->mTicksPerSecond;

		//Create Animation Holder
		AnimationHolder animHolder;

		//Add Animation Name
		animHolder.m_Name = animName;

		if(ticksPerSecond == 0.0f)
		{
			std::wstring errMsg = L"";
			fastformat::fmt(errMsg, XELOCMAN->GetLiteral(L"ASSIMP_DEFAULT_FPS_ERR_MSG"), __FUNCTIONW__, XE_IMPORTER_DEFAULT_FPS);
			XELOGGER->AddNewLog(LogLevel::Warning, errMsg);

			ticksPerSecond = XE_IMPORTER_DEFAULT_FPS;
		}

		duration /= ticksPerSecond;

		//Set Animation Duration
		animHolder.m_Duration = duration;

		for(j = 0; j < assimpAnim->mNumChannels; ++j)
		{
			//Get Animation Channel
			aiNodeAnim* assimpAnimChannel = assimpAnim->mChannels[j];
			
			//Get Affect Node Name
			std::wstring nodeName = XE_Base::String2WideStr(assimpAnimChannel->mNodeName.C_Str());

			if(m_BoneMap.find(nodeName) == m_BoneMap.end())
			{
				std::wstring errMsg = L"";
				fastformat::fmt(errMsg, XELOCMAN->GetLiteral(L"ASSIMP_ANIM_NO_BONE_ERR_MSG"), __FUNCTIONW__, nodeName, j, animName);
				XELOGGER->AddNewLog(LogLevel::Warning, errMsg);

				continue;
			}

			//Create KeyFrame Holder
			KeyFrameHolder keyFrameHolder;

			//Set Bone Name to Key Frame
			keyFrameHolder.m_BoneName = nodeName;

			//Get Pre Animation Behavior
			if(assimpAnimChannel->mPreState != aiAnimBehaviour_DEFAULT)
			{
				std::wstring errMsg = L"";
				fastformat::fmt(errMsg, XELOCMAN->GetLiteral(L"ASSIMP_ANIM_PRE_STATE_ERR_MSG"), __FUNCTIONW__, (uint32_t)assimpAnimChannel->mPreState, j, animName, (uint32_t)aiAnimBehaviour_DEFAULT);
				XELOGGER->AddNewLog(LogLevel::Warning, errMsg);
			}

			//Get Post Animation Behavior
			if(assimpAnimChannel->mPostState != aiAnimBehaviour_DEFAULT)
			{
				std::wstring errMsg = L"";
				fastformat::fmt(errMsg, XELOCMAN->GetLiteral(L"ASSIMP_ANIM_POST_STATE_ERR_MSG"), __FUNCTIONW__, (uint32_t)assimpAnimChannel->mPostState, j, animName, (uint32_t)aiAnimBehaviour_DEFAULT);
				XELOGGER->AddNewLog(LogLevel::Warning, errMsg);
			}

			std::map<float, BonePose> bonePoseMap;

			for(x = 0; x < assimpAnimChannel->mNumPositionKeys; ++x)
			{
				aiVectorKey key = assimpAnimChannel->mPositionKeys[x];

				float time = (float)(key.mTime / ticksPerSecond);

				bonePoseMap[time].m_Translation = glm::vec3(key.mValue.x, key.mValue.y, key.mValue.z);
			}
			
			for(x = 0; x < assimpAnimChannel->mNumRotationKeys; ++x)
			{
				aiQuatKey key = assimpAnimChannel->mRotationKeys[x];

				float time = (float)(key.mTime / ticksPerSecond);

				bonePoseMap[time].m_RotationQ = glm::quat(key.mValue.w, key.mValue.x, key.mValue.y, key.mValue.z);
			}
			
			for(x = 0; x < assimpAnimChannel->mNumScalingKeys; ++x)
			{
				aiVectorKey key = assimpAnimChannel->mScalingKeys[x];

				float time = (float)(key.mTime / ticksPerSecond);

				bonePoseMap[time].m_Scale = glm::vec3(key.mValue.x, key.mValue.y, key.mValue.z);
			}

			//Add Key Frame different Bone Poses
			keyFrameHolder.m_BonePoseMap = bonePoseMap;

			//Add Key Frames to Animation Holder
			animHolder.m_KeyFramesVec.push_back(keyFrameHolder);
		}

		//Add Animation to Animations Vector
		m_AnimationHolderVec.push_back(animHolder);
	}

	return XEResult::Ok;
}
		
XEResult ImporterAssimp::BuildAnimations(const std::vector<std::wstring>& animationNewNames)
{
	for(size_t i = 0; i < m_AnimationHolderVec.size(); ++i)
	{
		const AnimationHolder& animHolder = m_AnimationHolderVec[i];

		//Create our Engine Animation Clip
		AnimationContent* animContent = new AnimationContent();

		//Set Animation Name
		animContent->m_Name = animHolder.m_Name;

		//Replace Autoname
		ReplaceAutoGenAssimpName(animContent->m_Name, m_Model->m_FileName, XE_IMPORTER_TYPE_ANIMATION_NAME, static_cast<int32_t>(i), true);

		//Set Duration 
		animContent->m_Duration = animHolder.m_Duration;

		//Iterate to get Animation Clip Key Frames
		for(uint32_t j = 0; j < (uint32_t)animHolder.m_KeyFramesVec.size(); ++j)
		{
			const KeyFrameHolder& keyFrameHolder = animHolder.m_KeyFramesVec[j];

			XEAssert(m_BoneMap.find(keyFrameHolder.m_BoneName) != m_BoneMap.end());

			Bone* bone = m_BoneMap[keyFrameHolder.m_BoneName];

			auto it = keyFrameHolder.m_BonePoseMap.begin();
			auto itEnd = keyFrameHolder.m_BonePoseMap.end();

			for(; it != itEnd; ++it)
			{
				//KeyFrame Holder Map is sorted by time
				//Get it
				float time = it->first;

				//Get Bone Pose
				BonePose bonePose = it->second;

				//We have not calculated the Bone Pose Transformation Matrix
				//do it now
				bonePose.CalculateTransformMatrix();

				//Create our Engine Key Frame
				KeyFrame keyFrame;

				//Fill with data
				keyFrame.SetBoneIndex(bone->GetIndex());
				keyFrame.SetBonePose(bonePose);
				keyFrame.SetTime(time);

				//Add the Key Frame to our list
				//Do not worry about order, the animation clip
				//will order the key frames
				animContent->m_KeyFrames.push_back(keyFrame);
			}
		}

		if(animContent->m_KeyFrames.size() == 0)
		{
			XETODO("Log error");
			
			DeleteMem(animContent);

			continue;
		}

		//If we have a new name available for the Animation Clip, set it
		if((uint32_t)animationNewNames.size() > i)
		{
			animContent->m_Name = animationNewNames[i];
		}

		//Sort Key Frames to hay them ready
		animContent->SortKeyFrames();

		//Add Animation Clip to Model
		if(m_Model->m_Animations.find(animContent->m_Name) != m_Model->m_Animations.end())
		{
			std::wstring errMsg = L"";
			fastformat::fmt(errMsg, XELOCMAN->GetLiteral(L"ASSIMP_ANIM_ADD_ERR_MSG"), __FUNCTIONW__, animContent->m_Name);
			XELOGGER->AddNewLog(LogLevel::Warning, errMsg);

			DeleteMem(animContent);
		}
		else
		{
			m_Model->m_Animations[animContent->m_Name] = animContent;
		}
	}

	return XEResult::Ok;
}

XEResult ImporterAssimp::ProcessMaterials(const aiScene* scene)
{
	if(scene == nullptr)
	{
		return XEResult::NullParameter;
	}
	
	for(uint32_t i = 0; i < scene->mNumMaterials; ++i)
	{
		uint32_t textCount = 0;

		//Get Assimp Material
		aiMaterial* assimpMat = scene->mMaterials[i];
		
		//Create a Holder for the Materials
		MaterialHolder matHolder;

		//Assign Material ID
		matHolder.m_MaterialIndex = i;
		
		////////////////////////////////////////////
		//Start Extracting Texture Paths 
		////////////////////////////////////////////

		textCount = assimpMat->GetTextureCount(aiTextureType_DIFFUSE);
		if(textCount != 0)
		{
			matHolder.m_DiffuseTextureString = GetTexturePath(assimpMat, 0, aiTextureType_DIFFUSE);

			if(textCount > 1)
			{
				std::wstring errMsg = L"";
				fastformat::fmt(errMsg, XELOCMAN->GetLiteral(L"ASSIMP_TEXTURE_TYPE_NUM_ERR_MSG"), __FUNCTIONW__, L"DIFFUSE", textCount, 1);
				XELOGGER->AddNewLog(LogLevel::Warning, errMsg);
			}
		}

		textCount = assimpMat->GetTextureCount(aiTextureType_NORMALS);
		if(textCount != 0)
		{
			matHolder.m_NormalTextureString = GetTexturePath(assimpMat, 0, aiTextureType_NORMALS);

			if(textCount > 1)
			{
				std::wstring errMsg = L"";
				fastformat::fmt(errMsg, XELOCMAN->GetLiteral(L"ASSIMP_TEXTURE_TYPE_NUM_ERR_MSG"), __FUNCTIONW__, L"NORMAL", textCount, 1);
				XELOGGER->AddNewLog(LogLevel::Warning, errMsg);
			}
		}

		textCount = assimpMat->GetTextureCount(aiTextureType_SPECULAR);
		if(textCount != 0)
		{
			matHolder.m_SpecularTextureString = GetTexturePath(assimpMat, 0, aiTextureType_SPECULAR);

			if(textCount > 1)
			{
				std::wstring errMsg = L"";
				fastformat::fmt(errMsg, XELOCMAN->GetLiteral(L"ASSIMP_TEXTURE_TYPE_NUM_ERR_MSG"), __FUNCTIONW__, L"SPECULAR", textCount, 1);
				XELOGGER->AddNewLog(LogLevel::Warning, errMsg);
			}
		}

		textCount = assimpMat->GetTextureCount(aiTextureType_DISPLACEMENT);
		if(textCount != 0)
		{
			matHolder.m_DisplacementTextureString = GetTexturePath(assimpMat, 0, aiTextureType_DISPLACEMENT);

			if(textCount > 1)
			{
				std::wstring errMsg = L"";
				fastformat::fmt(errMsg, XELOCMAN->GetLiteral(L"ASSIMP_TEXTURE_TYPE_NUM_ERR_MSG"), __FUNCTIONW__, L"DISPLACEMENT", textCount, 1);
				XELOGGER->AddNewLog(LogLevel::Warning, errMsg);
			}
		}

		textCount = assimpMat->GetTextureCount(aiTextureType_LIGHTMAP);
		if(textCount != 0)
		{
			matHolder.m_LightMapTextureString = GetTexturePath(assimpMat, 0, aiTextureType_LIGHTMAP);

			if(textCount > 1)
			{
				std::wstring errMsg = L"";
				fastformat::fmt(errMsg, XELOCMAN->GetLiteral(L"ASSIMP_TEXTURE_TYPE_NUM_ERR_MSG"), __FUNCTIONW__, L"LIGHTMAP", textCount, 1);
				XELOGGER->AddNewLog(LogLevel::Warning, errMsg);
			}
		}
		
		////////////////////////////////////////////
		//The Following Textures are not supported
		//add a warning that these textures 
		//will be skip
		////////////////////////////////////////////

		textCount = assimpMat->GetTextureCount(aiTextureType_AMBIENT);
		if(textCount != 0)
		{
			std::wstring errMsg = L"";
			fastformat::fmt(errMsg, XELOCMAN->GetLiteral(L"ASSIMP_TEXTURE_TYPE_NUM_ERR_MSG"), __FUNCTIONW__, L"AMBIENT", textCount, 0);
			XELOGGER->AddNewLog(LogLevel::Warning, errMsg);
		}

		textCount = assimpMat->GetTextureCount(aiTextureType_EMISSIVE);
		if(textCount != 0)
		{
			std::wstring errMsg = L"";
			fastformat::fmt(errMsg, XELOCMAN->GetLiteral(L"ASSIMP_TEXTURE_TYPE_NUM_ERR_MSG"), __FUNCTIONW__, L"EMISSIVE", textCount, 0);
			XELOGGER->AddNewLog(LogLevel::Warning, errMsg);
		}

		textCount = assimpMat->GetTextureCount(aiTextureType_HEIGHT);
		if(textCount != 0)
		{
			std::wstring errMsg = L"";
			fastformat::fmt(errMsg, XELOCMAN->GetLiteral(L"ASSIMP_TEXTURE_TYPE_NUM_ERR_MSG"), __FUNCTIONW__, L"HEIGHT", textCount, 0);
			XELOGGER->AddNewLog(LogLevel::Warning, errMsg);
		}

		textCount = assimpMat->GetTextureCount(aiTextureType_SHININESS);
		if(textCount != 0)
		{
			std::wstring errMsg = L"";
			fastformat::fmt(errMsg, XELOCMAN->GetLiteral(L"ASSIMP_TEXTURE_TYPE_NUM_ERR_MSG"), __FUNCTIONW__, L"SHININESS", textCount, 0);
			XELOGGER->AddNewLog(LogLevel::Warning, errMsg);
		}

		textCount = assimpMat->GetTextureCount(aiTextureType_OPACITY);
		if(textCount != 0)
		{
			std::wstring errMsg = L"";
			fastformat::fmt(errMsg, XELOCMAN->GetLiteral(L"ASSIMP_TEXTURE_TYPE_NUM_ERR_MSG"), __FUNCTIONW__, L"OPACITY", textCount, 0);
			XELOGGER->AddNewLog(LogLevel::Warning, errMsg);
		}

		textCount = assimpMat->GetTextureCount(aiTextureType_REFLECTION);
		if(textCount != 0)
		{
			std::wstring errMsg = L"";
			fastformat::fmt(errMsg, XELOCMAN->GetLiteral(L"ASSIMP_TEXTURE_TYPE_NUM_ERR_MSG"), __FUNCTIONW__, L"REFLECTION", textCount, 0);
			XELOGGER->AddNewLog(LogLevel::Warning, errMsg);
		}

		textCount = assimpMat->GetTextureCount(aiTextureType_UNKNOWN);
		if(textCount != 0)
		{
			std::wstring errMsg = L"";
			fastformat::fmt(errMsg, XELOCMAN->GetLiteral(L"ASSIMP_TEXTURE_TYPE_NUM_ERR_MSG"), __FUNCTIONW__, L"UNKNOWN", textCount, 0);
			XELOGGER->AddNewLog(LogLevel::Warning, errMsg);
		}

		//No two materials can have the same ID
		//Do a Debug Check
		XEAssert(m_MaterialHolderMap.find(i) == m_MaterialHolderMap.end());

		//Add Material Holder to Map
		m_MaterialHolderMap[i] = matHolder;
	}

	//Now Build the Materials so we can add them to the model
	if(BuildMaterials() != XEResult::Ok)
	{
		return XEResult::Fail;
	}

	return XEResult::Ok;
}

std::wstring ImporterAssimp::GetTexturePath(aiMaterial* assimpMat, uint32_t textureID, aiTextureType textType)
{
	if(assimpMat == nullptr)
	{
		return L"";
	}
	
	aiString path("");

	assimpMat->GetTexture(textType, textureID, &path);

	std::wstring filePath = XE_Base::String2WideStr(path.C_Str());

	//Extract only the File Name as we will load the texture
	//from a path the user provides to us
	std::wstring fileName = XE_Base::GetFilename(filePath);

	return fileName;
}

XEResult ImporterAssimp::BuildMaterials()
{
	auto it = m_MaterialHolderMap.begin();
	auto itEnd = m_MaterialHolderMap.end();

	for(; it != itEnd; ++it)
	{
		const MaterialHolder& matHolder = it->second;

		m_Model->m_Materials.push_back(matHolder);
	}

	return XEResult::Ok;
}

XEResult ImporterAssimp::ProcessEmbeddedTextures(const aiScene* scene)
{
	if(scene == nullptr)
	{
		return XEResult::NullParameter;
	}

	if(scene->HasTextures())
	{
		std::wstring errMsg = L"";
		fastformat::fmt(errMsg, XELOCMAN->GetLiteral(L"ASSIMP_EMBEDDED_TEXTURES_ERR_MSG"), __FUNCTIONW__);
		XELOGGER->AddNewLog(LogLevel::Error, errMsg);
	}

	return XEResult::Ok;
}

XEResult ImporterAssimp::ConstructModel()
{
	////////////////////////////////////
	//Build the Meshes
	////////////////////////////////////

	for (auto meshHolderPair : m_MeshHolderMap)
	{
		//Get Mesh Holder
		MeshHolder* meshHolder = meshHolderPair.second;

		//Create our Engine Mesh
		MeshContent* mesh = new MeshContent();

		mesh->m_Name = meshHolder->m_Name;

		//Add our mesh to the Model
		if(m_Model->m_Meshes.find(mesh->m_Name) != m_Model->m_Meshes.end())
		{
			std::wstring errMsg = L"";
			fastformat::fmt(errMsg, XELOCMAN->GetLiteral(L"ASSIMP_MESH_ADD_ERR_MSG"), __FUNCTIONW__, mesh->m_Name);
			XELOGGER->AddNewLog(LogLevel::Error, errMsg);
			
			DeleteMem(mesh);

			return XEResult::Fail;
		}
		else
		{
			m_Model->m_Meshes[mesh->m_Name] = mesh;
		}

		for(uint32_t i = 0; i < (uint32_t)meshHolder->m_MeshPartHolderVec.size(); ++i)
		{
			const MeshPartHolder& meshPartHolder = meshHolder->m_MeshPartHolderVec[i];

			MeshPartContent* meshPart = nullptr;
			
			XETODO("Change this in loc");
			//ASSIMP_IDX_BUFFER_ERR_MSG
			//ASSIMP_VTX_BUFFER_ERR_MSG
			
			if(XEImporterHelpers::ConvertToMeshContent(meshPartHolder, &meshPart) != XEResult::Ok)
			{
				std::wstring errMsg = L"";
				fastformat::fmt(errMsg, XELOCMAN->GetLiteral(L"ASSIMP_MESH_PART_ADD_ERR_MSG"), __FUNCTIONW__, i, mesh->m_Name);
				XELOGGER->AddNewLog(LogLevel::Error, errMsg);

				return XEResult::Fail;
			}

			mesh->m_MeshParts.push_back(meshPart);
		}
	}

	return XEResult::Ok;
}

XEResult ImporterAssimp::CreateBoudingShapes()
{
	////////////////////////////////////
	//Build the Bounding Shapes
	////////////////////////////////////

	for (auto meshHolderPair : m_MeshHolderMap)
	{
		//Get Mesh Holder
		MeshHolder* meshHolder = meshHolderPair.second;

		//Add our mesh to the Model
		if (m_Model->m_Meshes.find(meshHolder->m_Name) == m_Model->m_Meshes.end())
		{
			XETODO("Add err message");
			//std::wstring errMsg = L"";
			//fastformat::fmt(errMsg, XELOCMAN->GetLiteral(L"ASSIMP_MESH_ADD_ERR_MSG"), __FUNCTIONW__, mesh->m_Name);
			//XELOGGER->AddNewLog(LogLevel::Error, errMsg);

			return XEResult::Fail;
		}

		MeshContent* mesh = m_Model->m_Meshes[meshHolder->m_Name];

		for (uint32_t i = 0; i < (uint32_t)meshHolder->m_MeshPartHolderVec.size(); ++i)
		{
			const MeshPartHolder& meshPartHolder = meshHolder->m_MeshPartHolderVec[i];

			for (uint32_t j = 0; j < (uint32_t)meshPartHolder.m_VtxHolderVec.size(); ++j)
			{
				const VertexHolder& vertexHolder = meshPartHolder.m_VtxHolderVec[i];

				mesh->m_BoundingBox.PushPositionPoint(vertexHolder.m_Position);
				mesh->m_BoundingSphere.PushPositionPoint(vertexHolder.m_Position);
			}
		}
	}

	return XEResult::Ok;
}

void ImporterAssimp::ReplaceAutoGenAssimpName(std::wstring& name, const std::wstring& newName, const std::wstring& typeName, int32_t idx, bool overrideNotAuto)
{
	XETODO("Add list so it will also check against others not only collada");

	if (!overrideNotAuto)
	{
		if (name.compare(0, wcslen(XE_IMPORTER_ASSIMP_COLLADA_AUTO_NAME), XE_IMPORTER_ASSIMP_COLLADA_AUTO_NAME) != 0)
		{
			return;
		}
	}

	name.clear();

	if (idx >= 0)
	{
		fastformat::fmt(name, L"{0} {1} {2}", newName, typeName, idx);
	}
	else
	{
		fastformat::fmt(name, L"{0} {1}", newName, typeName);
	}
}