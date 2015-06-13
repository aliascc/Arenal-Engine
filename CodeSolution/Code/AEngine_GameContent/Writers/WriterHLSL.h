/********************************************************
*
* Author: Carlos Chacón N.
*
* Copyright 2012-2015
*
*********************************************************/

#pragma once

#ifndef _WRITER_HLSL_H
#define _WRITER_HLSL_H

/**********************
*   System Includes   *
***********************/
#include <string>
#include <fstream>

/*************************
*   3rd Party Includes   *
**************************/

/***************************
*   Game Engine Includes   *
****************************/
#include "Base\Base.h"
#include "Base\AEObject.h"
#include "Content\ShaderContent.h"

/************
*   Using   *
*************/

/********************
*   Forward Decls   *
*********************/
struct ShaderContent;

/*****************
*   Class Decl   *
******************/
class WriterHLSL sealed : public AEObject
{
	private:
		std::wstring m_FileName = L"";

		std::wstring m_FilePath = L"";

		std::wstring m_FullFileName = L"";

		AEResult GetExtensionForShader(ShaderType type, std::wstring& extension);

		AEResult WriteShaderByteCode(std::ofstream& fileStream, ShaderContent* content);

		AEResult WriteShaderConstantBuffers(std::ofstream& fileStream, ShaderContent* content);

		AEResult WriteShaderStructuredBuffers(std::ofstream& fileStream, ShaderContent* content);

		AEResult WriteShaderSimpleBuffers(std::ofstream& fileStream, ShaderContent* content);

		AEResult WriteShaderTextureInputs(std::ofstream& fileStream, ShaderContent* content);

		AEResult WriteShaderTextureArraysInputs(std::ofstream& fileStream, ShaderContent* content);

		AEResult WriteShaderSamplers(std::ofstream& fileStream, ShaderContent* content);

	public:
		WriterHLSL();
		virtual ~WriterHLSL();

		const std::wstring& GetOutputFilePath() const 
		{
			return m_FullFileName;
		}

		//Framework
		AEResult WriteToFile(ShaderContent* content, const std::wstring& fileName, const std::wstring& filePath);
};

#endif