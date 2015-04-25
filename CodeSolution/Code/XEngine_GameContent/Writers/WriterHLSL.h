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
#include "Base\XEObject.h"
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
class WriterHLSL sealed : public XEObject
{
	private:
		std::wstring m_FileName = L"";

		std::wstring m_FilePath = L"";

		std::wstring m_FullFileName = L"";

		XEResult GetExtensionForShader(ShaderType type, std::wstring& extension);

		XEResult WriteShaderByteCode(std::ofstream& fileStream, ShaderContent* content);

		XEResult WriteShaderConstantBuffers(std::ofstream& fileStream, ShaderContent* content);

		XEResult WriteShaderStructuredBuffers(std::ofstream& fileStream, ShaderContent* content);

		XEResult WriteShaderSimpleBuffers(std::ofstream& fileStream, ShaderContent* content);

		XEResult WriteShaderTextureInputs(std::ofstream& fileStream, ShaderContent* content);

		XEResult WriteShaderTextureArraysInputs(std::ofstream& fileStream, ShaderContent* content);

		XEResult WriteShaderSamplers(std::ofstream& fileStream, ShaderContent* content);

	public:
		WriterHLSL();
		virtual ~WriterHLSL();

		const std::wstring& GetOutputFilePath() const 
		{
			return m_FullFileName;
		}

		//Framework
		XEResult WriteToFile(ShaderContent* content, const std::wstring& fileName, const std::wstring& filePath);
};

#endif