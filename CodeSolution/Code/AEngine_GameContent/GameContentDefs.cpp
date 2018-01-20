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

/*************************
*   3rd Party Includes   *
**************************/
#include "boost\algorithm\string.hpp"

/***************************
*   Game Engine Includes   *
****************************/
#include "GameContentDefs.h"
#include "Base\BaseFunctions.h"

//Always include last
#include "Memory\MemLeaks.h"

/************
*   Using   *
*************/

/********************
*   Function Defs   *
*********************/

/***************************
*   AEGameContentHelpers   *
****************************/
namespace AEGameContentHelpers
{
    std::wstring GameContentTypeToString(GameContentType gameContentType)
    {
        switch (gameContentType)
        {
            case GameContentType::Unknown:
                return L"Unknown";

            case GameContentType::Model:
                return L"Model";

            case GameContentType::Mesh:
                return L"Mesh";

            case GameContentType::Animation:
                return L"Animation";

            case GameContentType::Skeleton:
                return L"Skeleton";

            case GameContentType::Texture:
                return L"Texture";

            case GameContentType::Shader:
                return L"Shader";

            case GameContentType::GameObjectScript:
                return L"Game Object Script";

            case GameContentType::Audio:
                return L"Audio";

            default:
                AEAssert(false)
                return L"";
        }
    }

    std::wstring GameContentSubtypeToString(GameContentSubtype gameContentSubtype)
    {
        switch (gameContentSubtype)
        {
            case GameContentSubtype::Unknown:
                return L"Unknown";

            case GameContentSubtype::None:
                return L"None";

            case GameContentSubtype::AnimationClip:
                return L"Animation Clip";

            case GameContentSubtype::VertexShaderHLSL:
                return L"Vertex Shader HLSL";

            case GameContentSubtype::PixelShaderHLSL:
                return L"Pixel Shader HLSL";

            case GameContentSubtype::GeometryShaderHLSL:
                return L"Geometry Shader HLSL";

            case GameContentSubtype::ComputeShaderHLSL:
                return L"Compute Shader HLSL";

            case GameContentSubtype::DomainShaderHLSL:
                return L"Domain Shader HLSL";

            case GameContentSubtype::HullShaderHLSL:
                return L"Hull Shader HLSL";

            case GameContentSubtype::Texture2D:
                return L"Texture 2D";

            case GameContentSubtype::TextureCube:
                return L"Texture Cube";

            default:
                AEAssert(false);
                return L"";
        }
    }

    AEResult FileGameContentType(const std::wstring& file, GameContentType& contentType, GameContextFileExt& fileExt)
    {
        contentType = GameContentType::Unknown;

        AEAssert(!file.empty());
        if (file.empty())
        {
            return AEResult::EmptyFilename;
        }

        std::wstring ext = AE_Base::GetFilenameExtension(file);

        if (ext.empty())
        {
            return AEResult::EmptyFileExtension;
        }

        boost::algorithm::to_lower(ext);

        if (ext.compare(AE_CT_MODEL_COLLADA_EXT) == 0)
        {
            contentType = GameContentType::Model;
            fileExt = GameContextFileExt::ModelCollada;

            return AEResult::Ok;
        }

        if (ext.compare(AE_CT_SHADER_HLSL_1_EXT) == 0)
        {
            contentType = GameContentType::Shader;
            fileExt = GameContextFileExt::ShaderFX;

            return AEResult::Ok;
        }

        if (ext.compare(AE_CT_SHADER_HLSL_2_EXT) == 0)
        {
            contentType = GameContentType::Shader;
            fileExt = GameContextFileExt::ShaderHLSL;

            return AEResult::Ok;
        }
        
        if (ext.compare(AE_CT_TEXTURE_DDS_EXT) == 0)
        {
            contentType = GameContentType::Texture;
            fileExt = GameContextFileExt::TextureDDS;

            return AEResult::Ok;
        }
        
        if (ext.compare(AE_CT_GAMEOBJECT_SCRIPT_EXT) == 0)
        {
            contentType = GameContentType::GameObjectScript;
            fileExt = GameContextFileExt::GameObjectScript;

            return AEResult::Ok;
        }

        if (ext.compare(AE_CT_AUDIO_1_EXT) == 0)
        {
            contentType = GameContentType::Audio;
            fileExt = GameContextFileExt::AudioWav;

            return AEResult::Ok;
        }

        if (ext.compare(AE_CT_AUDIO_2_EXT) == 0)
        {
            contentType = GameContentType::Audio;
            fileExt = GameContextFileExt::AudioOgg;

            return AEResult::Ok;
        }

        return AEResult::UnknownFileExtension;
    }
        
    AEResult FileShaderyType(const std::wstring& file, ShaderType& shaderType)
    {
        AEAssert(!file.empty());

        if(file.empty())
        {
            return AEResult::EmptyFilename;
        }

        std::wstring ext = AE_Base::GetFilenameExtension(file);

        if(ext.empty())
        {
            return AEResult::EmptyFileExtension;
        }

        if(ext.compare(AE_CT_SHADER_HLSL_VS_EXT) == 0)
        {
            shaderType = ShaderType::VertexShader;
        }
        else if(ext.compare(AE_CT_SHADER_HLSL_PS_EXT) == 0)
        {
            shaderType = ShaderType::PixelShader;
        }
        else if(ext.compare(AE_CT_SHADER_HLSL_GS_EXT) == 0)
        {
            shaderType = ShaderType::GeometryShader;
        }
        else if(ext.compare(AE_CT_SHADER_HLSL_DS_EXT) == 0)
        {
            shaderType = ShaderType::DomainShader;
        }
        else if(ext.compare(AE_CT_SHADER_HLSL_HS_EXT) == 0)
        {
            shaderType = ShaderType::HullShader;
        }
        else if(ext.compare(AE_CT_SHADER_HLSL_CS_EXT) == 0)
        {
            shaderType = ShaderType::ComputeShader;
        }
        else
        {
            return AEResult::UnknownFileExtension;
        }

        return AEResult::Ok;
    }

