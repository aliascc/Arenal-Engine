/********************************************************
*
* Author: Carlos Chacón N.
*
* Created: 12/30/2012
*
* Desc:
*
*
* Copyright 2012
*
*********************************************************/

/**********************
*   System Includes   *
***********************/
#include <fstream>

/*************************
*   3rd Party Includes   *
**************************/

/***************************
*   Game Engine Includes   *
****************************/
#include "Bone.h"
#include "Skeleton.h"
#include "GameContentDefs.h"
#include "Math\XEMathDefs.h"
#include "Base\BaseFunctions.h"

//Always include last
#include "Memory\MemLeaks.h"

/********************
*   Function Defs   *
*********************/
Skeleton::Skeleton(const std::wstring& resourceName)
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

XEResult Skeleton::Load()
{
	std::lock_guard<std::mutex> lock(m_GameResourceMutex);

	XEAssert(!m_FileName.empty());
	if(m_FileName.empty())
	{
		return XEResult::EmptyFilename;
	}
	
	/////////////////////////////////////////////
	//Clean Up memory before loading File
	CleanUp();

	XEResult ret = XEResult::Ok;
	
	/////////////////////////////////////////////
	//Start reading file
	std::ifstream skeletonFile;

	skeletonFile.open(m_FileName, std::ios::binary | std::ios::in);

	if(!skeletonFile.is_open())
	{
		XETODO("add log");

		return XEResult::OpenFileFail;
	}

	char* tempPtr = nullptr;
	uint32_t sizeToRead = 0;
	
	/////////////////////////////////////////////
	//Verify Header
	bool verifyHeader = XEGameContentHelpers::ReadFileHeaderAndVerify(skeletonFile, XE_CT_XE3DS_FILE_HEADER, XE_CT_XE3DS_FILE_VERSION_MAYOR, XE_CT_XE3DS_FILE_VERSION_MINOR, XE_CT_XE3DS_FILE_VERSION_REVISON);

	if(!verifyHeader)
	{
		XETODO("Add log");

		return XEResult::InvalidFileHeader;
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
		glm::mat4 tempMat = XEMathHelpers::Mat4Identity;
		int32_t tempInt32 = 0;

		/////////////////////////////////////////////
		//Write name of Bone and size of Name
		bone->SetName(XEGameContentHelpers::ReadString(skeletonFile));

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
	bool verifyFooter = XEGameContentHelpers::ReadFileFooterAndVerify(skeletonFile, XE_CT_XE3DS_FILE_FOOTER);
	if(!verifyFooter)
	{
		XETODO("Add Warning log");
	}
	
	/////////////////////////////////////////////
	//Finish
	skeletonFile.close();

	return XEResult::Ok;
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

XEResult Skeleton::AddBone(Bone* bone)
{
	XEAssert(bone != nullptr);

	if(bone == nullptr)
	{
		return XEResult::NullParameter;
	}
	
	std::lock_guard<std::mutex> lock(m_GameResourceMutex);

	XETODO("add warning");
	//bone->GetIndex() != m_BoneHierarchy.size(); //should be the same

	m_BoneHierarchy.push_back(bone);

	return XEResult::Ok;
}