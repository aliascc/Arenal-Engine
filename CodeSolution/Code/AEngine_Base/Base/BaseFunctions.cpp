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

/*************************
*   Precompiled Header   *
**************************/
#include "precomp_base.h"

/**********************
*   System Includes   *
***********************/

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
#define __AE_VERSION_MAJOR       0
#define __AE_VERSION_MINOR       5
#define __AE_VERSION_REVISION    1

/****************
*   Constants   *
*****************/
namespace AE_Base 
{    
    const uint32_t      AE_VERSION_MAYOR    = __AE_VERSION_MAJOR;
    const uint32_t      AE_VERSION_MINOR    = __AE_VERSION_MINOR;
    const uint32_t      AE_VERSION_REVISION = __AE_VERSION_REVISION;
    const std::string   AE_VERSION          = AESTR(__AE_VERSION_MAJOR.__AE_VERSION_MINOR.__AE_VERSION_REVISION);
    const std::string   AE_CODENAME         = "Star Dust";

/****************************
*    Static Variables init   *
*****************************/

    /// <summary>
    /// Unique ID Counter
    /// </summary>
    static uint64_t     AE_UNIQUE_ID            = 1;

    /// <summary>
    /// Mutex to make GetNextUniqueID thread safe and guarantee that no ID
    /// will be the same
    /// </summary>
    static std::mutex   AE_UNIQUE_ID_MUTEX;

/********************
*   Function Defs   *
*********************/

    std::string GetRelativePath(const std::string& filepath)
    {
        std::string relativePath = "./";

        boost::filesystem::path boostFilepath(filepath);

        relativePath += boost::filesystem::make_relative(boost::filesystem::current_path(), boostFilepath).string();

        return relativePath;
    }

    void SplitWString(const std::wstring& str, std::vector<std::wstring>& tokens, const std::wstring& regex, bool trimEmpty)
    {
        tokens.clear();

        std::wregex regexVar(regex);

        std::wsregex_token_iterator it  = std::wsregex_token_iterator(str.begin(), str.end(), regexVar, -1);
        std::wsregex_token_iterator end = std::wsregex_token_iterator();

        for (; it != end; ++it)
        {
            if (trimEmpty && it->str().empty())
            {
                continue;
            }
            tokens.push_back(it->str());
        }
    };

    void SplitString(const std::string& str, std::vector<std::string>& tokens, const std::string& regex, bool trimEmpty)
    {
        tokens.clear();

        std::regex regexVar(regex);

        std::sregex_token_iterator it   = std::sregex_token_iterator(str.begin(), str.end(), regexVar, -1);
        std::sregex_token_iterator end  = std::sregex_token_iterator();

        for (; it != end; ++it)
        {
            if (trimEmpty && it->str().empty())
            {
                continue;
            }
            tokens.push_back(it->str());
        }
    };

    AEResult GetFileModifiedTime(const std::string& fileName, TimeStamp& timeStamp)
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

        timeStamp.m_Day     = clock.tm_mday;
        timeStamp.m_Month   = clock.tm_mon;
        timeStamp.m_Year    = clock.tm_year + 1900;
        timeStamp.m_Hour    = clock.tm_hour;
        timeStamp.m_Minute  = clock.tm_min;
        timeStamp.m_Second  = clock.tm_sec;

