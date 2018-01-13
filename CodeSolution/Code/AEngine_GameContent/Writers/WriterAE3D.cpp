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
#include "boost\filesystem.hpp"

/***************************
*   Game Engine Includes   *
****************************/
#include "WriterAE3D.h"
#include "GameContentDefs.h"
#include "Vertex\VertexDefs.h"
#include "Base\BaseFunctions.h"
#include "Content\MeshContent.h"
#include "Content\ModelContent.h"
#include "Models\Skinning\Bone.h"
#include "Content\SkeletonContent.h"
#include "Content\MeshPartContent.h"
#include "Content\AnimationContent.h"
#include "Models\Animation\KeyFrame.h"
#include "Content\VertexBufferContent.h"

//Always include last
#include "Memory\MemLeaks.h"

/************
*   Using   *
*************/

/********************
*   Function Defs   *
*********************/
AETODO("Need to change materials to create material and associate it with shader, that is when texture is read from the model, we will not associated with it, but create a Material (Shader + Texture Prop) and add it to a material folder");
WriterAE3D::WriterAE3D()
{
}

WriterAE3D::~WriterAE3D()
{
}

AEResult WriterAE3D::WriteToFile(ModelContent* content, const std::wstring& fileName, const std::wstring& filePath)
{
	m_FileName = L"";
	m_FullFileName = L"";

	AEAssert(content != nullptr);

	if(content == nullptr)
	{
		return AEResult::NullObj;
	}

	AEAssert(!fileName.empty());

	if(fileName.empty())
	{
		return AEResult::EmptyFilename;
	}

	AEAssert(!filePath.empty());

	if(filePath.empty())
	{
		return AEResult::EmptyString;
	}
	
	m_FilePath = filePath;
		
	if(m_FilePath.back() != L'\\' && m_FilePath.back() != L'/')
	{
		m_FilePath.push_back(L'\\');
	}

	//Set File Path and File Name
	m_FileName = fileName + L"." + AE_CT_MODEL_EXT;	
	m_FullFileName = m_FilePath + m_FileName;

	std::ofstream modelFile;

	modelFile.open(m_FullFileName, std::ios::binary | std::ios::out | std::ios::trunc);

	if(!modelFile.is_open())
	{
		AETODO("add log");
		return AEResult::OpenFileFail;
	}

	const char* tempPtr = nullptr;
	uint32_t tempUInt32 = 0;
	uint32_t sizeToWrite = 0;
	
	/////////////////////////////////////////////
	//Start by writing the header and File Version
	AEGameContentHelpers::WriteFileHeader(modelFile, AE_CT_AE3D_FILE_HEADER, AE_CT_AE3D_FILE_VERSION_MAYOR, AE_CT_AE3D_FILE_VERSION_MINOR, AE_CT_AE3D_FILE_VERSION_REVISON);
	
	/////////////////////////////////////////////
	//Write Name of Model and size of Name
	AEGameContentHelpers::WriteString(modelFile, content->m_Name);
	
	/////////////////////////////////////////////
	//Write Number and name of mesh files
	tempUInt32 = (uint32_t)content->m_Meshes.size();
	tempPtr = reinterpret_cast<const char*>(&tempUInt32);
	sizeToWrite = sizeof(uint32_t);
	modelFile.write(tempPtr, sizeToWrite);

	auto itMesh = content->m_Meshes.begin();
	auto itMeshEnd = content->m_Meshes.end();

	for(; itMesh != itMeshEnd; ++itMesh)
	{
		AETODO("change dir were to save textures");
		MeshContent* mesh = itMesh->second;
		
		/////////////////////////////////////////////
		//Write name of Mesh Name
		AEGameContentHelpers::WriteString(modelFile, mesh->m_Name);
		
		std::wstring meshFilePath = fileName + L"_" + content->m_Name + L"_" + mesh->m_Name + L"." + AE_CT_MESH_EXT;
		meshFilePath = m_FilePath + meshFilePath;
		AEGameContentHelpers::WriteString(modelFile, meshFilePath);
		
		//Write Mesh to file
		AEResult ret = WriteMesh(meshFilePath, mesh);
		if(ret != AEResult::Ok)
		{
			AETODO("add log");
			return ret;
		}
	}
	
	/////////////////////////////////////////////
	//Write if Skeleton is present
	bool skeletonExists = content->m_SkeletonContent->GetFlattenSkeletonSize() != 0;
	tempPtr = reinterpret_cast<const char*>(&skeletonExists);
	sizeToWrite = sizeof(bool);
	modelFile.write(tempPtr, sizeToWrite);
	
	if(skeletonExists)
	{
		/////////////////////////////////////////////
		//Write Name of Skeleton
		std::wstring skeletonName = content->m_Name + L" Skeleton";
		AEGameContentHelpers::WriteString(modelFile, skeletonName);

		/////////////////////////////////////////////
		//Write Skeleton File Name and size of Name
		std::wstring skeletonFile = fileName + L"_" + content->m_Name + L"_Skeleton" + L"." + AE_CT_SKELETON_EXT;
		skeletonFile = m_FilePath + skeletonFile;
		AEGameContentHelpers::WriteString(modelFile, skeletonFile);
		
		//Write Skeleton to file
		AEResult ret = WriteSkeleton(skeletonFile, content->m_SkeletonContent);
		if(ret != AEResult::Ok)
		{
			AETODO("add log");
			return ret;
		}
	}
	
	/////////////////////////////////////////////
	//Write Number and name of Animation files
	tempUInt32 = (uint32_t)content->m_Animations.size();
	tempPtr = reinterpret_cast<const char*>(&tempUInt32);
	sizeToWrite = sizeof(uint32_t);
	modelFile.write(tempPtr, sizeToWrite);

	auto itAnim = content->m_Animations.begin();
	auto itAnimEnd = content->m_Animations.end();
	
	for(; itAnim != itAnimEnd; ++itAnim)
	{
		AnimationContent* anim = itAnim->second;

		/////////////////////////////////////////////
		//Write name Animation
		AEGameContentHelpers::WriteString(modelFile, anim->m_Name);
		
		/////////////////////////////////////////////
		//Write name of Animation File and size of name
		std::wstring animFilePath = fileName + L"_" + content->m_Name + L"_" + anim->m_Name + L"." + AE_CT_ANIMATION_EXT;
		animFilePath = m_FilePath + animFilePath;
		AEGameContentHelpers::WriteString(modelFile, animFilePath);

		//Write Animation to file
		AEResult ret = WriteAnimation(animFilePath, anim);
		if(ret != AEResult::Ok)
		{
			AETODO("add log");
			return ret;
		}
	}
	
	/////////////////////////////////////////////
	//Finish by writing the footer
	AEGameContentHelpers::WriteFileFooter(modelFile, AE_CT_AE3D_FILE_FOOTER);
	
	/////////////////////////////////////////////
	//Finish 
	modelFile.flush();
	modelFile.close();

	return AEResult::Ok;
}

