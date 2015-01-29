
/********************************************************
*
* Author: Carlos Chacón N.
*
* Created: 2/1/2013
*
* Desc:
*
*
* Copyright 2013
*
*********************************************************/

#pragma once

#ifndef _WRITER_XE3D_H
#define _WRITER_XE3D_H

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

/************
*   Using   *
*************/

/********************
*   Forward Decls   *
*********************/
struct ModelContent;
struct MeshContent;
struct SkeletonContent;
struct AnimationContent;

/*****************
*   Class Decl   *
******************/
class WriterXE3D sealed : public XEObject
{
	private:
		std::wstring m_FileName = L"";

		std::wstring m_FilePath = L"";

		std::wstring m_FullFileName = L"";

		XEResult WriteMesh(const std::wstring& fileName, MeshContent* mesh);

		XEResult WriteSkeleton(const std::wstring& fileName, SkeletonContent* skeleton);

		XEResult WriteAnimation(const std::wstring& fileName, AnimationContent* animation);

		XEResult WriteModelBody();

	public:
		WriterXE3D();
		virtual ~WriterXE3D();

		const std::wstring& GetOutputFilePath() const 
		{
			return m_FullFileName;
		}

		//Framework
		XEResult WriteToFile(ModelContent* content, const std::wstring& fileName, const std::wstring& filePath);
};

#endif