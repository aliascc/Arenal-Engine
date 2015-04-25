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

XEResult WriterHLSL::GetExtensionForShader(ShaderType type, std::wstring& extension)
{
	switch (type)
	{
		case ShaderType::VertexShader:
			extension = XE_CT_SHADER_HLSL_VS_EXT;
			break;
		case ShaderType::PixelShader:
			extension = XE_CT_SHADER_HLSL_PS_EXT;
			break;
		case ShaderType::GeometryShader:
			extension = XE_CT_SHADER_HLSL_GS_EXT;
			break;
		case ShaderType::HullShader:
			extension = XE_CT_SHADER_HLSL_HS_EXT;
			break;
		case ShaderType::ComputeShader:
			extension = XE_CT_SHADER_HLSL_CS_EXT;
			break;
		case ShaderType::DomainShader:
			extension = XE_CT_SHADER_HLSL_DS_EXT;
			break;
		default:
			XEAssert(false);
			extension = L"";
			return XEResult::InvalidShaderType;
	}

	return XEResult::Ok;
}

XEResult WriterHLSL::WriteToFile(ShaderContent* content, const std::wstring& fileName, const std::wstring& filePath)
{
	m_FileName = L"";
	m_FullFileName = L"";

	XEAssert(content != nullptr);

	if(content == nullptr)
	{
		return XEResult::NullObj;
	}

	XEAssert(!fileName.empty());

	if(fileName.empty())
	{
		return XEResult::EmptyFilename;
	}

	XEAssert(!filePath.empty());

	if(filePath.empty())
	{
		return XEResult::EmptyString;
	}
	
	m_FilePath = filePath;
		
	if(m_FilePath.back() != L'\\' && m_FilePath.back() != L'/')
	{
		m_FilePath.push_back(L'\\');
	}

	//Get Extension for File
	std::wstring extension = L"";

	if(GetExtensionForShader(content->m_ShaderType, extension) != XEResult::Ok)
	{
		return XEResult::InvalidShaderType;
	}

	//Set File Path and File Name
	m_FileName = fileName + L"." + extension;
	m_FullFileName = m_FilePath + m_FileName;

	std::ofstream shaderFile;

	shaderFile.open(m_FullFileName, std::ios::binary | std::ios::out | std::ios::trunc);

	if(!shaderFile.is_open())
	{
		XETODO("add log");
		return XEResult::OpenFileFail;
	}

	XEResult ret = XEResult::Ok;

	const char* tempPtr = nullptr;
	uint32_t sizeToWrite = 0;
	
	/////////////////////////////////////////////
	//Start by writing the header and File Version
	XEGameContentHelpers::WriteFileHeader(shaderFile, XE_CT_XEHFX_FILE_HEADER, XE_CT_XEHFX_FILE_VERSION_MAYOR, XE_CT_XEHFX_FILE_VERSION_MINOR, XE_CT_XEHFX_FILE_VERSION_REVISON);
	
	/////////////////////////////////////////////
	//Write Shader Type
	tempPtr = reinterpret_cast<const char*>(&content->m_ShaderType);
	sizeToWrite = sizeof(ShaderType);
	shaderFile.write(tempPtr, sizeToWrite);
	
	/////////////////////////////////////////////
	//Write Name of Shader and size of Name
	XEGameContentHelpers::WriteString(shaderFile, content->m_Name);

	/////////////////////////////////////////////
	//Write Shader Byte Code
	ret = WriteShaderByteCode(shaderFile, content);
	if(ret != XEResult::Ok)
	{
		XETODO("Log error");

		return ret;
	}
	
	/////////////////////////////////////////////
	//Write Shader Constant Buffers
	ret = WriteShaderConstantBuffers(shaderFile, content);
	if(ret != XEResult::Ok)
	{
		XETODO("Log error");

		return ret;
	}

	/////////////////////////////////////////////
	//Write Simple Buffers
	ret = WriteShaderSimpleBuffers(shaderFile, content);

	if(ret != XEResult::Ok)
	{
		XETODO("Log error");

		return ret;
	}

	/////////////////////////////////////////////
	//Write Structured Buffers
	ret = WriteShaderStructuredBuffers(shaderFile, content);
	if(ret != XEResult::Ok)
	{
		XETODO("Log error");

		return ret;
	}

	/////////////////////////////////////////////
	//Write Shader Texture Inputs
	ret = WriteShaderTextureInputs(shaderFile, content);
	if(ret != XEResult::Ok)
	{
		XETODO("Log error");

		return ret;
	}

	/////////////////////////////////////////////
	//Write Shader Texture Array Inputs
	ret = WriteShaderTextureArraysInputs(shaderFile, content);
	if (ret != XEResult::Ok)
	{
		XETODO("Log error");

		return ret;
	}

	/////////////////////////////////////////////
	//Write Shader Samplers
	ret = WriteShaderSamplers(shaderFile, content);
	if (ret != XEResult::Ok)
	{
		XETODO("Log error");

		return ret;
	}

	/////////////////////////////////////////////
	//Finish by writing the footer
	XEGameContentHelpers::WriteFileFooter(shaderFile, XE_CT_XEHFX_FILE_FOOTER);
	
	/////////////////////////////////////////////
	//Finish 
	shaderFile.flush();
	shaderFile.close();

	return XEResult::Ok;
}

