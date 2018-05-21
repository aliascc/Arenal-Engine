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
#include "precomp_graphics.h"

/**********************
*   System Includes   *
***********************/

/*************************
*   3rd Party Includes   *
**************************/

/***************************
*   Game Engine Includes   *
****************************/
#include "Shader.h"
#include "GraphicDevice.h"
#include "GameContentDefs.h"

//Always include last
#include "Memory\MemLeaks.h"

/********************
*   Function Defs   *
*********************/
Shader::Shader(GraphicDevice* graphicDevice, ShaderType shaderType, const std::string& name)
    : GameResource(name, GameResourceType::Unknown)
    , m_GraphicDevice(graphicDevice)
    , m_ShaderType(shaderType)
{
    AEAssert(graphicDevice != nullptr);

    switch (shaderType)
    {
        case ShaderType::VertexShader:
            m_ResourceType = GameResourceType::VertexShader;
            break;
        case ShaderType::PixelShader:
            m_ResourceType = GameResourceType::PixelShader;
            break;
        case ShaderType::GeometryShader:
            m_ResourceType = GameResourceType::GeometryShader;
            break;
        case ShaderType::HullShader:
            m_ResourceType = GameResourceType::HullShader;
            break;
        case ShaderType::ComputeShader:
            m_ResourceType = GameResourceType::ComputeShader;
            break;
        case ShaderType::DomainShader:
            m_ResourceType = GameResourceType::DomainShader;
            break;
        default:
            AEAssert(false);
            break;
    }
}

Shader::~Shader()
{
}

AEResult Shader::LoadShader(const BYTE* shaderByteCode, uint32_t length)
{
    AEAssert(shaderByteCode != nullptr);
    if(shaderByteCode == nullptr)
    {
        return AEResult::NullParameter;
    }

    std::lock_guard<std::mutex> lock(m_GameResourceMutex);

    return LoadShaderWithoutLock(shaderByteCode, length);
}

