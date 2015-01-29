/********************************************************
*
* Author: Carlos Chacón N.
*
* Created: 2/1/2013
*
* Desc:
*
*
* Copyright 2013
*
*********************************************************/

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
#include "WriterXE3D.h"
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
XETODO("Need to change materials to create material and associate it with shader, that is when texture is read from the model, we will not associated with it, but create a Material (Shader + Texture Prop) and add it to a material folder");
WriterXE3D::WriterXE3D()
{
}

WriterXE3D::~WriterXE3D()
{
}

XEResult WriterXE3D::WriteToFile(ModelContent* content, const std::wstring& fileName, const std::wstring& filePath)
{
	m_FileName = L"";
	m_FullFileName = L"";

	XEAssert(content != nullptr);

	if(content == nullptr)
	{
		return XEResult::NullObj;
	}

	XEAssert(!fileName.empty());

	if(fileName.empty())
	{
		return XEResult::EmptyFilename;
	}

	XEAssert(!filePath.empty());

	if(filePath.empty())
	{
		return XEResult::EmptyString;
	}
	
	m_FilePath = filePath;
		
	if(m_FilePath.back() != L'\\' && m_FilePath.back() != L'/')
	{
		m_FilePath.push_back(L'\\');
	}

	//Set File Path and File Name
	m_FileName = fileName + L"." + XE_CT_MODEL_EXT;	
	m_FullFileName = m_FilePath + m_FileName;

	std::ofstream modelFile;

	modelFile.open(m_FullFileName, std::ios::binary | std::ios::out | std::ios::trunc);

	if(!modelFile.is_open())
	{
		XETODO("add log");
		return XEResult::OpenFileFail;
	}

	const char* tempPtr = nullptr;
	uint32_t tempUInt32 = 0;
	uint32_t sizeToWrite = 0;
	
	/////////////////////////////////////////////
	//Start by writing the header and File Version
	XEGameContentHelpers::WriteFileHeader(modelFile, XE_CT_XE3D_FILE_HEADER, XE_CT_XE3D_FILE_VERSION_MAYOR, XE_CT_XE3D_FILE_VERSION_MINOR, XE_CT_XE3D_FILE_VERSION_REVISON);
	
	/////////////////////////////////////////////
	//Write Name of Model and size of Name
	XEGameContentHelpers::WriteString(modelFile, content->m_Name);
	
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
		XETODO("change dir were to save textures");
		MeshContent* mesh = itMesh->second;
		
		/////////////////////////////////////////////
		//Write name of Mesh Name
		XEGameContentHelpers::WriteString(modelFile, mesh->m_Name);
		
		std::wstring meshFilePath = fileName + L"_" + content->m_Name + L"_" + mesh->m_Name + L"." + XE_CT_MESH_EXT;
		meshFilePath = m_FilePath + meshFilePath;
		XEGameContentHelpers::WriteString(modelFile, meshFilePath);
		
		//Write Mesh to file
		XEResult ret = WriteMesh(meshFilePath, mesh);
		if(ret != XEResult::Ok)
		{
			XETODO("add log");
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
		XEGameContentHelpers::WriteString(modelFile, skeletonName);

		/////////////////////////////////////////////
		//Write Skeleton File Name and size of Name
		std::wstring skeletonFile = fileName + L"_" + content->m_Name + L"_Skeleton" + L"." + XE_CT_SKELETON_EXT;
		skeletonFile = m_FilePath + skeletonFile;
		XEGameContentHelpers::WriteString(modelFile, skeletonFile);
		
		//Write Skeleton to file
		XEResult ret = WriteSkeleton(skeletonFile, content->m_SkeletonContent);
		if(ret != XEResult::Ok)
		{
			XETODO("add log");
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
		XEGameContentHelpers::WriteString(modelFile, anim->m_Name);
		
		/////////////////////////////////////////////
		//Write name of Animation File and size of name
		std::wstring animFilePath = fileName + L"_" + content->m_Name + L"_" + anim->m_Name + L"." + XE_CT_ANIMATION_EXT;
		animFilePath = m_FilePath + animFilePath;
		XEGameContentHelpers::WriteString(modelFile, animFilePath);

		//Write Animation to file
		XEResult ret = WriteAnimation(animFilePath, anim);
		if(ret != XEResult::Ok)
		{
			XETODO("add log");
			return ret;
		}
	}
	
	/////////////////////////////////////////////
	//Finish by writing the footer
	XEGameContentHelpers::WriteFileFooter(modelFile, XE_CT_XE3D_FILE_FOOTER);
	
	/////////////////////////////////////////////
	//Finish 
	modelFile.flush();
	modelFile.close();

	return XEResult::Ok;
}

XEResult WriterXE3D::WriteMesh(const std::wstring& fileName, MeshContent* mesh)
{
	XEAssert(mesh != nullptr);

	if(mesh == nullptr)
	{
		return XEResult::NullObj;
	}

	XEAssert(!fileName.empty());

	if(fileName.empty())
	{
		return XEResult::EmptyFilename;
	}

	std::ofstream meshFile;

	meshFile.open(fileName, std::ios::binary | std::ios::out | std::ios::trunc);

	if(!meshFile.is_open())
	{
		XETODO("add log");
		return XEResult::OpenFileFail;
	}

	const char* tempPtr = nullptr;
	uint32_t tempUInt32 = 0;
	uint32_t sizeToWrite = 0;
	
	/////////////////////////////////////////////
	//Start by writing the header and File Version
	XEGameContentHelpers::WriteFileHeader(meshFile, XE_CT_XE3DM_FILE_HEADER, XE_CT_XE3DM_FILE_VERSION_MAYOR, XE_CT_XE3DM_FILE_VERSION_MINOR, XE_CT_XE3DM_FILE_VERSION_REVISON);

	/////////////////////////////////////////////
	//Write Name of Mesh and Size of Name
	XEGameContentHelpers::WriteString(meshFile, mesh->m_Name);

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
		XETODO("To change to 16 or 32");
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
	XEGameContentHelpers::WriteFileFooter(meshFile, XE_CT_XE3DM_FILE_FOOTER);

	/////////////////////////////////////////////
	//Finish 
	meshFile.flush();
	meshFile.close();

	return XEResult::Ok;
}

XEResult WriterXE3D::WriteSkeleton(const std::wstring& fileName, SkeletonContent* skeleton)
{
	XEAssert(skeleton != nullptr);

	if(skeleton == nullptr)
	{
		return XEResult::NullObj;
	}

	XEAssert(!fileName.empty());

	if(fileName.empty())
	{
		return XEResult::EmptyFilename;
	}

	std::ofstream skeletonFile;

	skeletonFile.open(fileName, std::ios::binary | std::ios::out | std::ios::trunc);

	if(!skeletonFile.is_open())
	{
		XETODO("add log");
		return XEResult::OpenFileFail;
	}

	const char* tempPtr = nullptr;
	uint32_t tempUInt32 = 0;
	int32_t tempInt32 = 0;
	uint32_t sizeToWrite = 0;
	
	/////////////////////////////////////////////
	//Start by writing the header and File Version
	XEGameContentHelpers::WriteFileHeader(skeletonFile, XE_CT_XE3DS_FILE_HEADER, XE_CT_XE3DS_FILE_VERSION_MAYOR, XE_CT_XE3DS_FILE_VERSION_MINOR, XE_CT_XE3DS_FILE_VERSION_REVISON);
	
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
		XEGameContentHelpers::WriteString(skeletonFile, bone->GetName());

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
	XEGameContentHelpers::WriteFileFooter(skeletonFile, XE_CT_XE3DS_FILE_FOOTER);
	
	/////////////////////////////////////////////
	//Finish 
	skeletonFile.flush();
	skeletonFile.close();

	return XEResult::Ok;
}

XEResult WriterXE3D::WriteAnimation(const std::wstring& fileName, AnimationContent* animation)
{
	XEAssert(animation != nullptr);
	if(animation == nullptr)
	{
		return XEResult::NullObj;
	}

	XEAssert(!fileName.empty());
	if(fileName.empty())
	{
		return XEResult::EmptyFilename;
	}

	std::ofstream animationFile;

	animationFile.open(fileName, std::ios::binary | std::ios::out | std::ios::trunc);

	if(!animationFile.is_open())
	{
		XETODO("add log");
		return XEResult::OpenFileFail;
	}

	const char* tempPtr = nullptr;
	uint32_t sizeToWrite = 0;
	
	/////////////////////////////////////////////
	//Start by writing the header and File Version
	XEGameContentHelpers::WriteFileHeader(animationFile, XE_CT_XE3DA_FILE_HEADER, XE_CT_XE3DA_FILE_VERSION_MAYOR, XE_CT_XE3DA_FILE_VERSION_MINOR, XE_CT_XE3DA_FILE_VERSION_REVISON);

	/////////////////////////////////////////////
	//Write Name of Animation and Size of Name
	XEGameContentHelpers::WriteString(animationFile, animation->m_Name);
	
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
	XEGameContentHelpers::WriteFileFooter(animationFile, XE_CT_XE3DA_FILE_FOOTER);
	
	/////////////////////////////////////////////
	//Finish 
	animationFile.flush();
	animationFile.close();

	return XEResult::Ok;
}
