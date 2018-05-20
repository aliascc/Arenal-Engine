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
#include "precomp_gamecontent.h"

/**********************
*   System Includes   *
***********************/

/*************************
*   3rd Party Includes   *
**************************/

/***************************
*   Game Engine Includes   *
****************************/
#include "GameContentDefs.h"
#include "Base\BaseFunctions.h"
#include "IncludeInterfaceHLSL.h"

//Always include last
#include "Memory\MemLeaks.h"

/************
*   Using   *
*************/

/********************
*   Function Defs   *
*********************/
AETODO("Using D3DCompiler, add Editor Flag");
IncludeInterfaceHLSL::IncludeInterfaceHLSL(const std::string& currentDirectory)
    : m_ShaderCurrentDir(currentDirectory + "\\")
{
}

HRESULT __stdcall IncludeInterfaceHLSL::Open(D3D_INCLUDE_TYPE IncludeType, LPCSTR pFileName, LPCVOID pParentData, LPCVOID *ppData, UINT *pBytes)
{    
    std::string fileName = "";

    if(IncludeType == D3D_INCLUDE_SYSTEM)
    {
        fileName = AE_SHADER_SYS_INC_PATH + std::string(pFileName);
    }
    else
    {
        fileName = m_ShaderCurrentDir + pFileName;
    }

    //////////////////////////////////////////////
    //Load File to Memory
    std::ifstream shaderFile(fileName, std::ios::in | std::ios::binary);

    if(!shaderFile.is_open())
    {
        return E_FAIL;
    }
    
    //////////////////////////////////////////////
    //Get Size of Shader
    shaderFile.seekg(0, std::ios::end);
    uint32_t fileSize = static_cast<uint32_t>(shaderFile.tellg());
    shaderFile.seekg(0, std::ios::beg);
    
    //////////////////////////////////////////////
    //Get Byte Code
    char* shaderByteCode = new char[fileSize];
    ZeroMemory(shaderByteCode, sizeof(char) * fileSize);
    shaderFile.read((char*)shaderByteCode, sizeof(char) * fileSize);
    
    //////////////////////////////////////////////
    //Close File
    shaderFile.close();

    *ppData = shaderByteCode;
    *pBytes = sizeof(char) * fileSize;

    return S_OK;
}

HRESULT __stdcall IncludeInterfaceHLSL::Close(LPCVOID pData)
{
    char* da = (char*)pData;
    
    DeleteMemArr(da);

    return S_OK;
}
