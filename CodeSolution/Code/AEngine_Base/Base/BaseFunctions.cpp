/** \file 
 * Contains definitions and methods for basic use of the AEngine 
 */

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
#include <mutex>
#include <iostream>

/*************************
*   3rd Party Includes   *
**************************/

/***************************
*   Game Engine Includes   *
****************************/
#include "Base.h"
#include "BaseFunctions.h"
#include "Utils\BoostCustomFunctions.h"

//Always include last
#include "Memory\MemLeaks.h"

/**************
*   Defines   *
***************/
#define __AE_VERSION_MAJOR		0
#define __AE_VERSION_MINOR		5
#define __AE_VERSION_REVISION	0

/****************
*   Constants   *
*****************/
namespace AE_Base 
{	
	const uint32_t		AE_VERSION_MAYOR		= __AE_VERSION_MAJOR;
	const uint32_t		AE_VERSION_MINOR		= __AE_VERSION_MINOR;
	const uint32_t		AE_VERSION_REVISION		= __AE_VERSION_REVISION;
	const std::wstring	AE_VERSION				= AEWSTR(__AE_VERSION_MAJOR.__AE_VERSION_MINOR.__AE_VERSION_REVISION);
	const std::wstring	AE_CODENAME				= L"Star Dust";

/****************************
*	Static Variables init   *
*****************************/

	/// <summary>
	/// Unique ID Counter
	/// </summary>
	static uint64_t		AE_UNIQUE_ID		= 1;

	/// <summary>
	/// Mutex to make GetNextUniqueID thread safe and guarantee that no ID
	/// will be the same
	/// </summary>
	static std::mutex	AE_UNIQUE_ID_MUTEX;

/********************
*   Function Defs   *
*********************/

	std::wstring GetRelativePath(const std::wstring& filepath)
	{
		std::wstring relativePath = L"./";

		boost::filesystem::path boostFilepath(filepath);

		relativePath += boost::filesystem::make_relative(boost::filesystem::current_path(), boostFilepath).wstring();

		return relativePath;
	}

	std::vector<std::string> SplitString(const std::string& s, char delim)
	{
		std::stringstream ss(s);
		std::string item;
		std::vector<std::string> elems;

		while (std::getline(ss, item, delim))
		{
			elems.push_back(std::move(item));
		}

		return elems;
	}

	std::vector<std::wstring> SplitString(const std::wstring& s, wchar_t delim)
	{
		std::wstringstream ss(s);
		std::wstring item;
		std::vector<std::wstring> elems;

		while (std::getline(ss, item, delim))
		{
			elems.push_back(std::move(item));
		}

		return elems;
	}

	void SplitString(const std::wstring& str, std::vector<std::wstring>& tokens, const std::wstring& delimiters, bool trimEmpty)
	{
		std::wstring::size_type pos = 0;
		std::wstring::size_type lastPos = 0;

		while(true)
		{
			pos = str.find_first_of(delimiters, lastPos);
			if(pos == std::string::npos)
			{
				pos = str.length();

				if(pos != lastPos || !trimEmpty)
					tokens.push_back(std::vector<std::wstring>::value_type(str.data()+lastPos,
					(std::vector<std::wstring>::value_type::size_type)pos-lastPos ));

				break;
			}
			else
			{
				if(pos != lastPos || !trimEmpty)
					tokens.push_back(std::vector<std::wstring>::value_type(str.data()+lastPos,
					(std::vector<std::wstring>::value_type::size_type)pos-lastPos ));
			}

			lastPos = pos + 1;
		}
	};

	AEResult GetFileModifiedTime(const std::wstring& fileName, TimeStamp& timeStamp)
	{
		memset(&timeStamp, 0, sizeof(TimeStamp));

		if(fileName.empty())
		{
			return AEResult::EmptyFilename;
		}

		if(!boost::filesystem::exists(fileName))
		{
			return AEResult::Fail;
		}

		struct tm clock;

		time_t modifiedTime = boost::filesystem::last_write_time(fileName);

		errno_t error = gmtime_s(&clock, &(modifiedTime));

		if(error != 0)
		{
			return AEResult::Fail;
		}

		timeStamp.m_Day		= clock.tm_mday;
		timeStamp.m_Month	= clock.tm_mon;
		timeStamp.m_Year	= clock.tm_year + 1900;
		timeStamp.m_Hour	= clock.tm_hour;
		timeStamp.m_Minute	= clock.tm_min;
		timeStamp.m_Second	= clock.tm_sec;

		return AEResult::Ok;
	}

