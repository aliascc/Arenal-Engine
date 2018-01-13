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
#include "boost\filesystem.hpp"

/***************************
*   Game Engine Includes   *
****************************/
#include "WriterHLSL.h"
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
WriterHLSL::WriterHLSL()
{
}

WriterHLSL::~WriterHLSL()
{
}

AEResult WriterHLSL::GetExtensionForShader(ShaderType type, std::wstring& extension)
{
	switch (type)
	{
		case ShaderType::VertexShader:
			extension = AE_CT_SHADER_HLSL_VS_EXT;
			break;
		case ShaderType::PixelShader:
			extension = AE_CT_SHADER_HLSL_PS_EXT;
			break;
		case ShaderType::GeometryShader:
			extension = AE_CT_SHADER_HLSL_GS_EXT;
			break;
		case ShaderType::HullShader:
			extension = AE_CT_SHADER_HLSL_HS_EXT;
			break;
		case ShaderType::ComputeShader:
			extension = AE_CT_SHADER_HLSL_CS_EXT;
			break;
		case ShaderType::DomainShader:
			extension = AE_CT_SHADER_HLSL_DS_EXT;
			break;
		default:
			AEAssert(false);
			extension = L"";
			return AEResult::InvalidShaderType;
	}

	return AEResult::Ok;
}

AEResult WriterHLSL::WriteToFile(ShaderContent* content, const std::wstring& fileName, const std::wstring& filePath)
{
	m_FileName = L"";
	m_FullFileName = L"";

	AEAssert(content != nullptr);

	if(content == nullptr)
	{
		return AEResult::NullObj;
	}

	AEAssert(!fileName.empty());

	if(fileName.empty())
	{
		return AEResult::EmptyFilename;
	}

	AEAssert(!filePath.empty());

	if(filePath.empty())
	{
		return AEResult::EmptyString;
	}
	
	m_FilePath = filePath;
		
	if(m_FilePath.back() != L'\\' && m_FilePath.back() != L'/')
	{
		m_FilePath.push_back(L'\\');
	}

	//Get Extension for File
	std::wstring extension = L"";

	if(GetExtensionForShader(content->m_ShaderType, extension) != AEResult::Ok)
	{
		return AEResult::InvalidShaderType;
	}

	//Set File Path and File Name
	m_FileName = fileName + L"." + extension;
	m_FullFileName = m_FilePath + m_FileName;

	std::ofstream shaderFile;

	shaderFile.open(m_FullFileName, std::ios::binary | std::ios::out | std::ios::trunc);

	if(!shaderFile.is_open())
	{
		AETODO("add log");
		return AEResult::OpenFileFail;
	}

	AEResult ret = AEResult::Ok;

	const char* tempPtr = nullptr;
	uint32_t sizeToWrite = 0;
	
	/////////////////////////////////////////////
	//Start by writing the header and File Version
	AEGameContentHelpers::WriteFileHeader(shaderFile, AE_CT_AEHFX_FILE_HEADER, AE_CT_AEHFX_FILE_VERSION_MAYOR, AE_CT_AEHFX_FILE_VERSION_MINOR, AE_CT_AEHFX_FILE_VERSION_REVISON);
	
	/////////////////////////////////////////////
	//Write Shader Type
	tempPtr = reinterpret_cast<const char*>(&content->m_ShaderType);
	sizeToWrite = sizeof(ShaderType);
	shaderFile.write(tempPtr, sizeToWrite);
	
	/////////////////////////////////////////////
	//Write Name of Shader and size of Name
	AEGameContentHelpers::WriteString(shaderFile, content->m_Name);

	/////////////////////////////////////////////
	//Write Shader Byte Code
	ret = WriteShaderByteCode(shaderFile, content);
	if(ret != AEResult::Ok)
	{
		AETODO("Log error");

		return ret;
	}
	
	/////////////////////////////////////////////
	//Write Shader Constant Buffers
	ret = WriteShaderConstantBuffers(shaderFile, content);
	if(ret != AEResult::Ok)
	{
		AETODO("Log error");

		return ret;
	}

	/////////////////////////////////////////////
	//Write Simple Buffers
	ret = WriteShaderSimpleBuffers(shaderFile, content);

	if(ret != AEResult::Ok)
	{
		AETODO("Log error");

		return ret;
	}

	/////////////////////////////////////////////
	//Write Structured Buffers
	ret = WriteShaderStructuredBuffers(shaderFile, content);
	if(ret != AEResult::Ok)
	{
		AETODO("Log error");

		return ret;
	}

	/////////////////////////////////////////////
	//Write Shader Texture Inputs
	ret = WriteShaderTextureInputs(shaderFile, content);
	if(ret != AEResult::Ok)
	{
		AETODO("Log error");

		return ret;
	}

	/////////////////////////////////////////////
	//Write Shader Texture Array Inputs
	ret = WriteShaderTextureArraysInputs(shaderFile, content);
	if (ret != AEResult::Ok)
	{
		AETODO("Log error");

		return ret;
	}

	/////////////////////////////////////////////
	//Write Shader Samplers
	ret = WriteShaderSamplers(shaderFile, content);
	if (ret != AEResult::Ok)
	{
		AETODO("Log error");

		return ret;
	}

	/////////////////////////////////////////////
	//Finish by writing the footer
	AEGameContentHelpers::WriteFileFooter(shaderFile, AE_CT_AEHFX_FILE_FOOTER);
	
	/////////////////////////////////////////////
	//Finish 
	shaderFile.flush();
	shaderFile.close();

	return AEResult::Ok;
}