    void WriteFileHeader(std::ofstream& fileStream, uint16_t fileHeader, uint32_t mayorVersion, uint32_t minorVersion, uint32_t revisionVersion)
    {
        /////////////////////////////////////////////
        //Start by writing the header
        const char* tempPtr = reinterpret_cast<const char*>(&fileHeader);
        uint32_t sizeToWrite = sizeof(uint16_t);
        fileStream.write(tempPtr, sizeToWrite);

        /////////////////////////////////////////////
        //Write File version
        sizeToWrite = sizeof(uint32_t);
    
        tempPtr = reinterpret_cast<const char*>(&mayorVersion);
        fileStream.write(tempPtr, sizeToWrite);
    
        tempPtr = reinterpret_cast<const char*>(&minorVersion);
        fileStream.write(tempPtr, sizeToWrite);
    
        tempPtr = reinterpret_cast<const char*>(&revisionVersion);
        fileStream.write(tempPtr, sizeToWrite);
    }

    void WriteFileFooter(std::ofstream& fileStream, uint16_t fileFooter)
    {
        const char* tempPtr = reinterpret_cast<const char*>(&fileFooter);
        uint32_t sizeToWrite = sizeof(uint16_t);
        fileStream.write(tempPtr, sizeToWrite);
    }

    bool ReadFileHeaderAndVerify(std::ifstream& fileStream, uint16_t fileHeader, uint32_t mayorVersion, uint32_t minorVersion, uint32_t revisionVersion)
    {
        char* tempPtr = nullptr;
        uint32_t sizeToRead = 0;
    
        /////////////////////////////////////////////
        //Start by reading the header
        uint16_t fileHeaderFile = 0;
    
        tempPtr = reinterpret_cast<char*>(&fileHeaderFile);
        sizeToRead = sizeof(uint16_t);
        fileStream.read(tempPtr, sizeToRead);

        //Compare that this is a file we want
        if(fileHeaderFile != fileHeader)
        {
            return false;
        }
    
        /////////////////////////////////////////////
        //Read File Version
        uint32_t fileMayorFile = 0;
        uint32_t fileMinorFile = 0;
        uint32_t fileRevisionFile = 0;
        sizeToRead = sizeof(uint32_t);
    
        tempPtr = reinterpret_cast<char*>(&fileMayorFile);
        fileStream.read(tempPtr, sizeToRead);
    
        tempPtr = reinterpret_cast<char*>(&fileMinorFile);
        fileStream.read(tempPtr, sizeToRead);
    
        tempPtr = reinterpret_cast<char*>(&fileRevisionFile);
        fileStream.read(tempPtr, sizeToRead);

        //Compare file Versions

        //Mayor file versions have different layouts
        //can not read.
        if(fileMayorFile != mayorVersion)
        {
            return false;
        }

        //Minor File version has certain changes we should
        //check against each case
        if(fileMinorFile != minorVersion)
        {
            return false;
        }

        //File revisions only have fix bugs that should not
        //change the layout of the file and can be imported

        return true;
    }

    bool ReadFileFooterAndVerify(std::ifstream& fileStream, uint16_t fileFooter)
    {
        /////////////////////////////////////////////
        //Finish by reading the footer
        uint16_t fileFooterFile = 0;

        char* tempPtr = reinterpret_cast<char*>(&fileFooterFile);
        uint32_t sizeToRead = sizeof(uint16_t);
        fileStream.read(tempPtr, sizeToRead);

        if(fileFooterFile != fileFooter)
        {
            return false;
        }

        return true;
    }

    void WriteString(std::ofstream& fileStream, const std::wstring& str)
    {
        const char* tempPtr = nullptr;
        uint32_t size = 0;
        uint32_t sizeToWrite = 0;

        //size of name
        size        = (uint32_t)str.size();
        tempPtr        = reinterpret_cast<const char*>(&size);
        sizeToWrite = sizeof(uint32_t);
        fileStream.write(tempPtr, sizeToWrite);

        if(size != 0)
        {
            //Name
            tempPtr        = reinterpret_cast<const char*>(str.c_str());
            sizeToWrite    = sizeof(wchar_t) * size;
            fileStream.write(tempPtr, sizeToWrite);
        }
    }

    std::wstring ReadString(std::ifstream& fileStream)
    {
        std::wstring str = L"";

        char* tempPtr = nullptr;
        uint32_t size = 0;
        uint32_t sizeToRead = 0;

        //Get Size of name
        tempPtr         = reinterpret_cast<char*>(&size);
        sizeToRead      = sizeof(uint32_t);
        fileStream.read(tempPtr, sizeToRead);

        if(size != 0)
        {
            //Get Name
            tempPtr         = new char[sizeof(wchar_t) * (size + 1)];
            sizeToRead      = sizeof(wchar_t) * size;
            memset(tempPtr, 0, sizeof(wchar_t) * (size + 1));
            fileStream.read(tempPtr, sizeToRead);

            str = std::wstring(reinterpret_cast<wchar_t*>(tempPtr));

            DeleteMemArr(tempPtr);
        }
    
        return str;
    }

}
