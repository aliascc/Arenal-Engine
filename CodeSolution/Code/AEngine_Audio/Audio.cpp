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
#include "precomp_audio.h"

/**********************
*   System Includes   *
***********************/

/*************************
*   3rd Party Includes   *
**************************/

/***************************
*   Game Engine Includes   *
****************************/
#include "Audio.h"

//Always include last
#include "Memory\MemLeaks.h"

/********************
*   Function Defs   *
*********************/

Audio::Audio(const std::string& audioName)
    : GameResource(audioName, GameResourceType::Audio)
{
}

Audio::~Audio()
{
    DeleteMemArr(m_AudioData);
}

AEResult Audio::LoadFile(const std::string& file)
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
