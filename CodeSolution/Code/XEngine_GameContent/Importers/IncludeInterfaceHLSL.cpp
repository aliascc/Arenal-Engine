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
XETODO("Using D3DCompiler, add Editor Flag");
IncludeInterfaceHLSL::IncludeInterfaceHLSL(const std::wstring& currentDirectory)
	: m_ShaderCurrentDir(currentDirectory + L"\\")
{
}

HRESULT __stdcall IncludeInterfaceHLSL::Open(D3D_INCLUDE_TYPE IncludeType, LPCSTR pFileName, LPCVOID pParentData, LPCVOID *ppData, UINT *pBytes)
{	
	std::wstring fileName = L"";

	if(IncludeType == D3D_INCLUDE_SYSTEM)
	{
		fileName = XE_SHADER_SYS_INC_PATH + XE_Base::String2WideStr(pFileName);;
	}
	else
	{
		fileName = m_ShaderCurrentDir + XE_Base::String2WideStr(pFileName);
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