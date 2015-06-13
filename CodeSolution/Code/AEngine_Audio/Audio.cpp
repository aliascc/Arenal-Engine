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
#include <istream>

/*************************
*   3rd Party Includes   *
**************************/

/***************************
*   Game Engine Includes   *
****************************/
#include "Audio.h"
#include "Base\BaseFunctions.h"

//Always include last
#include "Memory\MemLeaks.h"

/********************
*   Function Defs   *
*********************/

Audio::Audio(const std::wstring& audioName)
	: GameResource(audioName, GameResourceType::Audio)
{
}

Audio::~Audio()
{
	DeleteMemArr(m_AudioData);
}

AEResult Audio::LoadFile(const std::wstring& file)
{
	if (file.empty())
	{
		return AEResult::EmptyString;
	}

	SetFileName(file);

	return Load();
}

AEResult Audio::Load()
{
	///////////////////////////////////////////
	//This has to be an atomic operation
	std::lock_guard<std::mutex> lock(m_GameResourceMutex);

	///////////////////////////////////////////
	//Pre-checks
	if (m_FileName.empty())
	{
		return AEResult::EmptyFilename;
	}

	///////////////////////////////////////////
	//Open File
	std::ifstream audioFile(m_FileName, std::ios::in | std::ios::binary | std::ios::ate);
	if (!audioFile.is_open())
	{
		return AEResult::OpenFileFail;
	}

	///////////////////////////////////////////
	//Get Size of File
	uint32_t sizeFile = (uint32_t)audioFile.tellg();
	if (sizeFile == 0)
	{
		audioFile.close();

		return AEResult::ZeroSize;
	}

	///////////////////////////////////////////
	//Load contents to memory
	uint8_t* audioData = new uint8_t[sizeFile];

	audioFile.seekg(0, std::ios::beg);
	audioFile.read((char*)audioData, sizeFile);

	///////////////////////////////////////////
	//Close File
	audioFile.close();

	///////////////////////////////////////////
	//Remove old memory
	DeleteMemArr(m_AudioData);

	///////////////////////////////////////////
	//Set new pointers
	m_AudioData = audioData;
	m_AudioDataSize = sizeFile;

	///////////////////////////////////////////
	//Finish
	return AEResult::Ok;
}