AEResult WriterAE3D::WriteMesh(const std::wstring& fileName, MeshContent* mesh)
{
	AEAssert(mesh != nullptr);

	if(mesh == nullptr)
	{
		return AEResult::NullObj;
	}

	AEAssert(!fileName.empty());

	if(fileName.empty())
	{
		return AEResult::EmptyFilename;
	}

	std::ofstream meshFile;

	meshFile.open(fileName, std::ios::binary | std::ios::out | std::ios::trunc);

	if(!meshFile.is_open())
	{
		AETODO("add log");
		return AEResult::OpenFileFail;
	}

	const char* tempPtr = nullptr;
	uint32_t tempUInt32 = 0;
	uint32_t sizeToWrite = 0;
	
	/////////////////////////////////////////////
	//Start by writing the header and File Version
	AEGameContentHelpers::WriteFileHeader(meshFile, AE_CT_AE3DM_FILE_HEADER, AE_CT_AE3DM_FILE_VERSION_MAYOR, AE_CT_AE3DM_FILE_VERSION_MINOR, AE_CT_AE3DM_FILE_VERSION_REVISON);

	/////////////////////////////////////////////
	//Write Name of Mesh and Size of Name
	AEGameContentHelpers::WriteString(meshFile, mesh->m_Name);

	/////////////////////////////////////////////
	//Write Number of Mesh Parts
	tempUInt32 = (uint32_t)mesh->m_MeshParts.size();
	tempPtr = reinterpret_cast<const char*>(&tempUInt32);
	sizeToWrite = sizeof(uint32_t);
	meshFile.write(tempPtr, sizeToWrite);
	
	/////////////////////////////////////////////
	//Write Mesh Parts to File
	uint32_t size = (uint32_t)mesh->m_MeshParts.size();
	for (uint32_t i = 0; i < size; ++i)
	{
		MeshPartContent* meshPart = mesh->m_MeshParts[i];

		/////////////////////////////////////////////
		//Write Index Type
		AETODO("To change to 16 or 32");
		tempUInt32 = (uint32_t)IndexBufferType::Index16;
		tempPtr = reinterpret_cast<const char*>(&tempUInt32);
		sizeToWrite = sizeof(uint32_t);
		meshFile.write(tempPtr, sizeToWrite);
		
		/////////////////////////////////////////////
		//Write size of Index Buffer
		tempPtr = reinterpret_cast<const char*>(&meshPart->m_IdxSize);
		sizeToWrite = sizeof(uint32_t);
		meshFile.write(tempPtr, sizeToWrite);
		
		/////////////////////////////////////////////
		//Write Index Buffer
		tempPtr = reinterpret_cast<const char*>(meshPart->m_IdxBuf16);
		sizeToWrite = sizeof(uint16_t) * meshPart->m_IdxSize;
		meshFile.write(tempPtr, sizeToWrite);
		
		/////////////////////////////////////////////
		//Write Vertex Buffer Type
		tempUInt32 = (uint32_t)meshPart->m_VertexType;
		tempPtr = reinterpret_cast<const char*>(&tempUInt32);
		sizeToWrite = sizeof(uint32_t);
		meshFile.write(tempPtr, sizeToWrite);
		
		/////////////////////////////////////////////
		//Write size of Vertex Buffer
		tempPtr = reinterpret_cast<const char*>(&meshPart->m_VtxBuff->m_Size);
		sizeToWrite = sizeof(uint32_t);
		meshFile.write(tempPtr, sizeToWrite);
		
		/////////////////////////////////////////////
		//Write Vertex Buffer
		tempPtr = reinterpret_cast<const char*>(meshPart->m_VtxBuff->GetBuffer());
		sizeToWrite = meshPart->m_VtxBuff->GetSizeOfVertex() * meshPart->m_VtxBuff->m_Size;
		meshFile.write(tempPtr, sizeToWrite);
	}

	/////////////////////////////////////////////
	//Write Bouding Shapes
	tempPtr = reinterpret_cast<const char*>(&mesh->m_BoundingBox.m_MinPos);
	sizeToWrite = sizeof(glm::vec3);
	meshFile.write(tempPtr, sizeToWrite);

	tempPtr = reinterpret_cast<const char*>(&mesh->m_BoundingBox.m_MaxPos);
	sizeToWrite = sizeof(glm::vec3);
	meshFile.write(tempPtr, sizeToWrite);

	tempPtr = reinterpret_cast<const char*>(&mesh->m_BoundingSphere.m_Radius);
	sizeToWrite = sizeof(float);
	meshFile.write(tempPtr, sizeToWrite);

	/////////////////////////////////////////////
	//Finish by writing the footer
	AEGameContentHelpers::WriteFileFooter(meshFile, AE_CT_AE3DM_FILE_FOOTER);

	/////////////////////////////////////////////
	//Finish 
	meshFile.flush();
	meshFile.close();

	return AEResult::Ok;
}