AEResult WriterHLSL::WriteShaderByteCode(std::ofstream& fileStream, ShaderContent* content)
{
	AEAssert(content != nullptr);

	if(content == nullptr)
	{
		return AEResult::NullParameter;
	}

	const char* tempPtr = nullptr;
	uint32_t sizeToWrite = 0;

	/////////////////////////////////////////////
	//Write Shader Byte Code Size and Memory
	tempPtr = reinterpret_cast<const char*>(&content->m_ByteCodeSize);
	sizeToWrite = sizeof(uint32_t);
	fileStream.write(tempPtr, sizeToWrite);

	tempPtr = reinterpret_cast<const char*>(content->m_ShaderByteCode);
	sizeToWrite = content->m_ByteCodeSize;
	fileStream.write(tempPtr, sizeToWrite);
	
	/////////////////////////////////////////////
	//Finish
	return AEResult::Ok;
}

AEResult WriterHLSL::WriteShaderConstantBuffers(std::ofstream& fileStream, ShaderContent* content)
{
	AEAssert(content != nullptr);
	if(content == nullptr)
	{
		return AEResult::NullParameter;
	}

	const char* tempPtr = nullptr;
	uint32_t tempUInt32 = 0;
	uint32_t sizeToWrite = 0;

	/////////////////////////////////////////////
	//Write Number of Shader Constant Buffers
	tempUInt32 = (uint32_t)content->m_ConstantBufferShaderHolderList.size();
	tempPtr = reinterpret_cast<const char*>(&tempUInt32);
	sizeToWrite = sizeof(uint32_t);
	fileStream.write(tempPtr, sizeToWrite);
	
	/////////////////////////////////////////////
	//Write Constant Buffers
	for (const ConstantBufferShaderHolder& cbHolder : content->m_ConstantBufferShaderHolderList)
	{
		/////////////////////////////////////////////
		//Write Name of Constant Buffer
		AEGameContentHelpers::WriteString(fileStream, cbHolder.m_Name);
		
		/////////////////////////////////////////////
		//Write Bind Index of Constant Buffer
		tempPtr = reinterpret_cast<const char*>(&cbHolder.m_BindIndex);
		sizeToWrite = sizeof(uint32_t);
		fileStream.write(tempPtr, sizeToWrite);

		/////////////////////////////////////////////
		//Write Number of Variables in Constant Buffer
		tempUInt32 = (uint32_t)cbHolder.m_ShaderCustomVariableList.size();
		tempPtr = reinterpret_cast<const char*>(&tempUInt32);
		sizeToWrite = sizeof(uint32_t);
		fileStream.write(tempPtr, sizeToWrite);
	
		/////////////////////////////////////////////
		//Write Variables of Constant Buffer
		for (const ShaderCustomVariable& scVar : cbHolder.m_ShaderCustomVariableList)
		{
			/////////////////////////////////////////////
			//Write Variable Info

			//1. Name
			AEGameContentHelpers::WriteString(fileStream, scVar.m_Name);
			
			//2. Start Offset
			tempPtr = reinterpret_cast<const char*>(&scVar.m_StartOffset);
			sizeToWrite = sizeof(uint32_t);
			fileStream.write(tempPtr, sizeToWrite);

			//3. Offset		
			tempPtr = reinterpret_cast<const char*>(&scVar.m_Offset);
			sizeToWrite = sizeof(uint32_t);
			fileStream.write(tempPtr, sizeToWrite);

			//4. Size
			tempPtr = reinterpret_cast<const char*>(&scVar.m_Size);
			sizeToWrite = sizeof(uint32_t);
			fileStream.write(tempPtr, sizeToWrite);

			//5. Elements
			tempPtr = reinterpret_cast<const char*>(&scVar.m_Elements);
			sizeToWrite = sizeof(uint32_t);
			fileStream.write(tempPtr, sizeToWrite);
			
			//6. Is Array
			tempPtr = reinterpret_cast<const char*>(&scVar.m_IsArray);
			sizeToWrite = sizeof(bool);
			fileStream.write(tempPtr, sizeToWrite);

			//7. Element Size
			tempPtr = reinterpret_cast<const char*>(&scVar.m_ElementSize);
			sizeToWrite = sizeof(uint32_t);
			fileStream.write(tempPtr, sizeToWrite);
			
			//8. User Variable
			tempPtr = reinterpret_cast<const char*>(&scVar.m_UserVariable);
			sizeToWrite = sizeof(bool);
			fileStream.write(tempPtr, sizeToWrite);

			//9. Columns
			tempPtr = reinterpret_cast<const char*>(&scVar.m_Columns);
			sizeToWrite = sizeof(uint32_t);
			fileStream.write(tempPtr, sizeToWrite);
			
			//10. Rows
			tempPtr = reinterpret_cast<const char*>(&scVar.m_Rows);
			sizeToWrite = sizeof(uint32_t);
			fileStream.write(tempPtr, sizeToWrite);
			
			//11. Shader Class
			tempPtr = reinterpret_cast<const char*>(&scVar.m_ShaderVariableClass);
			sizeToWrite = sizeof(ShaderVariableClass);
			fileStream.write(tempPtr, sizeToWrite);
			
			//12. Shader Type
			tempPtr = reinterpret_cast<const char*>(&scVar.m_ShaderVariableType);
			sizeToWrite = sizeof(ShaderVariableType);
			fileStream.write(tempPtr, sizeToWrite);
		}
	}
	
	/////////////////////////////////////////////
	//Finish
	return AEResult::Ok;
}