AEResult Shader::Load()
{
    AEAssert(m_GraphicDevice != nullptr);
    if(m_GraphicDevice == nullptr)
    {
        return AEResult::GraphicDeviceNull;
    }

    std::lock_guard<std::mutex> lock(m_GameResourceMutex);

    AEAssert(!m_FileName.empty());
    if(m_FileName.empty())
    {
        return AEResult::EmptyFilename;
    }

    /////////////////////////////////////////////
    //Start reading file
    std::ifstream shaderFile;

    shaderFile.open(m_FileName, std::ios::binary | std::ios::in);

    if(!shaderFile.is_open())
    {
        AETODO("add log");

        return AEResult::OpenFileFail;
    }

    char* tempPtr = nullptr;
    uint32_t sizeToRead = 0;
    AEResult ret = AEResult::Ok;

    /////////////////////////////////////////////
    //Verify Header
    bool verifyHeader = AEGameContentHelpers::ReadFileHeaderAndVerify(shaderFile, AE_CT_AEHFX_FILE_HEADER, AE_CT_AEHFX_FILE_VERSION_MAYOR, AE_CT_AEHFX_FILE_VERSION_MINOR, AE_CT_AEHFX_FILE_VERSION_REVISON);

    if(!verifyHeader)
    {
        AETODO("Add log");

        return AEResult::InvalidFileHeader;
    }

    /////////////////////////////////////////////
    //Read Shader Type and Verify
    ShaderType shaderTypeFile = ShaderType::VertexShader;
    tempPtr = reinterpret_cast<char*>(&shaderTypeFile);
    sizeToRead = sizeof(ShaderType);
    shaderFile.read(tempPtr, sizeToRead);

    if(m_ShaderType != shaderTypeFile)
    {
        AETODO("add log");

        return AEResult::ShaderTypeMismatch;
    }

    /////////////////////////////////////////////
    //Read Name of Shader
    this->SetNameWithoutLock(AEGameContentHelpers::ReadString(shaderFile));
    
    /////////////////////////////////////////////
    //Read Shader Byte Code
    BYTE* shaderByteCode = nullptr;
    uint32_t shaderByteCodeSize = 0;
    ret = ReadShaderByteCode(shaderFile, &shaderByteCode, shaderByteCodeSize);

    if(ret != AEResult::Ok)
    {
        return ret;
    }

    /////////////////////////////////////////////
    //Read Shader Constant Buffers
    ConstantBufferSignatureList cbSigList;
    ret = ReadShaderConstantBuffers(shaderFile, cbSigList);

    if(ret != AEResult::Ok)
    {
        DeleteMemArr(shaderByteCode);
        return ret;
    }

    /////////////////////////////////////////////
    //Read Shader Constant Buffers
    SimpleBufferSignatureList simpleBufferSigList;
    ret = ReadShaderSimpleBuffers(shaderFile, simpleBufferSigList);

    if(ret != AEResult::Ok)
    {
        DeleteMemArr(shaderByteCode);
        return ret;
    }

    /////////////////////////////////////////////
    //Read Shader Structured Buffers
    StructuredBufferSignatureList structuredBufferSigList;
    ret = ReadShaderStructuredBuffers(shaderFile, structuredBufferSigList);

    if(ret != AEResult::Ok)
    {
        DeleteMemArr(shaderByteCode);
        return ret;
    }

    /////////////////////////////////////////////
    //Read Shader Texture Inputs
    TextureInputSignatureList texInputSigList;
    ret = ReadShaderTextureInputs(shaderFile, texInputSigList);

    if(ret != AEResult::Ok)
    {
        DeleteMemArr(shaderByteCode);
        return ret;
    }

    /////////////////////////////////////////////
    //Read Shader Texture Array Inputs
    TextureArrayInputSignatureList texArrayInputSigList;
    ret = ReadShaderTextureArrayInputs(shaderFile, texArrayInputSigList);

    if (ret != AEResult::Ok)
    {
        DeleteMemArr(shaderByteCode);
        return ret;
    }

    /////////////////////////////////////////////
    //Read Shader Samplers
    SamplerSignatureList samplerSigList;
    ret = ReadShaderSamplers(shaderFile, samplerSigList);

    if (ret != AEResult::Ok)
    {
        DeleteMemArr(shaderByteCode);
        return ret;
    }

    /////////////////////////////////////////////
    //Read Footer 
    bool verifyFooter = AEGameContentHelpers::ReadFileFooterAndVerify(shaderFile, AE_CT_AEHFX_FILE_FOOTER);
    if(!verifyFooter)
    {
        AETODO("Add Warning log");
    }

    /////////////////////////////////////////////
    //Close File
    shaderFile.close();

    /////////////////////////////////////////////
    //Load Shader Into Memory and Set Signature Lists
    ret = this->LoadShaderWithoutLock(shaderByteCode, shaderByteCodeSize);
    
    DeleteMemArr(shaderByteCode);

    if(ret != AEResult::Ok)
    {
        return ret;
    }

    m_ConstantBufferSignatureList       = cbSigList;
    m_SimpleBufferSignatureList         = simpleBufferSigList;
    m_StructuredBufferSignatureList     = structuredBufferSigList;
    m_TextureInputSignatureList         = texInputSigList;
    m_TextureArrayInputSignatureList    = texArrayInputSigList;
    m_SamplerSignatureList              = samplerSigList;

    /////////////////////////////////////////////
    //Finish
    return AEResult::Ok;
}

AEResult Shader::ReadShaderByteCode(std::ifstream& fileStream, BYTE** shaderByteCode, uint32_t& shaderByteCodeSize)
{
    AEAssert(shaderByteCode != nullptr);

    if(shaderByteCode == nullptr)
    {
        return AEResult::NullParameter;
    }

    char* tempPtr = nullptr;
    uint32_t sizeToRead = 0;

    /////////////////////////////////////////////
    //Read Shader Byte Code Size and Memory
    uint32_t byteCodeSize = 0;
    tempPtr = reinterpret_cast<char*>(&byteCodeSize);
    sizeToRead = sizeof(uint32_t);
    fileStream.read(tempPtr, sizeToRead);

    if(byteCodeSize == 0)
    {
        return AEResult::ZeroSize;
    }
    
    /////////////////////////////////////////////
    //Create Holder for Shader Byte Code
    BYTE* shaderByteCodeTmp = new BYTE[byteCodeSize];
    memset(shaderByteCodeTmp, 0, byteCodeSize);

    tempPtr = reinterpret_cast<char*>(shaderByteCodeTmp);
    sizeToRead = byteCodeSize;
    fileStream.read(tempPtr, sizeToRead);

    /////////////////////////////////////////////
    //Finish
    *shaderByteCode = shaderByteCodeTmp;
    shaderByteCodeSize = byteCodeSize;

    return AEResult::Ok;
}

