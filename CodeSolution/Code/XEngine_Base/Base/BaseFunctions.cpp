/** \file 
 * Contains definitions and methods for basic use of the XEngine 
 */

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
#define __XE_VERSION_MAJOR		0
#define __XE_VERSION_MINOR		5
#define __XE_VERSION_REVISION	0

/****************
*   Constants   *
*****************/
namespace XE_Base 
{	
	const uint32_t		XE_VERSION_MAYOR		= __XE_VERSION_MAJOR;
	const uint32_t		XE_VERSION_MINOR		= __XE_VERSION_MINOR;
	const uint32_t		XE_VERSION_REVISION		= __XE_VERSION_REVISION;
	const std::wstring	XE_VERSION				= XEWSTR(__XE_VERSION_MAJOR.__XE_VERSION_MINOR.__XE_VERSION_REVISION);
	const std::wstring	XE_CODENAME				= L"Star Dust";

/****************************
*	Static Variables init   *
*****************************/

	/// <summary>
	/// Unique ID Counter
	/// </summary>
	static uint64_t		XE_UNIQUE_ID		= 1;

	/// <summary>
	/// Mutex to make GetNextUniqueID thread safe and guarantee that no ID
	/// will be the same
	/// </summary>
	static std::mutex	XE_UNIQUE_ID_MUTEX;

/********************
*   Function Defs   *
*********************/