AEResult WriterHLSL::WriteShaderStructuredBuffers(std::ofstream& fileStream, ShaderContent* content)
{
	AEAssert(content != nullptr);
	if(content == nullptr)
	{
		return AEResult::NullParameter;
	}

	const char* tempPtr = nullptr;
	uint32_t tempUInt32 = 0;
	uint32_t sizeToWrite = 0;

	/////////////////////////////////////////////
	//Write Number of Shader Structured Buffers
	tempUInt32 = (uint32_t)content->m_StructuredBufferShaderHolderList.size();
	tempPtr = reinterpret_cast<const char*>(&tempUInt32);
	sizeToWrite = sizeof(uint32_t);
	fileStream.write(tempPtr, sizeToWrite);
	
	/////////////////////////////////////////////
	//Write Structured Buffers
	for (const StructuredBufferShaderHolder& structuredBuffersHolder : content->m_StructuredBufferShaderHolderList)
	{
		/////////////////////////////////////////////
		//Write Name of Structured Buffer
		AEGameContentHelpers::WriteString(fileStream, structuredBuffersHolder.m_Name);
		
		/////////////////////////////////////////////
		//Write Bind Index of Structured Buffer
		tempPtr = reinterpret_cast<const char*>(&structuredBuffersHolder.m_BindIndex);
		sizeToWrite = sizeof(uint32_t);
		fileStream.write(tempPtr, sizeToWrite);
		
		/////////////////////////////////////////////
		//Write RW Flag of Structured Buffer
		tempPtr = reinterpret_cast<const char*>(&structuredBuffersHolder.m_IsRW);
		sizeToWrite = sizeof(bool);
		fileStream.write(tempPtr, sizeToWrite);
	}

	/////////////////////////////////////////////
	//Finish
	return AEResult::Ok;
}