AEResult WriterAE3D::WriteSkeleton(const std::wstring& fileName, SkeletonContent* skeleton)
{
	AEAssert(skeleton != nullptr);

	if(skeleton == nullptr)
	{
		return AEResult::NullObj;
	}

	AEAssert(!fileName.empty());

	if(fileName.empty())
	{
		return AEResult::EmptyFilename;
	}

	std::ofstream skeletonFile;

	skeletonFile.open(fileName, std::ios::binary | std::ios::out | std::ios::trunc);

	if(!skeletonFile.is_open())
	{
		AETODO("add log");
		return AEResult::OpenFileFail;
	}

	const char* tempPtr = nullptr;
	uint32_t tempUInt32 = 0;
	int32_t tempInt32 = 0;
	uint32_t sizeToWrite = 0;
	
	/////////////////////////////////////////////
	//Start by writing the header and File Version
	AEGameContentHelpers::WriteFileHeader(skeletonFile, AE_CT_AE3DS_FILE_HEADER, AE_CT_AE3DS_FILE_VERSION_MAYOR, AE_CT_AE3DS_FILE_VERSION_MINOR, AE_CT_AE3DS_FILE_VERSION_REVISON);
	
	//Get Number of Bones
	uint32_t size = (uint32_t)skeleton->m_BoneHierarchy.size();

	/////////////////////////////////////////////
	//Write Number of Bones
	tempPtr = reinterpret_cast<const char*>(&size);
	sizeToWrite = sizeof(uint32_t);
	skeletonFile.write(tempPtr, sizeToWrite);
	
	/////////////////////////////////////////////
	//Write Bones
	for(uint32_t i = 0; i < size; ++i)
	{
		Bone* bone = skeleton->m_BoneHierarchy[i];

		/////////////////////////////////////////////
		//Write name of Bone and size of Name
		AEGameContentHelpers::WriteString(skeletonFile, bone->GetName());

		/////////////////////////////////////////////
		//Write Bone Matrices
		sizeToWrite = sizeof(glm::mat4);

		//Local Matrix
		tempPtr = reinterpret_cast<const char*>(&bone->GetLocalMatrix());
		skeletonFile.write(tempPtr, sizeToWrite);

		//World Matrix
		tempPtr = reinterpret_cast<const char*>(&bone->GetWorldMatrix());
		skeletonFile.write(tempPtr, sizeToWrite);
		 
		//Off Set Matrix
		tempPtr = reinterpret_cast<const char*>(&bone->GetOffSetMatrix());
		skeletonFile.write(tempPtr, sizeToWrite);
		
		/////////////////////////////////////////////
		//Write Bone Indices

		//Parent Index
		tempInt32 = bone->GetParentIndex();
		tempPtr = reinterpret_cast<const char*>(&tempInt32);
		sizeToWrite = sizeof(int32_t);
		skeletonFile.write(tempPtr, sizeToWrite);

		//Bone Index
		tempInt32 = bone->GetIndex();
		tempPtr = reinterpret_cast<const char*>(&tempInt32);
		sizeToWrite = sizeof(int32_t);
		skeletonFile.write(tempPtr, sizeToWrite);
	}
	
	/////////////////////////////////////////////
	//Finish by writing the footer
	AEGameContentHelpers::WriteFileFooter(skeletonFile, AE_CT_AE3DS_FILE_FOOTER);
	
	/////////////////////////////////////////////
	//Finish 
	skeletonFile.flush();
	skeletonFile.close();

	return AEResult::Ok;
}