        return AEResult::Ok;
    }

    void ConsolePrintLine(const std::string& msg)
    {
        std::cout << msg << std::endl;
    }

    void ConsolePrint(const std::string& msg)
    {
        std::cout << msg;
    }

    uint64_t GetNextUniqueID()
    {
        std::lock_guard<std::mutex> lock(AE_UNIQUE_ID_MUTEX);
        
        return AE_UNIQUE_ID++;
    }

    std::string GetAEResultString(AEResult result)
    {
        switch (result)
        {
            case AEResult::Fail:
                return "Fail";

            case AEResult::NotReady:
                return "NotReady";

            case AEResult::GraphicDeviceNull:
                return "GraphicDeviceNull";

            case AEResult::NullParameter:
                return "NullParameter";

            case AEResult::EmptyFilename:
                return "EmptyFilename";

            case AEResult::CreateDXDeviceFail:
                return "CreateDXDeviceFail";

            case AEResult::CreateDXConfFail:
                return "CreateDXConfFail";

            case AEResult::FeatureLvlUnsupported:
                return "FeatureLvlUnsupported";

            case AEResult::CreateDXSwapChainFail:
                return "CreateDXSwapChainFail";

            case AEResult::CreateDXDefaultRTDSFail:
                return "CreateDXDefaultRTDSFail";

            case AEResult::InitViewportFail:
                return "InitViewportFail";

            case AEResult::RegGlobalPropFail:
                return "RegGlobalPropFail";

            case AEResult::RegGlobalFuncFail:
                return "RegGlobalFuncFail";

            case AEResult::RegObjTypeFail:
                return "RegObjTypeFail";

            case AEResult::RegEnumValFail:
                return "RegEnumValFail";

            case AEResult::RegEnumFail:
                return "RegEnumFail";

            case AEResult::RegObjMethodFail:
                return "RegObjMethodFail";

            case AEResult::RegObjPropFail:
                return "RegObjPropFail";

            case AEResult::RegObjBehaviorFail:
                return "RegObjBehaviorFail";

            case AEResult::RegInterfaceFail:
                return "RegInterfaceFail";

            case AEResult::OpenFileFail:
                return "OpenFileFail";

            case AEResult::NotFound:
                return "NotFound";

            case AEResult::XMLReadError:
                return "XMLReadError";

            case AEResult::InitLoggerFail:
                return "InitLoggerFail";

            case AEResult::Log2FileActive:
                return "Log2FileActive";

            case AEResult::ASModuleNotFound:
                return "ASModuleNotFound";

            case AEResult::ASModuleAlreadyCreated:
                return "ASModuleAlreadyCreated";

            case AEResult::ASModuleCreateFail:
                return "ASModuleCreateFail";

            case AEResult::ASAddSecModuleFail:
                return "ASAddSecModuleFail";

            case AEResult::ASBuildModuleFail:
                return "ASBuildModuleFail";

            case AEResult::ASDiscardModuleFail:
                return "ASDiscardModuleFail";

            case AEResult::ASPrepareContextFailed:
                return "ASPrepareContextFailed";

            case AEResult::ASSetArgObjectFailed:
                return "ASSetArgObjectFailed";

            case AEResult::KeyboardLockFailed:
                return "KeyboardLockFailed";

            case AEResult::KeyboardUnLockFailed:
                return "KeyboardUnLockFailed";

            case AEResult::StructuredBufferInitFailed:
                return "StructuredBufferInitFailed";

            case AEResult::StructuredBufferSetDateFailed:
                return "StructuredBufferSetDateFailed";

            case AEResult::SimpleBufferInitFailed:
                return "SimpleBufferInitFailed";

            case AEResult::SimpleBufferSetDateFailed:
                return "SimpleBufferSetDateFailed";

            case AEResult::ConstantBufferInitFailed:
                return "ConstantBufferInitFailed";

            case AEResult::ConstantBufferSetDataFailed:
                return "ConstantBufferSetDataFailed";

            case AEResult::VertexShaderLoadFailed:
                return "VertexShaderLoadFailed";

            case AEResult::PixelShaderLoadFailed:
                return "PixelShaderLoadFailed";

            case AEResult::HullShaderLoadFailed:
                return "HullShaderLoadFailed";

            case AEResult::DomainShaderLoadFailed:
                return "DomainShaderLoadFailed";

            case AEResult::GeometryShaderLoadFailed:
                return "GeometryShaderLoadFailed";

            case AEResult::ComputeShaderLoadFailed:
                return "ComputeShaderLoadFailed";

            case AEResult::EmptyString:
                return "EmptyString";

            case AEResult::ObjExists:
                return "ObjExists";

            case AEResult::InvalidPermutation:
                return "InvalidPermutation";

            case AEResult::SetTextureFailed:
                return "SetTextureFailed";

            case AEResult::NullObj:
                return "NullObj";

            case AEResult::OutsideRange:
                return "OutsideRange";

            case AEResult::LoadFileFailed:
                return "LoadFileFailed";

            case AEResult::AssimpProcessGraph:
                return "AssimpProcessGraph";

            case AEResult::AssimpProcessMaterials:
                return "AssimpProcessMaterials";

            case AEResult::AssimpProcessEmbeddedTextures:
                return "AssimpProcessEmbeddedTextures";

            case AEResult::AssimpProcessMeshes:
                return "AssimpProcessMeshes";

            case AEResult::AssimpProcessSkeleton:
                return "AssimpProcessSkeleton";

            case AEResult::AssimpProcessAnimation:
                return "AssimpProcessAnimation";

            case AEResult::AssimpProcessConstructModel:
                return "AssimpProcessConstructModel";

            case AEResult::EmptySkeleton:
                return "EmptySkeleton";

            case AEResult::EmptyName:
                return "EmptyName";

            case AEResult::GameResourceManagerNull:
                return "GameResourceManagerNull";

            case AEResult::ResourceIsManaged:
                return "ResourceIsManaged";

            case AEResult::ResourceManagedFailed:
                return "ResourceManagedFailed";

            case AEResult::ResourceManagedTypeError:
                return "ResourceManagedTypeError";

            case AEResult::CreateSRViewFailed:
                return "CreateSRViewFailed";

            case AEResult::CreateRTViewFailed:
                return "CreateRTViewFailed";

            case AEResult::CreateTextureFailed:
                return "CreateTextureFailed";

            case AEResult::CreateDSViewFailed:
                return "CreateDSViewFailed";

            case AEResult::MaximunItemType:
                return "MaximunItemType";

            case AEResult::InvalidObjType:
                return "InvalidObjType";

            case AEResult::ZeroSize:
                return "ZeroSize";

            case AEResult::InvalidTextureType:
                return "InvalidTextureType";

            case AEResult::InvalidShaderType:
                return "InvalidShaderType";

            case AEResult::SetConstantBufferFailed:
                return "SetConstantBufferFailed";

            case AEResult::SetSimpleBufferFailed:
                return "SetSimpleBufferFailed";

            case AEResult::SetStructuredBufferFailed:
                return "SetStructuredBufferFailed";

            case AEResult::SetVertexShaderFailed:
                return "SetVertexShaderFailed";

            case AEResult::SetPixelShaderFailed:
                return "SetPixelShaderFailed";

            case AEResult::SetGeometryShaderFailed:
                return "SetGeometryShaderFailed";

            case AEResult::SetDomainShaderFailed:
                return "SetDomainShaderFailed";

            case AEResult::SetHullShaderFailed:
                return "SetHullShaderFailed";

            case AEResult::SetComputeShaderFailed:
                return "SetComputeShaderFailed";

            case AEResult::SetVertexShaderPropsFailed:
                return "SetVertexShaderPropsFailed";

            case AEResult::SetPixelShaderPropsFailed:
                return "SetPixelShaderPropsFailed";

            case AEResult::SetGeometryShaderPropsFailed:
                return "SetGeometryShaderPropsFailed";

            case AEResult::SetDomainShaderPropsFailed:
                return "SetDomainShaderPropsFailed";

            case AEResult::SetHullShaderPropsFailed:
                return "SetHullShaderPropsFailed";

            case AEResult::SetComputeShaderPropsFailed:
                return "SetComputeShaderPropsFailed";

            case AEResult::ShaderTextureBindingInitFailed:
                return "ShaderTextureBindingInitFailed";

            case AEResult::ShaderStructuredBufferInitFailed:
                return "ShaderStructuredBufferInitFailed";

            case AEResult::ShaderSimpleBufferInitFailed:
                return "ShaderSimpleBufferInitFailed";

            case AEResult::UnknownFileExtension:
                return "UnknownFileExtension";

            case AEResult::EmptyFileExtension:
                return "EmptyFileExtension";

            case AEResult::UnknownContentType:
                return "UnknownContentType";

            case AEResult::GOCIsAssociated:
                return "GOCIsAssociated";

            case AEResult::GOCIsNotAssociated:
                return "GOCIsNotAssociated";

            case AEResult::GameObjectManagerNull:
                return "GameObjectManagerNull";

            case AEResult::LocalizationManagerNull:
                return "LocalizationManagerNull";

            case AEResult::LoggerNull:
                return "LoggerNull";

            case AEResult::ImportFail:
                return "ImportFail";

            case AEResult::InvalidFileHeader:
                return "InvalidFileHeader";

            case AEResult::InvalidShaderModel:
                return "InvalidShaderModel";

            case AEResult::ShaderCompiledFail:
                return "ShaderCompiledFail";

            case AEResult::ShaderExtractSimpleBuffersFail:
                return "ShaderExtractSimpleBuffersFail";

            case AEResult::ShaderExtractStructuredBuffersFail:
                return "ShaderExtractStructuredBuffersFail";

            case AEResult::ShaderExtractConstantBuffersFail:
                return "ShaderExtractConstantBuffersFail";

            case AEResult::ShaderExtractTextureInputFail:
                return "ShaderExtractTextureInputFail";

            case AEResult::ShaderTypeMismatch:
                return "ShaderTypeMismatch";

            case AEResult::FileNotPresent:
                return "FileNotPresent";

            case AEResult::GameAssetManagerNull:
                return "GameAssetManagerNull"; 

            case AEResult::GameAssetNotLoaded:
                return "GameAssetNotLoaded";

            case AEResult::GameAssetIDInUse:
                return "GameAssetIDInUse";

            case AEResult::GameAssetInvalidID:
                return "GameAssetInvalidID";

            case AEResult::RawGameAssetNotFound:
                return "RawGameAssetNotFound";

            case AEResult::ShaderNull:
                return "ShaderNull";

            case AEResult::InvalidResourceMapType:
                return "InvalidResourceMapType";

            case AEResult::ResourceMapFailed:
                return "ResourceMapFailed";

            case AEResult::InvalidIndexBufferType:
                return "InvalidIndexBufferType";

            case AEResult::FailIndexBufferUpdate:
                return "FailIndexBufferUpdate";

            case AEResult::FailIndexBufferInit:
                return "FailIndexBufferInit";

            case AEResult::FullContainer:
                return "FullContainer"; 

            case AEResult::LightManagerNull:
                return "LightManagerNull";

            case AEResult::CameraNull:
                return "CameraNull";

            case AEResult::WriteToFileFailed:
                return "WriteToFileFailed";

            case AEResult::ConfigLoadError:
                return "ConfigLoadError";

            case AEResult::ExceptionHandledFailed:
                return "ExceptionHandledFailed";

            case AEResult::Ok:
                return "Ok";

            default:
                AEAssert(false);
                return "N/A";
        }
    }
}
