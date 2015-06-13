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
#include <fstream>
#include <algorithm>
#include <stdexcept>

/*************************
*   3rd Party Includes   *
**************************/

/***************************
*   Game Engine Includes   *
****************************/
#include "Animation.h"
#include "GameContentDefs.h"
#include "Time\AETimeDefs.h"
#include "Base\BaseFunctions.h"

//Always include last
#include "Memory\MemLeaks.h"

/********************
*   Function Defs   *
*********************/
Animation::Animation(const std::wstring& resourceName)
	: GameResource(resourceName, GameResourceType::Animation)
{
}

Animation::~Animation()
{
}

void Animation::CleanUp()
{
	m_AnimationCallbacks.clear();

	m_KeyFrames.clear();

	m_LastKeyFrameCheck = 0;
}

AEResult Animation::Load()
{
	std::lock_guard<std::mutex> lock(m_GameResourceMutex);

	AEAssert(!m_FileName.empty());
	if(m_FileName.empty())
	{
		return AEResult::EmptyFilename;
	}
	
	/////////////////////////////////////////////
	//Clean Up memory before loading File
	CleanUp();
	
	AEResult ret = AEResult::Ok;
	
	/////////////////////////////////////////////
	//Start reading file
	std::ifstream animationFile;

	animationFile.open(m_FileName, std::ios::binary | std::ios::in);

	if(!animationFile.is_open())
	{
		AETODO("add log");

		return AEResult::OpenFileFail;
	}

	char* tempPtr = nullptr;
	uint32_t sizeToRead = 0;
	
	/////////////////////////////////////////////
	//Verify Header
	bool verifyHeader = AEGameContentHelpers::ReadFileHeaderAndVerify(animationFile, AE_CT_AE3DA_FILE_HEADER, AE_CT_AE3DA_FILE_VERSION_MAYOR, AE_CT_AE3DA_FILE_VERSION_MINOR, AE_CT_AE3DA_FILE_VERSION_REVISON);

	if(!verifyHeader)
	{
		AETODO("Add log");

		return AEResult::InvalidFileHeader;
	}
	
	/////////////////////////////////////////////
	//Get Name of Mesh
	this->SetFileNameWithoutLock(AEGameContentHelpers::ReadString(animationFile));

	/////////////////////////////////////////////
	//Read Duration of Animation
	float duration = 0.0f;
	tempPtr = reinterpret_cast<char*>(&duration);
	sizeToRead = sizeof(float);
	animationFile.read(tempPtr, sizeToRead);

	m_Duration = duration;
	
	/////////////////////////////////////////////
	//Write Number of Key Frames
	uint32_t sizeKeyFrames = 0;
	tempPtr = reinterpret_cast<char*>(&sizeKeyFrames);
	sizeToRead = sizeof(uint32_t);
	animationFile.read(tempPtr, sizeToRead);
	
	for (uint32_t i = 0; i < sizeKeyFrames; ++i)
	{
		KeyFrame keyFrame;
		
		/////////////////////////////////////////////
		//Read Frame Time and Bone Index

		//Time
		float time = 0.0f;
		tempPtr = reinterpret_cast<char*>(&time);
		sizeToRead = sizeof(float);
		animationFile.read(tempPtr, sizeToRead);

		keyFrame.SetTime(time);

		//Bone Index
		int32_t boneIndex = 0;
		tempPtr = reinterpret_cast<char*>(&boneIndex);
		sizeToRead = sizeof(int32_t);
		animationFile.read(tempPtr, sizeToRead);

		keyFrame.SetBoneIndex(boneIndex);
		
		/////////////////////////////////////////////
		//Read Bone Pose
		BonePose bonePose;

		//Rotation Quaternion
		tempPtr = reinterpret_cast<char*>(&bonePose.m_RotationQ);
		sizeToRead = sizeof(glm::quat);
		animationFile.read(tempPtr, sizeToRead);

		//Scale
		tempPtr = reinterpret_cast<char*>(&bonePose.m_Scale);
		sizeToRead = sizeof(glm::vec3);
		animationFile.read(tempPtr, sizeToRead);

		//Translation
		tempPtr = reinterpret_cast<char*>(&bonePose.m_Translation);
		sizeToRead = sizeof(glm::vec3);
		animationFile.read(tempPtr, sizeToRead);

		bonePose.CalculateTransformMatrix();

		keyFrame.SetBonePose(bonePose);
		
		/////////////////////////////////////////////
		//Add Key Frame
		m_KeyFrames.push_back(keyFrame);
	}
	
	/////////////////////////////////////////////
	//Sort Key Frames
	SortKeyFrames();
	
	/////////////////////////////////////////////
	//Read Footer 
	bool verifyFooter = AEGameContentHelpers::ReadFileFooterAndVerify(animationFile, AE_CT_AE3DA_FILE_FOOTER);
	if(!verifyFooter)
	{
		AETODO("Add Warning log");
	}
	
	/////////////////////////////////////////////
	//Finish
	animationFile.close();

	return AEResult::Ok;
}