AEResult WriterHLSL::WriteShaderSimpleBuffers(std::ofstream& fileStream, ShaderContent* content)
{
	AEAssert(content != nullptr);
	if(content == nullptr)
	{
		return AEResult::NullParameter;
	}

	const char* tempPtr = nullptr;
	uint32_t tempUInt32 = 0;
	uint32_t sizeToWrite = 0;

	/////////////////////////////////////////////
	//Write Number of Shader Simple Buffers
	tempUInt32 = (uint32_t)content->m_SimpleBufferShaderHolderList.size();
	tempPtr = reinterpret_cast<const char*>(&tempUInt32);
	sizeToWrite = sizeof(uint32_t);
	fileStream.write(tempPtr, sizeToWrite);
	
	/////////////////////////////////////////////
	//Write Simple Buffers
	for (const SimpleBufferShaderHolder& simpleBuffersHolder : content->m_SimpleBufferShaderHolderList)
	{
		/////////////////////////////////////////////
		//Write Name of Simple Buffer
		AEGameContentHelpers::WriteString(fileStream, simpleBuffersHolder.m_Name);
		
		/////////////////////////////////////////////
		//Write Bind Index of Simple Buffer
		tempPtr = reinterpret_cast<const char*>(&simpleBuffersHolder.m_BindIndex);
		sizeToWrite = sizeof(uint32_t);
		fileStream.write(tempPtr, sizeToWrite);
		
		/////////////////////////////////////////////
		//Write RW Flag of Simple Buffer
		tempPtr = reinterpret_cast<const char*>(&simpleBuffersHolder.m_IsRW);
		sizeToWrite = sizeof(bool);
		fileStream.write(tempPtr, sizeToWrite);
		
		/////////////////////////////////////////////
		//Write Element Count
		tempPtr = reinterpret_cast<const char*>(&simpleBuffersHolder.m_ElementCount);
		sizeToWrite = sizeof(uint32_t);
		fileStream.write(tempPtr, sizeToWrite);
		
		/////////////////////////////////////////////
		//Write Shader Variable Class
		tempPtr = reinterpret_cast<const char*>(&simpleBuffersHolder.m_VariableClass);
		sizeToWrite = sizeof(ShaderVariableClass);
		fileStream.write(tempPtr, sizeToWrite);
		
		/////////////////////////////////////////////
		//Write Shader Variable Type
		tempPtr = reinterpret_cast<const char*>(&simpleBuffersHolder.m_VariableType);
		sizeToWrite = sizeof(ShaderVariableType);
		fileStream.write(tempPtr, sizeToWrite);
	}

	/////////////////////////////////////////////
	//Finish
	return AEResult::Ok;
}