XEResult WriterHLSL::WriteShaderByteCode(std::ofstream& fileStream, ShaderContent* content)
{
	XEAssert(content != nullptr);

	if(content == nullptr)
	{
		return XEResult::NullParameter;
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
	return XEResult::Ok;
}

XEResult WriterHLSL::WriteShaderConstantBuffers(std::ofstream& fileStream, ShaderContent* content)
{
	XEAssert(content != nullptr);
	if(content == nullptr)
	{
		return XEResult::NullParameter;
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
		XEGameContentHelpers::WriteString(fileStream, cbHolder.m_Name);
		
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
			XEGameContentHelpers::WriteString(fileStream, scVar.m_Name);
			
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
	return XEResult::Ok;
}

XEResult WriterHLSL::WriteShaderStructuredBuffers(std::ofstream& fileStream, ShaderContent* content)
{
	XEAssert(content != nullptr);
	if(content == nullptr)
	{
		return XEResult::NullParameter;
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
		XEGameContentHelpers::WriteString(fileStream, structuredBuffersHolder.m_Name);
		
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
	return XEResult::Ok;
}

XEResult WriterHLSL::WriteShaderSimpleBuffers(std::ofstream& fileStream, ShaderContent* content)
{
	XEAssert(content != nullptr);
	if(content == nullptr)
	{
		return XEResult::NullParameter;
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
		XEGameContentHelpers::WriteString(fileStream, simpleBuffersHolder.m_Name);
		
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
	return XEResult::Ok;
}

XEResult WriterHLSL::WriteShaderTextureInputs(std::ofstream& fileStream, ShaderContent* content)
{
	XEAssert(content != nullptr);
	if(content == nullptr)
	{
		return XEResult::NullParameter;
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
		XEGameContentHelpers::WriteString(fileStream, tsHolder.m_Name);
		
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
	return XEResult::Ok;
}

XEResult WriterHLSL::WriteShaderTextureArraysInputs(std::ofstream& fileStream, ShaderContent* content)
{
	XEAssert(content != nullptr);
	if (content == nullptr)
	{
		return XEResult::NullParameter;
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
		XEGameContentHelpers::WriteString(fileStream, tsArrayHolder.m_Name);

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
	return XEResult::Ok;
}

XEResult WriterHLSL::WriteShaderSamplers(std::ofstream& fileStream, ShaderContent* content)
{
	XEAssert(content != nullptr);
	if (content == nullptr)
	{
		return XEResult::NullParameter;
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
		XEGameContentHelpers::WriteString(fileStream, samplerHolder.m_Name);

		/////////////////////////////////////////////
		//Write Bind Index of Sampler
		tempPtr = reinterpret_cast<const char*>(&samplerHolder.m_BindIndex);
		sizeToWrite = sizeof(uint32_t);
		fileStream.write(tempPtr, sizeToWrite);
	}

	/////////////////////////////////////////////
	//Finish
	return XEResult::Ok;
}