AEResult Shader::ReadShaderConstantBuffers(std::ifstream& fileStream, ConstantBufferSignatureList& cbSigList)
{
    char* tempPtr = nullptr;
    uint32_t sizeToRead = 0;

    /////////////////////////////////////////////
    //Read Number of Shader Constant Buffers
    uint32_t cbNum = 0;
    tempPtr = reinterpret_cast<char*>(&cbNum);
    sizeToRead = sizeof(uint32_t);
    fileStream.read(tempPtr, sizeToRead);

    /////////////////////////////////////////////
    //Read Constant Buffers
    for (uint32_t i = 0; i < cbNum; i++)
    {
        ConstantBufferSignature cbSig;

        /////////////////////////////////////////////
        //Read Name of Constant Buffer
        cbSig.m_Name = AEGameContentHelpers::ReadString(fileStream);

        /////////////////////////////////////////////
        //Read Bind Index of Constant Buffer
        tempPtr = reinterpret_cast<char*>(&cbSig.m_BindIndex);
        sizeToRead = sizeof(uint32_t);
        fileStream.read(tempPtr, sizeToRead);

        /////////////////////////////////////////////
        //Read Number of Variables in Constant Buffer
        uint32_t varNum = 0;
        tempPtr = reinterpret_cast<char*>(&varNum);
        sizeToRead = sizeof(uint32_t);
        fileStream.read(tempPtr, sizeToRead);

        /////////////////////////////////////////////
        //Read Variables of Constant Buffer
        for (uint32_t j = 0; j < varNum; j++)
        {
            ShaderCustomVariable scVar;

            /////////////////////////////////////////////
            //Read Variable Info

            //1. Name
            scVar.m_Name = AEGameContentHelpers::ReadString(fileStream);
            
            //2. Start Offset
            tempPtr = reinterpret_cast<char*>(&scVar.m_StartOffset);
            sizeToRead = sizeof(uint32_t);
            fileStream.read(tempPtr, sizeToRead);

            //3. Offset        
            tempPtr = reinterpret_cast<char*>(&scVar.m_Offset);
            sizeToRead = sizeof(uint32_t);
            fileStream.read(tempPtr, sizeToRead);

            //4. Size
            tempPtr = reinterpret_cast<char*>(&scVar.m_Size);
            sizeToRead = sizeof(uint32_t);
            fileStream.read(tempPtr, sizeToRead);

            //5. Elements
            tempPtr = reinterpret_cast<char*>(&scVar.m_Elements);
            sizeToRead = sizeof(uint32_t);
            fileStream.read(tempPtr, sizeToRead);

            //6. Is Array
            tempPtr = reinterpret_cast<char*>(&scVar.m_IsArray);
            sizeToRead = sizeof(bool);
            fileStream.read(tempPtr, sizeToRead);

            //7. Element Size
            tempPtr = reinterpret_cast<char*>(&scVar.m_ElementSize);
            sizeToRead = sizeof(uint32_t);
            fileStream.read(tempPtr, sizeToRead);

            //8. User Variable
            tempPtr = reinterpret_cast<char*>(&scVar.m_UserVariable);
            sizeToRead = sizeof(bool);
            fileStream.read(tempPtr, sizeToRead);

            //9. Columns
            tempPtr = reinterpret_cast<char*>(&scVar.m_Columns);
            sizeToRead = sizeof(uint32_t);
            fileStream.read(tempPtr, sizeToRead);

            //10. Rows
            tempPtr = reinterpret_cast<char*>(&scVar.m_Rows);
            sizeToRead = sizeof(uint32_t);
            fileStream.read(tempPtr, sizeToRead);

            //11. Shader Class
            tempPtr = reinterpret_cast<char*>(&scVar.m_ShaderVariableClass);
            sizeToRead = sizeof(ShaderVariableClass);
            fileStream.read(tempPtr, sizeToRead);

            //12. Shader Type
            tempPtr = reinterpret_cast<char*>(&scVar.m_ShaderVariableType);
            sizeToRead = sizeof(ShaderVariableType);
            fileStream.read(tempPtr, sizeToRead);

            /////////////////////////////////////////////
            //Add Shader Variable to Constant Buffer
            cbSig.m_ShaderCustomVariableList.push_back(scVar);
        }

        /////////////////////////////////////////////
        //Add Constant Buffer Signature to Main List
        cbSigList.push_back(cbSig);
    }

    /////////////////////////////////////////////
    //Finish
    return AEResult::Ok;
}

