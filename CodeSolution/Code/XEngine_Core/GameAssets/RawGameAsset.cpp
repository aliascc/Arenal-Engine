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

/*************************
*   3rd Party Includes   *
**************************/
#include "cppformat\format.h"
#include "boost\filesystem.hpp"

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
XETODO("Check if this class needs a mutex");
RawGameAsset::RawGameAsset(const std::wstring& filePath, const std::wstring& projectDirectory, const std::wstring& name)
	: UniqueXEObjectNamed(name)
	, m_CustomName(name)
	, m_FilePath(filePath)
{
	XEAssert(!filePath.empty());
	XEAssert(!projectDirectory.empty());

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
	return (m_ReloadNeeded || m_OutputDirChanged || m_ContentSubtypeChanged);
}

void RawGameAsset::SetOutputDirectory(const std::wstring& outputDirectory)
{
	if(m_OutputDirectory.compare(outputDirectory) == 0)
	{
		return;
	}

	m_OutputDirectory = outputDirectory;

	if(m_OutputDirectory.back() != L'\\' && m_OutputDirectory.back() != L'/')
	{
		m_OutputDirectory.push_back(L'\\');
	}

	m_OutputDirChanged = true;
}

void RawGameAsset::SetContentType(GameContentType contentType)
{
	if (m_ContentType == contentType)
	{
		return;
	}

	m_ContentType = contentType;

	XETODO("Check if it needs to add flag")
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

	XETODO("Check if it needs to add flag")
	//m_ContentSubtypeChanged = true;
}

void RawGameAsset::SetLastModifiedTimeStamp(TimeStamp lastModifiedTimeStamp)
{
	if (m_LastModifiedTimeStamp == lastModifiedTimeStamp)
	{
		return;
	}

	m_LastModifiedTimeStamp = lastModifiedTimeStamp;

	XETODO("Check if it needs to add flag")
	//m_ContentSubtypeChanged = true;
}

void RawGameAsset::SetOutputFileName(std::wstring outputFileName)
{
	if (m_OutputFileName == outputFileName)
	{
		return;
	}

	m_OutputFileName = outputFileName;

	XETODO("Check if it needs to add flag")
	//m_ContentSubtypeChanged = true;
}

XEResult RawGameAsset::ParseRawGameAssetFile()
{
	if(m_ContentType != GameContentType::Unknown && m_ContentSubtypeChanged == false)
	{
		return XEResult::Ok;
	}
	
	XEAssert(!m_FilePath.empty());
	if(m_FilePath.empty())
	{
		return XEResult::EmptyFilename;
	}

	std::wstring fileName = XE_Base::GetFilenameOnly(m_FilePath);
	if(fileName.empty())
	{
		return XEResult::EmptyFilename;
	}

	if(!boost::filesystem::is_regular_file(m_FilePath))
	{
		return XEResult::LoadFileFailed;
	}

	if(XEGameContentHelpers::FileGameContentType(m_FilePath, m_ContentType, m_GameContextFileExt) != XEResult::Ok)
	{
		return XEResult::UnknownContentType;
	}

	m_OutputFileName = fileName;

	m_ReloadNeeded = true;

	return XEResult::Ok;
}

XEResult RawGameAsset::CheckIfLatest()
{
	if(m_ContentType == GameContentType::Unknown)
	{
		return XEResult::Fail;
	}

	TimeStamp modifiedTime;

	if(XE_Base::GetFileModifiedTime(m_FilePath, modifiedTime) != XEResult::Ok)
	{
		return XEResult::Fail;
	}

	if(m_LastModifiedTimeStamp == modifiedTime)
	{
		m_ReloadNeeded = false;
	}
	else if(m_LastModifiedTimeStamp < modifiedTime)
	{
		m_ReloadNeeded = true;
	}

	return XEResult::Ok;
}

XEResult RawGameAsset::ImportDone()
{
	if(m_ContentType == GameContentType::Unknown)
	{
		return XEResult::Fail;
	}

	TimeStamp modifiedTime;

	if(XE_Base::GetFileModifiedTime(m_FilePath, modifiedTime) != XEResult::Ok)
	{
		return XEResult::Fail;
	}

	m_LastModifiedTimeStamp = modifiedTime;
	
	m_ReloadNeeded = false;

	m_OutputDirChanged = false;

	m_ContentSubtypeChanged = false;

	return XEResult::Ok;

}

std::wstring RawGameAsset::GetFullOutputPath() const
{
	std::wstring fullOutput = m_ProjectDirectory + m_OutputDirectory + m_OutputFileName;

	return fullOutput;
}