	std::wstring GetRelativePath(const std::wstring& filepath)
	{
		std::wstring relativePath = L".\\";

		boost::filesystem::path boostFilepath(filepath);

		relativePath += boost::filesystem::make_relative(boost::filesystem::current_path(), boostFilepath).c_str();

		return relativePath;
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

	XEResult GetFileModifiedTime(const std::wstring& fileName, TimeStamp& timeStamp)
	{
		memset(&timeStamp, 0, sizeof(TimeStamp));

		if(fileName.empty())
		{
			return XEResult::EmptyFilename;
		}

		if(!boost::filesystem::exists(fileName))
		{
			return XEResult::Fail;
		}

		struct tm clock;

		time_t modifiedTime = boost::filesystem::last_write_time(fileName);

		errno_t error = gmtime_s(&clock, &(modifiedTime));

		if(error != 0)
		{
			return XEResult::Fail;
		}

		timeStamp.m_Day		= clock.tm_mday;
		timeStamp.m_Month	= clock.tm_mon;
		timeStamp.m_Year	= clock.tm_year + 1900;
		timeStamp.m_Hour	= clock.tm_hour;
		timeStamp.m_Minute	= clock.tm_min;
		timeStamp.m_Second	= clock.tm_sec;

		return XEResult::Ok;
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
		std::lock_guard<std::mutex> lock(XE_UNIQUE_ID_MUTEX);
		
		return XE_UNIQUE_ID++;
	}

	std::wstring GetXEResultString(XEResult result)
	{
		switch (result)
		{
			case XEResult::Fail:
				return L"Fail";

			case XEResult::NotReady:
				return L"NotReady";

			case XEResult::GraphicDeviceNull:
				return L"GraphicDeviceNull";

			case XEResult::NullParameter:
				return L"NullParameter";

			case XEResult::EmptyFilename:
				return L"EmptyFilename";

			case XEResult::CreateDXDeviceFail:
				return L"CreateDXDeviceFail";

			case XEResult::CreateDXConfFail:
				return L"CreateDXConfFail";

			case XEResult::FeatureLvlUnsupported:
				return L"FeatureLvlUnsupported";

			case XEResult::CreateDXSwapChainFail:
				return L"CreateDXSwapChainFail";

			case XEResult::CreateDXDefaultRTDSFail:
				return L"CreateDXDefaultRTDSFail";

			case XEResult::InitViewportFail:
				return L"InitViewportFail";

			case XEResult::RegGlobalPropFail:
				return L"RegGlobalPropFail";

			case XEResult::RegGlobalFuncFail:
				return L"RegGlobalFuncFail";

			case XEResult::RegObjTypeFail:
				return L"RegObjTypeFail";

			case XEResult::RegEnumValFail:
				return L"RegEnumValFail";

			case XEResult::RegEnumFail:
				return L"RegEnumFail";

			case XEResult::RegObjMethodFail:
				return L"RegObjMethodFail";

			case XEResult::RegObjPropFail:
				return L"RegObjPropFail";

			case XEResult::RegObjBehaviorFail:
				return L"RegObjBehaviorFail";

			case XEResult::RegInterfaceFail:
				return L"RegInterfaceFail";

			case XEResult::OpenFileFail:
				return L"OpenFileFail";

			case XEResult::NotFound:
				return L"NotFound";

			case XEResult::XMLReadError:
				return L"XMLReadError";

			case XEResult::InitLoggerFail:
				return L"InitLoggerFail";

			case XEResult::Log2FileActive:
				return L"Log2FileActive";

			case XEResult::ASModuleNotFound:
				return L"ASModuleNotFound";

			case XEResult::ASModuleAlreadyCreated:
				return L"ASModuleAlreadyCreated";

			case XEResult::ASModuleCreateFail:
				return L"ASModuleCreateFail";

			case XEResult::ASAddSecModuleFail:
				return L"ASAddSecModuleFail";

			case XEResult::ASBuildModuleFail:
				return L"ASBuildModuleFail";

			case XEResult::ASDiscardModuleFail:
				return L"ASDiscardModuleFail";

			case XEResult::ASPrepareContextFailed:
				return L"ASPrepareContextFailed";

			case XEResult::ASSetArgObjectFailed:
				return L"ASSetArgObjectFailed";

			case XEResult::KeyboardLockFailed:
				return L"KeyboardLockFailed";

			case XEResult::KeyboardUnLockFailed:
				return L"KeyboardUnLockFailed";

			case XEResult::StructuredBufferInitFailed:
				return L"StructuredBufferInitFailed";

			case XEResult::StructuredBufferSetDateFailed:
				return L"StructuredBufferSetDateFailed";

			case XEResult::SimpleBufferInitFailed:
				return L"SimpleBufferInitFailed";

			case XEResult::SimpleBufferSetDateFailed:
				return L"SimpleBufferSetDateFailed";

			case XEResult::ConstantBufferInitFailed:
				return L"ConstantBufferInitFailed";

			case XEResult::ConstantBufferSetDataFailed:
				return L"ConstantBufferSetDataFailed";

			case XEResult::VertexShaderLoadFailed:
				return L"VertexShaderLoadFailed";

			case XEResult::PixelShaderLoadFailed:
				return L"PixelShaderLoadFailed";

			case XEResult::HullShaderLoadFailed:
				return L"HullShaderLoadFailed";

			case XEResult::DomainShaderLoadFailed:
				return L"DomainShaderLoadFailed";

			case XEResult::GeometryShaderLoadFailed:
				return L"GeometryShaderLoadFailed";

			case XEResult::ComputeShaderLoadFailed:
				return L"ComputeShaderLoadFailed";

			case XEResult::EmptyString:
				return L"EmptyString";

			case XEResult::ObjExists:
				return L"ObjExists";

			case XEResult::InvalidPermutation:
				return L"InvalidPermutation";

			case XEResult::SetTextureFailed:
				return L"SetTextureFailed";

			case XEResult::NullObj:
				return L"NullObj";

			case XEResult::OutsideRange:
				return L"OutsideRange";

			case XEResult::LoadFileFailed:
				return L"LoadFileFailed";

			case XEResult::AssimpProcessGraph:
				return L"AssimpProcessGraph";

			case XEResult::AssimpProcessMaterials:
				return L"AssimpProcessMaterials";

			case XEResult::AssimpProcessEmbeddedTextures:
				return L"AssimpProcessEmbeddedTextures";

			case XEResult::AssimpProcessMeshes:
				return L"AssimpProcessMeshes";

			case XEResult::AssimpProcessSkeleton:
				return L"AssimpProcessSkeleton";

			case XEResult::AssimpProcessAnimation:
				return L"AssimpProcessAnimation";

			case XEResult::AssimpProcessConstructModel:
				return L"AssimpProcessConstructModel";

			case XEResult::EmptySkeleton:
				return L"EmptySkeleton";

			case XEResult::EmptyName:
				return L"EmptyName";

			case XEResult::GameResourceManagerNull:
				return L"GameResourceManagerNull";

			case XEResult::ResourceIsManaged:
				return L"ResourceIsManaged";

			case XEResult::ResourceManagedFailed:
				return L"ResourceManagedFailed";

			case XEResult::ResourceManagedTypeError:
				return L"ResourceManagedTypeError";

			case XEResult::CreateSRViewFailed:
				return L"CreateSRViewFailed";

			case XEResult::CreateRTViewFailed:
				return L"CreateRTViewFailed";

			case XEResult::CreateTextureFailed:
				return L"CreateTextureFailed";

			case XEResult::CreateDSViewFailed:
				return L"CreateDSViewFailed";

			case XEResult::MaximunItemType:
				return L"MaximunItemType";

			case XEResult::InvalidObjType:
				return L"InvalidObjType";

			case XEResult::ZeroSize:
				return L"ZeroSize";

			case XEResult::InvalidTextureType:
				return L"InvalidTextureType";

			case XEResult::InvalidShaderType:
				return L"InvalidShaderType";

			case XEResult::SetConstantBufferFailed:
				return L"SetConstantBufferFailed";

			case XEResult::SetSimpleBufferFailed:
				return L"SetSimpleBufferFailed";

			case XEResult::SetStructuredBufferFailed:
				return L"SetStructuredBufferFailed";

			case XEResult::SetVertexShaderFailed:
				return L"SetVertexShaderFailed";

			case XEResult::SetPixelShaderFailed:
				return L"SetPixelShaderFailed";

			case XEResult::SetGeometryShaderFailed:
				return L"SetGeometryShaderFailed";

			case XEResult::SetDomainShaderFailed:
				return L"SetDomainShaderFailed";

			case XEResult::SetHullShaderFailed:
				return L"SetHullShaderFailed";

			case XEResult::SetComputeShaderFailed:
				return L"SetComputeShaderFailed";

			case XEResult::SetVertexShaderPropsFailed:
				return L"SetVertexShaderPropsFailed";

			case XEResult::SetPixelShaderPropsFailed:
				return L"SetPixelShaderPropsFailed";

			case XEResult::SetGeometryShaderPropsFailed:
				return L"SetGeometryShaderPropsFailed";

			case XEResult::SetDomainShaderPropsFailed:
				return L"SetDomainShaderPropsFailed";

			case XEResult::SetHullShaderPropsFailed:
				return L"SetHullShaderPropsFailed";

			case XEResult::SetComputeShaderPropsFailed:
				return L"SetComputeShaderPropsFailed";

			case XEResult::ShaderTextureBindingInitFailed:
				return L"ShaderTextureBindingInitFailed";

			case XEResult::ShaderStructuredBufferInitFailed:
				return L"ShaderStructuredBufferInitFailed";

			case XEResult::ShaderSimpleBufferInitFailed:
				return L"ShaderSimpleBufferInitFailed";

			case XEResult::UnknownFileExtension:
				return L"UnknownFileExtension";

			case XEResult::EmptyFileExtension:
				return L"EmptyFileExtension";

			case XEResult::UnknownContentType:
				return L"UnknownContentType";

			case XEResult::GOCIsAssociated:
				return L"GOCIsAssociated";

			case XEResult::GOCIsNotAssociated:
				return L"GOCIsNotAssociated";

			case XEResult::GameObjectManagerNull:
				return L"GameObjectManagerNull";

			case XEResult::LocalizationManagerNull:
				return L"LocalizationManagerNull";

			case XEResult::LoggerNull:
				return L"LoggerNull";

			case XEResult::ImportFail:
				return L"ImportFail";

			case XEResult::InvalidFileHeader:
				return L"InvalidFileHeader";

			case XEResult::InvalidShaderModel:
				return L"InvalidShaderModel";

			case XEResult::ShaderCompiledFail:
				return L"ShaderCompiledFail";

			case XEResult::ShaderExtractSimpleBuffersFail:
				return L"ShaderExtractSimpleBuffersFail";

			case XEResult::ShaderExtractStructuredBuffersFail:
				return L"ShaderExtractStructuredBuffersFail";

			case XEResult::ShaderExtractConstantBuffersFail:
				return L"ShaderExtractConstantBuffersFail";

			case XEResult::ShaderExtractTextureInputFail:
				return L"ShaderExtractTextureInputFail";

			case XEResult::ShaderTypeMismatch:
				return L"ShaderTypeMismatch";

			case XEResult::FileNotPresent:
				return L"FileNotPresent";

			case XEResult::GameAssetManagerNull:
				return L"GameAssetManagerNull"; 

			case XEResult::GameAssetNotLoaded:
				return L"GameAssetNotLoaded"; 

			case XEResult::GameAssetIDInUse:
				return L"GameAssetIDInUse";

			case XEResult::GameAssetInvalidID:
				return L"GameAssetInvalidID";

			case XEResult::ShaderNull:
				return L"ShaderNull";

			case XEResult::InvalidResourceMapType:
				return L"InvalidResourceMapType";

			case XEResult::ResourceMapFailed:
				return L"ResourceMapFailed";

			case XEResult::InvalidIndexBufferType:
				return L"InvalidIndexBufferType";

			case XEResult::FailIndexBufferUpdate:
				return L"FailIndexBufferUpdate";

			case XEResult::FailIndexBufferInit:
				return L"FailIndexBufferInit";

			case XEResult::FullContainer:
				return L"FullContainer"; 

			case XEResult::LightManagerNull:
				return L"LightManagerNull";

			case XEResult::CameraNull:
				return L"CameraNull";

			case XEResult::Ok:
				return L"Ok";

			default:
				XEAssert(false);
				return L"N/A";
		}
	}
}