void Animation::SortKeyFrames()
{
	std::sort(m_KeyFrames.begin(), m_KeyFrames.end());

	m_NeedSort = false;
}

AEResult Animation::AddKeyFrame(const KeyFrame& keyFrame)
{
	std::lock_guard<std::mutex> lock(m_GameResourceMutex);
	
	AETODO("If Key frame is added, a change in duration might be need, we would need to check");
	AETODO("Possible checks are keyFrame.GetTime(), if is more than duration, then set it to new duration");
	m_KeyFrames.push_back(keyFrame);

	m_NeedSort = true;

	return AEResult::Ok;
}

AETODO("Change this to sort key frames if needed");
void Animation::Update(const TimerParams& timerParams)
{
	std::lock_guard<std::mutex> lock(m_GameResourceMutex);

	if(m_NeedSort)
	{
		SortKeyFrames();
	}
}

const KeyFrame& Animation::GetKeyFrame(uint32_t index)
{
	std::lock_guard<std::mutex> lock(m_GameResourceMutex);

	if(index >= (uint32_t)m_KeyFrames.size())
	{
		AEAssert(false);

		AETODO("Add loc lang");
		throw std::out_of_range("Key Index is out of range");
	}

	return m_KeyFrames[index];
}

const KeyFrame& Animation::operator[](uint32_t index) 
{
	return GetKeyFrame(index);
}

AEResult Animation::AddAnimationCallBack(float time, AnimationCallBackFunc animCallBack)
{
	if(animCallBack == nullptr)
	{
		return AEResult::NullParameter;
	}

	std::lock_guard<std::mutex> lock(m_GameResourceMutex);

	AnimationCallBack callBack;

	callBack.m_Time = time;
	callBack.m_AnimFuncPtr = animCallBack;

	m_AnimationCallbacks.push_back(callBack);

	return AEResult::Ok;
}

AEResult Animation::CheckCallBacks(uint32_t currentKeyFrame)
{
	std::lock_guard<std::mutex> lock(m_GameResourceMutex);

	if(currentKeyFrame >= m_KeyFrames.size())
	{
		return AEResult::OutsideRange;
	}

	if(m_LastKeyFrameCheck >= m_KeyFrames.size())
	{
		m_LastKeyFrameCheck = 0;
	}

	uint32_t size = (uint32_t)m_AnimationCallbacks.size();

	float previousTime = m_KeyFrames[m_LastKeyFrameCheck].GetTime();
	float currentTime = m_KeyFrames[currentKeyFrame].GetTime();

	for(uint32_t i = 0; i < size; ++i)
	{
		if(previousTime < m_AnimationCallbacks[i].m_Time && m_AnimationCallbacks[i].m_Time <= currentTime)
		{
			m_AnimationCallbacks[i].m_AnimFuncPtr();
		}
	}

	m_LastKeyFrameCheck = currentKeyFrame;

	return AEResult::Ok;
}