	void ConsolePrintLine(const std::wstring& msg)
	{
		std::wcout << msg << std::endl;
	}

	void ConsolePrint(const std::wstring& msg)
	{
		std::wcout << msg;
	}

	uint64_t GetNextUniqueID()
	{
		std::lock_guard<std::mutex> lock(AE_UNIQUE_ID_MUTEX);
		
		return AE_UNIQUE_ID++;
	}

	std::wstring GetAEResultString(AEResult result)
	{
		switch (result)
		{
			case AEResult::Fail:
				return L"Fail";

			case AEResult::NotReady:
				return L"NotReady";

			case AEResult::GraphicDeviceNull:
				return L"GraphicDeviceNull";

			case AEResult::NullParameter:
				return L"NullParameter";

			case AEResult::EmptyFilename:
				return L"EmptyFilename";

			case AEResult::CreateDXDeviceFail:
				return L"CreateDXDeviceFail";

			case AEResult::CreateDXConfFail:
				return L"CreateDXConfFail";

			case AEResult::FeatureLvlUnsupported:
				return L"FeatureLvlUnsupported";

			case AEResult::CreateDXSwapChainFail:
				return L"CreateDXSwapChainFail";

			case AEResult::CreateDXDefaultRTDSFail:
				return L"CreateDXDefaultRTDSFail";

			case AEResult::InitViewportFail:
				return L"InitViewportFail";

			case AEResult::RegGlobalPropFail:
				return L"RegGlobalPropFail";

			case AEResult::RegGlobalFuncFail:
				return L"RegGlobalFuncFail";

			case AEResult::RegObjTypeFail:
				return L"RegObjTypeFail";

			case AEResult::RegEnumValFail:
				return L"RegEnumValFail";

			case AEResult::RegEnumFail:
				return L"RegEnumFail";

			case AEResult::RegObjMethodFail:
				return L"RegObjMethodFail";

			case AEResult::RegObjPropFail:
				return L"RegObjPropFail";

			case AEResult::RegObjBehaviorFail:
				return L"RegObjBehaviorFail";

			case AEResult::RegInterfaceFail:
				return L"RegInterfaceFail";

			case AEResult::OpenFileFail:
				return L"OpenFileFail";

			case AEResult::NotFound:
				return L"NotFound";

			case AEResult::XMLReadError:
				return L"XMLReadError";

			case AEResult::InitLoggerFail:
				return L"InitLoggerFail";

			case AEResult::Log2FileActive:
				return L"Log2FileActive";

			case AEResult::ASModuleNotFound:
				return L"ASModuleNotFound";

			case AEResult::ASModuleAlreadyCreated:
				return L"ASModuleAlreadyCreated";

			case AEResult::ASModuleCreateFail:
				return L"ASModuleCreateFail";

			case AEResult::ASAddSecModuleFail:
				return L"ASAddSecModuleFail";

			case AEResult::ASBuildModuleFail:
				return L"ASBuildModuleFail";

			case AEResult::ASDiscardModuleFail:
				return L"ASDiscardModuleFail";

			case AEResult::ASPrepareContextFailed:
				return L"ASPrepareContextFailed";

			case AEResult::ASSetArgObjectFailed:
				return L"ASSetArgObjectFailed";

			case AEResult::KeyboardLockFailed:
				return L"KeyboardLockFailed";

			case AEResult::KeyboardUnLockFailed:
				return L"KeyboardUnLockFailed";

			case AEResult::StructuredBufferInitFailed:
				return L"StructuredBufferInitFailed";

			case AEResult::StructuredBufferSetDateFailed:
				return L"StructuredBufferSetDateFailed";

			case AEResult::SimpleBufferInitFailed:
				return L"SimpleBufferInitFailed";

			case AEResult::SimpleBufferSetDateFailed:
				return L"SimpleBufferSetDateFailed";

			case AEResult::ConstantBufferInitFailed:
				return L"ConstantBufferInitFailed";

			case AEResult::ConstantBufferSetDataFailed:
				return L"ConstantBufferSetDataFailed";

			case AEResult::VertexShaderLoadFailed:
				return L"VertexShaderLoadFailed";

			case AEResult::PixelShaderLoadFailed:
				return L"PixelShaderLoadFailed";

			case AEResult::HullShaderLoadFailed:
				return L"HullShaderLoadFailed";

			case AEResult::DomainShaderLoadFailed:
				return L"DomainShaderLoadFailed";

			case AEResult::GeometryShaderLoadFailed:
				return L"GeometryShaderLoadFailed";

			case AEResult::ComputeShaderLoadFailed:
				return L"ComputeShaderLoadFailed";

			case AEResult::EmptyString:
				return L"EmptyString";

			case AEResult::ObjExists:
				return L"ObjExists";

			case AEResult::InvalidPermutation:
				return L"InvalidPermutation";

			case AEResult::SetTextureFailed:
				return L"SetTextureFailed";

			case AEResult::NullObj:
				return L"NullObj";

			case AEResult::OutsideRange:
				return L"OutsideRange";

			case AEResult::LoadFileFailed:
				return L"LoadFileFailed";

			case AEResult::AssimpProcessGraph:
				return L"AssimpProcessGraph";

			case AEResult::AssimpProcessMaterials:
				return L"AssimpProcessMaterials";

			case AEResult::AssimpProcessEmbeddedTextures:
				return L"AssimpProcessEmbeddedTextures";

			case AEResult::AssimpProcessMeshes:
				return L"AssimpProcessMeshes";

			case AEResult::AssimpProcessSkeleton:
				return L"AssimpProcessSkeleton";

			case AEResult::AssimpProcessAnimation:
				return L"AssimpProcessAnimation";

			case AEResult::AssimpProcessConstructModel:
				return L"AssimpProcessConstructModel";

			case AEResult::EmptySkeleton:
				return L"EmptySkeleton";

			case AEResult::EmptyName:
				return L"EmptyName";

			case AEResult::GameResourceManagerNull:
				return L"GameResourceManagerNull";

			case AEResult::ResourceIsManaged:
				return L"ResourceIsManaged";

			case AEResult::ResourceManagedFailed:
				return L"ResourceManagedFailed";

			case AEResult::ResourceManagedTypeError:
				return L"ResourceManagedTypeError";

			case AEResult::CreateSRViewFailed:
				return L"CreateSRViewFailed";

			case AEResult::CreateRTViewFailed:
				return L"CreateRTViewFailed";

			case AEResult::CreateTextureFailed:
				return L"CreateTextureFailed";

			case AEResult::CreateDSViewFailed:
				return L"CreateDSViewFailed";

			case AEResult::MaximunItemType:
				return L"MaximunItemType";

			case AEResult::InvalidObjType:
				return L"InvalidObjType";

			case AEResult::ZeroSize:
				return L"ZeroSize";

			case AEResult::InvalidTextureType:
				return L"InvalidTextureType";

			case AEResult::InvalidShaderType:
				return L"InvalidShaderType";

			case AEResult::SetConstantBufferFailed:
				return L"SetConstantBufferFailed";

			case AEResult::SetSimpleBufferFailed:
				return L"SetSimpleBufferFailed";

			case AEResult::SetStructuredBufferFailed:
				return L"SetStructuredBufferFailed";

			case AEResult::SetVertexShaderFailed:
				return L"SetVertexShaderFailed";

			case AEResult::SetPixelShaderFailed:
				return L"SetPixelShaderFailed";

			case AEResult::SetGeometryShaderFailed:
				return L"SetGeometryShaderFailed";

			case AEResult::SetDomainShaderFailed:
				return L"SetDomainShaderFailed";

			case AEResult::SetHullShaderFailed:
				return L"SetHullShaderFailed";

			case AEResult::SetComputeShaderFailed:
				return L"SetComputeShaderFailed";

			case AEResult::SetVertexShaderPropsFailed:
				return L"SetVertexShaderPropsFailed";

			case AEResult::SetPixelShaderPropsFailed:
				return L"SetPixelShaderPropsFailed";

			case AEResult::SetGeometryShaderPropsFailed:
				return L"SetGeometryShaderPropsFailed";

			case AEResult::SetDomainShaderPropsFailed:
				return L"SetDomainShaderPropsFailed";

			case AEResult::SetHullShaderPropsFailed:
				return L"SetHullShaderPropsFailed";

			case AEResult::SetComputeShaderPropsFailed:
				return L"SetComputeShaderPropsFailed";

			case AEResult::ShaderTextureBindingInitFailed:
				return L"ShaderTextureBindingInitFailed";

			case AEResult::ShaderStructuredBufferInitFailed:
				return L"ShaderStructuredBufferInitFailed";

			case AEResult::ShaderSimpleBufferInitFailed:
				return L"ShaderSimpleBufferInitFailed";

			case AEResult::UnknownFileExtension:
				return L"UnknownFileExtension";

			case AEResult::EmptyFileExtension:
				return L"EmptyFileExtension";

			case AEResult::UnknownContentType:
				return L"UnknownContentType";

			case AEResult::GOCIsAssociated:
				return L"GOCIsAssociated";

			case AEResult::GOCIsNotAssociated:
				return L"GOCIsNotAssociated";

			case AEResult::GameObjectManagerNull:
				return L"GameObjectManagerNull";

			case AEResult::LocalizationManagerNull:
				return L"LocalizationManagerNull";

			case AEResult::LoggerNull:
				return L"LoggerNull";

			case AEResult::ImportFail:
				return L"ImportFail";

			case AEResult::InvalidFileHeader:
				return L"InvalidFileHeader";

			case AEResult::InvalidShaderModel:
				return L"InvalidShaderModel";

			case AEResult::ShaderCompiledFail:
				return L"ShaderCompiledFail";

			case AEResult::ShaderExtractSimpleBuffersFail:
				return L"ShaderExtractSimpleBuffersFail";

			case AEResult::ShaderExtractStructuredBuffersFail:
				return L"ShaderExtractStructuredBuffersFail";

			case AEResult::ShaderExtractConstantBuffersFail:
				return L"ShaderExtractConstantBuffersFail";

			case AEResult::ShaderExtractTextureInputFail:
				return L"ShaderExtractTextureInputFail";

			case AEResult::ShaderTypeMismatch:
				return L"ShaderTypeMismatch";

			case AEResult::FileNotPresent:
				return L"FileNotPresent";

			case AEResult::GameAssetManagerNull:
				return L"GameAssetManagerNull"; 

			case AEResult::GameAssetNotLoaded:
				return L"GameAssetNotLoaded";

			case AEResult::GameAssetIDInUse:
				return L"GameAssetIDInUse";

			case AEResult::GameAssetInvalidID:
				return L"GameAssetInvalidID";

			case AEResult::RawGameAssetNotFound:
				return L"RawGameAssetNotFound";

			case AEResult::ShaderNull:
				return L"ShaderNull";

			case AEResult::InvalidResourceMapType:
				return L"InvalidResourceMapType";

			case AEResult::ResourceMapFailed:
				return L"ResourceMapFailed";

			case AEResult::InvalidIndexBufferType:
				return L"InvalidIndexBufferType";

			case AEResult::FailIndexBufferUpdate:
				return L"FailIndexBufferUpdate";

			case AEResult::FailIndexBufferInit:
				return L"FailIndexBufferInit";

			case AEResult::FullContainer:
				return L"FullContainer"; 

			case AEResult::LightManagerNull:
				return L"LightManagerNull";

			case AEResult::CameraNull:
				return L"CameraNull";

			case AEResult::WriteToFileFailed:
				return L"WriteToFileFailed";

			case AEResult::Ok:
				return L"Ok";

			default:
				AEAssert(false);
				return L"N/A";
		}
	}
}