AEResult Shader::ReadShaderSimpleBuffers(std::ifstream& fileStream, SimpleBufferSignatureList& simpleBufferSignatureList)
{
    char* tempPtr = nullptr;
    uint32_t sizeToRead = 0;

    /////////////////////////////////////////////
    //Read Number of Shader Simple Buffers
    uint32_t numSimpleBuffers = 0;
    tempPtr = reinterpret_cast<char*>(&numSimpleBuffers);
    sizeToRead = sizeof(uint32_t);
    fileStream.read(tempPtr, sizeToRead);
    
    /////////////////////////////////////////////
    //Read Simple Buffers
    for (uint32_t i = 0; i < numSimpleBuffers; i++)
    {
        SimpleBufferSignature simpleBufferSignature;

        /////////////////////////////////////////////
        //Read Name of Simple Buffer
        simpleBufferSignature.m_Name = AEGameContentHelpers::ReadString(fileStream);

        /////////////////////////////////////////////
        //Read Bind Index of Simple Buffer
        tempPtr = reinterpret_cast<char*>(&simpleBufferSignature.m_BindIndex);
        sizeToRead = sizeof(uint32_t);
        fileStream.read(tempPtr, sizeToRead);

        /////////////////////////////////////////////
        //Read RW of Simple Buffer
        tempPtr = reinterpret_cast<char*>(&simpleBufferSignature.m_IsRW);
        sizeToRead = sizeof(bool);
        fileStream.read(tempPtr, sizeToRead);
        
        /////////////////////////////////////////////
        //Read Element Count
        tempPtr = reinterpret_cast<char*>(&simpleBufferSignature.m_ElementCount);
        sizeToRead = sizeof(uint32_t);
        fileStream.read(tempPtr, sizeToRead);
        
        /////////////////////////////////////////////
        //Read Shader Variable Class
        tempPtr = reinterpret_cast<char*>(&simpleBufferSignature.m_VariableClass);
        sizeToRead = sizeof(ShaderVariableClass);
        fileStream.read(tempPtr, sizeToRead);
        
        /////////////////////////////////////////////
        //Read Shader Variable Type
        tempPtr = reinterpret_cast<char*>(&simpleBufferSignature.m_VariableType);
        sizeToRead = sizeof(ShaderVariableType);
        fileStream.read(tempPtr, sizeToRead);

        /////////////////////////////////////////////
        //Add Simple Buffer Signature to Main List
        simpleBufferSignatureList.push_back(simpleBufferSignature);
    }

    /////////////////////////////////////////////
    //Finish
    return AEResult::Ok;
}

AEResult Shader::ReadShaderStructuredBuffers(std::ifstream& fileStream, StructuredBufferSignatureList& structuredBufferSigList)
{
    char* tempPtr = nullptr;
    uint32_t sizeToRead = 0;

    /////////////////////////////////////////////
    //Read Number of Shader Structured Buffers
    uint32_t numstructuredBuffers = 0;
    tempPtr = reinterpret_cast<char*>(&numstructuredBuffers);
    sizeToRead = sizeof(uint32_t);
    fileStream.read(tempPtr, sizeToRead);
    
    /////////////////////////////////////////////
    //Read Structured Buffers
    for (uint32_t i = 0; i < numstructuredBuffers; i++)
    {
        StructuredBufferSignature structuredBufferSig;

        /////////////////////////////////////////////
        //Read Name of Structured Buffer
        structuredBufferSig.m_Name = AEGameContentHelpers::ReadString(fileStream);

        /////////////////////////////////////////////
        //Read Bind Index of Structured Buffer
        tempPtr = reinterpret_cast<char*>(&structuredBufferSig.m_BindIndex);
        sizeToRead = sizeof(uint32_t);
        fileStream.read(tempPtr, sizeToRead);

        /////////////////////////////////////////////
        //Read RW of Structured Buffer
        tempPtr = reinterpret_cast<char*>(&structuredBufferSig.m_IsRW);
        sizeToRead = sizeof(bool);
        fileStream.read(tempPtr, sizeToRead);

        /////////////////////////////////////////////
        //Add Structured Buffer Signature to Main List
        structuredBufferSigList.push_back(structuredBufferSig);
    }

    /////////////////////////////////////////////
    //Finish
    return AEResult::Ok;
}