AEResult WriterHLSL::WriteShaderTextureInputs(std::ofstream& fileStream, ShaderContent* content)
{
	AEAssert(content != nullptr);
	if(content == nullptr)
	{
		return AEResult::NullParameter;
	}

	const char* tempPtr = nullptr;
	uint32_t tempUInt32 = 0;
	uint32_t sizeToWrite = 0;

	/////////////////////////////////////////////
	//Write Number of Shader Texture Inputs
	tempUInt32 = (uint32_t)content->m_TextureShaderVariableHolderList.size();
	tempPtr = reinterpret_cast<const char*>(&tempUInt32);
	sizeToWrite = sizeof(uint32_t);
	fileStream.write(tempPtr, sizeToWrite);
	
	/////////////////////////////////////////////
	//Write Texture Inputs
	for (const TextureShaderVariableHolder& tsHolder : content->m_TextureShaderVariableHolderList)
	{
		/////////////////////////////////////////////
		//Write Name of Texture Input
		AEGameContentHelpers::WriteString(fileStream, tsHolder.m_Name);
		
		/////////////////////////////////////////////
		//Write Bind Index of Texture Input
		tempPtr = reinterpret_cast<const char*>(&tsHolder.m_BindIndex);
		sizeToWrite = sizeof(uint32_t);
		fileStream.write(tempPtr, sizeToWrite);

		/////////////////////////////////////////////
		//Write Texture Input Type
		tempPtr = reinterpret_cast<const char*>(&tsHolder.m_TextureType);
		sizeToWrite = sizeof(TextureType);
		fileStream.write(tempPtr, sizeToWrite);
	}

	/////////////////////////////////////////////
	//Finish
	return AEResult::Ok;
}

AEResult WriterHLSL::WriteShaderTextureArraysInputs(std::ofstream& fileStream, ShaderContent* content)
{
	AEAssert(content != nullptr);
	if (content == nullptr)
	{
		return AEResult::NullParameter;
	}

	const char* tempPtr = nullptr;
	uint32_t tempUInt32 = 0;
	uint32_t sizeToWrite = 0;

	/////////////////////////////////////////////
	//Write Number of Shader Texture Inputs
	tempUInt32 = (uint32_t)content->m_TextureArrayShaderVariableHolderList.size();
	tempPtr = reinterpret_cast<const char*>(&tempUInt32);
	sizeToWrite = sizeof(uint32_t);
	fileStream.write(tempPtr, sizeToWrite);

	/////////////////////////////////////////////
	//Write Texture Inputs
	for (const TextureArrayShaderVariableHolder& tsArrayHolder : content->m_TextureArrayShaderVariableHolderList)
	{
		/////////////////////////////////////////////
		//Write Name of Texture Array Input
		AEGameContentHelpers::WriteString(fileStream, tsArrayHolder.m_Name);

		/////////////////////////////////////////////
		//Write Bind Index of Texture Array Input
		tempPtr = reinterpret_cast<const char*>(&tsArrayHolder.m_BindIndex);
		sizeToWrite = sizeof(uint32_t);
		fileStream.write(tempPtr, sizeToWrite);

		/////////////////////////////////////////////
		//Write Texture Array Input Type
		tempPtr = reinterpret_cast<const char*>(&tsArrayHolder.m_TextureType);
		sizeToWrite = sizeof(TextureType);
		fileStream.write(tempPtr, sizeToWrite);
	}

	/////////////////////////////////////////////
	//Finish
	return AEResult::Ok;
}

AEResult WriterHLSL::WriteShaderSamplers(std::ofstream& fileStream, ShaderContent* content)
{
	AEAssert(content != nullptr);
	if (content == nullptr)
	{
		return AEResult::NullParameter;
	}

	const char* tempPtr = nullptr;
	uint32_t tempUInt32 = 0;
	uint32_t sizeToWrite = 0;

	/////////////////////////////////////////////
	//Write Number of Samplers
	tempUInt32 = (uint32_t)content->m_SamplerShaderHolderList.size();
	tempPtr = reinterpret_cast<const char*>(&tempUInt32);
	sizeToWrite = sizeof(uint32_t);
	fileStream.write(tempPtr, sizeToWrite);

	/////////////////////////////////////////////
	//Write Samplers
	for (const SamplerShaderHolder& samplerHolder : content->m_SamplerShaderHolderList)
	{
		/////////////////////////////////////////////
		//Write Name of Sampler
		AEGameContentHelpers::WriteString(fileStream, samplerHolder.m_Name);

		/////////////////////////////////////////////
		//Write Bind Index of Sampler
		tempPtr = reinterpret_cast<const char*>(&samplerHolder.m_BindIndex);
		sizeToWrite = sizeof(uint32_t);
		fileStream.write(tempPtr, sizeToWrite);
	}

	/////////////////////////////////////////////
	//Finish
	return AEResult::Ok;
}
