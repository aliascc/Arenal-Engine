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
#include "precomp_core.h"

/**********************
*   System Includes   *
***********************/

/*************************
*   3rd Party Includes   *
**************************/

/***************************
*   Game Engine Includes   *
****************************/
#include "RawGameAsset.h"
#include "Base\BaseFunctions.h"

//Always include last
#include "Memory\MemLeaks.h"

/********************
*   Function Defs   *
*********************/
AETODO("Check if this class needs a mutex");
RawGameAsset::RawGameAsset(const std::string& filePath, const std::string& projectDirectory, const std::string& name)
    : UniqueAEObjectNamed(name)
    , m_CustomName(name)
    , m_FilePath(filePath)
{
    AEAssert(!filePath.empty());
    AEAssert(!projectDirectory.empty());

    m_ProjectDirectory = projectDirectory;

    if (m_ProjectDirectory.back() != L'\\' && m_ProjectDirectory.back() != L'/')
    {
        m_ProjectDirectory.push_back(L'\\');
    }
}

RawGameAsset::~RawGameAsset()
{
}

bool RawGameAsset::IsReloadNeeded() const
{
    return (m_ReloadNeeded || m_ContentSubtypeChanged);
}

void RawGameAsset::SetContentType(GameContentType contentType)
{
    if (m_ContentType == contentType)
    {
        return;
    }

    m_ContentType = contentType;

    AETODO("Check if it needs to add flag")
    //m_ContentSubtypeChanged = true;
}

void RawGameAsset::SetContentSubtype(GameContentSubtype contentSubtype)
{
    if(m_ContentSubtype == contentSubtype)
    {
        return;
    }

    m_ContentSubtype = contentSubtype;

    m_ContentSubtypeChanged = true;
}

void RawGameAsset::SetGameContextFileExt(GameContextFileExt gameContextFileExt)
{
    if (m_GameContextFileExt == gameContextFileExt)
    {
        return;
    }

    m_GameContextFileExt = gameContextFileExt;

    AETODO("Check if it needs to add flag")
    //m_ContentSubtypeChanged = true;
}

void RawGameAsset::SetLastModifiedTimeStamp(TimeStamp lastModifiedTimeStamp)
{
    if (m_LastModifiedTimeStamp == lastModifiedTimeStamp)
    {
        return;
    }

    m_LastModifiedTimeStamp = lastModifiedTimeStamp;

    AETODO("Check if it needs to add flag")
    //m_ContentSubtypeChanged = true;
}

void RawGameAsset::SetOutputFileName(std::string outputFileName)
{
    if (m_OutputFileName == outputFileName)
    {
        return;
    }

    m_OutputFileName = outputFileName;

    AETODO("Check if it needs to add flag")
    //m_ContentSubtypeChanged = true;
}

AEResult RawGameAsset::ParseRawGameAssetFile()
{
    if(m_ContentType != GameContentType::Unknown && m_ContentSubtypeChanged == false)
    {
        return AEResult::Ok;
    }
    
    AEAssert(!m_FilePath.empty());
    if(m_FilePath.empty())
    {
        return AEResult::EmptyFilename;
    }

    std::string fileName = AE_Base::GetFilenameOnly(m_FilePath);
    if(fileName.empty())
    {
        return AEResult::EmptyFilename;
    }

    if(!boost::filesystem::is_regular_file(m_FilePath))
    {
        return AEResult::LoadFileFailed;
    }

    if(AEGameContentHelpers::FileGameContentType(m_FilePath, m_ContentType, m_GameContextFileExt) != AEResult::Ok)
    {
        return AEResult::UnknownContentType;
    }

    m_OutputFileName = fileName;

    m_ReloadNeeded = true;

    return AEResult::Ok;
}

AEResult RawGameAsset::CheckIfLatest()
{
    if(m_ContentType == GameContentType::Unknown)
    {
        return AEResult::Fail;
    }

    TimeStamp modifiedTime;

    if(AE_Base::GetFileModifiedTime(m_FilePath, modifiedTime) != AEResult::Ok)
    {
        return AEResult::Fail;
    }

    if(m_LastModifiedTimeStamp == modifiedTime)
    {
        m_ReloadNeeded = false;
    }
    else if(m_LastModifiedTimeStamp < modifiedTime)
    {
        m_ReloadNeeded = true;
    }

    return AEResult::Ok;
}

AEResult RawGameAsset::ImportDone()
{
    if(m_ContentType == GameContentType::Unknown)
    {
        return AEResult::Fail;
    }

    TimeStamp modifiedTime;

    if(AE_Base::GetFileModifiedTime(m_FilePath, modifiedTime) != AEResult::Ok)
    {
        return AEResult::Fail;
    }

    m_LastModifiedTimeStamp = modifiedTime;
    
    m_ReloadNeeded = false;

    m_ContentSubtypeChanged = false;

    return AEResult::Ok;

}