AEResult Shader::ReadShaderTextureInputs(std::ifstream& fileStream, TextureInputSignatureList& texInputSigList)
{
    char* tempPtr = nullptr;
    uint32_t sizeToRead = 0;

    /////////////////////////////////////////////
    //Read Number of Shader Texture Inputs
    uint32_t numTexInputs = 0;
    tempPtr = reinterpret_cast<char*>(&numTexInputs);
    sizeToRead = sizeof(uint32_t);
    fileStream.read(tempPtr, sizeToRead);
    
    /////////////////////////////////////////////
    //Read Texture Inputs
    for (uint32_t i = 0; i < numTexInputs; i++)
    {
        TextureInputSignature texInputSig;

        /////////////////////////////////////////////
        //Read Name of Texture Input
        texInputSig.m_Name = AEGameContentHelpers::ReadString(fileStream);

        /////////////////////////////////////////////
        //Read Bind Index of Texture Input
        tempPtr = reinterpret_cast<char*>(&texInputSig.m_BindIndex);
        sizeToRead = sizeof(uint32_t);
        fileStream.read(tempPtr, sizeToRead);

        /////////////////////////////////////////////
        //Read Texture Input Type
        tempPtr = reinterpret_cast<char*>(&texInputSig.m_TextureType);
        sizeToRead = sizeof(TextureType);
        fileStream.read(tempPtr, sizeToRead);

        /////////////////////////////////////////////
        //Add Texture Input Signature to Main List
        texInputSigList.push_back(texInputSig);
    }

    /////////////////////////////////////////////
    //Finish
    return AEResult::Ok;
}

AEResult Shader::ReadShaderTextureArrayInputs(std::ifstream& fileStream, TextureArrayInputSignatureList& texArrayInputSigList)
{
    char* tempPtr = nullptr;
    uint32_t sizeToRead = 0;

    /////////////////////////////////////////////
    //Read Number of Shader Texture Inputs
    uint32_t numTexInputs = 0;
    tempPtr = reinterpret_cast<char*>(&numTexInputs);
    sizeToRead = sizeof(uint32_t);
    fileStream.read(tempPtr, sizeToRead);

    /////////////////////////////////////////////
    //Read Texture Inputs
    for (uint32_t i = 0; i < numTexInputs; i++)
    {
        TextureArrayInputSignature texArrayInputSig;

        /////////////////////////////////////////////
        //Read Name of Texture Input
        texArrayInputSig.m_Name = AEGameContentHelpers::ReadString(fileStream);

        /////////////////////////////////////////////
        //Read Bind Index of Texture Input
        tempPtr = reinterpret_cast<char*>(&texArrayInputSig.m_BindIndex);
        sizeToRead = sizeof(uint32_t);
        fileStream.read(tempPtr, sizeToRead);

        /////////////////////////////////////////////
        //Read Texture Input Type
        tempPtr = reinterpret_cast<char*>(&texArrayInputSig.m_TextureType);
        sizeToRead = sizeof(TextureType);
        fileStream.read(tempPtr, sizeToRead);

        /////////////////////////////////////////////
        //Add Texture Input Signature to Main List
        texArrayInputSigList.push_back(texArrayInputSig);
    }

    /////////////////////////////////////////////
    //Finish
    return AEResult::Ok;
}

AEResult Shader::ReadShaderSamplers(std::ifstream& fileStream, SamplerSignatureList& samplerSignatureList)
{
    char* tempPtr = nullptr;
    uint32_t sizeToRead = 0;

    /////////////////////////////////////////////
    //Read Number of Shader Samplers
    uint32_t numSamplers = 0;
    tempPtr = reinterpret_cast<char*>(&numSamplers);
    sizeToRead = sizeof(uint32_t);
    fileStream.read(tempPtr, sizeToRead);

    /////////////////////////////////////////////
    //Read Samplers
    for (uint32_t i = 0; i < numSamplers; i++)
    {
        SamplerSignature samplerSig;

        /////////////////////////////////////////////
        //Read Name of Sampler
        samplerSig.m_Name = AEGameContentHelpers::ReadString(fileStream);

        /////////////////////////////////////////////
        //Read Bind Index of Sampler
        tempPtr = reinterpret_cast<char*>(&samplerSig.m_BindIndex);
        sizeToRead = sizeof(uint32_t);
        fileStream.read(tempPtr, sizeToRead);

        /////////////////////////////////////////////
        //Add Sampler Signature to Main List
        samplerSignatureList.push_back(samplerSig);
    }

    /////////////////////////////////////////////
    //Finish
    return AEResult::Ok;
}