AEResult WriterAE3D::WriteAnimation(const std::wstring& fileName, AnimationContent* animation)
{
	AEAssert(animation != nullptr);
	if(animation == nullptr)
	{
		return AEResult::NullObj;
	}

	AEAssert(!fileName.empty());
	if(fileName.empty())
	{
		return AEResult::EmptyFilename;
	}

	std::ofstream animationFile;

	animationFile.open(fileName, std::ios::binary | std::ios::out | std::ios::trunc);

	if(!animationFile.is_open())
	{
		AETODO("add log");
		return AEResult::OpenFileFail;
	}

	const char* tempPtr = nullptr;
	uint32_t sizeToWrite = 0;
	
	/////////////////////////////////////////////
	//Start by writing the header and File Version
	AEGameContentHelpers::WriteFileHeader(animationFile, AE_CT_AE3DA_FILE_HEADER, AE_CT_AE3DA_FILE_VERSION_MAYOR, AE_CT_AE3DA_FILE_VERSION_MINOR, AE_CT_AE3DA_FILE_VERSION_REVISON);

	/////////////////////////////////////////////
	//Write Name of Animation and Size of Name
	AEGameContentHelpers::WriteString(animationFile, animation->m_Name);
	
	/////////////////////////////////////////////
	//Write Duration of Animation
	tempPtr = reinterpret_cast<const char*>(&animation->m_Duration);
	sizeToWrite = sizeof(float);
	animationFile.write(tempPtr, sizeToWrite);
	
	/////////////////////////////////////////////
	//Write Number of Key Frames
	uint32_t sizeKeyFrames = (uint32_t)animation->m_KeyFrames.size();
	tempPtr = reinterpret_cast<const char*>(&sizeKeyFrames);
	sizeToWrite = sizeof(uint32_t);
	animationFile.write(tempPtr, sizeToWrite);
	
	for (uint32_t i = 0; i < sizeKeyFrames; ++i)
	{
		const KeyFrame& keyFrame = animation->m_KeyFrames[i];
		
		/////////////////////////////////////////////
		//Write Frame Time and Bone Index

		//Time
		float time = keyFrame.GetTime();
		tempPtr = reinterpret_cast<const char*>(&time);
		sizeToWrite = sizeof(float);
		animationFile.write(tempPtr, sizeToWrite);

		//Bone Index
		int32_t boneIndex = keyFrame.GetBoneIndex();
		tempPtr = reinterpret_cast<const char*>(&boneIndex);
		sizeToWrite = sizeof(int32_t);
		animationFile.write(tempPtr, sizeToWrite);
		
		/////////////////////////////////////////////
		//Write Bone Pose
		const BonePose& bonePose = keyFrame.GetBonePose();

		//Rotation Quaternion
		tempPtr = reinterpret_cast<const char*>(&bonePose.m_RotationQ);
		sizeToWrite = sizeof(glm::quat);
		animationFile.write(tempPtr, sizeToWrite);

		//Scale
		tempPtr = reinterpret_cast<const char*>(&bonePose.m_Scale);
		sizeToWrite = sizeof(glm::vec3);
		animationFile.write(tempPtr, sizeToWrite);

		//Translation
		tempPtr = reinterpret_cast<const char*>(&bonePose.m_Translation);
		sizeToWrite = sizeof(glm::vec3);
		animationFile.write(tempPtr, sizeToWrite);
	}
	
	/////////////////////////////////////////////
	//Finish by writing the footer
	AEGameContentHelpers::WriteFileFooter(animationFile, AE_CT_AE3DA_FILE_FOOTER);
	
	/////////////////////////////////////////////
	//Finish 
	animationFile.flush();
	animationFile.close();

	return AEResult::Ok;
}
